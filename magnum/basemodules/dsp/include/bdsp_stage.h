/******************************************************************************
 * (c) 2006-2015 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
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
 *****************************************************************************/


#ifndef BDSP_STAGE_H_
#define BDSP_STAGE_H_

#include "bdsp_raaga_fw_settings.h"
#include "bdsp_raaga_fw.h"
#include "bdsp_raaga_fw_status.h"

typedef struct
{
     uint32_t wrcnt;
}BDSP_RateController;


/***************************************************************************
Summary:
Stage Source and Destination Details
***************************************************************************/
typedef struct BDSP_StageSrcDstDetails
{
    BDSP_AF_P_ValidInvalid      eNodeValid;

    BDSP_ConnectionType eConnectionType;

    union
    {
        struct
            {
                BDSP_StageHandle hStage;
            } stage;
        struct
            {
                BDSP_FmmBufferDescriptor descriptor;
            } fmm;
        struct
            {
                const BAVC_XptContextMap *pContextMap;
            } rave;
        struct
            {
                BDSP_InterTaskBufferHandle hInterTask;
            } interTask;
        struct
            {
                BDSP_QueueHandle pQHandle;
            } rdb;
    } connectionDetails;

    BDSP_AF_P_sIO_BUFFER    IoBuffer;
    /*unlike other cases, not a pointer. just save a copy here during stage creation.
    Task create will allocate memory for a IOBuffer structure and these values can be copied then*/
    union{
        BDSP_AF_P_sIO_GENERIC_BUFFER IoGenericBuffer;   /*FW output*/    /* similar comment like above*/
        BDSP_RateController          rateController[4]; /*FMM output*/
        }Metadata;

    BDSP_StageHandle connectionHandle;

    /*populated during stage creation, the content itself populated in node configuration */
    void *pIoBuffDesc;
    void *pIoGenBuffDesc;

    raaga_dramaddr ui32StageIOBuffCfgAddr; /* These are offsets to the structure BDSP_AF_P_sIO_BUFFER*/
    raaga_dramaddr ui32StageIOGenericDataBuffCfgAddr; /* These are offsets to the structure BDSP_AF_P_sIO_GENERIC_BUFFER */

}BDSP_StageSrcDstDetails;


/***************************************************************************
Summary:
Audio Capture Handle
***************************************************************************/
typedef struct BDSP_AudioCapture *BDSP_AudioCaptureHandle;

/***************************************************************************
Summary:
Audio Capture Create Settings
***************************************************************************/
typedef struct BDSP_AudioCaptureCreateSettings
{
    unsigned maxChannels;       /* Maximum number of channels to capture.  1 = mono/compressed.  2 = stereo.  6 = 5.1.  Default = 2. */
    size_t channelBufferSize;   /* Channel buffer size in bytes.  Default is 1536kB. */
    BMEM_Heap_Handle hHeap;     /* Memory Heap to use for allocating buffers.  If NULL, the default heap will be used. */
} BDSP_AudioCaptureCreateSettings;

/***************************************************************************
Summary:
Queue Create Settings
***************************************************************************/
typedef struct BDSP_QueueCreateSettings
{
    BDSP_DataType dataType;
    unsigned numBuffers;
    struct
    {
        unsigned bufferSize;              /* Buffer length in bytes (must be a multiple of 4) */
        uint32_t bufferAddress;           /* Physical address of the buffer base */
    } bufferInfo[BDSP_AF_P_MAX_CHANNELS];
    bool input;                           /* True if the host will write to the queue instead of reading output */
} BDSP_QueueCreateSettings;

/***************************************************************************
Summary:
Buffer Descriptor
***************************************************************************/
typedef struct BDSP_BufferDescriptor
{
    bool interleaved;               /* If true, every other channel will have valid pointers,
                                       e.g. L for L/R, Ls for Ls/Rs, etc.  */
    unsigned numBuffers;            /* Number of buffers.  For mono/interleaved stereo this is 1.  For
                                       non-interleaved stereo, it's 2.  For non-interleaved 7.1 it's 8. */
    struct
    {
        void *pBuffer;              /* Buffer base address prior to wraparound */
        void *pWrapBuffer;          /* Buffer address after wraparound (NULL if no wrap has occurred) */
    } buffers[BDSP_AF_P_MAX_CHANNELS];

    unsigned bufferSize;            /* Buffer size before wraparound in bytes */
    unsigned wrapBufferSize;        /* Buffer size after wraparound in bytes */
} BDSP_BufferDescriptor;

/***************************************************************************
Summary:
Get default stage create settings
***************************************************************************/
void BDSP_Stage_GetDefaultCreateSettings(
    BDSP_ContextHandle hContext,
    BDSP_AlgorithmType algoType,
    BDSP_StageCreateSettings *pSettings /* [out] */
    );

/***************************************************************************
Summary:
Create a Stage
***************************************************************************/
BERR_Code BDSP_Stage_Create(
    BDSP_ContextHandle hContext,
    const BDSP_StageCreateSettings *pSettings,
    BDSP_StageHandle *pStage /* [out] */
    );

/***************************************************************************
Summary:
Destory a Stage
***************************************************************************/
void BDSP_Stage_Destroy(
    BDSP_StageHandle hStage
    );

/***************************************************************************
Summary:
Set the algorithm for a stage
***************************************************************************/
BERR_Code BDSP_Stage_SetAlgorithm(
    BDSP_StageHandle hStage,
    BDSP_Algorithm algorithm
    );

/***************************************************************************
Summary:
Get settings for a particular stage
***************************************************************************/
BERR_Code BDSP_Stage_GetSettings(
    BDSP_StageHandle hStage,
    void *pSettingsBuffer,        /* [out] */
    size_t settingsBufferSize     /* [in] */
    );

/***************************************************************************
Summary:
Set settings for a particular stage
***************************************************************************/
BERR_Code BDSP_Stage_SetSettings(
    BDSP_StageHandle hStage,
    const void *pSettingsBuffer,    /* [out] */
    size_t settingsBufferSize       /* [in] */
    );

/***************************************************************************
Summary:
Get status from a particular stage
***************************************************************************/
BERR_Code BDSP_Stage_GetStatus(
    BDSP_StageHandle hStage,
    void *pStatusBuffer,        /* [out] */
    size_t statusBufferSize     /* [in] */
    );

/***************************************************************************
Summary:
Add an FMM output to a stage
***************************************************************************/
BERR_Code BDSP_Stage_AddFmmOutput(
    BDSP_StageHandle hStage,
    BDSP_DataType dataType,
    const BDSP_FmmBufferDescriptor *pDescriptor,
    unsigned *pOutputIndex
    );

/***************************************************************************
Summary:
Add a Rave output to a stage
***************************************************************************/
BERR_Code BDSP_Stage_AddRaveOutput(
    BDSP_StageHandle hStage,
    const BAVC_XptContextMap *pContext,
    unsigned *pOutputIndex /* [out] */
    );

/***************************************************************************
Summary:
Connect an output stage to a stage
***************************************************************************/
BERR_Code BDSP_Stage_AddOutputStage(
    BDSP_StageHandle hSourceStage,
    BDSP_DataType dataType,
    BDSP_StageHandle hDestinationStage,
    unsigned *pSourceOutputIndex, /* [out] */    /* Output index, reflects the source stage's output index for the destination stage. */
    unsigned *pDestinationInputIndex /* [out] */    /* Input index, reflects the destination stage's input index for the source stage. */
    );

/***************************************************************************
Summary:
Connect an inter task buffer at output
***************************************************************************/
BERR_Code BDSP_Stage_AddInterTaskBufferOutput(
    BDSP_StageHandle hStage,
    BDSP_DataType dataType,
    BDSP_InterTaskBufferHandle hInterTaskBuffer,    /* inter-task buffer handle */
    unsigned *pOutputIndex /* [out] */    /* Output index, reflects the source stage's output index for the inter task buffer. */
    );

/***************************************************************************
Summary:
Connect an inter task buffer at input
***************************************************************************/
BERR_Code BDSP_Stage_AddInterTaskBufferInput(
    BDSP_StageHandle hStage,
    BDSP_DataType dataType,
    BDSP_InterTaskBufferHandle hInterTaskBuffer,    /* inter-task buffer handle */
    unsigned *pInputIndex /* [out] */    /* input index, reflects the source stage's input index for the inter task buffer. */
    );

/***************************************************************************
Summary:
Remove an output from a stage
***************************************************************************/
void BDSP_Stage_RemoveOutput(
    BDSP_StageHandle hStage,
    unsigned outputIndex
    );

/***************************************************************************
Summary:
Remove all outputs from a stage
***************************************************************************/
void BDSP_Stage_RemoveAllOutputs(
    BDSP_StageHandle hStage
    );

/***************************************************************************
Summary:
Add an FMM Input to a stage
***************************************************************************/
BERR_Code BDSP_Stage_AddFmmInput(
    BDSP_StageHandle hStage,
    BDSP_DataType dataType,
    const BDSP_FmmBufferDescriptor *pDescriptor,
    unsigned *pInputIndex
    );

/***************************************************************************
Summary:
Add a rave input to a stage
***************************************************************************/
BERR_Code BDSP_Stage_AddRaveInput(
    BDSP_StageHandle hStage,
    const BAVC_XptContextMap *pContext,
    unsigned *pInputIndex /* [out] */
    );

/***************************************************************************
Summary:
Remove an input to a stage
***************************************************************************/
void BDSP_Stage_RemoveInput(
    BDSP_StageHandle hStage,
    unsigned inputIndex
    );

/***************************************************************************
Summary:
Remove all inputs to a stage
***************************************************************************/
void BDSP_Stage_RemoveAllInputs(
    BDSP_StageHandle hStage
    );

/***************************************************************************
Summary:
Get TSM settings for a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_GetTsmSettings_isr(
    BDSP_StageHandle stage,
    BDSP_AudioTaskTsmSettings *pTsmSettings         /* [out] */
    );

/***************************************************************************
Summary:
Set TSM settings for a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_SetTsmSettings_isr(
    BDSP_StageHandle stage,
    const BDSP_AudioTaskTsmSettings *pTsmSettings
    );

/***************************************************************************
Summary:
Get TSM status from a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_GetTsmStatus_isr(
    BDSP_StageHandle stage,
    BDSP_AudioTaskTsmStatus *pTsmStatus         /* [out] */
    );

/***************************************************************************
Summary:
Get Datasync settings for a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_GetDatasyncSettings(
    BDSP_StageHandle stage,
    BDSP_AudioTaskDatasyncSettings *pDatasyncSettings         /* [out] */
    );

/***************************************************************************
Summary:
Set Datasync settings for a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_SetDatasyncSettings(
    BDSP_StageHandle stage,
    const BDSP_AudioTaskDatasyncSettings *pDatasyncSettings
    );

/***************************************************************************
Summary:
Get Datasync status from a particular stage
***************************************************************************/
BERR_Code BDSP_AudioStage_GetDatasyncStatus_isr(
    BDSP_StageHandle stage,
    BDSP_AudioTaskDatasyncStatus *pDatasyncStatus         /* [out] */
    );

/* Inter task buffer apis */
/***************************************************************************
Summary:
Create an inter task buffer
***************************************************************************/
BERR_Code BDSP_InterTaskBuffer_Create(
    BDSP_ContextHandle hContext,
    BDSP_DataType dataType,
    BDSP_BufferType bufferType,
    BDSP_InterTaskBufferHandle *pHandle /* [out] */
    );

/***************************************************************************
Summary:
Destroy an inter task buffer
***************************************************************************/
void BDSP_InterTaskBuffer_Destroy(
    BDSP_InterTaskBufferHandle hInterTaskBuffer
    );

/***************************************************************************
Summary:
Flush an inter task buffer
***************************************************************************/
void BDSP_InterTaskBuffer_Flush(
    BDSP_InterTaskBufferHandle hInterTaskBuffer
    );

/*Queue apis */
/***************************************************************************
Summary:
Default Values for Queue Settings
***************************************************************************/
BERR_Code BDSP_Queue_GetDefaultSettings(
                    BDSP_ContextHandle hContext,
                    BDSP_QueueCreateSettings *pSettings);

/***************************************************************************
Summary:
Create an Queue
***************************************************************************/
BERR_Code BDSP_Queue_Create(
    BDSP_ContextHandle hContext,
    unsigned dspIndex,
    BDSP_QueueCreateSettings *pSettings,
    BDSP_QueueHandle *pHandle /* [out] */
    );

/***************************************************************************
Summary:
Destroy an Queue
***************************************************************************/
void BDSP_Queue_Destroy(
    BDSP_QueueHandle hQueue
    );

/***************************************************************************
Summary:
Flush an Queue
***************************************************************************/
void BDSP_Queue_Flush(
    BDSP_QueueHandle hQueue
    );

/***************************************************************************
Summary:
Add an Queue as Input
***************************************************************************/
BERR_Code BDSP_Stage_AddQueueInput(
    BDSP_StageHandle hStage,
    BDSP_QueueHandle hQueue,    /*Queue handle */
    unsigned *pInputIndex /* [out] */    /* input index, reflects the source stage's input index for the queue */
    );

/***************************************************************************
Summary:
Add an Queue as Output
***************************************************************************/
BERR_Code BDSP_Stage_AddQueueOutput(
    BDSP_StageHandle hStage,
    BDSP_QueueHandle hQueue,    /*Queue handle */
    unsigned *pOutputIndex /* [out] */    /* output index, reflects the source stage's output index for the queue. */
    );

/***************************************************************************
Summary:
Get Default Audio Capture Create Settings
***************************************************************************/
void BDSP_AudioCapture_GetDefaultCreateSettings(
    BDSP_AudioCaptureCreateSettings *pSettings  /* [out] */
    );

/***************************************************************************
Summary:
Get Datasync settings for a particular task of video Encode type
***************************************************************************/
BERR_Code BDSP_VideoEncodeTask_GetDatasyncSettings(
    BDSP_StageHandle stage,
    BDSP_VideoEncodeTaskDatasyncSettings *pDataSyncSettings         /* [out] */
    );

/***************************************************************************
Summary:
Set Datasync settings for a particular task of video Encode type
***************************************************************************/
BERR_Code BDSP_VideoEncodeTask_SetDatasyncSettings(
    BDSP_StageHandle stage,
    const BDSP_VideoEncodeTaskDatasyncSettings *pDataSyncSettings
    );

/***************************************************************************
Summary:
Create an audio capture context

Description:
An Audio Capture object is used to capture a copy of the DSP data into host
memory prior to being consumed by intended consumer (e.g. audio mixer hardware).
For the data to be copied, a thread must call BDSP_ProcessAudioCapture() on a
background thread at a frequent interval (e.g. 10ms).

To bind a capture handle to a task and output, you must call
BDSP_AudioCapture_Add before calling BDSP_Task_Start.  After
BDSP_Task_Stop has been called, BDSP_AudioCapture_Remove should
also be called.
***************************************************************************/
BERR_Code BDSP_AudioCapture_Create(
    BDSP_ContextHandle hContext,
    const BDSP_AudioCaptureCreateSettings *pSettings,
    BDSP_AudioCaptureHandle *pCapture   /* [out] */
    );

/***************************************************************************
Summary:
Destroy an audio capture context
***************************************************************************/
void BDSP_AudioCapture_Destroy(
    BDSP_AudioCaptureHandle hCapture
    );

/***************************************************************************
Summary:
Settings to add a capture handle to a task
***************************************************************************/
typedef struct BDSP_StageAudioCaptureSettings
{
    bool updateRead; /* This flag is enabled when there is no consumer for the output buffer
                       and the capture thread is expected to update the buffer read pointers */
} BDSP_StageAudioCaptureSettings;

/***************************************************************************
Summary:
Get Default settings to add a capture handle to a task
***************************************************************************/
void BDSP_Stage_GetDefaultAudioCaptureSettings(
    BDSP_StageAudioCaptureSettings *pSettings /* [out] */
    );

/***************************************************************************
Summary:
Add an audio capture handle to a task

Description:
An Audio Capture object is used to capture a copy of the DSP data into host
memory prior to being consumed by intended consumer (e.g. audio mixer hardware).
For the data to be copied, a thread must call BDSP_ProcessAudioCapture() on a
background thread at a frequent interval (e.g. 10ms).

To bind a capture handle to a task and output, you must call
BDSP_AudioCapture_Add before calling BDSP_Task_Start.  After
BDSP_Task_Stop has been called, BDSP_AudioCapture_Remove should
also be called.
***************************************************************************/
BERR_Code BDSP_Stage_AddAudioCapture(
    BDSP_StageHandle hStage,
    BDSP_AudioCaptureHandle hCapture,
    unsigned outputId,                  /* The output port number that needs to be captured */
    const BDSP_StageAudioCaptureSettings *pSettings  /* Optional, pass NULL for defaults */
    );

/***************************************************************************
Summary:
Remove an audio capture handle from a task
***************************************************************************/
void BDSP_Stage_RemoveAudioCapture(
    BDSP_StageHandle hStage,
    BDSP_AudioCaptureHandle hCapture
    );


/***************************************************************************
Summary:
Get the Buffer Descriptors
***************************************************************************/
BERR_Code BDSP_AudioCapture_GetBuffer(
    BDSP_AudioCaptureHandle hCapture,
    BDSP_BufferDescriptor *pBuffers /* [out] */
    );

/***************************************************************************
Summary:
Consume data from the capture buffers
***************************************************************************/
BERR_Code BDSP_AudioCapture_ConsumeData(
    BDSP_AudioCaptureHandle hCapture,
    unsigned numBytes                   /* Number of bytes read from each buffer */
    );


/***************************************************************************
Summary:
Get Circular Buffer Pointers of Queue for use in algorithm settings.
***************************************************************************/
void BDSP_Queue_GetIoBuffer(
    BDSP_QueueHandle hQueue,
    BDSP_AF_P_sIO_BUFFER *pBuffer /*[out]*/
    );

/***************************************************************************
Summary:
Get the buffer address(es) and size available to read data from the queue.
***************************************************************************/
BERR_Code BDSP_Queue_GetBuffer(
    BDSP_QueueHandle hQueue,
    BDSP_BufferDescriptor *pDescriptor /*[out] */
);

/***************************************************************************
Summary:
Advances read pointer of Queue on checking the BDSP_DataType. Called each
time host reads data from the queue.  If more than one buffer is returned, each buffer is
advanced by the number of bytes specified in readBytes.
***************************************************************************/
BERR_Code BDSP_Queue_ConsumeData(
    BDSP_QueueHandle hQueue,
    size_t readBytes
    );

/***************************************************************************
Summary:
Advances write pointer of Queue on checking the BDSP_DataType. Called each
time host puts data into the queue.  If more than one buffer is returned, each buffer is
advanced by the number of bytes specified in bytesWritten.
***************************************************************************/
BERR_Code BDSP_Queue_CommitData(
    BDSP_QueueHandle hQueue,
    size_t bytesWritten
    );

/***************************************************************************
Summary:
Returns the addresses of the Buffer which is stored as part of RDB by the BDSP back to the VEE
***************************************************************************/
BERR_Code BDSP_Queue_GetBufferAddr(
    BDSP_QueueHandle hQueue,
    unsigned numbuf,
    void *pBuffer
    );

#endif /* BDSP_STAGE_H_ */
