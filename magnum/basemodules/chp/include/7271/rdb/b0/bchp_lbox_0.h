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
 * Date:           Generated on               Thu Jun 16 14:47:12 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1009
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_LBOX_0_H__
#define BCHP_LBOX_0_H__

/***************************************************************************
 *LBOX_0 - Letter Box Detector 0 Registers
 ***************************************************************************/
#define BCHP_LBOX_0_REVISION_ID                  0x20624000 /* [RO] Chip Revision ID. */
#define BCHP_LBOX_0_CTRL                         0x20624004 /* [RW] OPERATIONS MODES (Control). */
#define BCHP_LBOX_0_CONFIGURATION                0x20624008 /* [RW] OPERATIONS MODES (Control). */
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION     0x2062400c /* [RW] Top/Bottom Horizontal Region (Control). */
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION       0x20624010 /* [RW] Left/Right Vertical Region (Control). */
#define BCHP_LBOX_0_PIXEL_THRESHOLD              0x20624014 /* [RW] Black Pixel Threshold (Control). */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA     0x20624018 /* [RW] Black Line Threshold (Control - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA      0x2062401c /* [RW] Black Column Threshold (Control - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA     0x20624020 /* [RW] White Line Threshold (Control - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA      0x20624024 /* [RW] White Column Threshold (Control - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM    0x20624028 /* [RW] Black Line Threshold (Control - Luma Sum Algorithm) */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM    0x2062402c /* [RW] White Line Threshold (Control - Luma Sum Algorithm) */
#define BCHP_LBOX_0_NON_BLACK_LINE               0x20624030 /* [RO] First/Last Non-Black Line (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_FIRST_WHITE_LINE             0x20624034 /* [RO] First White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_LAST_WHITE_LINE              0x20624038 /* [RO] Last White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES       0x2062403c /* [RO] Min/Max Non-Black Line (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_MIN_WHITE_LINE               0x20624040 /* [RO] Minimum First White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_MAX_WHITE_LINE               0x20624044 /* [RO] Maximum Last White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms) */
#define BCHP_LBOX_0_NON_BLACK_COL                0x20624048 /* [RO] First/Last Non-Black Column (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_FIRST_WHITE_COL              0x2062404c /* [RO] First White Column Statistics (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_LAST_WHITE_COL               0x20624050 /* [RO] Last White Column Statistics (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL         0x20624054 /* [RO] Min/Max Non-Black Column (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_MIN_WHITE_COL                0x20624058 /* [RO] Minimum First White Column Statistics (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_MAX_WHITE_COL                0x2062405c /* [RO] Maximum Last White Column Statistics (Status - Number & Threshold Algorithm) */
#define BCHP_LBOX_0_MIN_LUMA_VALUE               0x20624060 /* [RO] The minimum luma value over all time (Control). */
#define BCHP_LBOX_0_CLEAR_STATUS                 0x2062406c /* [WO] Clear all of the Status registers. */
#define BCHP_LBOX_0_SCRATCH                      0x20624070 /* [RW] Reserved Register For Future Use. */

/***************************************************************************
 *REVISION_ID - Chip Revision ID.
 ***************************************************************************/
/* LBOX_0 :: REVISION_ID :: reserved0 [31:16] */
#define BCHP_LBOX_0_REVISION_ID_reserved0_MASK                     0xffff0000
#define BCHP_LBOX_0_REVISION_ID_reserved0_SHIFT                    16

/* LBOX_0 :: REVISION_ID :: REVISION_ID_MAJOR [15:08] */
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MAJOR_MASK             0x0000ff00
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MAJOR_SHIFT            8
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MAJOR_DEFAULT          0x00000000

/* LBOX_0 :: REVISION_ID :: REVISION_ID_MINOR [07:00] */
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MINOR_MASK             0x000000ff
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MINOR_SHIFT            0
#define BCHP_LBOX_0_REVISION_ID_REVISION_ID_MINOR_DEFAULT          0x00000044

/***************************************************************************
 *CTRL - OPERATIONS MODES (Control).
 ***************************************************************************/
/* LBOX_0 :: CTRL :: reserved0 [31:02] */
#define BCHP_LBOX_0_CTRL_reserved0_MASK                            0xfffffffc
#define BCHP_LBOX_0_CTRL_reserved0_SHIFT                           2

/* LBOX_0 :: CTRL :: ENABLE_CTRL [01:01] */
#define BCHP_LBOX_0_CTRL_ENABLE_CTRL_MASK                          0x00000002
#define BCHP_LBOX_0_CTRL_ENABLE_CTRL_SHIFT                         1
#define BCHP_LBOX_0_CTRL_ENABLE_CTRL_DEFAULT                       0x00000000
#define BCHP_LBOX_0_CTRL_ENABLE_CTRL_ENABLE_BY_PICTURE             1
#define BCHP_LBOX_0_CTRL_ENABLE_CTRL_ALWAYS_ENABLE                 0

/* LBOX_0 :: CTRL :: ENABLE_ACCEPT [00:00] */
#define BCHP_LBOX_0_CTRL_ENABLE_ACCEPT_MASK                        0x00000001
#define BCHP_LBOX_0_CTRL_ENABLE_ACCEPT_SHIFT                       0
#define BCHP_LBOX_0_CTRL_ENABLE_ACCEPT_DEFAULT                     0x00000000
#define BCHP_LBOX_0_CTRL_ENABLE_ACCEPT_DIS                         0
#define BCHP_LBOX_0_CTRL_ENABLE_ACCEPT_EN                          1

/***************************************************************************
 *CONFIGURATION - OPERATIONS MODES (Control).
 ***************************************************************************/
/* LBOX_0 :: CONFIGURATION :: reserved0 [31:05] */
#define BCHP_LBOX_0_CONFIGURATION_reserved0_MASK                   0xffffffe0
#define BCHP_LBOX_0_CONFIGURATION_reserved0_SHIFT                  5

/* LBOX_0 :: CONFIGURATION :: TB_HORIZONTAL_REGION_ENABLE [04:04] */
#define BCHP_LBOX_0_CONFIGURATION_TB_HORIZONTAL_REGION_ENABLE_MASK 0x00000010
#define BCHP_LBOX_0_CONFIGURATION_TB_HORIZONTAL_REGION_ENABLE_SHIFT 4
#define BCHP_LBOX_0_CONFIGURATION_TB_HORIZONTAL_REGION_ENABLE_DEFAULT 0x00000001
#define BCHP_LBOX_0_CONFIGURATION_TB_HORIZONTAL_REGION_ENABLE_DISABLE 0
#define BCHP_LBOX_0_CONFIGURATION_TB_HORIZONTAL_REGION_ENABLE_ENABLE 1

/* LBOX_0 :: CONFIGURATION :: TB_VERTICAL_REGION_ENABLE [03:03] */
#define BCHP_LBOX_0_CONFIGURATION_TB_VERTICAL_REGION_ENABLE_MASK   0x00000008
#define BCHP_LBOX_0_CONFIGURATION_TB_VERTICAL_REGION_ENABLE_SHIFT  3
#define BCHP_LBOX_0_CONFIGURATION_TB_VERTICAL_REGION_ENABLE_DEFAULT 0x00000000
#define BCHP_LBOX_0_CONFIGURATION_TB_VERTICAL_REGION_ENABLE_DISABLE 0
#define BCHP_LBOX_0_CONFIGURATION_TB_VERTICAL_REGION_ENABLE_ENABLE 1

/* LBOX_0 :: CONFIGURATION :: LR_HORIZONTAL_REGION_ENABLE [02:02] */
#define BCHP_LBOX_0_CONFIGURATION_LR_HORIZONTAL_REGION_ENABLE_MASK 0x00000004
#define BCHP_LBOX_0_CONFIGURATION_LR_HORIZONTAL_REGION_ENABLE_SHIFT 2
#define BCHP_LBOX_0_CONFIGURATION_LR_HORIZONTAL_REGION_ENABLE_DEFAULT 0x00000000
#define BCHP_LBOX_0_CONFIGURATION_LR_HORIZONTAL_REGION_ENABLE_DISABLE 0
#define BCHP_LBOX_0_CONFIGURATION_LR_HORIZONTAL_REGION_ENABLE_ENABLE 1

/* LBOX_0 :: CONFIGURATION :: LR_VERTICAL_REGION_ENABLE [01:01] */
#define BCHP_LBOX_0_CONFIGURATION_LR_VERTICAL_REGION_ENABLE_MASK   0x00000002
#define BCHP_LBOX_0_CONFIGURATION_LR_VERTICAL_REGION_ENABLE_SHIFT  1
#define BCHP_LBOX_0_CONFIGURATION_LR_VERTICAL_REGION_ENABLE_DEFAULT 0x00000001
#define BCHP_LBOX_0_CONFIGURATION_LR_VERTICAL_REGION_ENABLE_DISABLE 0
#define BCHP_LBOX_0_CONFIGURATION_LR_VERTICAL_REGION_ENABLE_ENABLE 1

/* LBOX_0 :: CONFIGURATION :: DETECT_MODE [00:00] */
#define BCHP_LBOX_0_CONFIGURATION_DETECT_MODE_MASK                 0x00000001
#define BCHP_LBOX_0_CONFIGURATION_DETECT_MODE_SHIFT                0
#define BCHP_LBOX_0_CONFIGURATION_DETECT_MODE_DEFAULT              0x00000000
#define BCHP_LBOX_0_CONFIGURATION_DETECT_MODE_NUMBER               0
#define BCHP_LBOX_0_CONFIGURATION_DETECT_MODE_SUM                  1

/***************************************************************************
 *HORIZONTAL_WINDOW_REGION - Top/Bottom Horizontal Region (Control).
 ***************************************************************************/
/* LBOX_0 :: HORIZONTAL_WINDOW_REGION :: reserved0 [31:29] */
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_reserved0_MASK        0xe0000000
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_reserved0_SHIFT       29

/* LBOX_0 :: HORIZONTAL_WINDOW_REGION :: HORIZONTAL_START_POINT [28:16] */
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_START_POINT_MASK 0x1fff0000
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_START_POINT_SHIFT 16
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_START_POINT_DEFAULT 0x00000000

/* LBOX_0 :: HORIZONTAL_WINDOW_REGION :: reserved1 [15:13] */
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_reserved1_MASK        0x0000e000
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_reserved1_SHIFT       13

/* LBOX_0 :: HORIZONTAL_WINDOW_REGION :: HORIZONTAL_END_POINT [12:00] */
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_END_POINT_MASK 0x00001fff
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_END_POINT_SHIFT 0
#define BCHP_LBOX_0_HORIZONTAL_WINDOW_REGION_HORIZONTAL_END_POINT_DEFAULT 0x00001fff

/***************************************************************************
 *VERTICAL_WINDOW_REGION - Left/Right Vertical Region (Control).
 ***************************************************************************/
/* LBOX_0 :: VERTICAL_WINDOW_REGION :: reserved0 [31:28] */
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_reserved0_MASK          0xf0000000
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_reserved0_SHIFT         28

/* LBOX_0 :: VERTICAL_WINDOW_REGION :: VERTICAL_START_POINT [27:16] */
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_START_POINT_MASK 0x0fff0000
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_START_POINT_SHIFT 16
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_START_POINT_DEFAULT 0x00000000

/* LBOX_0 :: VERTICAL_WINDOW_REGION :: reserved1 [15:12] */
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_reserved1_MASK          0x0000f000
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_reserved1_SHIFT         12

/* LBOX_0 :: VERTICAL_WINDOW_REGION :: VERTICAL_END_POINT [11:00] */
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_END_POINT_MASK 0x00000fff
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_END_POINT_SHIFT 0
#define BCHP_LBOX_0_VERTICAL_WINDOW_REGION_VERTICAL_END_POINT_DEFAULT 0x00000fff

/***************************************************************************
 *PIXEL_THRESHOLD - Black Pixel Threshold (Control).
 ***************************************************************************/
/* LBOX_0 :: PIXEL_THRESHOLD :: reserved0 [31:24] */
#define BCHP_LBOX_0_PIXEL_THRESHOLD_reserved0_MASK                 0xff000000
#define BCHP_LBOX_0_PIXEL_THRESHOLD_reserved0_SHIFT                24

/* LBOX_0 :: PIXEL_THRESHOLD :: BLACK_LEVEL_LUMA_SUM [23:16] */
#define BCHP_LBOX_0_PIXEL_THRESHOLD_BLACK_LEVEL_LUMA_SUM_MASK      0x00ff0000
#define BCHP_LBOX_0_PIXEL_THRESHOLD_BLACK_LEVEL_LUMA_SUM_SHIFT     16
#define BCHP_LBOX_0_PIXEL_THRESHOLD_BLACK_LEVEL_LUMA_SUM_DEFAULT   0x00000010

/* LBOX_0 :: PIXEL_THRESHOLD :: TB_BLACK_LEVEL_NTA [15:08] */
#define BCHP_LBOX_0_PIXEL_THRESHOLD_TB_BLACK_LEVEL_NTA_MASK        0x0000ff00
#define BCHP_LBOX_0_PIXEL_THRESHOLD_TB_BLACK_LEVEL_NTA_SHIFT       8
#define BCHP_LBOX_0_PIXEL_THRESHOLD_TB_BLACK_LEVEL_NTA_DEFAULT     0x00000010

/* LBOX_0 :: PIXEL_THRESHOLD :: LR_BLACK_LEVEL_NTA [07:00] */
#define BCHP_LBOX_0_PIXEL_THRESHOLD_LR_BLACK_LEVEL_NTA_MASK        0x000000ff
#define BCHP_LBOX_0_PIXEL_THRESHOLD_LR_BLACK_LEVEL_NTA_SHIFT       0
#define BCHP_LBOX_0_PIXEL_THRESHOLD_LR_BLACK_LEVEL_NTA_DEFAULT     0x00000010

/***************************************************************************
 *BLACK_LINE_THRESHOLD_NTA - Black Line Threshold (Control - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: BLACK_LINE_THRESHOLD_NTA :: reserved0 [31:13] */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA_reserved0_MASK        0xffffe000
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA_reserved0_SHIFT       13

/* LBOX_0 :: BLACK_LINE_THRESHOLD_NTA :: BLACK_LINE_THRESHOLD_SIZE [12:00] */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA_BLACK_LINE_THRESHOLD_SIZE_MASK 0x00001fff
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA_BLACK_LINE_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_NTA_BLACK_LINE_THRESHOLD_SIZE_DEFAULT 0x00000005

/***************************************************************************
 *BLACK_COL_THRESHOLD_NTA - Black Column Threshold (Control - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: BLACK_COL_THRESHOLD_NTA :: reserved0 [31:12] */
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA_reserved0_MASK         0xfffff000
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA_reserved0_SHIFT        12

/* LBOX_0 :: BLACK_COL_THRESHOLD_NTA :: BLACK_COL_THRESHOLD_SIZE [11:00] */
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA_BLACK_COL_THRESHOLD_SIZE_MASK 0x00000fff
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA_BLACK_COL_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_BLACK_COL_THRESHOLD_NTA_BLACK_COL_THRESHOLD_SIZE_DEFAULT 0x00000003

/***************************************************************************
 *WHITE_LINE_THRESHOLD_NTA - White Line Threshold (Control - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: WHITE_LINE_THRESHOLD_NTA :: reserved0 [31:13] */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA_reserved0_MASK        0xffffe000
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA_reserved0_SHIFT       13

/* LBOX_0 :: WHITE_LINE_THRESHOLD_NTA :: WHITE_LINE_THRESHOLD_SIZE [12:00] */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA_WHITE_LINE_THRESHOLD_SIZE_MASK 0x00001fff
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA_WHITE_LINE_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_NTA_WHITE_LINE_THRESHOLD_SIZE_DEFAULT 0x00000032

/***************************************************************************
 *WHITE_COL_THRESHOLD_NTA - White Column Threshold (Control - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: WHITE_COL_THRESHOLD_NTA :: reserved0 [31:12] */
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA_reserved0_MASK         0xfffff000
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA_reserved0_SHIFT        12

/* LBOX_0 :: WHITE_COL_THRESHOLD_NTA :: WHITE_COL_THRESHOLD_SIZE [11:00] */
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA_WHITE_COL_THRESHOLD_SIZE_MASK 0x00000fff
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA_WHITE_COL_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_WHITE_COL_THRESHOLD_NTA_WHITE_COL_THRESHOLD_SIZE_DEFAULT 0x00000019

/***************************************************************************
 *BLACK_LINE_THRESHOLD_LSUM - Black Line Threshold (Control - Luma Sum Algorithm)
 ***************************************************************************/
/* LBOX_0 :: BLACK_LINE_THRESHOLD_LSUM :: reserved0 [31:20] */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM_reserved0_MASK       0xfff00000
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM_reserved0_SHIFT      20

/* LBOX_0 :: BLACK_LINE_THRESHOLD_LSUM :: BLACK_LINE_THRESHOLD_SIZE [19:00] */
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM_BLACK_LINE_THRESHOLD_SIZE_MASK 0x000fffff
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM_BLACK_LINE_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_BLACK_LINE_THRESHOLD_LSUM_BLACK_LINE_THRESHOLD_SIZE_DEFAULT 0x00001b58

/***************************************************************************
 *WHITE_LINE_THRESHOLD_LSUM - White Line Threshold (Control - Luma Sum Algorithm)
 ***************************************************************************/
/* LBOX_0 :: WHITE_LINE_THRESHOLD_LSUM :: reserved0 [31:20] */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM_reserved0_MASK       0xfff00000
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM_reserved0_SHIFT      20

/* LBOX_0 :: WHITE_LINE_THRESHOLD_LSUM :: WHITE_LINE_THRESHOLD_SIZE [19:00] */
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM_WHITE_LINE_THRESHOLD_SIZE_MASK 0x000fffff
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM_WHITE_LINE_THRESHOLD_SIZE_SHIFT 0
#define BCHP_LBOX_0_WHITE_LINE_THRESHOLD_LSUM_WHITE_LINE_THRESHOLD_SIZE_DEFAULT 0x00002328

/***************************************************************************
 *NON_BLACK_LINE - First/Last Non-Black Line (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: NON_BLACK_LINE :: reserved0 [31:24] */
#define BCHP_LBOX_0_NON_BLACK_LINE_reserved0_MASK                  0xff000000
#define BCHP_LBOX_0_NON_BLACK_LINE_reserved0_SHIFT                 24

/* LBOX_0 :: NON_BLACK_LINE :: FIRST_NON_BLACK_LINE_NUMBER [23:12] */
#define BCHP_LBOX_0_NON_BLACK_LINE_FIRST_NON_BLACK_LINE_NUMBER_MASK 0x00fff000
#define BCHP_LBOX_0_NON_BLACK_LINE_FIRST_NON_BLACK_LINE_NUMBER_SHIFT 12
#define BCHP_LBOX_0_NON_BLACK_LINE_FIRST_NON_BLACK_LINE_NUMBER_DEFAULT 0x00000fff

/* LBOX_0 :: NON_BLACK_LINE :: LAST_NON_BLACK_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_NON_BLACK_LINE_LAST_NON_BLACK_LINE_NUMBER_MASK 0x00000fff
#define BCHP_LBOX_0_NON_BLACK_LINE_LAST_NON_BLACK_LINE_NUMBER_SHIFT 0
#define BCHP_LBOX_0_NON_BLACK_LINE_LAST_NON_BLACK_LINE_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *FIRST_WHITE_LINE - First White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: FIRST_WHITE_LINE :: FIRST_WHITE_LINE_STRENGTH [31:12] */
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_STRENGTH_MASK 0xfffff000
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_STRENGTH_SHIFT 12
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: FIRST_WHITE_LINE :: FIRST_WHITE_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_NUMBER_MASK  0x00000fff
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_NUMBER_SHIFT 0
#define BCHP_LBOX_0_FIRST_WHITE_LINE_FIRST_WHITE_LINE_NUMBER_DEFAULT 0x00000fff

/***************************************************************************
 *LAST_WHITE_LINE - Last White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: LAST_WHITE_LINE :: LAST_WHITE_LINE_STRENGTH [31:12] */
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_STRENGTH_MASK  0xfffff000
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_STRENGTH_SHIFT 12
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: LAST_WHITE_LINE :: LAST_WHITE_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_NUMBER_MASK    0x00000fff
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_NUMBER_SHIFT   0
#define BCHP_LBOX_0_LAST_WHITE_LINE_LAST_WHITE_LINE_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *MINMAX_NON_BLACK_LINES - Min/Max Non-Black Line (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: MINMAX_NON_BLACK_LINES :: reserved0 [31:24] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_reserved0_MASK          0xff000000
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_reserved0_SHIFT         24

/* LBOX_0 :: MINMAX_NON_BLACK_LINES :: SMALL_NON_BLACK_LINE_NUMBER [23:12] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_SMALL_NON_BLACK_LINE_NUMBER_MASK 0x00fff000
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_SMALL_NON_BLACK_LINE_NUMBER_SHIFT 12
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_SMALL_NON_BLACK_LINE_NUMBER_DEFAULT 0x00000fff

/* LBOX_0 :: MINMAX_NON_BLACK_LINES :: LARGE_NON_BLACK_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_LARGE_NON_BLACK_LINE_NUMBER_MASK 0x00000fff
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_LARGE_NON_BLACK_LINE_NUMBER_SHIFT 0
#define BCHP_LBOX_0_MINMAX_NON_BLACK_LINES_LARGE_NON_BLACK_LINE_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *MIN_WHITE_LINE - Minimum First White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: MIN_WHITE_LINE :: SMALL_WHITE_LINE_STRENGTH [31:12] */
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_STRENGTH_MASK  0xfffff000
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_STRENGTH_SHIFT 12
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: MIN_WHITE_LINE :: SMALL_WHITE_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_NUMBER_MASK    0x00000fff
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_NUMBER_SHIFT   0
#define BCHP_LBOX_0_MIN_WHITE_LINE_SMALL_WHITE_LINE_NUMBER_DEFAULT 0x00000fff

/***************************************************************************
 *MAX_WHITE_LINE - Maximum Last White Line Statistics (Status - Number & Threshold and Luma Sum Algorithms)
 ***************************************************************************/
/* LBOX_0 :: MAX_WHITE_LINE :: LARGE_WHITE_LINE_STRENGTH [31:12] */
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_STRENGTH_MASK  0xfffff000
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_STRENGTH_SHIFT 12
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: MAX_WHITE_LINE :: LARGE_WHITE_LINE_NUMBER [11:00] */
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_NUMBER_MASK    0x00000fff
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_NUMBER_SHIFT   0
#define BCHP_LBOX_0_MAX_WHITE_LINE_LARGE_WHITE_LINE_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *NON_BLACK_COL - First/Last Non-Black Column (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: NON_BLACK_COL :: reserved0 [31:26] */
#define BCHP_LBOX_0_NON_BLACK_COL_reserved0_MASK                   0xfc000000
#define BCHP_LBOX_0_NON_BLACK_COL_reserved0_SHIFT                  26

/* LBOX_0 :: NON_BLACK_COL :: FIRST_NON_BLACK_COL_NUMBER [25:13] */
#define BCHP_LBOX_0_NON_BLACK_COL_FIRST_NON_BLACK_COL_NUMBER_MASK  0x03ffe000
#define BCHP_LBOX_0_NON_BLACK_COL_FIRST_NON_BLACK_COL_NUMBER_SHIFT 13
#define BCHP_LBOX_0_NON_BLACK_COL_FIRST_NON_BLACK_COL_NUMBER_DEFAULT 0x00001fff

/* LBOX_0 :: NON_BLACK_COL :: LAST_NON_BLACK_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_NON_BLACK_COL_LAST_NON_BLACK_COL_NUMBER_MASK   0x00001fff
#define BCHP_LBOX_0_NON_BLACK_COL_LAST_NON_BLACK_COL_NUMBER_SHIFT  0
#define BCHP_LBOX_0_NON_BLACK_COL_LAST_NON_BLACK_COL_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *FIRST_WHITE_COL - First White Column Statistics (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: FIRST_WHITE_COL :: reserved0 [31:25] */
#define BCHP_LBOX_0_FIRST_WHITE_COL_reserved0_MASK                 0xfe000000
#define BCHP_LBOX_0_FIRST_WHITE_COL_reserved0_SHIFT                25

/* LBOX_0 :: FIRST_WHITE_COL :: FIRST_WHITE_COL_STRENGTH [24:13] */
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_STRENGTH_MASK  0x01ffe000
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_STRENGTH_SHIFT 13
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: FIRST_WHITE_COL :: FIRST_WHITE_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_NUMBER_MASK    0x00001fff
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_NUMBER_SHIFT   0
#define BCHP_LBOX_0_FIRST_WHITE_COL_FIRST_WHITE_COL_NUMBER_DEFAULT 0x00001fff

/***************************************************************************
 *LAST_WHITE_COL - Last White Column Statistics (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: LAST_WHITE_COL :: reserved0 [31:25] */
#define BCHP_LBOX_0_LAST_WHITE_COL_reserved0_MASK                  0xfe000000
#define BCHP_LBOX_0_LAST_WHITE_COL_reserved0_SHIFT                 25

/* LBOX_0 :: LAST_WHITE_COL :: LAST_WHITE_COL_STRENGTH [24:13] */
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_STRENGTH_MASK    0x01ffe000
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_STRENGTH_SHIFT   13
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: LAST_WHITE_COL :: LAST_WHITE_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_NUMBER_MASK      0x00001fff
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_NUMBER_SHIFT     0
#define BCHP_LBOX_0_LAST_WHITE_COL_LAST_WHITE_COL_NUMBER_DEFAULT   0x00000000

/***************************************************************************
 *MINMAX_NON_BLACK_COL - Min/Max Non-Black Column (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: MINMAX_NON_BLACK_COL :: reserved0 [31:26] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_reserved0_MASK            0xfc000000
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_reserved0_SHIFT           26

/* LBOX_0 :: MINMAX_NON_BLACK_COL :: SMALL_NON_BLACK_COL_NUMBER [25:13] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_SMALL_NON_BLACK_COL_NUMBER_MASK 0x03ffe000
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_SMALL_NON_BLACK_COL_NUMBER_SHIFT 13
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_SMALL_NON_BLACK_COL_NUMBER_DEFAULT 0x00001fff

/* LBOX_0 :: MINMAX_NON_BLACK_COL :: LARGE_NON_BLACK_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_LARGE_NON_BLACK_COL_NUMBER_MASK 0x00001fff
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_LARGE_NON_BLACK_COL_NUMBER_SHIFT 0
#define BCHP_LBOX_0_MINMAX_NON_BLACK_COL_LARGE_NON_BLACK_COL_NUMBER_DEFAULT 0x00000000

/***************************************************************************
 *MIN_WHITE_COL - Minimum First White Column Statistics (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: MIN_WHITE_COL :: reserved0 [31:25] */
#define BCHP_LBOX_0_MIN_WHITE_COL_reserved0_MASK                   0xfe000000
#define BCHP_LBOX_0_MIN_WHITE_COL_reserved0_SHIFT                  25

/* LBOX_0 :: MIN_WHITE_COL :: SMALL_WHITE_COL_STRENGTH [24:13] */
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_STRENGTH_MASK    0x01ffe000
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_STRENGTH_SHIFT   13
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: MIN_WHITE_COL :: SMALL_WHITE_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_NUMBER_MASK      0x00001fff
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_NUMBER_SHIFT     0
#define BCHP_LBOX_0_MIN_WHITE_COL_SMALL_WHITE_COL_NUMBER_DEFAULT   0x00001fff

/***************************************************************************
 *MAX_WHITE_COL - Maximum Last White Column Statistics (Status - Number & Threshold Algorithm)
 ***************************************************************************/
/* LBOX_0 :: MAX_WHITE_COL :: reserved0 [31:25] */
#define BCHP_LBOX_0_MAX_WHITE_COL_reserved0_MASK                   0xfe000000
#define BCHP_LBOX_0_MAX_WHITE_COL_reserved0_SHIFT                  25

/* LBOX_0 :: MAX_WHITE_COL :: LARGE_WHITE_COL_STRENGTH [24:13] */
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_STRENGTH_MASK    0x01ffe000
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_STRENGTH_SHIFT   13
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_STRENGTH_DEFAULT 0x00000000

/* LBOX_0 :: MAX_WHITE_COL :: LARGE_WHITE_COL_NUMBER [12:00] */
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_NUMBER_MASK      0x00001fff
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_NUMBER_SHIFT     0
#define BCHP_LBOX_0_MAX_WHITE_COL_LARGE_WHITE_COL_NUMBER_DEFAULT   0x00000000

/***************************************************************************
 *MIN_LUMA_VALUE - The minimum luma value over all time (Control).
 ***************************************************************************/
/* LBOX_0 :: MIN_LUMA_VALUE :: reserved0 [31:16] */
#define BCHP_LBOX_0_MIN_LUMA_VALUE_reserved0_MASK                  0xffff0000
#define BCHP_LBOX_0_MIN_LUMA_VALUE_reserved0_SHIFT                 16

/* LBOX_0 :: MIN_LUMA_VALUE :: TB_LUMA_VALUE [15:08] */
#define BCHP_LBOX_0_MIN_LUMA_VALUE_TB_LUMA_VALUE_MASK              0x0000ff00
#define BCHP_LBOX_0_MIN_LUMA_VALUE_TB_LUMA_VALUE_SHIFT             8
#define BCHP_LBOX_0_MIN_LUMA_VALUE_TB_LUMA_VALUE_DEFAULT           0x000000ff

/* LBOX_0 :: MIN_LUMA_VALUE :: LR_LUMA_VALUE [07:00] */
#define BCHP_LBOX_0_MIN_LUMA_VALUE_LR_LUMA_VALUE_MASK              0x000000ff
#define BCHP_LBOX_0_MIN_LUMA_VALUE_LR_LUMA_VALUE_SHIFT             0
#define BCHP_LBOX_0_MIN_LUMA_VALUE_LR_LUMA_VALUE_DEFAULT           0x000000ff

/***************************************************************************
 *CLEAR_STATUS - Clear all of the Status registers.
 ***************************************************************************/
/* LBOX_0 :: CLEAR_STATUS :: reserved0 [31:02] */
#define BCHP_LBOX_0_CLEAR_STATUS_reserved0_MASK                    0xfffffffc
#define BCHP_LBOX_0_CLEAR_STATUS_reserved0_SHIFT                   2

/* LBOX_0 :: CLEAR_STATUS :: CLEAR_EDGES [01:01] */
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_EDGES_MASK                  0x00000002
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_EDGES_SHIFT                 1
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_EDGES_DEFAULT               0x00000000

/* LBOX_0 :: CLEAR_STATUS :: CLEAR_LUMA [00:00] */
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_LUMA_MASK                   0x00000001
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_LUMA_SHIFT                  0
#define BCHP_LBOX_0_CLEAR_STATUS_CLEAR_LUMA_DEFAULT                0x00000000

/***************************************************************************
 *SCRATCH - Reserved Register For Future Use.
 ***************************************************************************/
/* LBOX_0 :: SCRATCH :: RESERVED [31:00] */
#define BCHP_LBOX_0_SCRATCH_RESERVED_MASK                          0xffffffff
#define BCHP_LBOX_0_SCRATCH_RESERVED_SHIFT                         0
#define BCHP_LBOX_0_SCRATCH_RESERVED_DEFAULT                       0x00000000

#endif /* #ifndef BCHP_LBOX_0_H__ */

/* End of File */
