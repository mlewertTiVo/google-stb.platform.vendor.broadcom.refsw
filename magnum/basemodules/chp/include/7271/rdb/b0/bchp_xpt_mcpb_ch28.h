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
 * Date:           Generated on               Thu Jun 16 14:47:13 2016
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

#ifndef BCHP_XPT_MCPB_CH28_H__
#define BCHP_XPT_MCPB_CH28_H__

/***************************************************************************
 *XPT_MCPB_CH28 - MCPB Channel 28 Configuration
 ***************************************************************************/
#define BCHP_XPT_MCPB_CH28_DMA_DESC_CONTROL      0x20a74400 /* [RW] MCPB Channel x Descriptor control information */
#define BCHP_XPT_MCPB_CH28_DMA_DATA_CONTROL      0x20a74404 /* [RW] MCPB Channel x Data control information */
#define BCHP_XPT_MCPB_CH28_DMA_CURR_DESC_ADDRESS 0x20a74408 /* [RW] MCPB Channel x Current Descriptor address information */
#define BCHP_XPT_MCPB_CH28_DMA_NEXT_DESC_ADDRESS 0x20a7440c /* [RW] MCPB Channel x Next Descriptor address information */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_BASE_ADDRESS_UPPER 0x20a74410 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_BASE_ADDRESS_LOWER 0x20a74414 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_END_ADDRESS_UPPER 0x20a74418 /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_END_ADDRESS_LOWER 0x20a7441c /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_CURR_RD_ADDRESS_UPPER 0x20a74420 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_CURR_RD_ADDRESS_LOWER 0x20a74424 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_WRITE_ADDRESS_UPPER 0x20a74428 /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH28_DMA_BUFF_WRITE_ADDRESS_LOWER 0x20a7442c /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH28_DMA_STATUS_0          0x20a74430 /* [RW] MCPB Channel x Status information */
#define BCHP_XPT_MCPB_CH28_DMA_STATUS_1          0x20a74434 /* [RW] MCPB Channel x CRC value */
#define BCHP_XPT_MCPB_CH28_DMA_STATUS_2          0x20a74438 /* [RW] MCPB Channel x Manual mode status */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_STATUS_0 0x20a7443c /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_STATUS_1 0x20a74440 /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_CURR_DESC_ADDR 0x20a74444 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_CURR_DATA_ADDR_UPPER 0x20a74448 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_CURR_DATA_ADDR_LOWER 0x20a7444c /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_NEXT_TIMESTAMP 0x20a74450 /* [RW] MCPB Channel x Descriptor Slot 0 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT0_PKT2PKT_TIMESTAMP_DELTA 0x20a74454 /* [RW] MCPB Channel x Descriptor Slot 0 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_STATUS_0 0x20a74458 /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_STATUS_1 0x20a7445c /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_CURR_DESC_ADDR 0x20a74460 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_CURR_DATA_ADDR_UPPER 0x20a74464 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_CURR_DATA_ADDR_LOWER 0x20a74468 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_NEXT_TIMESTAMP 0x20a7446c /* [RW] MCPB Channel x Descriptor Slot 1 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH28_DMA_DESC_SLOT1_PKT2PKT_TIMESTAMP_DELTA 0x20a74470 /* [RW] MCPB Channel x Descriptor Slot 1 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH28_SP_PKT_LEN            0x20a74474 /* [RW] MCPB Channel x Packet length control */
#define BCHP_XPT_MCPB_CH28_SP_PARSER_CTRL        0x20a74478 /* [RW] MCPB Channel x Parser control */
#define BCHP_XPT_MCPB_CH28_SP_PARSER_CTRL1       0x20a7447c /* [RW] MCPB Channel x Parser control 1 */
#define BCHP_XPT_MCPB_CH28_SP_TS_CONFIG          0x20a74480 /* [RW] MCPB Channel x TS Configuration */
#define BCHP_XPT_MCPB_CH28_SP_PES_ES_CONFIG      0x20a74484 /* [RW] MCPB Channel x PES and ES Configuration */
#define BCHP_XPT_MCPB_CH28_SP_PES_SYNC_COUNTER   0x20a74488 /* [RW] MCPB Channel x PES Sync counter */
#define BCHP_XPT_MCPB_CH28_SP_ASF_CONFIG         0x20a7448c /* [RW] MCPB Channel x ASF Configuration */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_0        0x20a74490 /* [RW] MCPB Channel x Stream Processor State Register 0 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_1        0x20a74494 /* [RW] MCPB Channel x Stream Processor State Register 1 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_2        0x20a74498 /* [RW] MCPB Channel x Stream Processor State Register 2 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_3        0x20a7449c /* [RW] MCPB Channel x Stream Processor State Register 3 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_4        0x20a744a0 /* [RW] MCPB Channel x Stream Processor State Register 4 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_5        0x20a744a4 /* [RW] MCPB Channel x Stream Processor State Register 5 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_6        0x20a744a8 /* [RW] MCPB Channel x Stream Processor State Register 6 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_7        0x20a744ac /* [RW] MCPB Channel x Stream Processor State Register 7 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_8        0x20a744b0 /* [RW] MCPB Channel x Stream Processor State Register 8 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_9        0x20a744b4 /* [RW] MCPB Channel x Stream Processor State Register 9 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_10       0x20a744b8 /* [RW] MCPB Channel x Stream Processor State Register 10 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_11       0x20a744bc /* [RW] MCPB Channel x Stream Processor State Register 11 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_12       0x20a744c0 /* [RW] MCPB Channel x Stream Processor State Register 12 */
#define BCHP_XPT_MCPB_CH28_SP_STATE_REG_13       0x20a744c4 /* [RW] MCPB Channel x Stream Processor State Register 13 */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF_CTRL        0x20a744c8 /* [RW] MCPB Channel x Burst buffer control */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF_CRC         0x20a744cc /* [RW] MCPB Channel x Current CRC value */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF0_RW_STATUS  0x20a744d0 /* [RW] MCPB Channel x Burst buffer 0 data specific information */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF0_RO_STATUS  0x20a744d4 /* [RW] MCPB Channel x Burst buffer 0 control specific information */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF1_RW_STATUS  0x20a744d8 /* [RW] MCPB Channel x Burst buffer 1 data specific information */
#define BCHP_XPT_MCPB_CH28_DMA_BBUFF1_RO_STATUS  0x20a744dc /* [RW] MCPB Channel x Burst buffer 1 control specific information */
#define BCHP_XPT_MCPB_CH28_TMEU_BLOCKOUT_CTRL    0x20a744e0 /* [RW] MCPB Channel x Blockout control information */
#define BCHP_XPT_MCPB_CH28_TMEU_NEXT_BO_MON      0x20a744e4 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_CTRL      0x20a744e8 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH28_TMEU_REF_DIFF_VALUE_TS_MBOX 0x20a744ec /* [RW] MCPB Channel x reference difference value and next Timestamp information */
#define BCHP_XPT_MCPB_CH28_TMEU_TS_ERR_BOUND_EARLY 0x20a744f0 /* [RW] MCPB Channel x TS error bound early information */
#define BCHP_XPT_MCPB_CH28_TMEU_TS_ERR_BOUND_LATE 0x20a744f4 /* [RW] MCPB Channel x TS error bound late information */
#define BCHP_XPT_MCPB_CH28_TMEU_NEXT_GPC_MON     0x20a744f8 /* [RW] MCPB Channel x next Global Pacing Counter and Timestamp monitor information */
#define BCHP_XPT_MCPB_CH28_TMEU_REF_DIFF_VALUE_SIGN 0x20a744fc /* [RW] MCPB Channel x reference difference value sign information */
#define BCHP_XPT_MCPB_CH28_TMEU_PES_PACING_CTRL  0x20a74500 /* [RW] MCPB Channel x PES pacing control information */
#define BCHP_XPT_MCPB_CH28_TMEU_SLOT_STATUS      0x20a74504 /* [RW] MCPB Channel x Slot 0 and Slot 1 information */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_SLOT0_REG1 0x20a74508 /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_SLOT0_REG2 0x20a7450c /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_SLOT1_REG1 0x20a74510 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_SLOT1_REG2 0x20a74514 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_LAST_TIMESTAMP_DELTA 0x20a74518 /* [RW] MCPB Channel x last TS delta value */
#define BCHP_XPT_MCPB_CH28_TMEU_TIMING_INFO_LAST_NEXT_TIMESTAMP 0x20a7451c /* [RW] MCPB Channel x last NEXT TS value */
#define BCHP_XPT_MCPB_CH28_DCPM_STATUS           0x20a74520 /* [RW] MCPB Channel x DCPM status information */
#define BCHP_XPT_MCPB_CH28_DCPM_DESC_ADDR        0x20a74524 /* [RW] MCPB Channel x DCPM descriptor address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DESC_DONE_INT_ADDR 0x20a74528 /* [RW] MCPB Channel x DCPM descriptor done interrupt address information */
#define BCHP_XPT_MCPB_CH28_DCPM_PAUSE_AFTER_GROUP_PACKETS_CTRL 0x20a7452c /* [RW] MCPB Channel x Pause after group of packets control information */
#define BCHP_XPT_MCPB_CH28_DCPM_PAUSE_AFTER_GROUP_PACKETS_PKT_COUNTER 0x20a74530 /* [RW] MCPB Channel x Pause after group of packets local packet counter */
#define BCHP_XPT_MCPB_CH28_DCPM_LOCAL_PACKET_COUNTER 0x20a74534 /* [RW] MCPB Channel x local packet counter */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_UPPER  0x20a74538 /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_LOWER  0x20a7453c /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH28_DCPM_CURR_DESC_ADDR   0x20a74540 /* [RW] MCPB Channel x DCPM current descriptor address information */
#define BCHP_XPT_MCPB_CH28_DCPM_SLOT_STATUS      0x20a74544 /* [RW] MCPB Channel x DCPM slot status information */
#define BCHP_XPT_MCPB_CH28_DCPM_DESC_ADDR_SLOT_0 0x20a74548 /* [RW] MCPB Channel x DCPM completed slot 0 descriptor address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_SLOT_0_UPPER 0x20a7454c /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_SLOT_0_LOWER 0x20a74550 /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DESC_ADDR_SLOT_1 0x20a74554 /* [RW] MCPB Channel x DCPM completed slot 1 descriptor address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_SLOT_1_UPPER 0x20a74558 /* [RW] MCPB Channel x DCPM completed slot 1 data address information */
#define BCHP_XPT_MCPB_CH28_DCPM_DATA_ADDR_SLOT_1_LOWER 0x20a7455c /* [RW] MCPB Channel x DCPM completed slot 1 data address information */

#endif /* #ifndef BCHP_XPT_MCPB_CH28_H__ */

/* End of File */
