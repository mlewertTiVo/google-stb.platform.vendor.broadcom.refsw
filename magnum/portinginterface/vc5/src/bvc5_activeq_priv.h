/***************************************************************************
 *     (c)2015 Broadcom Corporation
 *
 *  This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
 *
 **************************************************************************/

#ifndef BVC5_ACTIVEQ_H__
#define BVC5_ACTIVEQ_H__

#include "bvc5.h"
#include "bvc5_internal_job_priv.h"
#include "blst_queue.h"

typedef struct BVC5_P_ActiveQ *BVC5_ActiveQHandle;

/***************************************************************************/

BERR_Code BVC5_P_ActiveQCreate(
   BVC5_ActiveQHandle *phActiveQ
);

/***************************************************************************/

BERR_Code BVC5_P_ActiveQDestroy(
   BVC5_ActiveQHandle hActiveQ
);

/***************************************************************************/

void BVC5_P_ActiveQInsertTail(
   BVC5_ActiveQHandle   hActiveQ,
   BVC5_P_InternalJob  *psJob
);

/***************************************************************************/

void BVC5_P_ActiveQInsertHead(
   BVC5_ActiveQHandle      hActiveQ,
   BVC5_P_InternalJob  *psJob
);

/***************************************************************************/

void BVC5_P_ActiveQRemove(
   BVC5_ActiveQHandle      hActiveQ,
   BVC5_P_InternalJob  *psJob
);

/***************************************************************************/

/* BVC5_P_ActiveQFindById
 * Removes a job given its id.  Returns the job.
 */
BVC5_P_InternalJob *BVC5_P_ActiveQFindById(
   BVC5_ActiveQHandle   hActiveQ,
   uint64_t          uiJobId
);

/***************************************************************************/

/* BVC5_P_ActiveQRemoveById
 * Removes a job given its id.  Returns the job.
 */
BVC5_P_InternalJob *BVC5_P_ActiveQRemoveById(
   BVC5_ActiveQHandle   hActiveQ,
   uint64_t          uiJobId
);

/***************************************************************************/

bool BVC5_P_ActiveQContainsId(
   BVC5_ActiveQHandle   hActiveQ,
   uint64_t          uiJobId
);

/***************************************************************************/

uint32_t BVC5_P_ActiveQSize(
   BVC5_ActiveQHandle hActiveQ
);

/***************************************************************************/

BVC5_P_InternalJob *BVC5_P_ActiveQFirst(
   BVC5_ActiveQHandle hActiveQ
);

/***************************************************************************/

BVC5_P_InternalJob *BVC5_P_ActiveQNext(
   BVC5_P_InternalJob  *psIter
);

/***************************************************************************/

BVC5_P_InternalJob *BVC5_P_ActiveQLast(
   BVC5_ActiveQHandle hActiveQ
);

/***************************************************************************/

BVC5_P_InternalJob *BVC5_P_ActiveQPrev(
   BVC5_P_InternalJob  *psIter
);

/***************************************************************************/

void BVC5_P_ActiveQInsert(
   BVC5_ActiveQHandle      hActiveQ,
   BVC5_P_InternalJob  *pJob
);

/***************************************************************************/

#endif
