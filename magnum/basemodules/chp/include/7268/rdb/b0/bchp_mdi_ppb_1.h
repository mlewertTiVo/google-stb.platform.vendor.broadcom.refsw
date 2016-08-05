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
 * Date:           Generated on               Mon Jul 18 11:33:03 2016
 *                 Full Compile MD5 Checksum  d473fbf4aefc82fe025f19a353b681d2
 *                     (minus title and desc)
 *                 MD5 Checksum               a04cbea2e001ec837c188fede55d60fb
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1066
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_MDI_PPB_1_H__
#define BCHP_MDI_PPB_1_H__

/***************************************************************************
 *MDI_PPB_1 - MADR Pixel Processing Block Registers
 ***************************************************************************/
#define BCHP_MDI_PPB_1_CONSTANT_COLOR            0x20692800 /* [RW] Constant Color */
#define BCHP_MDI_PPB_1_STATS_RANGE               0x20692804 /* [RW] Statistics Results Collection Range */
#define BCHP_MDI_PPB_1_LA_MAX_ANGLE_STRENGTH     0x20692860 /* [RW] Low Angle Maximum Angle Strength */
#define BCHP_MDI_PPB_1_LA_MIN_NORTH_STRENGTH     0x20692864 /* [RW] Low Angle Minimum North Strength */
#define BCHP_MDI_PPB_1_LA_STRENGTH_CONTROL       0x20692868 /* [RW] Low Angle Strength Control */
#define BCHP_MDI_PPB_1_LA_SCALE_0                0x2069286c /* [RW] Low Angle Strength Scale Set 0 */
#define BCHP_MDI_PPB_1_LA_BC_CONTROL             0x20692874 /* [RW] Low Angle Bound Check Control */
#define BCHP_MDI_PPB_1_XCHROMA_CONTROL_4         0x20692888 /* [RW] Cross Chroma Control Set 4 */
#define BCHP_MDI_PPB_1_IT_REPF_HISTOGRAM_THRESH_0 0x20692894 /* [RW] Inverse Telecine Repeat Field Histogram Threshold Control 0 */
#define BCHP_MDI_PPB_1_IT_REPF_HISTOGRAM_THRESH_1 0x20692898 /* [RW] Inverse Telecine Repeat Field Histogram Threshold Control 1 */
#define BCHP_MDI_PPB_1_IT_PCC_CONTROL            0x2069289c /* [RW] IT PCC Control for IT cadence and Bad Edit */
#define BCHP_MDI_PPB_1_BWV_CHECK                 0x206928ac /* [RW] Bad Weave pattern Checking */
#define BCHP_MDI_PPB_1_DEBUG_MODE                0x206928b0 /* [RW] PPB Debug Mode */
#define BCHP_MDI_PPB_1_STAIR_DETECT_THRESH       0x206928bc /* [RW] Stairstep detection thresholds */
#define BCHP_MDI_PPB_1_MOTION_CAL_CONTROL        0x206928e0 /* [RW] Motion Calculation Control */
#define BCHP_MDI_PPB_1_QM_MAPPING_RANGE          0x206928e4 /* [RW] Motion Histody 2-bit Quantized Motion Mapping RANGE */
#define BCHP_MDI_PPB_1_MH_MAPPING_VALUE          0x206928e8 /* [RW] History Motion Mapping Value */
#define BCHP_MDI_PPB_1_OBTS_CONTROL_0            0x206928ec /* [RW] OBTS Control Set 0 */
#define BCHP_MDI_PPB_1_PIXEL_COMPUTATION_CONTROL 0x206928f0 /* [RW] Pixel Computation Control */
#define BCHP_MDI_PPB_1_BWV_CHECK_MADR_PCC_IT     0x206928f4 /* [RW] Bad Weave pattern Checking Set for MADR PCC-based IT enhancement */
#define BCHP_MDI_PPB_1_SCRATCH_0                 0x20692bfc /* [RW] Scratch register 0 */

#endif /* #ifndef BCHP_MDI_PPB_1_H__ */

/* End of File */
