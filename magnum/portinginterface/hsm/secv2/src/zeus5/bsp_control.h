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

/* This file is autogenerated, do not edit. */

#ifndef BSP_CONTROL__H
#define BSP_CONTROL__H

#ifdef __cplusplus
extern "C"
{
#endif

/* COMMANDS for the "Control" component */
typedef enum
{
    Bsp_CmdControl_eRedacted_0x01 = 0x01,
    Bsp_CmdControl_eRedacted_0x02 = 0x02,
    Bsp_CmdControl_eExceptionCaptureRegRead = 0x03,
    Bsp_CmdControl_eForceScBit = 0x09,
    Bsp_CmdControl_eRedacted_0x0a = 0x0a,
    Bsp_CmdControl_eRedacted_0x0b = 0x0b,
    Bsp_CmdControl_eMax
} Bsp_CmdControl_e;


/*PARAMETERS for the Bsp_CmdControl_eExceptionCaptureRegRead command  */
/*input*/
typedef struct
{
    uint8_t  keepStatus; /* enum type BCMD_ExceptionCaptureRegRead_KeepStatus_e */
    uint8_t  device; /* enum type BCMD_ExceptionCaptureRegRead_Device_e */
    uint8_t  unit;
    uint8_t  subUnit;

}Bsp_Control_ExceptionCaptureRegRead_InFields_t;
/*output*/
typedef struct
{
    uint8_t  device; /* enum type BCMD_ExceptionCaptureRegRead_Device_e */
    uint8_t  unit;
    uint8_t  subUnit;
    uint8_t  numRegs;
    uint32_t registerX[20];

}Bsp_Control_ExceptionCaptureRegRead_OutFields_t;


/*PARAMETERS for the Bsp_CmdControl_eForceScBit command  */
/*input*/
typedef struct
{
    uint8_t  xptBandSel; /* enum type Bsp_XptBandSel_e */
    uint8_t  xptBandNum;
    uint8_t  forceScBit;
    uint8_t  reserved;

}Bsp_Control_ForceScBit_InFields_t;
/*output*/
typedef struct
{
    unsigned dummy; /*no elements*/

}Bsp_Control_ForceScBit_OutFields_t;


#ifdef __cplusplus
}
#endif
#endif
