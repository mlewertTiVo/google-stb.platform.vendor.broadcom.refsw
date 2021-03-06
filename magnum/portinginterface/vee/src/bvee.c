/***************************************************************************
 *     Copyright (c) 2003-2014, Broadcom Corporation
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
 * [File Description:]
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ***************************************************************************/
#include "bstd.h"
#include "bkni.h"
#include "bavc.h"
#include "bvee.h"
#include "bvee_priv.h"
#include "bdsp_raaga.h"

BDBG_MODULE(bvee);

BDBG_OBJECT_ID(BVEE_Device);

void BVEE_GetDefaultOpenSettings(
    BVEE_OpenSettings *pVeeOpenSettings    /* [out] */
    )
{
    BDBG_ASSERT(NULL != pVeeOpenSettings);
    BKNI_Memset(pVeeOpenSettings, 0, sizeof(*pVeeOpenSettings));
    pVeeOpenSettings->maxdsptasks = BVEE_MAX_DSP_TASKS;
}

BERR_Code BVEE_Open(
            BVEE_Handle *pHandle,   /* [out] returned handle */
            BCHP_Handle chpHandle,
            BREG_Handle regHandle,
            BMMA_Heap_Handle mmahandle,
            BINT_Handle intHandle,
            BTMR_Handle tmrHandle,
            BDSP_Handle dspHandle,
            const BVEE_OpenSettings *pSettings  /* NULL will use default settings */
            )
{
    BVEE_OpenSettings defaultSettings;
    BDSP_ContextCreateSettings dspContextSettings;
    BVEE_Handle handle;
    BERR_Code errCode;

    /* BDBG_OBJECT_ASSERT(dspHandle, BDSP_Raaga); */

    BDBG_ASSERT(NULL != pHandle);
    BDBG_ASSERT(NULL != chpHandle);
    BDBG_ASSERT(NULL != regHandle);
    BDBG_ASSERT(NULL != mmahandle);
    BDBG_ASSERT(NULL != intHandle);
    BDBG_ASSERT(NULL != tmrHandle);
    BDBG_ASSERT(NULL != dspHandle);
    
    if ( NULL == pSettings )
    {
        BDBG_WRN (("pSettings is NULL. Using Defaults"));
        BVEE_GetDefaultOpenSettings(&defaultSettings);
        pSettings = &defaultSettings;
    }

    /* Allocate device structure */
    handle = BKNI_Malloc(sizeof(BVEE_Device));
    if ( NULL == handle )
    {
        errCode = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        goto err_handle;
    }
    /* Initialize structure */    
    BKNI_Memset(handle, 0, sizeof(BVEE_Device));
    BDBG_OBJECT_SET(handle, BVEE_Device);
    handle->chpHandle = chpHandle;
    handle->regHandle = regHandle;
    handle->mmahandle = mmahandle;
    handle->intHandle = intHandle;
    handle->tmrHandle = tmrHandle;
    handle->dspHandle = dspHandle;
    handle->opensettings = *pSettings;

    /* Create DSP Context */
    BDSP_Context_GetDefaultCreateSettings(handle->dspHandle, BDSP_ContextType_eVideoEncode, &dspContextSettings);
    dspContextSettings.maxTasks = pSettings->maxdsptasks;
    errCode = BDSP_Context_Create(handle->dspHandle, &dspContextSettings, &handle->dspContext);
    if ( errCode )
    {
        (void)BERR_TRACE(errCode);
        goto err_context;
    }
    
    /* Success */
    *pHandle = handle;

    return BERR_SUCCESS;

err_context:
    BDBG_OBJECT_DESTROY(handle, BVEE_Device);
    BKNI_Free(handle);
err_handle:
    *pHandle = NULL;

    return errCode;
}
void BVEE_Close(BVEE_Handle handle)
{
    unsigned i=0;
    BERR_Code errCode;

    BDBG_OBJECT_ASSERT(handle, BVEE_Device);

    /* Stop all Channels first */
#if BVEE_MAX_CHANNELS > 0
    for ( i = 0; i < BVEE_MAX_CHANNELS; i++ )
    {
        if ( handle->channels[i])
        {
            BDBG_MSG(("Stopping channel %p (%d)", handle->channels[i], i));
            BVEE_Channel_Stop(handle->channels[i]);
            BDBG_MSG(("Closing channel %p (%d)", handle->channels[i], i));
            errCode = BVEE_Channel_Close(handle->channels[i]);
            if ( errCode )
            {
                (void)BERR_TRACE(errCode);
            }
        }
    }
#endif

    if ( handle->dspContext )
    {
        BDSP_Context_Destroy(handle->dspContext);
        handle->dspContext = NULL;
    }

    BDBG_OBJECT_DESTROY(handle, BVEE_Device);
    BKNI_Free(handle);
}

void BVEE_GetInterruptHandlers(
    BVEE_Handle handle,
    BVEE_InterruptHandlers *pInterrupts     /* [out] */
    )
{
    BDBG_OBJECT_ASSERT(handle, BVEE_Device);
    BDBG_ASSERT(NULL != pInterrupts);
    *pInterrupts = handle->interrupts;
}

BERR_Code BVEE_SetInterruptHandlers(
    BVEE_Handle handle,
    const BVEE_InterruptHandlers *pInterrupts
    )
{
    BDBG_OBJECT_ASSERT(handle, BVEE_Device);
    BDBG_ASSERT(NULL != pInterrupts);

    if ( NULL != handle->dspContext )
    {
        BERR_Code errCode;
        BDSP_ContextInterruptHandlers contextInterrupts;

        BDSP_Context_GetInterruptHandlers(handle->dspContext, &contextInterrupts);
        contextInterrupts.watchdog.pCallback_isr = pInterrupts->watchdog.pCallback_isr;
        contextInterrupts.watchdog.pParam1 = pInterrupts->watchdog.pParam1;
        contextInterrupts.watchdog.param2 = pInterrupts->watchdog.param2;
        errCode = BDSP_Context_SetInterruptHandlers(handle->dspContext, &contextInterrupts);
        if ( errCode )
        {
            return BERR_TRACE(errCode);
        }
    }

    handle->interrupts = *pInterrupts;

    return BERR_SUCCESS;
}
BERR_Code BVEE_ProcessWatchdogInterrupt(
    BVEE_Handle handle
    )
{
    BERR_Code errCode;
    unsigned i;

    BDBG_OBJECT_ASSERT(handle, BVEE_Device);

    if ( handle->dspContext )
    {
        /* Stop all running channels */
        for ( i = 0; i < BVEE_MAX_CHANNELS; i++ )
        {
            if ( handle->channels[i] )
            {
                handle->channelWatchdogInfo[i].state = handle->channels[i]->state;
                if ( handle->channelWatchdogInfo[i].state != BVEE_ChannelState_eStopped )
                {
                    handle->channelWatchdogInfo[i].startsettings = handle->channels[i]->startsettings;
                    BVEE_Channel_Stop(handle->channels[i]);
                }
            }
            else
            {
                handle->channelWatchdogInfo[i].state = BVEE_ChannelState_eMax;
            }
        }

        /* Reboot the DSP */
        errCode = BDSP_Context_ProcessWatchdogInterrupt(handle->dspContext);
        if ( errCode )
        {
            return BERR_TRACE(errCode);
        }

		/* Propogate the Watchdog Flag to VEE Internal/Private functions. This Flag will be reset just before the Start Command is issued*/
		handle->VEEWatchdogFlag = true;

        /* Reset all channel state */
        for ( i = 0; i < BVEE_MAX_CHANNELS; i++ )
        {
            if ( handle->channels[i] )
            {
                if ( handle->channelWatchdogInfo[i].state != BVEE_ChannelState_eStopped )
                {
                    /* Restart channel */
                    errCode = BVEE_Channel_Start(handle->channels[i], &handle->channelWatchdogInfo[i].startsettings);
                    if ( errCode )
                    {
                        BDBG_ERR(("Error restarting channel %d", i));
                        errCode = BERR_TRACE(errCode);
                    }
                }
            }
        }
    }

    return BERR_SUCCESS;
}
BERR_Code BVEE_GetA2PDelay(uint32_t *delayms,BVEE_Resolution resolution,BAVC_FrameRateCode framerate, uint32_t targetbitrate)
{
    BSTD_UNUSED(resolution);
    BSTD_UNUSED(targetbitrate);
	
    *delayms = 0;
    if((framerate == BAVC_FrameRateCode_e14_985)||(framerate == BAVC_FrameRateCode_e15)||(framerate == BAVC_FrameRateCode_e7_493))
    {
        *delayms = 1600;
        return BERR_SUCCESS;
    }
    else if((framerate == BAVC_FrameRateCode_e29_97)||(framerate == BAVC_FrameRateCode_e30))
    {
        *delayms = 900;
        return BERR_SUCCESS;
    }

    *delayms = 900; /*Default A2P Delay*/
    return BERR_SUCCESS;
}
