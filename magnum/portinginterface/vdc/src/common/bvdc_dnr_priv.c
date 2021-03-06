/***************************************************************************
 *     Copyright (c) 2004-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

#include "bstd.h"
#include "bkni.h"
#include "brdc.h"

#include "bvdc.h"
#include "bvdc_dnr_priv.h"
#include "bvdc_buffer_priv.h"
#include "bvdc_source_priv.h"
#include "bvdc_window_priv.h"
#include "bvdc_vnet_priv.h"
#include "bvdc_subrul_priv.h"

#if BVDC_P_SUPPORT_DNR
#include "bchp_mmisc.h"

BDBG_MODULE(BVDC_DNR);
BDBG_OBJECT_ID(BVDC_DNR);

#define BVDC_P_MAKE_DNR(pDnr, sw_init, id, channel_init)                        \
{                                                                               \
	(pDnr)->ulResetRegAddr  = BCHP_##sw_init;                                   \
	(pDnr)->ulResetMask     = BCHP_##sw_init##_DNR_##id##_MASK;                 \
	(pDnr)->ulVnetResetAddr = BCHP_##channel_init;                              \
	(pDnr)->ulVnetResetMask = BCHP_##channel_init##_DNR_##id##_MASK;            \
	(pDnr)->ulVnetMuxAddr   = BCHP_VNET_F_DNR_##id##_SRC;                       \
	(pDnr)->ulVnetMuxValue  = BCHP_VNET_B_CAP_0_SRC_SOURCE_DNR_##id;            \
}


#define BVDC_P_MNR_LIMIT_FILTER_SHIFT             (4)
#define BVDC_P_MNR_LIMIT_ALPHA_SHIFT              (3)

/***************************************************************************
 * {private}
 *
 */
BERR_Code BVDC_P_Dnr_Create
	( BVDC_P_Dnr_Handle            *phDnr,
	  BVDC_P_DnrId                  eDnrId,
	  BREG_Handle                   hRegister,
	  BVDC_P_Resource_Handle        hResource )
{
	uint32_t ulDnrIdx;
	BVDC_P_DnrContext *pDnr;
#if BCHP_DNR_0_HW_CONFIGURATION_DNR_CORE_MASK
	uint32_t ulReg;
#else
	BSTD_UNUSED(hRegister);
#endif

	BDBG_ENTER(BVDC_P_Dnr_Create);

	BDBG_ASSERT(phDnr);
	BDBG_ASSERT(eDnrId < BVDC_P_DnrId_eUnknown);

	/* (1) Alloc the context. */
	pDnr = (BVDC_P_DnrContext*)
		(BKNI_Malloc(sizeof(BVDC_P_DnrContext)));
	if(!pDnr)
	{
		return BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
	}

	/* Clear out the context and set defaults. */
	BKNI_Memset((void*)pDnr, 0x0, sizeof(BVDC_P_DnrContext));
	BDBG_OBJECT_SET(pDnr, BVDC_DNR);

	pDnr->eId              = eDnrId;
	ulDnrIdx = (eDnrId - BVDC_P_DnrId_eDnr0);

	/* DNR reset address */
	switch(eDnrId)
	{
#if BVDC_P_SUPPORT_NEW_SW_INIT
	case BVDC_P_DnrId_eDnr0:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  0, MMISC_VNET_B_CHANNEL_SW_INIT);
		BSTD_UNUSED(ulDnrIdx);
		break;
#if (BVDC_P_SUPPORT_DNR > 1)
	case BVDC_P_DnrId_eDnr1:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  1, MMISC_VNET_B_CHANNEL_SW_INIT);
		break;
#endif
#else
	case BVDC_P_DnrId_eDnr0:
	case BVDC_P_DnrId_eDnr1:
		pDnr->ulResetRegAddr = BCHP_MMISC_SOFT_RESET;
		pDnr->ulResetMask    = BCHP_MMISC_SOFT_RESET_DNR_0_MASK << (ulDnrIdx);
		pDnr->ulVnetMuxAddr  = BCHP_VNET_F_DNR_0_SRC + ((uint32_t)eDnrId) * sizeof(uint32_t);
		pDnr->ulVnetMuxValue =
			(eDnrId == BVDC_P_DnrId_eDnr0)?
			BCHP_VNET_B_CAP_0_SRC_SOURCE_DNR_0 :
			BCHP_VNET_B_CAP_0_SRC_SOURCE_DNR_1;
#if BCHP_MMISC_VNET_B_CHANNEL_RESET
		pDnr->ulVnetResetAddr = BCHP_MMISC_VNET_B_CHANNEL_RESET;
		pDnr->ulVnetResetMask = BCHP_MMISC_VNET_B_CHANNEL_RESET_DNR_0_RESET_MASK << (ulDnrIdx);
#endif
		break;
#endif
#if (BVDC_P_SUPPORT_DNR > 2)
	case BVDC_P_DnrId_eDnr2:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  2, MMISC_VNET_B_CHANNEL_SW_INIT);
		break;
#endif
#if (BVDC_P_SUPPORT_DNR > 3)
	case BVDC_P_DnrId_eDnr3:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  3, MMISC_VNET_B_CHANNEL_SW_INIT);
		break;
#endif
#if (BVDC_P_SUPPORT_DNR > 4)
	case BVDC_P_DnrId_eDnr4:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  4, MMISC_VNET_B_CHANNEL_SW_INIT);
		break;
#endif
#if (BVDC_P_SUPPORT_DNR > 5)
	case BVDC_P_DnrId_eDnr5:
		BVDC_P_MAKE_DNR(pDnr, MMISC_SW_INIT,  5, MMISC_VNET_B_CHANNEL_SW_INIT_1);
		break;
#endif
	default:
		BDBG_ERR(("Need to handle BVDC_P_Dnr_eDnr%d", eDnrId));
		BDBG_ASSERT(0);
		break;
	}

	pDnr->ulRegOffset = BVDC_P_DNR_GET_REG_OFFSET(eDnrId);

#if BCHP_DNR_0_HW_CONFIGURATION_DNR_CORE_MASK
	ulReg = BREG_Read32(hRegister, BCHP_DNR_0_HW_CONFIGURATION + pDnr->ulRegOffset);
	switch(BCHP_GET_FIELD_DATA(ulReg, DNR_0_HW_CONFIGURATION,  DNR_CORE))
	{
		case BCHP_DNR_0_HW_CONFIGURATION_DNR_CORE_DNR_H_WITH_DCR:
			pDnr->bDnrH = true;
			break;
		default:
			pDnr->bDnrH = false;
			break;
	}
#else
	pDnr->bDnrH = false;
#endif

	/* init the SubRul sub-module */
	BVDC_P_SubRul_Init(&(pDnr->SubRul), pDnr->ulVnetMuxAddr,
		pDnr->ulVnetMuxValue,BVDC_P_DrainMode_eBack, 0, hResource);

	/* All done. now return the new fresh context to user. */
	*phDnr = (BVDC_P_Dnr_Handle)pDnr;

	BDBG_LEAVE(BVDC_P_Dnr_Create);

	return BERR_SUCCESS;
}


/***************************************************************************
 * {private}
 *
 */
void BVDC_P_Dnr_Destroy
	( BVDC_P_Dnr_Handle             hDnr )
{
	BDBG_ENTER(BVDC_P_Dnr_Destroy);
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

	BDBG_OBJECT_DESTROY(hDnr, BVDC_DNR);
	/* [1] Free the context. */
	BKNI_Free((void*)hDnr);

	BDBG_LEAVE(BVDC_P_Dnr_Destroy);

	return;
}


/***************************************************************************
 * {private}
 *
 */
void BVDC_P_Dnr_Init_isr
	( BVDC_P_Dnr_Handle             hDnr )
{
	BDBG_ENTER(BVDC_P_Dnr_Init_isr);
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

	/* Clear out shadow registers. */
	BKNI_Memset_isr((void*)hDnr->aulRegs, 0x0, sizeof(hDnr->aulRegs));

#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
	/* Initial value */
	BVDC_P_DNR_GET_REG_DATA(DNR_0_EXT_FILT_CTRL) =
		BCHP_FIELD_ENUM(DNR_0_EXT_FILT_CTRL, ENABLE, DISABLE);
#endif

	/* Initialize state. */
	hDnr->bInitial      = true;
	hDnr->ulUpdateAll   = BVDC_P_RUL_UPDATE_THRESHOLD;
	hDnr->ulFilterLimit = 0;

	BDBG_LEAVE(BVDC_P_Dnr_Init_isr);

	return;
}

/***************************************************************************
 * {private}
 *
 * BVDC_P_Dnr_AcquireConnect_isr
 *
 * It is called by BVDC_Window_Validate after changing from diable dnr to
 * enable dnr.
 */
BERR_Code BVDC_P_Dnr_AcquireConnect_isr
	( BVDC_P_Dnr_Handle             hDnr,
	  BVDC_Source_Handle            hSource )
{
	BERR_Code  eResult = BERR_SUCCESS;

	BDBG_ENTER(BVDC_P_Dnr_AcquireConnect_isr);


	hDnr->hSource = hSource;
	BVDC_P_Dnr_Init_isr(hDnr);

	BDBG_LEAVE(BVDC_P_Dnr_AcquireConnect_isr);
	return BERR_TRACE(eResult);
}

/***************************************************************************
 * {private}
 *
 * BVDC_P_Dnr_ReleaseConnect_isr
 *
 * It is called after window decided that dnr is no-longer used by HW in its
 * vnet mode (i.e. it is really shut down and teared off from vnet).
 */
BERR_Code BVDC_P_Dnr_ReleaseConnect_isr
	( BVDC_P_Dnr_Handle         *phDnr )
{
	BERR_Code  eResult = BERR_SUCCESS;

	BDBG_ENTER(BVDC_P_Dnr_ReleaseConnect_isr);

	/* handle validation */
	if (NULL != *phDnr)
	{
		BDBG_OBJECT_ASSERT(*phDnr, BVDC_DNR);

		/* another win might still using it */
		BVDC_P_Resource_ReleaseHandle_isr(
			BVDC_P_SubRul_GetResourceHandle_isr(&(*phDnr)->SubRul),
			BVDC_P_ResourceType_eDnr, (void *)(*phDnr));

		/* this makes win to stop calling dnr code */
		*phDnr = NULL;
	}

	BDBG_LEAVE(BVDC_P_Dnr_ReleaseConnect_isr);
	return BERR_TRACE(eResult);
}

/***************************************************************************
 * {private}
 *
 * BVDC_P_Dnr_BuildRul_DrainVnet_isr
 *
 * called by BVDC_P_Dnr_BuildRul_isr after resetting to drain the module and
 * its pre-patch FreeCh or LpBack
 */
static void BVDC_P_Dnr_BuildRul_DrainVnet_isr
	( BVDC_P_Dnr_Handle              hDnr,
	  BVDC_P_ListInfo               *pList,
	  bool                           bNoCoreReset)
{
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

#if (BVDC_P_SUPPORT_DNR_VER<BVDC_P_SUPPORT_DNR_VER_7)
	/* reset sub and connect the module to a drain */
	BVDC_P_SubRul_Drain_isr(&(hDnr->SubRul), pList,
		hDnr->ulVnetResetAddr,hDnr->ulVnetResetMask,
		hDnr->ulVnetResetAddr, hDnr->ulVnetResetMask);
	BSTD_UNUSED(bNoCoreReset);
#else
	/* reset sub and connect the module to a drain */
	BVDC_P_SubRul_Drain_isr(&(hDnr->SubRul), pList,
		bNoCoreReset ? 0 : hDnr->ulResetRegAddr,
		bNoCoreReset ? 0 : hDnr->ulResetMask,
		bNoCoreReset ? 0 : hDnr->ulVnetResetAddr,
		bNoCoreReset ? 0 : hDnr->ulVnetResetMask);
#endif
}

/***************************************************************************
 * {private}
 *
 */
void BVDC_P_Dnr_BuildRul_isr
	( const BVDC_P_Dnr_Handle       hDnr,
	  BVDC_P_ListInfo              *pList,
	  BVDC_P_State                  eVnetState,
	  BVDC_P_PicComRulInfo         *pPicComRulInfo )
{
	uint32_t  ulRulOpsFlags;

	BDBG_ENTER(BVDC_P_Dnr_BuildRul_isr);
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

	/* currently this is only for vnet building / tearing-off*/

	ulRulOpsFlags = BVDC_P_SubRul_GetOps_isr(
		&(hDnr->SubRul), pPicComRulInfo->eWin, eVnetState, pList->bLastExecuted);

	if ((0 == ulRulOpsFlags) ||
		(ulRulOpsFlags & BVDC_P_RulOp_eReleaseHandle))
		return;
	else if (ulRulOpsFlags & BVDC_P_RulOp_eDisable)
	{
		BVDC_P_SubRul_DropOffVnet_isr(&(hDnr->SubRul), pList);
		BVDC_P_Dnr_SetEnable_isr(hDnr, false);
		BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_TOP_CTRL, pList->pulCurrent);
	}

	/* If rul failed to execute last time we'd re reprogrammed possible
	 * missing registers. */
	if((!pList->bLastExecuted) || (hDnr->bInitial))
	{
		hDnr->ulUpdateAll = BVDC_P_RUL_UPDATE_THRESHOLD;
	}

	/* reset DNR */
	if(hDnr->bInitial)
	{
#if (BVDC_P_SUPPORT_DNR_VER<BVDC_P_SUPPORT_DNR_VER_7)
		BVDC_P_BUILD_RESET(pList->pulCurrent,
			hDnr->ulResetRegAddr, hDnr->ulResetMask);
#endif
		hDnr->bInitial = false;
	}

	if (ulRulOpsFlags & BVDC_P_RulOp_eEnable)
	{
		if(hDnr->ulUpdateAll)
		{
			BDBG_MSG(("DNR update RUL"));

#if (BVDC_P_SUPPORT_DNR_VER <= BVDC_P_SUPPORT_DNR_VER_1)
			/* PR35221: Disabled/modify/afterward */
			BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_TOP_CTRL) &= ~(
				BCHP_MASK(DNR_0_DNR_TOP_CTRL, DNR_ENABLE));
			BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_TOP_CTRL, pList->pulCurrent);
			BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_TOP_CTRL) |=  (
				BCHP_FIELD_ENUM(DNR_0_DNR_TOP_CTRL, DNR_ENABLE, ENABLE));
#endif

#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_6)
			BVDC_P_DNR_BLOCK_WRITE_TO_RUL(DNR_0_SRC_PIC_SIZE, DNR_0_MNR_CONFIG, pList->pulCurrent);
#else
			BVDC_P_DNR_BLOCK_WRITE_TO_RUL(DNR_0_LINE_STORE_CONFIG, DNR_0_EXT_FILT_CONFIG, pList->pulCurrent);
#endif
			BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_DEMO_SETTING, pList->pulCurrent);
#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_7)
			BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_DEMO_CTRL, pList->pulCurrent);
			BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_VIDEO_MODE, pList->pulCurrent);
#endif
			BVDC_P_DNR_BLOCK_WRITE_TO_RUL(DNR_0_DCR_CTRL, DNR_0_DCR_DITH_OUT_CTRL, pList->pulCurrent);
			hDnr->ulUpdateAll--;
		}

		/* must be the last, and every field */
		BVDC_P_DNR_WRITE_TO_RUL(DNR_0_SRC_SCAN_SETTING, pList->pulCurrent);
		BVDC_P_DNR_WRITE_TO_RUL(DNR_0_DNR_TOP_CTRL,     pList->pulCurrent);

		/* join in vnet after enable. note: its src mux is initialed as disabled */
		if (ulRulOpsFlags & BVDC_P_RulOp_eVnetInit)
		{
			BVDC_P_SubRul_JoinInVnet_isr(&(hDnr->SubRul), pList);
		}
	}

	if (ulRulOpsFlags & BVDC_P_RulOp_eDrainVnet)
	{
		BVDC_P_Dnr_BuildRul_DrainVnet_isr(hDnr, pList, pPicComRulInfo->bNoCoreReset);
	}

	BDBG_LEAVE(BVDC_P_Dnr_BuildRul_isr);

	return;
}

/***************************************************************************
 * {private}
 *
 */
BERR_Code BVDC_P_Dnr_SetInfo_isr
	( BVDC_P_Dnr_Handle                hDnr,
	  const BVDC_P_PictureNodePtr      pPicture )
{
	uint32_t ulSrcVSize, ulSrcHSize;
	uint32_t ulMnrQp, ulBnrQp, ulDcrQp;
	uint32_t ulAdjQp, ulVOffset, ulHOffset;
	uint32_t ulDitherEn, ulDcrFiltEn;
	uint32_t ulBright0, ulBright1, ulBright2;
	const BVDC_P_Rect  *pScanOut = &pPicture->hBuffer->hWindow->stCurInfo.hSource->stScanOut;
	BVDC_P_Source_Info *pSrcInfo;
	BVDC_P_DcrCfgEntry *pDcrCfg;
	BVDC_P_BnrCfgEntry *pBnrCfg;
	BVDC_P_MnrCfgEntry *pMnrCfg;
	const BVDC_Dnr_Settings *pDnrSettings;
	BFMT_Orientation eOrientation;
	int32_t iBnrLevel, iMnrLevel, iDcrLevel;

	BDBG_ENTER(BVDC_P_Dnr_SetInfo_isr);
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

	ulSrcVSize = (pPicture->pDnrIn->ulHeight);
	ulSrcHSize = (pPicture->pDnrIn->ulWidth);
	eOrientation = pPicture->eOrigSrcOrientation;

	/* the Dnr source picture size is the feeder's scanout size */
	if(BAVC_Polarity_eFrame!=pPicture->PicComRulInfo.eSrcOrigPolarity)
	{
		ulSrcVSize = (ulSrcVSize + 1) / BVDC_P_FIELD_PER_FRAME;
	}

	pSrcInfo     = &hDnr->hSource->stCurInfo;
	pDnrSettings = &hDnr->hSource->stCurInfo.stDnrSettings;

#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_7)
        /* remapping DNR level according to ARBVN-41 */
        iBnrLevel =
                (pDnrSettings->iBnrLevel <= 100) ?
                (-20 + (4 * pDnrSettings->iBnrLevel / 5)) :
                (-80 + (14 * pDnrSettings->iBnrLevel / 10));
        iMnrLevel =
                (pDnrSettings->iMnrLevel <= 100) ?
                (-20 + (4 * pDnrSettings->iMnrLevel / 5)) :
                (-80 + (14 * pDnrSettings->iMnrLevel / 10));
        iDcrLevel =
                (pDnrSettings->iDcrLevel <= 100) ?
                (-20 + (4 * pDnrSettings->iDcrLevel / 5)) :
                (-80 + (14 * pDnrSettings->iDcrLevel / 10));
#else
	iBnrLevel = pDnrSettings->iBnrLevel;
	iMnrLevel = pDnrSettings->iMnrLevel;
	iDcrLevel = pDnrSettings->iDcrLevel;
#endif

	ulAdjQp = (pDnrSettings->ulQp != 0) ?
		pDnrSettings->ulQp : pPicture->ulAdjQp;

	/* setup DNR filters */
	ulMnrQp = (pDnrSettings->eMnrMode == BVDC_FilterMode_eEnable)
		? BVDC_P_DNR_CLAMP(0, BVDC_P_DNR_MAX_HW_QP_STEPS, (ulAdjQp *
		 (iMnrLevel + BVDC_QP_ADJUST_STEPS)/ BVDC_QP_ADJUST_STEPS))
		: 0;

	ulBnrQp = (pDnrSettings->eBnrMode == BVDC_FilterMode_eEnable)
		? BVDC_P_DNR_CLAMP(0, BVDC_P_DNR_MAX_HW_QP_STEPS, (ulAdjQp *
		 (iBnrLevel + BVDC_QP_ADJUST_STEPS)/ BVDC_QP_ADJUST_STEPS))
		: 0;

	/* No need to support Qp-dependent DCR adjustment. */
	ulDcrQp = (pDnrSettings->eDcrMode == BVDC_FilterMode_eEnable)
		? BVDC_P_DNR_CLAMP(0, BVDC_P_DNR_MAX_HW_QP_STEPS,
		 ((BVDC_P_DNR_MAX_HW_QP_STEPS / 2) *
		 (iDcrLevel + BVDC_QP_ADJUST_STEPS)/ BVDC_QP_ADJUST_STEPS))
		: 0;

	/* Every Vsync settings */
	/* panscan offset within an 8x8 block */
	BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_SCAN_SETTING) &= ~(
		  BCHP_MASK(DNR_0_SRC_SCAN_SETTING, H_OFFSET)
		| BCHP_MASK(DNR_0_SRC_SCAN_SETTING, V_OFFSET)
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
		| BCHP_MASK(DNR_0_SRC_SCAN_SETTING, SRC_FORMAT)
		| BCHP_MASK(DNR_0_SRC_SCAN_SETTING, SRC_FIELD_ID)
#endif
		);

	/* Internal computed or user computed */
	if(!pDnrSettings->bUserOffset)
	{
		ulVOffset = pScanOut->lTop  & 7;
		ulHOffset = pScanOut->lLeft & 7;
	}
	else
	{
		ulVOffset = pDnrSettings->ulVertOffset & 7;
		ulHOffset = pDnrSettings->ulHorzOffset & 7;
	}

	if(BAVC_Polarity_eTopField == pPicture->PicComRulInfo.eSrcOrigPolarity)
	{
		ulVOffset = (ulVOffset + 1) / 2;
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_SCAN_SETTING) |= (
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FORMAT,   FIELD) |
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FIELD_ID, TOP  ));
#endif
	}
	else if(BAVC_Polarity_eBotField == pPicture->PicComRulInfo.eSrcOrigPolarity)
	{
		ulVOffset = (ulVOffset + 0) / 2;
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_SCAN_SETTING) |= (
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FORMAT,   FIELD) |
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FIELD_ID, BOTTOM));
#endif
	}
	else
	{
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_SCAN_SETTING) |= (
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FORMAT,   FRAME) |
			BCHP_FIELD_ENUM(DNR_0_SRC_SCAN_SETTING, SRC_FIELD_ID, TOP  ));
#endif
	}

	BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_SCAN_SETTING) |= (
		BCHP_FIELD_DATA(DNR_0_SRC_SCAN_SETTING, V_OFFSET, ulVOffset) |
		BCHP_FIELD_DATA(DNR_0_SRC_SCAN_SETTING, H_OFFSET, ulHOffset));

	/* Source size and destination size.   Detecting dynamics format (size)
	 * change.  Base on these information we can further bypass
	 * unnecessary computations. */
	if((hDnr->ulMnrQp != ulMnrQp) ||
	   (hDnr->ulBnrQp != ulBnrQp) ||
	   (hDnr->ulDcrQp != ulDcrQp) ||
	   (hDnr->eOrientation != eOrientation ) ||
	   (pSrcInfo->stDirty.stBits.bDnrAdjust) ||
	   !BVDC_P_DNR_COMPARE_FIELD_DATA(DNR_0_SRC_PIC_SIZE, HSIZE, ulSrcHSize) ||
	   !BVDC_P_DNR_COMPARE_FIELD_DATA(DNR_0_SRC_PIC_SIZE, VSIZE, ulSrcVSize) ||
	   !BVDC_P_DNR_COMPARE_FIELD_DATA(DNR_0_DNR_TOP_CTRL, DNR_ENABLE, 1))
	{
		/* Always re-enable after set info. */
		uint32_t ulDnrDemoEnable, ulDnrDemoSide, ulDnrDemoBoundary;

		ulDnrDemoEnable = (pSrcInfo->stSplitScreenSetting.eDnr == BVDC_SplitScreenMode_eDisable) ? 0 :1;
		ulDnrDemoSide = (pSrcInfo->stSplitScreenSetting.eDnr == BVDC_SplitScreenMode_eLeft) ?
			BCHP_DNR_0_DNR_DEMO_SETTING_DEMO_L_R_LEFT : BCHP_DNR_0_DNR_DEMO_SETTING_DEMO_L_R_RIGHT;
		ulDnrDemoBoundary = ulSrcHSize / 2;

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_TOP_CTRL) =
#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_5)
			BCHP_FIELD_DATA(DNR_0_DNR_TOP_CTRL, DNR_ENABLE_MODE, 1) |
#endif
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_7)
			BCHP_FIELD_DATA(DNR_0_DNR_TOP_CTRL, DNR_DEMO_ENABLE, ulDnrDemoEnable) |
#endif
			BCHP_FIELD_ENUM(DNR_0_DNR_TOP_CTRL, DNR_ENABLE, ENABLE);

#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_7)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_DEMO_CTRL) =
			BCHP_FIELD_DATA(DNR_0_DNR_DEMO_CTRL, DNR_DEMO_ENABLE, ulDnrDemoEnable);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_VIDEO_MODE) =
			BCHP_FIELD_DATA(DNR_0_DNR_VIDEO_MODE, BVB_VIDEO, eOrientation);
#endif

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_DEMO_SETTING) &= ~(
			BCHP_MASK(DNR_0_DNR_DEMO_SETTING, DEMO_L_R) |
			BCHP_MASK(DNR_0_DNR_DEMO_SETTING, DEMO_BOUNDARY));

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_DEMO_SETTING) |=  (
			BCHP_FIELD_DATA(DNR_0_DNR_DEMO_SETTING, DEMO_L_R, ulDnrDemoSide) |
			BCHP_FIELD_DATA(DNR_0_DNR_DEMO_SETTING, DEMO_BOUNDARY, ulDnrDemoBoundary));

		pSrcInfo->stDirty.stBits.bDnrAdjust = false;
		hDnr->ulMnrQp = ulMnrQp;
		hDnr->ulBnrQp = ulBnrQp;
		hDnr->ulDcrQp = ulDcrQp;
		hDnr->eOrientation = eOrientation;
		hDnr->ulUpdateAll = BVDC_P_RUL_UPDATE_THRESHOLD;

		/* Set src size. */
		BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_PIC_SIZE) &= ~(
			BCHP_MASK(DNR_0_SRC_PIC_SIZE, HSIZE) |
			BCHP_MASK(DNR_0_SRC_PIC_SIZE, VSIZE));

		BVDC_P_DNR_GET_REG_DATA(DNR_0_SRC_PIC_SIZE) |=  (
			BCHP_FIELD_DATA(DNR_0_SRC_PIC_SIZE, HSIZE, ulSrcHSize) |
			BCHP_FIELD_DATA(DNR_0_SRC_PIC_SIZE, VSIZE, ulSrcVSize));

#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
#ifndef BCHP_DNR_0_LINE_STORE_CONFIG_MOD_WIDTH_MASK
		/* line store config */
		BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) &=
			~(BCHP_MASK(DNR_0_LINE_STORE_CONFIG, LS_MODE));

		if(ulSrcHSize <= BFMT_NTSC_WIDTH)
		{
			BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) |=
				(BCHP_FIELD_ENUM(DNR_0_LINE_STORE_CONFIG, LS_MODE, SD));
		}
		else
		{
			BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) |=
				(BCHP_FIELD_ENUM(DNR_0_LINE_STORE_CONFIG, LS_MODE, HD));
		}
#else
		/* line store config */
		BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) &= ~(
			BCHP_MASK(DNR_0_LINE_STORE_CONFIG, LS_MODE) |
			BCHP_MASK(DNR_0_LINE_STORE_CONFIG, MOD_WIDTH));

		if(ulSrcHSize <= BFMT_NTSC_WIDTH)
		{
			BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) |=  (
				BCHP_FIELD_ENUM(DNR_0_LINE_STORE_CONFIG, LS_MODE, SD) |
				BCHP_FIELD_DATA(DNR_0_LINE_STORE_CONFIG, MOD_WIDTH, ulSrcHSize));
		}
		else
		{
			BVDC_P_DNR_GET_REG_DATA(DNR_0_LINE_STORE_CONFIG) |=  (
				BCHP_FIELD_ENUM(DNR_0_LINE_STORE_CONFIG, LS_MODE, HD) |
				BCHP_FIELD_DATA(DNR_0_LINE_STORE_CONFIG, MOD_WIDTH, (ulSrcHSize + 2)/3));
		}
#endif
#endif

		/* Block Noise Reduction (BNR) */
		pBnrCfg = (BVDC_P_BnrCfgEntry *)BVDC_P_Dnr_GetBnrCfg_isr(ulBnrQp,
			pPicture->PicComRulInfo.eSrcOrigPolarity,
			pSrcInfo->pFmtInfo, pDnrSettings->pvUserInfo);

		/* If Get funtion returns NULL pointer, generate the BNR config table */
		/* locally, otherwise, copying the BNR config table to the context */
		if(!pBnrCfg)
		{
			pBnrCfg = &hDnr->stBnrCfg;
			pBnrCfg->ulHBnr = (ulBnrQp > 0);
			pBnrCfg->ulVBnr = (ulBnrQp > 0);
			pBnrCfg->ulLrLimit = (ulBnrQp == 0) ? 0 : BVDC_P_DNR_CLAMP(2, 14, 2 + (ulBnrQp >> 2) );
			pBnrCfg->ulSmallGrid = (pPicture->PicComRulInfo.eSrcOrigPolarity != BAVC_Polarity_eFrame);
#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_3)
			pBnrCfg->ulHRel = (ulBnrQp == 0) ? 0 :
				BVDC_P_DNR_CLAMP(4 - (uint32_t)(iBnrLevel + 100) / 150, BCHP_GET_FIELD_DATA(~0, DNR_0_VBNR_CONFIG, VBNR_REL),
				2 + 14 / ulBnrQp);
			pBnrCfg->ulVRel = (ulBnrQp == 0) ? 0 :
				BVDC_P_DNR_CLAMP(4 - (uint32_t)(iBnrLevel + 100) / 150, BCHP_GET_FIELD_DATA(~0, DNR_0_VBNR_CONFIG, VBNR_REL),
				2 + 14 / ulBnrQp);
			if(hDnr->bDnrH)
			{
				if(iBnrLevel < 100)
				{
					if(pBnrCfg->ulVRel < 5)
						pBnrCfg->ulVRel = 5;
				}
				else
				{
					if(pBnrCfg->ulVRel < 4)
						pBnrCfg->ulVRel = 4;
				}
			}
#else
			pBnrCfg->ulHRel = (ulBnrQp == 0) ? 0 :
				BVDC_P_DNR_CLAMP(4 - (uint32_t)(iBnrLevel + 100) / 150, BCHP_GET_FIELD_DATA(~0, DNR_0_BNR_CONFIG, BNR_REL),
				2 + 14 / ulBnrQp);
			pBnrCfg->ulVRel = (ulBnrQp == 0) ? 0 :
				BVDC_P_DNR_CLAMP(4 - (uint32_t)(iBnrLevel + 100) / 150, BCHP_GET_FIELD_DATA(~0, DNR_0_BNR_CONFIG, BNR_REL),
				2 + 14 / ulBnrQp);
#endif
			pBnrCfg->ulHLimit = BVDC_P_DNR_CLAMP(0, 80 + 25 * (uint32_t)((iBnrLevel + 100) / 150), (7 * ulBnrQp / 2));
			pBnrCfg->ulVLimit = BVDC_P_DNR_CLAMP(0, 80 + 25 * (uint32_t)((iBnrLevel + 100) / 150), (7 * ulBnrQp / 2));
		}
		else
		{
			hDnr->stBnrCfg = *pBnrCfg;
			pBnrCfg = &hDnr->stBnrCfg;
		}

		BVDC_P_DNR_GET_REG_DATA(DNR_0_BNR_CTRL) = (
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
			BCHP_FIELD_DATA(DNR_0_BNR_CTRL, HBNR_ENABLE, pBnrCfg->ulHBnr) |
#endif
			BCHP_FIELD_DATA(DNR_0_BNR_CTRL, VBNR_ENABLE, pBnrCfg->ulVBnr));
#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_3)
		/* TODO: program with the correct settings for VBNR and HBNR REL and LIMIT */
		BVDC_P_DNR_GET_REG_DATA(DNR_0_VBNR_CONFIG) &= ~(
			BCHP_MASK(DNR_0_VBNR_CONFIG, VBNR_LR_LIMIT) |
			BCHP_MASK(DNR_0_VBNR_CONFIG, VBNR_REL) |
			BCHP_MASK(DNR_0_VBNR_CONFIG, VBNR_LIMIT));
		BVDC_P_DNR_GET_REG_DATA(DNR_0_VBNR_CONFIG) |=
			BCHP_FIELD_DATA(DNR_0_VBNR_CONFIG, VBNR_LR_LIMIT, pBnrCfg->ulLrLimit) |
			BCHP_FIELD_DATA(DNR_0_VBNR_CONFIG, VBNR_REL,      pBnrCfg->ulVRel)     |
			BCHP_FIELD_DATA(DNR_0_VBNR_CONFIG, VBNR_LIMIT,    pBnrCfg->ulVLimit);
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_HBNR_CONFIG) &= ~(
			BCHP_MASK(DNR_0_HBNR_CONFIG, HBNR_SMALL_GRID) |
			BCHP_MASK(DNR_0_HBNR_CONFIG, HBNR_REL) |
			BCHP_MASK(DNR_0_HBNR_CONFIG, HBNR_LIMIT));
		BVDC_P_DNR_GET_REG_DATA(DNR_0_HBNR_CONFIG) |=
			BCHP_FIELD_DATA(DNR_0_HBNR_CONFIG, HBNR_SMALL_GRID, pBnrCfg->ulSmallGrid) |
			BCHP_FIELD_DATA(DNR_0_HBNR_CONFIG, HBNR_REL,        pBnrCfg->ulHRel)     |
			BCHP_FIELD_DATA(DNR_0_HBNR_CONFIG, HBNR_LIMIT,      pBnrCfg->ulHLimit);
#endif
#else
		BVDC_P_DNR_GET_REG_DATA(DNR_0_BNR_CONFIG) &= ~(
			BCHP_MASK(DNR_0_BNR_CONFIG, VBNR_LR_LIMIT) |
			BCHP_MASK(DNR_0_BNR_CONFIG, BNR_REL) |
			BCHP_MASK(DNR_0_BNR_CONFIG, BNR_LIMIT) |
			BCHP_MASK(DNR_0_BNR_CONFIG, HBNR_SMALL_GRID));
		BVDC_P_DNR_GET_REG_DATA(DNR_0_BNR_CONFIG) |=
			BCHP_FIELD_DATA(DNR_0_BNR_CONFIG, VBNR_LR_LIMIT,   pBnrCfg->ulLrLimit) |
			BCHP_FIELD_DATA(DNR_0_BNR_CONFIG, BNR_REL,         pBnrCfg->ulHRel)     |
			BCHP_FIELD_DATA(DNR_0_BNR_CONFIG, BNR_LIMIT,       pBnrCfg->ulHLimit)   |
			BCHP_FIELD_DATA(DNR_0_BNR_CONFIG, HBNR_SMALL_GRID, pBnrCfg->ulSmallGrid);
#endif

		/* Mosquito Noise Reduction (MNR) */
		pMnrCfg = (BVDC_P_MnrCfgEntry *)BVDC_P_Dnr_GetMnrCfg_isr(ulMnrQp,
			ulSrcHSize, pSrcInfo->pFmtInfo, pDnrSettings->pvUserInfo);

		/* If Get funtion returns NULL pointer, generate the MNR config table */
		/* locally, otherwise, copying the MNR config table to the context */
		if(!pMnrCfg)
		{
			uint32_t ulMnrMerge;

			pMnrCfg = &hDnr->stMnrCfg;
			pMnrCfg->ulMnr = (ulMnrQp > 0);
			pMnrCfg->ulSpot = (ulMnrQp > 10);
			ulMnrMerge = ulMnrQp / 6 + ((ulSrcHSize > BFMT_NTSC_WIDTH) ? 1 : 0);

			pMnrCfg->ulMerge = BVDC_P_DNR_CLAMP(0,
				BVDC_P_MIN(BCHP_GET_FIELD_DATA(~0, DNR_0_MNR_CONFIG, MNR_MERGE), 4),
				ulMnrMerge);

#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_6)
			pMnrCfg->ulRel = BVDC_P_DNR_CLAMP(36 - (3 * (uint32_t)((iMnrLevel + 100) / 150)),
				BVDC_P_MIN(BCHP_GET_FIELD_DATA(~0, DNR_0_MNR_CONFIG, MNR_REL), 120),
				120 - (ulMnrQp * 7) / 2);
#else
			pMnrCfg->ulRel = BVDC_P_DNR_CLAMP(12 - (uint32_t)((iMnrLevel + 100) / 150),
				BVDC_P_MIN(BCHP_GET_FIELD_DATA(~0, DNR_0_MNR_CONFIG, MNR_REL), 100),
				40 - (ulMnrQp * 7) / 2);
#endif

			/* PR32066: zoneplate pattern cause video flicking/beating when either
			 * BNR or MNR is turned on. */
			/*
			pMnrCfg->ulLimit = BVDC_P_DNR_CLAMP(0,
				BCHP_GET_FIELD_DATA(~0, DNR_0_MNR_CONFIG, MNR_LIMIT),
				ulMnrQp * 14 / 10); */
			/* PR34386: */
			/*pMnrCfg->ulLimit = 0;*/
#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_6)
			pMnrCfg->ulLimit = BVDC_P_DNR_CLAMP(0, 30 + 7 * (uint32_t)((iMnrLevel + 100) / 150), ((ulMnrQp > 3)? ulMnrQp-3 : 0) * 10 / 10);
#else
			pMnrCfg->ulLimit = BVDC_P_DNR_CLAMP(0, 50 + 7 * (uint32_t)((iMnrLevel + 100) / 150), ((ulMnrQp > 3)? ulMnrQp-3 : 0) * 15 / 10);
#endif
			if(hDnr->ulFilterLimit == 0)
			{
				hDnr->ulFilterLimit = pMnrCfg->ulLimit * 100;
			}
			if(pMnrCfg->ulLimit * 10000 > hDnr->ulFilterLimit * 110)
			{
				hDnr->ulFilterLimit = hDnr->ulFilterLimit * 110;
			}
			else if(pMnrCfg->ulLimit * 10000 < hDnr->ulFilterLimit * 98)
			{
				hDnr->ulFilterLimit = hDnr->ulFilterLimit * 98;
			}
			else
			{
				hDnr->ulFilterLimit = pMnrCfg->ulLimit * 10000;
			}

			pMnrCfg->ulLimit = (hDnr->ulFilterLimit + 5000) / 10000;
			hDnr->ulFilterLimit = hDnr->ulFilterLimit / 100;
		}
		else
		{
			hDnr->stMnrCfg = *pMnrCfg;
			pMnrCfg = &hDnr->stMnrCfg;
		}

		BVDC_P_DNR_GET_REG_DATA(DNR_0_MNR_CTRL) =
			BCHP_FIELD_DATA(DNR_0_MNR_CTRL, MNR_ENABLE, pMnrCfg->ulMnr);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_MNR_CONFIG) &= ~(
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_SPOT) |
#endif
#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_7)
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_FILTER_SIZE) |
#endif
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_MERGE) |
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_REL) |
#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_2)
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_BLOCK_BOUND) |
#endif
			BCHP_MASK(DNR_0_MNR_CONFIG, MNR_LIMIT));

		BVDC_P_DNR_GET_REG_DATA(DNR_0_MNR_CONFIG) |= (
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_SPOT,  pMnrCfg->ulSpot ) |
#endif
#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_7)
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_FILTER_SIZE, 0         ) |
#endif
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_MERGE, pMnrCfg->ulMerge) |
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_REL,   pMnrCfg->ulRel  ) |
#if (BVDC_P_SUPPORT_DNR_VER >= BVDC_P_SUPPORT_DNR_VER_2)
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_BLOCK_BOUND, 1         ) |
#endif
			BCHP_FIELD_DATA(DNR_0_MNR_CONFIG, MNR_LIMIT, pMnrCfg->ulLimit));

		pDcrCfg = (BVDC_P_DcrCfgEntry *)BVDC_P_Dnr_GetDcrCfg_isr(ulDcrQp,
			pSrcInfo->pFmtInfo, pDnrSettings->pvUserInfo);
		hDnr->stDcrCfg = *pDcrCfg;
		pDcrCfg = &hDnr->stDcrCfg;

#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_0)
		ulDitherEn = 0;
#else
		ulDitherEn = (hDnr->hSource->bIs10BitCore)? 0 : (ulDcrQp > 0);
#endif
		ulDcrFiltEn = (ulDcrQp > 0);
		ulBright0 = 0;
		ulBright1 = 1;
		ulBright2 = 0;

		/* PR47349: always disable DITH in DCR block for 10-bit chips */
		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_CTRL) =
			BCHP_FIELD_DATA(DNR_0_DCR_CTRL, DITH_ENABLE, ulDitherEn  ) |
			BCHP_FIELD_DATA(DNR_0_DCR_CTRL, FILT_ENABLE, ulDcrFiltEn );

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_FILT_LIMIT) =
#if (BVDC_P_SUPPORT_DNR_VER < BVDC_P_SUPPORT_DNR_VER_6)
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_LIMIT, FILT_3_LIMIT, pDcrCfg->ulFilt3Limit) |
#endif
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_LIMIT, FILT_2_LIMIT, pDcrCfg->ulFilt2Limit) |
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_LIMIT, FILT_1_LIMIT, pDcrCfg->ulFilt1Limit) |
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_LIMIT, FILT_0_LIMIT, pDcrCfg->ulFilt0Limit);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_FILT_CONFIG) =
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_CONFIG, BRIGHT_0,   ulBright0    ) |
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_CONFIG, BRIGHT_1,   ulBright1    ) |
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_CONFIG, BRIGHT_2,   ulBright2    ) |
			BCHP_FIELD_DATA(DNR_0_DCR_FILT_CONFIG, FILT_CLAMP, pDcrCfg->ulFiltClamp);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_ORDER_PATTERN) =
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_PATTERN, ALTERNATE_Y, 0) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_PATTERN, ALTERNATE_X, 1) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_PATTERN, INVERT_Y,    0) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_PATTERN, INVERT_X,    0) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_PATTERN, AUTO_DITH,   1);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_ORDER_VALUE) =
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_VALUE, ORDER_A, pDcrCfg->ulOrderA) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_VALUE, ORDER_B, pDcrCfg->ulOrderB);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_RANDOM_PATTERN) =
			BCHP_FIELD_ENUM(DNR_0_DCR_DITH_RANDOM_PATTERN, RNG_MODE, RUN   ) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_RANDOM_PATTERN, RNG_SEED, 0x1111);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_RANDOM_VALUE) =
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_RANDOM_VALUE, RANDOM_A, pDcrCfg->ulRandomA) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_RANDOM_VALUE, RANDOM_B, pDcrCfg->ulRandomB) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_RANDOM_VALUE, RANDOM_C, pDcrCfg->ulRandomC) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_RANDOM_VALUE, RANDOM_D, pDcrCfg->ulRandomD);

		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_OUT_CTRL) =
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_OUT_CTRL, DITH_CLAMP, 7);

#if (BVDC_P_SUPPORT_DNR_VER == BVDC_P_SUPPORT_DNR_VER_0)
		BVDC_P_DNR_GET_REG_DATA(DNR_0_DCR_DITH_ORDER_VALUE) =
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_VALUE, ORDER_A, 0) |
			BCHP_FIELD_DATA(DNR_0_DCR_DITH_ORDER_VALUE, ORDER_B, 0);
#endif

		BDBG_MSG(("-------------------------"));
		BDBG_MSG(("Dnr[%d]         : %dx%d", hDnr->eId - BVDC_P_DnrId_eDnr0,
			pPicture->pDnrIn->ulWidth,  pPicture->pDnrIn->ulHeight));
		BDBG_MSG(("Dnr[%d]'offset  : %dx%d", hDnr->eId - BVDC_P_DnrId_eDnr0,
			ulHOffset, ulVOffset));
		BDBG_MSG(("ulAdjQp        : %d", ulAdjQp));
		BDBG_MSG(("iBnrLevel: %d - ulBnrQp: %d", iBnrLevel, ulBnrQp));
		BDBG_MSG(("iMnrLevel: %d - ulMnrQp: %d", iMnrLevel, ulMnrQp));
		BDBG_MSG(("iDcrLevel: %d - ulDcrQp: %d", iDcrLevel, ulDcrQp));
		BDBG_MSG(("SrcPolarity    : %d", pPicture->PicComRulInfo.eSrcOrigPolarity));
	}

	BDBG_LEAVE(BVDC_P_Dnr_SetInfo_isr);

	return BERR_SUCCESS;
}

/***************************************************************************
 * {private}
 *
 */
BERR_Code BVDC_P_Dnr_SetEnable_isr
	( BVDC_P_Dnr_Handle                hDnr,
	  bool                             bEnable )
{
	BDBG_OBJECT_ASSERT(hDnr, BVDC_DNR);

	if(!BVDC_P_DNR_COMPARE_FIELD_DATA(DNR_0_DNR_TOP_CTRL, DNR_ENABLE, bEnable))
	{
		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_TOP_CTRL) &= ~(
			BCHP_MASK(DNR_0_DNR_TOP_CTRL, DNR_ENABLE));
		BVDC_P_DNR_GET_REG_DATA(DNR_0_DNR_TOP_CTRL) |=  bEnable;
		hDnr->ulUpdateAll = BVDC_P_RUL_UPDATE_THRESHOLD;
	}

	return BERR_SUCCESS;
}

#else

/***************************************************************************
 * Stub function for chipset with no DNR.
 */
BERR_Code BVDC_P_Dnr_Create
	( BVDC_P_Dnr_Handle            *phDnr,
	  BVDC_P_DnrId                  eDnrId,
	  BVDC_P_Resource_Handle        hResource )
{
	BSTD_UNUSED(phDnr);
	BSTD_UNUSED(eDnrId);
	BSTD_UNUSED(hResource);
	return BERR_SUCCESS;
}

void BVDC_P_Dnr_Destroy
	( BVDC_P_Dnr_Handle             hDnr )
{
	BSTD_UNUSED(hDnr);
	return;
}

void BVDC_P_Dnr_Init_isr
	( BVDC_P_Dnr_Handle             hDnr )
{
	BSTD_UNUSED(hDnr);
	return;
}

BERR_Code BVDC_P_Dnr_AcquireConnect_isr
	( BVDC_P_Dnr_Handle             hDnr,
	  BVDC_Source_Handle            hSource )
{
	BSTD_UNUSED(hDnr);
	BSTD_UNUSED(hSource);
	return BERR_SUCCESS;
}

BERR_Code BVDC_P_Dnr_ReleaseConnect_isr
	( BVDC_P_Dnr_Handle            *phDnr )
{
	BSTD_UNUSED(phDnr);
	return BERR_SUCCESS;
}

void BVDC_P_Dnr_BuildRul_isr
	( const BVDC_P_Dnr_Handle       hDnr,
	  BVDC_P_ListInfo              *pList,
	  BVDC_P_State                  eVnetState,
	  BVDC_P_PicComRulInfo         *pPicComRulInfo )
{
	BSTD_UNUSED(hDnr);
	BSTD_UNUSED(pList);
	BSTD_UNUSED(eVnetState);
	BSTD_UNUSED(pPicComRulInfo);
	return;
}

BERR_Code BVDC_P_Dnr_SetInfo_isr
	( BVDC_P_Dnr_Handle                hDnr,
	  const BVDC_P_PictureNodePtr      pPicture )
{
	BSTD_UNUSED(hDnr);
	BSTD_UNUSED(pPicture);
	return BERR_SUCCESS;
}

BERR_Code BVDC_P_Dnr_SetEnable_isr
	( BVDC_P_Dnr_Handle                hDnr,
	  bool                             bEnable )
{
	BSTD_UNUSED(hDnr);
	BSTD_UNUSED(bEnable);
	return BERR_SUCCESS;
}

#endif

/* End of file. */
