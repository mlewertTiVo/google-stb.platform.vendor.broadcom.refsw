/****************************************************************************
 *     Copyright (c) 1999-2015, Broadcom Corporation
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
 * Date:           Generated on               Wed Apr  1 11:23:11 2015
 *                 Full Compile MD5 Checksum  267f8e92d9b43928c0a06f1ab29c511c
 *                     (minus title and desc)
 *                 MD5 Checksum               0548f7f0a8e20364fd383a7aa29c0b86
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15956
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
 ***************************************************************************/

#ifndef BCHP_XPT_WDMA_CH3_H__
#define BCHP_XPT_WDMA_CH3_H__

/***************************************************************************
 *XPT_WDMA_CH3 - WDMA Channel 3 Configuration
 ***************************************************************************/
#define BCHP_XPT_WDMA_CH3_FIRST_DESC_ADDR        0x00a6a300 /* [RW] First Descriptor Address */
#define BCHP_XPT_WDMA_CH3_NEXT_DESC_ADDR         0x00a6a304 /* [RW] Next Descriptor Address */
#define BCHP_XPT_WDMA_CH3_COMPLETED_DESC_ADDRESS 0x00a6a308 /* [RW] Completed Descriptor Address */
#define BCHP_XPT_WDMA_CH3_BTP_PACKET_GROUP_ID    0x00a6a30c /* [RW] Packet Group ID reported per BTP command */
#define BCHP_XPT_WDMA_CH3_RUN_VERSION_CONFIG     0x00a6a310 /* [RW] RUN_VERSION configuration */
#define BCHP_XPT_WDMA_CH3_OVERFLOW_REASONS       0x00a6a314 /* [RW] Overflow Reason */
#define BCHP_XPT_WDMA_CH3_DMQ_CONTROL_STRUCT     0x00a6a318 /* [RO] Descriptor Memory Queue Control Structure */
#define BCHP_XPT_WDMA_CH3_DATA_CONTROL           0x00a6a31c /* [RW] Data Control */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_BASE_PTR_HI  0x00a6a380 /* [RW] DRAM Buffer Base Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_BASE_PTR     0x00a6a384 /* [RW] DRAM Buffer Base Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_END_PTR_HI   0x00a6a388 /* [RW] DRAM Buffer End Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_END_PTR      0x00a6a38c /* [RW] DRAM Buffer End Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_RD_PTR_HI    0x00a6a390 /* [RW] DRAM Buffer Read Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_RD_PTR       0x00a6a394 /* [RW] DRAM Buffer Read Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_WR_PTR_HI    0x00a6a398 /* [RW] DRAM Buffer Write Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_WR_PTR       0x00a6a39c /* [RW] DRAM Buffer Write Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_VALID_PTR_HI 0x00a6a3a0 /* [RW] DRAM Buffer Valid Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_VALID_PTR    0x00a6a3a4 /* [RW] DRAM Buffer Valid Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_LOWER_THRESHOLD_HI 0x00a6a3a8 /* [RW] DRAM Buffer Lower Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_LOWER_THRESHOLD 0x00a6a3ac /* [RW] DRAM Buffer Lower Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_UPPER_THRESHOLD_HI 0x00a6a3b0 /* [RW] DRAM Buffer Upper Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_UPPER_THRESHOLD 0x00a6a3b4 /* [RW] DRAM Buffer Upper Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_STATUS       0x00a6a3b8 /* [RW] DRAM Buffer Status */
#define BCHP_XPT_WDMA_CH3_DRAM_BUFF_CONTROL      0x00a6a3bc /* [RW] DRAM Buffer Control */
#define BCHP_XPT_WDMA_CH3_DMQ_0_0                0x00a6a3c0 /* [RW] DMQ descriptor 0 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH3_DMQ_0_1                0x00a6a3c4 /* [RW] DMQ descriptor 0 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DMQ_0_2                0x00a6a3c8 /* [RW] DMQ descriptor 0 - Transfer Size */
#define BCHP_XPT_WDMA_CH3_DMQ_0_3                0x00a6a3cc /* [RW] DMQ descriptor 0 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH3_DMQ_1_0                0x00a6a3d0 /* [RW] DMQ descriptor 1 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH3_DMQ_1_1                0x00a6a3d4 /* [RW] DMQ descriptor 1 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DMQ_1_2                0x00a6a3d8 /* [RW] DMQ descriptor 1 - Transfer Size */
#define BCHP_XPT_WDMA_CH3_DMQ_1_3                0x00a6a3dc /* [RW] DMQ descriptor 1 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH3_DMQ_2_0                0x00a6a3e0 /* [RW] DMQ descriptor 2 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH3_DMQ_2_1                0x00a6a3e4 /* [RW] DMQ descriptor 2 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DMQ_2_2                0x00a6a3e8 /* [RW] DMQ descriptor 2 - Transfer Size */
#define BCHP_XPT_WDMA_CH3_DMQ_2_3                0x00a6a3ec /* [RW] DMQ descriptor 2 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH3_DMQ_3_0                0x00a6a3f0 /* [RW] DMQ descriptor 3 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH3_DMQ_3_1                0x00a6a3f4 /* [RW] DMQ descriptor 3 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH3_DMQ_3_2                0x00a6a3f8 /* [RW] DMQ descriptor 3 - Transfer Size */
#define BCHP_XPT_WDMA_CH3_DMQ_3_3                0x00a6a3fc /* [RW] DMQ descriptor 3 - Current Descriptor Address and Control */

#endif /* #ifndef BCHP_XPT_WDMA_CH3_H__ */

/* End of File */
