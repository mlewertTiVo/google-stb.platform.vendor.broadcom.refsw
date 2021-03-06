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
 * Date:           Generated on              Mon Sep 23 09:50:33 2013
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

#ifndef BCHP_DECODE_MAIN_1_H__
#define BCHP_DECODE_MAIN_1_H__

/***************************************************************************
 *DECODE_MAIN_1
 ***************************************************************************/
#define BCHP_DECODE_MAIN_1_REG_MAINCTL           0x00120100 /* Decoder Control */
#define BCHP_DECODE_MAIN_1_REG_FRAMESIZE         0x00120104 /* Size of the picture being decoded */
#define BCHP_DECODE_MAIN_1_REG_DEC_VERSION       0x00120108 /* Version of the decoder core */
#define BCHP_DECODE_MAIN_1_REG_STATUS            0x00120110 /* Provides back-end decoder processing status */
#define BCHP_DECODE_MAIN_1_REG_PMONCTL           0x00120120 /* Performance Monitoring */
#define BCHP_DECODE_MAIN_1_REG_PMONCNT0          0x00120124 /* REG_PMONCNT0 */
#define BCHP_DECODE_MAIN_1_REG_PMONCNT1          0x00120128 /* REG_PMONCNT1 */
#define BCHP_DECODE_MAIN_1_REG_PMON_MBCTL        0x0012012c /* REG_PMON_MBCTL */
#define BCHP_DECODE_MAIN_1_DBLK_BUFF_CONTROL     0x00120130 /* DBLK_BUFF_CONTROL */
#define BCHP_DECODE_MAIN_1_CRC_CONTROL           0x00120134 /* DBLK CRC CONTROL register */
#define BCHP_DECODE_MAIN_1_CRC_SEED              0x00120138 /* DBLK CRC SEED register */
#define BCHP_DECODE_MAIN_1_CRC_CHKSUM_Y          0x0012013c /* DBLK Luma CRC/Checksum result register */
#define BCHP_DECODE_MAIN_1_CRC_CHKSUM_CB         0x00120140 /* DBLK Chroma (Cb) CRC/Checksum result register */
#define BCHP_DECODE_MAIN_1_CRC_CHKSUM_CR         0x00120144 /* DBLK Chroma (Cr) CRC/Checksum result register */
#define BCHP_DECODE_MAIN_1_REG_VP6_MCOM_CONTROL  0x00120150 /* VP6 Mocomp Control */
#define BCHP_DECODE_MAIN_1_REG_VP6_MCOM_AUTO     0x00120154 /* VP6 Mocomp Auto Filter Selection */
#define BCHP_DECODE_MAIN_1_REG_VP6_MCOM_ALPHA    0x00120158 /* VP6 Mocomp Alpha Filter Selection */
#define BCHP_DECODE_MAIN_1_REG_VP6_MCOM_FLIMIT   0x0012015c /* VP6 Mocomp Flimit Control */
#define BCHP_DECODE_MAIN_1_REG_BACKEND_DEBUG     0x00120160 /* Backend debug Select */
#define BCHP_DECODE_MAIN_1_REG_VC1_MC_DEBUG      0x00120164 /* VC1 Mocomp Debug */
#define BCHP_DECODE_MAIN_1_REG_QPEL_FIFO_DEBUG   0x00120168 /* Qpel FIFO Debug */
#define BCHP_DECODE_MAIN_1_REG_MAIN_END          0x001201fc /* REG_MAIN_END */

#endif /* #ifndef BCHP_DECODE_MAIN_1_H__ */

/* End of File */
