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
 * Date:           Generated on               Wed Apr 15 16:00:30 2015
 *                 Full Compile MD5 Checksum  798b3ac22e50cf765b00b72a7779366d
 *                     (minus title and desc)
 *                 MD5 Checksum               ccf80b3ba114a13bf874c64a54245c9a
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     16006
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
 ***************************************************************************/

#ifndef BCHP_LEAP_HOST_L2_H__
#define BCHP_LEAP_HOST_L2_H__

/***************************************************************************
 *LEAP_HOST_L2 - Host L2 Interrupt Controller Registers
 ***************************************************************************/
#define BCHP_LEAP_HOST_L2_STATUS0                0x00100500 /* [RO] Host Interrupt Status Register */
#define BCHP_LEAP_HOST_L2_SET0                   0x00100504 /* [WO] Host Interrupt Set Register */
#define BCHP_LEAP_HOST_L2_CLEAR0                 0x00100508 /* [WO] Host Interrupt Clear Register */
#define BCHP_LEAP_HOST_L2_MASK_STATUS0           0x0010050c /* [RO] Host Interrupt Mask Status Register */
#define BCHP_LEAP_HOST_L2_MASK_SET0              0x00100510 /* [WO] Host Interrupt Mask Set Register */
#define BCHP_LEAP_HOST_L2_MASK_CLEAR0            0x00100514 /* [WO] Host Interrupt Mask Clear Register */
#define BCHP_LEAP_HOST_L2_STATUS1                0x00100518 /* [RO] Host Interrupt Status Register */
#define BCHP_LEAP_HOST_L2_SET1                   0x0010051c /* [WO] Host Interrupt Set Register */
#define BCHP_LEAP_HOST_L2_CLEAR1                 0x00100520 /* [WO] Host Interrupt Clear Register */
#define BCHP_LEAP_HOST_L2_MASK_STATUS1           0x00100524 /* [RO] Host Interrupt Mask Status Register */
#define BCHP_LEAP_HOST_L2_MASK_SET1              0x00100528 /* [WO] Host Interrupt Mask Set Register */
#define BCHP_LEAP_HOST_L2_MASK_CLEAR1            0x0010052c /* [WO] Host Interrupt Mask Clear Register */

/***************************************************************************
 *STATUS0 - Host Interrupt Status Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: STATUS0 :: IRQ_STATUS [31:00] */
#define BCHP_LEAP_HOST_L2_STATUS0_IRQ_STATUS_MASK                  0xffffffff
#define BCHP_LEAP_HOST_L2_STATUS0_IRQ_STATUS_SHIFT                 0
#define BCHP_LEAP_HOST_L2_STATUS0_IRQ_STATUS_DEFAULT               0x00000000

/***************************************************************************
 *SET0 - Host Interrupt Set Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: SET0 :: IRQ_SET [31:00] */
#define BCHP_LEAP_HOST_L2_SET0_IRQ_SET_MASK                        0xffffffff
#define BCHP_LEAP_HOST_L2_SET0_IRQ_SET_SHIFT                       0

/***************************************************************************
 *CLEAR0 - Host Interrupt Clear Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: CLEAR0 :: IRQ_CLEAR [31:00] */
#define BCHP_LEAP_HOST_L2_CLEAR0_IRQ_CLEAR_MASK                    0xffffffff
#define BCHP_LEAP_HOST_L2_CLEAR0_IRQ_CLEAR_SHIFT                   0

/***************************************************************************
 *MASK_STATUS0 - Host Interrupt Mask Status Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_STATUS0 :: MASK_STATUS [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_STATUS0_MASK_STATUS_MASK            0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_STATUS0_MASK_STATUS_SHIFT           0
#define BCHP_LEAP_HOST_L2_MASK_STATUS0_MASK_STATUS_DEFAULT         0xffffffff

/***************************************************************************
 *MASK_SET0 - Host Interrupt Mask Set Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_SET0 :: MASK_SET [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_SET0_MASK_SET_MASK                  0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_SET0_MASK_SET_SHIFT                 0

/***************************************************************************
 *MASK_CLEAR0 - Host Interrupt Mask Clear Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_CLEAR0 :: MASK_CLEAR [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_CLEAR0_MASK_CLEAR_MASK              0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_CLEAR0_MASK_CLEAR_SHIFT             0

/***************************************************************************
 *STATUS1 - Host Interrupt Status Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: STATUS1 :: IRQ_STATUS [31:00] */
#define BCHP_LEAP_HOST_L2_STATUS1_IRQ_STATUS_MASK                  0xffffffff
#define BCHP_LEAP_HOST_L2_STATUS1_IRQ_STATUS_SHIFT                 0
#define BCHP_LEAP_HOST_L2_STATUS1_IRQ_STATUS_DEFAULT               0x00000000

/***************************************************************************
 *SET1 - Host Interrupt Set Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: SET1 :: IRQ_SET [31:00] */
#define BCHP_LEAP_HOST_L2_SET1_IRQ_SET_MASK                        0xffffffff
#define BCHP_LEAP_HOST_L2_SET1_IRQ_SET_SHIFT                       0

/***************************************************************************
 *CLEAR1 - Host Interrupt Clear Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: CLEAR1 :: IRQ_CLEAR [31:00] */
#define BCHP_LEAP_HOST_L2_CLEAR1_IRQ_CLEAR_MASK                    0xffffffff
#define BCHP_LEAP_HOST_L2_CLEAR1_IRQ_CLEAR_SHIFT                   0

/***************************************************************************
 *MASK_STATUS1 - Host Interrupt Mask Status Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_STATUS1 :: MASK_STATUS [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_STATUS1_MASK_STATUS_MASK            0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_STATUS1_MASK_STATUS_SHIFT           0
#define BCHP_LEAP_HOST_L2_MASK_STATUS1_MASK_STATUS_DEFAULT         0xffffffff

/***************************************************************************
 *MASK_SET1 - Host Interrupt Mask Set Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_SET1 :: MASK_SET [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_SET1_MASK_SET_MASK                  0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_SET1_MASK_SET_SHIFT                 0

/***************************************************************************
 *MASK_CLEAR1 - Host Interrupt Mask Clear Register
 ***************************************************************************/
/* LEAP_HOST_L2 :: MASK_CLEAR1 :: MASK_CLEAR [31:00] */
#define BCHP_LEAP_HOST_L2_MASK_CLEAR1_MASK_CLEAR_MASK              0xffffffff
#define BCHP_LEAP_HOST_L2_MASK_CLEAR1_MASK_CLEAR_SHIFT             0

#endif /* #ifndef BCHP_LEAP_HOST_L2_H__ */

/* End of File */
