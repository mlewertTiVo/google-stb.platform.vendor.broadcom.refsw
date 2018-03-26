/******************************************************************************
 *  Copyright (C) 2018 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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

/* This file is autogenerated, do not edit. */

#ifndef BSP_HWKL__H
#define BSP_HWKL__H

#ifdef __cplusplus
extern "C"
{
#endif

/* COMMANDS for the "Hwkl" component */
typedef enum
{
    Bsp_CmdHwkl_eRouteKey = 0x00,
    Bsp_CmdHwkl_eRootConfig = 0x01,
    Bsp_CmdHwkl_eLayerSet = 0x02,
    Bsp_CmdHwkl_eQuery = 0x03,
    Bsp_CmdHwkl_eMax
} Bsp_CmdHwkl_e;


/*PARAMETERS for the Bsp_CmdHwkl_eRouteKey command  */
/*input*/
typedef struct
{
    uint8_t  blockType; /* enum type Bsp_KeySlotBlockType_e */
    uint8_t  entryType; /* enum type Bsp_KeySlotEntryType_e */
    uint8_t  keySlotType; /* enum type Bsp_KeySlotType_e */
    uint8_t  keySlotNumber;
    uint8_t  sc01ModeWordMapping; /* enum type Bsp_Sc01ModeWordMapping_e */
    uint8_t  gPipeSc01EntryType;
    uint8_t  rPipeSc01EntryType;
    uint8_t  keyMode; /* enum type Bsp_KeyMode_e */
    uint32_t modeWords[4];
    uint16_t extIvPtr;
    uint8_t  keyLayer; /* enum type Bsp_HwklLayer_e */
    uint8_t  reserved0;

}Bsp_Hwkl_RouteKey_InFields_t;
/*output*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Hwkl_RouteKey_OutFields_t;


/*PARAMETERS for the Bsp_CmdHwkl_eRootConfig command  */
/*input*/
typedef struct
{
    uint8_t  askmTdesKlRootKeySwapEnable;
    uint8_t  rootKeySrc; /* enum type Bsp_RootKeySrc_e */
    uint8_t  reserved0;
    uint8_t  globalKeyOwnerIdSelect; /* enum type Bsp_OwnerIdSelect_e */
    uint8_t  globalKeyIndex;
    uint8_t  reserved1;
    uint8_t  kdfVersion;
    uint8_t  reserved2;
    uint32_t procIn[4];
    uint16_t caVendorId;
    uint8_t  reserved3;
    uint8_t  stbOwnerIdSel; /* enum type Bsp_StbOwnerId_e */
    uint8_t  askmMaskKeySel; /* enum type Bsp_Askm_MaskKeySel_e */
    uint8_t  moduleId;
    uint8_t  reserved4;
    uint8_t  hwklLength; /* enum type Bsp_HwklLength_e */
    uint8_t  hwklDestinationAlg; /* enum type Bsp_KeySlotCryptoAlg_e */
    uint8_t  reserved5[3];

}Bsp_Hwkl_RootConfig_InFields_t;
/*output*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Hwkl_RootConfig_OutFields_t;


/*PARAMETERS for the Bsp_CmdHwkl_eLayerSet command  */
/*input*/
typedef struct
{
    uint8_t  destinationKeyLayer; /* enum type Bsp_HwklLayer_e */
    uint8_t  reserved0[3];
    uint32_t procIn[4];

}Bsp_Hwkl_LayerSet_InFields_t;
/*output*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Hwkl_LayerSet_OutFields_t;


/*PARAMETERS for the Bsp_CmdHwkl_eQuery command  */
/*input*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Hwkl_Query_InFields_t;
/*output*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Hwkl_Query_OutFields_t;


#ifdef __cplusplus
}
#endif
#endif
