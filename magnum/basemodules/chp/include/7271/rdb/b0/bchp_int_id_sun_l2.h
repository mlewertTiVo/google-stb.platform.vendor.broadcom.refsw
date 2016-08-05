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
 * Date:           Generated on               Thu Jun 16 14:48:13 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     1009
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/generate_int_id.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#include "bchp.h"
#include "bchp_sun_l2.h"

#ifndef BCHP_INT_ID_SUN_L2_H__
#define BCHP_INT_ID_SUN_L2_H__

#define BCHP_INT_ID_AUX_INTR                  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_AUX_INTR_SHIFT)
#define BCHP_INT_ID_BBSI_RG_BRIDGE_ERROR_INTR BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_BBSI_RG_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_BT_GR_BRIDGE_ERROR_INTR   BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_BT_GR_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_CLK_GEN_BRIDGE_ERROR_INTR BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_CLK_GEN_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_GISB_BREAKPOINT_ERROR_INTR BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_GISB_BREAKPOINT_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_GISB_TEA_INTR             BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_GISB_TEA_INTR_SHIFT)
#define BCHP_INT_ID_GISB_TIMEOUT_INTR         BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_GISB_TIMEOUT_INTR_SHIFT)
#define BCHP_INT_ID_JTAG_GISB_RG_ERROR        BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_JTAG_GISB_RG_ERROR_SHIFT)
#define BCHP_INT_ID_MPM_TOP_GR_BRIDGE_ERROR_INTR BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_MPM_TOP_GR_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_RF4CE_GR_BRIDGE_ERROR_INTR BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_RF4CE_GR_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_SERS_CLK_ERR              BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SERS_CLK_ERR_SHIFT)
#define BCHP_INT_ID_SERS_PKT_ERR              BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SERS_PKT_ERR_SHIFT)
#define BCHP_INT_ID_SERS_R_PKT                BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SERS_R_PKT_SHIFT)
#define BCHP_INT_ID_SSP_RG_BRIDGE_ERROR_INTR  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SSP_RG_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_SUN_L2_SPARE_INTR_30      BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SPARE_INTR_30_SHIFT)
#define BCHP_INT_ID_SUN_L2_SPARE_INTR_31      BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SPARE_INTR_31_SHIFT)
#define BCHP_INT_ID_SYS_GR_BRIDGE_ERROR_INTR  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_SYS_GR_BRIDGE_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_TPCAP_FIFO_OVERFLOW       BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_TPCAP_FIFO_OVERFLOW_SHIFT)
#define BCHP_INT_ID_TPCAP_LOWER_THRESHOLD     BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_TPCAP_LOWER_THRESHOLD_SHIFT)
#define BCHP_INT_ID_TPCAP_UPPER_THRESHOLD     BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_TPCAP_UPPER_THRESHOLD_SHIFT)
#define BCHP_INT_ID_VTRAP_WARNING_0           BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_VTRAP_WARNING_0_SHIFT)
#define BCHP_INT_ID_VTRAP_WARNING_1           BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_VTRAP_WARNING_1_SHIFT)
#define BCHP_INT_ID_WATCHDOG_0_DISABLE_INTR   BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_0_DISABLE_INTR_SHIFT)
#define BCHP_INT_ID_WATCHDOG_0_TIMEOUT_INTR   BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_0_TIMEOUT_INTR_SHIFT)
#define BCHP_INT_ID_WATCHDOG_0_WR_ERROR_INTR  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_0_WR_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_WATCHDOG_1_DISABLE_INTR   BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_1_DISABLE_INTR_SHIFT)
#define BCHP_INT_ID_WATCHDOG_1_TIMEOUT_INTR   BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_1_TIMEOUT_INTR_SHIFT)
#define BCHP_INT_ID_WATCHDOG_1_WR_ERROR_INTR  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_WATCHDOG_1_WR_ERROR_INTR_SHIFT)
#define BCHP_INT_ID_cmd_fifo_mode_SERS_FIFO_FULL BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_cmd_fifo_mode_SERS_FIFO_FULL_SHIFT)
#define BCHP_INT_ID_cmd_fifo_mode_SERS_FIFO_THRESHOLD BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_cmd_fifo_mode_SERS_FIFO_THRESHOLD_SHIFT)
#define BCHP_INT_ID_cmd_fifo_mode_SERS_W_PKT  BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_cmd_fifo_mode_SERS_W_PKT_SHIFT)
#define BCHP_INT_ID_mapped_buffer_mode_SERS_W_PKT1 BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_mapped_buffer_mode_SERS_W_PKT1_SHIFT)
#define BCHP_INT_ID_mapped_buffer_mode_SERS_W_PKT2 BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_mapped_buffer_mode_SERS_W_PKT2_SHIFT)
#define BCHP_INT_ID_mapped_buffer_mode_SERS_W_PKT3 BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_mapped_buffer_mode_SERS_W_PKT3_SHIFT)
#define BCHP_INT_ID_mapped_buffer_mode_SERS_W_PKT4 BCHP_INT_ID_CREATE(BCHP_SUN_L2_CPU_STATUS, BCHP_SUN_L2_CPU_STATUS_mapped_buffer_mode_SERS_W_PKT4_SHIFT)

#endif /* #ifndef BCHP_INT_ID_SUN_L2_H__ */

/* End of File */
