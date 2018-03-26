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
 * Date:           Generated on               Tue Mar 14 11:31:57 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                unknown
 *                 RDB.pm                     1255
 *                 generate_int_id.pl         1.0
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/generate_int_id.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#include "bchp.h"
#include "bchp_hdmi_tx_hae_intr2_0.h"

#ifndef BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_H__
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_H__

#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_AES_FRAME_NUMBER_ROLLOVER BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_AES_FRAME_NUMBER_ROLLOVER_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MBOX_UPDATE_0 BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MBOX_UPDATE_0_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MBOX_UPDATE_1 BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MBOX_UPDATE_1_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MSG_RDY_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MSG_RDY_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MSG_RDY_PENDING_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_MSG_RDY_PENDING_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_OK_TO_ENC_EN BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_OK_TO_ENC_EN_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_RD_MSG_START BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_RD_MSG_START_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READY_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READY_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_FIFO_OVERFLOW_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_FIFO_OVERFLOW_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_FIFO_UNDERFLOW_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_FIFO_UNDERFLOW_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_MESSAGE_RDY_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_MESSAGE_RDY_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_MESSAGE_READY BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_READ_MESSAGE_READY_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_REAUTH_REQ_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_REAUTH_REQ_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_SPARE_0 BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_SPARE_0_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_SPARE_1 BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_SPARE_1_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_TIMEOUT_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_TIMEOUT_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_FIFO_OVERFLOW_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_FIFO_OVERFLOW_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_FIFO_UNDERFLOW_INTR BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_FIFO_UNDERFLOW_INTR_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_MESSAGE_COMPLETE BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WRITE_MESSAGE_COMPLETE_SHIFT)
#define BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WR_MSG_START BCHP_INT_ID_CREATE(BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS, BCHP_HDMI_TX_HAE_INTR2_0_CPU_STATUS_WR_MSG_START_SHIFT)

#endif /* #ifndef BCHP_INT_ID_HDMI_TX_HAE_INTR2_0_H__ */

/* End of File */
