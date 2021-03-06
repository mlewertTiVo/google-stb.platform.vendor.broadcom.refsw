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
 * Date:           Generated on               Fri Aug 14 09:27:43 2015
 *                 Full Compile MD5 Checksum  498077c0ce1e95e6ab4307854b2612d7
 *                     (minus title and desc)
 *                 MD5 Checksum               d5991b102b67d0c014966f49ee5cb996
 *
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     16421
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
 ***************************************************************************/

#ifndef BCHP_DVI_CSC_0_H__
#define BCHP_DVI_CSC_0_H__

/***************************************************************************
 *DVI_CSC_0 - DVI Frontend Color Space Converter 0
 ***************************************************************************/
#define BCHP_DVI_CSC_0_CSC_REV_ID                0x00683900 /* [RO] Revision ID register */
#define BCHP_DVI_CSC_0_CSC_MODE                  0x00683908 /* [RW] Color space converter mode register */
#define BCHP_DVI_CSC_0_CSC_MIN_MAX               0x0068390c /* [RW] Color space converter min_max clamp register */
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00         0x00683910 /* [RW] Video Encoder Color Matrix coefficients c01 and c00 */
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02         0x00683914 /* [RW] Video Encoder Color Matrix coefficients c03 and c02 */
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10         0x00683918 /* [RW] Video Encoder Color Matrix coefficients c11 and c10 */
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12         0x0068391c /* [RW] Video Encoder Color Matrix coefficients c13 and c12 */
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20         0x00683920 /* [RW] Video Encoder Color Matrix coefficients c21 and c20 */
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22         0x00683924 /* [RW] Video Encoder Color Matrix coefficients c23 and c22 */
#define BCHP_DVI_CSC_0_DITHER_CONTROL            0x00683928 /* [RW] Color Space Converter Dither Control */
#define BCHP_DVI_CSC_0_DITHER_LFSR               0x0068392c /* [RW] Color Space Converter Dither LFSR Control */
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT          0x00683930 /* [RW] Color Space Converter Dither LFSR Init value and control */

/***************************************************************************
 *CSC_REV_ID - Revision ID register
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_REV_ID :: reserved0 [31:16] */
#define BCHP_DVI_CSC_0_CSC_REV_ID_reserved0_MASK                   0xffff0000
#define BCHP_DVI_CSC_0_CSC_REV_ID_reserved0_SHIFT                  16

/* DVI_CSC_0 :: CSC_REV_ID :: REVISION_ID [15:00] */
#define BCHP_DVI_CSC_0_CSC_REV_ID_REVISION_ID_MASK                 0x0000ffff
#define BCHP_DVI_CSC_0_CSC_REV_ID_REVISION_ID_SHIFT                0
#define BCHP_DVI_CSC_0_CSC_REV_ID_REVISION_ID_DEFAULT              0x00003000

/***************************************************************************
 *CSC_MODE - Color space converter mode register
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_MODE :: reserved0 [31:20] */
#define BCHP_DVI_CSC_0_CSC_MODE_reserved0_MASK                     0xfff00000
#define BCHP_DVI_CSC_0_CSC_MODE_reserved0_SHIFT                    20

/* DVI_CSC_0 :: CSC_MODE :: CLAMP_MODE_C2 [19:18] */
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_MASK                 0x000c0000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_SHIFT                18
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_DEFAULT              0x00000000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_MIN_MAX              0
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_RANGE1               1
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_RANGE2               2
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C2_reserved             3

/* DVI_CSC_0 :: CSC_MODE :: CLAMP_MODE_C1 [17:16] */
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_MASK                 0x00030000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_SHIFT                16
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_DEFAULT              0x00000000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_MIN_MAX              0
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_RANGE1               1
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_RANGE2               2
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C1_reserved             3

/* DVI_CSC_0 :: CSC_MODE :: CLAMP_MODE_C0 [15:14] */
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_MASK                 0x0000c000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_SHIFT                14
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_DEFAULT              0x00000000
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_MIN_MAX              0
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_RANGE1               1
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_RANGE2               2
#define BCHP_DVI_CSC_0_CSC_MODE_CLAMP_MODE_C0_reserved             3

/* DVI_CSC_0 :: CSC_MODE :: RANGE2 [13:07] */
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE2_MASK                        0x00003f80
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE2_SHIFT                       7
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE2_DEFAULT                     0x0000007f

/* DVI_CSC_0 :: CSC_MODE :: RANGE1 [06:00] */
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE1_MASK                        0x0000007f
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE1_SHIFT                       0
#define BCHP_DVI_CSC_0_CSC_MODE_RANGE1_DEFAULT                     0x0000005a

/***************************************************************************
 *CSC_MIN_MAX - Color space converter min_max clamp register
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_MIN_MAX :: reserved0 [31:26] */
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_reserved0_MASK                  0xfc000000
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_reserved0_SHIFT                 26

/* DVI_CSC_0 :: CSC_MIN_MAX :: MAX [25:13] */
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MAX_MASK                        0x03ffe000
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MAX_SHIFT                       13
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MAX_DEFAULT                     0x00000fff

/* DVI_CSC_0 :: CSC_MIN_MAX :: MIN [12:00] */
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MIN_MASK                        0x00001fff
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MIN_SHIFT                       0
#define BCHP_DVI_CSC_0_CSC_MIN_MAX_MIN_DEFAULT                     0x00001000

/***************************************************************************
 *CSC_COEFF_C01_C00 - Video Encoder Color Matrix coefficients c01 and c00
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C01_C00 :: COEFF_C1 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C1_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C1_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C1_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C01_C00 :: COEFF_C0 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C0_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C0_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C01_C00_COEFF_C0_DEFAULT          0x00000000

/***************************************************************************
 *CSC_COEFF_C03_C02 - Video Encoder Color Matrix coefficients c03 and c02
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C03_C02 :: COEFF_C3 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C3_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C3_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C3_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C03_C02 :: COEFF_C2 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C2_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C2_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C03_C02_COEFF_C2_DEFAULT          0x00000000

/***************************************************************************
 *CSC_COEFF_C11_C10 - Video Encoder Color Matrix coefficients c11 and c10
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C11_C10 :: COEFF_C1 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C1_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C1_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C1_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C11_C10 :: COEFF_C0 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C0_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C0_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C11_C10_COEFF_C0_DEFAULT          0x00000000

/***************************************************************************
 *CSC_COEFF_C13_C12 - Video Encoder Color Matrix coefficients c13 and c12
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C13_C12 :: COEFF_C3 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C3_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C3_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C3_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C13_C12 :: COEFF_C2 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C2_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C2_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C13_C12_COEFF_C2_DEFAULT          0x00000000

/***************************************************************************
 *CSC_COEFF_C21_C20 - Video Encoder Color Matrix coefficients c21 and c20
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C21_C20 :: COEFF_C1 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C1_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C1_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C1_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C21_C20 :: COEFF_C0 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C0_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C0_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C21_C20_COEFF_C0_DEFAULT          0x00000000

/***************************************************************************
 *CSC_COEFF_C23_C22 - Video Encoder Color Matrix coefficients c23 and c22
 ***************************************************************************/
/* DVI_CSC_0 :: CSC_COEFF_C23_C22 :: COEFF_C3 [31:16] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C3_MASK             0xffff0000
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C3_SHIFT            16
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C3_DEFAULT          0x00000000

/* DVI_CSC_0 :: CSC_COEFF_C23_C22 :: COEFF_C2 [15:00] */
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C2_MASK             0x0000ffff
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C2_SHIFT            0
#define BCHP_DVI_CSC_0_CSC_COEFF_C23_C22_COEFF_C2_DEFAULT          0x00000000

/***************************************************************************
 *DITHER_CONTROL - Color Space Converter Dither Control
 ***************************************************************************/
/* DVI_CSC_0 :: DITHER_CONTROL :: MODE [31:30] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_MASK                    0xc0000000
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_SHIFT                   30
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_DEFAULT                 0x00000000
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_ROUNDING                0
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_TRUNCATE                1
#define BCHP_DVI_CSC_0_DITHER_CONTROL_MODE_DITHER                  2

/* DVI_CSC_0 :: DITHER_CONTROL :: OFFSET_CH2 [29:25] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH2_MASK              0x3e000000
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH2_SHIFT             25
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH2_DEFAULT           0x00000001

/* DVI_CSC_0 :: DITHER_CONTROL :: SCALE_CH2 [24:20] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH2_MASK               0x01f00000
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH2_SHIFT              20
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH2_DEFAULT            0x00000000

/* DVI_CSC_0 :: DITHER_CONTROL :: OFFSET_CH1 [19:15] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH1_MASK              0x000f8000
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH1_SHIFT             15
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH1_DEFAULT           0x00000001

/* DVI_CSC_0 :: DITHER_CONTROL :: SCALE_CH1 [14:10] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH1_MASK               0x00007c00
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH1_SHIFT              10
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH1_DEFAULT            0x00000000

/* DVI_CSC_0 :: DITHER_CONTROL :: OFFSET_CH0 [09:05] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH0_MASK              0x000003e0
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH0_SHIFT             5
#define BCHP_DVI_CSC_0_DITHER_CONTROL_OFFSET_CH0_DEFAULT           0x00000001

/* DVI_CSC_0 :: DITHER_CONTROL :: SCALE_CH0 [04:00] */
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH0_MASK               0x0000001f
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH0_SHIFT              0
#define BCHP_DVI_CSC_0_DITHER_CONTROL_SCALE_CH0_DEFAULT            0x00000000

/***************************************************************************
 *DITHER_LFSR - Color Space Converter Dither LFSR Control
 ***************************************************************************/
/* DVI_CSC_0 :: DITHER_LFSR :: reserved0 [31:11] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved0_MASK                  0xfffff800
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved0_SHIFT                 11

/* DVI_CSC_0 :: DITHER_LFSR :: T2 [10:08] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_MASK                         0x00000700
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_SHIFT                        8
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_DEFAULT                      0x00000000
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_ZERO                         0
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B12                          1
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B13                          2
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B14                          3
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B15                          4
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B16                          5
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B17                          6
#define BCHP_DVI_CSC_0_DITHER_LFSR_T2_B18                          7

/* DVI_CSC_0 :: DITHER_LFSR :: reserved1 [07:07] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved1_MASK                  0x00000080
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved1_SHIFT                 7

/* DVI_CSC_0 :: DITHER_LFSR :: T1 [06:04] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_MASK                         0x00000070
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_SHIFT                        4
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_DEFAULT                      0x00000000
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_ZERO                         0
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B8                           1
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B9                           2
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B10                          3
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B11                          4
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B12                          5
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B13                          6
#define BCHP_DVI_CSC_0_DITHER_LFSR_T1_B14                          7

/* DVI_CSC_0 :: DITHER_LFSR :: reserved2 [03:03] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved2_MASK                  0x00000008
#define BCHP_DVI_CSC_0_DITHER_LFSR_reserved2_SHIFT                 3

/* DVI_CSC_0 :: DITHER_LFSR :: T0 [02:00] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_MASK                         0x00000007
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_SHIFT                        0
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_DEFAULT                      0x00000000
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B2                           0
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B3                           1
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B4                           2
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B6                           3
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B7                           4
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B8                           5
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B9                           6
#define BCHP_DVI_CSC_0_DITHER_LFSR_T0_B10                          7

/***************************************************************************
 *DITHER_LFSR_INIT - Color Space Converter Dither LFSR Init value and control
 ***************************************************************************/
/* DVI_CSC_0 :: DITHER_LFSR_INIT :: reserved0 [31:22] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_reserved0_MASK             0xffc00000
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_reserved0_SHIFT            22

/* DVI_CSC_0 :: DITHER_LFSR_INIT :: SEQ [21:20] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_MASK                   0x00300000
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_SHIFT                  20
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_DEFAULT                0x00000003
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_ONCE                   0
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_ONCE_PER_SOP           1
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_ONCE_PER_2SOP          2
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_SEQ_NEVER                  3

/* DVI_CSC_0 :: DITHER_LFSR_INIT :: VALUE [19:00] */
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_VALUE_MASK                 0x000fffff
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_VALUE_SHIFT                0
#define BCHP_DVI_CSC_0_DITHER_LFSR_INIT_VALUE_DEFAULT              0x00000000

#endif /* #ifndef BCHP_DVI_CSC_0_H__ */

/* End of File */
