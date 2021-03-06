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
 * Date:           Generated on               Fri Aug 15 15:20:56 2014
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

#ifndef BCHP_XPT_WDMA_CH10_H__
#define BCHP_XPT_WDMA_CH10_H__

/***************************************************************************
 *XPT_WDMA_CH10 - WDMA Channel 10 Configuration
 ***************************************************************************/
#define BCHP_XPT_WDMA_CH10_FIRST_DESC_ADDR       0x00a6aa00 /* [RW] First Descriptor Address */
#define BCHP_XPT_WDMA_CH10_NEXT_DESC_ADDR        0x00a6aa04 /* [RW] Next Descriptor Address */
#define BCHP_XPT_WDMA_CH10_COMPLETED_DESC_ADDRESS 0x00a6aa08 /* [RW] Completed Descriptor Address */
#define BCHP_XPT_WDMA_CH10_BTP_PACKET_GROUP_ID   0x00a6aa0c /* [RW] Packet Group ID reported per BTP command */
#define BCHP_XPT_WDMA_CH10_RUN_VERSION_CONFIG    0x00a6aa10 /* [RW] RUN_VERSION configuration */
#define BCHP_XPT_WDMA_CH10_OVERFLOW_REASONS      0x00a6aa14 /* [RW] Overflow Reason */
#define BCHP_XPT_WDMA_CH10_DMQ_CONTROL_STRUCT    0x00a6aa18 /* [RO] Descriptor Memory Queue Control Structure */
#define BCHP_XPT_WDMA_CH10_DATA_CONTROL          0x00a6aa1c /* [RW] Data Control */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_BASE_PTR_HI 0x00a6aa80 /* [RW] DRAM Buffer Base Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_BASE_PTR    0x00a6aa84 /* [RW] DRAM Buffer Base Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_END_PTR_HI  0x00a6aa88 /* [RW] DRAM Buffer End Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_END_PTR     0x00a6aa8c /* [RW] DRAM Buffer End Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_RD_PTR_HI   0x00a6aa90 /* [RW] DRAM Buffer Read Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_RD_PTR      0x00a6aa94 /* [RW] DRAM Buffer Read Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_WR_PTR_HI   0x00a6aa98 /* [RW] DRAM Buffer Write Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_WR_PTR      0x00a6aa9c /* [RW] DRAM Buffer Write Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_VALID_PTR_HI 0x00a6aaa0 /* [RW] DRAM Buffer Valid Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_VALID_PTR   0x00a6aaa4 /* [RW] DRAM Buffer Valid Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_LOWER_THRESHOLD_HI 0x00a6aaa8 /* [RW] DRAM Buffer Lower Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_LOWER_THRESHOLD 0x00a6aaac /* [RW] DRAM Buffer Lower Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_UPPER_THRESHOLD_HI 0x00a6aab0 /* [RW] DRAM Buffer Upper Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_UPPER_THRESHOLD 0x00a6aab4 /* [RW] DRAM Buffer Upper Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_STATUS      0x00a6aab8 /* [RW] DRAM Buffer Status */
#define BCHP_XPT_WDMA_CH10_DRAM_BUFF_CONTROL     0x00a6aabc /* [RW] DRAM Buffer Control */
#define BCHP_XPT_WDMA_CH10_DMQ_0_0               0x00a6aac0 /* [RW] DMQ descriptor 0 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH10_DMQ_0_1               0x00a6aac4 /* [RW] DMQ descriptor 0 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DMQ_0_2               0x00a6aac8 /* [RW] DMQ descriptor 0 - Transfer Size */
#define BCHP_XPT_WDMA_CH10_DMQ_0_3               0x00a6aacc /* [RW] DMQ descriptor 0 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH10_DMQ_1_0               0x00a6aad0 /* [RW] DMQ descriptor 1 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH10_DMQ_1_1               0x00a6aad4 /* [RW] DMQ descriptor 1 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DMQ_1_2               0x00a6aad8 /* [RW] DMQ descriptor 1 - Transfer Size */
#define BCHP_XPT_WDMA_CH10_DMQ_1_3               0x00a6aadc /* [RW] DMQ descriptor 1 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH10_DMQ_2_0               0x00a6aae0 /* [RW] DMQ descriptor 2 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH10_DMQ_2_1               0x00a6aae4 /* [RW] DMQ descriptor 2 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DMQ_2_2               0x00a6aae8 /* [RW] DMQ descriptor 2 - Transfer Size */
#define BCHP_XPT_WDMA_CH10_DMQ_2_3               0x00a6aaec /* [RW] DMQ descriptor 2 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH10_DMQ_3_0               0x00a6aaf0 /* [RW] DMQ descriptor 3 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH10_DMQ_3_1               0x00a6aaf4 /* [RW] DMQ descriptor 3 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH10_DMQ_3_2               0x00a6aaf8 /* [RW] DMQ descriptor 3 - Transfer Size */
#define BCHP_XPT_WDMA_CH10_DMQ_3_3               0x00a6aafc /* [RW] DMQ descriptor 3 - Current Descriptor Address and Control */

#endif /* #ifndef BCHP_XPT_WDMA_CH10_H__ */

/* End of File */
