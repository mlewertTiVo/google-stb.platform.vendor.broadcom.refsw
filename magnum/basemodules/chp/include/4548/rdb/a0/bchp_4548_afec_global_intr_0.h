/***************************************************************************
 *     Copyright (c) 1999-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 * AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 * EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 *
 * Date:           Generated on         Wed Mar 27 10:49:17 2013
 *                 MD5 Checksum         d41d8cd98f00b204e9800998ecf8427e
 *
 * Compiled with:  RDB Utility          combo_header.pl
 *                 RDB Parser           3.0
 *                 unknown              unknown
 *                 Perl Interpreter     5.008008
 *                 Operating System     linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_AFEC_GLOBAL_INTR_0_H__
#define BCHP_AFEC_GLOBAL_INTR_0_H__

/***************************************************************************
 *AFEC_GLOBAL_INTR_0 - 0 AFEC Global L2 Interrupt Control Registers
 ***************************************************************************/
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS       0x01253800 /* CPU interrupt Status Register */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET          0x01253804 /* CPU interrupt Set Register */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR        0x01253808 /* CPU interrupt Clear Register */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS  0x0125380c /* CPU interrupt Mask Status Register */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET     0x01253810 /* CPU interrupt Mask Set Register */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR   0x01253814 /* CPU interrupt Mask Clear Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS       0x01253818 /* PCI interrupt Status Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET          0x0125381c /* PCI interrupt Set Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR        0x01253820 /* PCI interrupt Clear Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS  0x01253824 /* PCI interrupt Mask Status Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET     0x01253828 /* PCI interrupt Mask Set Register */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR   0x0125382c /* PCI interrupt Mask Clear Register */

/***************************************************************************
 *CPU_STATUS - CPU interrupt Status Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_STATUS :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_STATUS :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_reserved0_MASK          0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_reserved0_SHIFT         17

/* AFEC_GLOBAL_INTR_0 :: CPU_STATUS :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_STATUS :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_STATUS :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_reserved1_MASK          0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_STATUS_reserved1_SHIFT         0

/***************************************************************************
 *CPU_SET - CPU interrupt Set Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_SET :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_SET :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_reserved0_MASK             0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_reserved0_SHIFT            17

/* AFEC_GLOBAL_INTR_0 :: CPU_SET :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_SET :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_OVERFLOW_INTR_MASK    0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_OVERFLOW_INTR_SHIFT   15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_SET :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_reserved1_MASK             0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_SET_reserved1_SHIFT            0

/***************************************************************************
 *CPU_CLEAR - CPU interrupt Clear Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_CLEAR :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_CLEAR :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_reserved0_MASK           0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_reserved0_SHIFT          17

/* AFEC_GLOBAL_INTR_0 :: CPU_CLEAR :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_CLEAR :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_OVERFLOW_INTR_MASK  0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: CPU_CLEAR :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_reserved1_MASK           0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_CLEAR_reserved1_SHIFT          0

/***************************************************************************
 *CPU_MASK_STATUS - CPU interrupt Mask Status Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_STATUS :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_STATUS :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_reserved0_MASK     0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_reserved0_SHIFT    17

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_STATUS :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_STATUS :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_STATUS :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_reserved1_MASK     0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_STATUS_reserved1_SHIFT    0

/***************************************************************************
 *CPU_MASK_SET - CPU interrupt Mask Set Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_SET :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_SET :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_reserved0_MASK        0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_reserved0_SHIFT       17

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_SET :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_SET :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_SET :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_reserved1_MASK        0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_SET_reserved1_SHIFT       0

/***************************************************************************
 *CPU_MASK_CLEAR - CPU interrupt Mask Clear Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_CLEAR :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_CLEAR :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_reserved0_MASK      0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_reserved0_SHIFT     17

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_CLEAR :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_CLEAR :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: CPU_MASK_CLEAR :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_reserved1_MASK      0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_CPU_MASK_CLEAR_reserved1_SHIFT     0

/***************************************************************************
 *PCI_STATUS - PCI interrupt Status Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_STATUS :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_STATUS :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_reserved0_MASK          0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_reserved0_SHIFT         17

/* AFEC_GLOBAL_INTR_0 :: PCI_STATUS :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_STATUS :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_STATUS :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_reserved1_MASK          0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_STATUS_reserved1_SHIFT         0

/***************************************************************************
 *PCI_SET - PCI interrupt Set Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_SET :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_SET :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_reserved0_MASK             0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_reserved0_SHIFT            17

/* AFEC_GLOBAL_INTR_0 :: PCI_SET :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_SET :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_OVERFLOW_INTR_MASK    0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_OVERFLOW_INTR_SHIFT   15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_SET :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_reserved1_MASK             0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_SET_reserved1_SHIFT            0

/***************************************************************************
 *PCI_CLEAR - PCI interrupt Clear Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_CLEAR :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_CLEAR :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_reserved0_MASK           0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_reserved0_SHIFT          17

/* AFEC_GLOBAL_INTR_0 :: PCI_CLEAR :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_CLEAR :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_OVERFLOW_INTR_MASK  0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_QBUF_OVERFLOW_INTR_DEFAULT 0x00000000

/* AFEC_GLOBAL_INTR_0 :: PCI_CLEAR :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_reserved1_MASK           0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_CLEAR_reserved1_SHIFT          0

/***************************************************************************
 *PCI_MASK_STATUS - PCI interrupt Mask Status Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_STATUS :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_STATUS :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_reserved0_MASK     0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_reserved0_SHIFT    17

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_STATUS :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_STATUS :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_STATUS :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_reserved1_MASK     0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_STATUS_reserved1_SHIFT    0

/***************************************************************************
 *PCI_MASK_SET - PCI interrupt Mask Set Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_SET :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_SET :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_reserved0_MASK        0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_reserved0_SHIFT       17

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_SET :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_SET :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_SET :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_reserved1_MASK        0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_SET_reserved1_SHIFT       0

/***************************************************************************
 *PCI_MASK_CLEAR - PCI interrupt Mask Clear Register
 ***************************************************************************/
/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_CLEAR :: BCH_DEC_INFIFO_OVERFLOW_INTR [31:31] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_MASK 0x80000000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_SHIFT 31
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_BCH_DEC_INFIFO_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_CLEAR :: reserved0 [30:17] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_reserved0_MASK      0x7ffe0000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_reserved0_SHIFT     17

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_CLEAR :: QBUF_ALMOST_OVERFLOW_INTR [16:16] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_MASK 0x00010000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_SHIFT 16
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_ALMOST_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_CLEAR :: QBUF_OVERFLOW_INTR [15:15] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_OVERFLOW_INTR_MASK 0x00008000
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_OVERFLOW_INTR_SHIFT 15
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_QBUF_OVERFLOW_INTR_DEFAULT 0x00000001

/* AFEC_GLOBAL_INTR_0 :: PCI_MASK_CLEAR :: reserved1 [14:00] */
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_reserved1_MASK      0x00007fff
#define BCHP_AFEC_GLOBAL_INTR_0_PCI_MASK_CLEAR_reserved1_SHIFT     0

#endif /* #ifndef BCHP_AFEC_GLOBAL_INTR_0_H__ */

/* End of File */
