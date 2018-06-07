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


#ifndef BSP_S_USER_RSA_H__
#define BSP_S_USER_RSA_H__



#define USER_RSA_COUNTERMEASURES_MASK     0x0000000FU

#define USER_RSA_DPA_MODEXP_PROTECT_MASK  0x00000008U
#define USER_RSA_DPA_MODEXP_PROTECT_SHIFT 3

#define USER_RSA_DPA_RANDOM_STALL_MASK    0x00000006U
#define USER_RSA_DPA_RANDOM_STALL_SHIFT   1

#define USER_RSA_FW_RSA_MODE1_MASK       0x00000001U
#define USER_RSA_FW_RSA_MODE1_SHIFT      0

typedef enum BCMD_User_RSA_InCmdField_e
{
    BCMD_User_RSA_InCmdField_eCountermeasure = (5 << 2) + 1,
    BCMD_User_RSA_InCmdField_eRSASize = (5 << 2) + 2,
    BCMD_User_RSA_InCmdField_esubCmdId = (5 << 2) + 3,
    BCMD_User_RSA_InCmdField_eRSAKeyData = (8 << 2) + 0,
    BCMD_User_RSA_InCmdField_eMax

}BCMD_User_RSA_InCmdField_e;

typedef enum BCMD_User_RSA_OutCmdField_e
{
    BCMD_User_RSA_OutCmdField_eOutputDataSize = (6 << 2) + 2,
    BCMD_User_RSA_OutCmdField_eOutputData = (7 << 2),
    BCMD_User_RSA_OutCmdField_eMax

}BCMD_User_RSA_OutCmdField_e;

typedef enum BCMD_User_RSA_SubCommand_e
{
    BCMD_User_RSA_SubCommand_eLoadModulus = 0,
    BCMD_User_RSA_SubCommand_eLoadExponent = 1,
    BCMD_User_RSA_SubCommand_eLoadBaseStartOp = 2,
    BCMD_User_RSA_SubCommand_eReserved3  =  3,
    BCMD_User_RSA_SubCommand_eMax
}BCMD_User_RSA_SubCommand_e;


typedef enum BCMD_User_RSA_Size_e
{
    BCMD_User_RSA_Size_e1024 = 0,
    BCMD_User_RSA_Size_e2048 = 1,
    BCMD_User_RSA_Size_eMax
}BCMD_User_RSA_Size_e;

#endif
