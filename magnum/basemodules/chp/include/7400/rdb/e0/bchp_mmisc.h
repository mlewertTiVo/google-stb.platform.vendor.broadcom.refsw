/***************************************************************************
 *     Copyright (c) 1999-2008, Broadcom Corporation
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
 * Date:           Generated on         Wed Jan  9 09:00:03 2008
 *                 MD5 Checksum         847dc12a9d71c4c68a648bbf19a883e3
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

#ifndef BCHP_MMISC_H__
#define BCHP_MMISC_H__

/***************************************************************************
 *MMISC - BVN Middle Control Registers
 ***************************************************************************/
#define BCHP_MMISC_SOFT_RESET                    0x00122800 /* BVN Middle Soft Reset */
#define BCHP_MMISC_TEST_PORT_SEL                 0x00122804 /* BVN Middle Test Port Select */
#define BCHP_MMISC_TEST_PORT_DATA                0x00122808 /* BVN Middle Test Port Status */
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL        0x00122814 /* BVN Middle SCL_0-3 MBIST TM Control */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL        0x00122818 /* BVN Middle DNR_0-1 MBIST TM Control */
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL     0x0012281c /* BVN Middle MAD_0 PDB SRAM MBIST TM Control */
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL   0x00122820 /* BVN Middle MAD_0 BIOC SRAM MBIST TM Control */
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL        0x00122824 /* BVN Middle FGT_0 MBIST TM Control */
#define BCHP_MMISC_SCRATCH_0                     0x0012282c /* Scratch Register */

/***************************************************************************
 *SOFT_RESET - BVN Middle Soft Reset
 ***************************************************************************/
/* MMISC :: SOFT_RESET :: reserved0 [31:31] */
#define BCHP_MMISC_SOFT_RESET_reserved0_MASK                       0x80000000
#define BCHP_MMISC_SOFT_RESET_reserved0_SHIFT                      31

/* MMISC :: SOFT_RESET :: DNR_1 [30:30] */
#define BCHP_MMISC_SOFT_RESET_DNR_1_MASK                           0x40000000
#define BCHP_MMISC_SOFT_RESET_DNR_1_SHIFT                          30

/* MMISC :: SOFT_RESET :: DNR_0 [29:29] */
#define BCHP_MMISC_SOFT_RESET_DNR_0_MASK                           0x20000000
#define BCHP_MMISC_SOFT_RESET_DNR_0_SHIFT                          29

/* MMISC :: SOFT_RESET :: TAB [28:28] */
#define BCHP_MMISC_SOFT_RESET_TAB_MASK                             0x10000000
#define BCHP_MMISC_SOFT_RESET_TAB_SHIFT                            28

/* MMISC :: SOFT_RESET :: reserved1 [27:26] */
#define BCHP_MMISC_SOFT_RESET_reserved1_MASK                       0x0c000000
#define BCHP_MMISC_SOFT_RESET_reserved1_SHIFT                      26

/* MMISC :: SOFT_RESET :: LBOX_1 [25:25] */
#define BCHP_MMISC_SOFT_RESET_LBOX_1_MASK                          0x02000000
#define BCHP_MMISC_SOFT_RESET_LBOX_1_SHIFT                         25

/* MMISC :: SOFT_RESET :: LBOX_0 [24:24] */
#define BCHP_MMISC_SOFT_RESET_LBOX_0_MASK                          0x01000000
#define BCHP_MMISC_SOFT_RESET_LBOX_0_SHIFT                         24

/* MMISC :: SOFT_RESET :: reserved2 [23:21] */
#define BCHP_MMISC_SOFT_RESET_reserved2_MASK                       0x00e00000
#define BCHP_MMISC_SOFT_RESET_reserved2_SHIFT                      21

/* MMISC :: SOFT_RESET :: FGT_0 [20:20] */
#define BCHP_MMISC_SOFT_RESET_FGT_0_MASK                           0x00100000
#define BCHP_MMISC_SOFT_RESET_FGT_0_SHIFT                          20

/* MMISC :: SOFT_RESET :: reserved3 [19:17] */
#define BCHP_MMISC_SOFT_RESET_reserved3_MASK                       0x000e0000
#define BCHP_MMISC_SOFT_RESET_reserved3_SHIFT                      17

/* MMISC :: SOFT_RESET :: MAD_0 [16:16] */
#define BCHP_MMISC_SOFT_RESET_MAD_0_MASK                           0x00010000
#define BCHP_MMISC_SOFT_RESET_MAD_0_SHIFT                          16

/* MMISC :: SOFT_RESET :: reserved4 [15:10] */
#define BCHP_MMISC_SOFT_RESET_reserved4_MASK                       0x0000fc00
#define BCHP_MMISC_SOFT_RESET_reserved4_SHIFT                      10

/* MMISC :: SOFT_RESET :: VNET_B [09:09] */
#define BCHP_MMISC_SOFT_RESET_VNET_B_MASK                          0x00000200
#define BCHP_MMISC_SOFT_RESET_VNET_B_SHIFT                         9

/* MMISC :: SOFT_RESET :: VNET_F [08:08] */
#define BCHP_MMISC_SOFT_RESET_VNET_F_MASK                          0x00000100
#define BCHP_MMISC_SOFT_RESET_VNET_F_SHIFT                         8

/* MMISC :: SOFT_RESET :: reserved5 [07:04] */
#define BCHP_MMISC_SOFT_RESET_reserved5_MASK                       0x000000f0
#define BCHP_MMISC_SOFT_RESET_reserved5_SHIFT                      4

/* MMISC :: SOFT_RESET :: SCL_3 [03:03] */
#define BCHP_MMISC_SOFT_RESET_SCL_3_MASK                           0x00000008
#define BCHP_MMISC_SOFT_RESET_SCL_3_SHIFT                          3

/* MMISC :: SOFT_RESET :: SCL_2 [02:02] */
#define BCHP_MMISC_SOFT_RESET_SCL_2_MASK                           0x00000004
#define BCHP_MMISC_SOFT_RESET_SCL_2_SHIFT                          2

/* MMISC :: SOFT_RESET :: SCL_1 [01:01] */
#define BCHP_MMISC_SOFT_RESET_SCL_1_MASK                           0x00000002
#define BCHP_MMISC_SOFT_RESET_SCL_1_SHIFT                          1

/* MMISC :: SOFT_RESET :: SCL_0 [00:00] */
#define BCHP_MMISC_SOFT_RESET_SCL_0_MASK                           0x00000001
#define BCHP_MMISC_SOFT_RESET_SCL_0_SHIFT                          0

/***************************************************************************
 *TEST_PORT_SEL - BVN Middle Test Port Select
 ***************************************************************************/
/* MMISC :: TEST_PORT_SEL :: reserved0 [31:02] */
#define BCHP_MMISC_TEST_PORT_SEL_reserved0_MASK                    0xfffffffc
#define BCHP_MMISC_TEST_PORT_SEL_reserved0_SHIFT                   2

/* MMISC :: TEST_PORT_SEL :: TEST_PORT_SEL [01:00] */
#define BCHP_MMISC_TEST_PORT_SEL_TEST_PORT_SEL_MASK                0x00000003
#define BCHP_MMISC_TEST_PORT_SEL_TEST_PORT_SEL_SHIFT               0
#define BCHP_MMISC_TEST_PORT_SEL_TEST_PORT_SEL_TP_OUT_2            2
#define BCHP_MMISC_TEST_PORT_SEL_TEST_PORT_SEL_TP_OUT_1            1
#define BCHP_MMISC_TEST_PORT_SEL_TEST_PORT_SEL_TP_OUT_0            0

/***************************************************************************
 *TEST_PORT_DATA - BVN Middle Test Port Status
 ***************************************************************************/
/* MMISC :: TEST_PORT_DATA :: TEST_PORT_DATA [31:00] */
#define BCHP_MMISC_TEST_PORT_DATA_TEST_PORT_DATA_MASK              0xffffffff
#define BCHP_MMISC_TEST_PORT_DATA_TEST_PORT_DATA_SHIFT             0

/***************************************************************************
 *BVNM_SCL_MBIST_TM_CTRL - BVN Middle SCL_0-3 MBIST TM Control
 ***************************************************************************/
/* MMISC :: BVNM_SCL_MBIST_TM_CTRL :: SCL_TM3 [31:24] */
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM3_MASK             0xff000000
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM3_SHIFT            24

/* MMISC :: BVNM_SCL_MBIST_TM_CTRL :: SCL_TM2 [23:16] */
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM2_MASK             0x00ff0000
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM2_SHIFT            16

/* MMISC :: BVNM_SCL_MBIST_TM_CTRL :: SCL_TM1 [15:08] */
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM1_MASK             0x0000ff00
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM1_SHIFT            8

/* MMISC :: BVNM_SCL_MBIST_TM_CTRL :: SCL_TM0 [07:00] */
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM0_MASK             0x000000ff
#define BCHP_MMISC_BVNM_SCL_MBIST_TM_CTRL_SCL_TM0_SHIFT            0

/***************************************************************************
 *BVNM_DNR_MBIST_TM_CTRL - BVN Middle DNR_0-1 MBIST TM Control
 ***************************************************************************/
/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: reserved0 [31:28] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_reserved0_MASK           0xf0000000
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_reserved0_SHIFT          28

/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: DNR_TM3 [27:24] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM3_MASK             0x0f000000
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM3_SHIFT            24

/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: DNR_TM2 [23:16] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM2_MASK             0x00ff0000
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM2_SHIFT            16

/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: reserved1 [15:12] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_reserved1_MASK           0x0000f000
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_reserved1_SHIFT          12

/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: DNR_TM1 [11:08] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM1_MASK             0x00000f00
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM1_SHIFT            8

/* MMISC :: BVNM_DNR_MBIST_TM_CTRL :: DNR_TM0 [07:00] */
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM0_MASK             0x000000ff
#define BCHP_MMISC_BVNM_DNR_MBIST_TM_CTRL_DNR_TM0_SHIFT            0

/***************************************************************************
 *BVNM_MAD_PD_MBIST_TM_CTRL - BVN Middle MAD_0 PDB SRAM MBIST TM Control
 ***************************************************************************/
/* MMISC :: BVNM_MAD_PD_MBIST_TM_CTRL :: reserved0 [31:24] */
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_reserved0_MASK        0xff000000
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_reserved0_SHIFT       24

/* MMISC :: BVNM_MAD_PD_MBIST_TM_CTRL :: MAD_TM2 [23:16] */
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM2_MASK          0x00ff0000
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM2_SHIFT         16

/* MMISC :: BVNM_MAD_PD_MBIST_TM_CTRL :: MAD_TM1 [15:08] */
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM1_MASK          0x0000ff00
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM1_SHIFT         8

/* MMISC :: BVNM_MAD_PD_MBIST_TM_CTRL :: MAD_TM0 [07:00] */
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM0_MASK          0x000000ff
#define BCHP_MMISC_BVNM_MAD_PD_MBIST_TM_CTRL_MAD_TM0_SHIFT         0

/***************************************************************************
 *BVNM_MAD_BIOC_MBIST_TM_CTRL - BVN Middle MAD_0 BIOC SRAM MBIST TM Control
 ***************************************************************************/
/* MMISC :: BVNM_MAD_BIOC_MBIST_TM_CTRL :: reserved0 [31:24] */
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_reserved0_MASK      0xff000000
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_reserved0_SHIFT     24

/* MMISC :: BVNM_MAD_BIOC_MBIST_TM_CTRL :: MAD_TM2 [23:16] */
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM2_MASK        0x00ff0000
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM2_SHIFT       16

/* MMISC :: BVNM_MAD_BIOC_MBIST_TM_CTRL :: MAD_TM1 [15:08] */
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM1_MASK        0x0000ff00
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM1_SHIFT       8

/* MMISC :: BVNM_MAD_BIOC_MBIST_TM_CTRL :: MAD_TM0 [07:00] */
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM0_MASK        0x000000ff
#define BCHP_MMISC_BVNM_MAD_BIOC_MBIST_TM_CTRL_MAD_TM0_SHIFT       0

/***************************************************************************
 *BVNM_FGT_MBIST_TM_CTRL - BVN Middle FGT_0 MBIST TM Control
 ***************************************************************************/
/* MMISC :: BVNM_FGT_MBIST_TM_CTRL :: FGT_TM3 [31:24] */
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM3_MASK             0xff000000
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM3_SHIFT            24

/* MMISC :: BVNM_FGT_MBIST_TM_CTRL :: FGT_TM2 [23:16] */
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM2_MASK             0x00ff0000
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM2_SHIFT            16

/* MMISC :: BVNM_FGT_MBIST_TM_CTRL :: FGT_TM1 [15:08] */
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM1_MASK             0x0000ff00
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM1_SHIFT            8

/* MMISC :: BVNM_FGT_MBIST_TM_CTRL :: FGT_TM0 [07:00] */
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM0_MASK             0x000000ff
#define BCHP_MMISC_BVNM_FGT_MBIST_TM_CTRL_FGT_TM0_SHIFT            0

/***************************************************************************
 *SCRATCH_0 - Scratch Register
 ***************************************************************************/
/* MMISC :: SCRATCH_0 :: VALUE [31:00] */
#define BCHP_MMISC_SCRATCH_0_VALUE_MASK                            0xffffffff
#define BCHP_MMISC_SCRATCH_0_VALUE_SHIFT                           0

#endif /* #ifndef BCHP_MMISC_H__ */

/* End of File */
