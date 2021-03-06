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

#ifndef BCHP_REG_CABAC2BINS2_0_H__
#define BCHP_REG_CABAC2BINS2_0_H__

/***************************************************************************
 *REG_CABAC2BINS2_0
 ***************************************************************************/
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_ADDR 0x00112588 /* [RO] REG_CABAC2BINS_RD_BUFF_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL 0x0011258c /* [RW] REG_CABAC2BINS_RD_BUFF_CTL */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_START_ADDR 0x00112594 /* [RW] REG_CABAC2BINS_RD_BUFF_START_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_END_ADDR 0x00112598 /* [RW] REG_CABAC2BINS_RD_BUFF_END_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_STALL_CNT 0x0011259c /* [RO] REG_CABAC2BINS_RD_BUFF_STALL_CNT */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_START_ADDR 0x001125a8 /* [RW] REG_CABAC2BINS_WR_BUFF_START_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL 0x001125ac /* [RW] REG_CABAC2BINS_WR_BUFF_CTL */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_END_ADDR 0x001125b0 /* [RW] REG_CABAC2BINS_WR_BUFF_END_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_MARK_ADDR 0x001125b4 /* [RW] REG_CABAC2BINS_WR_BUFF_MARK_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_STALL_CNT 0x001125b8 /* [RO] REG_CABAC2BINS_WR_BUFF_STALL_CNT */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_ADDR 0x001125bc /* [RO] REG_CABAC2BINS_WR_BUFF_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_ID 0x001125c0 /* [RO] REG_CABAC2BINS_CHANNEL_ID */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_NOT_IDLE_CYCLES 0x00112620 /* [RW] REG_CABAC2BINS_NOT_IDLE_CYCLES */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL 0x00112624 /* [RW] REG_CABAC2BINS_PERF_MON_CTL */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG 0x00112628 /* [RO] REG_CABAC2BINS_BOOL_DBG */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE0 0x00112630 /* [RO] REG_CABAC2BINS_STATE0 */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE1 0x00112634 /* [RO] REG_CABAC2BINS_STATE1 */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG 0x00112638 /* [RO] REG_CABAC2BINS_VP8_DBG */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG 0x0011263c /* [RO] REG_CABAC2BINS_VP6_DBG */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL 0x00112640 /* [RW] REG_CABAC2BINS_INIT_TBL_CTL */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_COMMAND_ADDR 0x00112710 /* [RW] REG_CABAC2BINS_PICTURE_COMMAND_ADDR */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN 0x00112714 /* [RW] REG_CABAC2BINS_LITTLE_ENDIAN */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS 0x00112718 /* [RO] REG_CABAC2BINS_PICTURE_STATUS */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE 0x0011271c /* [RW] REG_CABAC2BINS_AVS_MODE */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL 0x0011272c /* [RW] REG_CABAC2BINS_CTL */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_END_END_1 0x001127fc /* [RW] REG_CABAC2BINS_END_END_1 */

/***************************************************************************
 *REG_CABAC2BINS_CHANNEL_WR_POSITION_%i - REG_CABAC2BINS_CHANNEL_WR_POSITION_0..31
 ***************************************************************************/
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_ARRAY_BASE 0x00112400
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_ARRAY_START 0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_ARRAY_END 31
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_ARRAY_ELEMENT_SIZE 32

/***************************************************************************
 *REG_CABAC2BINS_CHANNEL_WR_POSITION_%i - REG_CABAC2BINS_CHANNEL_WR_POSITION_0..31
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CHANNEL_WR_POSITION_i :: Addr [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_Addr_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_WR_POSITION_i_Addr_SHIFT 0


/***************************************************************************
 *REG_CABAC2BINS_RD_BUFF_ADDR - REG_CABAC2BINS_RD_BUFF_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_ADDR :: Addr [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_ADDR_Addr_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_ADDR_Addr_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_RD_BUFF_CTL - REG_CABAC2BINS_RD_BUFF_CTL
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: reserved0 [31:05] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_reserved0_MASK 0xffffffe0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_reserved0_SHIFT 5

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: At_mark [04:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_At_mark_MASK 0x00000010
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_At_mark_SHIFT 4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: Not_Rdy [03:03] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Not_Rdy_MASK 0x00000008
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Not_Rdy_SHIFT 3

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: Wrap_En [02:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Wrap_En_MASK 0x00000004
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Wrap_En_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: Init [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Init_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Init_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_CTL :: Buff_En [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Buff_En_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_CTL_Buff_En_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_RD_BUFF_START_ADDR - REG_CABAC2BINS_RD_BUFF_START_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_START_ADDR :: Addr [31:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_START_ADDR_Addr_MASK 0xffffff80
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_START_ADDR_Addr_SHIFT 7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_START_ADDR :: reserved0 [06:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_START_ADDR_reserved0_MASK 0x0000007f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_START_ADDR_reserved0_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_RD_BUFF_END_ADDR - REG_CABAC2BINS_RD_BUFF_END_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_END_ADDR :: Addr [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_END_ADDR_Addr_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_END_ADDR_Addr_SHIFT 0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_END_ADDR_Addr_DEFAULT 0x0000001f

/***************************************************************************
 *REG_CABAC2BINS_RD_BUFF_STALL_CNT - REG_CABAC2BINS_RD_BUFF_STALL_CNT
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_RD_BUFF_STALL_CNT :: Count [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_STALL_CNT_Count_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_RD_BUFF_STALL_CNT_Count_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_START_ADDR - REG_CABAC2BINS_WR_BUFF_START_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_START_ADDR :: Addr [31:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_START_ADDR_Addr_MASK 0xffffff80
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_START_ADDR_Addr_SHIFT 7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_START_ADDR :: reserved0 [06:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_START_ADDR_reserved0_MASK 0x0000007f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_START_ADDR_reserved0_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_CTL - REG_CABAC2BINS_WR_BUFF_CTL
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: reserved0 [31:05] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_reserved0_MASK 0xffffffe0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_reserved0_SHIFT 5

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: At_mark [04:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_At_mark_MASK 0x00000010
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_At_mark_SHIFT 4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: Not_Rdy [03:03] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Not_Rdy_MASK 0x00000008
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Not_Rdy_SHIFT 3

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: Buff_close [02:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Buff_close_MASK 0x00000004
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Buff_close_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: Init [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Init_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Init_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_CTL :: Buff_En [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Buff_En_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_CTL_Buff_En_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_END_ADDR - REG_CABAC2BINS_WR_BUFF_END_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_END_ADDR :: Addr [31:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_END_ADDR_Addr_MASK 0xffffff80
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_END_ADDR_Addr_SHIFT 7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_END_ADDR :: reserved0 [06:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_END_ADDR_reserved0_MASK 0x0000007f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_END_ADDR_reserved0_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_MARK_ADDR - REG_CABAC2BINS_WR_BUFF_MARK_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_MARK_ADDR :: Addr [31:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_MARK_ADDR_Addr_MASK 0xffffff80
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_MARK_ADDR_Addr_SHIFT 7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_MARK_ADDR :: reserved0 [06:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_MARK_ADDR_reserved0_MASK 0x0000007f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_MARK_ADDR_reserved0_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_STALL_CNT - REG_CABAC2BINS_WR_BUFF_STALL_CNT
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_STALL_CNT :: Count [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_STALL_CNT_Count_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_STALL_CNT_Count_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_WR_BUFF_ADDR - REG_CABAC2BINS_WR_BUFF_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_WR_BUFF_ADDR :: Addr [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_ADDR_Addr_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_WR_BUFF_ADDR_Addr_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_CHANNEL_ID - REG_CABAC2BINS_CHANNEL_ID
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CHANNEL_ID :: reserved0 [31:05] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_ID_reserved0_MASK 0xffffffe0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_ID_reserved0_SHIFT 5

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CHANNEL_ID :: ID [04:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_ID_ID_MASK   0x0000001f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CHANNEL_ID_ID_SHIFT  0

/***************************************************************************
 *REG_CABAC2BINS_NOT_IDLE_CYCLES - REG_CABAC2BINS_NOT_IDLE_CYCLES
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_NOT_IDLE_CYCLES :: Cycle_Count [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_NOT_IDLE_CYCLES_Cycle_Count_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_NOT_IDLE_CYCLES_Cycle_Count_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_PERF_MON_CTL - REG_CABAC2BINS_PERF_MON_CTL
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PERF_MON_CTL :: reserved0 [31:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_reserved0_MASK 0xfffffff0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_reserved0_SHIFT 4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PERF_MON_CTL :: wait_count [03:03] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_wait_count_MASK 0x00000008
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_wait_count_SHIFT 3

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PERF_MON_CTL :: bit_count [02:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_bit_count_MASK 0x00000004
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_bit_count_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PERF_MON_CTL :: bin_count [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_bin_count_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_bin_count_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PERF_MON_CTL :: no_cycle_count [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_no_cycle_count_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PERF_MON_CTL_no_cycle_count_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_BOOL_DBG - REG_CABAC2BINS_BOOL_DBG
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_BOOL_DBG :: reserved0 [31:25] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_reserved0_MASK 0xfe000000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_reserved0_SHIFT 25

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_BOOL_DBG :: last_bool [24:24] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_last_bool_MASK 0x01000000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_last_bool_SHIFT 24

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_BOOL_DBG :: prob [23:16] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_prob_MASK   0x00ff0000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_prob_SHIFT  16

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_BOOL_DBG :: range [15:08] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_range_MASK  0x0000ff00
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_range_SHIFT 8

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_BOOL_DBG :: value [07:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_value_MASK  0x000000ff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_BOOL_DBG_value_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_STATE0 - REG_CABAC2BINS_STATE0
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_STATE0 :: reserved0 [31:10] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE0_reserved0_MASK 0xfffffc00
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE0_reserved0_SHIFT 10

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_STATE0 :: State [09:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE0_State_MASK    0x000003ff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE0_State_SHIFT   0

/***************************************************************************
 *REG_CABAC2BINS_STATE1 - REG_CABAC2BINS_STATE1
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_STATE1 :: reserved0 [31:10] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE1_reserved0_MASK 0xfffffc00
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE1_reserved0_SHIFT 10

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_STATE1 :: State [09:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE1_State_MASK    0x000003ff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_STATE1_State_SHIFT   0

/***************************************************************************
 *REG_CABAC2BINS_VP8_DBG - REG_CABAC2BINS_VP8_DBG
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: reserved0 [31:28] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_reserved0_MASK 0xf0000000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_reserved0_SHIFT 28

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: huff_state [27:25] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_huff_state_MASK 0x0e000000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_huff_state_SHIFT 25

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: pic_state [24:23] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_pic_state_MASK 0x01800000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_pic_state_SHIFT 23

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: part_state [22:19] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_part_state_MASK 0x00780000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_part_state_SHIFT 19

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: mb_state [18:14] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_mb_state_MASK 0x0007c000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_mb_state_SHIFT 14

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: blk_state [13:11] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_blk_state_MASK 0x00003800
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_blk_state_SHIFT 11

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: coef_state [10:08] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_coef_state_MASK 0x00000700
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_coef_state_SHIFT 8

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: blk_idx [07:06] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_blk_idx_MASK 0x000000c0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_blk_idx_SHIFT 6

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP8_DBG :: coef_idx [05:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_coef_idx_MASK 0x0000003f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP8_DBG_coef_idx_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_VP6_DBG - REG_CABAC2BINS_VP6_DBG
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP6_DBG :: reserved0 [31:06] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_reserved0_MASK 0xffffffc0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_reserved0_SHIFT 6

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP6_DBG :: avail_type [05:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_avail_type_MASK 0x00000030
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_avail_type_SHIFT 4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_VP6_DBG :: mv_state [03:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_mv_state_MASK 0x0000000f
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_VP6_DBG_mv_state_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_INIT_TBL_CTL - REG_CABAC2BINS_INIT_TBL_CTL
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_INIT_TBL_CTL :: reserved0 [31:13] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_reserved0_MASK 0xffffe000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_reserved0_SHIFT 13

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_INIT_TBL_CTL :: Enable [12:12] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_Enable_MASK 0x00001000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_Enable_SHIFT 12

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_INIT_TBL_CTL :: reserved1 [11:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_reserved1_MASK 0x00000fff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_INIT_TBL_CTL_reserved1_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_PICTURE_COMMAND_ADDR - REG_CABAC2BINS_PICTURE_COMMAND_ADDR
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_COMMAND_ADDR :: Addr [31:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_COMMAND_ADDR_Addr_MASK 0xfffffffc
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_COMMAND_ADDR_Addr_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_COMMAND_ADDR :: reserved0 [01:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_COMMAND_ADDR_reserved0_MASK 0x00000003
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_COMMAND_ADDR_reserved0_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_LITTLE_ENDIAN - REG_CABAC2BINS_LITTLE_ENDIAN
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_LITTLE_ENDIAN :: reserved0 [31:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_reserved0_MASK 0xfffffffc
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_reserved0_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_LITTLE_ENDIAN :: Highmem_mode [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_Highmem_mode_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_Highmem_mode_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_LITTLE_ENDIAN :: Little_Endian [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_Little_Endian_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_LITTLE_ENDIAN_Little_Endian_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_PICTURE_STATUS - REG_CABAC2BINS_PICTURE_STATUS
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: reserved0 [31:16] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_reserved0_MASK 0xffff0000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_reserved0_SHIFT 16

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: Picture_Cmd_Done [15:12] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Done_MASK 0x0000f000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Done_SHIFT 12

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: Picture_Cmd_Space [11:08] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Space_MASK 0x00000f00
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Space_SHIFT 8

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: reserved1 [07:06] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_reserved1_MASK 0x000000c0
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_reserved1_SHIFT 6

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: Picture_Cmd_Count [05:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Count_MASK 0x0000003c
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Picture_Cmd_Count_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: Full [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Full_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Full_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_PICTURE_STATUS :: Busy [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Busy_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_PICTURE_STATUS_Busy_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_AVS_MODE - REG_CABAC2BINS_AVS_MODE
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: reserved0 [31:10] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_reserved0_MASK 0xfffffc00
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_reserved0_SHIFT 10

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: mb_skip_run_dis [09:09] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_skip_run_dis_MASK 0x00000200
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_skip_run_dis_SHIFT 9

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: mb_type_decoding_dis [08:08] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_type_decoding_dis_MASK 0x00000100
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_type_decoding_dis_SHIFT 8

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: mb_reference_index_dis [07:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_reference_index_dis_MASK 0x00000080
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_reference_index_dis_SHIFT 7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: mv_diff_dis [06:06] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mv_diff_dis_MASK 0x00000040
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mv_diff_dis_SHIFT 6

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: mb_qp_delta_dis [05:05] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_qp_delta_dis_MASK 0x00000020
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_mb_qp_delta_dis_SHIFT 5

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: trans_coefficient_dis [04:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_trans_coefficient_dis_MASK 0x00000010
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_trans_coefficient_dis_SHIFT 4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: input_end_dis [03:03] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_input_end_dis_MASK 0x00000008
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_input_end_dis_SHIFT 3

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: skip_run_dis [02:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_skip_run_dis_MASK 0x00000004
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_skip_run_dis_SHIFT 2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: missing_eos_dis [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_missing_eos_dis_MASK 0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_missing_eos_dis_SHIFT 1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_AVS_MODE :: early_eos_dis [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_early_eos_dis_MASK 0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_AVS_MODE_early_eos_dis_SHIFT 0

/***************************************************************************
 *REG_CABAC2BINS_CTL - REG_CABAC2BINS_CTL
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: reserved0 [31:12] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_reserved0_MASK   0xfffff000
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_reserved0_SHIFT  12

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: Int [11:11] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Int_MASK         0x00000800
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Int_SHIFT        11

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: Active [10:10] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Active_MASK      0x00000400
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Active_SHIFT     10

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: WrNR [09:09] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_WrNR_MASK        0x00000200
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_WrNR_SHIFT       9

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: RdNR [08:08] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_RdNR_MASK        0x00000100
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_RdNR_SHIFT       8

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: WrMk [07:07] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_WrMk_MASK        0x00000080
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_WrMk_SHIFT       7

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: RdMk [06:06] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_RdMk_MASK        0x00000040
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_RdMk_SHIFT       6

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: reserved1 [05:05] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_reserved1_MASK   0x00000020
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_reserved1_SHIFT  5

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: SdQ [04:04] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdQ_MASK         0x00000010
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdQ_SHIFT        4

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: SdWr [03:03] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdWr_MASK        0x00000008
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdWr_SHIFT       3

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: SdAct [02:02] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdAct_MASK       0x00000004
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdAct_SHIFT      2

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: SdReq [01:01] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdReq_MASK       0x00000002
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_SdReq_SHIFT      1

/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_CTL :: Reset [00:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Reset_MASK       0x00000001
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_CTL_Reset_SHIFT      0

/***************************************************************************
 *REG_CABAC2BINS_END_END_1 - REG_CABAC2BINS_END_END_1
 ***************************************************************************/
/* REG_CABAC2BINS2_0 :: REG_CABAC2BINS_END_END_1 :: reserved0 [31:00] */
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_END_END_1_reserved0_MASK 0xffffffff
#define BCHP_REG_CABAC2BINS2_0_REG_CABAC2BINS_END_END_1_reserved0_SHIFT 0

#endif /* #ifndef BCHP_REG_CABAC2BINS2_0_H__ */

/* End of File */
