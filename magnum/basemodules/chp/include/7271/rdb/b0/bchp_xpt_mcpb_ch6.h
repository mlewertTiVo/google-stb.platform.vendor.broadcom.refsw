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
 * Date:           Generated on               Thu Jun 16 14:47:09 2016
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

#ifndef BCHP_XPT_MCPB_CH6_H__
#define BCHP_XPT_MCPB_CH6_H__

/***************************************************************************
 *XPT_MCPB_CH6 - MCPB Channel 6 Configuration
 ***************************************************************************/
#define BCHP_XPT_MCPB_CH6_DMA_DESC_CONTROL       0x20a71800 /* [RW] MCPB Channel x Descriptor control information */
#define BCHP_XPT_MCPB_CH6_DMA_DATA_CONTROL       0x20a71804 /* [RW] MCPB Channel x Data control information */
#define BCHP_XPT_MCPB_CH6_DMA_CURR_DESC_ADDRESS  0x20a71808 /* [RW] MCPB Channel x Current Descriptor address information */
#define BCHP_XPT_MCPB_CH6_DMA_NEXT_DESC_ADDRESS  0x20a7180c /* [RW] MCPB Channel x Next Descriptor address information */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_BASE_ADDRESS_UPPER 0x20a71810 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_BASE_ADDRESS_LOWER 0x20a71814 /* [RW] MCPB Channel x Data Buffer Base address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_END_ADDRESS_UPPER 0x20a71818 /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_END_ADDRESS_LOWER 0x20a7181c /* [RW] MCPB Channel x Data Buffer End address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_CURR_RD_ADDRESS_UPPER 0x20a71820 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_CURR_RD_ADDRESS_LOWER 0x20a71824 /* [RW] MCPB Channel x Current Data Buffer Read address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_WRITE_ADDRESS_UPPER 0x20a71828 /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH6_DMA_BUFF_WRITE_ADDRESS_LOWER 0x20a7182c /* [RW] MCPB Channel x Data Buffer Write address */
#define BCHP_XPT_MCPB_CH6_DMA_STATUS_0           0x20a71830 /* [RW] MCPB Channel x Status information */
#define BCHP_XPT_MCPB_CH6_DMA_STATUS_1           0x20a71834 /* [RW] MCPB Channel x CRC value */
#define BCHP_XPT_MCPB_CH6_DMA_STATUS_2           0x20a71838 /* [RW] MCPB Channel x Manual mode status */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_STATUS_0 0x20a7183c /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_STATUS_1 0x20a71840 /* [RW] MCPB channel x Descriptor Slot 0 status information */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_CURR_DESC_ADDR 0x20a71844 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_CURR_DATA_ADDR_UPPER 0x20a71848 /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_CURR_DATA_ADDR_LOWER 0x20a7184c /* [RW] MCPB Channel x  Descriptor Slot 0 Current Data Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_NEXT_TIMESTAMP 0x20a71850 /* [RW] MCPB Channel x Descriptor Slot 0 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT0_PKT2PKT_TIMESTAMP_DELTA 0x20a71854 /* [RW] MCPB Channel x Descriptor Slot 0 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_STATUS_0 0x20a71858 /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_STATUS_1 0x20a7185c /* [RW] MCPB channel x Descriptor Slot 1 status information */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_CURR_DESC_ADDR 0x20a71860 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Descriptor Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_CURR_DATA_ADDR_UPPER 0x20a71864 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_CURR_DATA_ADDR_LOWER 0x20a71868 /* [RW] MCPB Channel x  Descriptor Slot 1 Current Data Address */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_NEXT_TIMESTAMP 0x20a7186c /* [RW] MCPB Channel x Descriptor Slot 1 Next Packet Timestamp */
#define BCHP_XPT_MCPB_CH6_DMA_DESC_SLOT1_PKT2PKT_TIMESTAMP_DELTA 0x20a71870 /* [RW] MCPB Channel x Descriptor Slot 1 Packet to packet Timestamp delta */
#define BCHP_XPT_MCPB_CH6_SP_PKT_LEN             0x20a71874 /* [RW] MCPB Channel x Packet length control */
#define BCHP_XPT_MCPB_CH6_SP_PARSER_CTRL         0x20a71878 /* [RW] MCPB Channel x Parser control */
#define BCHP_XPT_MCPB_CH6_SP_PARSER_CTRL1        0x20a7187c /* [RW] MCPB Channel x Parser control 1 */
#define BCHP_XPT_MCPB_CH6_SP_TS_CONFIG           0x20a71880 /* [RW] MCPB Channel x TS Configuration */
#define BCHP_XPT_MCPB_CH6_SP_PES_ES_CONFIG       0x20a71884 /* [RW] MCPB Channel x PES and ES Configuration */
#define BCHP_XPT_MCPB_CH6_SP_PES_SYNC_COUNTER    0x20a71888 /* [RW] MCPB Channel x PES Sync counter */
#define BCHP_XPT_MCPB_CH6_SP_ASF_CONFIG          0x20a7188c /* [RW] MCPB Channel x ASF Configuration */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_0         0x20a71890 /* [RW] MCPB Channel x Stream Processor State Register 0 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_1         0x20a71894 /* [RW] MCPB Channel x Stream Processor State Register 1 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_2         0x20a71898 /* [RW] MCPB Channel x Stream Processor State Register 2 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_3         0x20a7189c /* [RW] MCPB Channel x Stream Processor State Register 3 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_4         0x20a718a0 /* [RW] MCPB Channel x Stream Processor State Register 4 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_5         0x20a718a4 /* [RW] MCPB Channel x Stream Processor State Register 5 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_6         0x20a718a8 /* [RW] MCPB Channel x Stream Processor State Register 6 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_7         0x20a718ac /* [RW] MCPB Channel x Stream Processor State Register 7 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_8         0x20a718b0 /* [RW] MCPB Channel x Stream Processor State Register 8 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_9         0x20a718b4 /* [RW] MCPB Channel x Stream Processor State Register 9 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_10        0x20a718b8 /* [RW] MCPB Channel x Stream Processor State Register 10 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_11        0x20a718bc /* [RW] MCPB Channel x Stream Processor State Register 11 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_12        0x20a718c0 /* [RW] MCPB Channel x Stream Processor State Register 12 */
#define BCHP_XPT_MCPB_CH6_SP_STATE_REG_13        0x20a718c4 /* [RW] MCPB Channel x Stream Processor State Register 13 */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF_CTRL         0x20a718c8 /* [RW] MCPB Channel x Burst buffer control */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF_CRC          0x20a718cc /* [RW] MCPB Channel x Current CRC value */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF0_RW_STATUS   0x20a718d0 /* [RW] MCPB Channel x Burst buffer 0 data specific information */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF0_RO_STATUS   0x20a718d4 /* [RW] MCPB Channel x Burst buffer 0 control specific information */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF1_RW_STATUS   0x20a718d8 /* [RW] MCPB Channel x Burst buffer 1 data specific information */
#define BCHP_XPT_MCPB_CH6_DMA_BBUFF1_RO_STATUS   0x20a718dc /* [RW] MCPB Channel x Burst buffer 1 control specific information */
#define BCHP_XPT_MCPB_CH6_TMEU_BLOCKOUT_CTRL     0x20a718e0 /* [RW] MCPB Channel x Blockout control information */
#define BCHP_XPT_MCPB_CH6_TMEU_NEXT_BO_MON       0x20a718e4 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_CTRL       0x20a718e8 /* [RW] MCPB Channel x next Blockout monitor information */
#define BCHP_XPT_MCPB_CH6_TMEU_REF_DIFF_VALUE_TS_MBOX 0x20a718ec /* [RW] MCPB Channel x reference difference value and next Timestamp information */
#define BCHP_XPT_MCPB_CH6_TMEU_TS_ERR_BOUND_EARLY 0x20a718f0 /* [RW] MCPB Channel x TS error bound early information */
#define BCHP_XPT_MCPB_CH6_TMEU_TS_ERR_BOUND_LATE 0x20a718f4 /* [RW] MCPB Channel x TS error bound late information */
#define BCHP_XPT_MCPB_CH6_TMEU_NEXT_GPC_MON      0x20a718f8 /* [RW] MCPB Channel x next Global Pacing Counter and Timestamp monitor information */
#define BCHP_XPT_MCPB_CH6_TMEU_REF_DIFF_VALUE_SIGN 0x20a718fc /* [RW] MCPB Channel x reference difference value sign information */
#define BCHP_XPT_MCPB_CH6_TMEU_PES_PACING_CTRL   0x20a71900 /* [RW] MCPB Channel x PES pacing control information */
#define BCHP_XPT_MCPB_CH6_TMEU_SLOT_STATUS       0x20a71904 /* [RW] MCPB Channel x Slot 0 and Slot 1 information */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_SLOT0_REG1 0x20a71908 /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_SLOT0_REG2 0x20a7190c /* [RW] MCPB Channel x timing information for Slot 0 */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_SLOT1_REG1 0x20a71910 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_SLOT1_REG2 0x20a71914 /* [RW] MCPB Channel x timing information for Slot 1 */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_LAST_TIMESTAMP_DELTA 0x20a71918 /* [RW] MCPB Channel x last TS delta value */
#define BCHP_XPT_MCPB_CH6_TMEU_TIMING_INFO_LAST_NEXT_TIMESTAMP 0x20a7191c /* [RW] MCPB Channel x last NEXT TS value */
#define BCHP_XPT_MCPB_CH6_DCPM_STATUS            0x20a71920 /* [RW] MCPB Channel x DCPM status information */
#define BCHP_XPT_MCPB_CH6_DCPM_DESC_ADDR         0x20a71924 /* [RW] MCPB Channel x DCPM descriptor address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DESC_DONE_INT_ADDR 0x20a71928 /* [RW] MCPB Channel x DCPM descriptor done interrupt address information */
#define BCHP_XPT_MCPB_CH6_DCPM_PAUSE_AFTER_GROUP_PACKETS_CTRL 0x20a7192c /* [RW] MCPB Channel x Pause after group of packets control information */
#define BCHP_XPT_MCPB_CH6_DCPM_PAUSE_AFTER_GROUP_PACKETS_PKT_COUNTER 0x20a71930 /* [RW] MCPB Channel x Pause after group of packets local packet counter */
#define BCHP_XPT_MCPB_CH6_DCPM_LOCAL_PACKET_COUNTER 0x20a71934 /* [RW] MCPB Channel x local packet counter */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_UPPER   0x20a71938 /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_LOWER   0x20a7193c /* [RW] MCPB Channel x DCPM data address information */
#define BCHP_XPT_MCPB_CH6_DCPM_CURR_DESC_ADDR    0x20a71940 /* [RW] MCPB Channel x DCPM current descriptor address information */
#define BCHP_XPT_MCPB_CH6_DCPM_SLOT_STATUS       0x20a71944 /* [RW] MCPB Channel x DCPM slot status information */
#define BCHP_XPT_MCPB_CH6_DCPM_DESC_ADDR_SLOT_0  0x20a71948 /* [RW] MCPB Channel x DCPM completed slot 0 descriptor address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_SLOT_0_UPPER 0x20a7194c /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_SLOT_0_LOWER 0x20a71950 /* [RW] MCPB Channel x DCPM completed slot 0 data address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DESC_ADDR_SLOT_1  0x20a71954 /* [RW] MCPB Channel x DCPM completed slot 1 descriptor address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_SLOT_1_UPPER 0x20a71958 /* [RW] MCPB Channel x DCPM completed slot 1 data address information */
#define BCHP_XPT_MCPB_CH6_DCPM_DATA_ADDR_SLOT_1_LOWER 0x20a7195c /* [RW] MCPB Channel x DCPM completed slot 1 data address information */

#endif /* #ifndef BCHP_XPT_MCPB_CH6_H__ */

/* End of File */
