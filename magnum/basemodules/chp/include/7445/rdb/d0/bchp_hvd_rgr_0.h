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
 * Date:           Generated on              Mon Sep 23 09:50:32 2013
 *                 Full Compile MD5 Checksum fcccce298b546dd6a1f4cbad288478da
 *                   (minus title and desc)  
 *                 MD5 Checksum              211556602e37a33262598b3d5eeba81c
 *
 * Compiled with:  RDB Utility               combo_header.pl
 *                 RDB Parser                3.0
 *                 unknown                   unknown
 *                 Perl Interpreter          5.008008
 *                 Operating System          linux
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BCHP_HVD_RGR_0_H__
#define BCHP_HVD_RGR_0_H__

/***************************************************************************
 *HVD_RGR_0
 ***************************************************************************/
#define BCHP_HVD_RGR_0_REVISION                  0x00080400 /* RGR Bridge Revision */
#define BCHP_HVD_RGR_0_CTRL                      0x00080404 /* RGR Bridge Control Register */
#define BCHP_HVD_RGR_0_RBUS_TIMER                0x00080408 /* RGR Bridge RBUS Timer Register */
#define BCHP_HVD_RGR_0_SW_INIT_0                 0x0008040c /* RGR Bridge Software Init 0 Register */
#define BCHP_HVD_RGR_0_SW_INIT_1                 0x00080410 /* RGR Bridge Software Init 1 Register */

/***************************************************************************
 *REVISION - RGR Bridge Revision
 ***************************************************************************/
/* HVD_RGR_0 :: REVISION :: reserved0 [31:16] */
#define BCHP_HVD_RGR_0_REVISION_reserved0_MASK                     0xffff0000
#define BCHP_HVD_RGR_0_REVISION_reserved0_SHIFT                    16

/* HVD_RGR_0 :: REVISION :: MAJOR [15:08] */
#define BCHP_HVD_RGR_0_REVISION_MAJOR_MASK                         0x0000ff00
#define BCHP_HVD_RGR_0_REVISION_MAJOR_SHIFT                        8
#define BCHP_HVD_RGR_0_REVISION_MAJOR_DEFAULT                      0x00000002

/* HVD_RGR_0 :: REVISION :: MINOR [07:00] */
#define BCHP_HVD_RGR_0_REVISION_MINOR_MASK                         0x000000ff
#define BCHP_HVD_RGR_0_REVISION_MINOR_SHIFT                        0
#define BCHP_HVD_RGR_0_REVISION_MINOR_DEFAULT                      0x00000000

/***************************************************************************
 *CTRL - RGR Bridge Control Register
 ***************************************************************************/
/* HVD_RGR_0 :: CTRL :: reserved0 [31:02] */
#define BCHP_HVD_RGR_0_CTRL_reserved0_MASK                         0xfffffffc
#define BCHP_HVD_RGR_0_CTRL_reserved0_SHIFT                        2

/* HVD_RGR_0 :: CTRL :: rbus_error_intr [01:01] */
#define BCHP_HVD_RGR_0_CTRL_rbus_error_intr_MASK                   0x00000002
#define BCHP_HVD_RGR_0_CTRL_rbus_error_intr_SHIFT                  1
#define BCHP_HVD_RGR_0_CTRL_rbus_error_intr_DEFAULT                0x00000000
#define BCHP_HVD_RGR_0_CTRL_rbus_error_intr_INTR_DISABLE           0
#define BCHP_HVD_RGR_0_CTRL_rbus_error_intr_INTR_ENABLE            1

/* HVD_RGR_0 :: CTRL :: gisb_error_intr [00:00] */
#define BCHP_HVD_RGR_0_CTRL_gisb_error_intr_MASK                   0x00000001
#define BCHP_HVD_RGR_0_CTRL_gisb_error_intr_SHIFT                  0
#define BCHP_HVD_RGR_0_CTRL_gisb_error_intr_DEFAULT                0x00000000
#define BCHP_HVD_RGR_0_CTRL_gisb_error_intr_INTR_DISABLE           0
#define BCHP_HVD_RGR_0_CTRL_gisb_error_intr_INTR_ENABLE            1

/***************************************************************************
 *RBUS_TIMER - RGR Bridge RBUS Timer Register
 ***************************************************************************/
/* HVD_RGR_0 :: RBUS_TIMER :: reserved0 [31:16] */
#define BCHP_HVD_RGR_0_RBUS_TIMER_reserved0_MASK                   0xffff0000
#define BCHP_HVD_RGR_0_RBUS_TIMER_reserved0_SHIFT                  16

/* HVD_RGR_0 :: RBUS_TIMER :: timer_value [15:00] */
#define BCHP_HVD_RGR_0_RBUS_TIMER_timer_value_MASK                 0x0000ffff
#define BCHP_HVD_RGR_0_RBUS_TIMER_timer_value_SHIFT                0
#define BCHP_HVD_RGR_0_RBUS_TIMER_timer_value_DEFAULT              0x0000ffff

/***************************************************************************
 *SW_INIT_0 - RGR Bridge Software Init 0 Register
 ***************************************************************************/
/* HVD_RGR_0 :: SW_INIT_0 :: reserved0 [31:01] */
#define BCHP_HVD_RGR_0_SW_INIT_0_reserved0_MASK                    0xfffffffe
#define BCHP_HVD_RGR_0_SW_INIT_0_reserved0_SHIFT                   1

/* HVD_RGR_0 :: SW_INIT_0 :: AVD00_SW_INIT [00:00] */
#define BCHP_HVD_RGR_0_SW_INIT_0_AVD00_SW_INIT_MASK                0x00000001
#define BCHP_HVD_RGR_0_SW_INIT_0_AVD00_SW_INIT_SHIFT               0
#define BCHP_HVD_RGR_0_SW_INIT_0_AVD00_SW_INIT_DEFAULT             0x00000000
#define BCHP_HVD_RGR_0_SW_INIT_0_AVD00_SW_INIT_DEASSERT            0
#define BCHP_HVD_RGR_0_SW_INIT_0_AVD00_SW_INIT_ASSERT              1

/***************************************************************************
 *SW_INIT_1 - RGR Bridge Software Init 1 Register
 ***************************************************************************/
/* HVD_RGR_0 :: SW_INIT_1 :: reserved0 [31:01] */
#define BCHP_HVD_RGR_0_SW_INIT_1_reserved0_MASK                    0xfffffffe
#define BCHP_HVD_RGR_0_SW_INIT_1_reserved0_SHIFT                   1

/* HVD_RGR_0 :: SW_INIT_1 :: AVD00_SW_INIT [00:00] */
#define BCHP_HVD_RGR_0_SW_INIT_1_AVD00_SW_INIT_MASK                0x00000001
#define BCHP_HVD_RGR_0_SW_INIT_1_AVD00_SW_INIT_SHIFT               0
#define BCHP_HVD_RGR_0_SW_INIT_1_AVD00_SW_INIT_DEFAULT             0x00000001
#define BCHP_HVD_RGR_0_SW_INIT_1_AVD00_SW_INIT_DEASSERT            0
#define BCHP_HVD_RGR_0_SW_INIT_1_AVD00_SW_INIT_ASSERT              1

#endif /* #ifndef BCHP_HVD_RGR_0_H__ */

/* End of File */
