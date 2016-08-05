/********************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom and/or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and Broadcom
 * expressly reserves all rights in and to the Software and all intellectual
 * property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use all
 *    reasonable efforts to protect the confidentiality thereof, and to use
 *    this information only in connection with your use of Broadcom integrated
 *    circuit products.
 *
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
 *    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
 *    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
 *    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 *    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 *    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 * Module Description:
 *                     DO NOT EDIT THIS FILE DIRECTLY
 *
 * This module was generated magically with RDB from a source description
 * file. You must edit the source file for changes to be made to this file.
 *
 * The launch point for all information concerning RDB is found at:
 *   http://bcgbu.broadcom.com/RDB/SitePages/Home.aspx
 *
 * Date:           Generated on               Thu Jun 16 14:47:11 2016
 *                 Full Compile MD5 Checksum  098033a88f172abd8fa618ee2737bdb7
 *                     (minus title and desc)
 *                 MD5 Checksum               d174f8c92909befa902ff630df348d55
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1009
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              /home/pntruong/sbin/combo_header.pl
 *                 DVTSWVER                   n/a
 *
 *
********************************************************************************/

#ifndef BCHP_HSCL_1_H__
#define BCHP_HSCL_1_H__

/***************************************************************************
 *HSCL_1 - BVN_MADR_TOP HSCL Registers
 ***************************************************************************/
#define BCHP_HSCL_1_REVISION_ID                  0x20690400 /* [RO] Revision register */
#define BCHP_HSCL_1_HW_CONFIGURATION             0x20690404 /* [RO] Hardware Configuration Register */
#define BCHP_HSCL_1_TOP_CONTROL                  0x20690408 /* [RW] Top Level Control register */
#define BCHP_HSCL_1_HORIZ_CONTROL                0x2069040c /* [RW] Video Horizontal Scaler Control register */
#define BCHP_HSCL_1_BVB_IN_SIZE                  0x20690410 /* [RW] BVB Input Picture Size Information */
#define BCHP_HSCL_1_PIC_OFFSET                   0x20690414 /* [RW] BVB Input Picture OFFSET Information for 2D or 3D left view */
#define BCHP_HSCL_1_PIC_OFFSET_R                 0x20690418 /* [RW] BVB Input Picture OFFSET Information for 3D right view */
#define BCHP_HSCL_1_SRC_PIC_SIZE                 0x2069041c /* [RW] Source Picture Size Information */
#define BCHP_HSCL_1_DEST_PIC_SIZE                0x20690420 /* [RW] Destination Picture Size Information */
#define BCHP_HSCL_1_SRC_PIC_HORIZ_PAN_SCAN       0x20690424 /* [RW] Source Picture Horizontal Pan/Scan Information */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_SRC_PIC_OFFSET 0x20690428 /* [RW] Horizontal Poly-Phase Filter Source Picture Luma Offset */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_SRC_PIC_OFFSET 0x2069042c /* [RW] Horizontal Poly-Phase Filter Source Picture Chroma Offset */
#define BCHP_HSCL_1_HORIZ_FIR_INIT_PHASE_ACC     0x20690430 /* [RW] Horizontal Poly-Phase Filter Inital Phase Accumulate Value */
#define BCHP_HSCL_1_HORIZ_FIR_INIT_PHASE_ACC_R   0x20690434 /* [RW] Horizontal Poly-Phase Filter Initial Phase Accumulate Value */
#define BCHP_HSCL_1_HORIZ_FIR_INIT_STEP_FRAC     0x20690438 /* [RW] Horizontal Poly-Phase Filter Initial Stepping Size (fractional part) for Region 0 */
#define BCHP_HSCL_1_HORIZ_FIR_INIT_STEP_INT      0x2069043c /* [RW] Horizontal Poly-Phase Filter Initial Stepping Size (integer part) for Region 0 */
#define BCHP_HSCL_1_HORIZ_FIR_DEST_PIC_REGION_0_STEP_DELTA 0x20690440 /* [RW] Horizontal Poly-Phase Filter Picture Delta Increment for Region 0 Stepping Size */
#define BCHP_HSCL_1_HORIZ_FIR_DEST_PIC_REGION_2_STEP_DELTA 0x20690444 /* [RW] Horizontal Poly-Phase Filter Picture Delta Increment for Region 2 Stepping Size */
#define BCHP_HSCL_1_HORIZ_DEST_PIC_REGION_N1_END 0x20690448 /* [RW] Horizontal Poly-Phase Filter Destination Region N1 Ending Position */
#define BCHP_HSCL_1_HORIZ_DEST_PIC_REGION_0_END  0x2069044c /* [RW] Horizontal Poly-Phase Filter Destination Region 0 Ending Position */
#define BCHP_HSCL_1_HORIZ_DEST_PIC_REGION_1_END  0x20690450 /* [RW] Horizontal Poly-Phase Filter Destination Region 1 Ending Position */
#define BCHP_HSCL_1_HORIZ_DEST_PIC_REGION_2_END  0x20690454 /* [RW] Horizontal Poly-Phase Filter Destination Region 2 Ending Position */
#define BCHP_HSCL_1_HORIZ_DEST_PIC_REGION_3_END  0x20690458 /* [RW] Horizontal Poly-Phase Filter Destination Region 3 Ending Position */
#define BCHP_HSCL_1_HORIZ_DITHER_LFSR_INIT       0x2069045c /* [RW] Dither LFSR Init value and control */
#define BCHP_HSCL_1_HORIZ_DITHER_LFSR_CTRL       0x20690460 /* [RW] Dither LFSR Control */
#define BCHP_HSCL_1_HORIZ_DITHER_CTRL            0x20690464 /* [RW] Dither Control */
#define BCHP_HSCL_1_DERINGING                    0x20690468 /* [RW] Horizontal only Scaler Deringing Control */
#define BCHP_HSCL_1_VIDEO_3D_MODE                0x2069046c /* [RW] Video 3D mode */
#define BCHP_HSCL_1_CCA_CONFIG                   0x20690470 /* [RW] Cb/Cr Amplifier Configuration */
#define BCHP_HSCL_1_ENABLE                       0x20690474 /* [RW] Enable */
#define BCHP_HSCL_1_TEST_PORT_CONTROL            0x20690478 /* [RW] Testportl control register */
#define BCHP_HSCL_1_TEST_PORT_DATA               0x2069047c /* [RO] Testport data register */
#define BCHP_HSCL_1_SCRATCH_0                    0x20690480 /* [RW] Scratch register 0 */
#define BCHP_HSCL_1_SCRATCH_1                    0x20690484 /* [RW] Scratch register 1 */
#define BCHP_HSCL_1_BVB_IN_STATUS_CLEAR          0x20690488 /* [WO] Broadcom Video Bus Input Status Clear */
#define BCHP_HSCL_1_BVB_IN_STATUS                0x2069048c /* [RO] Broadcom Video Bus Input Status */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE0_00_01 0x20690740 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE0_02_03 0x20690744 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE0_04_05 0x20690748 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE1_00_01 0x2069074c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE1_02_03 0x20690750 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE1_04_05 0x20690754 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE2_00_01 0x20690758 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE2_02_03 0x2069075c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE2_04_05 0x20690760 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE3_00_01 0x20690764 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE3_02_03 0x20690768 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE3_04_05 0x2069076c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE4_00_01 0x20690770 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE4_02_03 0x20690774 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE4_04_05 0x20690778 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE5_00_01 0x2069077c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE5_02_03 0x20690780 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE5_04_05 0x20690784 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE6_00_01 0x20690788 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE6_02_03 0x2069078c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE6_04_05 0x20690790 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE7_00_01 0x20690794 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 0 and 1 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE7_02_03 0x20690798 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 2 and 3 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_LUMA_COEFF_PHASE7_04_05 0x2069079c /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 4 and 5 LUMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_00_01 0x206907a0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_02_03 0x206907a4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE0_04_05 0x206907a8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 0 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_00_01 0x206907ac /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_02_03 0x206907b0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE1_04_05 0x206907b4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 1 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_00_01 0x206907b8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_02_03 0x206907bc /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE2_04_05 0x206907c0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 2 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_00_01 0x206907c4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_02_03 0x206907c8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE3_04_05 0x206907cc /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 3 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_00_01 0x206907d0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_02_03 0x206907d4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE4_04_05 0x206907d8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 4 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_00_01 0x206907dc /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_02_03 0x206907e0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE5_04_05 0x206907e4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 5 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_00_01 0x206907e8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_02_03 0x206907ec /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE6_04_05 0x206907f0 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 6 Tap 4 and 5 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_00_01 0x206907f4 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 0 and 1 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_02_03 0x206907f8 /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 2 and 3 CHROMA Coefficients */
#define BCHP_HSCL_1_HORIZ_FIR_CHROMA_COEFF_PHASE7_04_05 0x206907fc /* [RW] Horizontal Scaler 12-tap Poly-Phase Filter Phase 7 Tap 4 and 5 CHROMA Coefficients */

#endif /* #ifndef BCHP_HSCL_1_H__ */

/* End of File */
