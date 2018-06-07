/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
#include "bstd.h"
#include "bvc5.h"
#include "bvc5_priv.h"
#include "bvc5_internal_job_priv.h"
#include "bvc5_bin_mem_priv.h"
#include "bvc5_client_priv.h"

#include "bvc5_fence_priv.h"

/* BVC5_P_CreateInternalJob

   Fill in all the common fields of the job.
 */
static BVC5_P_InternalJob *BVC5_P_CreateInternalJob(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   BVC5_JobBase            *toJob,
   const BVC5_JobBase      *fromJob
   )
{
   BVC5_P_InternalJob   *pJob = NULL;

   BSTD_UNUSED(hVC5);

   if (toJob == NULL)
      return NULL;

   BDBG_CASSERT(sizeof(BVC5_P_InternalJob) <= CPU_PAGE_SIZE);
   pJob = (BVC5_P_InternalJob *)BKNI_Malloc(sizeof(BVC5_P_InternalJob));
   if (pJob != NULL)
   {
      BKNI_Memset(pJob, 0, sizeof(BVC5_P_InternalJob));
      pJob->pBase      = toJob;
      pJob->uiJobId    = fromJob->uiJobId;
      pJob->uiClientId = uiClientId;
      BKNI_Memcpy(&pJob->sRunDep_NotCompleted, &fromJob->sCompletedDependencies, sizeof(BVC5_SchedDependencies));
      BKNI_Memcpy(&pJob->sRunDep_NotFinalized, &fromJob->sFinalizedDependencies, sizeof(BVC5_SchedDependencies));
      BKNI_Memcpy(&pJob->sFinDep_NotFinalized, &fromJob->sCompletedDependencies, sizeof(BVC5_SchedDependencies));
      pJob->eStatus    = BVC5_JobStatus_eSUCCESS;
   }

   return pJob;
}

static void BVC5_P_FreeInternalJob(BVC5_P_InternalJob *pJob)
{
   if (pJob != NULL)
      BKNI_Free(pJob);
}

BVC5_P_InternalJob *BVC5_P_JobCreateNull(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobNull      *psJob
)
{
   BVC5_JobNull         *pNullJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobNull) <= CPU_PAGE_SIZE);
   pNullJob = (BVC5_JobNull *)BKNI_Malloc(sizeof(BVC5_JobNull));

   if (pNullJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)pNullJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
         BKNI_Memcpy(pNullJob, psJob, sizeof(BVC5_JobNull));
      else
         BKNI_Free(pNullJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateBarrier(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobBarrier   *psJob
)
{
   BVC5_JobBarrier      *pBarrierJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobBarrier) <= CPU_PAGE_SIZE);
   pBarrierJob = (BVC5_JobBarrier *)BKNI_Malloc(sizeof(BVC5_JobBarrier));

   if (pBarrierJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)pBarrierJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
         BKNI_Memcpy(pBarrierJob, psJob, sizeof(BVC5_JobNull));
      else
         BKNI_Free(pBarrierJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateBin(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobBin       *psJob,
   BVC5_P_InternalJob      *pRenderJob
)
{
   BVC5_JobBin          *pBinJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobBin) <= CPU_PAGE_SIZE);
   pBinJob = (BVC5_JobBin *)BKNI_Malloc(sizeof(BVC5_JobBin));

   if (pBinJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)pBinJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
      {
         BKNI_Memcpy(pBinJob, psJob, sizeof(BVC5_JobBin));
         pJob->jobData.sBin.psInternalRenderJob = pRenderJob;
         pJob->jobData.sBin.uiMinInitialBinBlockSize = psJob->uiMinInitialBinBlockSize;
         pJob->jobData.sBin.uiTileStateSize = psJob->uiTileStateSize;
      }
      else
         BKNI_Free(pBinJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateRender(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobRender    *psJob
)
{
   BVC5_JobRender       *pRenderJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobRender) <= CPU_PAGE_SIZE);
   pRenderJob = (BVC5_JobRender *)BKNI_Malloc(sizeof(BVC5_JobRender));

   if (pRenderJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, &pRenderJob->sBase, &psJob->sBase);

      if (pJob != NULL)
      {
         BVC5_BinPoolHandle hBinPool = (psJob->sBase.bSecure && hVC5->hSecureBinPool) ? hVC5->hSecureBinPool : hVC5->hBinPool;

         BKNI_Memcpy(pRenderJob, psJob, sizeof(BVC5_JobRender));

         if (BVC5_P_BinMemArrayCreate(hBinPool, &pJob->jobData.sRender.sBinMemArray) != BERR_SUCCESS)
         {
            BVC5_P_BinMemArrayDestroy(&pJob->jobData.sRender.sBinMemArray);
            BVC5_P_FreeInternalJob(pJob);
            BKNI_Free(pRenderJob);
            pJob = NULL;
         }
      }
      else
         BKNI_Free(pRenderJob);
   }

   return pJob;
}

#if V3D_VER_AT_LEAST(4,1,34,0)
BVC5_P_InternalJob *BVC5_P_JobCreateCompute(
   BVC5_Handle             hVC5,
   uint32_t                uiClientId,
   const BVC5_JobCompute   *pComputeJob,
   BVC5_P_ComputeSubjobs   *pSubjobs)
{
   BVC5_JobCompute *pComputeJobCopy = NULL;
   BVC5_P_InternalJob *pJob = NULL;

   /* Copy compute job.*/
   BDBG_CASSERT(sizeof(BVC5_JobCompute) <= CPU_PAGE_SIZE);
   pComputeJobCopy = BKNI_Malloc(sizeof(BVC5_JobCompute));
   if (!pComputeJobCopy)
      goto fail;
   BKNI_Memcpy(pComputeJobCopy, pComputeJob, sizeof(BVC5_JobCompute));

   pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, &pComputeJobCopy->sBase, &pComputeJob->sBase);
   if (!pJob)
      goto fail;
   pJob->jobData.sCompute.pSubjobs = pSubjobs;
   pJob->jobData.sCompute.uiNumIssued = 0;
   pJob->jobData.sCompute.uiNumDone = 0;
   return pJob;

fail:
   if (pComputeJobCopy)
      BKNI_Free(pComputeJobCopy);
   return NULL;
}
#endif

static void BVC5_P_JobWaitCallback(void *context, uint64_t param)
{
   BVC5_Handle        hVC5  = (BVC5_Handle)context;
   BSTD_UNUSED(param);

   /* Prod the scheduler */
   BKNI_SetEvent(hVC5->hSchedulerWakeEvent);
}

BVC5_P_InternalJob *BVC5_P_JobCreateFenceWait(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobFenceWait *psJob
)
{
   BVC5_JobFenceWait    *pWaitJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobFenceWait) <= CPU_PAGE_SIZE);
   pWaitJob = (BVC5_JobFenceWait *)BKNI_Malloc(sizeof(BVC5_JobFenceWait));

   if (pWaitJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)pWaitJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
      {
         int res;
         BKNI_Memcpy(pWaitJob, psJob, sizeof(BVC5_JobFenceWait));

         res = BVC5_P_FenceWaitAsync(hVC5->hFences, uiClientId, psJob->iFence, BVC5_P_JobWaitCallback, hVC5, 0,
               &pJob->jobData.sWait.waitData);
         if (res < 0)
         {
            BVC5_P_FreeInternalJob(pJob);
            BKNI_Free(pWaitJob);
            pJob = NULL;
         }
         else if (res > 0)
         {
            /* the fence was already signalled;*/
            pJob->jobData.sWait.signaled = true;
            pJob->jobData.sWait.waitData = NULL;
         }
         if (res != 0)
         {
            /* the wait async didn't succedd or the fence was already signalled,
             * we must close the fd */
            BVC5_P_FenceClose(hVC5->hFences, psJob->iFence);
         }
      }
      else
         BKNI_Free(pWaitJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateTFU(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobTFU       *psJob
)
{
   BVC5_JobTFU          *psTFUJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobTFU) <= CPU_PAGE_SIZE);
   psTFUJob = (BVC5_JobTFU *)BKNI_Malloc(sizeof(BVC5_JobTFU));

   if (psTFUJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)psTFUJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
         BKNI_Memcpy(psTFUJob, psJob, sizeof(BVC5_JobTFU));
      else
         BKNI_Free(psTFUJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateTest(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobTest      *psJob
)
{
   BVC5_JobTest         *psTestJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobTest) <= CPU_PAGE_SIZE);
   psTestJob = (BVC5_JobTest *)BKNI_Malloc(sizeof(BVC5_JobTest));

   if (psTestJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)psTestJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
         BKNI_Memcpy(psTestJob, psJob, sizeof(BVC5_JobTest));
      else
         BKNI_Free(psTestJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateUsermode(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobUsermode  *psJob
)
{
   BVC5_JobUsermode     *psUsermodeJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobUsermode) <= CPU_PAGE_SIZE);
   psUsermodeJob = (BVC5_JobUsermode *)BKNI_Malloc(sizeof(BVC5_JobUsermode));

   if (psUsermodeJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)psUsermodeJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
         BKNI_Memcpy(psUsermodeJob, psJob, sizeof(BVC5_JobUsermode));
      else
         BKNI_Free(psUsermodeJob);
   }

   return pJob;
}

BVC5_P_InternalJob *BVC5_P_JobCreateSchedEvent(
   BVC5_Handle              hVC5,
   uint32_t                 uiClientId,
   const BVC5_JobSchedJob  *psJob
)
{
   BVC5_JobSchedJob     *psSchedEventJob;
   BVC5_P_InternalJob   *pJob = NULL;
   BDBG_CASSERT(sizeof(BVC5_JobSchedJob) <= CPU_PAGE_SIZE);
   psSchedEventJob = (BVC5_JobSchedJob *)BKNI_Malloc(sizeof(BVC5_JobSchedJob));

   if (psSchedEventJob != NULL)
   {
      pJob = BVC5_P_CreateInternalJob(hVC5, uiClientId, (BVC5_JobBase *)psSchedEventJob, (BVC5_JobBase *)psJob);

      if (pJob != NULL)
      {
         BKNI_Memcpy(psSchedEventJob, psJob, sizeof(BVC5_JobSchedJob));
         pJob->jobData.sEvent.eventId = psJob->uiEventId;
      }
      else
         BKNI_Free(psSchedEventJob);
   }

   return pJob;
}

void BVC5_P_JobDestroy(
   BVC5_Handle           hVC5,
   BVC5_P_InternalJob   *pJob
)
{
   BVC5_JobBase   *pJobBase;

   if (pJob == NULL)
      return;

   pJobBase = pJob->pBase;

   switch(pJobBase->eType)
   {
      case BVC5_JobType_eRender:
      /* Need to free bin memory if still allocated */
         BVC5_P_BinMemArrayDestroy(&pJob->jobData.sRender.sBinMemArray);
         break;
#if V3D_VER_AT_LEAST(4,1,34,0)
      case BVC5_JobType_eCompute:
         {
            uint32_t uiSubjobsId = ((const BVC5_JobCompute*)pJobBase)->uiSubjobsId;
            if (uiSubjobsId != 0)
            {
               /* If uiSubjobsId is non-zero, then need to clear out this pointer from the client's lookup table. */
               BVC5_ClientHandle hClient = BVC5_P_ClientMapGet(hVC5, hVC5->hClientMap, pJob->uiClientId);
               BVC5_P_ClientDeleteComputeSubjobs(hClient, uiSubjobsId, pJob->jobData.sCompute.pSubjobs);
            }
            else
            {
               /* Otherwise we can just delete this object. */
               BVC5_P_DeleteComputeSubjobs(pJob->jobData.sCompute.pSubjobs);
            }
         }
         break;
#endif
      case BVC5_JobType_eFenceWait:
         if (pJob->jobData.sWait.waitData)
            BVC5_P_FenceWaitAsyncCleanup(hVC5->hFences, pJob->uiClientId,
                  BVC5_P_JobWaitCallback, hVC5, 0, pJob->jobData.sWait.waitData);
         break;
      default:
         break;
   }

   BKNI_Free(pJobBase);
   BVC5_P_FreeInternalJob(pJob);
}

#if V3D_VER_AT_LEAST(4,1,34,0)

BVC5_P_ComputeSubjobs *BVC5_P_NewComputeSubjobs(uint32_t uiCapacity)
{
   /* Note that zero sized arrays are not supported in ancient C, so workaround this. */
   BVC5_P_ComputeSubjobs *pSubjobs = BKNI_Malloc(offsetof(BVC5_P_ComputeSubjobs, pData) + sizeof(BVC5_JobComputeSubjob)*uiCapacity);
   if (!pSubjobs)
      return NULL;
   pSubjobs->uiCapacity = uiCapacity;
   pSubjobs->uiSize = 0;
   return pSubjobs;
}

void BVC5_P_UpdateComputeSubjobs(BVC5_P_ComputeSubjobs *pSubjobs, uint32_t uiSize, const BVC5_JobComputeSubjob *pNewData)
{
   BDBG_ASSERT(uiSize <= pSubjobs->uiCapacity);
   BKNI_Memcpy(pSubjobs->pData, pNewData, sizeof(BVC5_JobComputeSubjob) * uiSize);
   pSubjobs->uiSize = uiSize;
}

void BVC5_P_DeleteComputeSubjobs(BVC5_P_ComputeSubjobs *pSubjobs)
{
   if (pSubjobs)
      BKNI_Free(pSubjobs);
}

#endif
