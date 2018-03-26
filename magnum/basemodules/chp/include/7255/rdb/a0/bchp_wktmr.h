/********************************************************************************
 * Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
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
 * Date:           Generated on               Tue Mar 14 11:30:18 2017
 *                 Full Compile MD5 Checksum  139982b678394685d13b1997fb7229b7
 *                     (minus title and desc)
 *                 MD5 Checksum               a3969ecea7c60b5dda53c39c3e5f90df
 *
 * lock_release:   r_1255
 * Compiled with:  RDB Utility                combo_header.pl
 *                 RDB.pm                     1255
 *                 unknown                    unknown
 *                 Perl Interpreter           5.014001
 *                 Operating System           linux
 *                 Script Source              tools/dvtsw/r_1255/Linux/combo_header.pl
 *                 DVTSWVER                   LOCAL
 *
 *
********************************************************************************/

#ifndef BCHP_WKTMR_H__
#define BCHP_WKTMR_H__

/***************************************************************************
 *WKTMR - Wakeup timer
 ***************************************************************************/
#define BCHP_WKTMR_EVENT                         0x20417e40 /* [RW][32] Wakeup Timer Register */
#define BCHP_WKTMR_COUNTER                       0x20417e44 /* [RW][32] Wakeup Counter */
#define BCHP_WKTMR_ALARM                         0x20417e48 /* [RW][32] Wakeup Timer Alarm */
#define BCHP_WKTMR_PRESCALER                     0x20417e4c /* [RW][32] Wakeup Timer Prescaler */
#define BCHP_WKTMR_PRESCALER_VAL                 0x20417e50 /* [RO][32] Wakeup Timer Prescaler Value */

/***************************************************************************
 *EVENT - Wakeup Timer Register
 ***************************************************************************/
/* WKTMR :: EVENT :: reserved0 [31:01] */
#define BCHP_WKTMR_EVENT_reserved0_MASK                            0xfffffffe
#define BCHP_WKTMR_EVENT_reserved0_SHIFT                           1

/* WKTMR :: EVENT :: wktmr_alarm_event [00:00] */
#define BCHP_WKTMR_EVENT_wktmr_alarm_event_MASK                    0x00000001
#define BCHP_WKTMR_EVENT_wktmr_alarm_event_SHIFT                   0
#define BCHP_WKTMR_EVENT_wktmr_alarm_event_DEFAULT                 0x00000000

/***************************************************************************
 *COUNTER - Wakeup Counter
 ***************************************************************************/
/* WKTMR :: COUNTER :: wktmr_counter [31:00] */
#define BCHP_WKTMR_COUNTER_wktmr_counter_MASK                      0xffffffff
#define BCHP_WKTMR_COUNTER_wktmr_counter_SHIFT                     0
#define BCHP_WKTMR_COUNTER_wktmr_counter_DEFAULT                   0x00000000

/***************************************************************************
 *ALARM - Wakeup Timer Alarm
 ***************************************************************************/
/* WKTMR :: ALARM :: wktmr_alarm [31:00] */
#define BCHP_WKTMR_ALARM_wktmr_alarm_MASK                          0xffffffff
#define BCHP_WKTMR_ALARM_wktmr_alarm_SHIFT                         0
#define BCHP_WKTMR_ALARM_wktmr_alarm_DEFAULT                       0x00000000

/***************************************************************************
 *PRESCALER - Wakeup Timer Prescaler
 ***************************************************************************/
/* WKTMR :: PRESCALER :: reserved0 [31:25] */
#define BCHP_WKTMR_PRESCALER_reserved0_MASK                        0xfe000000
#define BCHP_WKTMR_PRESCALER_reserved0_SHIFT                       25

/* WKTMR :: PRESCALER :: wktmr_prescaler [24:00] */
#define BCHP_WKTMR_PRESCALER_wktmr_prescaler_MASK                  0x01ffffff
#define BCHP_WKTMR_PRESCALER_wktmr_prescaler_SHIFT                 0
#define BCHP_WKTMR_PRESCALER_wktmr_prescaler_DEFAULT               0x019bfcc0

/***************************************************************************
 *PRESCALER_VAL - Wakeup Timer Prescaler Value
 ***************************************************************************/
/* WKTMR :: PRESCALER_VAL :: reserved0 [31:25] */
#define BCHP_WKTMR_PRESCALER_VAL_reserved0_MASK                    0xfe000000
#define BCHP_WKTMR_PRESCALER_VAL_reserved0_SHIFT                   25

/* WKTMR :: PRESCALER_VAL :: wktmr_prescaler_val [24:00] */
#define BCHP_WKTMR_PRESCALER_VAL_wktmr_prescaler_val_MASK          0x01ffffff
#define BCHP_WKTMR_PRESCALER_VAL_wktmr_prescaler_val_SHIFT         0
#define BCHP_WKTMR_PRESCALER_VAL_wktmr_prescaler_val_DEFAULT       0x00000000

#endif /* #ifndef BCHP_WKTMR_H__ */

/* End of File */
