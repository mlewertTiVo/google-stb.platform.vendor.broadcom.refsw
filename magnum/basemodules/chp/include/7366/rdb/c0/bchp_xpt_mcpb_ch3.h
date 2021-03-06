/****************************************************************************
 *     Copyright (c) 1999-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on               Thu Feb 12 15:15:54 2015
 *                 Full Compile MD5 Checksum  ca339b82db08da0250a17ca09932699d
 *                     (minus title and desc)
 *                 MD5 Checksum               502556bfbdc2f4341f93db8b4326b3ab
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_XPT_MCPB_CH3_H__
#define BCHP_XPT_MCPB_CH3_H__

/***************************************************************************
 *XPT_MCPB_CH3 - MCPB Channel 3 Configuration
 ***************************************************************************/
#define BCHP_XPT_MCPB_CH3_DMA_DESC_CONTROL       0x00a71200 /* [RW] MCPB Channel x Descriptor control information */
#define BCHP_XPT_MCPB_CH3_DMA_DATA_CONTROL       0x00a71204 /* [RW] MCPB Channel x Data control information */
#define BCHP_XPT_MCPB_CH3_DMA_CURR_DESC_ADDRESS  0x00a71208 /* [RW] MCPB Channel x Current Descriptor address information */
#define BCHP_XPT_MCPB_CH3_DMA_NEXT_DESC_ADDRESS  0x00a7120c /* [RW] MCPB Channel x Next Descriptor address information */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_BASE_ADDRESS_UPPER 0x00a71210 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_BASE_ADDRESS_LOWER 0x00a71214 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_END_ADDRESS_UPPER 0x00a71218 /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_END_ADDRESS_LOWER 0x00a7121c /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_CURR_RD_ADDRESS_UPPER 0x00a71220 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_CURR_RD_ADDRESS_LOWER 0x00a71224 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_WRITE_ADDRESS_UPPER 0x00a71228 /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH3_DMA_BUFF_WRITE_ADDRESS_LOWER 0x00a7122c /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH3_DMA_STATUS_0           0x00a71230 /* [RW] MCPB Channel x Status information */
#define BCHP_XPT_MCPB_CH3_DMA_STATUS_1           0x00a71234 /* [RW] MCPB Channel x CRC value */
#define BCHP_XPT_MCPB_CH3_DMA_STATUS_2           0x00a71238 /* [RW] MCPB Channel x Manual mode status */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_STATUS_0 0x00a7123c /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_STATUS_1 0x00a71240 /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_CURR_DESC_ADDR 0x00a71244 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_CURR_DATA_ADDR_UPPER 0x00a71248 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_CURR_DATA_ADDR_LOWER 0x00a7124c /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_NEXT_TIMESTAMP 0x00a71250 /* [RW] MCPB Channel x Descriptor Slot 0 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT0_PKT2PKT_TIMESTAMP_DELTA 0x00a71254 /* [RW] MCPB Channel x Descriptor Slot 0 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_STATUS_0 0x00a71258 /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_STATUS_1 0x00a7125c /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_CURR_DESC_ADDR 0x00a71260 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_CURR_DATA_ADDR_UPPER 0x00a71264 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_CURR_DATA_ADDR_LOWER 0x00a71268 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_NEXT_TIMESTAMP 0x00a7126c /* [RW] MCPB Channel x Descriptor Slot 1 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH3_DMA_DESC_SLOT1_PKT2PKT_TIMESTAMP_DELTA 0x00a71270 /* [RW] MCPB Channel x Descriptor Slot 1 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH3_SP_PKT_LEN             0x00a71274 /* [RW] MCPB Channel x Packet length control */
#define BCHP_XPT_MCPB_CH3_SP_PARSER_CTRL         0x00a71278 /* [RW] MCPB Channel x Parser control */
#define BCHP_XPT_MCPB_CH3_SP_PARSER_CTRL1        0x00a7127c /* [RW] MCPB Channel x Parser control 1 */
#define BCHP_XPT_MCPB_CH3_SP_TS_CONFIG           0x00a71280 /* [RW] MCPB Channel x TS Configuration */
#define BCHP_XPT_MCPB_CH3_SP_PES_ES_CONFIG       0x00a71284 /* [RW] MCPB Channel x PES and ES Configuration */
#define BCHP_XPT_MCPB_CH3_SP_PES_SYNC_COUNTER    0x00a71288 /* [RW] MCPB Channel x PES Sync counter */
#define BCHP_XPT_MCPB_CH3_SP_ASF_CONFIG          0x00a7128c /* [RW] MCPB Channel x ASF Configuration */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_0         0x00a71290 /* [RW] MCPB Channel x Stream Processor State Register 0 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_1         0x00a71294 /* [RW] MCPB Channel x Stream Processor State Register 1 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_2         0x00a71298 /* [RW] MCPB Channel x Stream Processor State Register 2 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_3         0x00a7129c /* [RW] MCPB Channel x Stream Processor State Register 3 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_4         0x00a712a0 /* [RW] MCPB Channel x Stream Processor State Register 4 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_5         0x00a712a4 /* [RW] MCPB Channel x Stream Processor State Register 5 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_6         0x00a712a8 /* [RW] MCPB Channel x Stream Processor State Register 6 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_7         0x00a712ac /* [RW] MCPB Channel x Stream Processor State Register 7 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_8         0x00a712b0 /* [RW] MCPB Channel x Stream Processor State Register 8 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_9         0x00a712b4 /* [RW] MCPB Channel x Stream Processor State Register 9 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_10        0x00a712b8 /* [RW] MCPB Channel x Stream Processor State Register 10 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_11        0x00a712bc /* [RW] MCPB Channel x Stream Processor State Register 11 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_12        0x00a712c0 /* [RW] MCPB Channel x Stream Processor State Register 12 */
#define BCHP_XPT_MCPB_CH3_SP_STATE_REG_13        0x00a712c4 /* [RW] MCPB Channel x Stream Processor State Register 13 */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF_CTRL         0x00a712c8 /* [RW] MCPB Channel x Burst buffer control */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF_CRC          0x00a712cc /* [RW] MCPB Channel x Current CRC value */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF0_RW_STATUS   0x00a712d0 /* [RW] MCPB Channel x Burst buffer 0 data specific information */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF0_RO_STATUS   0x00a712d4 /* [RW] MCPB Channel x Burst buffer 0 control specific information */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF1_RW_STATUS   0x00a712d8 /* [RW] MCPB Channel x Burst buffer 1 data specific information */
#define BCHP_XPT_MCPB_CH3_DMA_BBUFF1_RO_STATUS   0x00a712dc /* [RW] MCPB Channel x Burst buffer 1 control specific information */
#define BCHP_XPT_MCPB_CH3_TMEU_BLOCKOUT_CTRL     0x00a712e0 /* [RW] MCPB Channel x Blockout control information */
#define BCHP_XPT_MCPB_CH3_TMEU_NEXT_BO_MON       0x00a712e4 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_CTRL       0x00a712e8 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH3_TMEU_REF_DIFF_VALUE_TS_MBOX 0x00a712ec /* [RW] MCPB Channel x reference difference value and next Timestamp information */
#define BCHP_XPT_MCPB_CH3_TMEU_TS_ERR_BOUND_EARLY 0x00a712f0 /* [RW] MCPB Channel x TS error bound early information */
#define BCHP_XPT_MCPB_CH3_TMEU_TS_ERR_BOUND_LATE 0x00a712f4 /* [RW] MCPB Channel x TS error bound late information */
#define BCHP_XPT_MCPB_CH3_TMEU_NEXT_GPC_MON      0x00a712f8 /* [RW] MCPB Channel x next Global Pacing Counter and Timestamp monitor information */
#define BCHP_XPT_MCPB_CH3_TMEU_REF_DIFF_VALUE_SIGN 0x00a712fc /* [RW] MCPB Channel x reference difference value sign information */
#define BCHP_XPT_MCPB_CH3_TMEU_PES_PACING_CTRL   0x00a71300 /* [RW] MCPB Channel x PES pacing control information */
#define BCHP_XPT_MCPB_CH3_TMEU_SLOT_STATUS       0x00a71304 /* [RW] MCPB Channel x Slot 0 and Slot 1 information */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_SLOT0_REG1 0x00a71308 /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_SLOT0_REG2 0x00a7130c /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_SLOT1_REG1 0x00a71310 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_SLOT1_REG2 0x00a71314 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_LAST_TIMESTAMP_DELTA 0x00a71318 /* [RW] MCPB Channel x last TS delta value */
#define BCHP_XPT_MCPB_CH3_TMEU_TIMING_INFO_LAST_NEXT_TIMESTAMP 0x00a7131c /* [RW] MCPB Channel x last NEXT TS value */
#define BCHP_XPT_MCPB_CH3_DCPM_STATUS            0x00a71320 /* [RW] MCPB Channel x DCPM status information */
#define BCHP_XPT_MCPB_CH3_DCPM_DESC_ADDR         0x00a71324 /* [RW] MCPB Channel x DCPM descriptor address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DESC_DONE_INT_ADDR 0x00a71328 /* [RW] MCPB Channel x DCPM descriptor done interrupt address information */
#define BCHP_XPT_MCPB_CH3_DCPM_PAUSE_AFTER_GROUP_PACKETS_CTRL 0x00a7132c /* [RW] MCPB Channel x Pause after group of packets control information */
#define BCHP_XPT_MCPB_CH3_DCPM_PAUSE_AFTER_GROUP_PACKETS_PKT_COUNTER 0x00a71330 /* [RW] MCPB Channel x Pause after group of packets local packet counter */
#define BCHP_XPT_MCPB_CH3_DCPM_LOCAL_PACKET_COUNTER 0x00a71334 /* [RW] MCPB Channel x local packet counter */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_UPPER   0x00a71338 /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_LOWER   0x00a7133c /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH3_DCPM_CURR_DESC_ADDR    0x00a71340 /* [RW] MCPB Channel x DCPM current descriptor address information */
#define BCHP_XPT_MCPB_CH3_DCPM_SLOT_STATUS       0x00a71344 /* [RW] MCPB Channel x DCPM slot status information */
#define BCHP_XPT_MCPB_CH3_DCPM_DESC_ADDR_SLOT_0  0x00a71348 /* [RW] MCPB Channel x DCPM completed slot 0 descriptor address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_SLOT_0_UPPER 0x00a7134c /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_SLOT_0_LOWER 0x00a71350 /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DESC_ADDR_SLOT_1  0x00a71354 /* [RW] MCPB Channel x DCPM completed slot 1 descriptor address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_SLOT_1_UPPER 0x00a71358 /* [RW] MCPB Channel x DCPM completed slot 1 data address information */
#define BCHP_XPT_MCPB_CH3_DCPM_DATA_ADDR_SLOT_1_LOWER 0x00a7135c /* [RW] MCPB Channel x DCPM completed slot 1 data address information */

#endif /* #ifndef BCHP_XPT_MCPB_CH3_H__ */

/* End of File */
