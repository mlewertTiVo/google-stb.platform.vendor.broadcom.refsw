/***************************************************************************
 *     Copyright (c) 2006-2013, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * Module Description: Audio Output Capture Interface
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/

#ifndef BAPE_OUTPUT_CAPTURE_H_
#define BAPE_OUTPUT_CAPTURE_H_

/***************************************************************************
Summary:
Output Capture Handle
***************************************************************************/
typedef struct BAPE_OutputCapture *BAPE_OutputCaptureHandle;

/***************************************************************************
Summary:
Output Capture Open Settings
***************************************************************************/
typedef struct BAPE_OutputCaptureOpenSettings
{
    BMEM_Heap_Handle heap;          /* Heap used for allocating buffers.  If not set, the heap
                                       provided in BAPE_Open will be used. */
    unsigned numBuffers;            /* Number of buffers to allocate.  For mono or interleaved stereo,
                                       one buffer is required.  For non-interleaved 7.1 data 8 buffers
                                       are required.  For interleaved 7.1 data, 4 buffers are required. */
    unsigned bufferSize;            /* Buffer size in bytes, must be a multiple of 256.  If more than one
                                       buffer is allocated, all buffers will be this size.  If this size
                                       is 0, buffers will be allocated from the internal buffer pool. */
    unsigned watermarkThreshold;    /* FIFO interrupt threshold in bytes.  
                                       When an amuont >= this threshold is available, 
                                       an interrupt will be raised. This value should
                                       be a multiple of 256. */    
} BAPE_OutputCaptureOpenSettings;

/***************************************************************************
Summary:
Get Default Output Capture Open Settings
***************************************************************************/
void BAPE_OutputCapture_GetDefaultOpenSettings(
    BAPE_OutputCaptureOpenSettings *pSettings       /* [out] */
    );

/***************************************************************************
Summary:
Open a capture output
***************************************************************************/
BERR_Code BAPE_OutputCapture_Open(
    BAPE_Handle deviceHandle,
    unsigned index,
    const BAPE_OutputCaptureOpenSettings *pSettings,
    BAPE_OutputCaptureHandle *pHandle             /* [out] */
    );

/***************************************************************************
Summary:
Close a capture output
***************************************************************************/
void BAPE_OutputCapture_Close(
    BAPE_OutputCaptureHandle handle
    );

/***************************************************************************
Summary:
OutputCapture Settings
***************************************************************************/
typedef struct BAPE_OutputCaptureSettings
{
    unsigned bitsPerSample;                 /* Currently, 32-bit PCM capture is supported on all chips and
                                               16 bit captures is supported on HW that supports it.  */
    bool interleaveData;                    /* If true, PCM data for a stereo pair will be interleaved
                                               into a single buffer.  False will use a separate buffer
                                               for each channel. */
    unsigned watermark;                     /* FIFO interrupt threshold in bytes.  
                                               When an amuont >= this threshold is available,
                                               an interrupt will be raised. This value should
                                               be a multiple of 256. */    
} BAPE_OutputCaptureSettings;

/***************************************************************************
Summary:
Get OutputCapture Settings
***************************************************************************/
void BAPE_OutputCapture_GetSettings(
    BAPE_OutputCaptureHandle handle,
    BAPE_OutputCaptureSettings *pSettings       /* [out] */
    );

/***************************************************************************
Summary:
Set OutputCapture Settings
***************************************************************************/
BERR_Code BAPE_OutputCapture_SetSettings(
    BAPE_OutputCaptureHandle handle,
    const BAPE_OutputCaptureSettings *pSettings
    );

/***************************************************************************
Summary:
Flush OutputCapture Buffer
***************************************************************************/
void BAPE_OutputCapture_Flush(
    BAPE_OutputCaptureHandle handle
    );

/***************************************************************************
Summary:
Flush OutputCapture Buffer from interrupt context
***************************************************************************/
void BAPE_OutputCapture_Flush_isr(
    BAPE_OutputCaptureHandle handle
    );

/***************************************************************************
Summary:
Get Capture Buffer 
 
Description: 
This routine will return the next contiguous buffer address and size.  If 
interrupts are enabled, the caller should call this routine and commit data 
until the size returned from this function is zero. 
***************************************************************************/
BERR_Code BAPE_OutputCapture_GetBuffer(
    BAPE_OutputCaptureHandle handle,
    BAPE_BufferDescriptor *pBuffers      /* [out] */
    );

/***************************************************************************
Summary:
Mark data as consumed from the OutputCapture hardware
***************************************************************************/
BERR_Code BAPE_OutputCapture_ConsumeData(
    BAPE_OutputCaptureHandle handle,
    unsigned numBytes                   /* Number of bytes read from the buffer */
    );

/***************************************************************************
Summary:
Get Audio Output Connector
***************************************************************************/
void BAPE_OutputCapture_GetOutputPort(
    BAPE_OutputCaptureHandle handle,
    BAPE_OutputPort *pConnector
    );

/***************************************************************************
Summary:
Audio Output Capture Interrupts
***************************************************************************/
typedef struct BAPE_OutputCaptureInterruptHandlers
{
    struct
    {
        void (*pCallback_isr)(void *pParam1, int param2);
        void *pParam1;
        int param2;
    } watermark;
    struct
    {
        void (*pCallback_isr)(void *pParam1, int param2);
        void *pParam1;
        int param2;
    } overflow;
    struct
    {
        void (*pCallback_isr)(void *pParam1, int param2, unsigned sampleRate);
        void *pParam1;
        int param2;
    } sampleRate;
} BAPE_OutputCaptureInterruptHandlers;

/***************************************************************************
Summary:
Get Audio Output Capture Interrupts
***************************************************************************/
void BAPE_OutputCapture_GetInterruptHandlers(
    BAPE_OutputCaptureHandle handle,
    BAPE_OutputCaptureInterruptHandlers *pInterrupts    /* [out] */
    );

/***************************************************************************
Summary:
Set Audio Output Capture Interrupts
***************************************************************************/
BERR_Code BAPE_OutputCapture_SetInterruptHandlers(
    BAPE_OutputCaptureHandle handle,
    const BAPE_OutputCaptureInterruptHandlers *pInterrupts
    );

#endif /* #ifndef BAPE_OUTPUT_CAPTURE_H_ */

