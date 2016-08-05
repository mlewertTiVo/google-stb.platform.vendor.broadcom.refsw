/***************************************************************************
 *  Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
 *
 * Module Description:
 *
 **************************************************************************/
#include "bstd.h"
#include "bsynclib_priv.h"
#include "bsynclib_channel_priv.h"
#include "bsynclib_audio_sink.h"

BDBG_MODULE(synclib);

BSYNClib_AudioSink * BSYNClib_AudioSink_Create(void)
{
	BSYNClib_AudioSink * psSink = NULL;

	BDBG_ENTER(BSYNClib_AudioSink_Create);

	psSink = (BSYNClib_AudioSink *)BKNI_Malloc(sizeof(BSYNClib_AudioSink));

	if (psSink)
	{
		BKNI_Memset(psSink, 0, sizeof(BSYNClib_AudioSink));
		BSYNClib_DelayElement_Init(&psSink->sElement);
		psSink->sData.uiSamplingRate = 48000;
		BSYNClib_AudioSink_GetDefaultConfig(&psSink->sConfig);
		BSYNClib_AudioSink_P_GetDefaultStatus(&psSink->sStatus);
	}

	BDBG_LEAVE(BSYNClib_AudioSink_Create);
	return psSink;
}

void BSYNClib_AudioSink_Destroy(BSYNClib_AudioSink * psSink)
{
	BDBG_ENTER(BSYNClib_AudioSink_Destroy);
	BDBG_ASSERT(psSink);
	BKNI_Free(psSink);
	BDBG_LEAVE(BSYNClib_AudioSink_Destroy);
}

bool BSYNClib_AudioSink_SyncCheck(BSYNClib_AudioSink * psSink)
{
	bool bPass = false;

	BDBG_ENTER(BSYNClib_AudioSink_SyncCheck);

	BDBG_ASSERT(psSink);

	bPass = !psSink->sElement.sSnapshot.bSynchronize
		|| (psSink->sElement.sDelay.sSnapshot.bValid /* have a valid measured delay */
			&& psSink->sElement.sDelay.sResults.bAccepted); /* and be accepted */

	BDBG_MSG(("[%d] Audio sink %u sync check:", psSink->sElement.hParent->iIndex, psSink->sElement.uiIndex));
	BDBG_MSG(("[%d]  %s", psSink->sElement.hParent->iIndex, psSink->sElement.sSnapshot.bSynchronize ? "synchronized" : "ignored"));
	BDBG_MSG(("[%d]  delay %s, %s", psSink->sElement.hParent->iIndex,
		psSink->sElement.sDelay.sSnapshot.bValid ? "valid" : "invalid",
			psSink->sElement.sDelay.sResults.bAccepted ? "accepted" : "unaccepted"));

	BDBG_LEAVE(BSYNClib_AudioSink_SyncCheck);
	return bPass;
}

void BSYNClib_AudioSink_Reset_isr(BSYNClib_AudioSink * psSink)
{
	BDBG_ENTER(BSYNClib_AudioSink_Reset_isr);
	BDBG_ASSERT(psSink);
	psSink->sData.uiSamplingRate = 48000;
	BDBG_LEAVE(BSYNClib_AudioSink_Reset_isr);
}

void BSYNClib_AudioSink_GetDefaultConfig(
	BSYNClib_AudioSink_Config * psConfig
)
{
	BDBG_ENTER(BSYNClib_AudioSink_GetDefaultConfig);
	BDBG_ASSERT(psConfig);
	BKNI_Memset(psConfig, 0, sizeof(BSYNClib_AudioSink_Config));
	psConfig->bCompressed = false;
	psConfig->uiSamplingRate = 48000;
	BDBG_LEAVE(BSYNClib_AudioSink_GetDefaultConfig);
}

void BSYNClib_AudioSink_P_SelfClearConfig_isr(
	BSYNClib_AudioSink * psSink
)
{
	BDBG_ENTER(BSYNClib_AudioSink_P_SelfClearConfig_isr);
	BDBG_ASSERT(psSink);
	psSink->sConfig.sDelay.bReceived = false;
	BDBG_LEAVE(BSYNClib_AudioSink_P_SelfClearConfig_isr);
}

BERR_Code BSYNClib_AudioSink_P_ProcessConfig_isr(
	BSYNClib_AudioSink * psSink
)
{
	BERR_Code rc = BERR_SUCCESS;
	BSYNClib_AudioSink_Config * psConfig;
	BSYNClib_Channel_Handle hChn;
	bool bChanged = false;
	BSYNClib_DelayElement sDesired;
	BSYNClib_DelayElement * psCurrent;
	BSYNClib_DelayElement_DiffResults sElementDiffResults;

	BDBG_ENTER(BSYNClib_AudioSink_P_ProcessConfig_isr);

	BDBG_ASSERT(psSink);

	hChn = psSink->sElement.hParent;
	psConfig = &psSink->sConfig;
	psCurrent = &psSink->sElement;

	BKNI_Memset(&sElementDiffResults, 0, sizeof(BSYNClib_DelayElement_DiffResults));

	/* check lifecycle first, not yet available for sinks */
	BSYNClib_DelayElement_CheckLifecycle_isr(true, psCurrent, &sElementDiffResults);
	if (sElementDiffResults.eLifecycleEvent == BSYNClib_DelayElement_LifecycleEvent_eStarted)
	{
		BSYNClib_DelayElement_Reset_isr(psCurrent);
	}

	/* create "desired" delay element config */
	sDesired = *psCurrent;
	sDesired.sData.bStarted = true;
	sDesired.sData.bSynchronize = psConfig->bSynchronize;
	sDesired.sDelay.sData.uiMeasured = BSYNClib_P_Convert_isrsafe(
		psConfig->sDelay.sMeasured.uiValue,
		psConfig->sDelay.sMeasured.eUnits,
		BSYNClib_Units_e27MhzTicks);
	sDesired.sDelay.sData.uiCustom = BSYNClib_P_Convert_isrsafe(
		psConfig->sDelay.sCustom.uiValue,
		psConfig->sDelay.sCustom.eUnits,
		BSYNClib_Units_e27MhzTicks);
	sDesired.sDelay.sData.eOriginalUnits = psConfig->sDelay.sMeasured.eUnits;
	sDesired.sNotification.sData.bReceived = psConfig->sDelay.bReceived;

	BSYNClib_DelayElement_Diff_isr(&sDesired, &psSink->sElement, &sElementDiffResults);

	if (sElementDiffResults.bChanged)
	{
		bChanged = true;
		BSYNClib_DelayElement_Patch_isr(&sDesired, &psSink->sElement, &sElementDiffResults);
	}

	if (sElementDiffResults.eLifecycleEvent == BSYNClib_DelayElement_LifecycleEvent_eStarted)
	{
		/* reset sink data */
		BSYNClib_AudioSink_Reset_isr(psSink);

		/* NOTE: cannot change compressed on the fly */
		if (psConfig->bCompressed != psSink->sData.bCompressed)
		{
			bChanged = true;
			psSink->sData.bCompressed = psConfig->bCompressed;
		}
	}

	/* received sampling rate info */
	if (psSink->sData.uiSamplingRate != psConfig->uiSamplingRate)
	{
		psSink->sData.uiSamplingRate = psConfig->uiSamplingRate;
		bChanged = true;
	}

	/* all audio delays are accepted immediately */
	/* TODO: when we add dynamic audio delays, we need to revisit immediate acceptance */
	if (sElementDiffResults.bDelayReceived)
	{
		psSink->sElement.sDelay.sResults.bAccepted = true;
	}

	/* if anything changed in the config, reprocess based on current state */
	if (bChanged)
	{
		BDBG_MSG(("[%d] Audio sink %u properties changed:", psSink->sElement.hParent->iIndex, psSink->sElement.uiIndex));
		BDBG_MSG(("[%d]  %s", psSink->sElement.hParent->iIndex, psSink->sElement.sData.bSynchronize ? "synchronized" : "ignored"));
		if (sElementDiffResults.eLifecycleEvent != BSYNClib_DelayElement_LifecycleEvent_eNone)
		{
			BDBG_MSG(("[%d]  %s", psSink->sElement.hParent->iIndex, BSYNClib_DelayElement_LifecycleEventNames[sElementDiffResults.eLifecycleEvent]));
		}
		BDBG_MSG(("[%d]  measured delay %u ms",
			psSink->sElement.hParent->iIndex,
			BSYNClib_P_Convert_isrsafe(psSink->sElement.sDelay.sData.uiMeasured, BSYNClib_Units_e27MhzTicks, BSYNClib_Units_eMilliseconds)));
		BDBG_MSG(("[%d]  custom delay %u ms",
			psSink->sElement.hParent->iIndex,
			BSYNClib_P_Convert_isrsafe(psSink->sElement.sDelay.sData.uiCustom, BSYNClib_Units_e27MhzTicks, BSYNClib_Units_eMilliseconds)));
		BDBG_MSG(("[%d]  %s", psSink->sElement.hParent->iIndex, psSink->sData.bCompressed ? "compressed" : "pcm"));
		BDBG_MSG(("[%d]  sampling rate %u", psSink->sElement.hParent->iIndex, psSink->sData.uiSamplingRate));
	}

	if (bChanged)
	{
		if (BSYNClib_Channel_P_Enabled_isrsafe(hChn))
		{
			/* immediate reprocessing based on current state */
			BSYNClib_Channel_P_ScheduleTask_isr(hChn);
		}
		else
		{
			/* reprocessing based on current state deferred until re-enabled */
			BSYNClib_Channel_P_EnqueueTaskRequest_isr(hChn);
		}
	}

	BDBG_LEAVE(BSYNClib_AudioSink_P_ProcessConfig_isr);
	return rc;
}

void BSYNClib_AudioSink_Snapshot_isr(BSYNClib_AudioSink * psSink)
{
	BDBG_ENTER(BSYNClib_AudioSink_Snapshot_isr);
	BDBG_ASSERT(psSink);
	psSink->sSnapshot = psSink->sData;
	BSYNClib_DelayElement_Snapshot_isr(&psSink->sElement);
	BDBG_LEAVE(BSYNClib_AudioSink_Snapshot_isr);
}

void BSYNClib_AudioSink_P_GetDefaultStatus(
	BSYNClib_Sink_Status * psStatus
)
{
	BDBG_ENTER(BSYNClib_AudioSink_P_GetDefaultStatus);
	BDBG_ASSERT(psStatus);
	psStatus->sDelayNotification.bEnabled = false;
	psStatus->sDelayNotification.sThreshold.uiValue = 0;
	psStatus->sAppliedDelay.uiValue = 0;
	BDBG_LEAVE(BSYNClib_AudioSink_P_GetDefaultStatus);
}
