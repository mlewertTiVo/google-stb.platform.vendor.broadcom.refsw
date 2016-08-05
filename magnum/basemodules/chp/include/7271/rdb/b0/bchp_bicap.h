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

#ifndef BCHP_BICAP_H__
#define BCHP_BICAP_H__

/***************************************************************************
 *BICAP - Buffered Input Capture
 ***************************************************************************/
#define BCHP_BICAP_REVISION                      0x2041a0c0 /* [RO] BICAP Revision */
#define BCHP_BICAP_CONTROL0                      0x2041a0c4 /* [RW] BICAP CONTROL FOR INPUT 0 */
#define BCHP_BICAP_CONTROL1                      0x2041a0c8 /* [RW] BICAP CONTROL FOR INPUT 1 */
#define BCHP_BICAP_FILTER0                       0x2041a0cc /* [RW] BICAP FILTER REGISTER FOR INPUT 0 */
#define BCHP_BICAP_FILTER1                       0x2041a0d0 /* [RW] BICAP FILTER REGISTER FOR INPUT 1 */
#define BCHP_BICAP_TIMEOUT0                      0x2041a0d4 /* [RW] TIMEOUT REGISTER 0 (Timeout Code: 0xFFF) */
#define BCHP_BICAP_TIMEOUT1                      0x2041a0d8 /* [RW] TIMEOUT REGISTER 1 (Timeout Code: 0xFFE) */
#define BCHP_BICAP_TIMEOUT2                      0x2041a0dc /* [RW] TIMEOUT REGISTER 2 (Timeout Code: 0xFFD) */
#define BCHP_BICAP_TIMEOUT3                      0x2041a0e0 /* [RW] TIMEOUT REGISTER 3 (Timeout Code: 0xFFC) */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT            0x2041a0e4 /* [RW] FIFO INACTIVITY TIMEOUT REGISTER */
#define BCHP_BICAP_FIFO_DATA                     0x2041a0e8 /* [RO] BICAP FIFO DATA REGISTER */
#define BCHP_BICAP_FIFO_CONTROL                  0x2041a0ec /* [RW] BICAP FIFO CONTROL */
#define BCHP_BICAP_STATUS                        0x2041a0f0 /* [RO] BICAP FIFO and TIMEOUT STATUS */
#define BCHP_BICAP_PWC0                          0x2041a0f4 /* [RW] BICAP PULSE WIDTH COUNTER VALUE FOR INPUT 0 */
#define BCHP_BICAP_PWC1                          0x2041a0f8 /* [RW] BICAP PULSE WIDTH COUNTER VALUE FOR INPUT 1 */

/***************************************************************************
 *REVISION - BICAP Revision
 ***************************************************************************/
/* BICAP :: REVISION :: reserved0 [31:16] */
#define BCHP_BICAP_REVISION_reserved0_MASK                         0xffff0000
#define BCHP_BICAP_REVISION_reserved0_SHIFT                        16

/* BICAP :: REVISION :: MAJOR [15:08] */
#define BCHP_BICAP_REVISION_MAJOR_MASK                             0x0000ff00
#define BCHP_BICAP_REVISION_MAJOR_SHIFT                            8
#define BCHP_BICAP_REVISION_MAJOR_DEFAULT                          0x00000001

/* BICAP :: REVISION :: MINOR [07:00] */
#define BCHP_BICAP_REVISION_MINOR_MASK                             0x000000ff
#define BCHP_BICAP_REVISION_MINOR_SHIFT                            0
#define BCHP_BICAP_REVISION_MINOR_DEFAULT                          0x00000001

/***************************************************************************
 *CONTROL0 - BICAP CONTROL FOR INPUT 0
 ***************************************************************************/
/* BICAP :: CONTROL0 :: tout_clk_div [31:24] */
#define BCHP_BICAP_CONTROL0_tout_clk_div_MASK                      0xff000000
#define BCHP_BICAP_CONTROL0_tout_clk_div_SHIFT                     24
#define BCHP_BICAP_CONTROL0_tout_clk_div_DEFAULT                   0x00000000

/* BICAP :: CONTROL0 :: reserved_for_eco0 [23:22] */
#define BCHP_BICAP_CONTROL0_reserved_for_eco0_MASK                 0x00c00000
#define BCHP_BICAP_CONTROL0_reserved_for_eco0_SHIFT                22
#define BCHP_BICAP_CONTROL0_reserved_for_eco0_DEFAULT              0x00000000

/* BICAP :: CONTROL0 :: sys_clk_div [21:08] */
#define BCHP_BICAP_CONTROL0_sys_clk_div_MASK                       0x003fff00
#define BCHP_BICAP_CONTROL0_sys_clk_div_SHIFT                      8
#define BCHP_BICAP_CONTROL0_sys_clk_div_DEFAULT                    0x0000000f

/* BICAP :: CONTROL0 :: reserved_for_eco1 [07:06] */
#define BCHP_BICAP_CONTROL0_reserved_for_eco1_MASK                 0x000000c0
#define BCHP_BICAP_CONTROL0_reserved_for_eco1_SHIFT                6
#define BCHP_BICAP_CONTROL0_reserved_for_eco1_DEFAULT              0x00000000

/* BICAP :: CONTROL0 :: cnt_mode [05:04] */
#define BCHP_BICAP_CONTROL0_cnt_mode_MASK                          0x00000030
#define BCHP_BICAP_CONTROL0_cnt_mode_SHIFT                         4
#define BCHP_BICAP_CONTROL0_cnt_mode_DEFAULT                       0x00000000

/* BICAP :: CONTROL0 :: pedgedet_en [03:03] */
#define BCHP_BICAP_CONTROL0_pedgedet_en_MASK                       0x00000008
#define BCHP_BICAP_CONTROL0_pedgedet_en_SHIFT                      3
#define BCHP_BICAP_CONTROL0_pedgedet_en_DEFAULT                    0x00000000

/* BICAP :: CONTROL0 :: nedgedet_en [02:02] */
#define BCHP_BICAP_CONTROL0_nedgedet_en_MASK                       0x00000004
#define BCHP_BICAP_CONTROL0_nedgedet_en_SHIFT                      2
#define BCHP_BICAP_CONTROL0_nedgedet_en_DEFAULT                    0x00000000

/* BICAP :: CONTROL0 :: insig_inv [01:01] */
#define BCHP_BICAP_CONTROL0_insig_inv_MASK                         0x00000002
#define BCHP_BICAP_CONTROL0_insig_inv_SHIFT                        1
#define BCHP_BICAP_CONTROL0_insig_inv_DEFAULT                      0x00000000

/* BICAP :: CONTROL0 :: bicap_en [00:00] */
#define BCHP_BICAP_CONTROL0_bicap_en_MASK                          0x00000001
#define BCHP_BICAP_CONTROL0_bicap_en_SHIFT                         0
#define BCHP_BICAP_CONTROL0_bicap_en_DEFAULT                       0x00000000

/***************************************************************************
 *CONTROL1 - BICAP CONTROL FOR INPUT 1
 ***************************************************************************/
/* BICAP :: CONTROL1 :: tout_clk_div [31:24] */
#define BCHP_BICAP_CONTROL1_tout_clk_div_MASK                      0xff000000
#define BCHP_BICAP_CONTROL1_tout_clk_div_SHIFT                     24
#define BCHP_BICAP_CONTROL1_tout_clk_div_DEFAULT                   0x00000000

/* BICAP :: CONTROL1 :: reserved_for_eco0 [23:22] */
#define BCHP_BICAP_CONTROL1_reserved_for_eco0_MASK                 0x00c00000
#define BCHP_BICAP_CONTROL1_reserved_for_eco0_SHIFT                22
#define BCHP_BICAP_CONTROL1_reserved_for_eco0_DEFAULT              0x00000000

/* BICAP :: CONTROL1 :: sys_clk_div [21:08] */
#define BCHP_BICAP_CONTROL1_sys_clk_div_MASK                       0x003fff00
#define BCHP_BICAP_CONTROL1_sys_clk_div_SHIFT                      8
#define BCHP_BICAP_CONTROL1_sys_clk_div_DEFAULT                    0x0000000f

/* BICAP :: CONTROL1 :: reserved_for_eco1 [07:06] */
#define BCHP_BICAP_CONTROL1_reserved_for_eco1_MASK                 0x000000c0
#define BCHP_BICAP_CONTROL1_reserved_for_eco1_SHIFT                6
#define BCHP_BICAP_CONTROL1_reserved_for_eco1_DEFAULT              0x00000000

/* BICAP :: CONTROL1 :: cnt_mode [05:04] */
#define BCHP_BICAP_CONTROL1_cnt_mode_MASK                          0x00000030
#define BCHP_BICAP_CONTROL1_cnt_mode_SHIFT                         4
#define BCHP_BICAP_CONTROL1_cnt_mode_DEFAULT                       0x00000000

/* BICAP :: CONTROL1 :: pedgedet_en [03:03] */
#define BCHP_BICAP_CONTROL1_pedgedet_en_MASK                       0x00000008
#define BCHP_BICAP_CONTROL1_pedgedet_en_SHIFT                      3
#define BCHP_BICAP_CONTROL1_pedgedet_en_DEFAULT                    0x00000000

/* BICAP :: CONTROL1 :: nedgedet_en [02:02] */
#define BCHP_BICAP_CONTROL1_nedgedet_en_MASK                       0x00000004
#define BCHP_BICAP_CONTROL1_nedgedet_en_SHIFT                      2
#define BCHP_BICAP_CONTROL1_nedgedet_en_DEFAULT                    0x00000000

/* BICAP :: CONTROL1 :: insig_inv [01:01] */
#define BCHP_BICAP_CONTROL1_insig_inv_MASK                         0x00000002
#define BCHP_BICAP_CONTROL1_insig_inv_SHIFT                        1
#define BCHP_BICAP_CONTROL1_insig_inv_DEFAULT                      0x00000000

/* BICAP :: CONTROL1 :: bicap_en [00:00] */
#define BCHP_BICAP_CONTROL1_bicap_en_MASK                          0x00000001
#define BCHP_BICAP_CONTROL1_bicap_en_SHIFT                         0
#define BCHP_BICAP_CONTROL1_bicap_en_DEFAULT                       0x00000000

/***************************************************************************
 *FILTER0 - BICAP FILTER REGISTER FOR INPUT 0
 ***************************************************************************/
/* BICAP :: FILTER0 :: reserved0 [31:10] */
#define BCHP_BICAP_FILTER0_reserved0_MASK                          0xfffffc00
#define BCHP_BICAP_FILTER0_reserved0_SHIFT                         10

/* BICAP :: FILTER0 :: filter_bypass [09:09] */
#define BCHP_BICAP_FILTER0_filter_bypass_MASK                      0x00000200
#define BCHP_BICAP_FILTER0_filter_bypass_SHIFT                     9
#define BCHP_BICAP_FILTER0_filter_bypass_DEFAULT                   0x00000000

/* BICAP :: FILTER0 :: filter_clk_sel [08:08] */
#define BCHP_BICAP_FILTER0_filter_clk_sel_MASK                     0x00000100
#define BCHP_BICAP_FILTER0_filter_clk_sel_SHIFT                    8
#define BCHP_BICAP_FILTER0_filter_clk_sel_DEFAULT                  0x00000000
#define BCHP_BICAP_FILTER0_filter_clk_sel_bicap_clk_filter         0
#define BCHP_BICAP_FILTER0_filter_clk_sel_sys_clk_filter           1

/* BICAP :: FILTER0 :: reserved_for_eco1 [07:07] */
#define BCHP_BICAP_FILTER0_reserved_for_eco1_MASK                  0x00000080
#define BCHP_BICAP_FILTER0_reserved_for_eco1_SHIFT                 7
#define BCHP_BICAP_FILTER0_reserved_for_eco1_DEFAULT               0x00000000

/* union - case bicap_clk_sel [06:00] */
/* BICAP :: FILTER0 :: bicap_clk_sel :: filter [06:00] */
#define BCHP_BICAP_FILTER0_bicap_clk_sel_filter_MASK               0x0000007f
#define BCHP_BICAP_FILTER0_bicap_clk_sel_filter_SHIFT              0
#define BCHP_BICAP_FILTER0_bicap_clk_sel_filter_DEFAULT            0x00000000

/* union - case sys_clk_sel [06:00] */
/* BICAP :: FILTER0 :: sys_clk_sel :: filter [06:00] */
#define BCHP_BICAP_FILTER0_sys_clk_sel_filter_MASK                 0x0000007f
#define BCHP_BICAP_FILTER0_sys_clk_sel_filter_SHIFT                0
#define BCHP_BICAP_FILTER0_sys_clk_sel_filter_DEFAULT              0x00000000

/***************************************************************************
 *FILTER1 - BICAP FILTER REGISTER FOR INPUT 1
 ***************************************************************************/
/* BICAP :: FILTER1 :: reserved0 [31:10] */
#define BCHP_BICAP_FILTER1_reserved0_MASK                          0xfffffc00
#define BCHP_BICAP_FILTER1_reserved0_SHIFT                         10

/* BICAP :: FILTER1 :: filter_bypass [09:09] */
#define BCHP_BICAP_FILTER1_filter_bypass_MASK                      0x00000200
#define BCHP_BICAP_FILTER1_filter_bypass_SHIFT                     9
#define BCHP_BICAP_FILTER1_filter_bypass_DEFAULT                   0x00000000

/* BICAP :: FILTER1 :: filter_clk_sel [08:08] */
#define BCHP_BICAP_FILTER1_filter_clk_sel_MASK                     0x00000100
#define BCHP_BICAP_FILTER1_filter_clk_sel_SHIFT                    8
#define BCHP_BICAP_FILTER1_filter_clk_sel_DEFAULT                  0x00000000
#define BCHP_BICAP_FILTER1_filter_clk_sel_bicap_clk_filter         0
#define BCHP_BICAP_FILTER1_filter_clk_sel_sys_clk_filter           1

/* BICAP :: FILTER1 :: reserved_for_eco1 [07:07] */
#define BCHP_BICAP_FILTER1_reserved_for_eco1_MASK                  0x00000080
#define BCHP_BICAP_FILTER1_reserved_for_eco1_SHIFT                 7
#define BCHP_BICAP_FILTER1_reserved_for_eco1_DEFAULT               0x00000000

/* union - case bicap_clk_sel [06:00] */
/* BICAP :: FILTER1 :: bicap_clk_sel :: filter [06:00] */
#define BCHP_BICAP_FILTER1_bicap_clk_sel_filter_MASK               0x0000007f
#define BCHP_BICAP_FILTER1_bicap_clk_sel_filter_SHIFT              0
#define BCHP_BICAP_FILTER1_bicap_clk_sel_filter_DEFAULT            0x00000000

/* union - case sys_clk_sel [06:00] */
/* BICAP :: FILTER1 :: sys_clk_sel :: filter [06:00] */
#define BCHP_BICAP_FILTER1_sys_clk_sel_filter_MASK                 0x0000007f
#define BCHP_BICAP_FILTER1_sys_clk_sel_filter_SHIFT                0
#define BCHP_BICAP_FILTER1_sys_clk_sel_filter_DEFAULT              0x00000000

/***************************************************************************
 *TIMEOUT0 - TIMEOUT REGISTER 0 (Timeout Code: 0xFFF)
 ***************************************************************************/
/* BICAP :: TIMEOUT0 :: reserved0 [31:20] */
#define BCHP_BICAP_TIMEOUT0_reserved0_MASK                         0xfff00000
#define BCHP_BICAP_TIMEOUT0_reserved0_SHIFT                        20

/* BICAP :: TIMEOUT0 :: input_sel [19:19] */
#define BCHP_BICAP_TIMEOUT0_input_sel_MASK                         0x00080000
#define BCHP_BICAP_TIMEOUT0_input_sel_SHIFT                        19
#define BCHP_BICAP_TIMEOUT0_input_sel_DEFAULT                      0x00000000

/* BICAP :: TIMEOUT0 :: edge_sel [18:17] */
#define BCHP_BICAP_TIMEOUT0_edge_sel_MASK                          0x00060000
#define BCHP_BICAP_TIMEOUT0_edge_sel_SHIFT                         17
#define BCHP_BICAP_TIMEOUT0_edge_sel_DEFAULT                       0x00000000

/* BICAP :: TIMEOUT0 :: clk_sel [16:16] */
#define BCHP_BICAP_TIMEOUT0_clk_sel_MASK                           0x00010000
#define BCHP_BICAP_TIMEOUT0_clk_sel_SHIFT                          16
#define BCHP_BICAP_TIMEOUT0_clk_sel_DEFAULT                        0x00000000

/* BICAP :: TIMEOUT0 :: reserved1 [15:12] */
#define BCHP_BICAP_TIMEOUT0_reserved1_MASK                         0x0000f000
#define BCHP_BICAP_TIMEOUT0_reserved1_SHIFT                        12

/* BICAP :: TIMEOUT0 :: tout [11:00] */
#define BCHP_BICAP_TIMEOUT0_tout_MASK                              0x00000fff
#define BCHP_BICAP_TIMEOUT0_tout_SHIFT                             0
#define BCHP_BICAP_TIMEOUT0_tout_DEFAULT                           0x00000000

/***************************************************************************
 *TIMEOUT1 - TIMEOUT REGISTER 1 (Timeout Code: 0xFFE)
 ***************************************************************************/
/* BICAP :: TIMEOUT1 :: reserved0 [31:20] */
#define BCHP_BICAP_TIMEOUT1_reserved0_MASK                         0xfff00000
#define BCHP_BICAP_TIMEOUT1_reserved0_SHIFT                        20

/* BICAP :: TIMEOUT1 :: input_sel [19:19] */
#define BCHP_BICAP_TIMEOUT1_input_sel_MASK                         0x00080000
#define BCHP_BICAP_TIMEOUT1_input_sel_SHIFT                        19
#define BCHP_BICAP_TIMEOUT1_input_sel_DEFAULT                      0x00000000

/* BICAP :: TIMEOUT1 :: edge_sel [18:17] */
#define BCHP_BICAP_TIMEOUT1_edge_sel_MASK                          0x00060000
#define BCHP_BICAP_TIMEOUT1_edge_sel_SHIFT                         17
#define BCHP_BICAP_TIMEOUT1_edge_sel_DEFAULT                       0x00000000

/* BICAP :: TIMEOUT1 :: clk_sel [16:16] */
#define BCHP_BICAP_TIMEOUT1_clk_sel_MASK                           0x00010000
#define BCHP_BICAP_TIMEOUT1_clk_sel_SHIFT                          16
#define BCHP_BICAP_TIMEOUT1_clk_sel_DEFAULT                        0x00000000

/* BICAP :: TIMEOUT1 :: reserved1 [15:12] */
#define BCHP_BICAP_TIMEOUT1_reserved1_MASK                         0x0000f000
#define BCHP_BICAP_TIMEOUT1_reserved1_SHIFT                        12

/* BICAP :: TIMEOUT1 :: tout [11:00] */
#define BCHP_BICAP_TIMEOUT1_tout_MASK                              0x00000fff
#define BCHP_BICAP_TIMEOUT1_tout_SHIFT                             0
#define BCHP_BICAP_TIMEOUT1_tout_DEFAULT                           0x00000000

/***************************************************************************
 *TIMEOUT2 - TIMEOUT REGISTER 2 (Timeout Code: 0xFFD)
 ***************************************************************************/
/* BICAP :: TIMEOUT2 :: reserved0 [31:20] */
#define BCHP_BICAP_TIMEOUT2_reserved0_MASK                         0xfff00000
#define BCHP_BICAP_TIMEOUT2_reserved0_SHIFT                        20

/* BICAP :: TIMEOUT2 :: input_sel [19:19] */
#define BCHP_BICAP_TIMEOUT2_input_sel_MASK                         0x00080000
#define BCHP_BICAP_TIMEOUT2_input_sel_SHIFT                        19
#define BCHP_BICAP_TIMEOUT2_input_sel_DEFAULT                      0x00000000

/* BICAP :: TIMEOUT2 :: edge_sel [18:17] */
#define BCHP_BICAP_TIMEOUT2_edge_sel_MASK                          0x00060000
#define BCHP_BICAP_TIMEOUT2_edge_sel_SHIFT                         17
#define BCHP_BICAP_TIMEOUT2_edge_sel_DEFAULT                       0x00000000

/* BICAP :: TIMEOUT2 :: clk_sel [16:16] */
#define BCHP_BICAP_TIMEOUT2_clk_sel_MASK                           0x00010000
#define BCHP_BICAP_TIMEOUT2_clk_sel_SHIFT                          16
#define BCHP_BICAP_TIMEOUT2_clk_sel_DEFAULT                        0x00000000

/* BICAP :: TIMEOUT2 :: reserved1 [15:12] */
#define BCHP_BICAP_TIMEOUT2_reserved1_MASK                         0x0000f000
#define BCHP_BICAP_TIMEOUT2_reserved1_SHIFT                        12

/* BICAP :: TIMEOUT2 :: tout [11:00] */
#define BCHP_BICAP_TIMEOUT2_tout_MASK                              0x00000fff
#define BCHP_BICAP_TIMEOUT2_tout_SHIFT                             0
#define BCHP_BICAP_TIMEOUT2_tout_DEFAULT                           0x00000000

/***************************************************************************
 *TIMEOUT3 - TIMEOUT REGISTER 3 (Timeout Code: 0xFFC)
 ***************************************************************************/
/* BICAP :: TIMEOUT3 :: reserved0 [31:20] */
#define BCHP_BICAP_TIMEOUT3_reserved0_MASK                         0xfff00000
#define BCHP_BICAP_TIMEOUT3_reserved0_SHIFT                        20

/* BICAP :: TIMEOUT3 :: input_sel [19:19] */
#define BCHP_BICAP_TIMEOUT3_input_sel_MASK                         0x00080000
#define BCHP_BICAP_TIMEOUT3_input_sel_SHIFT                        19
#define BCHP_BICAP_TIMEOUT3_input_sel_DEFAULT                      0x00000000

/* BICAP :: TIMEOUT3 :: edge_sel [18:17] */
#define BCHP_BICAP_TIMEOUT3_edge_sel_MASK                          0x00060000
#define BCHP_BICAP_TIMEOUT3_edge_sel_SHIFT                         17
#define BCHP_BICAP_TIMEOUT3_edge_sel_DEFAULT                       0x00000000

/* BICAP :: TIMEOUT3 :: clk_sel [16:16] */
#define BCHP_BICAP_TIMEOUT3_clk_sel_MASK                           0x00010000
#define BCHP_BICAP_TIMEOUT3_clk_sel_SHIFT                          16
#define BCHP_BICAP_TIMEOUT3_clk_sel_DEFAULT                        0x00000000

/* BICAP :: TIMEOUT3 :: reserved1 [15:12] */
#define BCHP_BICAP_TIMEOUT3_reserved1_MASK                         0x0000f000
#define BCHP_BICAP_TIMEOUT3_reserved1_SHIFT                        12

/* BICAP :: TIMEOUT3 :: tout [11:00] */
#define BCHP_BICAP_TIMEOUT3_tout_MASK                              0x00000fff
#define BCHP_BICAP_TIMEOUT3_tout_SHIFT                             0
#define BCHP_BICAP_TIMEOUT3_tout_DEFAULT                           0x00000000

/***************************************************************************
 *FIFO_INACT_TIMEOUT - FIFO INACTIVITY TIMEOUT REGISTER
 ***************************************************************************/
/* BICAP :: FIFO_INACT_TIMEOUT :: reserved0 [31:20] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_reserved0_MASK               0xfff00000
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_reserved0_SHIFT              20

/* BICAP :: FIFO_INACT_TIMEOUT :: bicap_clk_sel [19:19] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_bicap_clk_sel_MASK           0x00080000
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_bicap_clk_sel_SHIFT          19
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_bicap_clk_sel_DEFAULT        0x00000000

/* BICAP :: FIFO_INACT_TIMEOUT :: event_sel [18:17] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_event_sel_MASK               0x00060000
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_event_sel_SHIFT              17
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_event_sel_DEFAULT            0x00000000

/* BICAP :: FIFO_INACT_TIMEOUT :: clk_sel [16:16] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_clk_sel_MASK                 0x00010000
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_clk_sel_SHIFT                16
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_clk_sel_DEFAULT              0x00000000

/* BICAP :: FIFO_INACT_TIMEOUT :: reserved1 [15:12] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_reserved1_MASK               0x0000f000
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_reserved1_SHIFT              12

/* BICAP :: FIFO_INACT_TIMEOUT :: tout [11:00] */
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_tout_MASK                    0x00000fff
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_tout_SHIFT                   0
#define BCHP_BICAP_FIFO_INACT_TIMEOUT_tout_DEFAULT                 0x00000000

/***************************************************************************
 *FIFO_DATA - BICAP FIFO DATA REGISTER
 ***************************************************************************/
/* BICAP :: FIFO_DATA :: reserved0 [31:14] */
#define BCHP_BICAP_FIFO_DATA_reserved0_MASK                        0xffffc000
#define BCHP_BICAP_FIFO_DATA_reserved0_SHIFT                       14

/* BICAP :: FIFO_DATA :: insig_source [13:13] */
#define BCHP_BICAP_FIFO_DATA_insig_source_MASK                     0x00002000
#define BCHP_BICAP_FIFO_DATA_insig_source_SHIFT                    13

/* BICAP :: FIFO_DATA :: edge_pol [12:12] */
#define BCHP_BICAP_FIFO_DATA_edge_pol_MASK                         0x00001000
#define BCHP_BICAP_FIFO_DATA_edge_pol_SHIFT                        12

/* BICAP :: FIFO_DATA :: data [11:00] */
#define BCHP_BICAP_FIFO_DATA_data_MASK                             0x00000fff
#define BCHP_BICAP_FIFO_DATA_data_SHIFT                            0

/***************************************************************************
 *FIFO_CONTROL - BICAP FIFO CONTROL
 ***************************************************************************/
/* BICAP :: FIFO_CONTROL :: reserved0 [31:14] */
#define BCHP_BICAP_FIFO_CONTROL_reserved0_MASK                     0xffffc000
#define BCHP_BICAP_FIFO_CONTROL_reserved0_SHIFT                    14

/* BICAP :: FIFO_CONTROL :: fifo_depth [13:08] */
#define BCHP_BICAP_FIFO_CONTROL_fifo_depth_MASK                    0x00003f00
#define BCHP_BICAP_FIFO_CONTROL_fifo_depth_SHIFT                   8

/* BICAP :: FIFO_CONTROL :: reserved1 [07:07] */
#define BCHP_BICAP_FIFO_CONTROL_reserved1_MASK                     0x00000080
#define BCHP_BICAP_FIFO_CONTROL_reserved1_SHIFT                    7

/* BICAP :: FIFO_CONTROL :: trig_lvl [06:01] */
#define BCHP_BICAP_FIFO_CONTROL_trig_lvl_MASK                      0x0000007e
#define BCHP_BICAP_FIFO_CONTROL_trig_lvl_SHIFT                     1
#define BCHP_BICAP_FIFO_CONTROL_trig_lvl_DEFAULT                   0x00000000

/* BICAP :: FIFO_CONTROL :: reset [00:00] */
#define BCHP_BICAP_FIFO_CONTROL_reset_MASK                         0x00000001
#define BCHP_BICAP_FIFO_CONTROL_reset_SHIFT                        0
#define BCHP_BICAP_FIFO_CONTROL_reset_DEFAULT                      0x00000000

/***************************************************************************
 *STATUS - BICAP FIFO and TIMEOUT STATUS
 ***************************************************************************/
/* BICAP :: STATUS :: reserved0 [31:16] */
#define BCHP_BICAP_STATUS_reserved0_MASK                           0xffff0000
#define BCHP_BICAP_STATUS_reserved0_SHIFT                          16

/* BICAP :: STATUS :: fifo_count [15:09] */
#define BCHP_BICAP_STATUS_fifo_count_MASK                          0x0000fe00
#define BCHP_BICAP_STATUS_fifo_count_SHIFT                         9
#define BCHP_BICAP_STATUS_fifo_count_DEFAULT                       0x00000000

/* BICAP :: STATUS :: fifo_full [08:08] */
#define BCHP_BICAP_STATUS_fifo_full_MASK                           0x00000100
#define BCHP_BICAP_STATUS_fifo_full_SHIFT                          8
#define BCHP_BICAP_STATUS_fifo_full_DEFAULT                        0x00000000

/* BICAP :: STATUS :: fifo_empty [07:07] */
#define BCHP_BICAP_STATUS_fifo_empty_MASK                          0x00000080
#define BCHP_BICAP_STATUS_fifo_empty_SHIFT                         7
#define BCHP_BICAP_STATUS_fifo_empty_DEFAULT                       0x00000001

/* BICAP :: STATUS :: fifo_overflow [06:06] */
#define BCHP_BICAP_STATUS_fifo_overflow_MASK                       0x00000040
#define BCHP_BICAP_STATUS_fifo_overflow_SHIFT                      6
#define BCHP_BICAP_STATUS_fifo_overflow_DEFAULT                    0x00000000

/* BICAP :: STATUS :: fifo_lvl_event [05:05] */
#define BCHP_BICAP_STATUS_fifo_lvl_event_MASK                      0x00000020
#define BCHP_BICAP_STATUS_fifo_lvl_event_SHIFT                     5
#define BCHP_BICAP_STATUS_fifo_lvl_event_DEFAULT                   0x00000000

/* BICAP :: STATUS :: fifo_inact_event [04:04] */
#define BCHP_BICAP_STATUS_fifo_inact_event_MASK                    0x00000010
#define BCHP_BICAP_STATUS_fifo_inact_event_SHIFT                   4
#define BCHP_BICAP_STATUS_fifo_inact_event_DEFAULT                 0x00000000

/* BICAP :: STATUS :: timeout3_event [03:03] */
#define BCHP_BICAP_STATUS_timeout3_event_MASK                      0x00000008
#define BCHP_BICAP_STATUS_timeout3_event_SHIFT                     3
#define BCHP_BICAP_STATUS_timeout3_event_DEFAULT                   0x00000000

/* BICAP :: STATUS :: timeout2_event [02:02] */
#define BCHP_BICAP_STATUS_timeout2_event_MASK                      0x00000004
#define BCHP_BICAP_STATUS_timeout2_event_SHIFT                     2
#define BCHP_BICAP_STATUS_timeout2_event_DEFAULT                   0x00000000

/* BICAP :: STATUS :: timeout1_event [01:01] */
#define BCHP_BICAP_STATUS_timeout1_event_MASK                      0x00000002
#define BCHP_BICAP_STATUS_timeout1_event_SHIFT                     1
#define BCHP_BICAP_STATUS_timeout1_event_DEFAULT                   0x00000000

/* BICAP :: STATUS :: timeout0_event [00:00] */
#define BCHP_BICAP_STATUS_timeout0_event_MASK                      0x00000001
#define BCHP_BICAP_STATUS_timeout0_event_SHIFT                     0
#define BCHP_BICAP_STATUS_timeout0_event_DEFAULT                   0x00000000

/***************************************************************************
 *PWC0 - BICAP PULSE WIDTH COUNTER VALUE FOR INPUT 0
 ***************************************************************************/
/* BICAP :: PWC0 :: reserved0 [31:12] */
#define BCHP_BICAP_PWC0_reserved0_MASK                             0xfffff000
#define BCHP_BICAP_PWC0_reserved0_SHIFT                            12

/* BICAP :: PWC0 :: count [11:00] */
#define BCHP_BICAP_PWC0_count_MASK                                 0x00000fff
#define BCHP_BICAP_PWC0_count_SHIFT                                0
#define BCHP_BICAP_PWC0_count_DEFAULT                              0x00000000

/***************************************************************************
 *PWC1 - BICAP PULSE WIDTH COUNTER VALUE FOR INPUT 1
 ***************************************************************************/
/* BICAP :: PWC1 :: reserved0 [31:12] */
#define BCHP_BICAP_PWC1_reserved0_MASK                             0xfffff000
#define BCHP_BICAP_PWC1_reserved0_SHIFT                            12

/* BICAP :: PWC1 :: count [11:00] */
#define BCHP_BICAP_PWC1_count_MASK                                 0x00000fff
#define BCHP_BICAP_PWC1_count_SHIFT                                0
#define BCHP_BICAP_PWC1_count_DEFAULT                              0x00000000

#endif /* #ifndef BCHP_BICAP_H__ */

/* End of File */
