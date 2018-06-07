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
#ifndef BDSP_RAAGA_IO_PRIV_H_
#define BDSP_RAAGA_IO_PRIV_H_

#include "bdsp_raaga_priv_include.h"

BERR_Code BDSP_Raaga_P_InterTaskBuffer_Create(
	void *pContextHandle,
	BDSP_DataType dataType,
	BDSP_BufferType bufferType,
	BDSP_InterTaskBufferHandle *pInterTaskBufferHandle
);
void BDSP_Raaga_P_InterTaskBuffer_Destroy(
	void *pInterTaskBufferHandle
);
void BDSP_Raaga_P_InterTaskBuffer_Flush(
	void *pInterTaskBufferHandle
);

BERR_Code BDSP_Raaga_P_GetDefaultCreateQueueSettings(
	void                     *pContextHandle,
	BDSP_QueueCreateSettings *pSettings
);
BERR_Code BDSP_Raaga_P_Queue_Create(
	void 					 *pContextHandle,
	unsigned 				  dspIndex,
	BDSP_QueueCreateSettings *pSettings,
	BDSP_QueueHandle 		 *pQueueHandle
);
void BDSP_Raaga_P_Queue_Destroy(
	void *pQueueHandle
);
void BDSP_Raaga_P_Queue_Flush(
	void *pQueueHandle
);
BERR_Code BDSP_Raaga_P_AddRaveInput(
	void *pStageHandle,
	const BAVC_XptContextMap *pContextMap,
	unsigned *pInputIndex
);
BERR_Code BDSP_Raaga_P_AddRaveOutput(
	void *pStageHandle,
	const BAVC_XptContextMap *pContextMap,
	unsigned *pOutputIndex
);
BERR_Code BDSP_Raaga_P_AddFmmInput(
	void *pStageHandle,
	BDSP_DataType dataType,
	const BDSP_FmmBufferDescriptor *pDescriptor,
	unsigned *pInputIndex
);
BERR_Code BDSP_Raaga_P_AddFmmOutput(
	void *pStageHandle,
	BDSP_DataType dataType,
	const BDSP_FmmBufferDescriptor *pDescriptor,
	unsigned *pOutputIndex
);
BERR_Code BDSP_Raaga_P_AddInterTaskBufferInput(
	void *pStageHandle,
	BDSP_DataType dataType,
	const BDSP_InterTaskBuffer *pBufferHandle,
	unsigned *pInputIndex
);
BERR_Code BDSP_Raaga_P_AddInterTaskBufferOutput(
	void *pStageHandle,
	BDSP_DataType dataType,
	const BDSP_InterTaskBuffer *pBufferHandle,
	unsigned *pOutputIndex
);
#if !B_REFSW_MINIMAL
BERR_Code BDSP_Raaga_P_AddQueueInput(
	void     *pStageHandle,
	void     *pQueueHandle,
	unsigned *pInputIndex /* [out] */
);
#endif /*!B_REFSW_MINIMAL*/
BERR_Code BDSP_Raaga_P_AddQueueOutput(
	void     *pStageHandle,
	void     *pQueueHandle,
	unsigned *pOutputIndex /* [out] */
);
BERR_Code BDSP_Raaga_P_AddOutputStage(
	void *pSrcStageHandle,
	BDSP_DataType dataType,
	void *pDstStageHandle,
	unsigned *pSourceOutputIndex,
	unsigned *pDestinationInputIndex
);
void BDSP_Raaga_P_RemoveInput(
	void *pStageHandle,
	unsigned inputIndex
);
void BDSP_Raaga_P_RemoveAllInputs(
	void *pStageHandle
);
void BDSP_Raaga_P_RemoveOutput(
	void *pStageHandle,
	unsigned outputIndex
);
void BDSP_Raaga_P_RemoveAllOutputs(
	void *pStageHandle
);
void BDSP_Raaga_P_Queue_GetIoBuffer(
	void *pQueueHandle,
	BDSP_AF_P_sIO_BUFFER *pBuffer /*[out]*/
);
BERR_Code BDSP_Raaga_P_Queue_GetBuffer(
	void                  *pQueueHandle,
	BDSP_BufferDescriptor *pDescriptor /*[out] */
);
BERR_Code BDSP_Raaga_P_Queue_CommitData(
	void  *pQueueHandle,
	size_t bytesWritten
);
BERR_Code BDSP_Raaga_P_Queue_ConsumeData(
	void  *pQueueHandle,
	size_t readBytes
);
BERR_Code BDSP_Raaga_P_Queue_GetBufferAddr(
    void    *pQueueHandle,
    unsigned numbuffers,
    void    *pBuffer /*[out] */
);
#endif /*BDSP_RAAGA_IO_PRIV_H_*/
