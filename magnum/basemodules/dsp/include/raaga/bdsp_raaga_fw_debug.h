/******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *****************************************************************************/



#ifndef __BDSP_RAAGA_DBG_MODULES_ID_DEFINES_H
#define __BDSP_RAAGA_DBG_MODULES_ID_DEFINES_H


/* Define all the modules name here */

#define BDSP_RAAGA_DBG_MODULE_CPT_ID                    0x1
#define BDSP_RAAGA_DBG_MODULE_DEC_TSM_ID                0x2
#define BDSP_RAAGA_DBG_MODULE_DEC_IDS_ID                0x3
#define BDSP_RAAGA_DBG_MODULE_DEC_COMMON_DS_ID      0x4
#define BDSP_RAAGA_DBG_MODULE_DEC_MPEG1_DS_ID           0x5
#define BDSP_RAAGA_DBG_MODULE_DEC_ACCHE_LOAS_DS_ID  0x6
#define BDSP_RAAGA_DBG_MODULE_DEC_ACCHE_ADTS_DS_ID  0x7
#define BDSP_RAAGA_DBG_MODULE_DEC_WMA_DS_ID         0x8
#define BDSP_RAAGA_DBG_MODULE_DEC_WMAPRO_DS_ID      0x9

#define BDSP_RAAGA_DBG_MODULE_ATM_ID                    0xA
#define BDSP_RAAGA_DBG_MODULE_DECODE_TASK_ID            0xB
#define BDSP_RAAGA_DBG_MODULE_ENCODE_TASK_ID            0xC
#define BDSP_RAAGA_DBG_MODULE_DECODE_DDP_ID         0xD
#define BDSP_RAAGA_DBG_MODULE_DECODE_MPEG1_ID           0xE
#define BDSP_RAAGA_DBG_MODULE_DEC_DDP_DS_ID         0xF
#define BDSP_RAAGA_DBG_MODULE_CLEAR_VOICE_ID            0x10
#define BDSP_RAAGA_DBG_MODULE_ALGO_LIB_ID               0x11





#define BDSP_RAAGA_DBG_MSG_DMA_LOG_ID                           0x0
    /*** Library functions can have their Msg IDs within the range 0 to 0xfff*/
#define BDSP_RAAGA_DBG_MSG_ID_ATM_TRACE_START_ID                0x1000
#define BDSP_RAAGA_DBG_MSG_ID_ATM_HOST2DSP_CMD              0x1001
#define BDSP_RAAGA_DBG_MSG_ID_ATM_HOST2DSP_CMD_BFR          0x1002
#define BDSP_RAAGA_DBG_MSG_ID_ATM_DSP2HOST_MSG          0x1003
#define BDSP_RAAGA_DBG_MSG_ID_ATM_DSP2HOST_MSG_ERROR        0x1004
#define BDSP_RAAGA_DBG_MSG_ID_ATM_DSP2HOST_CMD_RECVD        0x1005
    /*Define ATM Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_ATM_TRACE_END_ID              0x1FFF



#define BDSP_RAAGA_DBG_MSG_ID_CPT_TRACE_START_ID                0x2000
    /*Define CPT Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_CPT_HOST2DSPQ_PHY_ADDR            0x2001
#define BDSP_RAAGA_DBG_MSG_ID_TASK_DEADLINE                 0x2002
#define BDSP_RAAGA_DBG_MSG_ID_ATM_BUFFER                        0x2003
#define BDSP_RAAGA_DBG_MSG_ID_FIND_TASK_DEADLINE_TRACE_START    0x2004
#define BDSP_RAAGA_DBG_MSG_ID_FIND_TASK_DEADLINE_TRACE_END  0x2005
#define BDSP_RAAGA_DBG_MSG_ID_HIGHEST_DEADLINE              0x2006
#define BDSP_RAAGA_DBG_MSG_ID_ATM2CPT_COMMAND                   0x2007

#define BDSP_RAAGA_DBG_MSG_ID_CPT_TRACE_END_ID              0x2FFF



#define BDSP_RAAGA_DBG_MSG_ID_PI_TRACE_START_ID             0x3000
    /*Define PI Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_PI_TRACE_END_ID                   0x3FFF





/* Add all your messages ID for decoder TSM here */
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_TRACE_START_ID            0x4000
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_IP_STRUCT               0x4001
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_OP_STRUCT               0x4002
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_I32DIFF                 0x4003
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_CONFIG_STRUCT           0x4004
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_I32TIME                 0x4005
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_OP_LOG                  0x4006


#define BDSP_RAAGA_DBG_MSG_ID_DEC_TSM_TRACE_END_ID          0x4FFF





/* Add all your messages ID for decoder IDS here */
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_TRACE_START_ID            0x5000
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_ENTRY_CDB_BUF_INFO        0x5001
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_ENTRY_ITB_BUF_INFO        0x5002
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_CDB_CONSUMED        0x5003
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_ITB_CONSUMED        0x5004
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_SYNC_INFO           0x5005
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_TSM_PARAMS      0x5006
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_DATA_STATUS     0x5007
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_DS_TO_ACCUM     0x5008
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_TIMEOUT         0x5009
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_CMPT_DSCRD_SIZE           0x500A
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_START_CDB_CONSUMED        0x500B
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_START_ITB_CONSUMED        0x500C
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_START_IP_BUF_CFG      0x500D
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_START_TIME                0x500E
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_OP_SAMPLING_FREQ      0x500F
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_NUM_ACCUM_FRAME           0x5010
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_NUM_FRAME             0x5011
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_END_CDB_CONSUMED      0x5012
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_END_ITB_CONSUMED      0x5013
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_GEN_BUF_OP                0x5014
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_NEXT_STATE_ACCUM        0x5015
#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_DS_OP_LOG               0x5016
#define BDSP_RAAGA_DBG_MSG_ID_CDB_DUMP_MSG_ID                 0x5017
#define BDSP_RAAGA_DBG_MSG_ID_ITB_DUMP_MSG_ID                 0x5018

#define BDSP_RAAGA_DBG_MSG_ID_DEC_IDS_TRACE_END_ID          0x5FFF


/* Add AC3/DDP decoder data sync messages here */
#define BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID      0x6000
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_INTERFRAME_STRUCT_DMA_IN                            (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x01)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_INIT_INTERFRAME                                 (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x02)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_DATASYNC_BYPASS_TRUE                                (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x03)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_INVALID_BIT_OFFSET_INPUT                            (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x04)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_UNDESIRED_CDB_LEVEL                             (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x05)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_OUTOFSYNC_WHILE_ACCUM                     (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x06)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_DMA_IN_VAL_TARGET                         (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x07)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_TARGET_SYNC_STATUS_INVALID                    (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x08)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_FUTURE_PTS_RECVD                                    (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x09)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_DATASYNC_OP_STRUCT                              (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0A)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ACCUM_STATUS_INVALID                                (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0B)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_INTERFRAME_STRUCT_DMA_OUT                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0C)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_DMA_OUTOFSYNC_IN_SEARCH_BASE_SYNC         (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0D)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_DMA_INSYNC_IN_SEARCH_BASE_SYNC                (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0E)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_UNUSUAL_DMA_SIZE                                    (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x0F)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_DMA_ITB_DOWNLOAD                          (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x10)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_UNUSUAL_NUM_PTS_ENTRIES                         (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x11)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_UNUSUAL_NUM_PCR_ENTRIES                         (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x12)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ERROR_INVALID_DMA_SIZE_ITB                      (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x13)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_LOCATE_TARGET_WITH_NEGATIVE_OFFSET              (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x14)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_IN_DATASYNC_TOPLEVEL                                (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x15)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_OUT_DATASYNC_TOPLEVEL                               (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x16)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CDB_OVERFLOW                                        (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x17)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ITB_OVERFLOW                                        (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x18)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_TSM_ISSUE_LOGS                                  (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x19)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CDB_PTRS_SYNC_IP_BFR_UPDT                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1A)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CDB_PTRS_ENTRY_DS                                   (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1B)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_TIME_DIFFS                                      (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1C)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_FRM_ADV                                                      (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1D)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_WMAPRO_PASSTHRU                                              (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1E)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CDB_PTRS_EXIT_DS                                    (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x1F)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CDB_PTRS_SYNC_IP_AFT_UPDT                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x20)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ITB_PTRS_SYNC_IP_BFR_UPDT                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x21)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ITB_PTRS_ENTRY_DS                                   (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x22)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ITB_PTRS_EXIT_DS                                    (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x23)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_ITB_PTRS_SYNC_IP_AFT_UPDT                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x24)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_CODED_PTS                                           (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x25)
#define BDSP_RAAGA_DBG_MSG_ID_DATASYNC_COMMON_TRACE_INTERPOLATING_PTS                                   (BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_START_ID + 0x26)

#define BDSP_RAAGA_DBG_MSG_ID_DEC_COMMON_DS_TRACE_END_ID            0x6FFF


/* Add AC3/DDP decoder data sync messages here */
#define BDSP_RAAGA_DBG_MSG_ID_DEC_MPEG1_DS_TRACE_START_ID       0x7000


#define BDSP_RAAGA_DBG_MSG_ID_DEC_MPEG1_DS_TRACE_END_ID     0x7FFF

#define BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID     0x8000
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_CMPT_DISCARD_SIZE                       (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x01)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_DISCARDING_DEPENDENT_FRAMES                (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x02)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_CMPT_DISCARD_SIZE                  (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x03)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_SEARCH_BASE_SYNC                        (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x05)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_NODATA_BASE_SYNC                           (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x07)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_SEARCH_SYNC_RESULT                     (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x08)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_DATA_AVAILABLE_NEGATIVE_FOR_VAL_FRAME  (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x09)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_SEARCH_BASE_SYNC                       (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0A)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_BASE_SYNC_STATUS_INVALID         (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0B)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_VAL_FRAME_PARAMS                        (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0C)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_NODATA_VAL_FRAME                           (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0D)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_DMA_VAL_FRAME                        (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0E)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_INVALID_FRAME                          (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x0F)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_INVALID_BASE_NUMBLOCKS                 (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x10)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_DATA_AVAILABLE_NEGATIVE_VAL_TARGET     (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x11)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_VAL_FRAME_PARAMS                       (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x12)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_EXTRACT_TARGET_PARAMS                   (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x13)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_NODATA_EXTRACT_TARGET_PARAMS               (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x14)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_SYNC_LOCK                              (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x15)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_DMA_EXTRACT_TARGET_PARAMS            (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x16)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_INVALID_TARGET_NUMBLOCKS                   (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x17)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_EXTRACT_TARGET_PARAMS              (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x18)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_SET_ACCUM                               (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x19)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_SAMPLE_RATE_CHANGE                     (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1A)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_DEPENDENT_FRAME                            (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1B)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_CONVSYNC_ONE_TO_ZERO                       (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1C)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_MAX_SAMPLES_HIT                            (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1D)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_SET_ACCUM                          (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1E)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_INVALID_FRAME_SIZE_COD                 (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x1F)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_INVALID_ARGS_FOR_LOOKUP_FRAME_SIZE     (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x20)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_BSI_PARSER                          (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x21)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_BSI_PARSER_ERR                         (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x22)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_INVALID_SYNCWORD_BSI_PARSER      (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x23)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_BSI_PARSER                         (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x24)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_IN_FRAME_ADVANCE                           (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x25)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_DMA_SIZE_FRAME_ADVANCE               (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x26)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_ERROR_DMA_FRAME_ADVANCE                    (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x27)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DATASYNC_TRACE_OUT_FRAME_ADVANCE                      (BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_START_ID + 0x28)
#define BDSP_RAAGA_DBG_MSG_ID_DEC_DDP_DS_TRACE_END_ID           0x8FFF







#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_TRACE_START_ID           0xDEC00000
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_ALGO_OP_DATA         0xDEC00001
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_NUM_ACCUM_FRAME        0xDEC00002
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_PROFILE_NODES            0xDEC00003
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_INDEP_DELAY_RD_POINTERS_LOG (uint32_t)0xDEC00004
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_STACK_USAGE         (uint32_t)0xDEC00005

    /*Define Decode Task Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_TRACE_END_ID         0xDEC00fff
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_POINTERS_LOG1            0xDEC00ffe
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_POINTERS_LOG2            0xDEC00ffd
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_POINTERS_LOG3            0xDEC00ffc
#define BDSP_RAAGA_DBG_MSG_ID_DEC_TASK_LOW_DELAY                0xDEC00ffb
#define BDSP_RAAGA_DBG_MSG_ID_TIME_STAMP                        0xDEC00ffa
#define BDSP_RAAGA_DBG_MSG_ID_RBUF_CAP                      0xDEC00ff9



#define BDSP_RAAGA_DBG_MSG_ID_DDP_DECODER_START_ID          0xDEC01000
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DECODER_FRAME_INFO_ID     (BDSP_RAAGA_DBG_MSG_ID_DDP_DECODER_START_ID + 0x1)
#define BDSP_RAAGA_DBG_MSG_ID_DDP_METADATA_LOGS             0xDEC01002
    /*Define DDP Decoder Related Msg ID Defines within this range */
    /*Use the Msg ID's for different stages also within this range*/
#define BDSP_RAAGA_DBG_MSG_ID_DDP_DECODER_END_ID                0xDEC01fff



#define BDSP_RAAGA_DBG_MSG_ID_MP3_DECODER_START_ID          0xDEC02000
    /*Define DDP Decoder Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_MP3_DECODER_END_ID     0xDEC02fff



#define BDSP_RAAGA_DBG_MSG_ID_ENC_TASK_TRACE_START_ID           0xECE00000
    /*Define Encode Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_ENC_TASK_TRACE_END_ID         0xECE00fff



#define BDSP_RAAGA_DBG_MSG_ID_AAC_ENCODER_START_ID          0xECE01000
    /*Define Encode Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_AAC_ENCODER_END_ID                0xECE01fff



#define BDSP_RAAGA_DBG_MSG_ID_DTS_ENCODER_START_ID          0xECE02000
    /*Define Encode Related Msg ID Defines within this range */
#define BDSP_RAAGA_DBG_MSG_ID_DTS_ENCODER_END_ID                0xECE02fff

#define BDSP_RAAGA_DBG_MSG_ID_CLEAR_VOICE_ID                    0xDEC03000

#endif

#ifndef _BDSP_RAAGA_DEBUG_H_
#define _BDSP_RAAGA_DEBUG_H_

/* DEBUG typedefs are defined here */

typedef struct
{
    uint32_t ui32SyncPattern;       /* This is a constant sync pattern and is set to 0xFACCBABE - This pattern can be used as a seek point when the parse loses sync*/
    uint32_t ui32MsgSizeInBytes;    /* This field gives the size of the message in bytes, including the message header size*/
    uint32_t ui32MessageID;         /* This field gives the message id. Message id is used to differentiate between messages from the same Module*/
    uint32_t ui32TaskID;            /* This field identifies the task id of the audio context from which the message originates. The task ids start from 15 (0xF) and are incremented by 1 for every new context that is started in the DSP*/
    uint32_t ui32NodeID;            /* This field gives the node id for the module which sends the message Ex: If there are 2 SRC (sample rate converter) nodes in a task, then the node id can be used to distinguish between the messages from the 1st instance of the SRC module and the 2nd instance*/
    uint32_t ui32ModuleID;          /* This field gives the module ID of the module which sends the message. Examples of different modules would be: TSM, FrameSync, Decode Task, DDP decode, SRS PostProc, DTS Encode, etc ...*/
    uint32_t ui32Time45KHzTickBase; /* This field gives the time-stamp of the message in 45kHz ticks*/

}BDSP_RAAGA_DBG_MSG_HEADER;


#endif
