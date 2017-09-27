/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
 *  and may only be used, duplicated, modified or distributed pursuant to the terms and
 *  conditions of a separate, written license agreement executed between you and Broadcom
 *  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 *  no license (express or implied), right to use, or waiver of any kind with respect to the
 *  Software, and Broadcom expressly reserves all rights in and to the Software and all
 *  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 *  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 *  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 *  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 *  and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 *  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 *  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 *  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 *  USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 *  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 *  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 *  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 *  ANY LIMITED REMEDY.
 ******************************************************************************/


#ifndef BCHP_MM_M2MC_CXT_SCH_H__
#define BCHP_MM_M2MC_CXT_SCH_H__

/***************************************************************************
 *MM_M2MC_CXT_SCH
 ***************************************************************************/
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0     0x002e0100 /* [RW][32] Context 0 Weight Register */
#define BCHP_MM_M2MC_CXT_SCH_PACKET_COUNT_CONTEXT0 0x002e0108 /* [RO][32] Packet Count Context0 */
#define BCHP_MM_M2MC_CXT_SCH_SCB_REQ_COUNT_CONTEXT0 0x002e0110 /* [RO][32] SCB Request Count Context0 */
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR           0x002e0118 /* [RW][32] Status Count Clear */
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER          0x002e011c /* [RW][32] Idle Timer */
#define BCHP_MM_M2MC_CXT_SCH_THROTLE_COUNT       0x002e0124 /* [RW][32] Throtle Count */
#define BCHP_MM_M2MC_CXT_SCH_CONTEXT_OBSERVATION_CONTEXT0 0x002e012c /* [RO][32] Balance Observation  Context0 */
#define BCHP_MM_M2MC_CXT_SCH_DEBUG               0x002e0134 /* [RO][32] Debug */

/***************************************************************************
 *WEIGHT_CONTEXT0 - Context 0 Weight Register
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: WEIGHT_CONTEXT0 :: reserved0 [31:04] */
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0_reserved0_MASK        0xfffffff0
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0_reserved0_SHIFT       4

/* MM_M2MC_CXT_SCH :: WEIGHT_CONTEXT0 :: WEIGHT [03:00] */
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0_WEIGHT_MASK           0x0000000f
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0_WEIGHT_SHIFT          0
#define BCHP_MM_M2MC_CXT_SCH_WEIGHT_CONTEXT0_WEIGHT_DEFAULT        0x00000002

/***************************************************************************
 *PACKET_COUNT_CONTEXT0 - Packet Count Context0
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: PACKET_COUNT_CONTEXT0 :: PACKET_COUNT [31:00] */
#define BCHP_MM_M2MC_CXT_SCH_PACKET_COUNT_CONTEXT0_PACKET_COUNT_MASK 0xffffffff
#define BCHP_MM_M2MC_CXT_SCH_PACKET_COUNT_CONTEXT0_PACKET_COUNT_SHIFT 0

/***************************************************************************
 *SCB_REQ_COUNT_CONTEXT0 - SCB Request Count Context0
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: SCB_REQ_COUNT_CONTEXT0 :: SCB_REQ_COUNT [31:00] */
#define BCHP_MM_M2MC_CXT_SCH_SCB_REQ_COUNT_CONTEXT0_SCB_REQ_COUNT_MASK 0xffffffff
#define BCHP_MM_M2MC_CXT_SCH_SCB_REQ_COUNT_CONTEXT0_SCB_REQ_COUNT_SHIFT 0

/***************************************************************************
 *COUNT_CLR - Status Count Clear
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: COUNT_CLR :: reserved0 [31:03] */
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_reserved0_MASK              0xfffffff8
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_reserved0_SHIFT             3

/* MM_M2MC_CXT_SCH :: COUNT_CLR :: SCB_REQ_COUNT_CONTEXT0_CLR [02:02] */
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_SCB_REQ_COUNT_CONTEXT0_CLR_MASK 0x00000004
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_SCB_REQ_COUNT_CONTEXT0_CLR_SHIFT 2

/* MM_M2MC_CXT_SCH :: COUNT_CLR :: reserved1 [01:01] */
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_reserved1_MASK              0x00000002
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_reserved1_SHIFT             1

/* MM_M2MC_CXT_SCH :: COUNT_CLR :: PACKET_COUNT_CONTEXT0_CLR [00:00] */
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_PACKET_COUNT_CONTEXT0_CLR_MASK 0x00000001
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_PACKET_COUNT_CONTEXT0_CLR_SHIFT 0
#define BCHP_MM_M2MC_CXT_SCH_COUNT_CLR_PACKET_COUNT_CONTEXT0_CLR_DEFAULT 0x00000000

/***************************************************************************
 *IDLE_TIMER - Idle Timer
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: IDLE_TIMER :: reserved0 [31:08] */
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER_reserved0_MASK             0xffffff00
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER_reserved0_SHIFT            8

/* MM_M2MC_CXT_SCH :: IDLE_TIMER :: IDLE_TIMER [07:00] */
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER_IDLE_TIMER_MASK            0x000000ff
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER_IDLE_TIMER_SHIFT           0
#define BCHP_MM_M2MC_CXT_SCH_IDLE_TIMER_IDLE_TIMER_DEFAULT         0x00000020

/***************************************************************************
 *THROTLE_COUNT - Throtle Count
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: THROTLE_COUNT :: COUNT [31:00] */
#define BCHP_MM_M2MC_CXT_SCH_THROTLE_COUNT_COUNT_MASK              0xffffffff
#define BCHP_MM_M2MC_CXT_SCH_THROTLE_COUNT_COUNT_SHIFT             0
#define BCHP_MM_M2MC_CXT_SCH_THROTLE_COUNT_COUNT_DEFAULT           0x00000000

/***************************************************************************
 *CONTEXT_OBSERVATION_CONTEXT0 - Balance Observation  Context0
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: CONTEXT_OBSERVATION_CONTEXT0 :: BALANCE [31:00] */
#define BCHP_MM_M2MC_CXT_SCH_CONTEXT_OBSERVATION_CONTEXT0_BALANCE_MASK 0xffffffff
#define BCHP_MM_M2MC_CXT_SCH_CONTEXT_OBSERVATION_CONTEXT0_BALANCE_SHIFT 0

/***************************************************************************
 *DEBUG - Debug
 ***************************************************************************/
/* MM_M2MC_CXT_SCH :: DEBUG :: reserved0 [31:07] */
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_reserved0_MASK                  0xffffff80
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_reserved0_SHIFT                 7

/* MM_M2MC_CXT_SCH :: DEBUG :: TOP_STATE [06:04] */
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_TOP_STATE_MASK                  0x00000070
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_TOP_STATE_SHIFT                 4

/* MM_M2MC_CXT_SCH :: DEBUG :: ARB_STATE [03:02] */
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_ARB_STATE_MASK                  0x0000000c
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_ARB_STATE_SHIFT                 2

/* MM_M2MC_CXT_SCH :: DEBUG :: CURR_SEL [01:00] */
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_CURR_SEL_MASK                   0x00000003
#define BCHP_MM_M2MC_CXT_SCH_DEBUG_CURR_SEL_SHIFT                  0

#endif /* #ifndef BCHP_MM_M2MC_CXT_SCH_H__ */

/* End of File */
