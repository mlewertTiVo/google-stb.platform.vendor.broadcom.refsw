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
 * Date:           Generated on               Wed Feb 11 10:14:00 2015
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

#ifndef BCHP_BBSI_RG_H__
#define BCHP_BBSI_RG_H__

/***************************************************************************
 *BBSI_RG - Registers for the BBSI RG bridge
 ***************************************************************************/
#define BCHP_BBSI_RG_REVISION                    0x00405c00 /* [RO] RG Bridge Revision */
#define BCHP_BBSI_RG_CTRL                        0x00405c04 /* [RW] RG Bridge Control Register */
#define BCHP_BBSI_RG_SW_INIT_0                   0x00405c08 /* [RW] RG Bridge Software Init 0 Register */
#define BCHP_BBSI_RG_SW_INIT_1                   0x00405c0c /* [RW] RG Bridge Software Init 1 Register */

/***************************************************************************
 *REVISION - RG Bridge Revision
 ***************************************************************************/
/* BBSI_RG :: REVISION :: reserved0 [31:16] */
#define BCHP_BBSI_RG_REVISION_reserved0_MASK                       0xffff0000
#define BCHP_BBSI_RG_REVISION_reserved0_SHIFT                      16

/* BBSI_RG :: REVISION :: MAJOR [15:08] */
#define BCHP_BBSI_RG_REVISION_MAJOR_MASK                           0x0000ff00
#define BCHP_BBSI_RG_REVISION_MAJOR_SHIFT                          8
#define BCHP_BBSI_RG_REVISION_MAJOR_DEFAULT                        0x00000002

/* BBSI_RG :: REVISION :: MINOR [07:00] */
#define BCHP_BBSI_RG_REVISION_MINOR_MASK                           0x000000ff
#define BCHP_BBSI_RG_REVISION_MINOR_SHIFT                          0
#define BCHP_BBSI_RG_REVISION_MINOR_DEFAULT                        0x00000000

/***************************************************************************
 *CTRL - RG Bridge Control Register
 ***************************************************************************/
/* BBSI_RG :: CTRL :: reserved0 [31:02] */
#define BCHP_BBSI_RG_CTRL_reserved0_MASK                           0xfffffffc
#define BCHP_BBSI_RG_CTRL_reserved0_SHIFT                          2

/* BBSI_RG :: CTRL :: rbus_error_intr [01:01] */
#define BCHP_BBSI_RG_CTRL_rbus_error_intr_MASK                     0x00000002
#define BCHP_BBSI_RG_CTRL_rbus_error_intr_SHIFT                    1
#define BCHP_BBSI_RG_CTRL_rbus_error_intr_DEFAULT                  0x00000000
#define BCHP_BBSI_RG_CTRL_rbus_error_intr_INTR_DISABLE             0
#define BCHP_BBSI_RG_CTRL_rbus_error_intr_INTR_ENABLE              1

/* BBSI_RG :: CTRL :: reserved1 [00:00] */
#define BCHP_BBSI_RG_CTRL_reserved1_MASK                           0x00000001
#define BCHP_BBSI_RG_CTRL_reserved1_SHIFT                          0

/***************************************************************************
 *SW_INIT_0 - RG Bridge Software Init 0 Register
 ***************************************************************************/
/* BBSI_RG :: SW_INIT_0 :: reserved0 [31:01] */
#define BCHP_BBSI_RG_SW_INIT_0_reserved0_MASK                      0xfffffffe
#define BCHP_BBSI_RG_SW_INIT_0_reserved0_SHIFT                     1

/* BBSI_RG :: SW_INIT_0 :: SPARE_SW_INIT [00:00] */
#define BCHP_BBSI_RG_SW_INIT_0_SPARE_SW_INIT_MASK                  0x00000001
#define BCHP_BBSI_RG_SW_INIT_0_SPARE_SW_INIT_SHIFT                 0
#define BCHP_BBSI_RG_SW_INIT_0_SPARE_SW_INIT_DEFAULT               0x00000000
#define BCHP_BBSI_RG_SW_INIT_0_SPARE_SW_INIT_DEASSERT              0
#define BCHP_BBSI_RG_SW_INIT_0_SPARE_SW_INIT_ASSERT                1

/***************************************************************************
 *SW_INIT_1 - RG Bridge Software Init 1 Register
 ***************************************************************************/
/* BBSI_RG :: SW_INIT_1 :: reserved0 [31:01] */
#define BCHP_BBSI_RG_SW_INIT_1_reserved0_MASK                      0xfffffffe
#define BCHP_BBSI_RG_SW_INIT_1_reserved0_SHIFT                     1

/* BBSI_RG :: SW_INIT_1 :: SPARE_SW_INIT [00:00] */
#define BCHP_BBSI_RG_SW_INIT_1_SPARE_SW_INIT_MASK                  0x00000001
#define BCHP_BBSI_RG_SW_INIT_1_SPARE_SW_INIT_SHIFT                 0
#define BCHP_BBSI_RG_SW_INIT_1_SPARE_SW_INIT_DEFAULT               0x00000001
#define BCHP_BBSI_RG_SW_INIT_1_SPARE_SW_INIT_DEASSERT              0
#define BCHP_BBSI_RG_SW_INIT_1_SPARE_SW_INIT_ASSERT                1

#endif /* #ifndef BCHP_BBSI_RG_H__ */

/* End of File */
