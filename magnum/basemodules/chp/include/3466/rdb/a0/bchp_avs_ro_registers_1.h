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

#ifndef BCHP_AVS_RO_REGISTERS_1_H__
#define BCHP_AVS_RO_REGISTERS_1_H__

/***************************************************************************
 *AVS_RO_REGISTERS_1 - AVS RO Registers 1 Core
 ***************************************************************************/
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS 0x04812800 /* [RO] Indicate power watchdog failure measurement data and validity of the data */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG 0x04812804 /* [RO] Indicate power watchdogs' status for static monitoring by software */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_1 0x04812808 /* [RO] Indicate power watchdogs' status for static monitoring by software */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_2 0x0481280c /* [RO] Indicate power watchdogs' status for static monitoring by software */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_3 0x04812810 /* [RO] Indicate power watchdogs' status for static monitoring by software */

/***************************************************************************
 *POW_WDOG_FAILURE_STATUS - Indicate power watchdog failure measurement data and validity of the data
 ***************************************************************************/
/* AVS_RO_REGISTERS_1 :: POW_WDOG_FAILURE_STATUS :: reserved0 [31:17] */
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_reserved0_MASK 0xfffe0000
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_reserved0_SHIFT 17

/* AVS_RO_REGISTERS_1 :: POW_WDOG_FAILURE_STATUS :: valid [16:16] */
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_valid_MASK 0x00010000
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_valid_SHIFT 16
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_valid_DEFAULT 0x00000000

/* AVS_RO_REGISTERS_1 :: POW_WDOG_FAILURE_STATUS :: failure_counts [15:00] */
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_failure_counts_MASK 0x0000ffff
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_failure_counts_SHIFT 0
#define BCHP_AVS_RO_REGISTERS_1_POW_WDOG_FAILURE_STATUS_failure_counts_DEFAULT 0x00000000

/***************************************************************************
 *INTERRUPT_STATUS_FAULTY_POW_WDOG - Indicate power watchdogs' status for static monitoring by software
 ***************************************************************************/
/* AVS_RO_REGISTERS_1 :: INTERRUPT_STATUS_FAULTY_POW_WDOG :: good [31:00] */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_good_MASK 0xffffffff
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_good_SHIFT 0
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_good_DEFAULT 0xffffffff

/***************************************************************************
 *INTERRUPT_STATUS_FAULTY_POW_WDOG_1 - Indicate power watchdogs' status for static monitoring by software
 ***************************************************************************/
/* AVS_RO_REGISTERS_1 :: INTERRUPT_STATUS_FAULTY_POW_WDOG_1 :: good [31:00] */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_1_good_MASK 0xffffffff
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_1_good_SHIFT 0
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_1_good_DEFAULT 0xffffffff

/***************************************************************************
 *INTERRUPT_STATUS_FAULTY_POW_WDOG_2 - Indicate power watchdogs' status for static monitoring by software
 ***************************************************************************/
/* AVS_RO_REGISTERS_1 :: INTERRUPT_STATUS_FAULTY_POW_WDOG_2 :: good [31:00] */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_2_good_MASK 0xffffffff
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_2_good_SHIFT 0
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_2_good_DEFAULT 0xffffffff

/***************************************************************************
 *INTERRUPT_STATUS_FAULTY_POW_WDOG_3 - Indicate power watchdogs' status for static monitoring by software
 ***************************************************************************/
/* AVS_RO_REGISTERS_1 :: INTERRUPT_STATUS_FAULTY_POW_WDOG_3 :: good [31:00] */
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_3_good_MASK 0xffffffff
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_3_good_SHIFT 0
#define BCHP_AVS_RO_REGISTERS_1_INTERRUPT_STATUS_FAULTY_POW_WDOG_3_good_DEFAULT 0xffffffff

#endif /* #ifndef BCHP_AVS_RO_REGISTERS_1_H__ */

/* End of File */
