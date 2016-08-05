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
 *
 * Date:           Generated on               Tue May  3 15:23:06 2016
 *                 Full Compile MD5 Checksum  1da1c3226a9ac5d06cc27c06c1aefe52
 *                     (minus title and desc)
 *                 MD5 Checksum               a58ec1309d39852a0c8fb54050da5f77
 *
 * lock_release:   n/a
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     930
 *                 unknown                    unknown
 *                 Perl Interpreter           5.008008
 *                 Operating System           linux
 *                 Script Source              /tools/dvtsw/current/Linux/combo_header.pl
 *                 DVTSWVER                   current
 *
 *
********************************************************************************/

#ifndef BCHP_UFE_MISC_1_H__
#define BCHP_UFE_MISC_1_H__

/***************************************************************************
 *UFE_MISC_1 - UFE Misc Register Set (including DPM, AAGC, IRQ, TP)
 ***************************************************************************/
#define BCHP_UFE_MISC_1_REVID                    0x04100600 /* [RO] Revision ID Register */
#define BCHP_UFE_MISC_1_CTRL                     0x04100604 /* [RW] Master clkgen/datapath resets & Misc control bits for final VID/HBU */
#define BCHP_UFE_MISC_1_CTRL_SEL                 0x0410062c /* [RW] DPM/CLIP/AAGC input source select */
#define BCHP_UFE_MISC_1_DPM                      0x04100630 /* [RW] DPM register for control signals */
#define BCHP_UFE_MISC_1_DPM_DCI                  0x04100634 /* [RW] DPM DC Canceller I Integrator Value */
#define BCHP_UFE_MISC_1_DPM_DCQ                  0x04100638 /* [RW] DPM DC Canceller Q Integrator Value */
#define BCHP_UFE_MISC_1_DPMK2                    0x0410063c /* [RW] DPM Register for Control Signals */
#define BCHP_UFE_MISC_1_DPM_DCK2I                0x04100640 /* [RW] DPM DC Canceller I Integrator Value */
#define BCHP_UFE_MISC_1_DPM_DCK2Q                0x04100644 /* [RW] DPM DC Canceller Q Integrator Value */
#define BCHP_UFE_MISC_1_CLIP_CTRL                0x04100648 /* [RW] Clip Detector Control Register */
#define BCHP_UFE_MISC_1_CLIP_THRI                0x0410064c /* [RW] Clip Detector Threshold Value for Channel I */
#define BCHP_UFE_MISC_1_CLIP_THRQ                0x04100650 /* [RW] Clip Detector Threshold Value for Channel Q */
#define BCHP_UFE_MISC_1_CLIP_CNTI                0x04100654 /* [RW] Clip Detector Counter Value for Channel I */
#define BCHP_UFE_MISC_1_CLIP_CNTQ                0x04100658 /* [RW] Clip Detector Counter Value for Channel Q */
#define BCHP_UFE_MISC_1_AGC_CTRL                 0x0410065c /* [RW] Analog AGC Control Register */
#define BCHP_UFE_MISC_1_AGC_THRES                0x04100660 /* [RW] Analog AGC Loop Threshold */
#define BCHP_UFE_MISC_1_AGC_DSFIX                0x04100664 /* [RW] Analog AGC Delta Sigma Fixed Control Word */
#define BCHP_UFE_MISC_1_AGC_INT_UT               0x04100668 /* [RW] Analog AGC Integrator Upper Limit */
#define BCHP_UFE_MISC_1_AGC_INT_LT               0x0410066c /* [RW] Analog AGC Integrator Lower Limit */
#define BCHP_UFE_MISC_1_AGC_INT                  0x04100670 /* [RW] Analog AGC Integrator */
#define BCHP_UFE_MISC_1_AGC_INT_DS               0x04100674 /* [RO] Analog AGC Delta Sigma Integrator */
#define BCHP_UFE_MISC_1_AGC_STS                  0x04100678 /* [RO] Analog AGC Status Register */
#define BCHP_UFE_MISC_1_CRC_EN                   0x04100694 /* [RW] CRC enable register */
#define BCHP_UFE_MISC_1_CRC                      0x04100698 /* [RO] CRC signature analyzer register */
#define BCHP_UFE_MISC_1_TP                       0x0410069c /* [RW] Master TP control */
#define BCHP_UFE_MISC_1_TP2                      0x041006a0 /* [RW] Master TP control2 */
#define BCHP_UFE_MISC_1_TP_THRES                 0x041006a4 /* [RW] Threshold levels for testport output comparator */
#define BCHP_UFE_MISC_1_TP_PWR                   0x041006a8 /* [RO] Avgerage power selected testport output (only for 540 MHz signals) */
#define BCHP_UFE_MISC_1_RC_CONFIG                0x041006d0 /* [RW] RC count configuration */
#define BCHP_UFE_MISC_1_RC_STATUS                0x041006d4 /* [RO] RC count status */
#define BCHP_UFE_MISC_1_TPCAP_CTRL               0x041006d8 /* [RW] TP capture control */
#define BCHP_UFE_MISC_1_TPCAP_STATUS             0x041006dc /* [RO] TP capture status */

#endif /* #ifndef BCHP_UFE_MISC_1_H__ */

/* End of File */
