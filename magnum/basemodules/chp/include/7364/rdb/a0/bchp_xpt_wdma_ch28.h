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
 * Date:           Generated on               Fri Aug 15 15:20:53 2014
 *                 Full Compile MD5 Checksum  a68bc62e9dd3be19fcad480c369d60fd
 *                     (minus title and desc)
 *                 MD5 Checksum               14382795d76d8497c2dd1bcf3f5d36da
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     14541
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_XPT_WDMA_CH28_H__
#define BCHP_XPT_WDMA_CH28_H__

/***************************************************************************
 *XPT_WDMA_CH28 - WDMA Channel 28 Configuration
 ***************************************************************************/
#define BCHP_XPT_WDMA_CH28_FIRST_DESC_ADDR       0x00a6bc00 /* [RW] First Descriptor Address */
#define BCHP_XPT_WDMA_CH28_NEXT_DESC_ADDR        0x00a6bc04 /* [RW] Next Descriptor Address */
#define BCHP_XPT_WDMA_CH28_COMPLETED_DESC_ADDRESS 0x00a6bc08 /* [RW] Completed Descriptor Address */
#define BCHP_XPT_WDMA_CH28_BTP_PACKET_GROUP_ID   0x00a6bc0c /* [RW] Packet Group ID reported per BTP command */
#define BCHP_XPT_WDMA_CH28_RUN_VERSION_CONFIG    0x00a6bc10 /* [RW] RUN_VERSION configuration */
#define BCHP_XPT_WDMA_CH28_OVERFLOW_REASONS      0x00a6bc14 /* [RW] Overflow Reason */
#define BCHP_XPT_WDMA_CH28_DMQ_CONTROL_STRUCT    0x00a6bc18 /* [RO] Descriptor Memory Queue Control Structure */
#define BCHP_XPT_WDMA_CH28_DATA_CONTROL          0x00a6bc1c /* [RW] Data Control */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_BASE_PTR_HI 0x00a6bc80 /* [RW] DRAM Buffer Base Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_BASE_PTR    0x00a6bc84 /* [RW] DRAM Buffer Base Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_END_PTR_HI  0x00a6bc88 /* [RW] DRAM Buffer End Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_END_PTR     0x00a6bc8c /* [RW] DRAM Buffer End Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_RD_PTR_HI   0x00a6bc90 /* [RW] DRAM Buffer Read Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_RD_PTR      0x00a6bc94 /* [RW] DRAM Buffer Read Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_WR_PTR_HI   0x00a6bc98 /* [RW] DRAM Buffer Write Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_WR_PTR      0x00a6bc9c /* [RW] DRAM Buffer Write Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_VALID_PTR_HI 0x00a6bca0 /* [RW] DRAM Buffer Valid Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_VALID_PTR   0x00a6bca4 /* [RW] DRAM Buffer Valid Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_LOWER_THRESHOLD_HI 0x00a6bca8 /* [RW] DRAM Buffer Lower Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_LOWER_THRESHOLD 0x00a6bcac /* [RW] DRAM Buffer Lower Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_UPPER_THRESHOLD_HI 0x00a6bcb0 /* [RW] DRAM Buffer Upper Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_UPPER_THRESHOLD 0x00a6bcb4 /* [RW] DRAM Buffer Upper Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_STATUS      0x00a6bcb8 /* [RW] DRAM Buffer Status */
#define BCHP_XPT_WDMA_CH28_DRAM_BUFF_CONTROL     0x00a6bcbc /* [RW] DRAM Buffer Control */
#define BCHP_XPT_WDMA_CH28_DMQ_0_0               0x00a6bcc0 /* [RW] DMQ descriptor 0 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH28_DMQ_0_1               0x00a6bcc4 /* [RW] DMQ descriptor 0 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DMQ_0_2               0x00a6bcc8 /* [RW] DMQ descriptor 0 - Transfer Size */
#define BCHP_XPT_WDMA_CH28_DMQ_0_3               0x00a6bccc /* [RW] DMQ descriptor 0 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH28_DMQ_1_0               0x00a6bcd0 /* [RW] DMQ descriptor 1 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH28_DMQ_1_1               0x00a6bcd4 /* [RW] DMQ descriptor 1 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DMQ_1_2               0x00a6bcd8 /* [RW] DMQ descriptor 1 - Transfer Size */
#define BCHP_XPT_WDMA_CH28_DMQ_1_3               0x00a6bcdc /* [RW] DMQ descriptor 1 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH28_DMQ_2_0               0x00a6bce0 /* [RW] DMQ descriptor 2 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH28_DMQ_2_1               0x00a6bce4 /* [RW] DMQ descriptor 2 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DMQ_2_2               0x00a6bce8 /* [RW] DMQ descriptor 2 - Transfer Size */
#define BCHP_XPT_WDMA_CH28_DMQ_2_3               0x00a6bcec /* [RW] DMQ descriptor 2 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH28_DMQ_3_0               0x00a6bcf0 /* [RW] DMQ descriptor 3 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH28_DMQ_3_1               0x00a6bcf4 /* [RW] DMQ descriptor 3 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH28_DMQ_3_2               0x00a6bcf8 /* [RW] DMQ descriptor 3 - Transfer Size */
#define BCHP_XPT_WDMA_CH28_DMQ_3_3               0x00a6bcfc /* [RW] DMQ descriptor 3 - Current Descriptor Address and Control */

#endif /* #ifndef BCHP_XPT_WDMA_CH28_H__ */

/* End of File */
