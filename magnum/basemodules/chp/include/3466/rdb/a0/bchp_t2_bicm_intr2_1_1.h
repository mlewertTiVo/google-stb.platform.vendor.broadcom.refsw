/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Tue May  3 15:23:06 2016
 *                 Full Compile MD5 Checksum  1da1c3226a9ac5d06cc27c06c1aefe52
 *                     (minus title and desc)
 *                 MD5 Checksum               a58ec1309d39852a0c8fb54050da5f77
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     930
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_T2_BICM_INTR2_1_1_H__
#define BCHP_T2_BICM_INTR2_1_1_H__

/***************************************************************************
 *T2_BICM_INTR2_1_1 - BICM L2 Interrupt Controller Registers - Set 1
 ***************************************************************************/
#define BCHP_T2_BICM_INTR2_1_1_CPU_STATUS        0x04500300 /* [RO] CPU interrupt Status Register */
#define BCHP_T2_BICM_INTR2_1_1_CPU_SET           0x04500304 /* [WO] CPU interrupt Set Register */
#define BCHP_T2_BICM_INTR2_1_1_CPU_CLEAR         0x04500308 /* [WO] CPU interrupt Clear Register */
#define BCHP_T2_BICM_INTR2_1_1_CPU_MASK_STATUS   0x0450030c /* [RO] CPU interrupt Mask Status Register */
#define BCHP_T2_BICM_INTR2_1_1_CPU_MASK_SET      0x04500310 /* [WO] CPU interrupt Mask Set Register */
#define BCHP_T2_BICM_INTR2_1_1_CPU_MASK_CLEAR    0x04500314 /* [WO] CPU interrupt Mask Clear Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_STATUS        0x04500318 /* [RO] PCI interrupt Status Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_SET           0x0450031c /* [WO] PCI interrupt Set Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_CLEAR         0x04500320 /* [WO] PCI interrupt Clear Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_MASK_STATUS   0x04500324 /* [RO] PCI interrupt Mask Status Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_MASK_SET      0x04500328 /* [WO] PCI interrupt Mask Set Register */
#define BCHP_T2_BICM_INTR2_1_1_PCI_MASK_CLEAR    0x0450032c /* [WO] PCI interrupt Mask Clear Register */

#endif /* #ifndef BCHP_T2_BICM_INTR2_1_1_H__ */

/* End of File */
