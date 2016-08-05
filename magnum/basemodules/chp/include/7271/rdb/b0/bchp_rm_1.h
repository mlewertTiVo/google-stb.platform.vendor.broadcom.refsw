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
 * Date:           Generated on               Thu Jun 16 14:47:13 2016
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

#ifndef BCHP_RM_1_H__
#define BCHP_RM_1_H__

/***************************************************************************
 *RM_1 - Rate Manger RM_1
 ***************************************************************************/
#define BCHP_RM_1_CONTROL                        0x206e2440 /* [RW] Rate Manager Controls */
#define BCHP_RM_1_RATE_RATIO                     0x206e2444 /* [RW] Rate Manager Output Rate Setting I */
#define BCHP_RM_1_SAMPLE_INC                     0x206e2448 /* [RW] Rate Manager Output Rate Setting II */
#define BCHP_RM_1_PHASE_INC                      0x206e244c /* [RW] Rate Manager NCO Phase Increment */
#define BCHP_RM_1_INTEGRATOR_HI                  0x206e2450 /* [WO] Loop Filter Integrator HI Value */
#define BCHP_RM_1_INTEGRATOR_LO                  0x206e2454 /* [WO] Loop Filter Integrator LO Value */
#define BCHP_RM_1_OFFSET                         0x206e2458 /* [RW] Rate Manager Output Formatting */
#define BCHP_RM_1_FORMAT                         0x206e245c /* [RW] Rate Manager Output Formatting */
#define BCHP_RM_1_SKIP_REPEAT_CONTROL            0x206e2460 /* [RW] Time Base Skip or Repeat Control Register */
#define BCHP_RM_1_SKIP_REPEAT_GAP                0x206e2464 /* [RW] Time Base Skip or Repeat Gap Count Register */
#define BCHP_RM_1_SKIP_REPEAT_NUMBER             0x206e2468 /* [RW] Time Base Skip or Repeat Count Register */
#define BCHP_RM_1_INTEGRATOR_HI_STATUS           0x206e246c /* [RO] Loop Filter Integrator HI Status */
#define BCHP_RM_1_INTEGRATOR_LO_STATUS           0x206e2470 /* [RO] Loop Filter Integrator LO Status */
#define BCHP_RM_1_STATUS                         0x206e2474 /* [RO] Rate Manager Status Register */
#define BCHP_RM_1_STATUS_CLEAR                   0x206e2478 /* [RW] Rate Manager Status Clear Register */

#endif /* #ifndef BCHP_RM_1_H__ */

/* End of File */
