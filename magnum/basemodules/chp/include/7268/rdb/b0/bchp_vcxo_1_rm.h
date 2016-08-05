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
 * Date:           Generated on               Mon Jul 18 11:35:16 2016
 *                 Full Compile MD5 Checksum  d473fbf4aefc82fe025f19a353b681d2
 *                     (minus title and desc)
 *                 MD5 Checksum               a04cbea2e001ec837c188fede55d60fb
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1066
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_VCXO_1_RM_H__
#define BCHP_VCXO_1_RM_H__

/***************************************************************************
 *VCXO_1_RM - PLL_VCXO1 Rate Manager
 ***************************************************************************/
#define BCHP_VCXO_1_RM_CONTROL                   0x204e2880 /* [RW] Rate Manager Controls */
#define BCHP_VCXO_1_RM_RATE_RATIO                0x204e2884 /* [RW] Rate Manager Output Rate Setting I */
#define BCHP_VCXO_1_RM_SAMPLE_INC                0x204e2888 /* [RW] Rate Manager Output Rate Setting II */
#define BCHP_VCXO_1_RM_PHASE_INC                 0x204e288c /* [RW] Rate Manager NCO Phase Increment */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI             0x204e2890 /* [WO] Loop Filter Integrator HI Value */
#define BCHP_VCXO_1_RM_INTEGRATOR_LO             0x204e2894 /* [WO] Loop Filter Integrator LO Value */
#define BCHP_VCXO_1_RM_OFFSET                    0x204e2898 /* [RW] Rate Manager Output Formatting */
#define BCHP_VCXO_1_RM_FORMAT                    0x204e289c /* [RW] Rate Manager Output Formatting */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL       0x204e28a0 /* [RW] Time Base Skip or Repeat Control Register */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP           0x204e28a4 /* [RW] Time Base Skip or Repeat Gap Count Register */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER        0x204e28a8 /* [RW] Time Base Skip or Repeat Count Register */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS      0x204e28ac /* [RO] Loop Filter Integrator HI Status */
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_STATUS      0x204e28b0 /* [RO] Loop Filter Integrator LO Status */
#define BCHP_VCXO_1_RM_STATUS                    0x204e28b4 /* [RO] Rate Manager Status Register */
#define BCHP_VCXO_1_RM_STATUS_CLEAR              0x204e28b8 /* [RW] Rate Manager Status Clear Register */

/***************************************************************************
 *CONTROL - Rate Manager Controls
 ***************************************************************************/
/* VCXO_1_RM :: CONTROL :: reserved0 [31:22] */
#define BCHP_VCXO_1_RM_CONTROL_reserved0_MASK                      0xffc00000
#define BCHP_VCXO_1_RM_CONTROL_reserved0_SHIFT                     22

/* VCXO_1_RM :: CONTROL :: READ_INTEGRATOR [21:21] */
#define BCHP_VCXO_1_RM_CONTROL_READ_INTEGRATOR_MASK                0x00200000
#define BCHP_VCXO_1_RM_CONTROL_READ_INTEGRATOR_SHIFT               21
#define BCHP_VCXO_1_RM_CONTROL_READ_INTEGRATOR_DEFAULT             0x00000000

/* VCXO_1_RM :: CONTROL :: FREEZE_COUNTERS [20:20] */
#define BCHP_VCXO_1_RM_CONTROL_FREEZE_COUNTERS_MASK                0x00100000
#define BCHP_VCXO_1_RM_CONTROL_FREEZE_COUNTERS_SHIFT               20
#define BCHP_VCXO_1_RM_CONTROL_FREEZE_COUNTERS_DEFAULT             0x00000000
#define BCHP_VCXO_1_RM_CONTROL_FREEZE_COUNTERS_ENABLE              1
#define BCHP_VCXO_1_RM_CONTROL_FREEZE_COUNTERS_DISABLE             0

/* VCXO_1_RM :: CONTROL :: EN_FREEZE_COUNTERS [19:19] */
#define BCHP_VCXO_1_RM_CONTROL_EN_FREEZE_COUNTERS_MASK             0x00080000
#define BCHP_VCXO_1_RM_CONTROL_EN_FREEZE_COUNTERS_SHIFT            19
#define BCHP_VCXO_1_RM_CONTROL_EN_FREEZE_COUNTERS_DEFAULT          0x00000000
#define BCHP_VCXO_1_RM_CONTROL_EN_FREEZE_COUNTERS_ENABLE           1
#define BCHP_VCXO_1_RM_CONTROL_EN_FREEZE_COUNTERS_DISABLE          0

/* VCXO_1_RM :: CONTROL :: LOAD_INTEGRATOR [18:18] */
#define BCHP_VCXO_1_RM_CONTROL_LOAD_INTEGRATOR_MASK                0x00040000
#define BCHP_VCXO_1_RM_CONTROL_LOAD_INTEGRATOR_SHIFT               18
#define BCHP_VCXO_1_RM_CONTROL_LOAD_INTEGRATOR_DEFAULT             0x00000000
#define BCHP_VCXO_1_RM_CONTROL_LOAD_INTEGRATOR_ENABLE              1
#define BCHP_VCXO_1_RM_CONTROL_LOAD_INTEGRATOR_DISABLE             0

/* VCXO_1_RM :: CONTROL :: EN_LOAD_INTEGRATOR [17:17] */
#define BCHP_VCXO_1_RM_CONTROL_EN_LOAD_INTEGRATOR_MASK             0x00020000
#define BCHP_VCXO_1_RM_CONTROL_EN_LOAD_INTEGRATOR_SHIFT            17
#define BCHP_VCXO_1_RM_CONTROL_EN_LOAD_INTEGRATOR_DEFAULT          0x00000000
#define BCHP_VCXO_1_RM_CONTROL_EN_LOAD_INTEGRATOR_ENABLE           1
#define BCHP_VCXO_1_RM_CONTROL_EN_LOAD_INTEGRATOR_DISABLE          0

/* VCXO_1_RM :: CONTROL :: DISABLE_FREQUENCY_CONFIG [16:16] */
#define BCHP_VCXO_1_RM_CONTROL_DISABLE_FREQUENCY_CONFIG_MASK       0x00010000
#define BCHP_VCXO_1_RM_CONTROL_DISABLE_FREQUENCY_CONFIG_SHIFT      16
#define BCHP_VCXO_1_RM_CONTROL_DISABLE_FREQUENCY_CONFIG_DEFAULT    0x00000000
#define BCHP_VCXO_1_RM_CONTROL_DISABLE_FREQUENCY_CONFIG_ENABLE     0
#define BCHP_VCXO_1_RM_CONTROL_DISABLE_FREQUENCY_CONFIG_DISABLE    1

/* VCXO_1_RM :: CONTROL :: TRACKING_RANGE [15:13] */
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_MASK                 0x0000e000
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_SHIFT                13
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_DEFAULT              0x00000000
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_15616        6
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_7808         5
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_3904         4
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_1952         3
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_976          2
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_488          1
#define BCHP_VCXO_1_RM_CONTROL_TRACKING_RANGE_ZERO_to_244          0

/* VCXO_1_RM :: CONTROL :: RESET [12:12] */
#define BCHP_VCXO_1_RM_CONTROL_RESET_MASK                          0x00001000
#define BCHP_VCXO_1_RM_CONTROL_RESET_SHIFT                         12
#define BCHP_VCXO_1_RM_CONTROL_RESET_DEFAULT                       0x00000000
#define BCHP_VCXO_1_RM_CONTROL_RESET_RESET_ON                      1
#define BCHP_VCXO_1_RM_CONTROL_RESET_RESET_OFF                     0

/* VCXO_1_RM :: CONTROL :: INT_GAIN [11:09] */
#define BCHP_VCXO_1_RM_CONTROL_INT_GAIN_MASK                       0x00000e00
#define BCHP_VCXO_1_RM_CONTROL_INT_GAIN_SHIFT                      9
#define BCHP_VCXO_1_RM_CONTROL_INT_GAIN_DEFAULT                    0x00000004

/* VCXO_1_RM :: CONTROL :: DIRECT_GAIN [08:06] */
#define BCHP_VCXO_1_RM_CONTROL_DIRECT_GAIN_MASK                    0x000001c0
#define BCHP_VCXO_1_RM_CONTROL_DIRECT_GAIN_SHIFT                   6
#define BCHP_VCXO_1_RM_CONTROL_DIRECT_GAIN_DEFAULT                 0x00000004

/* VCXO_1_RM :: CONTROL :: DITHER [05:05] */
#define BCHP_VCXO_1_RM_CONTROL_DITHER_MASK                         0x00000020
#define BCHP_VCXO_1_RM_CONTROL_DITHER_SHIFT                        5
#define BCHP_VCXO_1_RM_CONTROL_DITHER_DEFAULT                      0x00000000
#define BCHP_VCXO_1_RM_CONTROL_DITHER_DITHER_ON                    1
#define BCHP_VCXO_1_RM_CONTROL_DITHER_DITHER_OFF                   0

/* VCXO_1_RM :: CONTROL :: FREE_RUN [04:04] */
#define BCHP_VCXO_1_RM_CONTROL_FREE_RUN_MASK                       0x00000010
#define BCHP_VCXO_1_RM_CONTROL_FREE_RUN_SHIFT                      4
#define BCHP_VCXO_1_RM_CONTROL_FREE_RUN_DEFAULT                    0x00000000
#define BCHP_VCXO_1_RM_CONTROL_FREE_RUN_FREE_RUN_ON                1
#define BCHP_VCXO_1_RM_CONTROL_FREE_RUN_TIMEBASE                   0

/* VCXO_1_RM :: CONTROL :: TIMEBASE [03:00] */
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_MASK                       0x0000000f
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_SHIFT                      0
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_DEFAULT                    0x00000000
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_15                15
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_14                14
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_13                13
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_12                12
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_11                11
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_10                10
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_9                 9
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_8                 8
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_7                 7
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_6                 6
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_5                 5
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_4                 4
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_3                 3
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_2                 2
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_1                 1
#define BCHP_VCXO_1_RM_CONTROL_TIMEBASE_TIMEBASE_0                 0

/***************************************************************************
 *RATE_RATIO - Rate Manager Output Rate Setting I
 ***************************************************************************/
/* VCXO_1_RM :: RATE_RATIO :: reserved0 [31:24] */
#define BCHP_VCXO_1_RM_RATE_RATIO_reserved0_MASK                   0xff000000
#define BCHP_VCXO_1_RM_RATE_RATIO_reserved0_SHIFT                  24

/* VCXO_1_RM :: RATE_RATIO :: DENOMINATOR [23:00] */
#define BCHP_VCXO_1_RM_RATE_RATIO_DENOMINATOR_MASK                 0x00ffffff
#define BCHP_VCXO_1_RM_RATE_RATIO_DENOMINATOR_SHIFT                0
#define BCHP_VCXO_1_RM_RATE_RATIO_DENOMINATOR_DEFAULT              0x00000001

/***************************************************************************
 *SAMPLE_INC - Rate Manager Output Rate Setting II
 ***************************************************************************/
/* VCXO_1_RM :: SAMPLE_INC :: NUMERATOR [31:08] */
#define BCHP_VCXO_1_RM_SAMPLE_INC_NUMERATOR_MASK                   0xffffff00
#define BCHP_VCXO_1_RM_SAMPLE_INC_NUMERATOR_SHIFT                  8
#define BCHP_VCXO_1_RM_SAMPLE_INC_NUMERATOR_DEFAULT                0x00000000

/* VCXO_1_RM :: SAMPLE_INC :: SAMPLE_INC [07:00] */
#define BCHP_VCXO_1_RM_SAMPLE_INC_SAMPLE_INC_MASK                  0x000000ff
#define BCHP_VCXO_1_RM_SAMPLE_INC_SAMPLE_INC_SHIFT                 0
#define BCHP_VCXO_1_RM_SAMPLE_INC_SAMPLE_INC_DEFAULT               0x00000001

/***************************************************************************
 *PHASE_INC - Rate Manager NCO Phase Increment
 ***************************************************************************/
/* VCXO_1_RM :: PHASE_INC :: reserved0 [31:24] */
#define BCHP_VCXO_1_RM_PHASE_INC_reserved0_MASK                    0xff000000
#define BCHP_VCXO_1_RM_PHASE_INC_reserved0_SHIFT                   24

/* VCXO_1_RM :: PHASE_INC :: PHASE_INC [23:00] */
#define BCHP_VCXO_1_RM_PHASE_INC_PHASE_INC_MASK                    0x00ffffff
#define BCHP_VCXO_1_RM_PHASE_INC_PHASE_INC_SHIFT                   0
#define BCHP_VCXO_1_RM_PHASE_INC_PHASE_INC_DEFAULT                 0x00000000

/***************************************************************************
 *INTEGRATOR_HI - Loop Filter Integrator HI Value
 ***************************************************************************/
/* VCXO_1_RM :: INTEGRATOR_HI :: reserved0 [31:05] */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_reserved0_MASK                0xffffffe0
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_reserved0_SHIFT               5

/* VCXO_1_RM :: INTEGRATOR_HI :: INTEGRATOR_HI [04:00] */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_INTEGRATOR_HI_MASK            0x0000001f
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_INTEGRATOR_HI_SHIFT           0
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_INTEGRATOR_HI_DEFAULT         0x00000000

/***************************************************************************
 *INTEGRATOR_LO - Loop Filter Integrator LO Value
 ***************************************************************************/
/* VCXO_1_RM :: INTEGRATOR_LO :: INTEGRATOR_LO [31:00] */
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_INTEGRATOR_LO_MASK            0xffffffff
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_INTEGRATOR_LO_SHIFT           0
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_INTEGRATOR_LO_DEFAULT         0x00000000

/***************************************************************************
 *OFFSET - Rate Manager Output Formatting
 ***************************************************************************/
/* VCXO_1_RM :: OFFSET :: OFFSET_ONLY [31:31] */
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_ONLY_MASK                     0x80000000
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_ONLY_SHIFT                    31
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_ONLY_DEFAULT                  0x00000000

/* VCXO_1_RM :: OFFSET :: OFFSET [30:00] */
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_MASK                          0x7fffffff
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_SHIFT                         0
#define BCHP_VCXO_1_RM_OFFSET_OFFSET_DEFAULT                       0x10000000

/***************************************************************************
 *FORMAT - Rate Manager Output Formatting
 ***************************************************************************/
/* VCXO_1_RM :: FORMAT :: reserved0 [31:28] */
#define BCHP_VCXO_1_RM_FORMAT_reserved0_MASK                       0xf0000000
#define BCHP_VCXO_1_RM_FORMAT_reserved0_SHIFT                      28

/* VCXO_1_RM :: FORMAT :: EN_UPDATE_VCXO_CTRL [27:27] */
#define BCHP_VCXO_1_RM_FORMAT_EN_UPDATE_VCXO_CTRL_MASK             0x08000000
#define BCHP_VCXO_1_RM_FORMAT_EN_UPDATE_VCXO_CTRL_SHIFT            27
#define BCHP_VCXO_1_RM_FORMAT_EN_UPDATE_VCXO_CTRL_DEFAULT          0x00000000
#define BCHP_VCXO_1_RM_FORMAT_EN_UPDATE_VCXO_CTRL_ENABLE           1
#define BCHP_VCXO_1_RM_FORMAT_EN_UPDATE_VCXO_CTRL_DISABLE          0

/* VCXO_1_RM :: FORMAT :: SELF_TEST [26:26] */
#define BCHP_VCXO_1_RM_FORMAT_SELF_TEST_MASK                       0x04000000
#define BCHP_VCXO_1_RM_FORMAT_SELF_TEST_SHIFT                      26
#define BCHP_VCXO_1_RM_FORMAT_SELF_TEST_DEFAULT                    0x00000000
#define BCHP_VCXO_1_RM_FORMAT_SELF_TEST_NORMAL_MODE                0
#define BCHP_VCXO_1_RM_FORMAT_SELF_TEST_SELF_TEST_MODE             1

/* VCXO_1_RM :: FORMAT :: SHIFT [25:24] */
#define BCHP_VCXO_1_RM_FORMAT_SHIFT_MASK                           0x03000000
#define BCHP_VCXO_1_RM_FORMAT_SHIFT_SHIFT                          24
#define BCHP_VCXO_1_RM_FORMAT_SHIFT_DEFAULT                        0x00000002

/* VCXO_1_RM :: FORMAT :: STABLE_COUNT [23:00] */
#define BCHP_VCXO_1_RM_FORMAT_STABLE_COUNT_MASK                    0x00ffffff
#define BCHP_VCXO_1_RM_FORMAT_STABLE_COUNT_SHIFT                   0
#define BCHP_VCXO_1_RM_FORMAT_STABLE_COUNT_DEFAULT                 0x00002710

/***************************************************************************
 *SKIP_REPEAT_CONTROL - Time Base Skip or Repeat Control Register
 ***************************************************************************/
/* VCXO_1_RM :: SKIP_REPEAT_CONTROL :: reserved0 [31:03] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_reserved0_MASK          0xfffffff8
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_reserved0_SHIFT         3

/* VCXO_1_RM :: SKIP_REPEAT_CONTROL :: SKIP_REPEAT_FOREVER [02:02] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_SKIP_REPEAT_FOREVER_MASK 0x00000004
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_SKIP_REPEAT_FOREVER_SHIFT 2
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_SKIP_REPEAT_FOREVER_DEFAULT 0x00000000
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_SKIP_REPEAT_FOREVER_ENABLE 1
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_SKIP_REPEAT_FOREVER_DISABLE 0

/* VCXO_1_RM :: SKIP_REPEAT_CONTROL :: MODE [01:01] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_MODE_MASK               0x00000002
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_MODE_SHIFT              1
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_MODE_DEFAULT            0x00000000
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_MODE_SKIP               1
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_MODE_REPEAT             0

/* VCXO_1_RM :: SKIP_REPEAT_CONTROL :: ENABLE [00:00] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_ENABLE_MASK             0x00000001
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_ENABLE_SHIFT            0
#define BCHP_VCXO_1_RM_SKIP_REPEAT_CONTROL_ENABLE_DEFAULT          0x00000000

/***************************************************************************
 *SKIP_REPEAT_GAP - Time Base Skip or Repeat Gap Count Register
 ***************************************************************************/
/* VCXO_1_RM :: SKIP_REPEAT_GAP :: reserved0 [31:20] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP_reserved0_MASK              0xfff00000
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP_reserved0_SHIFT             20

/* VCXO_1_RM :: SKIP_REPEAT_GAP :: GAP_COUNT [19:00] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP_GAP_COUNT_MASK              0x000fffff
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP_GAP_COUNT_SHIFT             0
#define BCHP_VCXO_1_RM_SKIP_REPEAT_GAP_GAP_COUNT_DEFAULT           0x00000000

/***************************************************************************
 *SKIP_REPEAT_NUMBER - Time Base Skip or Repeat Count Register
 ***************************************************************************/
/* VCXO_1_RM :: SKIP_REPEAT_NUMBER :: reserved0 [31:20] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER_reserved0_MASK           0xfff00000
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER_reserved0_SHIFT          20

/* VCXO_1_RM :: SKIP_REPEAT_NUMBER :: COUNT [19:00] */
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER_COUNT_MASK               0x000fffff
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER_COUNT_SHIFT              0
#define BCHP_VCXO_1_RM_SKIP_REPEAT_NUMBER_COUNT_DEFAULT            0x00000000

/***************************************************************************
 *INTEGRATOR_HI_STATUS - Loop Filter Integrator HI Status
 ***************************************************************************/
/* VCXO_1_RM :: INTEGRATOR_HI_STATUS :: reserved0 [31:05] */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS_reserved0_MASK         0xffffffe0
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS_reserved0_SHIFT        5

/* VCXO_1_RM :: INTEGRATOR_HI_STATUS :: INTEGRATOR_HI_STATUS [04:00] */
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS_INTEGRATOR_HI_STATUS_MASK 0x0000001f
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS_INTEGRATOR_HI_STATUS_SHIFT 0
#define BCHP_VCXO_1_RM_INTEGRATOR_HI_STATUS_INTEGRATOR_HI_STATUS_DEFAULT 0x00000000

/***************************************************************************
 *INTEGRATOR_LO_STATUS - Loop Filter Integrator LO Status
 ***************************************************************************/
/* VCXO_1_RM :: INTEGRATOR_LO_STATUS :: INTEGRATOR_LO_STATUS [31:00] */
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_STATUS_INTEGRATOR_LO_STATUS_MASK 0xffffffff
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_STATUS_INTEGRATOR_LO_STATUS_SHIFT 0
#define BCHP_VCXO_1_RM_INTEGRATOR_LO_STATUS_INTEGRATOR_LO_STATUS_DEFAULT 0x00000000

/***************************************************************************
 *STATUS - Rate Manager Status Register
 ***************************************************************************/
/* VCXO_1_RM :: STATUS :: reserved0 [31:29] */
#define BCHP_VCXO_1_RM_STATUS_reserved0_MASK                       0xe0000000
#define BCHP_VCXO_1_RM_STATUS_reserved0_SHIFT                      29

/* VCXO_1_RM :: STATUS :: MAX_ERR [28:15] */
#define BCHP_VCXO_1_RM_STATUS_MAX_ERR_MASK                         0x1fff8000
#define BCHP_VCXO_1_RM_STATUS_MAX_ERR_SHIFT                        15
#define BCHP_VCXO_1_RM_STATUS_MAX_ERR_DEFAULT                      0x00000000

/* VCXO_1_RM :: STATUS :: MIN_ERR [14:01] */
#define BCHP_VCXO_1_RM_STATUS_MIN_ERR_MASK                         0x00007ffe
#define BCHP_VCXO_1_RM_STATUS_MIN_ERR_SHIFT                        1
#define BCHP_VCXO_1_RM_STATUS_MIN_ERR_DEFAULT                      0x00000000

/* VCXO_1_RM :: STATUS :: RM_UNLOCK [00:00] */
#define BCHP_VCXO_1_RM_STATUS_RM_UNLOCK_MASK                       0x00000001
#define BCHP_VCXO_1_RM_STATUS_RM_UNLOCK_SHIFT                      0
#define BCHP_VCXO_1_RM_STATUS_RM_UNLOCK_LOCK                       0
#define BCHP_VCXO_1_RM_STATUS_RM_UNLOCK_UNLOCK                     1

/***************************************************************************
 *STATUS_CLEAR - Rate Manager Status Clear Register
 ***************************************************************************/
/* VCXO_1_RM :: STATUS_CLEAR :: reserved0 [31:01] */
#define BCHP_VCXO_1_RM_STATUS_CLEAR_reserved0_MASK                 0xfffffffe
#define BCHP_VCXO_1_RM_STATUS_CLEAR_reserved0_SHIFT                1

/* VCXO_1_RM :: STATUS_CLEAR :: RM_UNLOCK_CLR [00:00] */
#define BCHP_VCXO_1_RM_STATUS_CLEAR_RM_UNLOCK_CLR_MASK             0x00000001
#define BCHP_VCXO_1_RM_STATUS_CLEAR_RM_UNLOCK_CLR_SHIFT            0

#endif /* #ifndef BCHP_VCXO_1_RM_H__ */

/* End of File */
