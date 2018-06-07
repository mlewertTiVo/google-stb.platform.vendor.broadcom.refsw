/******************************************************************************
 *  Copyright (C) 2018 Broadcom.
 *  The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to
 *  the terms and conditions of a separate, written license agreement executed
 *  between you and Broadcom (an "Authorized License").  Except as set forth in
 *  an Authorized License, Broadcom grants no license (express or implied),
 *  right to use, or waiver of any kind with respect to the Software, and
 *  Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 *  THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 *  IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization,
 *  constitutes the valuable trade secrets of Broadcom, and you shall use all
 *  reasonable efforts to protect the confidentiality thereof, and to use this
 *  information only in connection with your use of Broadcom integrated circuit
 *  products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 *  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 *  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 *  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 *  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 *  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 *  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 *  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 *  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 *  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 *  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 *  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/

#include "bstd.h"
#include "bhsm.h"
#include "bhsm_keyslot.h"
#include "bhsm_keyslot_priv.h"
#include "bhsm_priv.h"
#include "bhsm_bsp_msg.h"
#include "bsp_types.h"
#include "bhsm_p_keyslot.h"
#include "bhsm_p_keyladder.h"
#include "bsp_p_hw.h"
#ifdef BHSM_BUILD_HSM_FOR_SAGE
  #include "bhsm_keyslot_restricted.h"
#endif

BDBG_MODULE(BHSM);

#define BHSM_MAX_KEYSLOTS               (228)/*(1024)*/   /* TODO move to headerfile  */
#define BHSM_EXTERNAL_KEYSLOT_KEY_SIZE  (2)      /* 2*64bits */
#define BHSM_EXTERNAL_KEYSLOT_SLOT_SIZE (8)      /* number of 64bit locations per slot  */
#define BHSM_EXTERNAL_KEYSLOTS_MAX      (512/BHSM_EXTERNAL_KEYSLOT_SLOT_SIZE)

#define BHSM_PRESERVE_OWNERSHIP (true)

#define BHSM_NUM_BYPASS_KEYSLOTS  (3)



typedef struct BHSM_KeySlotModule
{
    BHSM_Handle hHsm;                         /* HSM handle. */

    BHSM_P_KeySlot *pKeySlotHandles[BHSM_MAX_KEYSLOTS]; /* array of dynamically allocated keyslots */

    struct{
        unsigned offset;     /* offset into pKeySlotHandles of first slot of this type */
        unsigned maxNumber;  /* number of slots of this type. */
    }types[BHSM_KeyslotType_eMax];

    BHSM_P_ExternalKeySlot externalKeySlots[BHSM_EXTERNAL_KEYSLOTS_MAX];

    BHSM_KeyslotHandle hBypassKeyslotG2GR; /* default */
    BHSM_KeyslotHandle hBypassKeyslotGR2R;
    BHSM_KeyslotHandle hBypassKeyslotGT2T;

}BHSM_KeySlotModule;

static BHSM_Handle _Keyslot_GetHsmHandle( BHSM_KeyslotHandle handle );
static BERR_Code _SetEntryKey( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry, const BHSM_KeyslotKey *pKey );
static BERR_Code _SetEntryIv( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry, const BHSM_KeyslotIv *pIv, const BHSM_KeyslotIv* pIv2);
static BERR_Code _InvalidateSlot( BHSM_KeyslotHandle handle, bool preserveOwner );
static BERR_Code _InvalidateEntry( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry );
static BHSM_KeyslotSettings* _GetSlotSetting( BHSM_KeyslotHandle handle );
static BHSM_P_KeyEntry* _GetEntry( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry );
static BERR_Code compileControl0_GlobalHi( BHSM_KeyslotHandle handle );
static BERR_Code compileControl1_GlobalLo( BHSM_KeyslotHandle handle );
static BERR_Code compileControl2_ModeHi( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry );
static BERR_Code compileControl3_ModeLo( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry );
static BHSM_KeyslotBlockType _GetEntryBlockType( BHSM_KeyslotBlockEntry entry );
static BHSM_KeyslotPolarity _GetEntryPolarity( BHSM_KeyslotBlockEntry entry );
static BERR_Code _SetKeyslotOwnership( BHSM_KeyslotHandle handle, BHSM_SecurityCpuContext owner );
static BERR_Code _GetKeyslotOwnership( BHSM_KeyslotHandle handle, BHSM_SecurityCpuContext* pOwner );
static uint8_t _Keyslot_MapKeyMode( BHSM_KeySlotKeyMode keyMode );
#if !BDBG_NO_MSG
static char* _KeyslotOwnerToString( BHSM_SecurityCpuContext owner );
#endif
#ifdef BHSM_BUILD_HSM_FOR_SAGE
static BERR_Code _Keyslot_LoadConfigurationFromBsp( BHSM_Handle hHsm );
#else
static BERR_Code _Keyslot_Init( BHSM_Handle hHsm, BHSM_KeyslotModuleSettings *pSettings );
#endif


BERR_Code BHSM_Keyslot_Init( BHSM_Handle hHsm, BHSM_KeyslotModuleSettings *pSettings )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_KeySlotModule* pModule;
    unsigned i;
   #ifdef BHSM_BUILD_HSM_FOR_HOST
    unsigned count = 0;
   #else
    BSTD_UNUSED(pSettings);
   #endif
    BDBG_ENTER( BHSM_Keyslot_Init );

    if( !hHsm ) { return BERR_TRACE( BERR_NOT_AVAILABLE ); }
    if( !pSettings ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( hHsm->modules.pKeyslots ) { return BERR_TRACE( BERR_NOT_AVAILABLE ); } /* keyslot module already initialised. */

    pModule = (BHSM_KeySlotModule*)BKNI_Malloc( sizeof(BHSM_KeySlotModule) );
    if( !pModule ) { return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY ); }

    BKNI_Memset( pModule, 0, sizeof(BHSM_KeySlotModule) );

    pModule->hHsm = hHsm;
    hHsm->modules.pKeyslots = pModule;

   #ifdef BHSM_BUILD_HSM_FOR_HOST
    /* reserve keyslot for bypass. */
    pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerSlot] += BHSM_NUM_BYPASS_KEYSLOTS;

    rc =  _Keyslot_Init( hHsm, pSettings );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto error; }

    for( i = 0; i < BHSM_KeyslotType_eMax; i++ ){
        pModule->types[i].offset = count;
        pModule->types[i].maxNumber = pSettings->numKeySlotsForType[i];
        count += pSettings->numKeySlotsForType[i];
    }
   #else
    rc =  _Keyslot_LoadConfigurationFromBsp( hHsm );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto error; }
   #endif

    /* initialise the external key table */
    for( i = 0; i < BHSM_EXTERNAL_KEYSLOTS_MAX; i++ )
    {
        pModule->externalKeySlots[i].allocated = false;
        pModule->externalKeySlots[i].slotPtr   = (i * BHSM_EXTERNAL_KEYSLOT_SLOT_SIZE);
        pModule->externalKeySlots[i].offsetIv  = 0;
        pModule->externalKeySlots[i].offsetKey = BHSM_EXTERNAL_KEYSLOT_KEY_SIZE;
    }

   #ifdef BHSM_BUILD_HSM_FOR_HOST
    rc = BHSM_InitialiseBypassKeyslots( hHsm );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto error; }
   #endif

    BDBG_LEAVE( BHSM_Keyslot_Init );
    return BERR_SUCCESS;
error:

    if( hHsm->modules.pKeyslots ) BKNI_Free( hHsm->modules.pKeyslots );
    hHsm->modules.pKeyslots = NULL;

    BDBG_LEAVE( BHSM_Keyslot_Init );
    return rc;
}


void BHSM_Keyslot_Uninit( BHSM_Handle hHsm )
{
    BDBG_ENTER( BHSM_Keyslot_Uninit );

    if( !hHsm ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }
    if( !hHsm->modules.pKeyslots ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    if( hHsm->modules.pKeyslots->hBypassKeyslotG2GR ){
        BHSM_Keyslot_Free( hHsm->modules.pKeyslots->hBypassKeyslotG2GR );
        hHsm->modules.pKeyslots->hBypassKeyslotG2GR = NULL;
    }

    if( hHsm->modules.pKeyslots->hBypassKeyslotGR2R ){
        BHSM_Keyslot_Free( hHsm->modules.pKeyslots->hBypassKeyslotGR2R );
        hHsm->modules.pKeyslots->hBypassKeyslotGR2R = NULL;
    }

    if( hHsm->modules.pKeyslots->hBypassKeyslotGT2T ){
        BHSM_Keyslot_Free( hHsm->modules.pKeyslots->hBypassKeyslotGT2T );
        hHsm->modules.pKeyslots->hBypassKeyslotGT2T = NULL;
    }

    BKNI_Free( hHsm->modules.pKeyslots );
    hHsm->modules.pKeyslots = NULL;

    BDBG_LEAVE( BHSM_Keyslot_Uninit );
    return;
}

BERR_Code BHSM_P_KeyslotModule_GetCapabilities( BHSM_Handle hHsm, BHSM_KeyslotModuleCapabilities *pCaps )
{
    BHSM_KeySlotModule* pModule;
    unsigned i;

    BDBG_ENTER( BHSM_P_KeyslotModule_GetCapabilities );

    if( !hHsm ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pCaps ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pModule = hHsm->modules.pKeyslots;

    if( !pModule ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset(pCaps, 0, sizeof(*pCaps));

    for( i = 0; i < BHSM_KeyslotType_eMax; i++ )
    {
        pCaps->numKeySlotsForType[i] = pModule->types[i].maxNumber;
    }

    if( pCaps->numKeySlotsForType[BHSM_KeyslotType_eIvPerSlot] >= BHSM_NUM_BYPASS_KEYSLOTS ) {
        pCaps->numKeySlotsForType[BHSM_KeyslotType_eIvPerSlot] -= BHSM_NUM_BYPASS_KEYSLOTS;
    }
    else{
        BERR_TRACE( BERR_UNKNOWN ); /*flag and error. */
    }

    BDBG_LEAVE( BHSM_P_KeyslotModule_GetCapabilities );

    return BERR_SUCCESS;
}

void BHSM_Keyslot_GetDefaultAllocateSettings( BHSM_KeyslotAllocateSettings *pSettings )
{
    BDBG_ENTER( BHSM_Keyslot_GetDefaultAllocateSettings );

    if( !pSettings ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    BKNI_Memset( pSettings, 0, sizeof(*pSettings) );

    BDBG_LEAVE( BHSM_Keyslot_GetDefaultAllocateSettings );
    return;
}

BHSM_KeyslotHandle BHSM_Keyslot_Allocate( BHSM_Handle hHsm, const BHSM_KeyslotAllocateSettings *pSettings )
{
    BHSM_KeySlotModule* pModule = NULL;
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = NULL;
    unsigned offset; /* offset to keyslot type */
    unsigned maxNumber;
   #ifdef BHSM_BUILD_HSM_FOR_HOST
    unsigned i;
   #endif
    BDBG_ENTER( BHSM_Keyslot_Allocate );

    if( !hHsm ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( !hHsm->modules.pKeyslots ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( !pSettings ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( pSettings->slotType >= BHSM_KeyslotType_eMax ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( (pSettings->owner != BHSM_SecurityCpuContext_eHost) &&
        (pSettings->owner != BHSM_SecurityCpuContext_eSage) ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }

    pModule = hHsm->modules.pKeyslots;

    /* find a free slot. */
    offset    = pModule->types[pSettings->slotType].offset;
    maxNumber = pModule->types[pSettings->slotType].maxNumber;

    if( offset >= BHSM_MAX_KEYSLOTS ) { BERR_TRACE( BERR_INVALID_PARAMETER ); goto error; }
    if( maxNumber >= BHSM_MAX_KEYSLOTS ) { BERR_TRACE( BERR_INVALID_PARAMETER ); goto error; }
    if( offset+maxNumber >= BHSM_MAX_KEYSLOTS ) { BERR_TRACE( BERR_INVALID_PARAMETER ); goto error; }

   #ifdef BHSM_BUILD_HSM_FOR_HOST
    /* search for a free slot.*/
    for( i = offset; (i < offset+maxNumber) && (i < BHSM_MAX_KEYSLOTS); i++ )
    {
        if( pModule->pKeySlotHandles[i] == NULL )
        {
            pSlot = (BHSM_P_KeySlot*) BKNI_Malloc( sizeof(BHSM_P_KeySlot) );
            if ( !pSlot ) { BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY ); goto error; }

            BKNI_Memset( pSlot, 0, sizeof(*pSlot) );
            pSlot->slotType = pSettings->slotType;
            pSlot->number = i-offset;
            pSlot->pModule = pModule;

            if( pSettings->owner == BHSM_SecurityCpuContext_eHost )
            {
                rc = _SetKeyslotOwnership( (BHSM_KeyslotHandle)pSlot, pSettings->owner );
                if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto error; }

                rc = _InvalidateSlot( (BHSM_KeyslotHandle)pSlot, BHSM_PRESERVE_OWNERSHIP );
                if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto  error;}
            }

            pModule->pKeySlotHandles[i] = pSlot;

            break;
        }
    }
    if( !pSlot ){
        BDBG_ERR(("Failed to allocate keyslot. Type[%u] Max[%u]", pSettings->slotType, maxNumber ));
        BERR_TRACE( BERR_NOT_AVAILABLE );
        goto error;
    }

   #else /* we're on SAGE. The exact keyslot type/number is specified.*/
    if( pSettings->keySlotNumber >= maxNumber ){
        BERR_TRACE( BERR_INVALID_PARAMETER );
        goto error; /* slot number greater that number reserved for that type. */
    }

    if( pModule->pKeySlotHandles[offset + pSettings->keySlotNumber] != NULL ){
        BERR_TRACE( BERR_INVALID_PARAMETER );
        goto error;  /* specified slot (type,number) is not available. */
    }

    BDBG_MSG(("slot being acquired by requested ownership: %s", _KeyslotOwnerToString(pSettings->owner) ));
    pSlot = BKNI_Malloc( sizeof(BHSM_P_KeySlot) );
    if( !pSlot ) { BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY ); goto error; }

    BKNI_Memset( pSlot, 0, sizeof(*pSlot) );
    pSlot->slotType = pSettings->slotType;
    pSlot->number = pSettings->keySlotNumber;
    pSlot->pModule = pModule;

    rc = _SetKeyslotOwnership( (BHSM_KeyslotHandle)pSlot, pSettings->owner );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto  error;}

    pModule->pKeySlotHandles[offset + pSettings->keySlotNumber] = pSlot;

    rc = _InvalidateSlot( (BHSM_KeyslotHandle)pSlot, BHSM_PRESERVE_OWNERSHIP );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto  error;}
   #endif

   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    rc = BHSM_Keyslot_InitExtensionRestricted( (BHSM_KeyslotHandle)pSlot );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); goto  error;}
   #endif

    BDBG_LEAVE( BHSM_Keyslot_Allocate );
    return (BHSM_KeyslotHandle)pSlot;

 error:
    if( pSlot ){ BKNI_Free( pSlot ); }

    BDBG_LEAVE( BHSM_Keyslot_Allocate );
    return NULL;

}

void BHSM_Keyslot_Free( BHSM_KeyslotHandle handle )
{
    BERR_Code rc = BERR_SUCCESS;
    unsigned offset;
    unsigned i;
    BHSM_P_KeySlot *pSlot;

    BDBG_ENTER( BHSM_Keyslot_Free );

    if( !handle ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    pSlot = (BHSM_P_KeySlot*)handle;

    if( pSlot->slotType >= BHSM_KeyslotType_eMax ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    rc = _InvalidateSlot( handle, !BHSM_PRESERVE_OWNERSHIP );
    if( rc != BERR_SUCCESS ) { BERR_TRACE(rc); } /* continue. Best effort. */

    /* free external keyslots. */
    for( i = 0; i  < BHSM_KeyslotBlockEntry_eMax; i++ )
    {
        BHSM_P_KeyEntry *pEntry = &pSlot->entry[i];

        if( pEntry->configured && pEntry->pExternalSlot )
        {
            pEntry->pExternalSlot->allocated = false;
        }
    }

    offset = pSlot->pModule->types[pSlot->slotType].offset + pSlot->number;

    if( offset < BHSM_MAX_KEYSLOTS ){
        pSlot->pModule->pKeySlotHandles[offset] = NULL;
    }
    else{ BERR_TRACE(rc); }

   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    BHSM_Keyslot_UninitExtensionRestricted( handle );
   #endif

    BKNI_Free( pSlot );

    BDBG_LEAVE( BHSM_Keyslot_Free );
    return;
}

void  BHSM_Keyslot_GetSettings( BHSM_KeyslotHandle handle, BHSM_KeyslotSettings *pSettings )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_Keyslot_GetSettings );

    if( !pSettings ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    *pSettings = pSlot->settings;

    BDBG_LEAVE( BHSM_Keyslot_GetSettings );
    return;
}

BERR_Code BHSM_Keyslot_SetSettings( BHSM_KeyslotHandle handle, const BHSM_KeyslotSettings *pSettings )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_Keyslot_SetSettings );

    if( !handle ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pSettings ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pSlot->settings = *pSettings;
    pSlot->configured = true;

    BDBG_LEAVE( BHSM_Keyslot_SetSettings );
    return BERR_SUCCESS;
}

void BHSM_Keyslot_GetEntrySettings( BHSM_KeyslotHandle handle,
                                    BHSM_KeyslotBlockEntry entry,
                                    BHSM_KeyslotEntrySettings *pSettings )
{
    BHSM_P_KeyEntry *pEntry;

    BDBG_ENTER( BHSM_Keyslot_GetEntrySettings );

    if( !handle ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }
    if( !pSettings ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return; }

    pEntry = _GetEntry( handle, entry );
    if( !pEntry ) { BERR_TRACE( BERR_INVALID_PARAMETER );  return; }

    *pSettings = pEntry->settings;

    BDBG_LEAVE( BHSM_Keyslot_GetEntrySettings );
    return;
}


BERR_Code BHSM_Keyslot_SetEntrySettings( BHSM_KeyslotHandle handle,
                                         BHSM_KeyslotBlockEntry entry,
                                         const BHSM_KeyslotEntrySettings *pSettings )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_KeySlotModule* pModule;
    BHSM_P_KeyEntry *pEntry;
    unsigned i;
    BERR_Code rc = BERR_SUCCESS;

    BDBG_ENTER( BHSM_Keyslot_SetEntrySettings );

    if( !handle )                              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pSettings )                           { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pModule = pSlot->pModule;
    pEntry = _GetEntry( handle, entry );
    if( pEntry == NULL ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    /* check if external keyslot is available. */
    if( pSettings->external.key || pSettings->external.iv )
    {
        if( !pEntry->pExternalSlot )
        {
            for( i = 0; i < BHSM_EXTERNAL_KEYSLOTS_MAX; i++ )
            {
                if( pModule->externalKeySlots[i].allocated == false )
                {
                    pModule->externalKeySlots[i].allocated = true;
                    pEntry->pExternalSlot = &pModule->externalKeySlots[i];
                    break;
                }
            }
            if( !pEntry->pExternalSlot ) /* failed to get a slot. */
            {
                return BERR_TRACE( BERR_NOT_AVAILABLE );
            }
        }
    }
    else
    {
        if( pEntry->pExternalSlot )
        {
            pEntry->pExternalSlot->allocated = false;
            pEntry->pExternalSlot = NULL;
        }
    }

    pEntry->settings = *pSettings;
    pEntry->configured = true;

    /* update BSP with KeySlot entry Configuration. */
    if( pEntry->pExternalSlot )
    {
        rc = _SetEntryKey( handle, entry, NULL );
        if( rc != BERR_SUCCESS ) return BERR_TRACE(rc);
    }

    BDBG_LEAVE( BHSM_Keyslot_SetEntrySettings );
    return BERR_SUCCESS;
}


BERR_Code BHSM_Keyslot_SetEntryKey ( BHSM_KeyslotHandle handle,
                                     BHSM_KeyslotBlockEntry entry,
                                     const BHSM_KeyslotKey *pKey )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_Keyslot_SetEntryKey );

    if( !handle )                              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pKey )                                { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    if( pSlot->configured == false )              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pSlot->entry[entry].configured == false ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    rc = _SetEntryKey( handle, entry, pKey );
    if( rc != BERR_SUCCESS ) return BERR_TRACE(rc);

    BDBG_LEAVE( BHSM_Keyslot_SetEntryKey );
    return BERR_SUCCESS;
}



BERR_Code BHSM_Keyslot_SetEntryIv( BHSM_KeyslotHandle handle,
                                    BHSM_KeyslotBlockEntry entry,
                                    const BHSM_KeyslotIv *pIv,
                                    const BHSM_KeyslotIv *pIv2 )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_Keyslot_SetEntryIv );

    if( !handle )                              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pIv && !pIv2 )                        { return BERR_TRACE( BERR_INVALID_PARAMETER ); } /* we need one IV */
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    if( pSlot->configured == false )              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pSlot->entry[entry].configured == false ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    rc = _SetEntryIv( handle, entry, pIv, pIv2 );
    if( rc != BERR_SUCCESS ) return BERR_TRACE(rc);

    BDBG_LEAVE( BHSM_Keyslot_SetEntryIv );
    return BERR_SUCCESS;
}


BERR_Code BHSM_Keyslot_Invalidate( BHSM_KeyslotHandle handle )
{
    BERR_Code rc = BERR_SUCCESS;
    BDBG_ENTER( BHSM_Keyslot_Invalidate );

    if( !handle ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    rc = _InvalidateSlot( handle, BHSM_PRESERVE_OWNERSHIP );
    if( rc != BERR_SUCCESS ) return BERR_TRACE(rc);

    BDBG_LEAVE( BHSM_Keyslot_Invalidate );
    return BERR_SUCCESS;
}


BERR_Code BHSM_Keyslot_GetEntryExternalKeySettings( BHSM_KeyslotHandle handle,
                                                    BHSM_KeyslotBlockEntry entry,
                                                    BHSM_KeyslotExternalKeyData *pData )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeyEntry *pEntry;

    BDBG_ENTER( BHSM_Keyslot_GetEntryExternalKeySettings );

    if( !handle )                              { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pData )                               { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pSlot->configured == false )           { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pEntry = _GetEntry( handle, entry );
    if( !pEntry ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pEntry->configured == false ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pEntry->pExternalSlot == false ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( pData, 0, sizeof(*pData) );

    pData->slotIndex = pEntry->pExternalSlot->slotPtr;

    if( pEntry->settings.external.key ) {
        pData->key.valid = true;
        pData->key.offset = pEntry->pExternalSlot->offsetKey;
    }
    if( pEntry->settings.external.iv ) {
        pData->iv.valid = true;
        pData->iv.offset = pEntry->pExternalSlot->offsetIv;
    }

    BDBG_LEAVE( BHSM_Keyslot_GetEntryExternalKeySettings );
    return BERR_SUCCESS;
}


BERR_Code BHSM_Keyslot_InvalidateEntry( BHSM_KeyslotHandle handle,
                                         BHSM_KeyslotBlockEntry entry )
{
    BERR_Code rc = BERR_SUCCESS;
    BDBG_ENTER( BHSM_Keyslot_InvalidateEntry );

    if( !handle ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    rc = _InvalidateEntry( handle, entry );
    if( rc != BERR_SUCCESS ) return BERR_TRACE(rc);

    BDBG_LEAVE( BHSM_Keyslot_InvalidateEntry );
    return BERR_SUCCESS;
}



BERR_Code BHSM_Keyslot_AddPidChannel_WithSettings( BHSM_KeyslotHandle handle,
                                                   unsigned pidChannelIndex,
                                                   const BHSM_KeyslotAddPidChannelSettings *pSettings )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlotPidAdd hsmAddPid;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BSTD_UNUSED( pSettings );

    BDBG_ENTER( BHSM_Keyslot_AddPidChannel );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( &hsmAddPid, 0, sizeof(hsmAddPid) );

    hsmAddPid.in.pidChanStart  = pidChannelIndex;
    hsmAddPid.in.keySlotType   = BHSM_P_ConvertSlotType( pSlot->slotType );
    hsmAddPid.in.keySlotNumber = pSlot->number;

    #if 0
    hsmAddPid.keySlotTypeB; hsmAddPid.keySlotNumberB; hsmAddPid.setMultiplePidChan;
    hsmAddPid.spidUsePointerB; hsmAddPid.estinationPipeSel;
    #endif

    rc = BHSM_P_KeySlot_PidAdd( _Keyslot_GetHsmHandle(handle), &hsmAddPid );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    BDBG_LEAVE( BHSM_Keyslot_AddPidChannel );
    return BERR_SUCCESS;
}

BERR_Code BHSM_Keyslot_AddPidChannel( BHSM_KeyslotHandle handle, unsigned pidChannelIndex )
{
    BERR_Code rc = BERR_SUCCESS;

    rc = BHSM_Keyslot_AddPidChannel_WithSettings( handle, pidChannelIndex, NULL );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    return rc;
}

BERR_Code BHSM_Keyslot_RemovePidChannel( BHSM_KeyslotHandle handle,
                                          unsigned pidChannelIndex )
{
    BERR_Code           rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeySlotPidRemove bspRemovePid;

    BDBG_ENTER( BHSM_Keyslot_RemovePidChannel );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( &bspRemovePid, 0, sizeof(bspRemovePid) );
    bspRemovePid.in.pidChanStart = pidChannelIndex;
    bspRemovePid.in.pidChanEnd = pidChannelIndex;

    rc = BHSM_P_KeySlot_PidRemove( _Keyslot_GetHsmHandle(handle), &bspRemovePid );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    BDBG_LEAVE( BHSM_Keyslot_RemovePidChannel );
    return BERR_SUCCESS;
}


BERR_Code BHSM_GetKeySlotInfo( BHSM_KeyslotHandle handle, BHSM_KeyslotInfo *pInfo )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_GetKeySlotInfo );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pInfo ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pInfo->type   = pSlot->slotType;
    pInfo->number = pSlot->number;

    BDBG_LEAVE( BHSM_GetKeySlotInfo );

    return BERR_SUCCESS;
}

static BERR_Code _SetEntryIv( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry, const BHSM_KeyslotIv *pIv, const BHSM_KeyslotIv *pIv2)
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeyEntry *pEntry;
    BHSM_P_KeySlotClearIvSet bspSetIv;

    BDBG_ENTER( _SetEntryIv );

    pEntry = _GetEntry( handle, entry );
    if( pEntry == NULL ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    if( !pIv && !pIv2 ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( (pIv && pIv2) && (pIv->size != pIv2->size) ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pIv && (pIv->size != 8) && (pIv->size != 16) ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( pIv2 && (pIv2->size != 8) && (pIv2->size != 16) ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( &bspSetIv, 0, sizeof(bspSetIv) );

    bspSetIv.in.blockType = _GetEntryBlockType(entry);
    bspSetIv.in.entryType = _GetEntryPolarity(entry);
    bspSetIv.in.keySlotType = BHSM_P_ConvertSlotType(pSlot->slotType);
    bspSetIv.in.keySlotNumber = pSlot->number;

    if( pIv && (pIv->size == 8) ) /* handle 64 bit IVs */
    {
        rc = BHSM_MemcpySwap( &bspSetIv.in.iv[2], pIv->iv, pIv->size );
        if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }

        if( pIv2 ){
            rc = BHSM_MemcpySwap( &bspSetIv.in.iv[0], pIv2->iv, pIv2->size );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }
        }

        bspSetIv.in.ivType = Bsp_KeySlotIvType_eIv;

        rc = BHSM_P_KeySlot_ClearIvSet( _Keyslot_GetHsmHandle(handle), &bspSetIv );
        if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }
    }
    else{ /* Send separately */

        if( pIv )
        {
            rc = BHSM_MemcpySwap( &bspSetIv.in.iv[0], pIv->iv, pIv->size );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }

            bspSetIv.in.ivType = Bsp_KeySlotIvType_eIv;

            rc = BHSM_P_KeySlot_ClearIvSet( _Keyslot_GetHsmHandle(handle), &bspSetIv );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }
        }

        if( pIv2 )
        {

            if( pIv2->size == 8 ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); } /* 8 bit IV2 must be issued
                                                                                      with a 8 bit IV */
            rc = BHSM_MemcpySwap( &bspSetIv.in.iv[0], pIv2->iv, pIv2->size ) ;
            if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }

            bspSetIv.in.ivType = Bsp_KeySlotIvType_eAesShortIv;

            rc = BHSM_P_KeySlot_ClearIvSet( _Keyslot_GetHsmHandle(handle), &bspSetIv );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }
        }
    }

    BDBG_LEAVE( _SetEntryIv );
    return BERR_SUCCESS;
}

static BERR_Code _SetEntryKey( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry, const BHSM_KeyslotKey *pKey )
{
    BERR_Code rc = BERR_UNKNOWN;
    BHSM_P_KeySlotClearKeySet bspSetKey;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeyEntry *pEntry;
    Bsp_KeySize_e keySize;
    uint8_t *pSecureKeyMemory = NULL;
    uint8_t secureKeyMemorySize = 0;

    BDBG_ENTER( _SetEntryKey );

    pEntry = _GetEntry( handle, entry );
    if( !pEntry ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    secureKeyMemorySize = sizeof( bspSetKey.in.keyData );
    pSecureKeyMemory = BHSM_SecureMemory_Allocate( secureKeyMemorySize );
    if( !pSecureKeyMemory ) { rc = BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY ); goto exit; }

    BKNI_Memset( &bspSetKey, 0, sizeof(bspSetKey) );

    /* For XPT packet head SC value 'reserved' which is not currently handled by APIs. */
    bspSetKey.in.gPipeSc01EntryType = 0x01;
    bspSetKey.in.rPipeSc01EntryType = 0x01;

    if( pKey ) {
        unsigned keyOffset = 0;

        switch( pKey->size ) {
            case 8:  { keySize = Bsp_KeySize_e64; break;}
            case 16: { keySize = Bsp_KeySize_e128; break;}
            case 24: { keySize = Bsp_KeySize_e192; break;}
            case 32: { keySize = Bsp_KeySize_e256; break;}
            default: { rc = BERR_TRACE( BERR_INVALID_PARAMETER ); goto exit; }
        }
        keyOffset = (8-(2*(keySize+1)))*4; /* offset of key in bytes */

        if( keyOffset >= secureKeyMemorySize ||
            keyOffset + pKey->size > secureKeyMemorySize ) {
            rc = BERR_TRACE( BERR_INVALID_PARAMETER );
            goto exit;
        }
        BKNI_Memcpy( pSecureKeyMemory + keyOffset, pKey->key, pKey->size );

        if( keySize  == Bsp_KeySize_e64 ) { /* duplicate the key. */
            keyOffset -= pKey->size;
            if( keyOffset >= secureKeyMemorySize ||
                keyOffset + pKey->size > secureKeyMemorySize ) {
                rc = BERR_TRACE( BERR_INVALID_PARAMETER );
                goto exit;
            }
            BKNI_Memcpy( pSecureKeyMemory + keyOffset, pKey->key, pKey->size ) ;
        }
    }
    else {
        unsigned i;
        /* dummy key data. All zero are not be allowed for some configurations.  */
        for( i = 0; i < secureKeyMemorySize; i++ ) {
            *(pSecureKeyMemory + i) = i & 0xFF;
        }
    }
    bspSetKey.in.blockType = _GetEntryBlockType(entry);
    bspSetKey.in.entryType = _GetEntryPolarity(entry);
    bspSetKey.in.keySlotType = BHSM_P_ConvertSlotType(pSlot->slotType);
    bspSetKey.in.keySlotNumber = pSlot->number;
    bspSetKey.in.keyMode = _Keyslot_MapKeyMode( pEntry->settings.keyMode );

    bspSetKey.in.modeWords[0] = compileControl0_GlobalHi( handle );
    bspSetKey.in.modeWords[1] = compileControl1_GlobalLo( handle );
    bspSetKey.in.modeWords[2] = compileControl2_ModeHi( handle, entry );
    bspSetKey.in.modeWords[3] = compileControl3_ModeLo( handle, entry );

    if( pEntry->pExternalSlot ) {
        if( pEntry->settings.external.key ) {
            bspSetKey.in.extKeyPtr = pEntry->pExternalSlot->offsetKey + pEntry->pExternalSlot->slotPtr;
        }
        if( pEntry->settings.external.iv )  {
            bspSetKey.in.extIvPtr = pEntry->pExternalSlot->offsetIv + pEntry->pExternalSlot->slotPtr;
        }
    }

    bspSetKey.in.pKeyData_inPlace = pSecureKeyMemory;
    bspSetKey.in.keyDataSize_inPlace = secureKeyMemorySize;

    rc = BHSM_P_KeySlot_ClearKeySet( _Keyslot_GetHsmHandle(handle), &bspSetKey );
    if( rc != BERR_SUCCESS ) {
        BDBG_ERR(("ClearKeySet failed for Slot Type[%u] Num[%u]", bspSetKey.in.keySlotType, bspSetKey.in.keySlotNumber ));
        rc = BERR_TRACE( rc );
        goto exit;
    }

exit:

    if( pSecureKeyMemory ) {
        BHSM_SecureMemory_ScrubBeforeFree( pSecureKeyMemory, secureKeyMemorySize );
        BHSM_SecureMemory_Free( pSecureKeyMemory );
    }

    BDBG_LEAVE( _SetEntryKey );
    return rc;
}

static BERR_Code _InvalidateSlot( BHSM_KeyslotHandle handle, bool preserveOwner )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeySlotInvalidate bspInvalidate;
    BHSM_SecurityCpuContext owner = BHSM_SecurityCpuContext_eNone;

    BDBG_ENTER( _InvalidateSlot );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    rc = _GetKeyslotOwnership( handle, &owner );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }

   #ifdef BHSM_BUILD_HSM_FOR_HOST
    if( owner == BHSM_SecurityCpuContext_eSage ) {
        rc = BERR_TRACE(BERR_NOT_AVAILABLE);       /* trying to invalidate a SAGE Keyslot from HOST.  */
        goto BHSM_P_DONE_LABEL;
    }
   #endif

    BKNI_Memset( &bspInvalidate, 0, sizeof(bspInvalidate) );
    bspInvalidate.in.invalidateMethod  = 1 /*Bsp_KeySlotInvalidateOperation_eInvalidateAll*/;
    bspInvalidate.in.keySlotType       = BHSM_P_ConvertSlotType( pSlot->slotType );
    bspInvalidate.in.keySlotNumber     = pSlot->number;

    rc = BHSM_P_KeySlot_Invalidate( _Keyslot_GetHsmHandle(handle), &bspInvalidate );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }

    if( preserveOwner && (owner != BHSM_SecurityCpuContext_eNone) )
    {
        rc = _SetKeyslotOwnership( handle, owner );
        if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto BHSM_P_DONE_LABEL; }
    }

BHSM_P_DONE_LABEL:

    BDBG_LEAVE( _InvalidateSlot );
    return BERR_SUCCESS;
}

static BERR_Code _InvalidateEntry( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    uint8_t block = 0;
    uint8_t polarity = 0;
    BHSM_P_KeySlotInvalidate bspInvalidate;

    BDBG_ENTER( _InvalidateEntry );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    switch(_GetEntryBlockType(entry))
    {
        case BHSM_KeyslotBlockType_eCpd: {block = 0; break;}
        case BHSM_KeyslotBlockType_eCa:  {block = 1; break;}
        case BHSM_KeyslotBlockType_eCps: {block = 2; break;}
        default: return BERR_TRACE( BERR_INVALID_PARAMETER );
    }
    switch(_GetEntryPolarity(entry))
    {
        case BHSM_KeyslotPolarity_eOdd:   {polarity = 0; break;}
        case BHSM_KeyslotPolarity_eEven:  {polarity = 1; break;}
        case BHSM_KeyslotPolarity_eClear: {polarity = 2; break;}
        default: return BERR_TRACE( BERR_INVALID_PARAMETER );
    }

    BKNI_Memset( &bspInvalidate, 0, sizeof(bspInvalidate) );
    bspInvalidate.in.blockType         = block;
    bspInvalidate.in.entryType         = polarity;
    bspInvalidate.in.keySlotType       = BHSM_P_ConvertSlotType( pSlot->slotType );
    bspInvalidate.in.keySlotNumber     = pSlot->number;
    /*bspInvalidate.sc01ModeWordMapping*/
    bspInvalidate.in.invalidateMethod  = 0 /*Bsp_KeySlotInvalidateOperation_eInvalidateOneEntry */;

    rc = BHSM_P_KeySlot_Invalidate( _Keyslot_GetHsmHandle(handle), &bspInvalidate );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    BDBG_LEAVE( _InvalidateEntry );
    return BERR_SUCCESS;
}

static uint32_t compileControl0_GlobalHi( BHSM_KeyslotHandle handle )
{

   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    return  BHSM_P_CompileControl0GlobalHiReserved( handle );
   #else
    BSTD_UNUSED( handle );
    return 0;
   #endif
}

static uint32_t compileControl1_GlobalLo( BHSM_KeyslotHandle handle )
{
    BHSM_KeyslotSettings *pSlotSettings;
    uint32_t control = 0;
    unsigned x;

    pSlotSettings = _GetSlotSetting( handle );
    if( pSlotSettings == NULL ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return control; }

    /* pack source regions */
    for( x = 0; x < BHSM_SecurityRegion_eMax; x++ ) {
        if( pSlotSettings->regions.source[x] ) {
            control |= 1 << (0+x);
        }
    }
    /* pack RPipe regions */
    for( x = 0; x < BHSM_SecurityRegion_eMax; x++ ) {
        if( pSlotSettings->regions.destinationRPipe[x] ) {
            control |= 1 << (8+x);
        }
    }
    /* pack GPipe regions */
    for( x = 0; x < BHSM_SecurityRegion_eMax; x++ ) {
        if( pSlotSettings->regions.destinationGPipe[x] ) {
            control |= 1 << (16+x);
        }
    }
    /* pack encrypt before rave. */
    if( pSlotSettings->encryptBeforeRave ) {
        control |= 1 << 24;
    }

    return control;
}

static uint32_t compileControl2_ModeHi( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry )
{
    BHSM_P_KeyEntry *pEntry;
    uint32_t control = 0;
    unsigned polarity = 0;

    pEntry = _GetEntry( handle, entry );
    if( pEntry == NULL ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return control; }

    /* pack external Key allowed */
    if( pEntry->settings.external.key ) { control |= 1; }         /* 1 bit. b00 */

    /* pack external IV allowed */
    if( pEntry->settings.external.iv ) { control |= (1 << 1); }  /* 1 bit. b01 */

    /* pack RPIPE_EN */
    if( pEntry->settings.rPipeEnable ) { control |= (1 << 2); }  /* 1 bit. b02 */

    /* pack GPIPE_EN */
    if( pEntry->settings.gPipeEnable ) { control |= (1 << 3); }  /* 1 bit. b03 */

    if( pEntry->settings.outputPolarity.specify ) {

       /* pack RPIPE_SC_VALUE */
       switch( pEntry->settings.outputPolarity.rPipe )
       {
            case BHSM_KeyslotPolarity_eClear:    { polarity = 0x0; break;}
            case BHSM_KeyslotPolarity_eReserved: { polarity = 0x1; break;}
            case BHSM_KeyslotPolarity_eEven:     { polarity = 0x2; break;}
            case BHSM_KeyslotPolarity_eOdd:      { polarity = 0x3; break;}
            default:{ polarity = 0x0;  BERR_TRACE( pEntry->settings.outputPolarity.rPipe ); } /* unsupported polarity. */
       }
       control |= ( (polarity & 0x3) << 4);         /* 2 bit. b04-b05 */

        /* pack GPIPE_SC_VALUE */
        switch( pEntry->settings.outputPolarity.gPipe )
        {
            case BHSM_KeyslotPolarity_eClear:    { polarity = 0x0; break;}
            case BHSM_KeyslotPolarity_eReserved: { polarity = 0x1; break;}
            case BHSM_KeyslotPolarity_eEven:     { polarity = 0x2; break;}
            case BHSM_KeyslotPolarity_eOdd:      { polarity = 0x3; break;}
            default:{ polarity = 0x0; BERR_TRACE( pEntry->settings.outputPolarity.gPipe ); } /* unsupported polarity. */
        }
        control |= ( (polarity & 0x3) << 6);         /* 2 bit. b06-b07 */
    }

    /* Multi2 */
    if( pEntry->settings.algorithm == BHSM_CryptographicAlgorithm_eMulti2 ) {
        control |= ( ( pEntry->settings.multi2.roundCount & 0x7)  << 21 );       /* 3 bit. b21-23 */
        control |= ( ( pEntry->settings.multi2.keySelect  & 0x3F) << 15 );       /* 6 bit. b15-20 */
    }

    return control;
}

static uint32_t compileControl3_ModeLo( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry )
{
    uint32_t control = 0;
    unsigned algorithm = 0;
    unsigned cipherMode = 0;
    unsigned terminationMode = 0;
    unsigned counterSize = 0;
    unsigned ivMode = 0;
    BHSM_P_KeyEntry *pEntry;

    pEntry = _GetEntry( handle, entry );
    if( pEntry == NULL ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return control; }

    /* pack algorithm */
    algorithm = BHSM_P_Map2KeySlotCryptoAlg( pEntry->settings.algorithm );
    control |= (algorithm & 0x1f); /* 5 bits. b00-b04 */

    if( pEntry->settings.algorithm != BHSM_CryptographicAlgorithm_eDvbCsa2 &&
        pEntry->settings.algorithm != BHSM_CryptographicAlgorithm_eDvbCsa3 ) {
        /* pack cipher mode */
        switch( pEntry->settings.algorithmMode )
        {
            case BHSM_CryptographicAlgorithmMode_eEcb:     { cipherMode = 0x0; break; }
            case BHSM_CryptographicAlgorithmMode_eCbc:     { cipherMode = 0x1; break; }
            case BHSM_CryptographicAlgorithmMode_eCounter: { cipherMode = 0x2; break; }
            case BHSM_CryptographicAlgorithmMode_eRcbc:    { cipherMode = 0x3; break; }
            default:BERR_TRACE( BERR_INVALID_PARAMETER );   /* unsupported cipher mode. */
        }
        control |= ((cipherMode & 0x7) << 5); /* 3 bits. b05-b07 */
    }

    if( pEntry->settings.algorithmMode == BHSM_CryptographicAlgorithmMode_eCounter ) {
        /* pack counter mode */
        control |= ( (pEntry->settings.counterMode & 0xf) << 8);  /* 4 bits. b08-b11  */
    }
    else{
        /* pack termination mode. */
        switch( pEntry->settings.terminationMode )
        {
            case BHSM_Keyslot_TerminationMode_eClear:             { terminationMode = 0x0; break; }
            case BHSM_Keyslot_TerminationMode_eScte52:            { terminationMode = 0x1; break; }
            case BHSM_Keyslot_TerminationMode_eCtsEcb:            { terminationMode = 0x2; break; }
            case BHSM_KeySlot_TerminationMode_eCoronado:          { terminationMode = 0x3; break; }
            case BHSM_Keyslot_TerminationMode_eCtsDvbCpcm:        { terminationMode = 0x4; break; }
            case BHSM_Keyslot_TerminationMode_eFrontResidue:      { terminationMode = 0x5; break; }
            case BHSM_Keyslot_TerminationMode_eMsc:               { terminationMode = 0x6; break; }
            case BHSM_Keyslot_TerminationMode_eReserved0x06:      { terminationMode = 0x7; break; }
            case BHSM_Keyslot_TerminationMode_eTsAndPacket:       { terminationMode = 0x8; break; }
            case BHSM_Keyslot_TerminationMode_ePacket:            { terminationMode = 0x9; break; }
            case BHSM_Keyslot_TerminationMode_eCbcMac:            { terminationMode = 0xa; break; }
            case BHSM_Keyslot_TerminationMode_eCMac:              { terminationMode = 0xb; break; }
            default:BERR_TRACE( BERR_INVALID_PARAMETER );   /* unsupported cipher mode. */
        }
        control |= ((terminationMode & 0xf) << 8); /* 4 bits. b08-b11 */
    }

    if( pEntry->settings.algorithmMode == BHSM_CryptographicAlgorithmMode_eCounter ) {
        /* pack counter size */
        switch( pEntry->settings.counterSize )
        {
            case 32:  { counterSize = 0x0; break; }
            case 64:  { counterSize = 0x1; break; }
            case 96:  { counterSize = 0x2; break; }
            case 128: { counterSize = 0x3; break; }
            default:BERR_TRACE( BERR_INVALID_PARAMETER );   /* unsupported counter size. */
        }
        control |= ((counterSize & 0x3) << 12); /* 2 bits. b12-b13 */
    }
    else {
        /* Iv Mode */
        switch( pEntry->settings.ivMode )
        {
            case BHSM_Keyslot_IvMode_eNoPreProc:          { ivMode = 0x00; break; };
            case BHSM_Keyslot_IvMode_eMdi:                { ivMode = 0x01; break; };
            case BHSM_Keyslot_IvMode_eMdd:                { ivMode = 0x02; break; };
            case BHSM_Keyslot_IvMode_eNoPreProcWriteBack: { ivMode = 0x03; break; };
            default:BERR_TRACE( BERR_INVALID_PARAMETER );   /* unsupported iv mode. */
        }
        control |= ((ivMode & 0x3) << 12); /* 2 bits, b12-b13 */
    }

    /* solitary block processing */
    control |= ((pEntry->settings.solitaryMode & 0x7) << 14); /* 3 bits, b14-b16 */

    /* Key Offset */   /* TODO ... maybe not be required. */
    if( pEntry->settings.external.key && pEntry->pExternalSlot ) {
        control |= ((pEntry->pExternalSlot->offsetKey & 0x7F) << 17); /* 7 bits, b17-b23 */
    }

    /* Iv  Offset */ /* TODO ... maybe not be required. */
    if( pEntry->settings.external.iv && pEntry->pExternalSlot ) {
        control |= ((pEntry->pExternalSlot->offsetIv & 0x7F) << 24); /* 7 bits, b24-b30 */
    }

    return control;
}



static BHSM_KeyslotSettings* _GetSlotSetting( BHSM_KeyslotHandle handle )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    if( pSlot == NULL )              { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( pSlot->configured == false ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }

    return &(pSlot->settings);
}

static BHSM_P_KeyEntry* _GetEntry( BHSM_KeyslotHandle handle, BHSM_KeyslotBlockEntry entry )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    if( pSlot == NULL ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( entry >= BHSM_KeyslotBlockEntry_eMax ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }

    return &(pSlot->entry[entry]);
}

static BHSM_Handle _Keyslot_GetHsmHandle( BHSM_KeyslotHandle handle )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    return pSlot->pModule->hHsm;
}

#ifdef BHSM_BUILD_HSM_FOR_HOST
static BERR_Code _Keyslot_Init( BHSM_Handle hHsm, BHSM_KeyslotModuleSettings *pSettings )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlotInit hsmInit;

    BDBG_ENTER( _Keyslot_Init );

    if( pSettings == NULL ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( &hsmInit, 0, sizeof(hsmInit) );

    hsmInit.in.keySlotNumberIvPerSlot128  = pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerSlot];
    hsmInit.in.keySlotNumberIvPerBlock128 = pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerBlock];
    hsmInit.in.keySlotNumberIvPerBlock256 = pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerBlock256];
    hsmInit.in.keySlotNumberIvPerEntry256 = pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerEntry] +
                                            pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerEntry256];

    rc = BHSM_P_KeySlot_Init( hHsm, &hsmInit );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    if( hsmInit.out.keySlotNumberIvPerSlot128  != pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerSlot] ||
        hsmInit.out.keySlotNumberIvPerBlock128 != pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerBlock] ||
        hsmInit.out.keySlotNumberIvPerBlock256 != pSettings->numKeySlotsForType[BHSM_KeyslotType_eIvPerBlock256] )
    {
        return BERR_TRACE( BERR_NOT_AVAILABLE ); /* not able to provide the requested number of keyslots. */
    }

    BDBG_LOG(("KeySlot types: IvPerSlot128[%u+%u] IvPerBlock128[%u] IvPerBlock256[%u] IvPerEntry256[%u]"
                                                             , hsmInit.out.keySlotNumberIvPerSlot128-BHSM_NUM_BYPASS_KEYSLOTS
                                                             , BHSM_NUM_BYPASS_KEYSLOTS
                                                             , hsmInit.out.keySlotNumberIvPerBlock128
                                                             , hsmInit.out.keySlotNumberIvPerBlock256
                                                             , hsmInit.out.keySlotNumberIvPerEntry256 ));

    BDBG_LEAVE( _Keyslot_Init );
    return rc;
}
#else
/* function that configures loads HSM configuration from BSP  */
static BERR_Code _Keyslot_LoadConfigurationFromBsp( BHSM_Handle hHsm )
{
    BERR_Code           rc = BERR_SUCCESS;
    BHSM_KeySlotModule* pModule;
    BHSM_P_KeySlotQuery bspQuery;
    uint8_t             numSlots[BHSM_KeyslotType_eMax] =  {0};
    unsigned            count = 0;
    unsigned            i;

    BDBG_ENTER( _Keyslot_LoadConfigurationFromBsp );

    BKNI_Memset( &bspQuery, 0, sizeof(bspQuery) );

    rc = BHSM_P_KeySlot_Query( hHsm, &bspQuery );
    BDBG_LOG((" BHSM_P_KeySlot_Query BSP ERROR workaround in place"));
    if( rc != BERR_SUCCESS ) {
    /*    return BERR_TRACE( BERR_NOT_AVAILABLE );*/
    }

    BDBG_LOG(("KeySlot types: IvPerSlot128[%u+%u] IvPerBlock128[%u] IvPerBlock256[%u] IvPerEntry256[%u]"
                                                                , bspQuery.out.keySlotNumberIvPerSlot128-BHSM_NUM_BYPASS_KEYSLOTS
                                                                , BHSM_NUM_BYPASS_KEYSLOTS
                                                                , bspQuery.out.keySlotNumberIvPerBlock128
                                                                , bspQuery.out.keySlotNumberIvPerBlock256
                                                                , bspQuery.out.keySlotNumberIvPerEntry256 ));

    pModule = hHsm->modules.pKeyslots;
    for( i = 0; i < BHSM_KeyslotType_eMax; i++ ){
        switch(i)
        {
            case BHSM_KeyslotType_eIvPerSlot:
                numSlots[i] = bspQuery.out.keySlotNumberIvPerSlot128;
                break;
            case BHSM_KeyslotType_eIvPerBlock:
                numSlots[i] = bspQuery.out.keySlotNumberIvPerBlock128;
                break;
            case BHSM_KeyslotType_eIvPerEntry:
                numSlots[i] = 0; /*BSP doesn't support this for now?*/
                break;
            case BHSM_KeyslotType_eIvPerBlock256:
                numSlots[i] = bspQuery.out.keySlotNumberIvPerBlock256;
                break;
            case BHSM_KeyslotType_eIvPerEntry256:
                numSlots[i] = bspQuery.out.keySlotNumberIvPerEntry256;
                break;
            default:
                break;
        }
        pModule->types[i].offset = count;
        pModule->types[i].maxNumber = numSlots[i];
        count += numSlots[i];
    }

    BDBG_LEAVE( _Keyslot_LoadConfigurationFromBsp );
    return BERR_SUCCESS;
}
#endif

static BHSM_KeyslotBlockType _GetEntryBlockType( BHSM_KeyslotBlockEntry entry )
{
    BDBG_CASSERT( BHSM_KeyslotBlockType_eCpd == 0 );
    BDBG_CASSERT( BHSM_KeyslotBlockType_eCa  == 1 );
    BDBG_CASSERT( BHSM_KeyslotBlockType_eCps == 2 );

    switch( entry )
    {
        case BHSM_KeyslotBlockEntry_eCpdOdd:
        case BHSM_KeyslotBlockEntry_eCpdEven:
        case BHSM_KeyslotBlockEntry_eCpdClear: { return BHSM_KeyslotBlockType_eCpd; }
        case BHSM_KeyslotBlockEntry_eCaOdd:
        case BHSM_KeyslotBlockEntry_eCaEven:
        case BHSM_KeyslotBlockEntry_eCaClear: { return BHSM_KeyslotBlockType_eCa; }
        case BHSM_KeyslotBlockEntry_eCpsOdd:
        case BHSM_KeyslotBlockEntry_eCpsEven:
        case BHSM_KeyslotBlockEntry_eCpsClear: { return BHSM_KeyslotBlockType_eCps; }
        default: { BERR_TRACE( entry ); }
    }

    return BHSM_KeyslotBlockType_eCa;
}

static BHSM_KeyslotPolarity _GetEntryPolarity( BHSM_KeyslotBlockEntry entry )
{
    BDBG_CASSERT( BHSM_KeyslotPolarity_eOdd == 0 );
    BDBG_CASSERT( BHSM_KeyslotPolarity_eEven == 1 );
    BDBG_CASSERT( BHSM_KeyslotPolarity_eClear == 2 );

    switch( entry )
    {
        case BHSM_KeyslotBlockEntry_eCpdOdd:
        case BHSM_KeyslotBlockEntry_eCaOdd:
        case BHSM_KeyslotBlockEntry_eCpsOdd: { return  BHSM_KeyslotPolarity_eOdd; }
        case BHSM_KeyslotBlockEntry_eCpdEven:
        case BHSM_KeyslotBlockEntry_eCaEven:
        case BHSM_KeyslotBlockEntry_eCpsEven: { return  BHSM_KeyslotPolarity_eEven; }
        case BHSM_KeyslotBlockEntry_eCpdClear:
        case BHSM_KeyslotBlockEntry_eCaClear:
        case BHSM_KeyslotBlockEntry_eCpsClear: { return  BHSM_KeyslotPolarity_eClear; }
        default: { BERR_TRACE( entry ); }
    }

    return BHSM_KeyslotPolarity_eClear;
}

BERR_Code BHSM_KeySlot_SetMulti2Key( BHSM_Handle hHsm, const BHSM_KeySlotSetMulti2Key *pKeyData )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlotMulti2SysKeySet bspConfig;

    BDBG_ENTER( BHSM_KeySlot_SetMulti2Key );

    if( pKeyData == NULL ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( &bspConfig, 0, sizeof(bspConfig) );

    BDBG_CASSERT( sizeof(bspConfig.in.systemKeys) == sizeof(pKeyData->key) );
    rc = BHSM_MemcpySwap( (void*)bspConfig.in.systemKeys, (void*)pKeyData->key, sizeof(pKeyData->key) );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }

    bspConfig.in.whichSysKey = pKeyData->keySelect;

    rc = BHSM_P_KeySlot_Multi2SysKeySet( hHsm, &bspConfig );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    BDBG_LEAVE( BHSM_KeySlot_SetMulti2Key );
    return BERR_SUCCESS;
}

static BERR_Code _SetKeyslotOwnership( BHSM_KeyslotHandle handle, BHSM_SecurityCpuContext owner )
{
    BHSM_P_KeySlotSetOwnership ownership;
    BHSM_P_KeySlot *pSlot;
    BERR_Code rc = BERR_SUCCESS;

    if (!handle) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_MSG(("KeySlot Type[%d] Number[%u] to owner[%s]", pSlot->slotType, pSlot->number, _KeyslotOwnerToString(owner) ));

    BKNI_Memset( &ownership, 0, sizeof(ownership) );
    ownership.in.keySlotType = BHSM_P_ConvertSlotType( pSlot->slotType );
    ownership.in.keySlotNumber = pSlot->number;

    switch( owner ) {
        case BHSM_SecurityCpuContext_eHost: ownership.in.setKtsOwnership = Bsp_KeySlotOwnership_eShared; break;
        case BHSM_SecurityCpuContext_eSage: ownership.in.setKtsOwnership = Bsp_KeySlotOwnership_eSage; break;
        case BHSM_SecurityCpuContext_eNone: ownership.in.setKtsOwnership = Bsp_KeySlotOwnership_eFree; break;
        default:  return BERR_TRACE( BERR_INVALID_PARAMETER );
    }

    rc = BHSM_P_KeySlot_SetOwnership( _Keyslot_GetHsmHandle(handle), &ownership );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE(rc); }

    return BERR_SUCCESS;
}


static BERR_Code _GetKeyslotOwnership( BHSM_KeyslotHandle handle, BHSM_SecurityCpuContext* pOwner )
{
    BERR_Code rc = BERR_SUCCESS;
    BHSM_P_KeySlotQuery bspQuery;
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    uint16_t index;

    if( !pOwner ) return BERR_TRACE( BERR_INVALID_PARAMETER );
    if( !pSlot ) return BERR_TRACE( BERR_INVALID_PARAMETER );
    if( pSlot->slotType >= BHSM_KeyslotType_eMax ) return BERR_TRACE( BERR_INVALID_PARAMETER );

    BKNI_Memset( &bspQuery, 0, sizeof(bspQuery) );

    rc = BHSM_P_KeySlot_Query( _Keyslot_GetHsmHandle(handle), &bspQuery );
    if( rc != BERR_SUCCESS ) { return BERR_TRACE( rc ); }

    index = pSlot->pModule->types[pSlot->slotType].offset + pSlot->number;

    if( index >= sizeof(bspQuery.out.keySlotOwnership) ) return BERR_TRACE( BERR_INVALID_PARAMETER );

    switch( bspQuery.out.keySlotOwnership[index] )
    {
        case Bsp_KeySlotOwnership_eFree: *pOwner = BHSM_SecurityCpuContext_eNone; break;
        case Bsp_KeySlotOwnership_eSage: *pOwner = BHSM_SecurityCpuContext_eSage; break;
        case Bsp_KeySlotOwnership_eShared: *pOwner = BHSM_SecurityCpuContext_eHost; break;
        default:  return BERR_TRACE( BERR_INVALID_PARAMETER );
    }

    BDBG_MSG(( "Keyslot type[%d] Number[%u] owned by[%s]", pSlot->slotType, pSlot->number, _KeyslotOwnerToString(*pOwner) ));

    return BERR_SUCCESS;
}

#if !BDBG_NO_MSG
static char* _KeyslotOwnerToString( BHSM_SecurityCpuContext owner )
{
    switch( owner )
    {
        case BHSM_SecurityCpuContext_eNone: return "None";
        case BHSM_SecurityCpuContext_eSage: return "Sage";
        case BHSM_SecurityCpuContext_eHost: return "Host";
        default: return "undetermined";
    }
}
#endif


typedef struct {
    unsigned keySlotNumber;  /* the required (from SAGE) or expected (from HOST) keyslot number */
    bool srcG;
    bool srcR;
    bool srcT;
    bool destG;
    bool destR;
    bool destT;
}BypassConfig;

/*  Conofigure a bypass keyslot that will allow transfers between regions. */
static BERR_Code _InitBypassKeyslot( BHSM_Handle hHsm, BHSM_KeyslotHandle *phBypassKeyslot, BypassConfig *pConfig )
{
    BERR_Code rc = BERR_UNKNOWN;
    BHSM_KeyslotHandle hKeyslot = *phBypassKeyslot;
    BHSM_KeyslotAllocateSettings keyslotAllocSettings;
    BHSM_KeyslotSettings keyslotSettings;
    BHSM_KeyslotEntrySettings keyslotEntrySettings;
    BHSM_KeyslotKey keyslotKey;
    BHSM_KeyslotInfo keyslotInfo;
    unsigned i = 0;

    if( !hKeyslot ) {
        BHSM_Keyslot_GetDefaultAllocateSettings( &keyslotAllocSettings );
        keyslotAllocSettings.slotType = BHSM_KeyslotType_eIvPerSlot;
        keyslotAllocSettings.keySlotNumber = pConfig->keySlotNumber;  /* ignored if called from HOST. */
        hKeyslot = BHSM_Keyslot_Allocate( hHsm, &keyslotAllocSettings );
        if( !hKeyslot ) { rc = BERR_TRACE( BERR_NOT_AVAILABLE ); goto error; }
    }

    rc = BHSM_GetKeySlotInfo( hKeyslot, &keyslotInfo );
    if( rc != BERR_SUCCESS ) { rc = BERR_TRACE( BERR_UNKNOWN ); goto error; }
    if( keyslotInfo.number != pConfig->keySlotNumber ) { rc = BERR_TRACE( BERR_NOT_AVAILABLE ); goto error; }

    /* keyslot configuration. */
    BHSM_Keyslot_GetSettings( hKeyslot, &keyslotSettings );
    /* allowed sources */
    keyslotSettings.regions.source[BHSM_SecurityRegion_eGlr] = pConfig->srcG;                 /* G ? */
    keyslotSettings.regions.source[BHSM_SecurityRegion_eCrr] = pConfig->srcR;                 /* R ? */
    keyslotSettings.regions.source[BHSM_SecurityRegion_eCrrT] = pConfig->srcT;                 /* T ? */
    /* allowed destinations */                                                                /* to  */
    keyslotSettings.regions.destinationRPipe[BHSM_SecurityRegion_eGlr] = pConfig->destG;      /* G ? */
    keyslotSettings.regions.destinationGPipe[BHSM_SecurityRegion_eGlr] = pConfig->destG;
    keyslotSettings.regions.destinationRPipe[BHSM_SecurityRegion_eCrr] = pConfig->destR;      /* R ? */
    keyslotSettings.regions.destinationGPipe[BHSM_SecurityRegion_eCrr] = pConfig->destR;
    keyslotSettings.regions.destinationRPipe[BHSM_SecurityRegion_eCrrT] = pConfig->destT;
    keyslotSettings.regions.destinationGPipe[BHSM_SecurityRegion_eCrrT] = pConfig->destT;
    rc = BHSM_Keyslot_SetSettings( hKeyslot, &keyslotSettings );
    if( rc != BERR_SUCCESS ) { rc = BERR_TRACE( BERR_NOT_AVAILABLE ); goto error; }

    /* entry configuration. We use the CPD **Clear** entry. */
    BHSM_Keyslot_GetEntrySettings( hKeyslot, BHSM_KeyslotBlockEntry_eCpdClear, &keyslotEntrySettings );
    keyslotEntrySettings.algorithm = BHSM_CryptographicAlgorithm_eAes128; /* any alg compatiable with CPD.*/
    /* ensure that the Clear TS SC bits remain clear */
    keyslotEntrySettings.outputPolarity.specify = true;
    keyslotEntrySettings.outputPolarity.rPipe = BHSM_KeyslotPolarity_eClear;
    keyslotEntrySettings.outputPolarity.gPipe = BHSM_KeyslotPolarity_eClear;
    /* output TS SC bits */
    keyslotEntrySettings.rPipeEnable = false; /* don't apply decryption to transport packets. */
    keyslotEntrySettings.gPipeEnable = false;
    rc = BHSM_Keyslot_SetEntrySettings( hKeyslot, BHSM_KeyslotBlockEntry_eCpdClear, &keyslotEntrySettings );
    if( rc != BERR_SUCCESS ) { rc = BERR_TRACE( BERR_NOT_AVAILABLE ); goto error; }

    /* set a key! */
    BKNI_Memset( &keyslotKey, 0, sizeof(keyslotKey) );
    keyslotKey.size = 16;
    for( i = 0; i < keyslotKey.size; i++ ) { keyslotKey.key[i] = i; } /* random non zero key. */
    rc = BHSM_Keyslot_SetEntryKey( hKeyslot, BHSM_KeyslotBlockEntry_eCpdClear, &keyslotKey );
    if( rc != BERR_SUCCESS ) { rc = BERR_TRACE( BERR_NOT_AVAILABLE ); goto error; }

    /* Using LOG for now. TODO disabled. */
    BDBG_MSG(("Bypass Keyslot initialised. Type[%d] Number[%u] [%s%s%s->%s%s%s]", keyslotInfo.type, keyslotInfo.number
                                                                                , pConfig->srcG  ? "G":"", pConfig->srcR  ? "R":"", pConfig->srcT  ? "T":""
                                                                                , pConfig->destG ? "G":"", pConfig->destR ? "R":"", pConfig->destT ? "T":"" ));
    *phBypassKeyslot = hKeyslot;
    return BERR_SUCCESS;

error:

    return BERR_TRACE(rc);
}

BERR_Code BHSM_InitialiseBypassKeyslots( BHSM_Handle hHsm )
{
    BERR_Code rc = BERR_UNKNOWN;
    BypassConfig bypassConfig;
    BHSM_KeySlotModule* pModule = NULL;
    BHSM_P_KeySlotPidAdd hsmAddPid;

    BDBG_ENTER( BHSM_InitialiseBypassKeyslots );

    #define BYPASS_KEYSLOT_NUMBER_G2GR (0)  /* the default */
    #define BYPASS_KEYSLOT_NUMBER_GR2R (1)
    #define BYPASS_KEYSLOT_NUMBER_GT2T (2)

    if( !hHsm ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pModule = hHsm->modules.pKeyslots;
    if( !pModule ) { return BERR_TRACE( BERR_NOT_INITIALIZED ); }

    /* setup bypass G->GR, the default */
    BKNI_Memset( &bypassConfig, 0, sizeof(bypassConfig) );
    bypassConfig.keySlotNumber = BYPASS_KEYSLOT_NUMBER_G2GR;
    bypassConfig.srcG  = true;
    bypassConfig.destG = true;
   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    bypassConfig.destR = true;
   #endif
    rc = _InitBypassKeyslot( hHsm, &pModule->hBypassKeyslotG2GR, &bypassConfig );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto error; }

    /* setup bypass GR->R */
    BKNI_Memset( &bypassConfig, 0, sizeof(bypassConfig) );
    bypassConfig.keySlotNumber = BYPASS_KEYSLOT_NUMBER_GR2R;
    bypassConfig.srcG  = true;
   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    bypassConfig.srcR  = true;
    bypassConfig.destR = true;
   #else
    bypassConfig.destG = true; /*we can only set G from HOST*/
   #endif
    rc = _InitBypassKeyslot( hHsm, &pModule->hBypassKeyslotGR2R, &bypassConfig );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto error; }

    /* setup bypass GT->T */
    BKNI_Memset( &bypassConfig, 0, sizeof(bypassConfig) );
    bypassConfig.keySlotNumber = BYPASS_KEYSLOT_NUMBER_GT2T;
    bypassConfig.srcG  = true;
   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    bypassConfig.srcT  = true;
    bypassConfig.destT = true;
   #else
    bypassConfig.destG = true; /*we can only set G from HOST*/
   #endif
    rc = _InitBypassKeyslot( hHsm, &pModule->hBypassKeyslotGT2T, &bypassConfig );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto error; }

    /* associate all pid channels with the default bypass keyslot. */
    BKNI_Memset( &hsmAddPid, 0, sizeof(hsmAddPid) );
    hsmAddPid.in.setMultiplePidChan = 1; /*true!*/
    hsmAddPid.in.pidChanStart  = 0;
    hsmAddPid.in.pidChanEnd    = BSP_TOTAL_PIDCHANNELS-1;
    hsmAddPid.in.keySlotType   = BHSM_P_ConvertSlotType( BHSM_KeyslotType_eIvPerSlot );
    hsmAddPid.in.keySlotNumber = BYPASS_KEYSLOT_NUMBER_G2GR;

    rc = BHSM_P_KeySlot_PidAdd( hHsm, &hsmAddPid );
    if( rc != BERR_SUCCESS ) { BERR_TRACE( rc ); goto error; }

    BDBG_LEAVE( BHSM_InitialiseBypassKeyslots );

    return BERR_SUCCESS;

error:

    /* we'll not clean up allocated bypass slots at this point, it will happen on close. */
    return BERR_TRACE( rc );
}

BERR_Code  BHSM_SetPidChannelBypassKeyslot( BHSM_Handle hHsm,
                                            unsigned pidChannelIndex,
                                            BHSM_BypassKeySlot_e bypassKeyslot )
{
    BERR_Code rc = BERR_UNKNOWN;
    BHSM_KeySlotModule* pModule = NULL;

    if( !hHsm ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pModule = hHsm->modules.pKeyslots;
    if( !pModule ) { return BERR_TRACE( BERR_NOT_INITIALIZED ); }
    if( !pModule->hBypassKeyslotG2GR ) { return BERR_TRACE( BERR_NOT_INITIALIZED ); }
    if( !pModule->hBypassKeyslotGR2R ) { return BERR_TRACE( BERR_NOT_INITIALIZED ); }

    switch( bypassKeyslot ) {
        default:
        {
            BERR_TRACE( BERR_INVALID_PARAMETER );
            /* fall though to G2GR*/
        }
        case BHSM_BypassKeySlot_eG2GR:
        {
            rc = BHSM_Keyslot_AddPidChannel( pModule->hBypassKeyslotG2GR, pidChannelIndex );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE( BERR_NOT_AVAILABLE ); }
            break;
        }
        case BHSM_BypassKeySlot_eGR2R:
        {
            rc = BHSM_Keyslot_AddPidChannel( pModule->hBypassKeyslotGR2R, pidChannelIndex );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE( BERR_NOT_AVAILABLE ); }
            break;
        }
        case BHSM_BypassKeySlot_eGT2T:
        {
            rc = BHSM_Keyslot_AddPidChannel( pModule->hBypassKeyslotGT2T, pidChannelIndex );
            if( rc != BERR_SUCCESS ) { return BERR_TRACE( BERR_NOT_AVAILABLE ); }
            break;
        }
    }

    return BERR_SUCCESS;
}

BERR_Code  BHSM_GetPidChannelBypassKeyslot( BHSM_Handle hHsm,
                                            unsigned pidChannelIndex,
                                            BHSM_BypassKeySlot_e *pBypassKeyslot )
{
    BSTD_UNUSED( hHsm );
    BSTD_UNUSED( pidChannelIndex );
    BSTD_UNUSED( pBypassKeyslot );

    BERR_TRACE( BERR_NOT_AVAILABLE ); /* not available for now! */

    return 0;
}

/*
    DEPRECATED use BHSM_P_Keyslot_GetSlotDetails or BHSM_P_Keyslot_GetEntryDetails
*/
BERR_Code BHSM_P_Keyslot_GetDetails( BHSM_KeyslotHandle handle,
                                     BHSM_KeyslotBlockEntry entry,
                                     BHSM_KeyslotDetails *pDetails )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_P_Keyslot_GetDetails );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pSlot->configured ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pDetails ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset(pDetails, 0, sizeof(BHSM_KeyslotDetails));

    pDetails->ctrlWord0 = compileControl0_GlobalHi( handle );
    pDetails->ctrlWord1 = compileControl1_GlobalLo( handle );

    pDetails->slotType = pSlot->slotType;
    pDetails->number =  pSlot->number;

    pDetails->hHsm = pSlot->pModule->hHsm;

    if(entry < BHSM_KeyslotBlockEntry_eMax)
    {
        BHSM_P_KeyEntry *pEntry = _GetEntry( handle, entry );
        if( !pEntry ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
        if( !pEntry->configured ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

        pDetails->keyModeBspMapped = _Keyslot_MapKeyMode(pEntry->settings.keyMode);

        pDetails->ctrlWord2 = compileControl2_ModeHi( handle, entry );
        pDetails->ctrlWord3 = compileControl3_ModeLo( handle, entry );

        pDetails->polarity = _GetEntryPolarity( entry );
        pDetails->blockType = _GetEntryBlockType( entry );

        if( pEntry->settings.external.iv )
        {
            pDetails->externalIvValid = true;
            pDetails->externalIvOffset = pEntry->pExternalSlot->offsetIv + pEntry->pExternalSlot->slotPtr;
        }

        if( pDetails->blockType >= BHSM_KeyslotBlockType_eMax )  { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

        pDetails->sc01.useEntry = pSlot->settings.sc01[pDetails->blockType].useEntry;
        pDetails->sc01.outputPolarity.rPipe = pSlot->settings.sc01[pDetails->blockType].outputPolarity.rPipe;
        pDetails->sc01.outputPolarity.gPipe = pSlot->settings.sc01[pDetails->blockType].outputPolarity.gPipe;
    }
    else
    {
        pDetails->polarity = BHSM_KeyslotPolarity_eMax;
        pDetails->blockType = BHSM_KeyslotBlockType_eMax;
    }

    BDBG_LEAVE( BHSM_P_Keyslot_GetDetails );
    return BERR_SUCCESS;
}


BERR_Code BHSM_P_Keyslot_GetSlotDetails( BHSM_KeyslotHandle handle, BHSM_KeyslotSlotDetails *pInfo )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;

    BDBG_ENTER( BHSM_P_Keyslot_GetSlotDetails );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pSlot->configured ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pInfo ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( pInfo, 0, sizeof(*pInfo) );

    pInfo->hHsm = pSlot->pModule->hHsm;
    pInfo->slotType = pSlot->slotType;
    pInfo->number =  pSlot->number;

    BDBG_LEAVE( BHSM_P_Keyslot_GetSlotDetails );
    return BERR_SUCCESS;
}


BERR_Code BHSM_P_Keyslot_GetEntryDetails( BHSM_KeyslotHandle handle,
                                          BHSM_KeyslotBlockEntry entry,
                                          BHSM_KeyslotEntryDetails *pInfo )
{
    BHSM_P_KeySlot *pSlot = (BHSM_P_KeySlot*)handle;
    BHSM_P_KeyEntry *pEntry = NULL;

    BDBG_ENTER( BHSM_P_Keyslot_GetEntryDetails );

    if( !pSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pSlot->configured ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pInfo ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    pEntry = _GetEntry( handle, entry );
    if( !pEntry ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }
    if( !pEntry->configured ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

    BKNI_Memset( pInfo, 0, sizeof(*pInfo) );

    pInfo->blockType = _GetEntryBlockType( entry );
    pInfo->polarity = _GetEntryPolarity( entry );

    pInfo->bspMapped.keyMode = _Keyslot_MapKeyMode( pEntry->settings.keyMode );
    pInfo->bspMapped.ctrlWord0 = compileControl0_GlobalHi( handle );
    pInfo->bspMapped.ctrlWord1 = compileControl1_GlobalLo( handle );
    pInfo->bspMapped.ctrlWord2 = compileControl2_ModeHi( handle, entry );
    pInfo->bspMapped.ctrlWord3 = compileControl3_ModeLo( handle, entry );

    pInfo->bspMapped.slotType = BHSM_P_ConvertSlotType( pSlot->slotType );
    pInfo->bspMapped.blockType = _GetEntryBlockType( entry );
    pInfo->bspMapped.entryType = _GetEntryPolarity( entry );

    if( pInfo->bspMapped.blockType >= BHSM_KeyslotBlockType_eMax ) return BERR_TRACE( BERR_INVALID_PARAMETER );

    pInfo->bspMapped.sc01.useEntry = pSlot->settings.sc01[pInfo->blockType].useEntry;
    pInfo->bspMapped.sc01.outputPolarity.rPipe = pSlot->settings.sc01[pInfo->blockType].outputPolarity.rPipe;
    pInfo->bspMapped.sc01.outputPolarity.gPipe = pSlot->settings.sc01[pInfo->blockType].outputPolarity.gPipe;

    if( pEntry->settings.external.iv ) {

        if( !pEntry->pExternalSlot ) { return BERR_TRACE( BERR_INVALID_PARAMETER ); }

        pInfo->bspMapped.externalIvValid = true;
        pInfo->bspMapped.externalIvOffset = pEntry->pExternalSlot->offsetIv + pEntry->pExternalSlot->slotPtr;
    }

    BDBG_LEAVE( BHSM_P_Keyslot_GetEntryDetails );
    return BERR_SUCCESS;
}


uint8_t BHSM_P_Map2KeySlotCryptoAlg( BHSM_CryptographicAlgorithm  algorithm )
{

    switch( algorithm ) {
        case BHSM_CryptographicAlgorithm_eDvbCsa2: return Bsp_KeySlotCryptoAlg_eDvbCsa2;
        case BHSM_CryptographicAlgorithm_eDvbCsa3: return Bsp_KeySlotCryptoAlg_eDvbCsa3;
        case BHSM_CryptographicAlgorithm_eMulti2:  return Bsp_KeySlotCryptoAlg_eMulti2;
        case BHSM_CryptographicAlgorithm_eDes:     return Bsp_KeySlotCryptoAlg_eDes;
        case BHSM_CryptographicAlgorithm_e3DesAba: return Bsp_KeySlotCryptoAlg_eTdesAba;
        case BHSM_CryptographicAlgorithm_e3DesAbc: return Bsp_KeySlotCryptoAlg_eTdesAbc;
        case BHSM_CryptographicAlgorithm_eAes128:  return Bsp_KeySlotCryptoAlg_eAes128;
        case BHSM_CryptographicAlgorithm_eAes192:  return Bsp_KeySlotCryptoAlg_eAes192;
        case BHSM_CryptographicAlgorithm_eAes256:  return Bsp_KeySlotCryptoAlg_eAes256;
        case BHSM_CryptographicAlgorithm_eReserved19: return Bsp_KeySlotCryptoAlg_eRedacted_9;
        case BHSM_CryptographicAlgorithm_eCam128:  return Bsp_KeySlotCryptoAlg_eCam128;
        case BHSM_CryptographicAlgorithm_eCam192:  return Bsp_KeySlotCryptoAlg_eCam192;
        case BHSM_CryptographicAlgorithm_eCam256:  return Bsp_KeySlotCryptoAlg_eCam256;
        case BHSM_CryptographicAlgorithm_eGhash:   return Bsp_KeySlotCryptoAlg_eGHash;
        default: { BERR_TRACE( BERR_INVALID_PARAMETER ); }
    }

    return  Bsp_KeySlotCryptoAlg_eMax;
}

uint8_t BHSM_P_ConvertSlotType( BHSM_KeyslotType type )
{

    switch ( type )
    {
        case BHSM_KeyslotType_eIvPerSlot:   return 0;
        case BHSM_KeyslotType_eIvPerBlock:  return 1;
        case BHSM_KeyslotType_eIvPerEntry:  return 3;  /* no 128bit per entry exists on zeus5 */
        case BHSM_KeyslotType_eIvPerBlock256:  return 2;
        case BHSM_KeyslotType_eIvPerEntry256:  return 3;  /* no 128bit per entry exists on zeus5 */
        default:  BERR_TRACE( type );
    }

    return 0;
}

BHSM_KeyslotHandle BHSM_P_GetKeySlotHandle( BHSM_Handle hHsm, BHSM_KeyslotType slotType, unsigned slotNumber )
{
    BHSM_KeySlotModule* pModule;
    unsigned offset;
    unsigned maxNumber;

    if( !hHsm ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }
    if( slotType >= BHSM_KeyslotType_eMax ) { BERR_TRACE( BERR_INVALID_PARAMETER ); return NULL; }

    pModule = hHsm->modules.pKeyslots;
    offset = pModule->types[slotType].offset;
    maxNumber = pModule->types[slotType].maxNumber;

    if( slotNumber >= maxNumber || ( offset + slotNumber ) >= BHSM_MAX_KEYSLOTS ) {
        BERR_TRACE( BERR_INVALID_PARAMETER );
        return NULL;
    }

    return (BHSM_KeyslotHandle)pModule->pKeySlotHandles[offset + slotNumber];
}

static uint8_t _Keyslot_MapKeyMode( BHSM_KeySlotKeyMode keyMode )
{
    switch( keyMode )
    {
        case BHSM_KeySlotKeyMode_eRegular:        { return 0; }
        case BHSM_KeySlotKeyMode_eDes56:          { return 1; }
        case BHSM_KeySlotKeyMode_eDvbConformance: { return 4; }
        default: { BERR_TRACE( BERR_INVALID_PARAMETER ); }
    }

    return (uint8_t)keyMode; /* return what was input. */
}
