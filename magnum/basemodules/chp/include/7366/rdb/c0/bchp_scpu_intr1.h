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
 * Date:           Generated on               Thu Feb 12 15:15:50 2015
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

#ifndef BCHP_SCPU_INTR1_H__
#define BCHP_SCPU_INTR1_H__

/***************************************************************************
 *SCPU_INTR1 - SCPU Level 1 Interrupt Controller Registers
 ***************************************************************************/
#define BCHP_SCPU_INTR1_INTR_W0_STATUS           0x00310480 /* [RO] Interrupt Status Register */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS      0x00310488 /* [RO] Interrupt Mask Status Register */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET         0x00310490 /* [WO] Interrupt Mask Set Register */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR       0x00310498 /* [WO] Interrupt Mask Clear Register */

/***************************************************************************
 *INTR_W0_STATUS - Interrupt Status Register
 ***************************************************************************/
/* SCPU_INTR1 :: INTR_W0_STATUS :: reserved0 [31:21] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_reserved0_MASK              0xffe00000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_reserved0_SHIFT             21

/* SCPU_INTR1 :: INTR_W0_STATUS :: SCPU_HW_INTR [20:20] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_HW_INTR_MASK           0x00100000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_HW_INTR_SHIFT          20
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_HW_INTR_DEFAULT        0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: SAGE_UART_IRQ [19:19] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SAGE_UART_IRQ_MASK          0x00080000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SAGE_UART_IRQ_SHIFT         19
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SAGE_UART_IRQ_DEFAULT       0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_MCPB_INTR [18:18] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MCPB_INTR_MASK          0x00040000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MCPB_INTR_SHIFT         18
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MCPB_INTR_DEFAULT       0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_WRDMA_INTR [17:17] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_WRDMA_INTR_MASK         0x00020000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_WRDMA_INTR_SHIFT        17
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_WRDMA_INTR_DEFAULT      0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPTSEC_INTR [16:16] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPTSEC_INTR_MASK            0x00010000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPTSEC_INTR_SHIFT           16
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPTSEC_INTR_DEFAULT         0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: MEMC_2_CPU_INTR [15:15] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_2_CPU_INTR_MASK        0x00008000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_2_CPU_INTR_SHIFT       15
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_2_CPU_INTR_DEFAULT     0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: BSP_IPI_CPU_INTR [14:14] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_BSP_IPI_CPU_INTR_MASK       0x00004000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_BSP_IPI_CPU_INTR_SHIFT      14
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_BSP_IPI_CPU_INTR_DEFAULT    0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: CPU_IPI_CPU_INTR [13:13] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_CPU_IPI_CPU_INTR_MASK       0x00002000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_CPU_IPI_CPU_INTR_SHIFT      13
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_CPU_IPI_CPU_INTR_DEFAULT    0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: SCPU_INTERNAL_CPU_INTR [12:12] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_INTERNAL_CPU_INTR_MASK 0x00001000
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_INTERNAL_CPU_INTR_SHIFT 12
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_INTERNAL_CPU_INTR_DEFAULT 0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: SCPU_TMR_CPU_INTR [11:11] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_TMR_CPU_INTR_MASK      0x00000800
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_TMR_CPU_INTR_SHIFT     11
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SCPU_TMR_CPU_INTR_DEFAULT   0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_RAVE_CPU_INTR [10:10] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_RAVE_CPU_INTR_MASK      0x00000400
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_RAVE_CPU_INTR_SHIFT     10
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_RAVE_CPU_INTR_DEFAULT   0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_MSG_CPU_INTR [09:09] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_CPU_INTR_MASK       0x00000200
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_CPU_INTR_SHIFT      9
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_CPU_INTR_DEFAULT    0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_MSG_STAT_CPU_INTR [08:08] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_STAT_CPU_INTR_MASK  0x00000100
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_STAT_CPU_INTR_SHIFT 8
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_MSG_STAT_CPU_INTR_DEFAULT 0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_OVFL_CPU_INTR [07:07] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_OVFL_CPU_INTR_MASK      0x00000080
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_OVFL_CPU_INTR_SHIFT     7
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_OVFL_CPU_INTR_DEFAULT   0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: XPT_STATUS_CPU_INTR [06:06] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_STATUS_CPU_INTR_MASK    0x00000040
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_STATUS_CPU_INTR_SHIFT   6
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_XPT_STATUS_CPU_INTR_DEFAULT 0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: SYS_CPU_INTR [05:05] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SYS_CPU_INTR_MASK           0x00000020
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SYS_CPU_INTR_SHIFT          5
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_SYS_CPU_INTR_DEFAULT        0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: MC_CPU_INTR [04:04] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MC_CPU_INTR_MASK            0x00000010
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MC_CPU_INTR_SHIFT           4
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MC_CPU_INTR_DEFAULT         0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: UPG_SC_CPU_INTR [03:03] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_UPG_SC_CPU_INTR_MASK        0x00000008
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_UPG_SC_CPU_INTR_SHIFT       3
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_UPG_SC_CPU_INTR_DEFAULT     0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: AVS_TOP [02:02] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_AVS_TOP_MASK                0x00000004
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_AVS_TOP_SHIFT               2
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_AVS_TOP_DEFAULT             0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: MEMC_1_CPU_INTR [01:01] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_1_CPU_INTR_MASK        0x00000002
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_1_CPU_INTR_SHIFT       1
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_1_CPU_INTR_DEFAULT     0x00000000

/* SCPU_INTR1 :: INTR_W0_STATUS :: MEMC_0_CPU_INTR [00:00] */
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_0_CPU_INTR_MASK        0x00000001
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_0_CPU_INTR_SHIFT       0
#define BCHP_SCPU_INTR1_INTR_W0_STATUS_MEMC_0_CPU_INTR_DEFAULT     0x00000000

/***************************************************************************
 *INTR_W0_MASK_STATUS - Interrupt Mask Status Register
 ***************************************************************************/
/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: reserved0 [31:21] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_reserved0_MASK         0xffe00000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_reserved0_SHIFT        21

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: SCPU_HW_INTR [20:20] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_HW_INTR_MASK      0x00100000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_HW_INTR_SHIFT     20
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_HW_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: reserved_for_padding1 [19:19] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_reserved_for_padding1_MASK 0x00080000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_reserved_for_padding1_SHIFT 19

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_MCPB_INTR [18:18] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MCPB_INTR_MASK     0x00040000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MCPB_INTR_SHIFT    18
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MCPB_INTR_DEFAULT  0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_WRDMA_INTR [17:17] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_WRDMA_INTR_MASK    0x00020000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_WRDMA_INTR_SHIFT   17
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_WRDMA_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPTSEC_INTR [16:16] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPTSEC_INTR_MASK       0x00010000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPTSEC_INTR_SHIFT      16
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPTSEC_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: MEMC_2_CPU_INTR [15:15] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_2_CPU_INTR_MASK   0x00008000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_2_CPU_INTR_SHIFT  15
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_2_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: BSP_IPI_CPU_INTR [14:14] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_BSP_IPI_CPU_INTR_MASK  0x00004000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_BSP_IPI_CPU_INTR_SHIFT 14
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_BSP_IPI_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: CPU_IPI_CPU_INTR [13:13] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_CPU_IPI_CPU_INTR_MASK  0x00002000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_CPU_IPI_CPU_INTR_SHIFT 13
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_CPU_IPI_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: SCPU_INTERNAL_CPU_INTR [12:12] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_INTERNAL_CPU_INTR_MASK 0x00001000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_INTERNAL_CPU_INTR_SHIFT 12
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_INTERNAL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: SCPU_TMR_CPU_INTR [11:11] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_TMR_CPU_INTR_MASK 0x00000800
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_TMR_CPU_INTR_SHIFT 11
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SCPU_TMR_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_RAVE_CPU_INTR [10:10] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_RAVE_CPU_INTR_MASK 0x00000400
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_RAVE_CPU_INTR_SHIFT 10
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_RAVE_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_MSG_CPU_INTR [09:09] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_CPU_INTR_MASK  0x00000200
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_CPU_INTR_SHIFT 9
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_MSG_STAT_CPU_INTR [08:08] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_STAT_CPU_INTR_MASK 0x00000100
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_STAT_CPU_INTR_SHIFT 8
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_MSG_STAT_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_OVFL_CPU_INTR [07:07] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_OVFL_CPU_INTR_MASK 0x00000080
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_OVFL_CPU_INTR_SHIFT 7
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_OVFL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: XPT_STATUS_CPU_INTR [06:06] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_STATUS_CPU_INTR_MASK 0x00000040
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_STATUS_CPU_INTR_SHIFT 6
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_XPT_STATUS_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: SYS_CPU_INTR [05:05] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SYS_CPU_INTR_MASK      0x00000020
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SYS_CPU_INTR_SHIFT     5
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_SYS_CPU_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: MC_CPU_INTR [04:04] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MC_CPU_INTR_MASK       0x00000010
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MC_CPU_INTR_SHIFT      4
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MC_CPU_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: UPG_SC_CPU_INTR [03:03] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_UPG_SC_CPU_INTR_MASK   0x00000008
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_UPG_SC_CPU_INTR_SHIFT  3
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_UPG_SC_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: AVS_TOP_INTR [02:02] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_AVS_TOP_INTR_MASK      0x00000004
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_AVS_TOP_INTR_SHIFT     2
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_AVS_TOP_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: MEMC_1_CPU_INTR [01:01] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_1_CPU_INTR_MASK   0x00000002
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_1_CPU_INTR_SHIFT  1
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_1_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_STATUS :: MEMC_0_CPU_INTR [00:00] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_0_CPU_INTR_MASK   0x00000001
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_0_CPU_INTR_SHIFT  0
#define BCHP_SCPU_INTR1_INTR_W0_MASK_STATUS_MEMC_0_CPU_INTR_DEFAULT 0x00000001

/***************************************************************************
 *INTR_W0_MASK_SET - Interrupt Mask Set Register
 ***************************************************************************/
/* SCPU_INTR1 :: INTR_W0_MASK_SET :: reserved0 [31:21] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_reserved0_MASK            0xffe00000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_reserved0_SHIFT           21

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: SCPU_HW_INTR [20:20] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_HW_INTR_MASK         0x00100000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_HW_INTR_SHIFT        20
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_HW_INTR_DEFAULT      0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: reserved_for_padding1 [19:19] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_reserved_for_padding1_MASK 0x00080000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_reserved_for_padding1_SHIFT 19

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_MCPB_INTR [18:18] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MCPB_INTR_MASK        0x00040000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MCPB_INTR_SHIFT       18
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MCPB_INTR_DEFAULT     0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_WRDMA_INTR [17:17] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_WRDMA_INTR_MASK       0x00020000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_WRDMA_INTR_SHIFT      17
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_WRDMA_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPTSEC_INTR [16:16] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPTSEC_INTR_MASK          0x00010000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPTSEC_INTR_SHIFT         16
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPTSEC_INTR_DEFAULT       0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: MEMC_2_CPU_INTR [15:15] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_2_CPU_INTR_MASK      0x00008000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_2_CPU_INTR_SHIFT     15
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_2_CPU_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: BSP_IPI_CPU_INTR [14:14] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_BSP_IPI_CPU_INTR_MASK     0x00004000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_BSP_IPI_CPU_INTR_SHIFT    14
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_BSP_IPI_CPU_INTR_DEFAULT  0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: CPU_IPI_CPU_INTR [13:13] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_CPU_IPI_CPU_INTR_MASK     0x00002000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_CPU_IPI_CPU_INTR_SHIFT    13
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_CPU_IPI_CPU_INTR_DEFAULT  0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: SCPU_INTERNAL_CPU_INTR [12:12] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_INTERNAL_CPU_INTR_MASK 0x00001000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_INTERNAL_CPU_INTR_SHIFT 12
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_INTERNAL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: SCPU_TMR_CPU_INTR [11:11] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_TMR_CPU_INTR_MASK    0x00000800
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_TMR_CPU_INTR_SHIFT   11
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SCPU_TMR_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_RAVE_CPU_INTR [10:10] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_RAVE_CPU_INTR_MASK    0x00000400
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_RAVE_CPU_INTR_SHIFT   10
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_RAVE_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_MSG_CPU_INTR [09:09] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_CPU_INTR_MASK     0x00000200
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_CPU_INTR_SHIFT    9
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_CPU_INTR_DEFAULT  0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_MSG_STAT_CPU_INTR [08:08] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_STAT_CPU_INTR_MASK 0x00000100
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_STAT_CPU_INTR_SHIFT 8
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_MSG_STAT_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_OVFL_CPU_INTR [07:07] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_OVFL_CPU_INTR_MASK    0x00000080
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_OVFL_CPU_INTR_SHIFT   7
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_OVFL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: XPT_STATUS_CPU_INTR [06:06] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_STATUS_CPU_INTR_MASK  0x00000040
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_STATUS_CPU_INTR_SHIFT 6
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_XPT_STATUS_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: SYS_CPU_INTR [05:05] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SYS_CPU_INTR_MASK         0x00000020
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SYS_CPU_INTR_SHIFT        5
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_SYS_CPU_INTR_DEFAULT      0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: MC_CPU_INTR [04:04] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MC_CPU_INTR_MASK          0x00000010
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MC_CPU_INTR_SHIFT         4
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MC_CPU_INTR_DEFAULT       0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: UPG_SC_CPU_INTR [03:03] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_UPG_SC_CPU_INTR_MASK      0x00000008
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_UPG_SC_CPU_INTR_SHIFT     3
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_UPG_SC_CPU_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: AVS_TOP_INTR [02:02] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_AVS_TOP_INTR_MASK         0x00000004
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_AVS_TOP_INTR_SHIFT        2
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_AVS_TOP_INTR_DEFAULT      0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: MEMC_1_CPU_INTR [01:01] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_1_CPU_INTR_MASK      0x00000002
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_1_CPU_INTR_SHIFT     1
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_1_CPU_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_SET :: MEMC_0_CPU_INTR [00:00] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_0_CPU_INTR_MASK      0x00000001
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_0_CPU_INTR_SHIFT     0
#define BCHP_SCPU_INTR1_INTR_W0_MASK_SET_MEMC_0_CPU_INTR_DEFAULT   0x00000001

/***************************************************************************
 *INTR_W0_MASK_CLEAR - Interrupt Mask Clear Register
 ***************************************************************************/
/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: reserved0 [31:21] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_reserved0_MASK          0xffe00000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_reserved0_SHIFT         21

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: SCPU_HW_INTR [20:20] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_HW_INTR_MASK       0x00100000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_HW_INTR_SHIFT      20
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_HW_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: reserved_for_padding1 [19:19] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_reserved_for_padding1_MASK 0x00080000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_reserved_for_padding1_SHIFT 19

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_MCPB_INTR [18:18] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MCPB_INTR_MASK      0x00040000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MCPB_INTR_SHIFT     18
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MCPB_INTR_DEFAULT   0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_WRDMA_INTR [17:17] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_WRDMA_INTR_MASK     0x00020000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_WRDMA_INTR_SHIFT    17
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_WRDMA_INTR_DEFAULT  0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPTSEC_INTR [16:16] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPTSEC_INTR_MASK        0x00010000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPTSEC_INTR_SHIFT       16
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPTSEC_INTR_DEFAULT     0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: MEMC_2_CPU_INTR [15:15] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_2_CPU_INTR_MASK    0x00008000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_2_CPU_INTR_SHIFT   15
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_2_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: BSP_IPI_CPU_INTR [14:14] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_BSP_IPI_CPU_INTR_MASK   0x00004000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_BSP_IPI_CPU_INTR_SHIFT  14
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_BSP_IPI_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: CPU_IPI_CPU_INTR [13:13] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_CPU_IPI_CPU_INTR_MASK   0x00002000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_CPU_IPI_CPU_INTR_SHIFT  13
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_CPU_IPI_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: SCPU_INTERNAL_CPU_INTR [12:12] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_INTERNAL_CPU_INTR_MASK 0x00001000
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_INTERNAL_CPU_INTR_SHIFT 12
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_INTERNAL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: SCPU_TMR_CPU_INTR [11:11] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_TMR_CPU_INTR_MASK  0x00000800
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_TMR_CPU_INTR_SHIFT 11
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SCPU_TMR_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_RAVE_CPU_INTR [10:10] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_RAVE_CPU_INTR_MASK  0x00000400
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_RAVE_CPU_INTR_SHIFT 10
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_RAVE_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_MSG_CPU_INTR [09:09] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_CPU_INTR_MASK   0x00000200
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_CPU_INTR_SHIFT  9
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_MSG_STAT_CPU_INTR [08:08] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_STAT_CPU_INTR_MASK 0x00000100
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_STAT_CPU_INTR_SHIFT 8
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_MSG_STAT_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_OVFL_CPU_INTR [07:07] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_OVFL_CPU_INTR_MASK  0x00000080
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_OVFL_CPU_INTR_SHIFT 7
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_OVFL_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: XPT_STATUS_CPU_INTR [06:06] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_STATUS_CPU_INTR_MASK 0x00000040
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_STATUS_CPU_INTR_SHIFT 6
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_XPT_STATUS_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: SYS_CPU_INTR [05:05] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SYS_CPU_INTR_MASK       0x00000020
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SYS_CPU_INTR_SHIFT      5
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_SYS_CPU_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: MC_CPU_INTR [04:04] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MC_CPU_INTR_MASK        0x00000010
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MC_CPU_INTR_SHIFT       4
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MC_CPU_INTR_DEFAULT     0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: UPG_SC_CPU_INTR [03:03] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_UPG_SC_CPU_INTR_MASK    0x00000008
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_UPG_SC_CPU_INTR_SHIFT   3
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_UPG_SC_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: AVS_TOP_INTR [02:02] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_AVS_TOP_INTR_MASK       0x00000004
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_AVS_TOP_INTR_SHIFT      2
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_AVS_TOP_INTR_DEFAULT    0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: MEMC_1_CPU_INTR [01:01] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_1_CPU_INTR_MASK    0x00000002
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_1_CPU_INTR_SHIFT   1
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_1_CPU_INTR_DEFAULT 0x00000001

/* SCPU_INTR1 :: INTR_W0_MASK_CLEAR :: MEMC_0_CPU_INTR [00:00] */
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_0_CPU_INTR_MASK    0x00000001
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_0_CPU_INTR_SHIFT   0
#define BCHP_SCPU_INTR1_INTR_W0_MASK_CLEAR_MEMC_0_CPU_INTR_DEFAULT 0x00000001

#endif /* #ifndef BCHP_SCPU_INTR1_H__ */

/* End of File */
