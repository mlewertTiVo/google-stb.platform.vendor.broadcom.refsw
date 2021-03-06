/***************************************************************************
 *     Copyright (c) 1999-2012, Broadcom Corporation
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
 * Date:           Generated on         Tue Feb 28 11:03:19 2012
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

#ifndef BCHP_SUN_GR_H__
#define BCHP_SUN_GR_H__

/***************************************************************************
 *SUN_GR - Registers for the sys_ctrl block's GR bridge
 ***************************************************************************/
#define BCHP_SUN_GR_REVISION                     0x00400400 /* GR Bridge Revision */
#define BCHP_SUN_GR_CTRL                         0x00400404 /* GR Bridge Control Register */
#define BCHP_SUN_GR_SW_INIT_0                    0x00400408 /* GR Bridge Software Init 0 Register */
#define BCHP_SUN_GR_SW_INIT_1                    0x0040040c /* GR Bridge Software Init 1 Register */

/***************************************************************************
 *REVISION - GR Bridge Revision
 ***************************************************************************/
/* SUN_GR :: REVISION :: reserved0 [31:16] */
#define BCHP_SUN_GR_REVISION_reserved0_MASK                        0xffff0000
#define BCHP_SUN_GR_REVISION_reserved0_SHIFT                       16

/* SUN_GR :: REVISION :: MAJOR [15:08] */
#define BCHP_SUN_GR_REVISION_MAJOR_MASK                            0x0000ff00
#define BCHP_SUN_GR_REVISION_MAJOR_SHIFT                           8
#define BCHP_SUN_GR_REVISION_MAJOR_DEFAULT                         0x00000002

/* SUN_GR :: REVISION :: MINOR [07:00] */
#define BCHP_SUN_GR_REVISION_MINOR_MASK                            0x000000ff
#define BCHP_SUN_GR_REVISION_MINOR_SHIFT                           0
#define BCHP_SUN_GR_REVISION_MINOR_DEFAULT                         0x00000000

/***************************************************************************
 *CTRL - GR Bridge Control Register
 ***************************************************************************/
/* SUN_GR :: CTRL :: reserved0 [31:01] */
#define BCHP_SUN_GR_CTRL_reserved0_MASK                            0xfffffffe
#define BCHP_SUN_GR_CTRL_reserved0_SHIFT                           1

/* SUN_GR :: CTRL :: gisb_error_intr [00:00] */
#define BCHP_SUN_GR_CTRL_gisb_error_intr_MASK                      0x00000001
#define BCHP_SUN_GR_CTRL_gisb_error_intr_SHIFT                     0
#define BCHP_SUN_GR_CTRL_gisb_error_intr_DEFAULT                   0x00000000
#define BCHP_SUN_GR_CTRL_gisb_error_intr_INTR_DISABLE              0
#define BCHP_SUN_GR_CTRL_gisb_error_intr_INTR_ENABLE               1

/***************************************************************************
 *SW_INIT_0 - GR Bridge Software Init 0 Register
 ***************************************************************************/
/* SUN_GR :: SW_INIT_0 :: reserved0 [31:01] */
#define BCHP_SUN_GR_SW_INIT_0_reserved0_MASK                       0xfffffffe
#define BCHP_SUN_GR_SW_INIT_0_reserved0_SHIFT                      1

/* SUN_GR :: SW_INIT_0 :: sm_sw_init [00:00] */
#define BCHP_SUN_GR_SW_INIT_0_sm_sw_init_MASK                      0x00000001
#define BCHP_SUN_GR_SW_INIT_0_sm_sw_init_SHIFT                     0
#define BCHP_SUN_GR_SW_INIT_0_sm_sw_init_DEFAULT                   0x00000000
#define BCHP_SUN_GR_SW_INIT_0_sm_sw_init_DEASSERT                  0
#define BCHP_SUN_GR_SW_INIT_0_sm_sw_init_ASSERT                    1

/***************************************************************************
 *SW_INIT_1 - GR Bridge Software Init 1 Register
 ***************************************************************************/
/* SUN_GR :: SW_INIT_1 :: reserved0 [31:01] */
#define BCHP_SUN_GR_SW_INIT_1_reserved0_MASK                       0xfffffffe
#define BCHP_SUN_GR_SW_INIT_1_reserved0_SHIFT                      1

/* SUN_GR :: SW_INIT_1 :: tpcap_sw_init [00:00] */
#define BCHP_SUN_GR_SW_INIT_1_tpcap_sw_init_MASK                   0x00000001
#define BCHP_SUN_GR_SW_INIT_1_tpcap_sw_init_SHIFT                  0
#define BCHP_SUN_GR_SW_INIT_1_tpcap_sw_init_DEFAULT                0x00000001
#define BCHP_SUN_GR_SW_INIT_1_tpcap_sw_init_DEASSERT               0
#define BCHP_SUN_GR_SW_INIT_1_tpcap_sw_init_ASSERT                 1

#endif /* #ifndef BCHP_SUN_GR_H__ */

/* End of File */
