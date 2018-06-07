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

#ifndef BHSM_OTP_MSP_INDEXES__H
#define BHSM_OTP_MSP_INDEXES__H

#ifdef __cplusplus
extern "C"
{
#endif

 /*  OTP MSP indexes. */
#if BHSM_ZEUS_VERSION >= BHSM_ZEUS_VERSION_CALC(5,0)

#define BHSM_OTPMSP_PCIE0_HOST_PROTECT  0
#define BHSM_OTPMSP_PCIE0_CLIENT_PROTECT  1
#define BHSM_OTPMSP_PCIE0_MWIN_RESTRICT_ENABLE  2
#define BHSM_OTPMSP_PCIE0_MWIN_DISABLE  3
#define BHSM_OTPMSP_PCIE0_GWIN_DISABLE  4
#define BHSM_OTPMSP_PCIE0_MWIN_SIZE_ENFORCE_ENABLE  5
#define BHSM_OTPMSP_PCIE1_HOST_PROTECT  6
#define BHSM_OTPMSP_PCIE1_CLIENT_PROTECT  7
#define BHSM_OTPMSP_PCIE1_MWIN_RESTRICT_ENABLE  8
#define BHSM_OTPMSP_PCIE1_MWIN_DISABLE  9
#define BHSM_OTPMSP_PCIE1_GWIN_DISABLE  10
#define BHSM_OTPMSP_PCIE1_MWIN_SIZE_ENFORCE_ENABLE  11
#define BHSM_OTPMSP_CR_LOCK_ENABLE  16
#define BHSM_OTPMSP_ASYMMETRIC_CR_LOCK_ENABLE  18
#define BHSM_OTPMSP_FORCE_DRAM_SCRAMBLER  33
#define BHSM_OTPMSP_DATA_SECTION1_LOCK  99
#define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_A  120
#define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_B  121
#define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_C  122
#define BHSM_OTPMSP_PUBLIC_KEY0_INDEX  139
#define BHSM_OTPMSP_SECURE_BOOT_ENABLE  140
#define BHSM_OTPMSP_RAAGA_A_VERIFY_ENABLE  143
#define BHSM_OTPMSP_RAAGA_B_VERIFY_ENABLE  144
#define BHSM_OTPMSP_VIDEO_VERIFY_ENABLE  145
#define BHSM_OTPMSP_VICE_VERIFY_ENABLE  146
#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_KEY_SRC  148
#define BHSM_OTPMSP_BOOT_CODE_HIDE_ENABLE  149
#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_KEY_INDEX  150
#define BHSM_OTPMSP_ASKM_STB_OWNER_ID  169
#define BHSM_OTPMSP_CP_CONTROL_BITS  170
#define BHSM_OTPMSP_CA_CONTROL_BITS  171
#define BHSM_OTPMSP_FORCE_SC_DISALLOW  172
#define BHSM_OTPMSP_EBI_CS_SWAP_DISABLE  181
#define BHSM_OTPMSP_CA_KEY_LADDER_DISABLE  182
#define BHSM_OTPMSP_CP_KEY_LADDER_DISABLE  183
#define BHSM_OTPMSP_GLOBAL_KEY_TO_CA_KEY_LADDER_DISABLE  184
#define BHSM_OTPMSP_GLOBAL_KEY_TO_CP_KEY_LADDER_DISABLE  185
#define BHSM_OTPMSP_TWO_STAGE_CA_KEY_LADDER_DISABLE  189
#define BHSM_OTPMSP_TWO_STAGE_CP_KEY_LADDER_DISABLE  190
#define BHSM_OTPMSP_CA_CPD_DES_TDES_CBC_SOFTWARE_KEY_DISABLE  191
#define BHSM_OTPMSP_CA_SOFTWARE_KEY_DISABLE  202
#define BHSM_OTPMSP_CA_DVB_CSA3_SOFTWARE_KEY_DISABLE  204
#define BHSM_OTPMSP_CP_MULTI2_ECB_CBC_SOFTWARE_KEY_DISABLE  206
#define BHSM_OTPMSP_RPMB_HOST_REPLAY_KEY_LADDER_DISABLE  209
#define BHSM_OTPMSP_RPMB_USE_KEY_MOD  210
#define BHSM_OTPMSP_RPMB_OUT_KEY_MOD  211
#define BHSM_OTPMSP_CP_DES_SOFTWARE_KEY_DISABLE  212
#define BHSM_OTPMSP_CP_TDES_SOFTWARE_KEY_DISABLE  213
#define BHSM_OTPMSP_CP_AES_SOFTWARE_KEY_DISABLE  214
#define BHSM_OTPMSP_CP_AES_COUNTER_SOFTWARE_KEY_DISABLE  215
#define BHSM_OTPMSP_IP_LICENSING_CHECK_ENABLE  217
#define BHSM_OTPMSP_MARKET_ID0  313
#define BHSM_OTPMSP_MARKET_ID1  314
#define BHSM_OTPMSP_SYSTEM_EPOCH0  326
#define BHSM_OTPMSP_SYSTEM_EPOCH1  327
#define BHSM_OTPMSP_SYSTEM_EPOCH2  328
#define BHSM_OTPMSP_SYSTEM_EPOCH3  329
#define BHSM_OTPMSP_KEYLADDER_KEY_CONTRIBUTION_DISALLOW  334
#define BHSM_OTPMSP_KEYLADDER_KEY_CONTRIBUTION_FEATURE_DISABLE  335
#define BHSM_OTPMSP_CP_G_HASH_SOFTWARE_KEY_DISABLE  339
#define BHSM_OTPMSP_CP_CAM_SOFTWARE_KEY_DISABLE  340
#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_ENABLE  342
#define BHSM_OTPMSP_MSP_PROG_DISABLE  344
#define BHSM_OTPMSP_MSP_PROG_LOCK_ENABLE  345

#else
 #define BHSM_OTPMSP_PCIE0_HOST_PROTECT                           (7)
 #define BHSM_OTPMSP_PCIE0_CLIENT_PROTECT                         (8)
 #define BHSM_OTPMSP_PCIE0_MWIN_RESTRICT_ENABLE                 (135)
 #define BHSM_OTPMSP_PCIE0_MWIN_DISABLE                         (134)
 #define BHSM_OTPMSP_PCIE0_GWIN_DISABLE                         (133)
 #define BHSM_OTPMSP_PCIE0_MWIN_SIZE_ENFORCE_ENABLE             (621)
 #define BHSM_OTPMSP_PCIE1_HOST_PROTECT                           (9)
 #define BHSM_OTPMSP_PCIE1_CLIENT_PROTECT                        (10)
 #define BHSM_OTPMSP_PCIE1_MWIN_RESTRICT_ENABLE                 (137)
 #define BHSM_OTPMSP_PCIE1_MWIN_DISABLE                         (136)
 #define BHSM_OTPMSP_PCIE1_GWIN_DISABLE                         (133)
 #define BHSM_OTPMSP_PCIE1_MWIN_SIZE_ENFORCE_ENABLE             (622)
 #define BHSM_OTPMSP_CR_LOCK_ENABLE                               (5)
 #define BHSM_OTPMSP_ASYMMETRIC_CR_LOCK_ENABLE                  (238)
 #define BHSM_OTPMSP_FORCE_DRAM_SCRAMBLER                        (18)
 #define BHSM_OTPMSP_DATA_SECTION1_LOCK                         (529)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_A                  (37)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_B                  (38)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_C                  (39)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_D                  (40)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_E                  (41)
 #define BHSM_OTPMSP_DESTINATION_DISALLOW_KEY_F                  (42)
 #define BHSM_OTPMSP_PUBLIC_KEY0_INDEX                           (73)
 #define BHSM_OTPMSP_SECURE_BOOT_ENABLE                          (74)
 #define BHSM_OTPMSP_RAAGA_A_VERIFY_ENABLE                       (76)
 #define BHSM_OTPMSP_RAAGA_B_VERIFY_ENABLE                      (193)
 #define BHSM_OTPMSP_VIDEO_VERIFY_ENABLE                         (77)
 #define BHSM_OTPMSP_VICE_VERIFY_ENABLE                         (131)
 /*#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_KEY_SRC*/
 #define BHSM_OTPMSP_BOOT_CODE_HIDE_ENABLE                       (79)
 /*#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_KEY_INDEX*/
 #define BHSM_OTPMSP_ASKM_STB_OWNER_ID                           (43)
 #define BHSM_OTPMSP_CP_CONTROL_BITS                             (46)
 #define BHSM_OTPMSP_CA_CONTROL_BITS                             (45)
 #define BHSM_OTPMSP_FORCE_SC_DISALLOW                          (620)
 #define BHSM_OTPMSP_EBI_CS_SWAP_DISABLE                        (156)
 #define BHSM_OTPMSP_CA_KEY_LADDER_DISABLE                       (47)
 #define BHSM_OTPMSP_CP_KEY_LADDER_DISABLE                       (48)
 /*#define BHSM_OTPMSP_GLOBAL_KEY_TO_CA_KEY_LADDER_DISABLE*/
 /*#define BHSM_OTPMSP_GLOBAL_KEY_TO_CP_KEY_LADDER_DISABLE*/
 #define BHSM_OTPMSP_TWO_STAGE_CA_KEY_LADDER_DISABLE            (163)
 #define BHSM_OTPMSP_TWO_STAGE_CP_KEY_LADDER_DISABLE            (164)
 /*#define BHSM_OTPMSP_CA_CPD_DES_TDES_CBC_SOFTWARE_KEY_DISABLE*/
 #define BHSM_OTPMSP_CA_SOFTWARE_KEY_DISABLE                     (59)
 #define BHSM_OTPMSP_CA_DVB_CSA3_SOFTWARE_KEY_DISABLE           (245)
 #define BHSM_OTPMSP_CP_MULTI2_ECB_CBC_SOFTWARE_KEY_DISABLE     (146)
 #define BHSM_OTPMSP_RPMB_HOST_REPLAY_KEY_LADDER_DISABLE        (640)
 #define BHSM_OTPMSP_RPMB_USE_KEY_MOD                           (641)
 #define BHSM_OTPMSP_RPMB_OUT_KEY_MOD                           (642)
 /*#define BHSM_OTPMSP_CP_DES_SOFTWARE_KEY_DISABLE*/
 /*#define BHSM_OTPMSP_CP_TDES_SOFTWARE_KEY_DISABLE*/
 #define BHSM_OTPMSP_CP_AES_SOFTWARE_KEY_DISABLE                 (63)
 #define BHSM_OTPMSP_CP_AES_COUNTER_SOFTWARE_KEY_DISABLE         (64)
 #define BHSM_OTPMSP_IP_LICENSING_CHECK_ENABLE                   (70)
 #define BHSM_OTPMSP_MARKET_ID0                                  (94)
 #define BHSM_OTPMSP_MARKET_ID1                                 (633)
 #define BHSM_OTPMSP_SYSTEM_EPOCH0                               (87)
 #define BHSM_OTPMSP_SYSTEM_EPOCH1                              (520)
 #define BHSM_OTPMSP_SYSTEM_EPOCH2                              (519)
 #define BHSM_OTPMSP_SYSTEM_EPOCH3                              (518)
 #define BHSM_OTPMSP_KEYLADDER_KEY_CONTRIBUTION_DISALLOW        (623)
 #define BHSM_OTPMSP_KEYLADDER_KEY_CONTRIBUTION_FEATURE_DISABLE (624)
 /*#define BHSM_OTPMSP_CP_G_HASH_SOFTWARE_KEY_DISABLE*/
 /*#define BHSM_OTPMSP_CP_CAM_SOFTWARE_KEY_DISABLE*/
 /*#define BHSM_OTPMSP_BOOT_CODE_DECRYPT_ENABLE*/
 /*#define BHSM_OTPMSP_MSP_PROG_DISABLE*/
 /*#define BHSM_OTPMSP_MSP_PROG_LOCK_ENABLE*/
#endif


#ifdef __cplusplus
}
#endif
#endif
