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

#ifndef BCHP_XPT_MCPB_CH30_H__
#define BCHP_XPT_MCPB_CH30_H__

/***************************************************************************
 *XPT_MCPB_CH30 - MCPB Channel 30 Configuration
 ***************************************************************************/
#define BCHP_XPT_MCPB_CH30_DMA_DESC_CONTROL      0x00a74800 /* [RW] MCPB Channel x Descriptor control information */
#define BCHP_XPT_MCPB_CH30_DMA_DATA_CONTROL      0x00a74804 /* [RW] MCPB Channel x Data control information */
#define BCHP_XPT_MCPB_CH30_DMA_CURR_DESC_ADDRESS 0x00a74808 /* [RW] MCPB Channel x Current Descriptor address information */
#define BCHP_XPT_MCPB_CH30_DMA_NEXT_DESC_ADDRESS 0x00a7480c /* [RW] MCPB Channel x Next Descriptor address information */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_BASE_ADDRESS_UPPER 0x00a74810 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_BASE_ADDRESS_LOWER 0x00a74814 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_END_ADDRESS_UPPER 0x00a74818 /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_END_ADDRESS_LOWER 0x00a7481c /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_CURR_RD_ADDRESS_UPPER 0x00a74820 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_CURR_RD_ADDRESS_LOWER 0x00a74824 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_WRITE_ADDRESS_UPPER 0x00a74828 /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH30_DMA_BUFF_WRITE_ADDRESS_LOWER 0x00a7482c /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH30_DMA_STATUS_0          0x00a74830 /* [RW] MCPB Channel x Status information */
#define BCHP_XPT_MCPB_CH30_DMA_STATUS_1          0x00a74834 /* [RW] MCPB Channel x CRC value */
#define BCHP_XPT_MCPB_CH30_DMA_STATUS_2          0x00a74838 /* [RW] MCPB Channel x Manual mode status */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_STATUS_0 0x00a7483c /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_STATUS_1 0x00a74840 /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_CURR_DESC_ADDR 0x00a74844 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_CURR_DATA_ADDR_UPPER 0x00a74848 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_CURR_DATA_ADDR_LOWER 0x00a7484c /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_NEXT_TIMESTAMP 0x00a74850 /* [RW] MCPB Channel x Descriptor Slot 0 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT0_PKT2PKT_TIMESTAMP_DELTA 0x00a74854 /* [RW] MCPB Channel x Descriptor Slot 0 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_STATUS_0 0x00a74858 /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_STATUS_1 0x00a7485c /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_CURR_DESC_ADDR 0x00a74860 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_CURR_DATA_ADDR_UPPER 0x00a74864 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_CURR_DATA_ADDR_LOWER 0x00a74868 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_NEXT_TIMESTAMP 0x00a7486c /* [RW] MCPB Channel x Descriptor Slot 1 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH30_DMA_DESC_SLOT1_PKT2PKT_TIMESTAMP_DELTA 0x00a74870 /* [RW] MCPB Channel x Descriptor Slot 1 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH30_SP_PKT_LEN            0x00a74874 /* [RW] MCPB Channel x Packet length control */
#define BCHP_XPT_MCPB_CH30_SP_PARSER_CTRL        0x00a74878 /* [RW] MCPB Channel x Parser control */
#define BCHP_XPT_MCPB_CH30_SP_PARSER_CTRL1       0x00a7487c /* [RW] MCPB Channel x Parser control 1 */
#define BCHP_XPT_MCPB_CH30_SP_TS_CONFIG          0x00a74880 /* [RW] MCPB Channel x TS Configuration */
#define BCHP_XPT_MCPB_CH30_SP_PES_ES_CONFIG      0x00a74884 /* [RW] MCPB Channel x PES and ES Configuration */
#define BCHP_XPT_MCPB_CH30_SP_PES_SYNC_COUNTER   0x00a74888 /* [RW] MCPB Channel x PES Sync counter */
#define BCHP_XPT_MCPB_CH30_SP_ASF_CONFIG         0x00a7488c /* [RW] MCPB Channel x ASF Configuration */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_0        0x00a74890 /* [RW] MCPB Channel x Stream Processor State Register 0 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_1        0x00a74894 /* [RW] MCPB Channel x Stream Processor State Register 1 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_2        0x00a74898 /* [RW] MCPB Channel x Stream Processor State Register 2 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_3        0x00a7489c /* [RW] MCPB Channel x Stream Processor State Register 3 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_4        0x00a748a0 /* [RW] MCPB Channel x Stream Processor State Register 4 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_5        0x00a748a4 /* [RW] MCPB Channel x Stream Processor State Register 5 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_6        0x00a748a8 /* [RW] MCPB Channel x Stream Processor State Register 6 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_7        0x00a748ac /* [RW] MCPB Channel x Stream Processor State Register 7 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_8        0x00a748b0 /* [RW] MCPB Channel x Stream Processor State Register 8 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_9        0x00a748b4 /* [RW] MCPB Channel x Stream Processor State Register 9 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_10       0x00a748b8 /* [RW] MCPB Channel x Stream Processor State Register 10 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_11       0x00a748bc /* [RW] MCPB Channel x Stream Processor State Register 11 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_12       0x00a748c0 /* [RW] MCPB Channel x Stream Processor State Register 12 */
#define BCHP_XPT_MCPB_CH30_SP_STATE_REG_13       0x00a748c4 /* [RW] MCPB Channel x Stream Processor State Register 13 */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF_CTRL        0x00a748c8 /* [RW] MCPB Channel x Burst buffer control */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF_CRC         0x00a748cc /* [RW] MCPB Channel x Current CRC value */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF0_RW_STATUS  0x00a748d0 /* [RW] MCPB Channel x Burst buffer 0 data specific information */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF0_RO_STATUS  0x00a748d4 /* [RW] MCPB Channel x Burst buffer 0 control specific information */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF1_RW_STATUS  0x00a748d8 /* [RW] MCPB Channel x Burst buffer 1 data specific information */
#define BCHP_XPT_MCPB_CH30_DMA_BBUFF1_RO_STATUS  0x00a748dc /* [RW] MCPB Channel x Burst buffer 1 control specific information */
#define BCHP_XPT_MCPB_CH30_TMEU_BLOCKOUT_CTRL    0x00a748e0 /* [RW] MCPB Channel x Blockout control information */
#define BCHP_XPT_MCPB_CH30_TMEU_NEXT_BO_MON      0x00a748e4 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_CTRL      0x00a748e8 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH30_TMEU_REF_DIFF_VALUE_TS_MBOX 0x00a748ec /* [RW] MCPB Channel x reference difference value and next Timestamp information */
#define BCHP_XPT_MCPB_CH30_TMEU_TS_ERR_BOUND_EARLY 0x00a748f0 /* [RW] MCPB Channel x TS error bound early information */
#define BCHP_XPT_MCPB_CH30_TMEU_TS_ERR_BOUND_LATE 0x00a748f4 /* [RW] MCPB Channel x TS error bound late information */
#define BCHP_XPT_MCPB_CH30_TMEU_NEXT_GPC_MON     0x00a748f8 /* [RW] MCPB Channel x next Global Pacing Counter and Timestamp monitor information */
#define BCHP_XPT_MCPB_CH30_TMEU_REF_DIFF_VALUE_SIGN 0x00a748fc /* [RW] MCPB Channel x reference difference value sign information */
#define BCHP_XPT_MCPB_CH30_TMEU_PES_PACING_CTRL  0x00a74900 /* [RW] MCPB Channel x PES pacing control information */
#define BCHP_XPT_MCPB_CH30_TMEU_SLOT_STATUS      0x00a74904 /* [RW] MCPB Channel x Slot 0 and Slot 1 information */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_SLOT0_REG1 0x00a74908 /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_SLOT0_REG2 0x00a7490c /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_SLOT1_REG1 0x00a74910 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_SLOT1_REG2 0x00a74914 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_LAST_TIMESTAMP_DELTA 0x00a74918 /* [RW] MCPB Channel x last TS delta value */
#define BCHP_XPT_MCPB_CH30_TMEU_TIMING_INFO_LAST_NEXT_TIMESTAMP 0x00a7491c /* [RW] MCPB Channel x last NEXT TS value */
#define BCHP_XPT_MCPB_CH30_DCPM_STATUS           0x00a74920 /* [RW] MCPB Channel x DCPM status information */
#define BCHP_XPT_MCPB_CH30_DCPM_DESC_ADDR        0x00a74924 /* [RW] MCPB Channel x DCPM descriptor address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DESC_DONE_INT_ADDR 0x00a74928 /* [RW] MCPB Channel x DCPM descriptor done interrupt address information */
#define BCHP_XPT_MCPB_CH30_DCPM_PAUSE_AFTER_GROUP_PACKETS_CTRL 0x00a7492c /* [RW] MCPB Channel x Pause after group of packets control information */
#define BCHP_XPT_MCPB_CH30_DCPM_PAUSE_AFTER_GROUP_PACKETS_PKT_COUNTER 0x00a74930 /* [RW] MCPB Channel x Pause after group of packets local packet counter */
#define BCHP_XPT_MCPB_CH30_DCPM_LOCAL_PACKET_COUNTER 0x00a74934 /* [RW] MCPB Channel x local packet counter */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_UPPER  0x00a74938 /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_LOWER  0x00a7493c /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH30_DCPM_CURR_DESC_ADDR   0x00a74940 /* [RW] MCPB Channel x DCPM current descriptor address information */
#define BCHP_XPT_MCPB_CH30_DCPM_SLOT_STATUS      0x00a74944 /* [RW] MCPB Channel x DCPM slot status information */
#define BCHP_XPT_MCPB_CH30_DCPM_DESC_ADDR_SLOT_0 0x00a74948 /* [RW] MCPB Channel x DCPM completed slot 0 descriptor address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_SLOT_0_UPPER 0x00a7494c /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_SLOT_0_LOWER 0x00a74950 /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DESC_ADDR_SLOT_1 0x00a74954 /* [RW] MCPB Channel x DCPM completed slot 1 descriptor address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_SLOT_1_UPPER 0x00a74958 /* [RW] MCPB Channel x DCPM completed slot 1 data address information */
#define BCHP_XPT_MCPB_CH30_DCPM_DATA_ADDR_SLOT_1_LOWER 0x00a7495c /* [RW] MCPB Channel x DCPM completed slot 1 data address information */

#endif /* #ifndef BCHP_XPT_MCPB_CH30_H__ */

/* End of File */
