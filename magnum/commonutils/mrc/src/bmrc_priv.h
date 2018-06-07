/***************************************************************************
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
 *
 * Module Description:
 *
 ***************************************************************************/

#ifndef BMRC_PRIV_H
#define BMRC_PRIV_H

#include "bchp.h"                /* Chip information */

#ifdef __cplusplus
extern "C" {
#endif


/* chip specific defines */
#if (BCHP_CHIP == 7271) || (BCHP_CHIP == 7268) || \
    (BCHP_CHIP == 7260) || (BCHP_CHIP == 7255) || \
    (BCHP_CHIP == 7211)
#define BMRC_P_MEMC_NUM 1

#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7216)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 129
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7278)
#define BMRC_P_MEMC_NUM 2
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1
#define BMRC_P_INTERRUPT_ARRAY          1

#elif (BCHP_CHIP == 7250)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1


#elif (BCHP_CHIP == 7422)
#define BMRC_P_MEMC_NUM 2
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7425)
#define BMRC_P_MEMC_NUM 2
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7435)
#define BMRC_P_MEMC_NUM 2
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 2
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7445)
#define BMRC_P_MEMC_NUM 3
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 8
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7344)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif ((BCHP_CHIP == 7346) || (BCHP_CHIP == 73465))
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7231)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7584)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 75845)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7438)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 64
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 0
#define BMRC_P_CHECKER_USE_NMBX_ID 1
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif ((BCHP_CHIP == 7563) || (BCHP_CHIP == 75635))
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7358)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7552)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif ((BCHP_CHIP == 7429) || (BCHP_CHIP == 74295))
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7360)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7362) || (BCHP_CHIP == 7228) || (BCHP_CHIP == 73625)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1


#elif (BCHP_CHIP == 7366)
#if (BCHP_VER < BCHP_VER_B0)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#else
#define BMRC_P_MEMC_NUM 2
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#endif
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7543 || BCHP_CHIP == 75525)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 4
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 0
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 0
#define BMRC_P_CLIENTS_MAX 128
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7439) || (BCHP_CHIP == 74371)
#if (BCHP_VER < BCHP_VER_B0)
#define BMRC_P_MEMC_NUM 1
#else
#define BMRC_P_MEMC_NUM 2
#endif
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 8
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1

#elif (BCHP_CHIP == 7364 || BCHP_CHIP==7586)
#define BMRC_P_MEMC_NUM 1
#define BMRC_P_MEMC_0_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_1_CHECKER_COUNT_MAX 8
#define BMRC_P_MEMC_2_CHECKER_COUNT_MAX 8
#define BMRC_P_CLIENTS_MAX 256
#define BMRC_P_CHECKER_USE_MEMC_GEN_VER 0
#define BMRC_P_CHECKER_USE_MEMC_ARC_VER 1
#define BMRC_P_CHECKER_USE_NEW_NAME_SUFFIX 1
#define BMRC_P_CHECKER_USE_NMBX_ID 0
#define BMRC_P_CHECKER_USE_VIOLATION_INFO_CLEAR 1
#else
#error "Platform not supported in MRC."
#endif


/* include platform specific header files */
#if (BMRC_P_CHECKER_USE_MEMC_GEN_VER)
#include "bchp_int_id_memc_l2_0.h"
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#include "bchp_int_id_memc_l2_1_0.h"
#endif

#if (BMRC_P_MEMC_NUM > 1)
#include "bchp_int_id_memc_l2_1.h"
#if (BMRC_P_MEMC_1_CHECKER_COUNT_MAX > 4)
#include "bchp_int_id_memc_l2_1_1.h"
#endif
#endif

#if (BMRC_P_MEMC_NUM > 2)
#include "bchp_int_id_memc_l2_2.h"
#if (BMRC_P_MEMC_2_CHECKER_COUNT_MAX > 4)
#include "bchp_int_id_memc_l2_1_2.h"
#endif
#endif /* #if (BMRC_P_CHECKER_USE_MEMC_GEN_VER) */


#elif (BMRC_P_CHECKER_USE_MEMC_ARC_VER)
#include "bchp_int_id_memc_l2_0_0.h"
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4 && !BMRC_P_INTERRUPT_ARRAY)
#include "bchp_int_id_memc_l2_0_1.h"
#endif

#if (BMRC_P_MEMC_NUM > 1)
#include "bchp_int_id_memc_l2_1_0.h"
#if (BMRC_P_MEMC_1_CHECKER_COUNT_MAX > 4 && !BMRC_P_INTERRUPT_ARRAY)
#include "bchp_int_id_memc_l2_1_1.h"
#endif
#endif

#if (BMRC_P_MEMC_NUM > 2)
#include "bchp_int_id_memc_l2_2_0.h"
#if (BMRC_P_MEMC_1_CHECKER_COUNT_MAX > 4 && !BMRC_P_INTERRUPT_ARRAY)
#include "bchp_int_id_memc_l2_2_1.h"
#endif
#endif /* #if (BMRC_P_CHECKER_USE_MEMC_ARC_VER) */

#endif


#define BMRC_P_CLIENTS_ARRAY_ELEMENT_SIZE 32
#define BMRC_P_CLIENTS_ARRAY_SIZE (BMRC_P_CLIENTS_MAX / BMRC_P_CLIENTS_ARRAY_ELEMENT_SIZE)


/* chip specific ARC INTR configuration */
/* memc 0 */
#if BMRC_P_CHECKER_USE_MEMC_GEN_VER && (BMRC_P_MEMC_NUM > 1)
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_0_ARC_0_INTR
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_0_ARC_1_INTR
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_0_ARC_2_INTR
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_0_ARC_3_INTR
#if (BMRC_P_CHECKER_USE_MEMC_GEN_VER > 1) && (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_0_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_4_INTR
#define BMRC_P_MEMC_0_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_5_INTR
#define BMRC_P_MEMC_0_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_6_INTR
#define BMRC_P_MEMC_0_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_7_INTR
#else
#define BMRC_P_MEMC_0_ARC_4_INTR    0
#define BMRC_P_MEMC_0_ARC_5_INTR    0
#define BMRC_P_MEMC_0_ARC_6_INTR    0
#define BMRC_P_MEMC_0_ARC_7_INTR    0
#endif /*(BMRC_P_CHECKER_USE_MEMC_GEN_VER > 1) && (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4) */

#elif defined(BCHP_INT_ID_ARC_INTR_0)
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_ARC_INTR_0
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_ARC_INTR_1
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_ARC_INTR_2
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_ARC_INTR_3
#define BMRC_P_MEMC_0_ARC_4_INTR    BCHP_INT_ID_ARC_INTR_4
#define BMRC_P_MEMC_0_ARC_5_INTR    BCHP_INT_ID_ARC_INTR_5
#define BMRC_P_MEMC_0_ARC_6_INTR    BCHP_INT_ID_ARC_INTR_6
#define BMRC_P_MEMC_0_ARC_7_INTR    BCHP_INT_ID_ARC_INTR_7
#elif BMRC_P_CHECKER_USE_MEMC_ARC_VER
#if BMRC_P_MEMC_NUM==1 && !BCHP_P_MEMC_INT_ID_GENERIC
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_ARC_0_INTR
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_ARC_1_INTR
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_ARC_2_INTR
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_ARC_3_INTR
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_0_ARC_4_INTR    BCHP_INT_ID_ARC_4_INTR
#define BMRC_P_MEMC_0_ARC_5_INTR    BCHP_INT_ID_ARC_5_INTR
#define BMRC_P_MEMC_0_ARC_6_INTR    BCHP_INT_ID_ARC_6_INTR
#define BMRC_P_MEMC_0_ARC_7_INTR    BCHP_INT_ID_ARC_7_INTR
#else
#define BMRC_P_MEMC_0_ARC_4_INTR    0
#define BMRC_P_MEMC_0_ARC_5_INTR    0
#define BMRC_P_MEMC_0_ARC_6_INTR    0
#define BMRC_P_MEMC_0_ARC_7_INTR    0
#endif
#else
#if BMRC_P_INTERRUPT_ARRAY
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_0
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_1
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_2
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_3
#define BMRC_P_MEMC_0_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_4
#define BMRC_P_MEMC_0_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_5
#define BMRC_P_MEMC_0_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_6
#define BMRC_P_MEMC_0_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_INTR_7
#else /* BMRC_P_INTERRUPT_ARRAY */
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_0_INTR
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_1_INTR
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_2_INTR
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_0_0_ARC_3_INTR
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_0_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_0_1_ARC_4_INTR
#define BMRC_P_MEMC_0_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_0_1_ARC_5_INTR
#define BMRC_P_MEMC_0_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_0_1_ARC_6_INTR
#define BMRC_P_MEMC_0_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_0_1_ARC_7_INTR
#else /*(BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4) */
#define BMRC_P_MEMC_0_ARC_4_INTR    0
#define BMRC_P_MEMC_0_ARC_5_INTR    0
#define BMRC_P_MEMC_0_ARC_6_INTR    0
#define BMRC_P_MEMC_0_ARC_7_INTR    0
#endif /*(BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4) */
#endif  /* BMRC_P_INTERRUPT_ARRAY */
#endif

#else
#define BMRC_P_MEMC_0_ARC_0_INTR    BCHP_INT_ID_ARC_0_INTR
#define BMRC_P_MEMC_0_ARC_1_INTR    BCHP_INT_ID_ARC_1_INTR
#define BMRC_P_MEMC_0_ARC_2_INTR    BCHP_INT_ID_ARC_2_INTR
#define BMRC_P_MEMC_0_ARC_3_INTR    BCHP_INT_ID_ARC_3_INTR
#define BMRC_P_MEMC_0_ARC_4_INTR    0
#define BMRC_P_MEMC_0_ARC_5_INTR    0
#define BMRC_P_MEMC_0_ARC_6_INTR    0
#define BMRC_P_MEMC_0_ARC_7_INTR    0
#endif

/* memc 1 */
#if BMRC_P_INTERRUPT_ARRAY
#define BMRC_P_MEMC_1_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_0
#define BMRC_P_MEMC_1_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_1
#define BMRC_P_MEMC_1_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_2
#define BMRC_P_MEMC_1_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_3
#define BMRC_P_MEMC_1_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_4
#define BMRC_P_MEMC_1_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_5
#define BMRC_P_MEMC_1_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_6
#define BMRC_P_MEMC_1_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_INTR_7
#else /* BMRC_P_INTERRUPT_ARRAY */
#if BMRC_P_CHECKER_USE_MEMC_GEN_VER && (BMRC_P_MEMC_NUM > 1)
#define BMRC_P_MEMC_1_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_1_ARC_0_INTR
#define BMRC_P_MEMC_1_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_1_ARC_1_INTR
#define BMRC_P_MEMC_1_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_1_ARC_2_INTR
#define BMRC_P_MEMC_1_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_1_ARC_3_INTR
#if (BMRC_P_CHECKER_USE_MEMC_GEN_VER > 1) && (BMRC_P_MEMC_1_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_1_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_4_INTR
#define BMRC_P_MEMC_1_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_5_INTR
#define BMRC_P_MEMC_1_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_6_INTR
#define BMRC_P_MEMC_1_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_7_INTR
#else
#define BMRC_P_MEMC_1_ARC_4_INTR    0
#define BMRC_P_MEMC_1_ARC_5_INTR    0
#define BMRC_P_MEMC_1_ARC_6_INTR    0
#define BMRC_P_MEMC_1_ARC_7_INTR    0
#endif

#elif BMRC_P_CHECKER_USE_MEMC_ARC_VER && (BMRC_P_MEMC_NUM > 1)
#define BMRC_P_MEMC_1_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_0_INTR
#define BMRC_P_MEMC_1_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_1_INTR
#define BMRC_P_MEMC_1_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_2_INTR
#define BMRC_P_MEMC_1_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_1_0_ARC_3_INTR
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_1_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_4_INTR
#define BMRC_P_MEMC_1_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_5_INTR
#define BMRC_P_MEMC_1_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_6_INTR
#define BMRC_P_MEMC_1_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_1_1_ARC_7_INTR
#else
#define BMRC_P_MEMC_1_ARC_4_INTR    0
#define BMRC_P_MEMC_1_ARC_5_INTR    0
#define BMRC_P_MEMC_1_ARC_6_INTR    0
#define BMRC_P_MEMC_1_ARC_7_INTR    0
#endif

#elif (BMRC_P_MEMC_NUM > 1)
#error "Port for MEMC 1 interrupts needed."

#else
#define BMRC_P_MEMC_1_ARC_0_INTR    0
#define BMRC_P_MEMC_1_ARC_1_INTR    0
#define BMRC_P_MEMC_1_ARC_2_INTR    0
#define BMRC_P_MEMC_1_ARC_3_INTR    0
#define BMRC_P_MEMC_1_ARC_4_INTR    0
#define BMRC_P_MEMC_1_ARC_5_INTR    0
#define BMRC_P_MEMC_1_ARC_6_INTR    0
#define BMRC_P_MEMC_1_ARC_7_INTR    0
#endif
#endif  /* BMRC_P_INTERRUPT_ARRAY */

/* memc 2 */
#if BMRC_P_CHECKER_USE_MEMC_GEN_VER && (BMRC_P_MEMC_NUM > 2)
#define BMRC_P_MEMC_2_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_2_ARC_0_INTR
#define BMRC_P_MEMC_2_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_2_ARC_1_INTR
#define BMRC_P_MEMC_2_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_2_ARC_2_INTR
#define BMRC_P_MEMC_2_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_2_ARC_3_INTR
#if (BMRC_P_CHECKER_USE_MEMC_GEN_VER > 1) && (BMRC_P_MEMC_2_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_2_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_1_2_ARC_4_INTR
#define BMRC_P_MEMC_2_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_1_2_ARC_5_INTR
#define BMRC_P_MEMC_2_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_1_2_ARC_6_INTR
#define BMRC_P_MEMC_2_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_1_2_ARC_7_INTR
#else
#define BMRC_P_MEMC_2_ARC_4_INTR    0
#define BMRC_P_MEMC_2_ARC_5_INTR    0
#define BMRC_P_MEMC_2_ARC_6_INTR    0
#define BMRC_P_MEMC_2_ARC_7_INTR    0
#endif

#elif BMRC_P_CHECKER_USE_MEMC_ARC_VER && (BMRC_P_MEMC_NUM > 2)
#define BMRC_P_MEMC_2_ARC_0_INTR    BCHP_INT_ID_MEMC_L2_2_0_ARC_0_INTR
#define BMRC_P_MEMC_2_ARC_1_INTR    BCHP_INT_ID_MEMC_L2_2_0_ARC_1_INTR
#define BMRC_P_MEMC_2_ARC_2_INTR    BCHP_INT_ID_MEMC_L2_2_0_ARC_2_INTR
#define BMRC_P_MEMC_2_ARC_3_INTR    BCHP_INT_ID_MEMC_L2_2_0_ARC_3_INTR
#if (BMRC_P_MEMC_0_CHECKER_COUNT_MAX > 4)
#define BMRC_P_MEMC_2_ARC_4_INTR    BCHP_INT_ID_MEMC_L2_2_1_ARC_4_INTR
#define BMRC_P_MEMC_2_ARC_5_INTR    BCHP_INT_ID_MEMC_L2_2_1_ARC_5_INTR
#define BMRC_P_MEMC_2_ARC_6_INTR    BCHP_INT_ID_MEMC_L2_2_1_ARC_6_INTR
#define BMRC_P_MEMC_2_ARC_7_INTR    BCHP_INT_ID_MEMC_L2_2_1_ARC_7_INTR
#else
#define BMRC_P_MEMC_2_ARC_4_INTR    0
#define BMRC_P_MEMC_2_ARC_5_INTR    0
#define BMRC_P_MEMC_2_ARC_6_INTR    0
#define BMRC_P_MEMC_2_ARC_7_INTR    0
#endif

#elif (BMRC_P_MEMC_NUM > 2)
#error "Port for MEMC 2 interrupts needed."

#else
#define BMRC_P_MEMC_2_ARC_0_INTR    0
#define BMRC_P_MEMC_2_ARC_1_INTR    0
#define BMRC_P_MEMC_2_ARC_2_INTR    0
#define BMRC_P_MEMC_2_ARC_3_INTR    0
#define BMRC_P_MEMC_2_ARC_4_INTR    0
#define BMRC_P_MEMC_2_ARC_5_INTR    0
#define BMRC_P_MEMC_2_ARC_6_INTR    0
#define BMRC_P_MEMC_2_ARC_7_INTR    0
#endif


#ifdef __cplusplus
}
#endif

#endif
/* End of File */
