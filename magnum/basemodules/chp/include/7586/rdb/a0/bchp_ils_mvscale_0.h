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
 * Date:           Generated on               Wed Feb 11 10:13:59 2015
 *                 Full Compile MD5 Checksum  f7f4bd55341805fcfe958ba5e47e65f4
 *                     (minus title and desc)
 *                 MD5 Checksum               95b679a9655597a92593cae55222c397
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15653
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_ILS_MVSCALE_0_H__
#define BCHP_ILS_MVSCALE_0_H__

/***************************************************************************
 *ILS_MVSCALE_0 - Spatial Scaler Fill Registers
 ***************************************************************************/
#define BCHP_ILS_MVSCALE_0_MODE                  0x00127200 /* [RW] MVSCALE_MODE */
#define BCHP_ILS_MVSCALE_0_MVS_CTL               0x00127204 /* [RW] MVSCALE_CTL */
#define BCHP_ILS_MVSCALE_0_DMA_ADDR              0x00127208 /* [RW] MVSCALE_DMA_ADDR */
#define BCHP_ILS_MVSCALE_0_X                     0x0012720c /* [RW] MVSCALE_X */
#define BCHP_ILS_MVSCALE_0_Y                     0x00127210 /* [RW] MVSCALE_Y */
#define BCHP_ILS_MVSCALE_0_STAT                  0x00127214 /* [RO] MVSCALE_STAT */
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL        0x00127218 /* [RW] MVSCALE_VECTOR_MEM_SEL */
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG           0x0012721c /* [RO] MVSCALE_STATE_DEBUG */
#define BCHP_ILS_MVSCALE_0_REFIDL0               0x00127220 /* [RO] MVSCALE_REFIDL0 */
#define BCHP_ILS_MVSCALE_0_REFIDL1               0x00127224 /* [RO] MVSCALE_REFIDL1 */
#define BCHP_ILS_MVSCALE_0_MVSCALE_DEBUG         0x00127228 /* [RO] MVSCALE_DEBUG */
#define BCHP_ILS_MVSCALE_0_END                   0x0012738c /* [RW] MVSCALE_END */

/***************************************************************************
 *MODE - MVSCALE_MODE
 ***************************************************************************/
/* ILS_MVSCALE_0 :: MODE :: reserved0 [31:29] */
#define BCHP_ILS_MVSCALE_0_MODE_reserved0_MASK                     0xe0000000
#define BCHP_ILS_MVSCALE_0_MODE_reserved0_SHIFT                    29

/* ILS_MVSCALE_0 :: MODE :: max_mbcount [28:16] */
#define BCHP_ILS_MVSCALE_0_MODE_max_mbcount_MASK                   0x1fff0000
#define BCHP_ILS_MVSCALE_0_MODE_max_mbcount_SHIFT                  16
#define BCHP_ILS_MVSCALE_0_MODE_max_mbcount_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: MODE :: reserved1 [15:04] */
#define BCHP_ILS_MVSCALE_0_MODE_reserved1_MASK                     0x0000fff0
#define BCHP_ILS_MVSCALE_0_MODE_reserved1_SHIFT                    4

/* ILS_MVSCALE_0 :: MODE :: no_pad_bytes [03:03] */
#define BCHP_ILS_MVSCALE_0_MODE_no_pad_bytes_MASK                  0x00000008
#define BCHP_ILS_MVSCALE_0_MODE_no_pad_bytes_SHIFT                 3
#define BCHP_ILS_MVSCALE_0_MODE_no_pad_bytes_DEFAULT               0x00000000

/* ILS_MVSCALE_0 :: MODE :: direct_8x8 [02:02] */
#define BCHP_ILS_MVSCALE_0_MODE_direct_8x8_MASK                    0x00000004
#define BCHP_ILS_MVSCALE_0_MODE_direct_8x8_SHIFT                   2
#define BCHP_ILS_MVSCALE_0_MODE_direct_8x8_DEFAULT                 0x00000000

/* ILS_MVSCALE_0 :: MODE :: restricted_res [01:01] */
#define BCHP_ILS_MVSCALE_0_MODE_restricted_res_MASK                0x00000002
#define BCHP_ILS_MVSCALE_0_MODE_restricted_res_SHIFT               1
#define BCHP_ILS_MVSCALE_0_MODE_restricted_res_DEFAULT             0x00000000

/* ILS_MVSCALE_0 :: MODE :: slice_is_b [00:00] */
#define BCHP_ILS_MVSCALE_0_MODE_slice_is_b_MASK                    0x00000001
#define BCHP_ILS_MVSCALE_0_MODE_slice_is_b_SHIFT                   0
#define BCHP_ILS_MVSCALE_0_MODE_slice_is_b_DEFAULT                 0x00000000

/***************************************************************************
 *MVS_CTL - MVSCALE_CTL
 ***************************************************************************/
/* ILS_MVSCALE_0 :: MVS_CTL :: reserved0 [31:08] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_reserved0_MASK                  0xffffff00
#define BCHP_ILS_MVSCALE_0_MVS_CTL_reserved0_SHIFT                 8

/* ILS_MVSCALE_0 :: MVS_CTL :: scale_single_step [07:07] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_scale_single_step_MASK          0x00000080
#define BCHP_ILS_MVSCALE_0_MVS_CTL_scale_single_step_SHIFT         7

/* ILS_MVSCALE_0 :: MVS_CTL :: scale_disable [06:06] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_scale_disable_MASK              0x00000040
#define BCHP_ILS_MVSCALE_0_MVS_CTL_scale_disable_SHIFT             6

/* ILS_MVSCALE_0 :: MVS_CTL :: fetch_load_done [05:05] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_load_done_MASK            0x00000020
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_load_done_SHIFT           5

/* ILS_MVSCALE_0 :: MVS_CTL :: fetch_load [04:04] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_load_MASK                 0x00000010
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_load_SHIFT                4

/* ILS_MVSCALE_0 :: MVS_CTL :: fetch_disable [03:03] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_disable_MASK              0x00000008
#define BCHP_ILS_MVSCALE_0_MVS_CTL_fetch_disable_SHIFT             3

/* ILS_MVSCALE_0 :: MVS_CTL :: mv_scale_flush [02:02] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_flush_MASK             0x00000004
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_flush_SHIFT            2
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_flush_DEFAULT          0x00000000

/* ILS_MVSCALE_0 :: MVS_CTL :: mv_output_done [01:01] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_output_done_MASK             0x00000002
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_output_done_SHIFT            1
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_output_done_DEFAULT          0x00000000

/* ILS_MVSCALE_0 :: MVS_CTL :: mv_scale_enable [00:00] */
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_enable_MASK            0x00000001
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_enable_SHIFT           0
#define BCHP_ILS_MVSCALE_0_MVS_CTL_mv_scale_enable_DEFAULT         0x00000000

/***************************************************************************
 *DMA_ADDR - MVSCALE_DMA_ADDR
 ***************************************************************************/
/* ILS_MVSCALE_0 :: DMA_ADDR :: dma_addr [31:02] */
#define BCHP_ILS_MVSCALE_0_DMA_ADDR_dma_addr_MASK                  0xfffffffc
#define BCHP_ILS_MVSCALE_0_DMA_ADDR_dma_addr_SHIFT                 2
#define BCHP_ILS_MVSCALE_0_DMA_ADDR_dma_addr_DEFAULT               0x00000000

/* ILS_MVSCALE_0 :: DMA_ADDR :: reserved0 [01:00] */
#define BCHP_ILS_MVSCALE_0_DMA_ADDR_reserved0_MASK                 0x00000003
#define BCHP_ILS_MVSCALE_0_DMA_ADDR_reserved0_SHIFT                0

/***************************************************************************
 *X - MVSCALE_X
 ***************************************************************************/
/* ILS_MVSCALE_0 :: X :: reserved0 [31:20] */
#define BCHP_ILS_MVSCALE_0_X_reserved0_MASK                        0xfff00000
#define BCHP_ILS_MVSCALE_0_X_reserved0_SHIFT                       20

/* ILS_MVSCALE_0 :: X :: scaleX [19:00] */
#define BCHP_ILS_MVSCALE_0_X_scaleX_MASK                           0x000fffff
#define BCHP_ILS_MVSCALE_0_X_scaleX_SHIFT                          0
#define BCHP_ILS_MVSCALE_0_X_scaleX_DEFAULT                        0x00000000

/***************************************************************************
 *Y - MVSCALE_Y
 ***************************************************************************/
/* ILS_MVSCALE_0 :: Y :: reserved0 [31:20] */
#define BCHP_ILS_MVSCALE_0_Y_reserved0_MASK                        0xfff00000
#define BCHP_ILS_MVSCALE_0_Y_reserved0_SHIFT                       20

/* ILS_MVSCALE_0 :: Y :: scaleY [19:00] */
#define BCHP_ILS_MVSCALE_0_Y_scaleY_MASK                           0x000fffff
#define BCHP_ILS_MVSCALE_0_Y_scaleY_SHIFT                          0
#define BCHP_ILS_MVSCALE_0_Y_scaleY_DEFAULT                        0x00000000

/***************************************************************************
 *STAT - MVSCALE_STAT
 ***************************************************************************/
/* ILS_MVSCALE_0 :: STAT :: part_pred_3 [31:30] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_3_MASK                   0xc0000000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_3_SHIFT                  30
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_3_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: part_pred_2 [29:28] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_2_MASK                   0x30000000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_2_SHIFT                  28
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_2_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: part_pred_1 [27:26] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_1_MASK                   0x0c000000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_1_SHIFT                  26
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_1_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: part_pred_0 [25:24] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_0_MASK                   0x03000000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_0_SHIFT                  24
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_0_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: reserved0 [23:20] */
#define BCHP_ILS_MVSCALE_0_STAT_reserved0_MASK                     0x00f00000
#define BCHP_ILS_MVSCALE_0_STAT_reserved0_SHIFT                    20

/* ILS_MVSCALE_0 :: STAT :: part_pred_b [19:18] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_b_MASK                   0x000c0000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_b_SHIFT                  18
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_b_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: part_pred_a [17:16] */
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_a_MASK                   0x00030000
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_a_SHIFT                  16
#define BCHP_ILS_MVSCALE_0_STAT_part_pred_a_DEFAULT                0x00000000

/* ILS_MVSCALE_0 :: STAT :: subpart3_size [15:14] */
#define BCHP_ILS_MVSCALE_0_STAT_subpart3_size_MASK                 0x0000c000
#define BCHP_ILS_MVSCALE_0_STAT_subpart3_size_SHIFT                14
#define BCHP_ILS_MVSCALE_0_STAT_subpart3_size_DEFAULT              0x00000000

/* ILS_MVSCALE_0 :: STAT :: subpart2_size [13:12] */
#define BCHP_ILS_MVSCALE_0_STAT_subpart2_size_MASK                 0x00003000
#define BCHP_ILS_MVSCALE_0_STAT_subpart2_size_SHIFT                12
#define BCHP_ILS_MVSCALE_0_STAT_subpart2_size_DEFAULT              0x00000000

/* ILS_MVSCALE_0 :: STAT :: subpart1_size [11:10] */
#define BCHP_ILS_MVSCALE_0_STAT_subpart1_size_MASK                 0x00000c00
#define BCHP_ILS_MVSCALE_0_STAT_subpart1_size_SHIFT                10
#define BCHP_ILS_MVSCALE_0_STAT_subpart1_size_DEFAULT              0x00000000

/* ILS_MVSCALE_0 :: STAT :: subpart0_size [09:08] */
#define BCHP_ILS_MVSCALE_0_STAT_subpart0_size_MASK                 0x00000300
#define BCHP_ILS_MVSCALE_0_STAT_subpart0_size_SHIFT                8
#define BCHP_ILS_MVSCALE_0_STAT_subpart0_size_DEFAULT              0x00000000

/* ILS_MVSCALE_0 :: STAT :: reserved1 [07:06] */
#define BCHP_ILS_MVSCALE_0_STAT_reserved1_MASK                     0x000000c0
#define BCHP_ILS_MVSCALE_0_STAT_reserved1_SHIFT                    6

/* ILS_MVSCALE_0 :: STAT :: part_size [05:04] */
#define BCHP_ILS_MVSCALE_0_STAT_part_size_MASK                     0x00000030
#define BCHP_ILS_MVSCALE_0_STAT_part_size_SHIFT                    4
#define BCHP_ILS_MVSCALE_0_STAT_part_size_DEFAULT                  0x00000000

/* ILS_MVSCALE_0 :: STAT :: reserved2 [03:03] */
#define BCHP_ILS_MVSCALE_0_STAT_reserved2_MASK                     0x00000008
#define BCHP_ILS_MVSCALE_0_STAT_reserved2_SHIFT                    3

/* ILS_MVSCALE_0 :: STAT :: intra_mb [02:02] */
#define BCHP_ILS_MVSCALE_0_STAT_intra_mb_MASK                      0x00000004
#define BCHP_ILS_MVSCALE_0_STAT_intra_mb_SHIFT                     2
#define BCHP_ILS_MVSCALE_0_STAT_intra_mb_DEFAULT                   0x00000000

/* ILS_MVSCALE_0 :: STAT :: fetch_done [01:01] */
#define BCHP_ILS_MVSCALE_0_STAT_fetch_done_MASK                    0x00000002
#define BCHP_ILS_MVSCALE_0_STAT_fetch_done_SHIFT                   1
#define BCHP_ILS_MVSCALE_0_STAT_fetch_done_DEFAULT                 0x00000000

/* ILS_MVSCALE_0 :: STAT :: mb_done [00:00] */
#define BCHP_ILS_MVSCALE_0_STAT_mb_done_MASK                       0x00000001
#define BCHP_ILS_MVSCALE_0_STAT_mb_done_SHIFT                      0
#define BCHP_ILS_MVSCALE_0_STAT_mb_done_DEFAULT                    0x00000000

/***************************************************************************
 *VECTOR_MEM_SEL - MVSCALE_VECTOR_MEM_SEL
 ***************************************************************************/
/* ILS_MVSCALE_0 :: VECTOR_MEM_SEL :: reserved0 [31:02] */
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL_reserved0_MASK           0xfffffffc
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL_reserved0_SHIFT          2

/* ILS_MVSCALE_0 :: VECTOR_MEM_SEL :: mvscale_ram_sel [01:00] */
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL_mvscale_ram_sel_MASK     0x00000003
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL_mvscale_ram_sel_SHIFT    0
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_SEL_mvscale_ram_sel_DEFAULT  0x00000000

/***************************************************************************
 *STATE_DEBUG - MVSCALE_STATE_DEBUG
 ***************************************************************************/
/* ILS_MVSCALE_0 :: STATE_DEBUG :: reserved0 [31:07] */
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG_reserved0_MASK              0xffffff80
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG_reserved0_SHIFT             7

/* ILS_MVSCALE_0 :: STATE_DEBUG :: mvscale_state [06:00] */
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG_mvscale_state_MASK          0x0000007f
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG_mvscale_state_SHIFT         0
#define BCHP_ILS_MVSCALE_0_STATE_DEBUG_mvscale_state_DEFAULT       0x00000000

/***************************************************************************
 *REFIDL0 - MVSCALE_REFIDL0
 ***************************************************************************/
/* ILS_MVSCALE_0 :: REFIDL0 :: unused3 [31:30] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused3_MASK                    0xc0000000
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused3_SHIFT                   30

/* ILS_MVSCALE_0 :: REFIDL0 :: RefIdx3 [29:24] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx3_MASK                    0x3f000000
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx3_SHIFT                   24

/* ILS_MVSCALE_0 :: REFIDL0 :: unused2 [23:22] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused2_MASK                    0x00c00000
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused2_SHIFT                   22

/* ILS_MVSCALE_0 :: REFIDL0 :: RefIdx2 [21:16] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx2_MASK                    0x003f0000
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx2_SHIFT                   16

/* ILS_MVSCALE_0 :: REFIDL0 :: unused1 [15:14] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused1_MASK                    0x0000c000
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused1_SHIFT                   14

/* ILS_MVSCALE_0 :: REFIDL0 :: RefIdx1 [13:08] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx1_MASK                    0x00003f00
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx1_SHIFT                   8

/* ILS_MVSCALE_0 :: REFIDL0 :: unused0 [07:06] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused0_MASK                    0x000000c0
#define BCHP_ILS_MVSCALE_0_REFIDL0_unused0_SHIFT                   6

/* ILS_MVSCALE_0 :: REFIDL0 :: RefIdx0 [05:00] */
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx0_MASK                    0x0000003f
#define BCHP_ILS_MVSCALE_0_REFIDL0_RefIdx0_SHIFT                   0

/***************************************************************************
 *REFIDL1 - MVSCALE_REFIDL1
 ***************************************************************************/
/* ILS_MVSCALE_0 :: REFIDL1 :: unused3 [31:30] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused3_MASK                    0xc0000000
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused3_SHIFT                   30

/* ILS_MVSCALE_0 :: REFIDL1 :: RefIdx3 [29:24] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx3_MASK                    0x3f000000
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx3_SHIFT                   24

/* ILS_MVSCALE_0 :: REFIDL1 :: unused2 [23:22] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused2_MASK                    0x00c00000
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused2_SHIFT                   22

/* ILS_MVSCALE_0 :: REFIDL1 :: RefIdx2 [21:16] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx2_MASK                    0x003f0000
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx2_SHIFT                   16

/* ILS_MVSCALE_0 :: REFIDL1 :: unused1 [15:14] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused1_MASK                    0x0000c000
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused1_SHIFT                   14

/* ILS_MVSCALE_0 :: REFIDL1 :: RefIdx1 [13:08] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx1_MASK                    0x00003f00
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx1_SHIFT                   8

/* ILS_MVSCALE_0 :: REFIDL1 :: unused0 [07:06] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused0_MASK                    0x000000c0
#define BCHP_ILS_MVSCALE_0_REFIDL1_unused0_SHIFT                   6

/* ILS_MVSCALE_0 :: REFIDL1 :: RefIdx0 [05:00] */
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx0_MASK                    0x0000003f
#define BCHP_ILS_MVSCALE_0_REFIDL1_RefIdx0_SHIFT                   0

/***************************************************************************
 *MVSCALE_DEBUG - MVSCALE_DEBUG
 ***************************************************************************/
/* ILS_MVSCALE_0 :: MVSCALE_DEBUG :: Debug [31:00] */
#define BCHP_ILS_MVSCALE_0_MVSCALE_DEBUG_Debug_MASK                0xffffffff
#define BCHP_ILS_MVSCALE_0_MVSCALE_DEBUG_Debug_SHIFT               0

/***************************************************************************
 *VECTOR_MEM_%i - MVSCALE_VECTOR_MEM_0..33
 ***************************************************************************/
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_ARRAY_BASE                 0x00127300
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_ARRAY_START                0
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_ARRAY_END                  33
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_ARRAY_ELEMENT_SIZE         32

/***************************************************************************
 *VECTOR_MEM_%i - MVSCALE_VECTOR_MEM_0..33
 ***************************************************************************/
/* ILS_MVSCALE_0 :: VECTOR_MEM_i :: mvscale_ram [31:00] */
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_mvscale_ram_MASK           0xffffffff
#define BCHP_ILS_MVSCALE_0_VECTOR_MEM_i_mvscale_ram_SHIFT          0


/***************************************************************************
 *END - MVSCALE_END
 ***************************************************************************/
/* ILS_MVSCALE_0 :: END :: reserved0 [31:00] */
#define BCHP_ILS_MVSCALE_0_END_reserved0_MASK                      0xffffffff
#define BCHP_ILS_MVSCALE_0_END_reserved0_SHIFT                     0

#endif /* #ifndef BCHP_ILS_MVSCALE_0_H__ */

/* End of File */
