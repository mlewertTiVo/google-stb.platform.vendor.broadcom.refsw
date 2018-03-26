/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Tue Apr 11 09:34:33 2017
 *                 Full Compile MD5 Checksum  4272585b45aedafcc6f8659ae1dfafd4
 *                     (minus title and desc)
 *                 MD5 Checksum               06edb082b66e1e47fa3a887d52c34fb5
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1395
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL tools/dvtsw/current/Linux/combo_header.pl
 *
 *
********************************************************************************/

#ifndef BCHP_BVNF_INTR2_5_H__
#define BCHP_BVNF_INTR2_5_H__

/***************************************************************************
 *BVNF_INTR2_5 - BVN Front Interrupt Controller 5 (BVN Errors INTR to CPU) Registers
 ***************************************************************************/
#define BCHP_BVNF_INTR2_5_R5F_STATUS             0x20606500 /* [RO][32] R5f interrupt Status Register */
#define BCHP_BVNF_INTR2_5_R5F_SET                0x20606504 /* [WO][32] R5f interrupt Set Register */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR              0x20606508 /* [WO][32] R5f interrupt Clear Register */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS        0x2060650c /* [RO][32] R5f interrupt Mask Status Register */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET           0x20606510 /* [WO][32] R5f interrupt Mask Set Register */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR         0x20606514 /* [WO][32] R5f interrupt Mask Clear Register */
#define BCHP_BVNF_INTR2_5_PCI_STATUS             0x20606518 /* [RO][32] PCI interrupt Status Register */
#define BCHP_BVNF_INTR2_5_PCI_SET                0x2060651c /* [WO][32] PCI interrupt Set Register */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR              0x20606520 /* [WO][32] PCI interrupt Clear Register */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS        0x20606524 /* [RO][32] PCI interrupt Mask Status Register */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET           0x20606528 /* [WO][32] PCI interrupt Mask Set Register */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR         0x2060652c /* [WO][32] PCI interrupt Mask Clear Register */

/***************************************************************************
 *R5F_STATUS - R5f interrupt Status Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_STATUS :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RGR_ERR_INTR_MASK             0x80000000
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RGR_ERR_INTR_SHIFT            31
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RGR_ERR_INTR_DEFAULT          0x00000000

/* BVNF_INTR2_5 :: R5F_STATUS :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_WATCHDOG_TIMER_INTR_MASK  0x40000000
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000000

/* BVNF_INTR2_5 :: R5F_STATUS :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_ERR_INTR_MASK             0x20000000
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_ERR_INTR_SHIFT            29
#define BCHP_BVNF_INTR2_5_R5F_STATUS_RDC_ERR_INTR_DEFAULT          0x00000000

/* BVNF_INTR2_5 :: R5F_STATUS :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_BVN_SEC_GR_ERR_INTR_MASK      0x10000000
#define BCHP_BVNF_INTR2_5_R5F_STATUS_BVN_SEC_GR_ERR_INTR_SHIFT     28
#define BCHP_BVNF_INTR2_5_R5F_STATUS_BVN_SEC_GR_ERR_INTR_DEFAULT   0x00000000

/* BVNF_INTR2_5 :: R5F_STATUS :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_reserved0_MASK                0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_STATUS_reserved0_SHIFT               5

/* BVNF_INTR2_5 :: R5F_STATUS :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_VFD_0_INTR_MASK               0x00000010
#define BCHP_BVNF_INTR2_5_R5F_STATUS_VFD_0_INTR_SHIFT              4
#define BCHP_BVNF_INTR2_5_R5F_STATUS_VFD_0_INTR_DEFAULT            0x00000000

/* BVNF_INTR2_5 :: R5F_STATUS :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_reserved1_MASK                0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_STATUS_reserved1_SHIFT               1

/* BVNF_INTR2_5 :: R5F_STATUS :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_STATUS_MFD_0_INTR_MASK               0x00000001
#define BCHP_BVNF_INTR2_5_R5F_STATUS_MFD_0_INTR_SHIFT              0
#define BCHP_BVNF_INTR2_5_R5F_STATUS_MFD_0_INTR_DEFAULT            0x00000000

/***************************************************************************
 *R5F_SET - R5f interrupt Set Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_SET :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_SET_RGR_ERR_INTR_MASK                0x80000000
#define BCHP_BVNF_INTR2_5_R5F_SET_RGR_ERR_INTR_SHIFT               31
#define BCHP_BVNF_INTR2_5_R5F_SET_RGR_ERR_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: R5F_SET :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_WATCHDOG_TIMER_INTR_MASK     0x40000000
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_WATCHDOG_TIMER_INTR_SHIFT    30
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_WATCHDOG_TIMER_INTR_DEFAULT  0x00000000

/* BVNF_INTR2_5 :: R5F_SET :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_ERR_INTR_MASK                0x20000000
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_ERR_INTR_SHIFT               29
#define BCHP_BVNF_INTR2_5_R5F_SET_RDC_ERR_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: R5F_SET :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_SET_BVN_SEC_GR_ERR_INTR_MASK         0x10000000
#define BCHP_BVNF_INTR2_5_R5F_SET_BVN_SEC_GR_ERR_INTR_SHIFT        28
#define BCHP_BVNF_INTR2_5_R5F_SET_BVN_SEC_GR_ERR_INTR_DEFAULT      0x00000000

/* BVNF_INTR2_5 :: R5F_SET :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_SET_reserved0_MASK                   0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_SET_reserved0_SHIFT                  5

/* BVNF_INTR2_5 :: R5F_SET :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_SET_VFD_0_INTR_MASK                  0x00000010
#define BCHP_BVNF_INTR2_5_R5F_SET_VFD_0_INTR_SHIFT                 4
#define BCHP_BVNF_INTR2_5_R5F_SET_VFD_0_INTR_DEFAULT               0x00000000

/* BVNF_INTR2_5 :: R5F_SET :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_SET_reserved1_MASK                   0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_SET_reserved1_SHIFT                  1

/* BVNF_INTR2_5 :: R5F_SET :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_SET_MFD_0_INTR_MASK                  0x00000001
#define BCHP_BVNF_INTR2_5_R5F_SET_MFD_0_INTR_SHIFT                 0
#define BCHP_BVNF_INTR2_5_R5F_SET_MFD_0_INTR_DEFAULT               0x00000000

/***************************************************************************
 *R5F_CLEAR - R5f interrupt Clear Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_CLEAR :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RGR_ERR_INTR_MASK              0x80000000
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RGR_ERR_INTR_SHIFT             31
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RGR_ERR_INTR_DEFAULT           0x00000000

/* BVNF_INTR2_5 :: R5F_CLEAR :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_WATCHDOG_TIMER_INTR_MASK   0x40000000
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_WATCHDOG_TIMER_INTR_SHIFT  30
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000000

/* BVNF_INTR2_5 :: R5F_CLEAR :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_ERR_INTR_MASK              0x20000000
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_ERR_INTR_SHIFT             29
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_RDC_ERR_INTR_DEFAULT           0x00000000

/* BVNF_INTR2_5 :: R5F_CLEAR :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_BVN_SEC_GR_ERR_INTR_MASK       0x10000000
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_BVN_SEC_GR_ERR_INTR_SHIFT      28
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_BVN_SEC_GR_ERR_INTR_DEFAULT    0x00000000

/* BVNF_INTR2_5 :: R5F_CLEAR :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_reserved0_MASK                 0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_reserved0_SHIFT                5

/* BVNF_INTR2_5 :: R5F_CLEAR :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_VFD_0_INTR_MASK                0x00000010
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_VFD_0_INTR_SHIFT               4
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_VFD_0_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: R5F_CLEAR :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_reserved1_MASK                 0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_reserved1_SHIFT                1

/* BVNF_INTR2_5 :: R5F_CLEAR :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_MFD_0_INTR_MASK                0x00000001
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_MFD_0_INTR_SHIFT               0
#define BCHP_BVNF_INTR2_5_R5F_CLEAR_MFD_0_INTR_DEFAULT             0x00000000

/***************************************************************************
 *R5F_MASK_STATUS - R5f interrupt Mask Status Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RGR_ERR_INTR_MASK        0x80000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RGR_ERR_INTR_SHIFT       31
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RGR_ERR_INTR_DEFAULT     0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_ERR_INTR_MASK        0x20000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_ERR_INTR_SHIFT       29
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_RDC_ERR_INTR_DEFAULT     0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_BVN_SEC_GR_ERR_INTR_MASK 0x10000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_BVN_SEC_GR_ERR_INTR_SHIFT 28
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_reserved0_MASK           0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_reserved0_SHIFT          5

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_VFD_0_INTR_MASK          0x00000010
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_VFD_0_INTR_SHIFT         4
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_VFD_0_INTR_DEFAULT       0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_reserved1_MASK           0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_reserved1_SHIFT          1

/* BVNF_INTR2_5 :: R5F_MASK_STATUS :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_MFD_0_INTR_MASK          0x00000001
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_MFD_0_INTR_SHIFT         0
#define BCHP_BVNF_INTR2_5_R5F_MASK_STATUS_MFD_0_INTR_DEFAULT       0x00000001

/***************************************************************************
 *R5F_MASK_SET - R5f interrupt Mask Set Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_MASK_SET :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RGR_ERR_INTR_MASK           0x80000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RGR_ERR_INTR_SHIFT          31
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RGR_ERR_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_SET :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_SET :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_ERR_INTR_MASK           0x20000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_ERR_INTR_SHIFT          29
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_RDC_ERR_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_SET :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_BVN_SEC_GR_ERR_INTR_MASK    0x10000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_BVN_SEC_GR_ERR_INTR_SHIFT   28
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_SET :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_reserved0_MASK              0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_reserved0_SHIFT             5

/* BVNF_INTR2_5 :: R5F_MASK_SET :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_VFD_0_INTR_MASK             0x00000010
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_VFD_0_INTR_SHIFT            4
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_VFD_0_INTR_DEFAULT          0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_SET :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_reserved1_MASK              0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_reserved1_SHIFT             1

/* BVNF_INTR2_5 :: R5F_MASK_SET :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_MFD_0_INTR_MASK             0x00000001
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_MFD_0_INTR_SHIFT            0
#define BCHP_BVNF_INTR2_5_R5F_MASK_SET_MFD_0_INTR_DEFAULT          0x00000001

/***************************************************************************
 *R5F_MASK_CLEAR - R5f interrupt Mask Clear Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RGR_ERR_INTR_MASK         0x80000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RGR_ERR_INTR_SHIFT        31
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RGR_ERR_INTR_DEFAULT      0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_ERR_INTR_MASK         0x20000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_ERR_INTR_SHIFT        29
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_RDC_ERR_INTR_DEFAULT      0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_MASK  0x10000000
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_SHIFT 28
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_reserved0_MASK            0x0fffffe0
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_reserved0_SHIFT           5

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_VFD_0_INTR_MASK           0x00000010
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_VFD_0_INTR_SHIFT          4
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_VFD_0_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_reserved1_MASK            0x0000000e
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_reserved1_SHIFT           1

/* BVNF_INTR2_5 :: R5F_MASK_CLEAR :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_MFD_0_INTR_MASK           0x00000001
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_MFD_0_INTR_SHIFT          0
#define BCHP_BVNF_INTR2_5_R5F_MASK_CLEAR_MFD_0_INTR_DEFAULT        0x00000001

/***************************************************************************
 *PCI_STATUS - PCI interrupt Status Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_STATUS :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RGR_ERR_INTR_MASK             0x80000000
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RGR_ERR_INTR_SHIFT            31
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RGR_ERR_INTR_DEFAULT          0x00000000

/* BVNF_INTR2_5 :: PCI_STATUS :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_WATCHDOG_TIMER_INTR_MASK  0x40000000
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000000

/* BVNF_INTR2_5 :: PCI_STATUS :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_ERR_INTR_MASK             0x20000000
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_ERR_INTR_SHIFT            29
#define BCHP_BVNF_INTR2_5_PCI_STATUS_RDC_ERR_INTR_DEFAULT          0x00000000

/* BVNF_INTR2_5 :: PCI_STATUS :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_BVN_SEC_GR_ERR_INTR_MASK      0x10000000
#define BCHP_BVNF_INTR2_5_PCI_STATUS_BVN_SEC_GR_ERR_INTR_SHIFT     28
#define BCHP_BVNF_INTR2_5_PCI_STATUS_BVN_SEC_GR_ERR_INTR_DEFAULT   0x00000000

/* BVNF_INTR2_5 :: PCI_STATUS :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_reserved0_MASK                0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_STATUS_reserved0_SHIFT               5

/* BVNF_INTR2_5 :: PCI_STATUS :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_VFD_0_INTR_MASK               0x00000010
#define BCHP_BVNF_INTR2_5_PCI_STATUS_VFD_0_INTR_SHIFT              4
#define BCHP_BVNF_INTR2_5_PCI_STATUS_VFD_0_INTR_DEFAULT            0x00000000

/* BVNF_INTR2_5 :: PCI_STATUS :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_reserved1_MASK                0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_STATUS_reserved1_SHIFT               1

/* BVNF_INTR2_5 :: PCI_STATUS :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_STATUS_MFD_0_INTR_MASK               0x00000001
#define BCHP_BVNF_INTR2_5_PCI_STATUS_MFD_0_INTR_SHIFT              0
#define BCHP_BVNF_INTR2_5_PCI_STATUS_MFD_0_INTR_DEFAULT            0x00000000

/***************************************************************************
 *PCI_SET - PCI interrupt Set Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_SET :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_SET_RGR_ERR_INTR_MASK                0x80000000
#define BCHP_BVNF_INTR2_5_PCI_SET_RGR_ERR_INTR_SHIFT               31
#define BCHP_BVNF_INTR2_5_PCI_SET_RGR_ERR_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: PCI_SET :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_WATCHDOG_TIMER_INTR_MASK     0x40000000
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_WATCHDOG_TIMER_INTR_SHIFT    30
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_WATCHDOG_TIMER_INTR_DEFAULT  0x00000000

/* BVNF_INTR2_5 :: PCI_SET :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_ERR_INTR_MASK                0x20000000
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_ERR_INTR_SHIFT               29
#define BCHP_BVNF_INTR2_5_PCI_SET_RDC_ERR_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: PCI_SET :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_SET_BVN_SEC_GR_ERR_INTR_MASK         0x10000000
#define BCHP_BVNF_INTR2_5_PCI_SET_BVN_SEC_GR_ERR_INTR_SHIFT        28
#define BCHP_BVNF_INTR2_5_PCI_SET_BVN_SEC_GR_ERR_INTR_DEFAULT      0x00000000

/* BVNF_INTR2_5 :: PCI_SET :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_SET_reserved0_MASK                   0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_SET_reserved0_SHIFT                  5

/* BVNF_INTR2_5 :: PCI_SET :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_SET_VFD_0_INTR_MASK                  0x00000010
#define BCHP_BVNF_INTR2_5_PCI_SET_VFD_0_INTR_SHIFT                 4
#define BCHP_BVNF_INTR2_5_PCI_SET_VFD_0_INTR_DEFAULT               0x00000000

/* BVNF_INTR2_5 :: PCI_SET :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_SET_reserved1_MASK                   0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_SET_reserved1_SHIFT                  1

/* BVNF_INTR2_5 :: PCI_SET :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_SET_MFD_0_INTR_MASK                  0x00000001
#define BCHP_BVNF_INTR2_5_PCI_SET_MFD_0_INTR_SHIFT                 0
#define BCHP_BVNF_INTR2_5_PCI_SET_MFD_0_INTR_DEFAULT               0x00000000

/***************************************************************************
 *PCI_CLEAR - PCI interrupt Clear Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_CLEAR :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RGR_ERR_INTR_MASK              0x80000000
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RGR_ERR_INTR_SHIFT             31
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RGR_ERR_INTR_DEFAULT           0x00000000

/* BVNF_INTR2_5 :: PCI_CLEAR :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_WATCHDOG_TIMER_INTR_MASK   0x40000000
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_WATCHDOG_TIMER_INTR_SHIFT  30
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000000

/* BVNF_INTR2_5 :: PCI_CLEAR :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_ERR_INTR_MASK              0x20000000
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_ERR_INTR_SHIFT             29
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_RDC_ERR_INTR_DEFAULT           0x00000000

/* BVNF_INTR2_5 :: PCI_CLEAR :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_BVN_SEC_GR_ERR_INTR_MASK       0x10000000
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_BVN_SEC_GR_ERR_INTR_SHIFT      28
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_BVN_SEC_GR_ERR_INTR_DEFAULT    0x00000000

/* BVNF_INTR2_5 :: PCI_CLEAR :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_reserved0_MASK                 0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_reserved0_SHIFT                5

/* BVNF_INTR2_5 :: PCI_CLEAR :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_VFD_0_INTR_MASK                0x00000010
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_VFD_0_INTR_SHIFT               4
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_VFD_0_INTR_DEFAULT             0x00000000

/* BVNF_INTR2_5 :: PCI_CLEAR :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_reserved1_MASK                 0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_reserved1_SHIFT                1

/* BVNF_INTR2_5 :: PCI_CLEAR :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_MFD_0_INTR_MASK                0x00000001
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_MFD_0_INTR_SHIFT               0
#define BCHP_BVNF_INTR2_5_PCI_CLEAR_MFD_0_INTR_DEFAULT             0x00000000

/***************************************************************************
 *PCI_MASK_STATUS - PCI interrupt Mask Status Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RGR_ERR_INTR_MASK        0x80000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RGR_ERR_INTR_SHIFT       31
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RGR_ERR_INTR_DEFAULT     0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_ERR_INTR_MASK        0x20000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_ERR_INTR_SHIFT       29
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_RDC_ERR_INTR_DEFAULT     0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_BVN_SEC_GR_ERR_INTR_MASK 0x10000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_BVN_SEC_GR_ERR_INTR_SHIFT 28
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_reserved0_MASK           0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_reserved0_SHIFT          5

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_VFD_0_INTR_MASK          0x00000010
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_VFD_0_INTR_SHIFT         4
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_VFD_0_INTR_DEFAULT       0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_reserved1_MASK           0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_reserved1_SHIFT          1

/* BVNF_INTR2_5 :: PCI_MASK_STATUS :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_MFD_0_INTR_MASK          0x00000001
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_MFD_0_INTR_SHIFT         0
#define BCHP_BVNF_INTR2_5_PCI_MASK_STATUS_MFD_0_INTR_DEFAULT       0x00000001

/***************************************************************************
 *PCI_MASK_SET - PCI interrupt Mask Set Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_MASK_SET :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RGR_ERR_INTR_MASK           0x80000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RGR_ERR_INTR_SHIFT          31
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RGR_ERR_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_SET :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_SET :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_ERR_INTR_MASK           0x20000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_ERR_INTR_SHIFT          29
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_RDC_ERR_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_SET :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_BVN_SEC_GR_ERR_INTR_MASK    0x10000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_BVN_SEC_GR_ERR_INTR_SHIFT   28
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_SET :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_reserved0_MASK              0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_reserved0_SHIFT             5

/* BVNF_INTR2_5 :: PCI_MASK_SET :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_VFD_0_INTR_MASK             0x00000010
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_VFD_0_INTR_SHIFT            4
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_VFD_0_INTR_DEFAULT          0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_SET :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_reserved1_MASK              0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_reserved1_SHIFT             1

/* BVNF_INTR2_5 :: PCI_MASK_SET :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_MFD_0_INTR_MASK             0x00000001
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_MFD_0_INTR_SHIFT            0
#define BCHP_BVNF_INTR2_5_PCI_MASK_SET_MFD_0_INTR_DEFAULT          0x00000001

/***************************************************************************
 *PCI_MASK_CLEAR - PCI interrupt Mask Clear Register
 ***************************************************************************/
/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: RGR_ERR_INTR [31:31] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RGR_ERR_INTR_MASK         0x80000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RGR_ERR_INTR_SHIFT        31
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RGR_ERR_INTR_DEFAULT      0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: RDC_WATCHDOG_TIMER_INTR [30:30] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_MASK 0x40000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_SHIFT 30
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_WATCHDOG_TIMER_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: RDC_ERR_INTR [29:29] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_ERR_INTR_MASK         0x20000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_ERR_INTR_SHIFT        29
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_RDC_ERR_INTR_DEFAULT      0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: BVN_SEC_GR_ERR_INTR [28:28] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_MASK  0x10000000
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_SHIFT 28
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_BVN_SEC_GR_ERR_INTR_DEFAULT 0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: reserved0 [27:05] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_reserved0_MASK            0x0fffffe0
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_reserved0_SHIFT           5

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: VFD_0_INTR [04:04] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_VFD_0_INTR_MASK           0x00000010
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_VFD_0_INTR_SHIFT          4
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_VFD_0_INTR_DEFAULT        0x00000001

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: reserved1 [03:01] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_reserved1_MASK            0x0000000e
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_reserved1_SHIFT           1

/* BVNF_INTR2_5 :: PCI_MASK_CLEAR :: MFD_0_INTR [00:00] */
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_MFD_0_INTR_MASK           0x00000001
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_MFD_0_INTR_SHIFT          0
#define BCHP_BVNF_INTR2_5_PCI_MASK_CLEAR_MFD_0_INTR_DEFAULT        0x00000001

#endif /* #ifndef BCHP_BVNF_INTR2_5_H__ */

/* End of File */
