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
 * Date:           Generated on               Fri Feb 20 00:05:23 2015
 *                 Full Compile MD5 Checksum  f4a546a20d0bd1f244e0d6a139e85ce0
 *                     (minus title and desc)
 *                 MD5 Checksum               a9d9eeea3a1c30a122d08de69d07786c
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     15715
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *
 *
 ***************************************************************************/

#ifndef BCHP_TNT_CMP_1_V0_H__
#define BCHP_TNT_CMP_1_V0_H__

/***************************************************************************
 *TNT_CMP_1_V0 - Transient Adjustment Block in Video Compositor 1/Video Intra Surface 0
 ***************************************************************************/
#define BCHP_TNT_CMP_1_V0_REVISION_ID            0x00687600 /* [RO] Revision ID */
#define BCHP_TNT_CMP_1_V0_DEMO_SETTING           0x00687614 /* [RW] Visual Effects Demo Setting */
#define BCHP_TNT_CMP_1_V0_TOP_CONTROL            0x00687618 /* [RW] Top Control */
#define BCHP_TNT_CMP_1_V0_SCRATCH_0              0x0068761c /* [RW] Scratch Register */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_GOFF_LOW  0x00687658 /* [RW] Luma Peaking Input Coring Low Band Gain Offsets */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_GOFF_HIGH 0x0068765c /* [RW] Luma Peaking Input Coring High Band Gain Offsets */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_THR_LOW   0x00687660 /* [RW] Luma Peaking Input Coring Low Band Thresholds */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_THR_HIGH  0x00687670 /* [RW] Luma Peaking Input Coring High Band Thresholds */
#define BCHP_TNT_CMP_1_V0_LPEAK_OUT_CORE         0x00687680 /* [RW] Luma Peaking Output Coring Control */
#define BCHP_TNT_CMP_1_V0_LPEAK_CLIP_AVOID       0x00687684 /* [RW] Luma Peaking Clip Avoidance Control */
#define BCHP_TNT_CMP_1_V0_LTI_CONTROL            0x00687688 /* [RW] LTI Control (Gain/Core/Avoid/Etc) */
#define BCHP_TNT_CMP_1_V0_LTI_FILTER             0x0068768c /* [RW] LTI Filter Control */
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_THR         0x00687690 /* [RW] LTI Input Coring Thresholds */
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_GOFF        0x006876a0 /* [RW] LTI Input Coring Gain Offsets */
#define BCHP_TNT_CMP_1_V0_CTI_H                  0x006876a4 /* [RW] CTI Horizontal Filter Controls */

/***************************************************************************
 *LPEAK_GAINS%i - Luma Peaking Overshoot/Undershoot Gains
 ***************************************************************************/
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_ARRAY_BASE                  0x00687620
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_ARRAY_START                 0
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_ARRAY_END                   3
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_ARRAY_ELEMENT_SIZE          32

/***************************************************************************
 *LPEAK_GAINS%i - Luma Peaking Overshoot/Undershoot Gains
 ***************************************************************************/
/* TNT_CMP_1_V0 :: LPEAK_GAINSi :: reserved0 [31:14] */
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_reserved0_MASK              0xffffc000
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_reserved0_SHIFT             14

/* TNT_CMP_1_V0 :: LPEAK_GAINSi :: GAIN_NEG [13:08] */
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_NEG_MASK               0x00003f00
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_NEG_SHIFT              8
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_NEG_DEFAULT            0x00000000

/* TNT_CMP_1_V0 :: LPEAK_GAINSi :: reserved1 [07:06] */
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_reserved1_MASK              0x000000c0
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_reserved1_SHIFT             6

/* TNT_CMP_1_V0 :: LPEAK_GAINSi :: GAIN_POS [05:00] */
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_POS_MASK               0x0000003f
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_POS_SHIFT              0
#define BCHP_TNT_CMP_1_V0_LPEAK_GAINSi_GAIN_POS_DEFAULT            0x00000000


/***************************************************************************
 *LPEAK_INCORE_DIV_LOW%i - Luma Peaking Input Coring Low Band Threshold Scale Factors
 ***************************************************************************/
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ARRAY_BASE         0x00687664
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ARRAY_START        0
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ARRAY_END          2
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ARRAY_ELEMENT_SIZE 32

/***************************************************************************
 *LPEAK_INCORE_DIV_LOW%i - Luma Peaking Input Coring Low Band Threshold Scale Factors
 ***************************************************************************/
/* TNT_CMP_1_V0 :: LPEAK_INCORE_DIV_LOWi :: reserved0 [31:16] */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_reserved0_MASK     0xffff0000
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_reserved0_SHIFT    16

/* TNT_CMP_1_V0 :: LPEAK_INCORE_DIV_LOWi :: ONE_OVER_T [15:00] */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ONE_OVER_T_MASK    0x0000ffff
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ONE_OVER_T_SHIFT   0
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_LOWi_ONE_OVER_T_DEFAULT 0x00000000


/***************************************************************************
 *LPEAK_INCORE_DIV_HIGH%i - Luma Peaking Input Coring High Band Threshold Scale Factors
 ***************************************************************************/
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ARRAY_BASE        0x00687674
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ARRAY_START       0
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ARRAY_END         2
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ARRAY_ELEMENT_SIZE 32

/***************************************************************************
 *LPEAK_INCORE_DIV_HIGH%i - Luma Peaking Input Coring High Band Threshold Scale Factors
 ***************************************************************************/
/* TNT_CMP_1_V0 :: LPEAK_INCORE_DIV_HIGHi :: reserved0 [31:16] */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_reserved0_MASK    0xffff0000
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_reserved0_SHIFT   16

/* TNT_CMP_1_V0 :: LPEAK_INCORE_DIV_HIGHi :: ONE_OVER_T [15:00] */
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ONE_OVER_T_MASK   0x0000ffff
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ONE_OVER_T_SHIFT  0
#define BCHP_TNT_CMP_1_V0_LPEAK_INCORE_DIV_HIGHi_ONE_OVER_T_DEFAULT 0x00000000


/***************************************************************************
 *LTI_INCORE_DIV%i - LTI Input Coring Threshold Scale Factors
 ***************************************************************************/
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ARRAY_BASE               0x00687694
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ARRAY_START              0
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ARRAY_END                2
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ARRAY_ELEMENT_SIZE       32

/***************************************************************************
 *LTI_INCORE_DIV%i - LTI Input Coring Threshold Scale Factors
 ***************************************************************************/
/* TNT_CMP_1_V0 :: LTI_INCORE_DIVi :: reserved0 [31:16] */
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_reserved0_MASK           0xffff0000
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_reserved0_SHIFT          16

/* TNT_CMP_1_V0 :: LTI_INCORE_DIVi :: ONE_OVER_T [15:00] */
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ONE_OVER_T_MASK          0x0000ffff
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ONE_OVER_T_SHIFT         0
#define BCHP_TNT_CMP_1_V0_LTI_INCORE_DIVi_ONE_OVER_T_DEFAULT       0x00000000


#endif /* #ifndef BCHP_TNT_CMP_1_V0_H__ */

/* End of File */
