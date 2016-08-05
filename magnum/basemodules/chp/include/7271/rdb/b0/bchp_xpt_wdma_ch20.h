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

#ifndef BCHP_XPT_WDMA_CH20_H__
#define BCHP_XPT_WDMA_CH20_H__

/***************************************************************************
 *XPT_WDMA_CH20 - WDMA Channel 20 Configuration
 ***************************************************************************/
#define BCHP_XPT_WDMA_CH20_FIRST_DESC_ADDR       0x20a6b400 /* [RW] First Descriptor Address */
#define BCHP_XPT_WDMA_CH20_NEXT_DESC_ADDR        0x20a6b404 /* [RW] Next Descriptor Address */
#define BCHP_XPT_WDMA_CH20_COMPLETED_DESC_ADDRESS 0x20a6b408 /* [RW] Completed Descriptor Address */
#define BCHP_XPT_WDMA_CH20_BTP_PACKET_GROUP_ID   0x20a6b40c /* [RW] Packet Group ID reported per BTP command */
#define BCHP_XPT_WDMA_CH20_RUN_VERSION_CONFIG    0x20a6b410 /* [RW] RUN_VERSION configuration */
#define BCHP_XPT_WDMA_CH20_OVERFLOW_REASONS      0x20a6b414 /* [RW] Overflow Reason */
#define BCHP_XPT_WDMA_CH20_DMQ_CONTROL_STRUCT    0x20a6b418 /* [RO] Descriptor Memory Queue Control Structure */
#define BCHP_XPT_WDMA_CH20_DATA_CONTROL          0x20a6b41c /* [RW] Data Control */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_BASE_PTR_HI 0x20a6b480 /* [RW] DRAM Buffer Base Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_BASE_PTR    0x20a6b484 /* [RW] DRAM Buffer Base Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_END_PTR_HI  0x20a6b488 /* [RW] DRAM Buffer End Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_END_PTR     0x20a6b48c /* [RW] DRAM Buffer End Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_RD_PTR_HI   0x20a6b490 /* [RW] DRAM Buffer Read Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_RD_PTR      0x20a6b494 /* [RW] DRAM Buffer Read Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_WR_PTR_HI   0x20a6b498 /* [RW] DRAM Buffer Write Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_WR_PTR      0x20a6b49c /* [RW] DRAM Buffer Write Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_VALID_PTR_HI 0x20a6b4a0 /* [RW] DRAM Buffer Valid Pointer - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_VALID_PTR   0x20a6b4a4 /* [RW] DRAM Buffer Valid Pointer - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_LOWER_THRESHOLD_HI 0x20a6b4a8 /* [RW] DRAM Buffer Lower Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_LOWER_THRESHOLD 0x20a6b4ac /* [RW] DRAM Buffer Lower Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_UPPER_THRESHOLD_HI 0x20a6b4b0 /* [RW] DRAM Buffer Upper Threshold - Upper bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_UPPER_THRESHOLD 0x20a6b4b4 /* [RW] DRAM Buffer Upper Threshold - Lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_STATUS      0x20a6b4b8 /* [RW] DRAM Buffer Status */
#define BCHP_XPT_WDMA_CH20_DRAM_BUFF_CONTROL     0x20a6b4bc /* [RW] DRAM Buffer Control */
#define BCHP_XPT_WDMA_CH20_DMQ_0_0               0x20a6b4c0 /* [RW] DMQ descriptor 0 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH20_DMQ_0_1               0x20a6b4c4 /* [RW] DMQ descriptor 0 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DMQ_0_2               0x20a6b4c8 /* [RW] DMQ descriptor 0 - Transfer Size */
#define BCHP_XPT_WDMA_CH20_DMQ_0_3               0x20a6b4cc /* [RW] DMQ descriptor 0 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH20_DMQ_1_0               0x20a6b4d0 /* [RW] DMQ descriptor 1 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH20_DMQ_1_1               0x20a6b4d4 /* [RW] DMQ descriptor 1 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DMQ_1_2               0x20a6b4d8 /* [RW] DMQ descriptor 1 - Transfer Size */
#define BCHP_XPT_WDMA_CH20_DMQ_1_3               0x20a6b4dc /* [RW] DMQ descriptor 1 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH20_DMQ_2_0               0x20a6b4e0 /* [RW] DMQ descriptor 2 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH20_DMQ_2_1               0x20a6b4e4 /* [RW] DMQ descriptor 2 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DMQ_2_2               0x20a6b4e8 /* [RW] DMQ descriptor 2 - Transfer Size */
#define BCHP_XPT_WDMA_CH20_DMQ_2_3               0x20a6b4ec /* [RW] DMQ descriptor 2 - Current Descriptor Address and Control */
#define BCHP_XPT_WDMA_CH20_DMQ_3_0               0x20a6b4f0 /* [RW] DMQ descriptor 3 - Write Address, Upper bits */
#define BCHP_XPT_WDMA_CH20_DMQ_3_1               0x20a6b4f4 /* [RW] DMQ descriptor 3 - Write Address, lower 32 bits */
#define BCHP_XPT_WDMA_CH20_DMQ_3_2               0x20a6b4f8 /* [RW] DMQ descriptor 3 - Transfer Size */
#define BCHP_XPT_WDMA_CH20_DMQ_3_3               0x20a6b4fc /* [RW] DMQ descriptor 3 - Current Descriptor Address and Control */

#endif /* #ifndef BCHP_XPT_WDMA_CH20_H__ */

/* End of File */
