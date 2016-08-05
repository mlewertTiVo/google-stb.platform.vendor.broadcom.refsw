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
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Thu Jun 23 18:55:21 2016
 *                 Full Compile MD5 Checksum  726d365438fe88693b6f0a492958face
 *                     (minus title and desc)
 *                 MD5 Checksum               942083b46396c70655d90e546c008796
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1048
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_DS_A_MICRO_H__
#define BCHP_DS_A_MICRO_H__

/***************************************************************************
 *DS_A_MICRO - Downstream Micro clock domain Registers
 ***************************************************************************/
#define BCHP_DS_A_MICRO_README                   0x04300000 /* [RW] DS Top SPARE Register */
#define BCHP_DS_A_MICRO_REVID                    0x04300004 /* [RO] Revision ID Register */
#define BCHP_DS_A_MICRO_STATUS                   0x04300008 /* [RO] Status */
#define BCHP_DS_A_MICRO_SW_SPARE1                0x0430000c /* [CFG] Reserved for software use */
#define BCHP_DS_A_MICRO_SW_SPARE2                0x04300010 /* [CFG] Reserved for software use */
#define BCHP_DS_A_MICRO_SW_SPARE3                0x04300014 /* [CFG] Reserved for software use */
#define BCHP_DS_A_MICRO_SW_SPARE4                0x04300018 /* [CFG] Reserved for software use */
#define BCHP_DS_A_MICRO_ECO_SPARE1               0x0430001c /* [CFG] Reserved for hardware ECO */
#define BCHP_DS_A_MICRO_ISO_CTL                  0x04300020 /* [RW] Isolation cell control */

/***************************************************************************
 *README - DS Top SPARE Register
 ***************************************************************************/
/* DS_A_MICRO :: README :: ECO_SPARE_0 [31:00] */
#define BCHP_DS_A_MICRO_README_ECO_SPARE_0_MASK                    0xffffffff
#define BCHP_DS_A_MICRO_README_ECO_SPARE_0_SHIFT                   0
#define BCHP_DS_A_MICRO_README_ECO_SPARE_0_DEFAULT                 0x00000000

/***************************************************************************
 *REVID - Revision ID Register
 ***************************************************************************/
/* DS_A_MICRO :: REVID :: reserved0 [31:12] */
#define BCHP_DS_A_MICRO_REVID_reserved0_MASK                       0xfffff000
#define BCHP_DS_A_MICRO_REVID_reserved0_SHIFT                      12

/* DS_A_MICRO :: REVID :: REVID [11:00] */
#define BCHP_DS_A_MICRO_REVID_REVID_MASK                           0x00000fff
#define BCHP_DS_A_MICRO_REVID_REVID_SHIFT                          0
#define BCHP_DS_A_MICRO_REVID_REVID_DEFAULT                        0x00000b2d

/***************************************************************************
 *STATUS - Status
 ***************************************************************************/
/* DS_A_MICRO :: STATUS :: reserved0 [31:04] */
#define BCHP_DS_A_MICRO_STATUS_reserved0_MASK                      0xfffffff0
#define BCHP_DS_A_MICRO_STATUS_reserved0_SHIFT                     4

/* DS_A_MICRO :: STATUS :: CLK_LEAP [03:03] */
#define BCHP_DS_A_MICRO_STATUS_CLK_LEAP_MASK                       0x00000008
#define BCHP_DS_A_MICRO_STATUS_CLK_LEAP_SHIFT                      3

/* DS_A_MICRO :: STATUS :: CLK_AIF [02:02] */
#define BCHP_DS_A_MICRO_STATUS_CLK_AIF_MASK                        0x00000004
#define BCHP_DS_A_MICRO_STATUS_CLK_AIF_SHIFT                       2

/* DS_A_MICRO :: STATUS :: CLK_270 [01:01] */
#define BCHP_DS_A_MICRO_STATUS_CLK_270_MASK                        0x00000002
#define BCHP_DS_A_MICRO_STATUS_CLK_270_SHIFT                       1

/* DS_A_MICRO :: STATUS :: CLK_540 [00:00] */
#define BCHP_DS_A_MICRO_STATUS_CLK_540_MASK                        0x00000001
#define BCHP_DS_A_MICRO_STATUS_CLK_540_SHIFT                       0

/***************************************************************************
 *SW_SPARE1 - Reserved for software use
 ***************************************************************************/
/* DS_A_MICRO :: SW_SPARE1 :: SPARE [31:00] */
#define BCHP_DS_A_MICRO_SW_SPARE1_SPARE_MASK                       0xffffffff
#define BCHP_DS_A_MICRO_SW_SPARE1_SPARE_SHIFT                      0
#define BCHP_DS_A_MICRO_SW_SPARE1_SPARE_DEFAULT                    0x00000000

/***************************************************************************
 *SW_SPARE2 - Reserved for software use
 ***************************************************************************/
/* DS_A_MICRO :: SW_SPARE2 :: SPARE [31:00] */
#define BCHP_DS_A_MICRO_SW_SPARE2_SPARE_MASK                       0xffffffff
#define BCHP_DS_A_MICRO_SW_SPARE2_SPARE_SHIFT                      0
#define BCHP_DS_A_MICRO_SW_SPARE2_SPARE_DEFAULT                    0x00000000

/***************************************************************************
 *SW_SPARE3 - Reserved for software use
 ***************************************************************************/
/* DS_A_MICRO :: SW_SPARE3 :: SPARE [31:00] */
#define BCHP_DS_A_MICRO_SW_SPARE3_SPARE_MASK                       0xffffffff
#define BCHP_DS_A_MICRO_SW_SPARE3_SPARE_SHIFT                      0
#define BCHP_DS_A_MICRO_SW_SPARE3_SPARE_DEFAULT                    0x00000000

/***************************************************************************
 *SW_SPARE4 - Reserved for software use
 ***************************************************************************/
/* DS_A_MICRO :: SW_SPARE4 :: SPARE [31:00] */
#define BCHP_DS_A_MICRO_SW_SPARE4_SPARE_MASK                       0xffffffff
#define BCHP_DS_A_MICRO_SW_SPARE4_SPARE_SHIFT                      0
#define BCHP_DS_A_MICRO_SW_SPARE4_SPARE_DEFAULT                    0x00000000

/***************************************************************************
 *ECO_SPARE1 - Reserved for hardware ECO
 ***************************************************************************/
/* DS_A_MICRO :: ECO_SPARE1 :: SPARE [31:00] */
#define BCHP_DS_A_MICRO_ECO_SPARE1_SPARE_MASK                      0xffffffff
#define BCHP_DS_A_MICRO_ECO_SPARE1_SPARE_SHIFT                     0
#define BCHP_DS_A_MICRO_ECO_SPARE1_SPARE_DEFAULT                   0x00000000

/***************************************************************************
 *ISO_CTL - Isolation cell control
 ***************************************************************************/
/* DS_A_MICRO :: ISO_CTL :: reserved0 [31:02] */
#define BCHP_DS_A_MICRO_ISO_CTL_reserved0_MASK                     0xfffffffc
#define BCHP_DS_A_MICRO_ISO_CTL_reserved0_SHIFT                    2

/* DS_A_MICRO :: ISO_CTL :: ISO_EN_DS2IFDAC [01:01] */
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_DS2IFDAC_MASK               0x00000002
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_DS2IFDAC_SHIFT              1
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_DS2IFDAC_DEFAULT            0x00000000

/* DS_A_MICRO :: ISO_CTL :: ISO_EN_IFDAC2DS [00:00] */
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_IFDAC2DS_MASK               0x00000001
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_IFDAC2DS_SHIFT              0
#define BCHP_DS_A_MICRO_ISO_CTL_ISO_EN_IFDAC2DS_DEFAULT            0x00000001

#endif /* #ifndef BCHP_DS_A_MICRO_H__ */

/* End of File */
