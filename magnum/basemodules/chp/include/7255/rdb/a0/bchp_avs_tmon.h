/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2017 Broadcom. All rights reserved.
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
 * Date:           Generated on               Tue Mar 14 11:30:18 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1255
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#ifndef BCHP_AVS_TMON_H__
#define BCHP_AVS_TMON_H__

/***************************************************************************
 *AVS_TMON - AVS Temperature Monitor
 ***************************************************************************/
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS 0x204d1500 /* [RO][32] Indicate temperature measurement data and validity of data */
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET 0x204d1504 /* [RW][32] Enable over temperature reset */
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD 0x204d1508 /* [RW][32] Represent threshold for over temperature reset */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME 0x204d1510 /* [RW][32] No new temperature interrupt can be generated for interval defined by this idle time */
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES 0x204d1514 /* [RW][32] Enable high and low temperature interrupts */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS 0x204d1518 /* [RW][32] Represent thresholds for high and low temperature interrupts */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE 0x204d151c /* [RO][32] Temperature code associated with temperature interrupt event */
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE        0x204d1520 /* [RW][32] Enabling TP mode to use data from tp_in for testing TMON */
#define BCHP_AVS_TMON_SPARE_0                    0x204d1524 /* [RW][32] Spare register 0 for AVS TEMPERATURE MONITOR core */

/***************************************************************************
 *TEMPERATURE_MEASUREMENT_STATUS - Indicate temperature measurement data and validity of data
 ***************************************************************************/
/* AVS_TMON :: TEMPERATURE_MEASUREMENT_STATUS :: reserved0 [31:12] */
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_reserved0_MASK 0xfffff000
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_reserved0_SHIFT 12

/* AVS_TMON :: TEMPERATURE_MEASUREMENT_STATUS :: valid [11:11] */
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_valid_MASK    0x00000800
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_valid_SHIFT   11
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_valid_DEFAULT 0x00000000

/* AVS_TMON :: TEMPERATURE_MEASUREMENT_STATUS :: data [10:00] */
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_data_MASK     0x000007ff
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_data_SHIFT    0
#define BCHP_AVS_TMON_TEMPERATURE_MEASUREMENT_STATUS_data_DEFAULT  0x00000000

/***************************************************************************
 *ENABLE_OVER_TEMPERATURE_RESET - Enable over temperature reset
 ***************************************************************************/
/* AVS_TMON :: ENABLE_OVER_TEMPERATURE_RESET :: reserved0 [31:01] */
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET_reserved0_MASK 0xfffffffe
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET_reserved0_SHIFT 1

/* AVS_TMON :: ENABLE_OVER_TEMPERATURE_RESET :: enable [00:00] */
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET_enable_MASK    0x00000001
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET_enable_SHIFT   0
#define BCHP_AVS_TMON_ENABLE_OVER_TEMPERATURE_RESET_enable_DEFAULT 0x00000000

/***************************************************************************
 *TEMPERATURE_RESET_THRESHOLD - Represent threshold for over temperature reset
 ***************************************************************************/
/* AVS_TMON :: TEMPERATURE_RESET_THRESHOLD :: reserved0 [31:11] */
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD_reserved0_MASK   0xfffff800
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD_reserved0_SHIFT  11

/* AVS_TMON :: TEMPERATURE_RESET_THRESHOLD :: threshold [10:00] */
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD_threshold_MASK   0x000007ff
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD_threshold_SHIFT  0
#define BCHP_AVS_TMON_TEMPERATURE_RESET_THRESHOLD_threshold_DEFAULT 0x0000047e

/***************************************************************************
 *TEMPERATURE_INTERRUPT_IDLE_TIME - No new temperature interrupt can be generated for interval defined by this idle time
 ***************************************************************************/
/* AVS_TMON :: TEMPERATURE_INTERRUPT_IDLE_TIME :: reserved0 [31:08] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME_reserved0_MASK 0xffffff00
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME_reserved0_SHIFT 8

/* AVS_TMON :: TEMPERATURE_INTERRUPT_IDLE_TIME :: idle_time [07:00] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME_idle_time_MASK 0x000000ff
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME_idle_time_SHIFT 0
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_IDLE_TIME_idle_time_DEFAULT 0x00000040

/***************************************************************************
 *ENABLE_TEMPERATURE_INTERRUPT_SOURCES - Enable high and low temperature interrupts
 ***************************************************************************/
/* AVS_TMON :: ENABLE_TEMPERATURE_INTERRUPT_SOURCES :: reserved0 [31:02] */
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_reserved0_MASK 0xfffffffc
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_reserved0_SHIFT 2

/* AVS_TMON :: ENABLE_TEMPERATURE_INTERRUPT_SOURCES :: enable_high [01:01] */
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_high_MASK 0x00000002
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_high_SHIFT 1
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_high_DEFAULT 0x00000000

/* AVS_TMON :: ENABLE_TEMPERATURE_INTERRUPT_SOURCES :: enable_low [00:00] */
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_low_MASK 0x00000001
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_low_SHIFT 0
#define BCHP_AVS_TMON_ENABLE_TEMPERATURE_INTERRUPT_SOURCES_enable_low_DEFAULT 0x00000000

/***************************************************************************
 *TEMPERATURE_INTERRUPT_THRESHOLDS - Represent thresholds for high and low temperature interrupts
 ***************************************************************************/
/* AVS_TMON :: TEMPERATURE_INTERRUPT_THRESHOLDS :: reserved0 [31:27] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_reserved0_MASK 0xf8000000
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_reserved0_SHIFT 27

/* AVS_TMON :: TEMPERATURE_INTERRUPT_THRESHOLDS :: high_threshold [26:16] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_high_threshold_MASK 0x07ff0000
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_high_threshold_SHIFT 16
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_high_threshold_DEFAULT 0x0000049a

/* AVS_TMON :: TEMPERATURE_INTERRUPT_THRESHOLDS :: reserved1 [15:11] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_reserved1_MASK 0x0000f800
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_reserved1_SHIFT 11

/* AVS_TMON :: TEMPERATURE_INTERRUPT_THRESHOLDS :: low_threshold [10:00] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_low_threshold_MASK 0x000007ff
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_low_threshold_SHIFT 0
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_THRESHOLDS_low_threshold_DEFAULT 0x000004a8

/***************************************************************************
 *TEMPERATURE_INTERRUPT_TEMPERATURE - Temperature code associated with temperature interrupt event
 ***************************************************************************/
/* AVS_TMON :: TEMPERATURE_INTERRUPT_TEMPERATURE :: reserved0 [31:10] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE_reserved0_MASK 0xfffffc00
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE_reserved0_SHIFT 10

/* AVS_TMON :: TEMPERATURE_INTERRUPT_TEMPERATURE :: code [09:00] */
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE_code_MASK  0x000003ff
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE_code_SHIFT 0
#define BCHP_AVS_TMON_TEMPERATURE_INTERRUPT_TEMPERATURE_code_DEFAULT 0x00000000

/***************************************************************************
 *TP_TMON_TEST_ENABLE - Enabling TP mode to use data from tp_in for testing TMON
 ***************************************************************************/
/* AVS_TMON :: TP_TMON_TEST_ENABLE :: reserved0 [31:01] */
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE_reserved0_MASK           0xfffffffe
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE_reserved0_SHIFT          1

/* AVS_TMON :: TP_TMON_TEST_ENABLE :: tp_mode_en [00:00] */
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE_tp_mode_en_MASK          0x00000001
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE_tp_mode_en_SHIFT         0
#define BCHP_AVS_TMON_TP_TMON_TEST_ENABLE_tp_mode_en_DEFAULT       0x00000000

/***************************************************************************
 *SPARE_0 - Spare register 0 for AVS TEMPERATURE MONITOR core
 ***************************************************************************/
/* AVS_TMON :: SPARE_0 :: spare [31:00] */
#define BCHP_AVS_TMON_SPARE_0_spare_MASK                           0xffffffff
#define BCHP_AVS_TMON_SPARE_0_spare_SHIFT                          0
#define BCHP_AVS_TMON_SPARE_0_spare_DEFAULT                        0x00000000

#endif /* #ifndef BCHP_AVS_TMON_H__ */

/* End of File */
