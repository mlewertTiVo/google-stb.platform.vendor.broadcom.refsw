/***************************************************************************
 *     Copyright (c) 1999-2011, Broadcom Corporation
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
 * Date:           Generated on         Mon Sep 19 21:37:13 2011
 *                 MD5 Checksum         e020a976c1177b900eff0f3d00f22e40
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

#ifndef BCHP_XPT_DPCR_PP_H__
#define BCHP_XPT_DPCR_PP_H__

/***************************************************************************
 *XPT_DPCR_PP - XPT DPCR Packet Processor Control Registers
 ***************************************************************************/
#define BCHP_XPT_DPCR_PP_PP_CTRL                 0x00a06400 /* Data Transport PCR Packet Processor Control Register */
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET         0x00a06404 /* Data Transport PCR Packet Processor Fixed Offset Register */

/***************************************************************************
 *PP_CTRL - Data Transport PCR Packet Processor Control Register
 ***************************************************************************/
/* XPT_DPCR_PP :: PP_CTRL :: reserved0 [31:23] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved0_MASK                    0xff800000
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved0_SHIFT                   23

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_AFID_FIELD [22:22] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_MASK 0x00400000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_SHIFT 22
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFID_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CFF_FIELD [21:21] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_MASK  0x00200000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_SHIFT 21
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CFF_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_RTS00_FIELD [20:20] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_MASK 0x00100000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_SHIFT 20
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_RTS00_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_AFS_FIELD [19:19] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_MASK  0x00080000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_SHIFT 19
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_AFS_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CC_FIELD [18:18] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_MASK   0x00040000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_SHIFT  18
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CC_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_OCF_FIELD [17:17] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_MASK  0x00020000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_SHIFT 17
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_OCF_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_IGNORE_DIRECTV_CF_FIELD [16:16] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_MASK   0x00010000
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_SHIFT  16
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_IGNORE_DIRECTV_CF_FIELD_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: reserved1 [15:10] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved1_MASK                    0x0000fc00
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved1_SHIFT                   10

/* XPT_DPCR_PP :: PP_CTRL :: PP_SOFT_INIT [09:09] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_MASK                 0x00000200
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_SHIFT                9
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_SOFT_INIT_DEFAULT              0

/* XPT_DPCR_PP :: PP_CTRL :: PP_PKT_ERR_DIS [08:08] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_MASK               0x00000100
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_SHIFT              8
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PKT_ERR_DIS_DEFAULT            0

/* XPT_DPCR_PP :: PP_CTRL :: reserved2 [07:05] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved2_MASK                    0x000000e0
#define BCHP_XPT_DPCR_PP_PP_CTRL_reserved2_SHIFT                   5

/* XPT_DPCR_PP :: PP_CTRL :: PP_JITTER_TIMESTAMP_MODE [04:03] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_MASK     0x00000018
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_SHIFT    3
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_JITTER_TIMESTAMP_MODE_DEFAULT  0

/* XPT_DPCR_PP :: PP_CTRL :: PP_FIXED_OFFSET_EN [02:02] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_MASK           0x00000004
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_SHIFT          2
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_FIXED_OFFSET_EN_DEFAULT        0

/* XPT_DPCR_PP :: PP_CTRL :: PP_PLAYBACK_PCR_JITTER_DIS [01:01] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_MASK   0x00000002
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_SHIFT  1
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_PLAYBACK_PCR_JITTER_DIS_DEFAULT 0

/* XPT_DPCR_PP :: PP_CTRL :: PP_LIVE_PCR_JITTER_DIS [00:00] */
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_MASK       0x00000001
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_SHIFT      0
#define BCHP_XPT_DPCR_PP_PP_CTRL_PP_LIVE_PCR_JITTER_DIS_DEFAULT    0

/***************************************************************************
 *PP_FIXED_OFFSET - Data Transport PCR Packet Processor Fixed Offset Register
 ***************************************************************************/
/* XPT_DPCR_PP :: PP_FIXED_OFFSET :: PP_FIXED_OFFSET [31:00] */
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_MASK      0xffffffff
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_SHIFT     0
#define BCHP_XPT_DPCR_PP_PP_FIXED_OFFSET_PP_FIXED_OFFSET_DEFAULT   0

#endif /* #ifndef BCHP_XPT_DPCR_PP_H__ */

/* End of File */
