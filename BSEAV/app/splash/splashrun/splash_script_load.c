/******************************************************************************
 *  Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.
 ******************************************************************************/

#include "splash_script_load.h"

#include "splash_magnum.h"

#include "bchp_rdc.h"
#include "bchp_common.h"
/* #include "lib_printf.h" */

#include "splash_script_load.h"

BDBG_MODULE(splash_script_load);

#define BRDC_REGISTER(reg)      ((reg) + BCHP_PHYSICAL_OFFSET)

#define IS_PROGRESSIVE(fmt) (\
	(fmt == BFMT_VideoFmt_e480p)               || \
	(fmt == BFMT_VideoFmt_e576p_50Hz)          || \
	(fmt == BFMT_VideoFmt_e720p)               || \
	(fmt == BFMT_VideoFmt_e720p_24Hz)          || \
	(fmt == BFMT_VideoFmt_e720p_25Hz)          || \
	(fmt == BFMT_VideoFmt_e720p_30Hz)          || \
	(fmt == BFMT_VideoFmt_e720p_50Hz)          || \
	(fmt == BFMT_VideoFmt_e1080p)              || \
	(fmt == BFMT_VideoFmt_e1080p_24Hz)         || \
	(fmt == BFMT_VideoFmt_e1080p_25Hz)         || \
	(fmt == BFMT_VideoFmt_e1080p_30Hz)         || \
	(fmt == BFMT_VideoFmt_e1080p_50Hz)         || \
	(fmt == BFMT_VideoFmt_e720p_60Hz_3DOU_AS)  || \
	(fmt == BFMT_VideoFmt_e720p_50Hz_3DOU_AS)  || \
	(fmt == BFMT_VideoFmt_e1080p_24Hz_3DOU_AS) || \
	(fmt == BFMT_VideoFmt_e1080p_30Hz_3DOU_AS) || \
	(fmt == BFMT_VideoFmt_eCUSTOM_1366x768p)   || \
	(fmt == BFMT_VideoFmt_eDVI_1600x1200p_60Hz)|| \
	(fmt == BFMT_VideoFmt_eCustom2)               \
)

/* Remember highest slot number ever used */
static int s_highSlot = -1;
/* A little package for remembering pointers */
#define REMEMBER_CHUNK 256
static unsigned int s_numPointers = 0;
static unsigned int s_maxPointers = 0;
static BMMA_Block_Handle* s_Pointers = NULL;
static void RememberPointer (BMMA_Block_Handle pointer)
{
	if (s_numPointers == s_maxPointers)
	{
		unsigned int new_length = s_maxPointers + REMEMBER_CHUNK;
		BMMA_Block_Handle* new_array = (BMMA_Block_Handle*)BKNI_Malloc (
                    (new_length) * sizeof (BMMA_Block_Handle*));
		BDBG_ASSERT(new_array);
		BKNI_Memcpy (
			(void*)new_array, (void*)s_Pointers,
			sizeof(BMMA_Block_Handle) * s_maxPointers);
		s_maxPointers = new_length;
		s_Pointers = new_array;
	}
	s_Pointers[s_numPointers++] = pointer;
}
static void FreePointers (BMMA_Heap_Handle hHeap)
{
	unsigned int index;
	for (index = 0 ; index < s_numPointers ; ++index)
		BMMA_Free ((void*)s_Pointers[index]);
	BKNI_Free ((void*)s_Pointers);
	s_Pointers = NULL;
	s_numPointers = 0;
	s_maxPointers = 0;
}

/**************************
 * This function takes a set of RULs for a given slot (say all the RULS for top
 * field or all for bottom field), copies them into memory, adds a command
 * to the first N-1 RULs so that on completion the next RUL is made active,
 * and puts the first RUL in the slot ready to be executed.
 *
 * Instead of having a function do this, this work can be done ahead of time
 * as long as the user knows exactly where each RUL is located in memory */
static void InitializeSlot
(
    BREG_Handle       hRegister,
    BMMA_Heap_Handle  hHeap,
    int               iSlotIndex,
    uint32_t          ulTriggerSelect,
    uint32_t         *pListCount,
    uint32_t         *pulList,
    int               iListCount,
    bool              bProgressive
)
{
    int              i;
    int              iNumEntries, iPrvNumEntries = 0, iCount, iRdcEntries = 6;
    uint32_t         ulAddrOffset;
    uint32_t         ulValue;
    uint32_t        *pulCurr;
    uint32_t         ulSlotOffset = (BCHP_RDC_desc_1_addr - BCHP_RDC_desc_0_addr) * iSlotIndex;
    bool             bArm = false, bDisArm = false;
    int              iRUL;
    BMMA_Block_Handle mmaBlock[30];
    void*             mmaSwAccess[30];
    BMMA_DeviceOffset mmaHwAccess[30];
    BERR_Code        rc;
#ifdef BCHP_RDC_sync_0_arm
    uint32_t         ulSyncId = iSlotIndex / 2;
    uint32_t         ulSlotArmOffset = (BCHP_RDC_sync_1_arm - BCHP_RDC_sync_0_arm) * ulSyncId;
    bArm = (iSlotIndex%2) ? true : (bProgressive ? true : false);
    bDisArm = (bProgressive) ? bArm : !bArm;
    if(bArm) iRdcEntries += 3;
    if(bDisArm) iRdcEntries += 3;
#endif
#ifdef BCHP_RDC_desc_0_count
    iRdcEntries += 6;
#endif

    BDBG_MSG(("InitializeSlot : iSlotIndex:%d ulTriggerSelect:%d pListCount:%p pulList:%p iListCount:%d",
			  iSlotIndex,   ulTriggerSelect,(void *)pListCount,(void *)pulList, iListCount));

    /************************
     * We have to place the dumped RULs into allocated memory. Instead a user could
     * have pre-determined memory locations for each of the RULs instead of directly
     * allocating them in an application. In that case, this entire loop is pre-defined.
     */
    iRUL = 0;
    for(i=0; i<iListCount; i++)
    {
        /* how many entries for this RUL? */
        iNumEntries = pListCount[i+1] - pListCount[i];

        /* RUL passed inspection */
        /* allocate aligned memory (add 6 more elements for extra entries -- see below) */
        mmaBlock[iRUL] = BMMA_Alloc(
            hHeap, sizeof(uint32_t) * (iNumEntries + iRdcEntries), 32, NULL);
        RememberPointer (mmaBlock[iRUL]);
        mmaSwAccess[iRUL] = BMMA_Lock(mmaBlock[iRUL]);
        mmaHwAccess[iRUL] = BMMA_LockOffset(mmaBlock[iRUL]);

        /* copy list into memory */
        BKNI_Memcpy(mmaSwAccess[iRUL], pulList + pListCount[i],
            sizeof(uint32_t) * iNumEntries);

		/* there is previous RUL in list? */
        if(iRUL > 0)
        {
            /* modify previous RUL (end of RUL) to install this new RUL when finished. This
             * can also be done ahead of time if the memory address is fixed. */

            /* get pointer to the current end of the list */
            pulCurr = (uint32_t*)(mmaSwAccess[iRUL-1]) + iPrvNumEntries;

            BDBG_MSG(("********* pulCurr = %p, mmaSwAccess[%d] = %p i=%d, iPrvNumEntries=%d",
                (void*)pulCurr, iRUL-1, mmaSwAccess[iRUL-1],i, iPrvNumEntries));

#ifdef BCHP_RDC_sync_0_arm
            if(bDisArm)
            {
                *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
                *pulCurr++ = BRDC_REGISTER(BCHP_RDC_sync_0_arm) + ulSlotArmOffset;
                *pulCurr++ =
                    BCHP_FIELD_DATA(RDC_sync_0_arm, arm, 0);
            }
#endif

            /* write command to install new RUL address into slot after the
               previous list is complete  */
            *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
            *pulCurr++ = BRDC_REGISTER(BCHP_RDC_desc_0_addr) + ulSlotOffset;
            *pulCurr++ =
                BCHP_FIELD_DATA(RDC_desc_0_addr, addr, mmaHwAccess[iRUL]);

            /* are we going to add more entries for this list (not the last list)? */
            iCount = (i != iListCount - 1) ? (iNumEntries+iRdcEntries) : iNumEntries ;

            /* write new count into slot */
            *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
            *pulCurr++ = BRDC_REGISTER(BCHP_RDC_desc_0_config) + ulSlotOffset;
            *pulCurr++ =
                #if defined(BCHP_RDC_desc_0_config_count_SHIFT)
                BCHP_FIELD_DATA(RDC_desc_0_config, count,          iCount - 1) |
                #endif
                BCHP_FIELD_DATA(RDC_desc_0_config, trigger_select, ulTriggerSelect) |
                #if defined(BCHP_RDC_desc_0_config_reserved0_SHIFT)
                BCHP_FIELD_DATA(RDC_desc_0_config, reserved0,      0)               |
                #elif defined(BCHP_RDC_desc_0_config_segmented_SHIFT)
                BCHP_FIELD_DATA(RDC_desc_0_config, segmented,      0)               |
                #endif
                BCHP_FIELD_DATA(RDC_desc_0_config, repeat,         1)               |
                #if defined(BCHP_RDC_desc_0_config_done_SHIFT)
                BCHP_FIELD_DATA(RDC_desc_0_config, done,           1)               |
                #endif
                #if BCHP_RDC_desc_0_config_sync_sel_MASK
                BCHP_FIELD_DATA(RDC_desc_0_config, sync_sel,       ulSyncId)        |
                #endif
                BCHP_FIELD_DATA(RDC_desc_0_config, enable,         1);

#ifdef BCHP_RDC_desc_0_count
            *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
            *pulCurr++ = BRDC_REGISTER(BCHP_RDC_desc_0_count) + ulSlotOffset;
            *pulCurr++ =
                BCHP_FIELD_DATA(RDC_desc_0_count, count,          iCount - 1);

            *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
            *pulCurr++ = BRDC_REGISTER(BCHP_RDC_desc_0_count_direct ) + ulSlotOffset;
            *pulCurr++ =
                BCHP_FIELD_DATA(RDC_desc_0_count_direct , count,          iCount - 1);
#endif

#ifdef BCHP_RDC_sync_0_arm
            if(bArm)
            {
                *pulCurr++ = BCHP_FIELD_ENUM(RDC_RUL, opcode, REG_WRITE_IMM) ;
                *pulCurr++ = BRDC_REGISTER(BCHP_RDC_sync_0_arm) + ulSlotArmOffset;
                *pulCurr++ =
                    BCHP_FIELD_DATA(RDC_sync_0_arm, arm, 1);
            }
#endif

            /* flush previous RUL: we just appended cmd to it to link the current RUL start addr */
            BMMA_FlushCache(
                mmaBlock[iRUL-1], mmaSwAccess[iRUL-1],
                sizeof(uint32_t)*(iPrvNumEntries+iRdcEntries));

            /* Remember this slot */
            if (iSlotIndex > s_highSlot) s_highSlot = iSlotIndex;
        }

        iRUL++;
        iPrvNumEntries = iNumEntries ;
    }

	/* flush the last RUL */
	BMMA_FlushCache(
            mmaBlock[iRUL-1], mmaSwAccess[iRUL-1],
            sizeof(uint32_t)*(iPrvNumEntries+iRdcEntries));

    /*****************************
     * Setup the initial RDC slot with the first list.
     * Again if the memory was fixed ahead of time then
     * the calculations below would be fixed and unchanging.
     */

    /* configure initial slot */
    iNumEntries = pListCount[1] - pListCount[0];
    if(iListCount > 1)
    {
        /* we have 6 more entries on this list */
        iNumEntries += iRdcEntries;
    }

#ifdef BCHP_RDC_sync_0_arm
    if(bDisArm)
    {
        BREG_Write32(hRegister, BCHP_RDC_sync_0_arm + ulSlotArmOffset, 0);
    }
#endif

    ulValue =
        #if defined(BCHP_RDC_desc_0_config_count_SHIFT)
        BCHP_FIELD_DATA(RDC_desc_0_config, count,          iNumEntries - 1) |
        #endif
        BCHP_FIELD_DATA(RDC_desc_0_config, trigger_select, ulTriggerSelect) |
		#if defined(BCHP_RDC_desc_0_config_reserved0_SHIFT)
		BCHP_FIELD_DATA(RDC_desc_0_config, reserved0,      0)               |
		#elif defined(BCHP_RDC_desc_0_config_segmented_SHIFT)
		BCHP_FIELD_DATA(RDC_desc_0_config, segmented,      0)               |
		#endif
        BCHP_FIELD_DATA(RDC_desc_0_config, repeat,         1)               |
        #if defined(BCHP_RDC_desc_0_config_done_SHIFT)
        BCHP_FIELD_DATA(RDC_desc_0_config, done,           1)               |
        #endif
        #if BCHP_RDC_desc_0_config_sync_sel_MASK
        BCHP_FIELD_DATA(RDC_desc_0_config, sync_sel,       ulSyncId)        |
        #endif
        BCHP_FIELD_DATA(RDC_desc_0_config, enable,         1);
    BREG_Write32(hRegister, BCHP_RDC_desc_0_config + ulSlotOffset,
        ulValue);

#ifdef BCHP_RDC_desc_0_count
    BREG_Write32(hRegister, BCHP_RDC_desc_0_count + ulSlotOffset,
        iNumEntries - 1);
    BREG_Write32(hRegister, BCHP_RDC_desc_0_count_direct + ulSlotOffset,
        iNumEntries - 1);
#endif

    /* put first entry into slot */
    BREG_WriteAddr(hRegister, BCHP_RDC_desc_0_addr + ulSlotOffset,
        mmaHwAccess[0]);

#ifdef BCHP_RDC_sync_0_arm
    if(bArm)
    {
        BREG_Write32(hRegister, BCHP_RDC_sync_0_arm + ulSlotArmOffset, 1);
    }
#endif

	/* Remember this slot */
	if (iSlotIndex > s_highSlot) s_highSlot = iSlotIndex;
}

void splash_bvn_init(
BREG_Handle hRegister, SplashBufInfo *pSplashBufInfo, SplashData* pSplashData)
{
    int  ii;
    int  iTriggerIndex;
	int  iSurIdx;

    if( 0 == pSplashData->nAulReg )
    {
        BDBG_ERR(("***************************************************************************")) ;
        BDBG_ERR(("Error ... Error ... Error ! "));
        BDBG_ERR(("Register Dump is empty !!! "));
        BDBG_ERR(("Looks like you have not chosen the instrumented version of REG basemodule")) ;
        BDBG_ERR(("Please rebuild/rerun the splash script generator with instrumented BREG ")) ;
        BDBG_ERR(("module")) ;
        BDBG_ERR(("***************************************************************************")) ;
        return ;
    }
    /* Dumped registers */
    for(ii=0 ; ii < pSplashData->nAulReg ; ++ii)
    {
        /* Write dumped register value */
        BREG_Write32(
			hRegister,
			*(pSplashData->pAulReg + 2*ii),
			*(pSplashData->pAulReg + 2*ii+1));
    }

#if CFG_SPLASH
	/* This is the I2C write to reset the scrambling setting
	 * of the HDMI reciever. */
	reset_HDMI_receiver_scrambling(pSplashData->uiHdmiChnNum);
#endif

	/* write our surface addr into RDC scratch registers,
	 * note that the RUL has been built to pick them up for gfx display */
	for (ii=0; ii<pSplashData->iNumDisplay; ii++)
	{
		iSurIdx = pSplashData->pDispInfo[ii].iSurIdx;
		BREG_WriteAddr(hRegister, pSplashData->pDispInfo[ii].ulRdcScratchReg0,
			pSplashBufInfo->aulSurfaceBufOffset[iSurIdx]);
		if (pSplashData->pDispInfo[ii].ulRdcScratchReg1)
		{
			BREG_WriteAddr(
				hRegister, pSplashData->pDispInfo[ii].ulRdcScratchReg1,
				pSplashBufInfo->aulSurfaceBufOffset[iSurIdx]);
		}
		BDBG_MSG(("** set surface offset 0x%8.8x for display %d",pSplashBufInfo->aulSurfaceBufOffset[iSurIdx], ii));
	}

    BDBG_MSG(("***** Number of triggers = %d", pSplashData->iNumTrigMap )) ;
    for( iTriggerIndex = 0 ;
	     iTriggerIndex < pSplashData->iNumTrigMap ;
		 ++iTriggerIndex)
    {
        bool bProgressive = false;
        if(pSplashData->pTrigMap[iTriggerIndex].TriggerHwNum != -1)
        {
            BDBG_MSG(("Slot number %d, Trigger number %d aList%d[]",
                pSplashData->pTrigMap[iTriggerIndex].SlotNum,
				pSplashData->pTrigMap[iTriggerIndex].TriggerHwNum,
				iTriggerIndex));

            if(IS_PROGRESSIVE(pSplashData->pDispInfo->eDspFmt) &&
               (pSplashData->pTrigMap[iTriggerIndex].SlotNum / 2) == 0) /* display 0 */
            {
                bProgressive = true;
            }

            /* Initialize slot for RUL list n */
            InitializeSlot(
                hRegister,
                pSplashBufInfo->hRulMma,
                pSplashData->pTrigMap[iTriggerIndex].SlotNum,
                pSplashData->pTrigMap[iTriggerIndex].TriggerHwNum,
                pSplashData->pTrigMap[iTriggerIndex].aListCountArray,
                pSplashData->pTrigMap[iTriggerIndex].aListArray,
                pSplashData->pTrigMap[iTriggerIndex].ListCountMaxIndex,
                bProgressive);
        }
    }

    for( iTriggerIndex = 0 ;
	     iTriggerIndex < pSplashData->iNumTrigMap ;
		 ++iTriggerIndex)
    {
        if(pSplashData->pTrigMap[iTriggerIndex].TriggerHwNum != -1)
        {
            /* Force Top slot to start engine for each display */
            if(0 == (pSplashData->pTrigMap[iTriggerIndex].SlotNum%2))
            {
                uint32_t ulSlotOffset =
                    (BCHP_RDC_desc_1_addr - BCHP_RDC_desc_0_addr) *
                    pSplashData->pTrigMap[iTriggerIndex].SlotNum;

                BDBG_MSG(("Writing immediate to %08x",
					BCHP_RDC_desc_0_immediate+ulSlotOffset));
                BREG_Write32(hRegister, BCHP_RDC_desc_0_immediate+ulSlotOffset, 1);
            }
        }
    }
}

void splash_bvn_uninit(BREG_Handle hRegister, SplashBufInfo *pSplashBufInfo)
{
	/* Program the register DMA controller to just stop */

	int iSlotIndex;
	for (iSlotIndex = 0 ; iSlotIndex <= s_highSlot ; ++iSlotIndex)
	{
        uint32_t ulSlotOffset = (BCHP_RDC_desc_1_addr - BCHP_RDC_desc_0_addr) * iSlotIndex;
		uint32_t ulValue =
            #if defined(BCHP_RDC_desc_0_config_count_SHIFT)
			BCHP_FIELD_DATA(RDC_desc_0_config, count,          0)              |
			#endif
			BCHP_FIELD_DATA(RDC_desc_0_config, trigger_select, 0)              |
			#if defined(BCHP_RDC_desc_0_config_reserved0_SHIFT)
			BCHP_FIELD_DATA(RDC_desc_0_config, reserved0,      0)              |
			#elif defined(BCHP_RDC_desc_0_config_segmented_SHIFT)
			BCHP_FIELD_DATA(RDC_desc_0_config, segmented,      0)              |
			#endif
			BCHP_FIELD_DATA(RDC_desc_0_config, repeat,         0)              |
            #if defined(BCHP_RDC_desc_0_config_done_SHIFT)
			BCHP_FIELD_DATA(RDC_desc_0_config, done,           1)              |
			#endif
			BCHP_FIELD_DATA(RDC_desc_0_config, enable,         0);
		BREG_Write32(hRegister, BCHP_RDC_desc_0_config + ulSlotOffset,
			ulValue);
	}

	/* It takes a non-zero amount of time for the old RULs to stop executing */
	BKNI_Sleep (500);

	/* Now the RULs can be freed, because RDC is no longer reading them. */
	FreePointers (pSplashBufInfo->hRulMma);
}

/* End of File */
