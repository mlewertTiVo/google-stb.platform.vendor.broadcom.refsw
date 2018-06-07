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
 *****************************************************************************/
#ifndef BDSP_RAAGA_CAPTURE_PRIV_H_
#define BDSP_RAAGA_CAPTURE_PRIV_H_

BERR_Code BDSP_Raaga_P_AudioCaptureCreate(
	void                                  *pContextHandle,
	const BDSP_AudioCaptureCreateSettings *pCaptureCreateSettings,
	BDSP_AudioCaptureHandle               *pCapture
);
void BDSP_Raaga_P_AudioCaptureDestroy (
	void *pCapture
);
BERR_Code BDSP_Raaga_P_AddAudioCaptureToStage(
	void *pCaptureHandle,
	void *pStageHandle,
	unsigned outputId,
	const BDSP_StageAudioCaptureSettings *pSettings
);
void BDSP_Raaga_P_RemoveAudioCaptureFromStage(
	void *pCaptureHandle,
	void *pStageHandle
);
BERR_Code BDSP_Raaga_P_AudioCaptureGetBuffer(
	void                  *pCaptureHandle,   /* [in] capture handle */
	BDSP_BufferDescriptor *pBuffers     /* [out] pointer to buffer descriptor */
);
BERR_Code BDSP_Raaga_P_AudioCaptureConsumeData(
	void    *pCaptureHandle, /* [in] capture handle */
	uint32_t numBytes       /* [in] sizes of data read from each intermediate buffer */
);
BERR_Code BDSP_Raaga_P_ProcessAudioCapture(
	void *pDevice /* [in] device handle */
);
#endif /*BDSP_RAAGA_CAPTURE_PRIV_H_*/
