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
 * Date:           Generated on              Mon Dec 23 13:19:23 2013
 *                 Full Compile MD5 Checksum e5d1378cc1475b750905e70cb70c73d9
 *                   (minus title and desc)  
 *                 MD5 Checksum              aa943f3142a624837db5321711723fcf
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

#ifndef BCHP_XSRC_1_H__
#define BCHP_XSRC_1_H__

/***************************************************************************
 *XSRC_1 - Cross-Source 1 Registers
 ***************************************************************************/
#define BCHP_XSRC_1_REVISION_ID                  0x00624c00 /* Revision register */
#define BCHP_XSRC_1_HW_CONFIGURATION             0x00624c04 /* Hardware Configuration Register */
#define BCHP_XSRC_1_TOP_CONTROL                  0x00624c08 /* Top Level Control register */
#define BCHP_XSRC_1_HORIZ_CONTROL                0x00624c0c /* Video Horizontal Scaler Control register */
#define BCHP_XSRC_1_BVB_IN_SIZE                  0x00624c10 /* BVB Input Picture Size Information */
#define BCHP_XSRC_1_PIC_OFFSET                   0x00624c14 /* BVB Input Picture OFFSET Information for 2D or 3D left view */
#define BCHP_XSRC_1_PIC_OFFSET_R                 0x00624c18 /* BVB Input Picture OFFSET Information for 3D right view */
#define BCHP_XSRC_1_SRC_PIC_SIZE                 0x00624c1c /* Source Picture Size Information */
#define BCHP_XSRC_1_DEST_PIC_SIZE                0x00624c20 /* Destination Picture Size Information */
#define BCHP_XSRC_1_SRC_PIC_HORIZ_PAN_SCAN       0x00624c24 /* Source Picture Horizontal Pan/Scan Information */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_SRC_PIC_OFFSET 0x00624c28 /* Horizontal Poly-Phase Filter Source Picture Luma Offset */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_SRC_PIC_OFFSET 0x00624c2c /* Horizontal Poly-Phase Filter Source Picture Chroma Offset */
#define BCHP_XSRC_1_HORIZ_FIR_INIT_PHASE_ACC     0x00624c30 /* Horizontal Poly-Phase Filter Inital Phase Accumulate Value */
#define BCHP_XSRC_1_HORIZ_FIR_INIT_PHASE_ACC_R   0x00624c34 /* Horizontal Poly-Phase Filter Initial Phase Accumulate Value */
#define BCHP_XSRC_1_HORIZ_FIR_INIT_STEP_FRAC     0x00624c38 /* Horizontal Poly-Phase Filter Initial Stepping Size (fractional part) for Region 0 */
#define BCHP_XSRC_1_HORIZ_FIR_INIT_STEP_INT      0x00624c3c /* Horizontal Poly-Phase Filter Initial Stepping Size (integer part) for Region 0 */
#define BCHP_XSRC_1_HORIZ_FIR_DEST_PIC_REGION_0_STEP_DELTA 0x00624c40 /* Horizontal Poly-Phase Filter Picture Delta Increment for Region 0 Stepping Size */
#define BCHP_XSRC_1_HORIZ_FIR_DEST_PIC_REGION_2_STEP_DELTA 0x00624c44 /* Horizontal Poly-Phase Filter Picture Delta Increment for Region 2 Stepping Size */
#define BCHP_XSRC_1_HORIZ_DEST_PIC_REGION_N1_END 0x00624c48 /* Horizontal Poly-Phase Filter Destination Region N1 Ending Position */
#define BCHP_XSRC_1_HORIZ_DEST_PIC_REGION_0_END  0x00624c4c /* Horizontal Poly-Phase Filter Destination Region 0 Ending Position */
#define BCHP_XSRC_1_HORIZ_DEST_PIC_REGION_1_END  0x00624c50 /* Horizontal Poly-Phase Filter Destination Region 1 Ending Position */
#define BCHP_XSRC_1_HORIZ_DEST_PIC_REGION_2_END  0x00624c54 /* Horizontal Poly-Phase Filter Destination Region 2 Ending Position */
#define BCHP_XSRC_1_HORIZ_DEST_PIC_REGION_3_END  0x00624c58 /* Horizontal Poly-Phase Filter Destination Region 3 Ending Position */
#define BCHP_XSRC_1_HORIZ_DITHER_LFSR_INIT       0x00624c5c /* Dither LFSR Init value and control */
#define BCHP_XSRC_1_HORIZ_DITHER_LFSR_CTRL       0x00624c60 /* Dither LFSR Control */
#define BCHP_XSRC_1_HORIZ_DITHER_CTRL            0x00624c64 /* Dither Control */
#define BCHP_XSRC_1_DERINGING                    0x00624c68 /* Horizontal only Scaler Deringing Control */
#define BCHP_XSRC_1_VIDEO_3D_MODE                0x00624c6c /* Video 3D mode */
#define BCHP_XSRC_1_ENABLE                       0x00624c70 /* Enable */
#define BCHP_XSRC_1_TEST_PORT_CONTROL            0x00624c74 /* Testportl control register */
#define BCHP_XSRC_1_TEST_PORT_DATA               0x00624c78 /* Testport data register */
#define BCHP_XSRC_1_SCRATCH_0                    0x00624c7c /* Scratch register 0 */
#define BCHP_XSRC_1_SCRATCH_1                    0x00624c80 /* Scratch register 1 */
#define BCHP_XSRC_1_BVB_IN_STATUS_CLEAR          0x00624c84 /* Broadcom Video Bus Input Status Clear */
#define BCHP_XSRC_1_BVB_IN_STATUS                0x00624c88 /* Broadcom Video Bus Input Status */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE0_00_01 0x00624f40 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE0_02_03 0x00624f44 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE0_04_05 0x00624f48 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE1_00_01 0x00624f4c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE1_02_03 0x00624f50 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE1_04_05 0x00624f54 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE2_00_01 0x00624f58 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE2_02_03 0x00624f5c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE2_04_05 0x00624f60 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE3_00_01 0x00624f64 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE3_02_03 0x00624f68 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE3_04_05 0x00624f6c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE4_00_01 0x00624f70 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE4_02_03 0x00624f74 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE4_04_05 0x00624f78 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE5_00_01 0x00624f7c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE5_02_03 0x00624f80 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE5_04_05 0x00624f84 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE6_00_01 0x00624f88 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE6_02_03 0x00624f8c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE6_04_05 0x00624f90 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE7_00_01 0x00624f94 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 0 and 1 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE7_02_03 0x00624f98 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 2 and 3 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_LUMA_COEFF_PHASE7_04_05 0x00624f9c /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 4 and 5 LUMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_00_01 0x00624fa0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_02_03 0x00624fa4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_04_05 0x00624fa8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_00_01 0x00624fac /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_02_03 0x00624fb0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_04_05 0x00624fb4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_00_01 0x00624fb8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_02_03 0x00624fbc /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_04_05 0x00624fc0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_00_01 0x00624fc4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_02_03 0x00624fc8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_04_05 0x00624fcc /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_00_01 0x00624fd0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_02_03 0x00624fd4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_04_05 0x00624fd8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_00_01 0x00624fdc /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_02_03 0x00624fe0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_04_05 0x00624fe4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_00_01 0x00624fe8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_02_03 0x00624fec /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_04_05 0x00624ff0 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_00_01 0x00624ff4 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_02_03 0x00624ff8 /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_XSRC_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_04_05 0x00624ffc /* Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 4 and 5 CHROMA Coefficients */

#endif /* #ifndef BCHP_XSRC_1_H__ */

/* End of File */
