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

#ifndef BHSM_KEYSLOT_PRIV_H__
#define BHSM_KEYSLOT_PRIV_H__

#include "bhsm_keyslot.h"


typedef struct{

    bool allocated;            /* external slot is allocated to a ragular keyslot*/
    unsigned slotPtr;

    unsigned offsetIv;
    unsigned offsetKey;

}BHSM_P_ExternalKeySlot;


/*data structure representing a keyslot entry */
typedef struct
{
    bool configured;
    BHSM_KeyslotEntrySettings settings; /* keyslot entry settings. */

    BHSM_P_ExternalKeySlot *pExternalSlot;

}BHSM_P_KeyEntry;

/*data structure representing a keyslot instance (the handle)*/
typedef struct
{
    bool configured;
    BHSM_KeyslotType slotType;  /* The keyslot type   */
    unsigned number;            /* The keyslot number */

    BHSM_KeyslotSettings settings;              /* generic keyslot settings. */
    BHSM_P_KeyEntry entry[BHSM_KeyslotBlockEntry_eMax];

    struct BHSM_KeySlotModule *pModule; /* pointer back to module data. */

   #ifdef BHSM_BUILD_HSM_FOR_SAGE
    void *pRestricted;                   /* restricted extension. */
   #endif

}BHSM_P_KeySlot;


/*DEPRECATED .. use BHSM_P_Keyslot_GetInfo and BHSM_P_Keyslot_GetEntryInfo */
typedef struct{
    unsigned numKeySlotsForType[BHSM_KeyslotType_eMax];
} BHSM_KeyslotModuleCapabilities;

typedef struct{
    unsigned numKeySlotsForType[BHSM_KeyslotType_eMax];
}BHSM_KeyslotModuleSettings;


typedef struct {

    uint8_t keyModeBspMapped;            /* the KeyMode mapped to BSP*/
    uint32_t ctrlWord0;
    uint32_t ctrlWord1;
    uint32_t ctrlWord2;
    uint32_t ctrlWord3;

    BHSM_KeyslotType slotType;      /* The keyslot TYPE */
    unsigned number;                /* Only required on SAGE side. Ignored on HOST side. */
    BHSM_KeyslotPolarity polarity;  /* odd/even/clear*/
    BHSM_KeyslotBlockType blockType;/* cps/ca/cpd */

    bool externalIvValid;
    unsigned externalIvOffset;

    struct
    {
        BHSM_KeyslotPolarity useEntry;
        struct
        {
            BHSM_KeyslotPolarity  rPipe;
            BHSM_KeyslotPolarity  gPipe;
        }outputPolarity;
    }sc01;

    BHSM_Handle hHsm;

}BHSM_KeyslotDetails;


/* Internal Keyslot paramters. */
typedef struct {

    BHSM_Handle hHsm;
    BHSM_KeyslotType slotType;      /* The keyslot TYPE */
    unsigned number;                /* Only required on SAGE side. Ignored on HOST side. */

}BHSM_KeyslotSlotDetails;


/* internal key entry parameters. */
typedef struct
{

    BHSM_KeyslotPolarity polarity;  /* odd/even/clear*/
    BHSM_KeyslotBlockType blockType;/* cps/ca/cpd */

    struct{
        uint8_t  keyMode;            /* the KeyMode mapped to BSP*/
        uint32_t ctrlWord0;
        uint32_t ctrlWord1;
        uint32_t ctrlWord2;
        uint32_t ctrlWord3;
        uint8_t  slotType;
        uint8_t  blockType;     /* cpd/ca/cps */
        uint8_t  entryType;     /* polarity ... even/odd/clear/reserved. */
        struct {
            uint8_t useEntry;   /* polarity ... even/odd/clear/reserved. */
            struct
            {
                uint8_t  rPipe;
                uint8_t  gPipe;
            }outputPolarity;
        }sc01;

        bool externalIvValid;
        unsigned externalIvOffset;
    }bspMapped;

}BHSM_KeyslotEntryDetails;


BERR_Code BHSM_Keyslot_Init( BHSM_Handle hHsm, BHSM_KeyslotModuleSettings *pSettings );
void BHSM_Keyslot_Uninit( BHSM_Handle hHsm );


BERR_Code BHSM_P_Keyslot_GetSlotDetails( BHSM_KeyslotHandle handle,
                                         BHSM_KeyslotSlotDetails *pInfo );
BERR_Code BHSM_P_Keyslot_GetEntryDetails( BHSM_KeyslotHandle handle,
                                          BHSM_KeyslotBlockEntry entry,
                                          BHSM_KeyslotEntryDetails *pInfo );

uint8_t BHSM_P_Map2KeySlotCryptoAlg( BHSM_CryptographicAlgorithm  algorithm );
uint8_t BHSM_P_ConvertSlotType( BHSM_KeyslotType type );

/* Look up existing BHSM_KeyslotHandle, given the type and number
 * RESTRICTED usage only. BHSM clients should typically use the allocate/free mechanism */
BHSM_KeyslotHandle BHSM_P_GetKeySlotHandle( BHSM_Handle hHsm,
                                            BHSM_KeyslotType slotType,
                                            unsigned slotNumber );

#ifdef BHSM_BUILD_HSM_FOR_SAGE
  BERR_Code BHSM_Keyslot_InitExtensionRestricted( BHSM_KeyslotHandle handle );
  void  BHSM_Keyslot_UninitExtensionRestricted( BHSM_KeyslotHandle handle );
  uint32_t BHSM_P_CompileControl0GlobalHiReserved( BHSM_KeyslotHandle handle );
#endif






/* DEPRECATED .. use BHSM_P_Keyslot_GetSlotDetails */
BERR_Code BHSM_P_KeyslotModule_GetCapabilities( BHSM_Handle hHsm,
                                                BHSM_KeyslotModuleCapabilities *pCaps );
/* DEPRECATED .. use BHSM_P_Keyslot_GetSlotDetails or BHSM_P_Keyslot_GetEntryDetails */
BERR_Code BHSM_P_Keyslot_GetDetails( BHSM_KeyslotHandle handle,
                                     BHSM_KeyslotBlockEntry entry,
                                     BHSM_KeyslotDetails *pDetails );
#endif
