/******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/

#pragma once

#include "fp_sdk_config.h"

typedef enum __attribute__((packed)) {
    TX_REG_ID_r0  = 0,
    TX_REG_ID_r1  = 1,
    TX_REG_ID_r2  = 2,
    TX_REG_ID_r3  = 3,
    TX_REG_ID_r4  = 4,
    TX_REG_ID_r5  = 5,
    TX_REG_ID_r6  = 6,
    TX_REG_ID_r7  = 7,
    TX_REG_ID_r8  = 8,
    TX_REG_ID_r9  = 9,
    TX_REG_ID_r10 = 10,
    TX_REG_ID_r11 = 11,
    TX_REG_ID_r12 = 12,
    TX_REG_ID_r13 = 13,
    TX_REG_ID_r14 = 14,
    TX_REG_ID_r15 = 15,
    TX_REG_ID_r16 = 16,
    TX_REG_ID_r17 = 17,
    TX_REG_ID_r18 = 18,
    TX_REG_ID_r19 = 19,
    TX_REG_ID_r20 = 20,
    TX_REG_ID_r21 = 21,
    TX_REG_ID_r22 = 22,
    TX_REG_ID_r23 = 23,
    TX_REG_ID_r24 = 24,
    TX_REG_ID_r25 = 25,
    TX_REG_ID_r26 = 26,
    TX_REG_ID_r27 = 27,
    TX_REG_ID_r28 = 28,
    TX_REG_ID_r29 = 29,
    TX_REG_ID_r30 = 30,
    TX_REG_ID_r31 = 31,
    TX_REG_ID_r32 = 32,
    TX_REG_ID_r33 = 33,
    TX_REG_ID_r34 = 34,
    TX_REG_ID_r35 = 35,
    TX_REG_ID_r36 = 36,
    TX_REG_ID_r37 = 37,
    TX_REG_ID_r38 = 38,
    TX_REG_ID_r39 = 39,
    TX_REG_ID_r40 = 40,
    TX_REG_ID_r41 = 41,
    TX_REG_ID_r42 = 42,
    TX_REG_ID_r43 = 43,
    TX_REG_ID_r44 = 44,
    TX_REG_ID_r45 = 45,
    TX_REG_ID_r46 = 46,
    TX_REG_ID_r47 = 47,
    TX_REG_ID_r48 = 48,
    TX_REG_ID_r49 = 49,
    TX_REG_ID_r50 = 50,
    TX_REG_ID_r51 = 51,
    TX_REG_ID_r52 = 52,
    TX_REG_ID_r53 = 53,
    TX_REG_ID_r54 = 54,
    TX_REG_ID_r55 = 55,
    TX_REG_ID_r56 = 56,
    TX_REG_ID_r57 = 57,
    TX_REG_ID_r58 = 58,
    TX_REG_ID_r59 = 59,
    TX_REG_ID_r60 = 60,
    TX_REG_ID_r61 = 61,
    TX_REG_ID_p0,
    TX_REG_ID_p1,
    TX_REG_ID_p2,
    TX_REG_ID_p3,
    TX_REG_ID_pc,
    TX_REG_ID_loop_match, /**< DIR_LOOP_MATCH */
    TX_REG_ID_loop_count, /**< DIR_LOOP_COUNT */
    TX_REG_ID_loop_start, /**< DIR_LOOP_START */
    TX_REG_ID_SHADOW_0,   /**< DIR_SHADOW_REG_0 */
    TX_REG_ID_SHADOW_1,   /**< DIR_SHADOW_REG_1 */
    TX_REG_LAST_READ_WRITE_ID = TX_REG_ID_SHADOW_1,
    /* Read-only registers. */
    /* Keep these contiguous with TX_REG_ID_SHADOW_1 for convenience. */
    TX_REG_ID_SHADOW_2,   /**< DIR_SHADOW_REG_2 */
    TX_REG_ID_SHADOW_3,   /**< DIR_SHADOW_REG_3 */
    TX_REG_ID_usrc,       /**< DIR_USRC */
    TX_NUM_REGISTER_IDS,
    /* Some useful constants */
    TX_REG_ID_sp = TX_REG_ID_r57,
    TX_REG_ID_link = TX_REG_ID_r58
} TX_REGISTER_ID;
