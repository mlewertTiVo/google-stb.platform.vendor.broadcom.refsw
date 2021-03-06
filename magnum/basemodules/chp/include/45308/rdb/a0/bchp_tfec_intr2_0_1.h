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

#ifndef BCHP_TFEC_INTR2_0_1_H__
#define BCHP_TFEC_INTR2_0_1_H__

/***************************************************************************
 *TFEC_INTR2_0_1 - Channel 0 TFEC L2 Interrupt Control Registers
 ***************************************************************************/
#define BCHP_TFEC_INTR2_0_1_CPU_STATUS           0x06100200 /* [RO] CPU interrupt Status Register */
#define BCHP_TFEC_INTR2_0_1_CPU_SET              0x06100204 /* [WO] CPU interrupt Set Register */
#define BCHP_TFEC_INTR2_0_1_CPU_CLEAR            0x06100208 /* [WO] CPU interrupt Clear Register */
#define BCHP_TFEC_INTR2_0_1_CPU_MASK_STATUS      0x0610020c /* [RO] CPU interrupt Mask Status Register */
#define BCHP_TFEC_INTR2_0_1_CPU_MASK_SET         0x06100210 /* [WO] CPU interrupt Mask Set Register */
#define BCHP_TFEC_INTR2_0_1_CPU_MASK_CLEAR       0x06100214 /* [WO] CPU interrupt Mask Clear Register */
#define BCHP_TFEC_INTR2_0_1_PCI_STATUS           0x06100218 /* [RO] PCI interrupt Status Register */
#define BCHP_TFEC_INTR2_0_1_PCI_SET              0x0610021c /* [WO] PCI interrupt Set Register */
#define BCHP_TFEC_INTR2_0_1_PCI_CLEAR            0x06100220 /* [WO] PCI interrupt Clear Register */
#define BCHP_TFEC_INTR2_0_1_PCI_MASK_STATUS      0x06100224 /* [RO] PCI interrupt Mask Status Register */
#define BCHP_TFEC_INTR2_0_1_PCI_MASK_SET         0x06100228 /* [WO] PCI interrupt Mask Set Register */
#define BCHP_TFEC_INTR2_0_1_PCI_MASK_CLEAR       0x0610022c /* [WO] PCI interrupt Mask Clear Register */

#endif /* #ifndef BCHP_TFEC_INTR2_0_1_H__ */

/* End of File */
