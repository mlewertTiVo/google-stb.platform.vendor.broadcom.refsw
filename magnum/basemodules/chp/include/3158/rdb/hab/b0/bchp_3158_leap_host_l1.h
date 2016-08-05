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
 * Date:           Generated on               Tue Feb 23 15:26:06 2016
 *                 Full Compile MD5 Checksum  4b84f30a4b3665aac5b824a1ed76e56c
 *                     (minus title and desc)
 *                 MD5 Checksum               4894bba0ec078aee10b5b5954262d56e
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     804
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_LEAP_HOST_L1_H__
#define BCHP_LEAP_HOST_L1_H__

/***************************************************************************
 *LEAP_HOST_L1 - Host L1 Interrupt Controller Registers
 ***************************************************************************/
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS         0x00100600 /* [RO] Interrupt Status Register */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS    0x00100610 /* [RO] Interrupt Mask Status Register */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET       0x00100620 /* [WO] Interrupt Mask Set Register */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR     0x00100630 /* [WO] Interrupt Mask Clear Register */

/***************************************************************************
 *INTR_W0_STATUS - Interrupt Status Register
 ***************************************************************************/
/* LEAP_HOST_L1 :: INTR_W0_STATUS :: reserved0 [31:23] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_reserved0_MASK            0xff800000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_reserved0_SHIFT           23

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: PERIPH_DEMOD_XPT_WAKEUP_INTR [22:22] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_MASK 0x00400000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_SHIFT 22
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: PERIPH_DEMODXPT_INTR [21:21] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMODXPT_INTR_MASK 0x00200000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMODXPT_INTR_SHIFT 21
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_DEMODXPT_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: PERIPH_TOP_PER_IRQ [20:20] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_PER_IRQ_MASK   0x00100000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_PER_IRQ_SHIFT  20
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_PER_IRQ_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A15_INTR_CPU_DS [19:19] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A15_INTR_CPU_DS_MASK 0x00080000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A15_INTR_CPU_DS_SHIFT 19
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A15_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A14_INTR_CPU_DS [18:18] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A14_INTR_CPU_DS_MASK 0x00040000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A14_INTR_CPU_DS_SHIFT 18
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A14_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A13_INTR_CPU_DS [17:17] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A13_INTR_CPU_DS_MASK 0x00020000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A13_INTR_CPU_DS_SHIFT 17
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A13_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A12_INTR_CPU_DS [16:16] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A12_INTR_CPU_DS_MASK 0x00010000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A12_INTR_CPU_DS_SHIFT 16
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A12_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A11_INTR_CPU_DS [15:15] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A11_INTR_CPU_DS_MASK 0x00008000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A11_INTR_CPU_DS_SHIFT 15
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A11_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A10_INTR_CPU_DS [14:14] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A10_INTR_CPU_DS_MASK 0x00004000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A10_INTR_CPU_DS_SHIFT 14
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A10_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A9_INTR_CPU_DS [13:13] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A9_INTR_CPU_DS_MASK 0x00002000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A9_INTR_CPU_DS_SHIFT 13
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A9_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A8_INTR_CPU_DS [12:12] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A8_INTR_CPU_DS_MASK 0x00001000
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A8_INTR_CPU_DS_SHIFT 12
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A8_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A7_INTR_CPU_DS [11:11] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A7_INTR_CPU_DS_MASK 0x00000800
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A7_INTR_CPU_DS_SHIFT 11
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A7_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A6_INTR_CPU_DS [10:10] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A6_INTR_CPU_DS_MASK 0x00000400
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A6_INTR_CPU_DS_SHIFT 10
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A6_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A5_INTR_CPU_DS [09:09] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A5_INTR_CPU_DS_MASK 0x00000200
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A5_INTR_CPU_DS_SHIFT 9
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A5_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A4_INTR_CPU_DS [08:08] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A4_INTR_CPU_DS_MASK 0x00000100
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A4_INTR_CPU_DS_SHIFT 8
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A4_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A3_INTR_CPU_DS [07:07] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A3_INTR_CPU_DS_MASK 0x00000080
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A3_INTR_CPU_DS_SHIFT 7
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A3_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A2_INTR_CPU_DS [06:06] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A2_INTR_CPU_DS_MASK 0x00000040
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A2_INTR_CPU_DS_SHIFT 6
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A2_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A1_INTR_CPU_DS [05:05] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A1_INTR_CPU_DS_MASK 0x00000020
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A1_INTR_CPU_DS_SHIFT 5
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A1_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DSTOP_A0_INTR_CPU_DS [04:04] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A0_INTR_CPU_DS_MASK 0x00000010
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A0_INTR_CPU_DS_SHIFT 4
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DSTOP_A0_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: PERIPH_TOP_AVS_INTR [03:03] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_AVS_INTR_MASK  0x00000008
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_AVS_INTR_SHIFT 3
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_PERIPH_TOP_AVS_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: DS_TOPA_INTR_OOB [02:02] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DS_TOPA_INTR_OOB_MASK     0x00000004
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DS_TOPA_INTR_OOB_SHIFT    2
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_DS_TOPA_INTR_OOB_DEFAULT  0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: AIF_CPU_INTR [01:01] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_AIF_CPU_INTR_MASK         0x00000002
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_AIF_CPU_INTR_SHIFT        1
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_AIF_CPU_INTR_DEFAULT      0x00000000

/* LEAP_HOST_L1 :: INTR_W0_STATUS :: LEAP_INTR [00:00] */
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_LEAP_INTR_MASK            0x00000001
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_LEAP_INTR_SHIFT           0
#define BCHP_LEAP_HOST_L1_INTR_W0_STATUS_LEAP_INTR_DEFAULT         0x00000000

/***************************************************************************
 *INTR_W0_MASK_STATUS - Interrupt Mask Status Register
 ***************************************************************************/
/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: reserved0 [31:23] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_reserved0_MASK       0xff800000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_reserved0_SHIFT      23

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: PERIPH_DEMOD_XPT_WAKEUP_INTR [22:22] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_MASK 0x00400000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_SHIFT 22
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMOD_XPT_WAKEUP_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: PERIPH_DEMODXPT_INTR [21:21] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMODXPT_INTR_MASK 0x00200000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMODXPT_INTR_SHIFT 21
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_DEMODXPT_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: PERIPH_TOP_PER_IRQ [20:20] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_PER_IRQ_MASK 0x00100000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_PER_IRQ_SHIFT 20
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_PER_IRQ_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A15_INTR_CPU_DS [19:19] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A15_INTR_CPU_DS_MASK 0x00080000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A15_INTR_CPU_DS_SHIFT 19
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A15_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A14_INTR_CPU_DS [18:18] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A14_INTR_CPU_DS_MASK 0x00040000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A14_INTR_CPU_DS_SHIFT 18
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A14_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A13_INTR_CPU_DS [17:17] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A13_INTR_CPU_DS_MASK 0x00020000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A13_INTR_CPU_DS_SHIFT 17
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A13_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A12_INTR_CPU_DS [16:16] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A12_INTR_CPU_DS_MASK 0x00010000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A12_INTR_CPU_DS_SHIFT 16
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A12_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A11_INTR_CPU_DS [15:15] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A11_INTR_CPU_DS_MASK 0x00008000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A11_INTR_CPU_DS_SHIFT 15
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A11_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A10_INTR_CPU_DS [14:14] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A10_INTR_CPU_DS_MASK 0x00004000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A10_INTR_CPU_DS_SHIFT 14
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A10_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A9_INTR_CPU_DS [13:13] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A9_INTR_CPU_DS_MASK 0x00002000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A9_INTR_CPU_DS_SHIFT 13
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A9_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A8_INTR_CPU_DS [12:12] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A8_INTR_CPU_DS_MASK 0x00001000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A8_INTR_CPU_DS_SHIFT 12
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A8_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A7_INTR_CPU_DS [11:11] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A7_INTR_CPU_DS_MASK 0x00000800
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A7_INTR_CPU_DS_SHIFT 11
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A7_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A6_INTR_CPU_DS [10:10] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A6_INTR_CPU_DS_MASK 0x00000400
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A6_INTR_CPU_DS_SHIFT 10
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A6_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A5_INTR_CPU_DS [09:09] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A5_INTR_CPU_DS_MASK 0x00000200
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A5_INTR_CPU_DS_SHIFT 9
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A5_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A4_INTR_CPU_DS [08:08] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A4_INTR_CPU_DS_MASK 0x00000100
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A4_INTR_CPU_DS_SHIFT 8
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A4_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A3_INTR_CPU_DS [07:07] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A3_INTR_CPU_DS_MASK 0x00000080
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A3_INTR_CPU_DS_SHIFT 7
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A3_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A2_INTR_CPU_DS [06:06] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A2_INTR_CPU_DS_MASK 0x00000040
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A2_INTR_CPU_DS_SHIFT 6
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A2_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A1_INTR_CPU_DS [05:05] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A1_INTR_CPU_DS_MASK 0x00000020
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A1_INTR_CPU_DS_SHIFT 5
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A1_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DSTOP_A0_INTR_CPU_DS [04:04] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A0_INTR_CPU_DS_MASK 0x00000010
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A0_INTR_CPU_DS_SHIFT 4
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DSTOP_A0_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: PERIPH_TOP_AVS_INTR [03:03] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_AVS_INTR_MASK 0x00000008
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_AVS_INTR_SHIFT 3
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_PERIPH_TOP_AVS_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: DS_TOPA_INTR_OOB [02:02] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DS_TOPA_INTR_OOB_MASK 0x00000004
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DS_TOPA_INTR_OOB_SHIFT 2
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_DS_TOPA_INTR_OOB_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: AIF_CPU_INTR [01:01] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_AIF_CPU_INTR_MASK    0x00000002
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_AIF_CPU_INTR_SHIFT   1
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_AIF_CPU_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_STATUS :: LEAP_INTR [00:00] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_LEAP_INTR_MASK       0x00000001
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_LEAP_INTR_SHIFT      0
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_STATUS_LEAP_INTR_DEFAULT    0x00000000

/***************************************************************************
 *INTR_W0_MASK_SET - Interrupt Mask Set Register
 ***************************************************************************/
/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: reserved0 [31:23] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_reserved0_MASK          0xff800000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_reserved0_SHIFT         23

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: PERIPH_DEMOD_XPT_WAKEUP_INTR [22:22] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMOD_XPT_WAKEUP_INTR_MASK 0x00400000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMOD_XPT_WAKEUP_INTR_SHIFT 22
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMOD_XPT_WAKEUP_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: PERIPH_DEMODXPT_INTR [21:21] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMODXPT_INTR_MASK 0x00200000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMODXPT_INTR_SHIFT 21
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_DEMODXPT_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: PERIPH_TOP_PER_IRQ [20:20] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_PER_IRQ_MASK 0x00100000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_PER_IRQ_SHIFT 20
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_PER_IRQ_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A15_INTR_CPU_DS [19:19] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A15_INTR_CPU_DS_MASK 0x00080000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A15_INTR_CPU_DS_SHIFT 19
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A15_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A14_INTR_CPU_DS [18:18] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A14_INTR_CPU_DS_MASK 0x00040000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A14_INTR_CPU_DS_SHIFT 18
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A14_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A13_INTR_CPU_DS [17:17] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A13_INTR_CPU_DS_MASK 0x00020000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A13_INTR_CPU_DS_SHIFT 17
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A13_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A12_INTR_CPU_DS [16:16] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A12_INTR_CPU_DS_MASK 0x00010000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A12_INTR_CPU_DS_SHIFT 16
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A12_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A11_INTR_CPU_DS [15:15] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A11_INTR_CPU_DS_MASK 0x00008000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A11_INTR_CPU_DS_SHIFT 15
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A11_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A10_INTR_CPU_DS [14:14] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A10_INTR_CPU_DS_MASK 0x00004000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A10_INTR_CPU_DS_SHIFT 14
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A10_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A9_INTR_CPU_DS [13:13] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A9_INTR_CPU_DS_MASK 0x00002000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A9_INTR_CPU_DS_SHIFT 13
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A9_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A8_INTR_CPU_DS [12:12] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A8_INTR_CPU_DS_MASK 0x00001000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A8_INTR_CPU_DS_SHIFT 12
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A8_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A7_INTR_CPU_DS [11:11] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A7_INTR_CPU_DS_MASK 0x00000800
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A7_INTR_CPU_DS_SHIFT 11
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A7_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A6_INTR_CPU_DS [10:10] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A6_INTR_CPU_DS_MASK 0x00000400
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A6_INTR_CPU_DS_SHIFT 10
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A6_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A5_INTR_CPU_DS [09:09] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A5_INTR_CPU_DS_MASK 0x00000200
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A5_INTR_CPU_DS_SHIFT 9
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A5_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A4_INTR_CPU_DS [08:08] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A4_INTR_CPU_DS_MASK 0x00000100
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A4_INTR_CPU_DS_SHIFT 8
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A4_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A3_INTR_CPU_DS [07:07] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A3_INTR_CPU_DS_MASK 0x00000080
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A3_INTR_CPU_DS_SHIFT 7
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A3_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A2_INTR_CPU_DS [06:06] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A2_INTR_CPU_DS_MASK 0x00000040
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A2_INTR_CPU_DS_SHIFT 6
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A2_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A1_INTR_CPU_DS [05:05] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A1_INTR_CPU_DS_MASK 0x00000020
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A1_INTR_CPU_DS_SHIFT 5
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A1_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DSTOP_A0_INTR_CPU_DS [04:04] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A0_INTR_CPU_DS_MASK 0x00000010
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A0_INTR_CPU_DS_SHIFT 4
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DSTOP_A0_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: PERIPH_TOP_AVS_INTR [03:03] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_AVS_INTR_MASK 0x00000008
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_AVS_INTR_SHIFT 3
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_PERIPH_TOP_AVS_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: DS_TOPA_INTR_OOB [02:02] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DS_TOPA_INTR_OOB_MASK   0x00000004
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DS_TOPA_INTR_OOB_SHIFT  2
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_DS_TOPA_INTR_OOB_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: AIF_CPU_INTR [01:01] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_AIF_CPU_INTR_MASK       0x00000002
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_AIF_CPU_INTR_SHIFT      1
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_AIF_CPU_INTR_DEFAULT    0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_SET :: LEAP_INTR [00:00] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_LEAP_INTR_MASK          0x00000001
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_LEAP_INTR_SHIFT         0
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_SET_LEAP_INTR_DEFAULT       0x00000000

/***************************************************************************
 *INTR_W0_MASK_CLEAR - Interrupt Mask Clear Register
 ***************************************************************************/
/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: reserved0 [31:23] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_reserved0_MASK        0xff800000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_reserved0_SHIFT       23

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: PERIPH_DEMOD_XPT_WAKEUP_INTR [22:22] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMOD_XPT_WAKEUP_INTR_MASK 0x00400000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMOD_XPT_WAKEUP_INTR_SHIFT 22
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMOD_XPT_WAKEUP_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: PERIPH_DEMODXPT_INTR [21:21] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMODXPT_INTR_MASK 0x00200000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMODXPT_INTR_SHIFT 21
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_DEMODXPT_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: PERIPH_TOP_PER_IRQ [20:20] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_PER_IRQ_MASK 0x00100000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_PER_IRQ_SHIFT 20
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_PER_IRQ_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A15_INTR_CPU_DS [19:19] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A15_INTR_CPU_DS_MASK 0x00080000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A15_INTR_CPU_DS_SHIFT 19
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A15_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A14_INTR_CPU_DS [18:18] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A14_INTR_CPU_DS_MASK 0x00040000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A14_INTR_CPU_DS_SHIFT 18
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A14_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A13_INTR_CPU_DS [17:17] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A13_INTR_CPU_DS_MASK 0x00020000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A13_INTR_CPU_DS_SHIFT 17
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A13_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A12_INTR_CPU_DS [16:16] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A12_INTR_CPU_DS_MASK 0x00010000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A12_INTR_CPU_DS_SHIFT 16
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A12_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A11_INTR_CPU_DS [15:15] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A11_INTR_CPU_DS_MASK 0x00008000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A11_INTR_CPU_DS_SHIFT 15
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A11_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A10_INTR_CPU_DS [14:14] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A10_INTR_CPU_DS_MASK 0x00004000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A10_INTR_CPU_DS_SHIFT 14
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A10_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A9_INTR_CPU_DS [13:13] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A9_INTR_CPU_DS_MASK 0x00002000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A9_INTR_CPU_DS_SHIFT 13
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A9_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A8_INTR_CPU_DS [12:12] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A8_INTR_CPU_DS_MASK 0x00001000
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A8_INTR_CPU_DS_SHIFT 12
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A8_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A7_INTR_CPU_DS [11:11] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A7_INTR_CPU_DS_MASK 0x00000800
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A7_INTR_CPU_DS_SHIFT 11
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A7_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A6_INTR_CPU_DS [10:10] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A6_INTR_CPU_DS_MASK 0x00000400
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A6_INTR_CPU_DS_SHIFT 10
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A6_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A5_INTR_CPU_DS [09:09] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A5_INTR_CPU_DS_MASK 0x00000200
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A5_INTR_CPU_DS_SHIFT 9
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A5_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A4_INTR_CPU_DS [08:08] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A4_INTR_CPU_DS_MASK 0x00000100
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A4_INTR_CPU_DS_SHIFT 8
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A4_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A3_INTR_CPU_DS [07:07] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A3_INTR_CPU_DS_MASK 0x00000080
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A3_INTR_CPU_DS_SHIFT 7
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A3_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A2_INTR_CPU_DS [06:06] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A2_INTR_CPU_DS_MASK 0x00000040
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A2_INTR_CPU_DS_SHIFT 6
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A2_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A1_INTR_CPU_DS [05:05] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A1_INTR_CPU_DS_MASK 0x00000020
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A1_INTR_CPU_DS_SHIFT 5
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A1_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DSTOP_A0_INTR_CPU_DS [04:04] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A0_INTR_CPU_DS_MASK 0x00000010
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A0_INTR_CPU_DS_SHIFT 4
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DSTOP_A0_INTR_CPU_DS_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: PERIPH_TOP_AVS_INTR [03:03] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_AVS_INTR_MASK 0x00000008
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_AVS_INTR_SHIFT 3
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_PERIPH_TOP_AVS_INTR_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: DS_TOPA_INTR_OOB [02:02] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DS_TOPA_INTR_OOB_MASK 0x00000004
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DS_TOPA_INTR_OOB_SHIFT 2
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_DS_TOPA_INTR_OOB_DEFAULT 0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: AIF_CPU_INTR [01:01] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_AIF_CPU_INTR_MASK     0x00000002
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_AIF_CPU_INTR_SHIFT    1
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_AIF_CPU_INTR_DEFAULT  0x00000000

/* LEAP_HOST_L1 :: INTR_W0_MASK_CLEAR :: LEAP_INTR [00:00] */
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_LEAP_INTR_MASK        0x00000001
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_LEAP_INTR_SHIFT       0
#define BCHP_LEAP_HOST_L1_INTR_W0_MASK_CLEAR_LEAP_INTR_DEFAULT     0x00000000

#endif /* #ifndef BCHP_LEAP_HOST_L1_H__ */

/* End of File */
