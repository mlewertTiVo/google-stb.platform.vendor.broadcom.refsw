/******************************************************************************
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
 ******************************************************************************/
#include "nexus_simple_decoder_module.h"

/* internal apis */
#include "nexus_video_decoder.h"
#include "nexus_video_decoder_extra.h"
#include "nexus_video_decoder_trick.h"
#include "nexus_video_decoder_primer.h"
#include "nexus_mosaic_video_decoder.h"
#include "priv/nexus_video_decoder_priv.h"
#include "nexus_video_window.h"
#include "nexus_video_input.h"
#include "nexus_video_input_crc.h"
#include "nexus_graphics2d.h"
#include "nexus_core_utils.h"
#include "nexus_client_resources.h"
#include "priv/nexus_pid_channel_priv.h"
#include "priv/nexus_stc_channel_priv.h"
#include "nexus_simple_decoder_impl.h"
#include "nexus_picture_ctrl.h"
#include "nexus_video_adj.h"
#include "nexus_display_vbi.h"
#if NEXUS_HAS_HDMI_INPUT
#include "priv/nexus_hdmi_input_priv.h"
#endif
#include "priv/nexus_display_priv.h"
#include "priv/nexus_video_window_priv.h"
#include "priv/nexus_core.h"

BDBG_MODULE(nexus_simple_video_decoder);

#define BDBG_MSG_TRACE(X)

struct nexus_captured_surface {
    NEXUS_SurfaceHandle surface;
    NEXUS_StripedSurfaceHandle stripedSurface;
    NEXUS_SimpleVideoDecoderCaptureStatus status;
    enum {
        nexus_captured_surface_empty,
        nexus_captured_surface_striped,    /* have striped surface, but haven't submitted to Graphics2D */
        nexus_captured_surface_destriping, /* waiting on Graphics2D */
        nexus_captured_surface_destriped,  /* destripe done. striped surface returned. */
        nexus_captured_surface_user        /* destriped surface given to user */
    } state;
};

static struct nexus_simplevideodecoder_defaultsettings {
    NEXUS_VideoDecoderSettings settings;
    NEXUS_VideoDecoderPlaybackSettings playbackSettings;
    NEXUS_VideoDecoderExtendedSettings extendedSettings;
} *g_default;

struct NEXUS_SimpleVideoDecoder
{
    NEXUS_OBJECT(NEXUS_SimpleVideoDecoder);
    BLST_S_ENTRY(NEXUS_SimpleVideoDecoder) link;
    unsigned index;
    bool acquired;
    bool started; /* decode is actually started. if started, must be connected. may or may not be acquired. if priming, this should be false. */
    bool clientStarted; /* user has started, StartSettings handles have been acquired. */
    bool connected; /* to VideoWindow */
    NEXUS_SimpleVideoDecoderServerSettings serverSettings;
    NEXUS_SimpleVideoDecoderStartSettings startSettings;
    NEXUS_VideoDecoderTrickState trickSettings; /* default is normal play */
    NEXUS_SimpleVideoDecoderClientSettings clientSettings;
    NEXUS_TaskCallbackHandle resourceChangedCallback;
    NEXUS_SimpleStcChannelHandle stcChannel;
    struct {
        NEXUS_SimpleEncoderHandle handle;
        NEXUS_VideoWindowHandle window;
    } encoder;
    NEXUS_SimpleVideoDecoderPictureQualitySettings pictureQualitySettings; /* default is all zeros, plus GetDefault for dnr/anr */
    NEXUS_VideoDecoderPlaybackSettings playbackSettings;
    NEXUS_VideoDecoderExtendedSettings extendedSettings;
    NEXUS_VideoDecoderSettings settings;

    struct {
        NEXUS_SimpleVideoDecoderStartCaptureSettings settings;
        bool started;
        struct nexus_captured_surface destripe[NEXUS_SIMPLE_DECODER_MAX_SURFACES];
        unsigned wptr, rptr, total; /* wptr moves on empty -> striped, rptr moves on user -> empty */
        bool displayConnected;
        struct {
            unsigned pictures;
            unsigned overflows;
        } stats;
    } capture;

    struct {
        NEXUS_VideoDecoderPrimerHandle handle;
        bool started; /* VideoDecoderPrimer has been started, including StopPrimerAndStartDecode. requires VideoDecoderPrimer_Stop to release */
        NEXUS_VideoDecoderPrimerSettings settings;
    } primer;

    struct {
        NEXUS_VideoImageInputHandle handle;
        /* no clientStarted or started because an internal stop closes the interface. */
    } imageInput;
    struct {
        NEXUS_HdmiInputHandle handle;
    } hdmiInput;
    struct {
        NEXUS_HdDviInputHandle handle;
    } hdDviInput;
};

static BLST_S_HEAD(NEXUS_SimpleVideoDecoder_P_List, NEXUS_SimpleVideoDecoder) g_videoDecoders;

static struct {
    struct {
        unsigned refcnt;
        NEXUS_TimerHandle timer;
        NEXUS_Graphics2DHandle gfx;
        bool checkpointPending;
    } videoAsGraphics;
    struct {
        NEXUS_SimpleVideoDecoderHandle handle;
        NEXUS_VideoInput videoInput;
    } sdOverride;
} g_state;

static NEXUS_Error nexus_simplevideodecoder_p_setdecodersettings(NEXUS_SimpleVideoDecoderHandle handle, bool currentSettings);
static NEXUS_Error nexus_simplevideodecoder_p_start( NEXUS_SimpleVideoDecoderHandle handle );
static void nexus_simplevideodecoder_p_stop( NEXUS_SimpleVideoDecoderHandle handle );
static NEXUS_Error nexus_simplevideodecoder_p_connect(NEXUS_SimpleVideoDecoderHandle handle);
static void nexus_simplevideodecoder_p_disconnect(NEXUS_SimpleVideoDecoderHandle handle, bool allow_cache);

static void nexus_simple_p_close_primer(NEXUS_SimpleVideoDecoderHandle handle);

/* server settings cache */
static bool add_settings_to_cache(NEXUS_SimpleVideoDecoderHandle handle);
static bool use_cache(NEXUS_SimpleVideoDecoderHandle handle);

/**
server functions
**/

void NEXUS_SimpleVideoDecoder_GetDefaultServerSettings( NEXUS_SimpleVideoDecoderServerSettings *pSettings )
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
    pSettings->enabled = true;
    pSettings->stcIndex = -1;
}

void NEXUS_SimpleVideoDecoderModule_LoadDefaultSettings( NEXUS_VideoDecoderHandle videoDecoder )
{
    if (!g_default) {
        g_default = BKNI_Malloc(sizeof(*g_default));
        if (!g_default) return;
        BKNI_Memset(g_default, 0, sizeof(*g_default));
    }
    NEXUS_VideoDecoder_GetSettings(videoDecoder, &g_default->settings);
    NEXUS_VideoDecoder_GetExtendedSettings(videoDecoder, &g_default->extendedSettings);

    /* default to an orthogonal feature set */
    g_default->settings.supportedCodecs[NEXUS_VideoCodec_eH264_Svc] = false;
    g_default->settings.supportedCodecs[NEXUS_VideoCodec_eH264_Mvc] = false;
    g_default->settings.colorDepth = 10; /* will be lowered to 8 bit if decoder doesn't support 10 */
    if (g_default->settings.maxHeight > 1088) {
        g_default->settings.maxWidth = 1920;
        g_default->settings.maxHeight = 1080;
    }
}

void NEXUS_SimpleVideoDecoderModule_P_UnloadDefaultSettings(void)
{
    if (g_default) {
        BKNI_Free(g_default);
        g_default = NULL;
    }
}

static void nexus_simplevideodecoder_p_set_default_settings(NEXUS_SimpleVideoDecoderHandle handle)
{
    if (g_default) {
        handle->settings = g_default->settings;
        handle->extendedSettings = g_default->extendedSettings;
        handle->playbackSettings = g_default->playbackSettings;
    }
    NEXUS_VideoDecoder_GetNormalPlay(&handle->trickSettings);
    BKNI_Memset(&handle->pictureQualitySettings, 0, sizeof(handle->pictureQualitySettings));
    NEXUS_VideoWindow_GetDefaultDnrSettings(&handle->pictureQualitySettings.dnr);
    NEXUS_VideoWindow_GetDefaultAnrSettings(&handle->pictureQualitySettings.anr);
    NEXUS_VideoWindow_GetDefaultMadSettings(&handle->pictureQualitySettings.mad);
    NEXUS_VideoWindow_GetDefaultScalerSettings(&handle->pictureQualitySettings.scaler);
    handle->clientSettings.closedCaptionRouting = true;
}

NEXUS_SimpleVideoDecoderHandle NEXUS_SimpleVideoDecoder_Create( unsigned index, const NEXUS_SimpleVideoDecoderServerSettings *pSettings )
{
    NEXUS_SimpleVideoDecoderHandle handle;
    NEXUS_Error rc;

    /* find dup */
    for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle=BLST_S_NEXT(handle, link)) {
        if (handle->index == index) {
            BERR_TRACE(NEXUS_INVALID_PARAMETER);
            return NULL;
        }
    }

    handle = BKNI_Malloc(sizeof(*handle));
    if (!handle) {
        BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
        return NULL;
    }
    NEXUS_OBJECT_INIT(NEXUS_SimpleVideoDecoder, handle);
    handle->index = index;
    NEXUS_OBJECT_REGISTER(NEXUS_SimpleVideoDecoder, handle, Create);

    /* insert in order. not required, but makes debug easier */
    if (!BLST_S_FIRST(&g_videoDecoders)) {
        BLST_S_INSERT_HEAD(&g_videoDecoders, handle, link);
    }
    else {
        NEXUS_SimpleVideoDecoderHandle prev;
        for (prev=BLST_S_FIRST(&g_videoDecoders);;prev=BLST_S_NEXT(prev, link)) {
            if (!BLST_S_NEXT(prev, link)) {
                BLST_S_INSERT_AFTER(&g_videoDecoders, prev, handle, link);
                break;
            }
        }
    }
    handle->resourceChangedCallback = NEXUS_TaskCallback_Create(handle, NULL);
    nexus_simplevideodecoder_p_set_default_settings(handle);
    /* now a valid object */

    rc = NEXUS_SimpleVideoDecoder_SetServerSettings(handle, pSettings);
    if (rc) { rc = BERR_TRACE(rc); goto error; }

    return handle;

error:
    NEXUS_SimpleVideoDecoder_Destroy(handle);
    return NULL;
}

/* stop or clear so that we're not holding anything open */
static void NEXUS_SimpleVideoDecoder_P_ReleaseResources( NEXUS_SimpleVideoDecoderHandle handle )
{
    if (handle == g_state.sdOverride.handle) {
        NEXUS_SimpleVideoDecoder_SetSdOverride(handle, false);
    }
    NEXUS_SimpleVideoDecoder_StopAndFree(handle);
    NEXUS_SimpleVideoDecoder_SetStcChannel(handle, NULL);
}

static void NEXUS_SimpleVideoDecoder_P_Release( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_OBJECT_ASSERT(NEXUS_SimpleVideoDecoder, handle);
    if (handle->acquired) {
        NEXUS_SimpleVideoDecoder_Release(handle);
    }
    NEXUS_SimpleVideoDecoder_P_ReleaseResources(handle); /* SimpleVideoDecoder may be used without Acquire */
    BDBG_ASSERT(!handle->acquired);
    BDBG_ASSERT(!handle->connected);
    BDBG_ASSERT(!handle->started);
    BDBG_ASSERT(!handle->primer.handle);
    NEXUS_OBJECT_UNREGISTER(NEXUS_SimpleVideoDecoder, handle, Destroy);
    return;
}

static void NEXUS_SimpleVideoDecoder_P_Finalizer( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_OBJECT_ASSERT(NEXUS_SimpleVideoDecoder, handle);
    BLST_S_REMOVE(&g_videoDecoders, handle, NEXUS_SimpleVideoDecoder, link);
    if (handle->resourceChangedCallback) {
        NEXUS_TaskCallback_Destroy(handle->resourceChangedCallback);
    }
    NEXUS_OBJECT_DESTROY(NEXUS_SimpleVideoDecoder, handle);
    BKNI_Free(handle);
    return;
}

NEXUS_OBJECT_CLASS_MAKE_WITH_RELEASE(NEXUS_SimpleVideoDecoder, NEXUS_SimpleVideoDecoder_Destroy);

void NEXUS_SimpleVideoDecoder_GetServerSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleVideoDecoderServerSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->serverSettings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetServerSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderServerSettings *pSettings )
{
    NEXUS_Error rc;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    /* testing for loss of secondary windows. this is a specific case for nxserver. */
    if (handle->serverSettings.videoDecoder && handle->serverSettings.videoDecoder == pSettings->videoDecoder && !pSettings->window[1] && handle->serverSettings.window[1]) {
        /* now verify the loss of secondary is the only change */
        NEXUS_SimpleVideoDecoderServerSettings temp = *pSettings;
        temp.window[1] = handle->serverSettings.window[1];
        temp.display[1] = handle->serverSettings.display[1];
        if (!BKNI_Memcmp(&temp, &handle->serverSettings, sizeof(temp))) {
            unsigned i;
            BDBG_WRN(("removing secondary display from decoder %p", (void*)handle));
            for (i=1;i<NEXUS_MAX_DISPLAYS;i++) {
                if (handle->serverSettings.window[i]) {
                    NEXUS_VideoWindow_RemoveAllInputs(handle->serverSettings.window[i]);
                }
            }
            handle->serverSettings = *pSettings;
            return 0;
        }
    }

    if (handle->imageInput.handle) {
        /* user stop. there is no auto-restart for image input. */
        NEXUS_SimpleVideoDecoder_Stop(handle);
    }
    else {
        /* otherwise do internal disconnect so we can resume when able. */
        nexus_simplevideodecoder_p_stop(handle);
        nexus_simplevideodecoder_p_disconnect(handle, true);
    }

    handle->serverSettings = *pSettings;

    if (!g_default && handle->serverSettings.videoDecoder) {
        /* this is added for backward compat with the risk that we wipe out client settings already made */
        NEXUS_SimpleVideoDecoderHandle d;
        NEXUS_SimpleVideoDecoderModule_LoadDefaultSettings(handle->serverSettings.videoDecoder);
        for (d=BLST_S_FIRST(&g_videoDecoders); d; d=BLST_S_NEXT(d, link)) {
            nexus_simplevideodecoder_p_set_default_settings(d);
        }
    }

    if (handle->stcChannel)
    {
        /* notify ssc that something may have changed */
        NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);
    }

    if (handle->serverSettings.videoDecoder && handle->serverSettings.enabled) {
        rc = NEXUS_SimpleVideoDecoder_SetPictureQualitySettings(handle, &handle->pictureQualitySettings);
        if (rc) {rc = BERR_TRACE(rc);} /* fall through */

        if (handle->clientStarted && !handle->primer.started) {
            rc = nexus_simplevideodecoder_p_start(handle);
            if (rc) {rc = BERR_TRACE(rc);} /* fall through */

            if (!rc) {
                rc = NEXUS_SimpleVideoDecoder_SetTrickState(handle, &handle->trickSettings);
                if (rc) {rc = BERR_TRACE(rc);} /* fall through */
            }
        }
    }

    NEXUS_TaskCallback_Fire(handle->resourceChangedCallback);

    return 0;
}

void NEXUS_SimpleVideoDecoder_GetStcStatus_priv(NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleStcChannelDecoderStatus * pStatus)
{
    NEXUS_OBJECT_ASSERT(NEXUS_SimpleVideoDecoder, handle);
    pStatus->connected = (handle->serverSettings.videoDecoder && handle->serverSettings.enabled) ||
                         handle->hdmiInput.handle || handle->hdDviInput.handle;
    pStatus->started = handle->started;
    pStatus->stcIndex = handle->serverSettings.stcIndex;
    pStatus->primer = handle->primer.started && !handle->started;
    pStatus->hdDviInput = handle->hdmiInput.handle || handle->hdDviInput.handle;
    NEXUS_SimpleEncoder_GetStcStatus_priv(handle->encoder.handle, &pStatus->encoder);
}

/**
client functions
**/

static NEXUS_Error nexus_simplevideodecoder_has_resource(NEXUS_SimpleVideoDecoderHandle handle, NEXUS_Error *pResult)
{
    if (handle->serverSettings.videoDecoder && handle->serverSettings.enabled && !handle->hdmiInput.handle) {
        *pResult = 0;
        return 0;
    }
    switch (handle->serverSettings.disableMode) {
    case NEXUS_SimpleDecoderDisableMode_eSuccess:
    case NEXUS_SimpleDecoderDisableMode_eFailOnStart:
        *pResult = 0;
        break;
    default:
        *pResult = NEXUS_SIMPLE_DECODER_NOT_ENABLED;
        break;
    }
    return NEXUS_SIMPLE_DECODER_NOT_ENABLED;
}

NEXUS_SimpleVideoDecoderHandle NEXUS_SimpleVideoDecoder_Acquire( unsigned index )
{
    NEXUS_Error rc;
    NEXUS_SimpleVideoDecoderHandle handle;

    for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle = BLST_S_NEXT(handle, link)) {
        BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
        if (handle->index == index) {
            if (handle->acquired) {
                BERR_TRACE(NEXUS_NOT_AVAILABLE);
                return NULL;
            }
            rc = NEXUS_CLIENT_RESOURCES_ACQUIRE(simpleVideoDecoder,IdList,index);
            if (rc) { rc = BERR_TRACE(rc); return NULL; }
            handle->acquired = true;
            return handle;
        }
    }
    return NULL;
}

void NEXUS_SimpleVideoDecoder_Release( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    /* IPC handle validation will only allow this call if handle is owned by client.
    For non-IPC used, acquiring is not required, so acquired boolean is not checked in any other API. */
    if (!handle->acquired) {
        BERR_TRACE(NEXUS_NOT_AVAILABLE);
        return;
    }
    handle->acquired = false;
    NEXUS_SimpleVideoDecoder_P_ReleaseResources(handle);
    nexus_simplevideodecoder_p_set_default_settings(handle);

    NEXUS_CLIENT_RESOURCES_RELEASE(simpleVideoDecoder,IdList,handle->index);
}

void NEXUS_SimpleVideoDecoder_GetDefaultStartSettings( NEXUS_SimpleVideoDecoderStartSettings *pSettings )
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
    NEXUS_VideoDecoder_GetDefaultStartSettings(&pSettings->settings);
    pSettings->maxWidth = 1920;
    pSettings->maxHeight = 1080;
    pSettings->displayEnabled = true;
    pSettings->lowDelayImageInput = true;
}

static NEXUS_VideoInput nexus_simplevideodecoder_p_getinput(NEXUS_SimpleVideoDecoderHandle handle)
{
    if (handle->hdmiInput.handle) {
#if NEXUS_HAS_HDMI_INPUT
        return NEXUS_HdmiInput_GetVideoConnector(handle->hdmiInput.handle);
#endif
    }
    else if (handle->hdDviInput.handle) {
        return NEXUS_HdDviInput_GetConnector(handle->hdDviInput.handle);
    }
    else if (handle->imageInput.handle) {
        return NEXUS_VideoImageInput_GetConnector(handle->imageInput.handle);
    }
    else if (handle->serverSettings.videoDecoder) {
        return NEXUS_VideoDecoder_GetConnector(handle->serverSettings.videoDecoder);
    }
    return NULL;
}

static bool nexus_simplevideodecoder_p_nondecoder(NEXUS_SimpleVideoDecoderHandle handle)
{
    return handle->hdmiInput.handle || handle->hdDviInput.handle || handle->imageInput.handle;
}

static NEXUS_Error nexus_simplevideodecoder_p_connect(NEXUS_SimpleVideoDecoderHandle handle)
{
    unsigned i;
    NEXUS_Error rc;
    bool disp = handle->startSettings.displayEnabled;
    NEXUS_VideoInput videoInput;

    if (handle->capture.started) {
        disp = handle->capture.settings.displayEnabled; /* StartCaptureSettings.displayEnabled overrides StartSettings.displayEnabled */
    }

    if (handle->connected) return 0;

    BDBG_MSG(("nexus_simplevideodecoder_p_connect %p", (void *)handle));
    rc = nexus_simplevideodecoder_p_setdecodersettings(handle, true);
    if (rc) return BERR_TRACE(rc);

    videoInput = nexus_simplevideodecoder_p_getinput(handle);
    if (!videoInput) {
        return BERR_TRACE(-1);
    }

    if (handle->encoder.window) {
        rc = NEXUS_VideoWindow_AddInput(handle->encoder.window, videoInput);
        if (rc) return BERR_TRACE(rc);
    }

    if (disp) {
        if (!use_cache(handle)) {
            NEXUS_SimpleVideoDecoder_SetClientSettings(handle, &handle->clientSettings);
            NEXUS_SimpleVideoDecoder_SetPictureQualitySettings(handle, &handle->pictureQualitySettings);

            for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
                NEXUS_VideoWindowHandle window = handle->serverSettings.window[i];
                if (window) {
                    NEXUS_VideoWindowSettings windowSettings;
                    NEXUS_VideoWindowScalerSettings scalerSettings;
                    bool change;

                    NEXUS_Module_Lock(g_NEXUS_simpleDecoderModuleSettings.modules.display);
                    NEXUS_VideoWindow_GetSettings_priv(window, &windowSettings);
                    change = false;
                    if (g_pCoreHandles->boxConfig->stBox.ulBoxId == 0) {
                        if (handle->startSettings.smoothResolutionChange && i == 0) {
                            if (!windowSettings.minimumDisplayFormat) {
                                NEXUS_VideoWindow_GetDefaultMinDisplayFormat_isrsafe(window, &windowSettings.minimumDisplayFormat);
                                windowSettings.allocateFullScreen = true;
                                change = true;
                            }
                        }
                        else {
                            if (windowSettings.minimumDisplayFormat) {
                                windowSettings.minimumDisplayFormat = NEXUS_VideoFormat_eUnknown;
                                windowSettings.allocateFullScreen = false;
                                change = true;
                            }
                        }
                    }
                    /* for box mode systems, the only effect of smoothResolutionChange is disabling scaleFactorRounding */
                    if (windowSettings.scaleFactorRounding.enabled != !handle->startSettings.smoothResolutionChange) {
                        windowSettings.scaleFactorRounding.enabled = !handle->startSettings.smoothResolutionChange;
                        change = true;
                    }
                    if (change) {
                        NEXUS_VideoWindow_SetSettings_priv(window, &windowSettings);
                    }
                    NEXUS_Module_Unlock(g_NEXUS_simpleDecoderModuleSettings.modules.display);

                    if (g_pCoreHandles->boxConfig->stBox.ulBoxId == 0) {
                        /* no priv for these functions because they are not called elsewhere in nxclient system */
                        NEXUS_VideoWindow_GetScalerSettings(window, &scalerSettings);
                        change = false;
                        if (handle->startSettings.smoothResolutionChange && i == 0) {
                            if (scalerSettings.bandwidthEquationParams.bias != NEXUS_ScalerCaptureBias_eScalerBeforeCapture) {
                                scalerSettings.bandwidthEquationParams.bias = NEXUS_ScalerCaptureBias_eScalerBeforeCapture;
                                scalerSettings.bandwidthEquationParams.delta = 1*1000*1000;
                                change = true;
                            }
                        }
                        else {
                            NEXUS_VideoWindowScalerSettings defaultSettings;
                            NEXUS_VideoWindow_GetDefaultScalerSettings(&defaultSettings);
                            /* avoid calling SetScalerSettings unless there's really a change */
                            if (scalerSettings.bandwidthEquationParams.bias != defaultSettings.bandwidthEquationParams.bias ||
                                scalerSettings.bandwidthEquationParams.delta != defaultSettings.bandwidthEquationParams.delta) {
                                scalerSettings.bandwidthEquationParams = defaultSettings.bandwidthEquationParams;
                                change = true;
                            }
                        }
                        if (change) {
                            NEXUS_VideoWindow_SetScalerSettings(window, &scalerSettings);
                        }
                    }

                    if (i == 1 && g_state.sdOverride.videoInput) {
                        rc = NEXUS_VideoWindow_AddInput(window, g_state.sdOverride.videoInput);
                        if (rc) { rc = BERR_TRACE(rc); goto error; }
                    }
                    else {
                        rc = NEXUS_VideoWindow_AddInput(window, videoInput);
                        if (rc) { rc = BERR_TRACE(rc); goto error; }
                    }
                }
            }
        }
    }
    else {
        NEXUS_SimpleVideoDecoderModule_CheckCache(NULL, handle->serverSettings.videoDecoder);
        rc = NEXUS_VideoDecoder_SetPowerState(handle->serverSettings.videoDecoder, true);
        if (rc) {rc = BERR_TRACE(rc); goto error;}
        handle->capture.displayConnected = true;
    }

    handle->connected = true;
    return 0;

error:
    nexus_simplevideodecoder_p_disconnect(handle, false);
    return rc;
}

static void nexus_simplevideodecoder_p_disconnect_settings(const NEXUS_SimpleVideoDecoderServerSettings *pSettings, NEXUS_SimpleVideoDecoderHandle handle)
{
    NEXUS_VideoInput videoInput;
    if (handle) {
        if (g_state.sdOverride.videoInput) {
            NEXUS_VideoWindowHandle window = handle->serverSettings.window[1];
            if (window) {
                NEXUS_VideoWindow_RemoveAllInputs(window);
            }
        }
        videoInput = nexus_simplevideodecoder_p_getinput(handle);
    }
    else if (pSettings->videoDecoder) {
        videoInput = NEXUS_VideoDecoder_GetConnector(pSettings->videoDecoder);
    }
    else {
        videoInput = NULL;
    }

    if (videoInput) {
        /* display module will internally call NEXUS_VideoWindow_RemoveInput in optimal way */
        NEXUS_VideoInput_Shutdown(videoInput);
    }
    if (handle) {
        /* only reset video decoder settings back to default when shutting down */
        nexus_simplevideodecoder_p_setdecodersettings(handle, false);
    }
}

static NEXUS_Error nexus_simplevideodecoder_p_setvbisetings(NEXUS_SimpleVideoDecoderHandle handle, bool closedCaptionRouting)
{
    unsigned i;
    bool connect = closedCaptionRouting && handle->serverSettings.videoDecoder;

    if (nexus_simplevideodecoder_p_nondecoder(handle)) {
        return 0;
    }

    if (!connect) {
        /* if disconnecting, try to throw to another connected decoder */
        NEXUS_SimpleVideoDecoderHandle d;
        for (d=BLST_S_FIRST(&g_videoDecoders); d; d=BLST_S_NEXT(d, link)) {
            if (d != handle && d->connected && d->clientSettings.closedCaptionRouting && d->serverSettings.display[0] == handle->serverSettings.display[0]) {
                return nexus_simplevideodecoder_p_setvbisetings(d, true);
            }
        }
    }

    for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
        if (handle->serverSettings.display[i]) {
            NEXUS_DisplayVbiSettings settings;
            NEXUS_Display_GetVbiSettings(handle->serverSettings.display[i], &settings);
            if (settings.closedCaptionEnabled) {
                int rc;
                if (connect) {
                    settings.vbiSource = NEXUS_VideoDecoder_GetConnector(handle->serverSettings.videoDecoder);
                    settings.closedCaptionRouting = true;
                }
                else {
                    settings.vbiSource = NULL;
                    settings.closedCaptionRouting = false;
                }
                rc = NEXUS_Display_SetVbiSettings(handle->serverSettings.display[i], &settings);
                if (rc) return BERR_TRACE(rc);
            }
        }
    }
    return 0;
}

static void nexus_simplevideodecoder_p_disconnect(NEXUS_SimpleVideoDecoderHandle handle, bool allow_cache)
{
    if (!handle->connected) return;

    if (handle->hdmiInput.handle) {
        allow_cache = false;
    }
    if (g_state.sdOverride.handle) {
        allow_cache = false;
    }

    BDBG_MSG(("nexus_simplevideodecoder_p_disconnect %p", (void *)handle));
    if (handle->encoder.window) {
        NEXUS_VideoWindow_RemoveAllInputs(handle->encoder.window);
    }
    if (handle->capture.displayConnected) {
        NEXUS_VideoDecoder_SetPowerState(handle->serverSettings.videoDecoder, false);
        handle->capture.displayConnected = false;
    }
    else if (!allow_cache || !add_settings_to_cache(handle)) {
        nexus_simplevideodecoder_p_disconnect_settings(&handle->serverSettings, handle);
    }

    handle->connected = false;
}

static void nexus_simplevideodecoder_p_reset_cap(NEXUS_SimpleVideoDecoderHandle handle, struct nexus_captured_surface *cap)
{
    switch (cap->state) {
    case nexus_captured_surface_striped:
    case nexus_captured_surface_destriping:
        (void)NEXUS_VideoDecoder_ReturnDecodedFrames(handle->serverSettings.videoDecoder, NULL, 1);
        NEXUS_StripedSurface_Destroy(cap->stripedSurface);
        cap->stripedSurface = NULL;
        break;
    default:
        break;
    }
    cap->state = nexus_captured_surface_empty;
}

static void nexus_simplevideodecoder_p_reset_caps(NEXUS_SimpleVideoDecoderHandle handle)
{
    unsigned i;
    for (i=0;i<handle->capture.total;i++) {
        nexus_simplevideodecoder_p_reset_cap(handle, &handle->capture.destripe[i]);
    }
    handle->capture.rptr = handle->capture.wptr = 0;
}

static void surface_timer_func(void *arg)
{
    NEXUS_VideoDecoderReturnFrameSettings returnSettings;
    NEXUS_Error rc;
    struct nexus_captured_surface *cap;
    unsigned rptr;

    BSTD_UNUSED(arg);

    g_state.videoAsGraphics.timer = NULL;

    if (!g_state.videoAsGraphics.checkpointPending) {
        NEXUS_SimpleVideoDecoderHandle handle;
        unsigned total = 0;

        for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle=BLST_S_NEXT(handle, link)) {
            if (!handle->capture.started || !handle->serverSettings.videoDecoder) continue;
            BDBG_ASSERT(handle->capture.total);
            while (1) {
                unsigned numEntries;
                NEXUS_VideoDecoderFrameStatus frameStatus;

                NEXUS_SimpleVideoDecoder_GetDefaultReturnFrameSettings(&returnSettings);
                returnSettings.display = false;

                rc = NEXUS_VideoDecoder_GetDecodedFrames(handle->serverSettings.videoDecoder, &frameStatus, 1, &numEntries);
                if (rc || numEntries == 0) break;

                handle->capture.stats.pictures++;
                BDBG_ASSERT(handle->capture.wptr < handle->capture.total);
                cap = &handle->capture.destripe[handle->capture.wptr];
                if (cap->state != nexus_captured_surface_empty) { /* overflow handling */
                    if (++handle->capture.stats.overflows % 10 == 0) {
                        BDBG_WRN(("surface_timer_func: %p has %d fifo overflows", (void *)handle, handle->capture.stats.overflows));
                    }
                    (void)NEXUS_VideoDecoder_ReturnDecodedFrames(handle->serverSettings.videoDecoder, &returnSettings, 1);
                    continue;
                }

                BDBG_ASSERT(!cap->stripedSurface);
                if (handle->capture.settings.forceFrameDestripe) {
                    frameStatus.surfaceCreateSettings.bufferType = NEXUS_VideoBufferType_eFrame;
                }
                cap->stripedSurface = NEXUS_StripedSurface_Create(&frameStatus.surfaceCreateSettings);
                if ( NULL == cap->stripedSurface )
                {
                    rc = BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
                    (void)NEXUS_VideoDecoder_ReturnDecodedFrames(handle->serverSettings.videoDecoder, &returnSettings, 1);
                    continue;
                }
                cap->state = nexus_captured_surface_striped;
                cap->status = frameStatus;
                if (++handle->capture.wptr == handle->capture.total) {
                    handle->capture.wptr = 0;
                }
            }
            rptr = handle->capture.rptr;
            while (1) {
                BDBG_ASSERT(rptr < handle->capture.total);
                cap = &handle->capture.destripe[rptr];
                if (cap->state == nexus_captured_surface_striped) {
                    rc = NEXUS_Graphics2D_DestripeToSurface(g_state.videoAsGraphics.gfx, cap->stripedSurface, cap->surface, NULL);
                    if (rc == NEXUS_GRAPHICS2D_QUEUE_FULL) {
                        break;
                    }
                    else if (rc) {
                        rc = BERR_TRACE(rc);
                        goto done;
                    }
                    cap->state = nexus_captured_surface_destriping;
                    total++;
                }
                if (++rptr == handle->capture.total) {
                    rptr = 0;
                }
                if (rptr == handle->capture.wptr) break;
            }
        }
        if (!total) goto done;
        BDBG_MSG_TRACE(("surface_timer_func: checkpoint %d surface(s)", total));
    }

    rc = NEXUS_Graphics2D_Checkpoint(g_state.videoAsGraphics.gfx, NULL);
    if (rc==NEXUS_GRAPHICS2D_BUSY) {
        BDBG_MSG_TRACE(("surface_timer_func: checkpoint busy"));
        g_state.videoAsGraphics.checkpointPending = true;
        goto done;
    }
    else if (rc!=NEXUS_SUCCESS) {
        g_state.videoAsGraphics.checkpointPending = false;
        rc = BERR_TRACE(rc);
        goto done;
    }
    else { /* success */
        NEXUS_SimpleVideoDecoderHandle handle;
        g_state.videoAsGraphics.checkpointPending = false;
        for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle=BLST_S_NEXT(handle, link)) {
            if (!handle->capture.total) continue;
            rptr = handle->capture.rptr;
            while (1) {
                BDBG_ASSERT(rptr < handle->capture.total);
                cap = &handle->capture.destripe[rptr];
                if (cap->state == nexus_captured_surface_destriping) {
                    nexus_simplevideodecoder_p_reset_cap(handle, cap);
                    cap->state = nexus_captured_surface_destriped;
                }
                if (++rptr == handle->capture.total) {
                    rptr = 0;
                }
                if (rptr == handle->capture.wptr) break;
            }
        }
        BDBG_MSG_TRACE(("surface_timer_func: checkpoint complete"));
    }

done:
    g_state.videoAsGraphics.timer = NEXUS_ScheduleTimer(g_state.videoAsGraphics.checkpointPending?1:5, surface_timer_func, NULL);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetCapturedSurfaces(NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SurfaceHandle *pSurface, NEXUS_SimpleVideoDecoderCaptureStatus *pStatus, unsigned numEntries, unsigned *pNumReturned)
{
    unsigned i = 0, rptr = handle->capture.rptr;
    while (i < numEntries) {
        struct nexus_captured_surface *cap;
        if (!handle->capture.total) break;
        BDBG_ASSERT(rptr < handle->capture.total);
        cap = &handle->capture.destripe[rptr];
        if (cap->state == nexus_captured_surface_user) {
            /* already returned, pending recycle */
        }
        else if (cap->state == nexus_captured_surface_destriped) {
            pSurface[i] = cap->surface;
            pStatus[i] = cap->status;
            cap->state = nexus_captured_surface_user;
            i++;
        }
        else {
            break;
        }
        if (++rptr == handle->capture.total) {
            rptr = 0;
        }
        if (rptr == handle->capture.wptr) break;
    }
    *pNumReturned = i;
    return 0;
}

void NEXUS_SimpleVideoDecoder_RecycleCapturedSurfaces(NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SurfaceHandle *pSurface, unsigned numEntries)
{
    unsigned i = 0;
    while (i < numEntries) {
        struct nexus_captured_surface *cap;
        if (!handle->capture.total) break;
        cap = &handle->capture.destripe[handle->capture.rptr];
        if (cap->state != nexus_captured_surface_user) {
            BDBG_WRN(("RecycleCapturedSurfaces: %u surfaces recycled, but only %u needed recycling", numEntries, i));
            break;
        }
        if (cap->surface!=pSurface[i]) {
            BDBG_WRN(("RecycleCapturedSurfaces: surface mismatch %p:%p", (void *)cap->surface, (void*)pSurface[i]));
            /* recycle anyway */
        }
        cap->state = nexus_captured_surface_empty;
        i++;
        if (++handle->capture.rptr == handle->capture.total) {
            handle->capture.rptr = 0;
        }
    }
}

/**
Summary:
Get a decoded frame

Description:
If NEXUS_VideoDecoderStartSettings.appDisplayManagement is true,
this call will return the next decoded frame to the application.
It must be returned with NEXUS_VideoDecoder_ReturnDecodedFrame.
*/
NEXUS_Error NEXUS_SimpleVideoDecoder_GetDecodedFrames(
    NEXUS_SimpleVideoDecoderHandle handle,
    NEXUS_VideoDecoderFrameStatus *pStatus,  /* attr{nelem=numEntries;nelem_out=pNumEntriesReturned;null_allowed=y} [out] */
    unsigned numEntries,
    unsigned *pNumEntriesReturned /* [out] */
    )
{
    NEXUS_Error rc;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        *pNumEntriesReturned = 0;
        return rc;
    }
    return NEXUS_VideoDecoder_GetDecodedFrames(handle->serverSettings.videoDecoder, pStatus, numEntries, pNumEntriesReturned);
}

/**
Summary:
Get Default Settings for a returned frame
*/
void NEXUS_SimpleVideoDecoder_GetDefaultReturnFrameSettings(
    NEXUS_VideoDecoderReturnFrameSettings *pSettings    /* [out] */
    )
{
    NEXUS_VideoDecoder_GetDefaultReturnFrameSettings(pSettings);
}

/**
Summary:
Return a decoded frame

Description:
If NEXUS_VideoDecoderStartSettings.appDisplayManagement is true,
this call will display or drop a frame returned from
NEXUS_VideoDecoder_GetDecodedFrames.  Once returned, the frame
can not be reused and will become invalid
*/
NEXUS_Error NEXUS_SimpleVideoDecoder_ReturnDecodedFrames(
    NEXUS_SimpleVideoDecoderHandle handle,
    const NEXUS_VideoDecoderReturnFrameSettings *pSettings, /* attr{null_allowed=y, nelem=numFrames} Settings for each returned frame.  Pass NULL for defaults. */
    unsigned numFrames                                      /* Number of frames to return to the decoder */
    )
{
    NEXUS_Error rc;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        return rc;
    }
    return NEXUS_VideoDecoder_ReturnDecodedFrames(handle->serverSettings.videoDecoder, pSettings, numFrames);
}

void NEXUS_SimpleVideoDecoder_GetDefaultStartCaptureSettings(NEXUS_SimpleVideoDecoderStartCaptureSettings *pSettings)
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StartCapture(NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderStartCaptureSettings *pSettings)
{
    NEXUS_Error rc;
    unsigned i;

    if (handle->clientStarted==false) {
        return BERR_TRACE(NEXUS_NOT_SUPPORTED); /* require user to start decode before capture, though this is not strictly required */
    }
    if ( handle->serverSettings.videoDecoder && NEXUS_VideoDecoder_VideoAsGraphicsSupported_priv( handle->serverSettings.videoDecoder ) == NEXUS_NOT_SUPPORTED ) {
        return BERR_TRACE(NEXUS_NOT_SUPPORTED); /* chip doesn't support Video As graphics yet */
    }

    handle->capture.settings = *pSettings;
    handle->capture.started = true;
    handle->capture.stats.pictures = 0;
    handle->capture.stats.overflows = 0;

    if (pSettings->displayEnabled != handle->startSettings.displayEnabled && handle->started) {
        nexus_simplevideodecoder_p_stop(handle);
        nexus_simplevideodecoder_p_disconnect(handle, false);
        rc = nexus_simplevideodecoder_p_start(handle);
        if (rc) { rc = BERR_TRACE(rc); goto err_start; }
    }

    BKNI_Memset(handle->capture.destripe, 0, sizeof(handle->capture.destripe));
    for (i=0;i<NEXUS_SIMPLE_DECODER_MAX_SURFACES;i++) {
        if (!handle->capture.settings.surface[i]) break;
        handle->capture.destripe[i].surface = handle->capture.settings.surface[i];
        BDBG_ASSERT(handle->capture.destripe[i].state == nexus_captured_surface_empty);
    }
    handle->capture.total = i;
    if (handle->capture.total == 0) {
        rc = BERR_TRACE(NEXUS_INVALID_PARAMETER);
        goto err_total;
    }
    handle->capture.rptr = handle->capture.wptr = 0;

    if (g_state.videoAsGraphics.refcnt++ == 0) {
        NEXUS_Graphics2DSettings gfxSettings;
        NEXUS_Graphics2DOpenSettings openSettings;

        NEXUS_Graphics2D_GetDefaultOpenSettings(&openSettings);
        openSettings.packetFifoSize = 4096; /* alloc space to destripe 12 mosaics, but also handle full queue */
        g_state.videoAsGraphics.gfx = NEXUS_Graphics2D_Open(NEXUS_ANY_ID, &openSettings);
        if (g_state.videoAsGraphics.gfx==NULL) {
            rc = BERR_TRACE(NEXUS_UNKNOWN);
            goto err_opengfx;
        }
        NEXUS_Graphics2D_GetSettings(g_state.videoAsGraphics.gfx, &gfxSettings);
        gfxSettings.pollingCheckpoint = true; /* must use pollingCheckpoint inside module */
        NEXUS_Graphics2D_SetSettings(g_state.videoAsGraphics.gfx, &gfxSettings);

        BDBG_ASSERT(!g_state.videoAsGraphics.timer);
        g_state.videoAsGraphics.timer = NEXUS_ScheduleTimer(5, surface_timer_func, NULL);
        g_state.videoAsGraphics.checkpointPending = false;
    }

    return 0;

err_opengfx:
err_total:
    nexus_simplevideodecoder_p_stop(handle);
    nexus_simplevideodecoder_p_disconnect(handle, false);
err_start:
    handle->capture.started = false;
    return rc;
}

void NEXUS_SimpleVideoDecoder_StopCapture(NEXUS_SimpleVideoDecoderHandle handle)
{
    if (!handle->capture.started) return;

    nexus_simplevideodecoder_p_reset_caps(handle);

    BDBG_ASSERT(g_state.videoAsGraphics.refcnt);
    if (--g_state.videoAsGraphics.refcnt == 0) {
        BDBG_ASSERT(g_state.videoAsGraphics.timer);
        NEXUS_CancelTimer(g_state.videoAsGraphics.timer);
        g_state.videoAsGraphics.timer = NULL;
        NEXUS_Graphics2D_Close(g_state.videoAsGraphics.gfx);
    }

    handle->capture.total = 0;
    handle->capture.started = false;
}

static NEXUS_Error nexus_p_start_prologue( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderStartSettings *pSettings, bool *bypass_p_start)
{
    *bypass_p_start = false;

    if (handle->clientStarted) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    if (!pSettings->settings.pidChannel) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }

    if (!(handle->serverSettings.videoDecoder && handle->serverSettings.enabled)) {
        if (handle->serverSettings.disableMode != NEXUS_SimpleDecoderDisableMode_eSuccess) {
            return BERR_TRACE(NEXUS_NOT_AVAILABLE);
        }
        *bypass_p_start = true;
    }

    NEXUS_OBJECT_ACQUIRE(handle, NEXUS_PidChannel, pSettings->settings.pidChannel);
    if (pSettings->settings.enhancementPidChannel) {
        NEXUS_OBJECT_ACQUIRE(handle, NEXUS_PidChannel, pSettings->settings.enhancementPidChannel);
    }
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetStcChannel( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleStcChannelHandle stcChannel )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (stcChannel == handle->stcChannel) {
        return 0;
    }
    if (handle->stcChannel) {
        NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, NULL);
        NEXUS_OBJECT_RELEASE(handle, NEXUS_SimpleStcChannel, handle->stcChannel);
    }
    if (stcChannel) {
        NEXUS_OBJECT_ACQUIRE(handle, NEXUS_SimpleStcChannel, stcChannel);
        NEXUS_SimpleStcChannel_SetVideo_priv(stcChannel, handle);
    }
    handle->stcChannel = stcChannel;
    return 0;
}

NEXUS_SimpleStcChannelHandle NEXUS_SimpleVideoDecoder_P_GetStcChannel( NEXUS_SimpleVideoDecoderHandle handle )
{
    return handle->stcChannel;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_Start( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderStartSettings *pSettings )
{
    NEXUS_Error rc;
    bool bypass_p_start;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    rc = nexus_p_start_prologue(handle, pSettings, &bypass_p_start);
    if (rc) return BERR_TRACE(rc);

    handle->clientStarted = true;
    handle->startSettings = *pSettings;

    if (!bypass_p_start) {
        rc = nexus_simplevideodecoder_p_start(handle);
        if (rc) {
            /* because clientStarted = true, we must stop */
            NEXUS_SimpleVideoDecoder_Stop(handle);
            return BERR_TRACE(rc);
        }
    }

    return 0;
}

static void nexus_simplevideodecoder_p_enable_tsm_extensions( NEXUS_SimpleVideoDecoderHandle handle )
{
    if (handle->stcChannel && !nexus_simplevideodecoder_p_nondecoder(handle) && handle->serverSettings.videoDecoder)
    {
#if NEXUS_HAS_ASTM
        NEXUS_SimpleStcChannel_SetAstmVideo_priv(handle->stcChannel, handle->serverSettings.videoDecoder);
#endif
#if NEXUS_HAS_SYNC_CHANNEL
        NEXUS_SimpleStcChannel_SetSyncVideo_priv(handle->stcChannel, NEXUS_VideoDecoder_GetConnector(handle->serverSettings.videoDecoder));
#endif
    }
}

static void nexus_simplevideodecoder_p_disable_tsm_extensions( NEXUS_SimpleVideoDecoderHandle handle )
{
    if (handle->stcChannel && handle->started && !nexus_simplevideodecoder_p_nondecoder(handle))
    {
#if NEXUS_HAS_ASTM
        NEXUS_SimpleStcChannel_SetAstmVideo_priv(handle->stcChannel, NULL);
#endif
#if NEXUS_HAS_SYNC_CHANNEL
        NEXUS_SimpleStcChannel_SetSyncVideo_priv(handle->stcChannel, NULL);
#endif
    }
}

static NEXUS_Error nexus_simplevideodecoder_p_start( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_Error rc;

    BDBG_MSG(("nexus_simplevideodecoder_p_start %p", (void *)handle));
    if (handle->started) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    /* Force re-connect if we must enable codec support or increase memory use.
    Also re-connect if decreasing memory use, but only when going from 4K to non-4K. */
    if (!handle->settings.supportedCodecs[handle->startSettings.settings.codec] ||
        handle->startSettings.maxWidth > handle->settings.maxWidth ||
        handle->startSettings.maxHeight > handle->settings.maxHeight ||
        (handle->settings.maxHeight > 1088 && handle->startSettings.maxHeight <= 1088))
    {
        nexus_simplevideodecoder_p_disconnect(handle, false);
    }
    rc = nexus_simplevideodecoder_p_connect(handle);
    if (rc) return BERR_TRACE(rc);

    if (nexus_simplevideodecoder_p_nondecoder(handle)) {
        goto start_encoder;
    }

    nexus_simplevideodecoder_p_enable_tsm_extensions(handle);

    {
        NEXUS_VideoDecoderStartSettings startSettings = handle->startSettings.settings;
        if (handle->stcChannel)
        {
            startSettings.stcChannel = NEXUS_SimpleStcChannel_GetServerStcChannel_priv(handle->stcChannel, NEXUS_SimpleDecoderType_eVideo);
        }
        startSettings.nonRealTime = nexus_simpleencoder_p_nonRealTime(handle->encoder.handle);
        rc = NEXUS_VideoDecoder_Start(handle->serverSettings.videoDecoder, &startSettings);
        if (rc) {
            rc = BERR_TRACE(rc);
            goto err_start_decoder;
        }
    }

start_encoder:
    handle->started = true;

    if (handle->encoder.handle) {
        rc = nexus_simpleencoder_p_start_video(handle->encoder.handle);
        if (rc) {
            rc = BERR_TRACE(rc);
            goto err_start_encoder;
        }
    }

    return 0;

err_start_encoder:
err_start_decoder:
    nexus_simplevideodecoder_p_disable_tsm_extensions(handle);
    nexus_simplevideodecoder_p_disconnect(handle, false);
    return rc;
}

static void nexus_p_stop_epilogue( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_OBJECT_RELEASE(handle, NEXUS_PidChannel, handle->startSettings.settings.pidChannel);
    if (handle->startSettings.settings.enhancementPidChannel) {
        NEXUS_OBJECT_RELEASE(handle, NEXUS_PidChannel, handle->startSettings.settings.enhancementPidChannel);
    }
    NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&handle->startSettings);
}

void NEXUS_SimpleVideoDecoder_Stop( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (!handle->clientStarted) return;

    if (handle->hdmiInput.handle) {
        NEXUS_SimpleVideoDecoder_StopHdmiInput(handle);
        return;
    }
    if (handle->hdDviInput.handle) {
        NEXUS_SimpleVideoDecoder_StopHdDviInput(handle);
        return;
    }
    if (handle->imageInput.handle) {
        NEXUS_SimpleVideoDecoder_StopImageInput(handle);
        return;
    }
    if (handle->primer.started) {
        NEXUS_SimpleVideoDecoder_StopPrimer(handle);
        return;
    }

    handle->clientStarted = false;
    if (handle->capture.started) {
        NEXUS_SimpleVideoDecoder_StopCapture(handle);
    }
    nexus_simplevideodecoder_p_stop(handle);
    nexus_p_stop_epilogue(handle);
}

static void nexus_simplevideodecoder_p_stop( NEXUS_SimpleVideoDecoderHandle handle )
{
    if (!handle->started) return;

    BDBG_MSG(("nexus_simplevideodecoder_p_stop %p", (void *)handle));

    if (handle->serverSettings.videoDecoder) {
        NEXUS_VideoDecoder_Stop(handle->serverSettings.videoDecoder);
    }
    nexus_simplevideodecoder_p_disable_tsm_extensions(handle); /* must be after stop, before started=false */
    handle->started = false;
    handle->primer.started = false;

    nexus_simplevideodecoder_p_reset_caps(handle);
    if (handle->encoder.handle) {
        nexus_simpleencoder_p_stop(handle->encoder.handle);
        BDBG_ASSERT(!handle->encoder.handle); /* it should unlink */
    }
}

void NEXUS_SimpleVideoDecoder_StopAndFree( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    NEXUS_SimpleVideoDecoder_Stop(handle);
    nexus_simplevideodecoder_p_disconnect(handle, true);
    nexus_simple_p_close_primer(handle);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetStatus( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderStatus *pStatus )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        if (handle->primer.handle) {
            rc = NEXUS_VideoDecoderPrimer_GetStatus(handle->primer.handle, pStatus);
        }
        else {
            BKNI_Memset(pStatus, 0, sizeof(*pStatus));
        }
        return rc; /* no BERR_TRACE */
    }
    return NEXUS_VideoDecoder_GetStatus(handle->serverSettings.videoDecoder, pStatus);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetExtendedStatus( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderExtendedStatus *pStatus )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        BKNI_Memset(pStatus, 0, sizeof(*pStatus));
        return rc; /* no BERR_TRACE */
    }
    return NEXUS_VideoDecoder_GetExtendedStatus(handle->serverSettings.videoDecoder, pStatus);
}

void NEXUS_SimpleVideoDecoder_GetStreamInformation( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderStreamInformation *pStreamInfo )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        BKNI_Memset(pStreamInfo, 0, sizeof(*pStreamInfo));
    }
    else {
        rc = NEXUS_VideoDecoder_GetStreamInformation(handle->serverSettings.videoDecoder, pStreamInfo);
        if (rc) {
            BKNI_Memset(pStreamInfo, 0, sizeof(*pStreamInfo));
        }
    }
}

NEXUS_Error NEXUS_SimpleVideoDecoder_Get3DTVStatus( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoder3DTVStatus *pStatus )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        BKNI_Memset(pStatus, 0, sizeof(*pStatus));
        return rc; /* no BERR_TRACE */
    }
    return NEXUS_VideoDecoder_Get3DTVStatus(handle->serverSettings.videoDecoder, pStatus);
}

void NEXUS_SimpleVideoDecoder_Flush( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return;
    NEXUS_VideoDecoder_Flush(handle->serverSettings.videoDecoder);
}

void NEXUS_SimpleVideoDecoder_GetTrickState( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderTrickState *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->trickSettings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetTrickState( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_VideoDecoderTrickState *pSettings )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    handle->trickSettings = *pSettings;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_SetTrickState(handle->serverSettings.videoDecoder, pSettings);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_FrameAdvance( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_FrameAdvance(handle->serverSettings.videoDecoder);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

void NEXUS_SimpleVideoDecoder_GetPlaybackSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderPlaybackSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->playbackSettings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetPlaybackSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_VideoDecoderPlaybackSettings *pSettings )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    handle->playbackSettings = *pSettings;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_SetPlaybackSettings(handle->serverSettings.videoDecoder, pSettings);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetClientStatus( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleVideoDecoderClientStatus *pStatus )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BKNI_Memset(pStatus, 0, sizeof(*pStatus));
    if (handle->serverSettings.videoDecoder) {
        NEXUS_VideoDecoderUserDataStatus userDataStatus;
        int rc;
        pStatus->enabled = handle->serverSettings.enabled;
        rc = NEXUS_VideoDecoder_GetUserDataStatus(handle->serverSettings.videoDecoder, &userDataStatus);
        if (rc) return BERR_TRACE(rc);
        pStatus->afdValue = userDataStatus.afdValue;
    }
    return 0;
}

void NEXUS_SimpleVideoDecoder_GetClientSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleVideoDecoderClientSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->clientSettings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetClientSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderClientSettings *pSettings )
{
    unsigned i;
    NEXUS_Error rc;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
        NEXUS_VideoWindowHandle window = handle->serverSettings.window[i];
        if (window) {
            rc = NEXUS_VideoWindow_SetAfdSettings(window, &pSettings->afdSettings);
            if (rc) return BERR_TRACE(rc);
        }
    }
    if (handle->connected) {
        rc = nexus_simplevideodecoder_p_setvbisetings(handle, pSettings->closedCaptionRouting);
        if (rc) return BERR_TRACE(rc);
    }
    if (pSettings != &handle->clientSettings) {
        NEXUS_TaskCallback_Set(handle->resourceChangedCallback, &pSettings->resourceChanged);
        handle->clientSettings = *pSettings;
    }
    return 0;
}

void NEXUS_SimpleVideoDecoder_GetSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->settings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_VideoDecoderSettings *pSettings )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    handle->settings = *pSettings;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_SetSettings(handle->serverSettings.videoDecoder, pSettings);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

void NEXUS_SimpleVideoDecoder_GetExtendedSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderExtendedSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->extendedSettings;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetExtendedSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_VideoDecoderExtendedSettings *pSettings )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    handle->extendedSettings = *pSettings;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_SetExtendedSettings(handle->serverSettings.videoDecoder, pSettings);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetStartPts( NEXUS_SimpleVideoDecoderHandle handle, uint32_t pts )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_SetStartPts(handle->serverSettings.videoDecoder, pts);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetNextPts( NEXUS_SimpleVideoDecoderHandle handle, uint32_t *pPts )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pPts = 0;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */
    rc = NEXUS_VideoDecoder_GetNextPts(handle->serverSettings.videoDecoder, pPts);
    if (rc) return rc; /* no BERR_TRACE. normal in polling mode. */
    return 0;
}

static NEXUS_Error nexus_simplevideodecoder_p_setdecodersettings(NEXUS_SimpleVideoDecoderHandle handle, bool currentSettings)
{
    NEXUS_Error rc;
    if (!handle->serverSettings.videoDecoder) {
        return NEXUS_SUCCESS;
    }
    if (currentSettings) {
        NEXUS_VideoDecoderSettings settings = handle->settings;
        /* apply the current simpledecoder settings back to the regular decoder */
        rc = NEXUS_VideoDecoder_SetPlaybackSettings(handle->serverSettings.videoDecoder, &handle->playbackSettings);
        if (rc) return BERR_TRACE(rc);

        /* this modifies the stored copy. requires app to not cache settings. */
        if (handle->startSettings.maxWidth && handle->startSettings.maxHeight) {
            settings.maxWidth = handle->startSettings.maxWidth;
            settings.maxHeight = handle->startSettings.maxHeight;
        }
        settings.supportedCodecs[handle->startSettings.settings.codec] = true;
        rc = NEXUS_VideoDecoder_SetSettings(handle->serverSettings.videoDecoder, &settings);
        if (rc) return BERR_TRACE(rc);

        handle->settings = settings;

        rc = NEXUS_VideoDecoder_SetExtendedSettings(handle->serverSettings.videoDecoder, &handle->extendedSettings);
        if (rc) return BERR_TRACE(rc);
    }
    else if (g_default) {
        /* restore the regular decoder's initial settings */
        rc = NEXUS_VideoDecoder_SetPlaybackSettings(handle->serverSettings.videoDecoder, &g_default->playbackSettings);
        if (rc) return BERR_TRACE(rc);
        rc = NEXUS_VideoDecoder_SetSettings(handle->serverSettings.videoDecoder, &g_default->settings);
        if (rc) return BERR_TRACE(rc);
        rc = NEXUS_VideoDecoder_SetExtendedSettings(handle->serverSettings.videoDecoder, &g_default->extendedSettings);
        if (rc) return BERR_TRACE(rc);
    }

    rc = nexus_simplevideodecoder_p_setvbisetings(handle, currentSettings && handle->clientSettings.closedCaptionRouting);
    if (rc) return BERR_TRACE(rc);

    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_ReadUserDataBuffer(NEXUS_SimpleVideoDecoderHandle handle, void *pBuffer, size_t bufferSize, size_t *pBytesCopied )
{
    NEXUS_Error rc;
    void *internalBuffer;
    unsigned internalSize;
    const NEXUS_UserDataHeader *pHeader;
    unsigned total;
    NEXUS_VideoDecoderSettings videoDecoderSettings;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pBytesCopied = 0;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return rc; /* no BERR_TRACE */

    /* implicitly turn on */
    NEXUS_VideoDecoder_GetSettings(handle->serverSettings.videoDecoder, &videoDecoderSettings);
    if (!videoDecoderSettings.userDataEnabled) {
        videoDecoderSettings.userDataEnabled = true;
        rc = NEXUS_VideoDecoder_SetSettings(handle->serverSettings.videoDecoder, &videoDecoderSettings);
        if (rc) return BERR_TRACE(rc);
    }

    rc = NEXUS_VideoDecoder_GetUserDataBuffer(handle->serverSettings.videoDecoder, &internalBuffer, &internalSize);
    if (rc) return BERR_TRACE(rc);

    /* mempcy is essential for simple decoder because untrusted clients should not have direct access to video memory.
    first, limit it by function params.
    second, limit it by NEXUS_UserDataHeader.blockSize. this makes app logic much easier */
    if (internalSize > bufferSize) {
        internalSize = bufferSize;
    }

    /* read as many whole blocks (header + payload) as possible */
    total = 0;
    while (total < internalSize) {
        if (total + sizeof(*pHeader) > internalSize) {
            break;
        }
        pHeader = (const NEXUS_UserDataHeader *)((uint8_t*)internalBuffer + total);
        if (total + pHeader->blockSize > internalSize) {
            break;
        }
        total += pHeader->blockSize;
    }
    if (total) {
        BKNI_Memcpy(pBuffer, internalBuffer, total);

        NEXUS_VideoDecoder_UserDataReadComplete(handle->serverSettings.videoDecoder, total);
        *pBytesCopied = total;
    }

    return 0;
}

void NEXUS_SimpleVideoDecoder_FlushUserData( NEXUS_SimpleVideoDecoderHandle handle )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) return;
    NEXUS_VideoDecoder_FlushUserData(handle->serverSettings.videoDecoder);
    return;
}

static int nexus_simple_p_open_primer(NEXUS_SimpleVideoDecoderHandle handle)
{
    if (!handle->primer.handle) {
        int rc;
        handle->primer.handle = NEXUS_VideoDecoderPrimer_Create(NULL);
        if (!handle->primer.handle) {
            return BERR_TRACE(NEXUS_NOT_AVAILABLE);
        }
        rc = NEXUS_VideoDecoderPrimer_SetSettings(handle->primer.handle, &handle->primer.settings);
        if (rc) return BERR_TRACE(rc);
    }
    return 0;
}

static void nexus_simple_p_close_primer(NEXUS_SimpleVideoDecoderHandle handle)
{
    if (handle->primer.handle) {
        BDBG_ASSERT(!handle->primer.started);
        NEXUS_VideoDecoderPrimer_Destroy(handle->primer.handle);
        handle->primer.handle = NULL;
    }
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StartPrimer( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderStartSettings *pSettings )
{
    bool bypass_p_start;
    int rc;
    NEXUS_VideoDecoderStartSettings startSettings;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BDBG_MSG(("NEXUS_SimpleVideoDecoder_StartPrimer %p", (void *)handle));

    /* priming requires an stcchannel */
    if (!handle->stcChannel) {
        return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    }
    rc = nexus_p_start_prologue(handle, pSettings, &bypass_p_start);
    if (rc) return rc;

    handle->clientStarted = true;
    handle->startSettings = *pSettings;
    BDBG_ASSERT(!handle->primer.started);

    rc = nexus_simple_p_open_primer(handle); /* lazy open */
    if (rc) {BERR_TRACE(rc); goto err_openprimer;}

    handle->primer.started = true;
    NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);

    startSettings = handle->startSettings.settings;
    startSettings.stcChannel = NEXUS_SimpleStcChannel_GetServerStcChannel_priv(handle->stcChannel, NEXUS_SimpleDecoderType_eVideo);
    rc = NEXUS_VideoDecoderPrimer_Start(handle->primer.handle, &startSettings);
    if (rc) {rc = BERR_TRACE(rc); goto err_start;}

    return 0;

err_start:
    handle->primer.started = false;
    NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);
err_openprimer:
    handle->clientStarted = false;
    nexus_p_stop_epilogue(handle);
    return rc;
}

void NEXUS_SimpleVideoDecoder_StopPrimer( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BDBG_MSG(("NEXUS_SimpleVideoDecoder_StopPrimer %p", (void *)handle));
    if (!handle->primer.started) return;

    NEXUS_VideoDecoderPrimer_Stop(handle->primer.handle);
    nexus_simplevideodecoder_p_disable_tsm_extensions(handle); /* must be between stop and started=false */
    handle->primer.started = false;
    handle->started = false;
    NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);
    handle->clientStarted = false;
    nexus_p_stop_epilogue(handle);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StopPrimerAndStartDecode( NEXUS_SimpleVideoDecoderHandle handle )
{
    int rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BDBG_MSG(("NEXUS_SimpleVideoDecoder_StopPrimerAndStartDecode %p", (void *)handle));
    if (!handle->primer.started) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }
    BDBG_ASSERT(handle->clientStarted);
    BDBG_ASSERT(!handle->started);
    if (!handle->serverSettings.videoDecoder) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    rc = nexus_simple_p_open_primer(handle); /* lazy open */
    if (rc) return BERR_TRACE(rc);

    rc = nexus_simplevideodecoder_p_connect(handle);
    if (rc) return BERR_TRACE(rc);

    nexus_simplevideodecoder_p_enable_tsm_extensions(handle);

    rc = NEXUS_VideoDecoderPrimer_StopPrimerAndStartDecode(handle->primer.handle, handle->serverSettings.videoDecoder);
    if (rc) {
        nexus_simplevideodecoder_p_disable_tsm_extensions(handle);
        return BERR_TRACE(rc);
    }
    handle->started = true;

    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StopDecodeAndStartPrimer( NEXUS_SimpleVideoDecoderHandle handle )
{
    int rc;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BDBG_MSG(("NEXUS_SimpleVideoDecoder_StopDecodeAndStartPrimer %p", (void *)handle));
    if (!handle->clientStarted || !handle->primer.started || !handle->started) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    rc = NEXUS_VideoDecoderPrimer_StopDecodeAndStartPrimer(handle->primer.handle, handle->serverSettings.videoDecoder);
    /* needs to disable regardless of rc, must be after NEXUS_VideoDecoder_Stop, but before started=false */
    nexus_simplevideodecoder_p_disable_tsm_extensions(handle);
    if (rc) {
        BERR_TRACE(rc);
        /* just stop then, release all resources */
        NEXUS_SimpleVideoDecoder_StopPrimer(handle);
    }
    else {
        handle->started = false;
    }

    return rc;
}

void NEXUS_SimpleVideoDecoderPrimer_GetSettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderPrimerSettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->primer.settings;
}

NEXUS_Error NEXUS_SimpleVideoDecoderPrimer_SetSettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_VideoDecoderPrimerSettings *pSettings )
{
    int rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (handle->primer.handle) {
        rc = NEXUS_VideoDecoderPrimer_SetSettings(handle->primer.handle, pSettings);
        if (rc) return BERR_TRACE(rc);
    }
    handle->primer.settings = *pSettings;
    return 0;
}

NEXUS_Error nexus_simplevideodecoder_p_add_encoder(NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoWindowHandle window, NEXUS_SimpleEncoderHandle encoder)
{
    handle->encoder.handle = encoder;
    handle->encoder.window = window;
    if (handle->stcChannel) {
        /* adding encoder may change timebase */
        NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);
    }
    return 0;
}

void nexus_simplevideodecoder_p_remove_encoder(NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoWindowHandle window, NEXUS_SimpleEncoderHandle encoder)
{
    if (handle->encoder.handle != encoder) {
        BERR_TRACE(NEXUS_NOT_AVAILABLE);
        return;
    }
    handle->encoder.handle = NULL;
    nexus_simplevideodecoder_p_stop(handle);
    nexus_simplevideodecoder_p_disconnect(handle, false);
    BSTD_UNUSED(window);
    handle->encoder.window = NULL;
}

BDBG_FILE_MODULE(nexus_simple_decoder_proc);

void NEXUS_SimpleDecoderModule_P_PrintVideoDecoder(void)
{
#if BDBG_DEBUG_BUILD
    NEXUS_SimpleVideoDecoderHandle handle;
    for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle=BLST_S_NEXT(handle, link)) {
        BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("video %u(%p)", handle->index, (void *)handle));
        BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("  acquired %d, clientStarted %d",
            handle->acquired, handle->clientStarted));
        BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("  videoDecoder %p, started %d",
            (void *)handle->serverSettings.videoDecoder,
            handle->started));
        BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("  connected %d, window[0] %p, stcIndex %d, encoder %p",
            handle->connected,
            (void *)handle->serverSettings.window[0],
            handle->serverSettings.stcIndex,
            (void *)handle->encoder.handle));
        if (handle->primer.started) {
            BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("  primer started %d, primer %p",
                handle->primer.started,
                (void *)handle->primer.handle));
        }
        if (handle->capture.started) {
            unsigned cnt[nexus_captured_surface_user+1], i;
            BKNI_Memset(cnt, 0, sizeof(cnt));
            for (i=0;i<handle->capture.total;i++) {
                cnt[handle->capture.destripe[i].state]++;
            }
            BDBG_MODULE_LOG(nexus_simple_decoder_proc, ("  capture started: queue %d/%d/%d/%d/%d, %d pics, %d overflows",
                cnt[nexus_captured_surface_empty],
                cnt[nexus_captured_surface_striped],
                cnt[nexus_captured_surface_destriping],
                cnt[nexus_captured_surface_destriped],
                cnt[nexus_captured_surface_user],
                handle->capture.stats.pictures,
                handle->capture.stats.overflows
                ));
        }
    }
#endif
}

/**
An entry in g_cache.list is a connected decoder and set of windows.
If they are disconnected, they should be removed from the list.
**/

struct settings_cache {
    BLST_S_ENTRY(settings_cache) link;
    NEXUS_SimpleVideoDecoderHandle handle;
    NEXUS_SimpleVideoDecoderServerSettings settings;
    NEXUS_Time time;
};
static struct {
    bool enabled;
    BLST_S_HEAD(settingslist, settings_cache) list;
    NEXUS_TimerHandle timer;
} g_cache;

static bool use_cache(NEXUS_SimpleVideoDecoderHandle handle)
{
    struct settings_cache *c;
    for (c = BLST_S_FIRST(&g_cache.list); c; ) {
        struct settings_cache *next = BLST_S_NEXT(c, link);
        if (!BKNI_Memcmp(&c->settings, &handle->serverSettings, sizeof(c->settings))) {
            /* remove from cache, but don't disconnect. settings will be in use. */
            BDBG_MSG(("use server settings from %p(%p) for %p in cache", (void *)c->handle, (void *)c->settings.videoDecoder, (void *)handle));
            BLST_S_REMOVE(&g_cache.list, c, settings_cache, link);
            BKNI_Free(c);
            return true;
        }
        if (c->settings.videoDecoder == handle->serverSettings.videoDecoder) {
            /* but if the video decoder is being reused with different settings, we must disconnect first */
            NEXUS_SimpleVideoDecoderModule_CheckCache(NULL, c->settings.videoDecoder);
            break;
        }
        c = next;
    }
    return false;
}

static void destroy_cache_entry(struct settings_cache *c, bool disconnect)
{
    BDBG_MSG(("remove server settings for %p(%p) from cache", (void *)c->handle, (void *)c->settings.videoDecoder));
    if (disconnect) {
        nexus_simplevideodecoder_p_disconnect_settings(&c->settings, NULL);
    }
    BLST_S_REMOVE(&g_cache.list, c, settings_cache, link);
    BKNI_Free(c);
}

/* Allow window/decoder connection to remain in cache for 500 msec. This reduces runtime memory usage
and minimizes black frame when the next video window starts.
We immediately hide the video window so no after-decode black frame is visible. */
#define CACHE_TIMEOUT 500
static void cache_timer(void *context)
{
    struct settings_cache *c;
    NEXUS_Time now;
    BSTD_UNUSED(context);
    g_cache.timer = NULL;
    NEXUS_Time_Get(&now);
    for (c = BLST_S_FIRST(&g_cache.list); c; ) {
        struct settings_cache *next = BLST_S_NEXT(c, link);
        if (NEXUS_Time_Diff(&now, &c->time) >= CACHE_TIMEOUT) {
            destroy_cache_entry(c, true);
        }
        c = next;
    }
    if (BLST_S_FIRST(&g_cache.list)) {
        g_cache.timer = NEXUS_ScheduleTimer(CACHE_TIMEOUT, cache_timer, NULL);
    }
}

void nexus_simplevideodecoder_p_remove_settings_from_cache(void)
{
    struct settings_cache *c;
    /* clear the entry without disconnecting */
    while ((c = BLST_S_FIRST(&g_cache.list))) {
        destroy_cache_entry(c, false);
    }
    if (g_cache.timer) {
        NEXUS_CancelTimer(g_cache.timer);
        g_cache.timer = NULL;
    }
}

void NEXUS_SimpleVideoDecoder_SetCacheEnabled( NEXUS_SimpleVideoDecoderHandle handle, bool enabled )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    BSTD_UNUSED(enabled);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetFifoStatus(
    NEXUS_SimpleVideoDecoderHandle handle,
    NEXUS_VideoDecoderFifoStatus *pStatus /* [out] */
    )
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        BKNI_Memset(pStatus, 0, sizeof(*pStatus));
        return rc;
    }
    rc = NEXUS_VideoDecoder_GetFifoStatus(handle->serverSettings.videoDecoder, pStatus);
    return BERR_TRACE(rc);
}


void NEXUS_SimpleVideoDecoderModule_SetCacheEnabled( bool enabled )
{
    if (g_cache.enabled && !enabled) {
        NEXUS_SimpleVideoDecoderModule_CheckCache(NULL,NULL);
    }
    g_cache.enabled = enabled;
}

void NEXUS_SimpleVideoDecoderModule_CheckCache( NEXUS_VideoWindowHandle window, NEXUS_VideoDecoderHandle videoDecoder )
{
    struct settings_cache *c;
    for (c = BLST_S_FIRST(&g_cache.list); c; ) {
        struct settings_cache *next = BLST_S_NEXT(c, link);
        bool has_window = false;
        bool clear;
        if (window) {
            unsigned i;
            for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
                if (c->settings.window[i] == window) {
                    has_window = true;
                    break;
                }
            }
        }
        if (!window && !videoDecoder) {
            clear = true;
        }
        else {
            clear = has_window ^ (c->settings.videoDecoder == videoDecoder);
        }
        BDBG_MSG(("CheckCache %p: %p %p %d %p ==> %d", (void *)c, (void *)window, (void *)videoDecoder, has_window, (void *)c->settings.videoDecoder, clear));
        if (clear) {
            destroy_cache_entry(c, true);
        }
        c = next;
    }
}

/* returns true if added to cache. */
static bool add_settings_to_cache(NEXUS_SimpleVideoDecoderHandle handle)
{
    struct settings_cache *c;

    if (!g_cache.enabled) {
        BDBG_MSG(("don't use cache %d", g_cache.enabled));
        return false;
    }

    c = BKNI_Malloc(sizeof(*c));
    if (!c) return false;

    c->settings = handle->serverSettings;
    c->handle = handle;
    NEXUS_Time_Get(&c->time);
    BLST_S_INSERT_HEAD(&g_cache.list, c, link);
    BDBG_MSG(("add server settings for %p(%p) cache", (void *)handle, (void *)c->settings.videoDecoder));
    if (!g_cache.timer) {
        g_cache.timer = NEXUS_ScheduleTimer(CACHE_TIMEOUT, cache_timer, NULL);
    }
    return true;
}

void NEXUS_SimpleVideoDecoder_GetPictureQualitySettings( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_SimpleVideoDecoderPictureQualitySettings *pSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pSettings = handle->pictureQualitySettings;
}

static NEXUS_Error nexus_simplevideodecoder_p_setwindowpq(NEXUS_VideoWindowHandle window, const NEXUS_SimpleVideoDecoderPictureQualitySettings *pSettings )
{
    NEXUS_Error rc;
    NEXUS_VideoWindowMadSettings madSettings = pSettings->mad;
    rc = NEXUS_PictureCtrl_SetCommonSettings(window, &pSettings->common);
    if (rc) return BERR_TRACE(rc);
    /* DNR is per source, but we must apply to both HD and SD display windows so we don't have inconsistent settings */
    rc = NEXUS_VideoWindow_SetDnrSettings(window, &pSettings->dnr);
    if (rc) return BERR_TRACE(rc);
    rc = NEXUS_VideoWindow_SetAnrSettings(window, &pSettings->anr);
    if (rc) return BERR_TRACE(rc);
    /* allow user to turn MAD off if already on, but don't allow MAD on beyond defaults */
    if (madSettings.deinterlace) {
        NEXUS_Module_Lock(g_NEXUS_simpleDecoderModuleSettings.modules.display);
        madSettings.deinterlace = NEXUS_VideoAdj_P_DefaultMadEnabled_priv(window);
        NEXUS_Module_Unlock(g_NEXUS_simpleDecoderModuleSettings.modules.display);
    }
    rc = NEXUS_VideoWindow_SetMadSettings(window, &madSettings);
    if (rc) return BERR_TRACE(rc);
    rc = NEXUS_VideoWindow_SetScalerSettings(window, &pSettings->scaler);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetPictureQualitySettings( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderPictureQualitySettings *pSettings )
{
    NEXUS_Error rc;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (handle->encoder.window) {
        rc = nexus_simplevideodecoder_p_setwindowpq(handle->encoder.window, pSettings);
        if (rc) return BERR_TRACE(rc);
    }
    else {
        unsigned i;
        for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
            if (handle->serverSettings.window[i]) {
                rc = nexus_simplevideodecoder_p_setwindowpq(handle->serverSettings.window[i], pSettings);
                if (rc) return BERR_TRACE(rc);
            }
        }
    }
    if (pSettings != &handle->pictureQualitySettings) {
        handle->pictureQualitySettings = *pSettings;
    }
    return 0;
}

void NEXUS_SimpleVideoDecoder_GetStcIndex( NEXUS_SimpleVideoDecoderHandle handle, int *pStcIndex )
{
    *pStcIndex = handle->serverSettings.stcIndex;
}

NEXUS_VideoImageInputHandle NEXUS_SimpleVideoDecoder_StartImageInput( NEXUS_SimpleVideoDecoderHandle handle, const NEXUS_SimpleVideoDecoderStartSettings *pStartSettings )
{
    NEXUS_VideoDecoderDisplayConnection connection;
    NEXUS_VideoImageInputSettings imageInputSettings;
    int rc;

    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (handle->clientStarted) {
        BDBG_ERR(("%p: regular decoder already started", (void *)handle));
        return NULL;
    }
    BDBG_ASSERT(!handle->imageInput.handle);
    BDBG_ASSERT(!handle->started);

    /* TODO: It would be nice to allow image input to start without a backing decoder, but we must have the MFD to Open.
    Also, unless we can switch the MFD after open, we can't restart image input when we receive a new decoder because
    it could be a different MFD. So we keep it simple now: if the backing decoder is removed, we stop and don't restart. */
    if (!handle->serverSettings.videoDecoder) {
        BDBG_ERR(("%p: image input not available", (void *)handle));
        return NULL;
    }
    if (handle->connected) {
        nexus_simplevideodecoder_p_disconnect(handle, false);
    }
    if (pStartSettings) {
        handle->startSettings = *pStartSettings;
    }
    else {
        NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&handle->startSettings);
    }

    NEXUS_Module_Lock(g_NEXUS_simpleDecoderModuleSettings.modules.videoDecoder);
    NEXUS_VideoDecoder_GetDisplayConnection_priv(handle->serverSettings.videoDecoder, &connection);
    NEXUS_Module_Unlock(g_NEXUS_simpleDecoderModuleSettings.modules.videoDecoder);
    /* connection.parentIndex is defined as the MFD index */
    NEXUS_VideoImageInput_GetDefaultSettings(&imageInputSettings);
    imageInputSettings.lowDelayMode = handle->startSettings.lowDelayImageInput;
    if ( false == imageInputSettings.lowDelayMode && handle->stcChannel )
    {
        imageInputSettings.stcChannel = NEXUS_SimpleStcChannel_GetServerStcChannel_priv(handle->stcChannel, NEXUS_SimpleDecoderType_eVideo);
    }
    handle->imageInput.handle = NEXUS_VideoImageInput_Open(connection.parentIndex, &imageInputSettings);
    if (!handle->imageInput.handle) {
        BDBG_ERR(("unable to open VideoImageInput %d", connection.parentIndex));
    }
    rc = nexus_simplevideodecoder_p_connect(handle);
    if (rc) {
        BERR_TRACE(rc);
        goto error;
    }
    rc = nexus_simplevideodecoder_p_start(handle);
    if (rc) {
        BERR_TRACE(rc);
        goto error;
    }
    handle->clientStarted = true;

    return handle->imageInput.handle;

error:
    NEXUS_SimpleVideoDecoder_StopImageInput(handle);
    return NULL;
}

void NEXUS_SimpleVideoDecoder_StopImageInput( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (!handle->imageInput.handle) {
        return;
    }
    nexus_simplevideodecoder_p_disconnect(handle, false);
    NEXUS_VideoImageInput_Close(handle->imageInput.handle);
    handle->imageInput.handle = NULL;
    handle->started = false;
    handle->clientStarted = false;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StartHdmiInput( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_HdmiInputHandle hdmiInput, const NEXUS_SimpleVideoDecoderStartSettings *pStartSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
#if NEXUS_HAS_HDMI_INPUT
    if (handle->hdmiInput.handle) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    BDBG_MSG(("NEXUS_SimpleVideoDecoder_StartHdmiInput %p %p", (void *)handle, (void *)hdmiInput));
    handle->clientStarted = true;
    handle->hdmiInput.handle = hdmiInput;
    NEXUS_OBJECT_ACQUIRE(handle, NEXUS_HdmiInput, hdmiInput);
    if (pStartSettings) {
        handle->startSettings = *pStartSettings;
    }
    else {
        NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&handle->startSettings);
    }

    if (handle->serverSettings.window[0] || handle->encoder.window) {
        int rc;
        if (handle->connected) {
            nexus_simplevideodecoder_p_disconnect(handle, false);
        }

        NEXUS_SimpleStcChannel_SetVideo_priv(handle->stcChannel, handle);

        rc = nexus_simplevideodecoder_p_start(handle);
        if (rc) {
            NEXUS_SimpleVideoDecoder_StopHdmiInput(handle);
            return BERR_TRACE(rc);
        }
    }
    return 0;
#else
    BSTD_UNUSED(hdmiInput);
    BSTD_UNUSED(pStartSettings);
    return BERR_TRACE(NEXUS_NOT_SUPPORTED);
#endif
}

void NEXUS_SimpleVideoDecoder_StopHdmiInput( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
#if NEXUS_HAS_HDMI_INPUT
    if (!handle->hdmiInput.handle) {
        return;
    }
    nexus_simplevideodecoder_p_disconnect(handle, false);
    NEXUS_OBJECT_RELEASE(handle, NEXUS_HdmiInput, handle->hdmiInput.handle);
    handle->hdmiInput.handle = NULL;
    handle->clientStarted = false;
    handle->started = false;
#endif
}

NEXUS_Error NEXUS_SimpleVideoDecoder_StartHdDviInput( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_HdDviInputHandle hdDviInput, const NEXUS_SimpleVideoDecoderStartSettings *pStartSettings )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);

    if (handle->hdDviInput.handle) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }

    handle->clientStarted = true;
    handle->hdDviInput.handle = hdDviInput;
    NEXUS_OBJECT_ACQUIRE(handle, NEXUS_HdDviInput, hdDviInput);
    if (pStartSettings) {
        handle->startSettings = *pStartSettings;
    }
    else {
        NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&handle->startSettings);
    }

    if (handle->serverSettings.window[0] || handle->encoder.window) {
        int rc;
        if (handle->connected) {
            nexus_simplevideodecoder_p_disconnect(handle, false);
        }
        rc = nexus_simplevideodecoder_p_start(handle);
        if (rc) {
            NEXUS_SimpleVideoDecoder_StopHdDviInput(handle);
            return BERR_TRACE(rc);
        }
    }
    return 0;
}

void NEXUS_SimpleVideoDecoder_StopHdDviInput( NEXUS_SimpleVideoDecoderHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    if (!handle->hdDviInput.handle) {
        return;
    }
    nexus_simplevideodecoder_p_disconnect(handle, false);
    NEXUS_OBJECT_RELEASE(handle, NEXUS_HdDviInput, handle->hdDviInput.handle);
    handle->hdDviInput.handle = NULL;
    handle->clientStarted = false;
    handle->started = false;
}

static int nexus_simplevideodecoder_p_setmanualpowerstate(NEXUS_SimpleVideoDecoderHandle handle, bool enabled)
{
    int rc;
    NEXUS_VideoDecoderSettings videoDecoderSettings;
    NEXUS_VideoDecoder_GetSettings(handle->serverSettings.videoDecoder, &videoDecoderSettings);
    videoDecoderSettings.manualPowerState = enabled;
    rc = NEXUS_VideoDecoder_SetSettings(handle->serverSettings.videoDecoder, &videoDecoderSettings);
    return rc;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SwapWindows( NEXUS_SimpleVideoDecoderHandle decoder1, NEXUS_SimpleVideoDecoderHandle decoder2 )
{
    NEXUS_SimpleVideoDecoderServerSettings swap;
    unsigned i;
    int rc;

    /* now we have a true swap. do it without stopping decode. */
    nexus_simplevideodecoder_p_setmanualpowerstate(decoder1, true);
    nexus_simplevideodecoder_p_setmanualpowerstate(decoder2, true);

    swap = decoder1->serverSettings;

    for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
        NEXUS_VideoWindowHandle window;

        /* disconnect */
        window = decoder1->serverSettings.window[i];
        if (window && decoder1->connected) {
            NEXUS_VideoWindow_RemoveAllInputs(window);
        }
        window = decoder2->serverSettings.window[i];
        if (window && decoder2->connected) {
            NEXUS_VideoWindow_RemoveAllInputs(window);
        }

        /* swap */
        decoder1->serverSettings.window[i] = decoder2->serverSettings.window[i];
        decoder2->serverSettings.window[i] = swap.window[i];
        decoder1->serverSettings.display[i] = decoder2->serverSettings.display[i];
        decoder2->serverSettings.display[i] = swap.display[i];
    }

    NEXUS_DisplayModule_SetUpdateMode(NEXUS_DisplayUpdateMode_eManual);
    for (i=0;i<NEXUS_MAX_DISPLAYS;i++) {
        NEXUS_VideoWindowHandle window;

        /* connect */
        window = decoder1->serverSettings.window[i];
        if (window && decoder1->connected) {
            rc = NEXUS_VideoWindow_AddInput(window, nexus_simplevideodecoder_p_getinput(decoder1));
            if (rc) BERR_TRACE(rc); /* keep going */
        }
        window = decoder2->serverSettings.window[i];
        if (window && decoder2->connected) {
            rc = NEXUS_VideoWindow_AddInput(window, nexus_simplevideodecoder_p_getinput(decoder2));
            if (rc) BERR_TRACE(rc); /* keep going */
        }
    }
    NEXUS_DisplayModule_SetUpdateMode(NEXUS_DisplayUpdateMode_eAuto);

    nexus_simplevideodecoder_p_setmanualpowerstate(decoder1, false);
    nexus_simplevideodecoder_p_setmanualpowerstate(decoder2, false);
    return 0;
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetCrcData( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoDecoderCrc *pEntries, unsigned numEntries, unsigned *pNumReturned )
{
    NEXUS_Error rc;
    NEXUS_VideoDecoderExtendedSettings settings;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pNumReturned = 0;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        return NEXUS_SUCCESS;
    }
    NEXUS_VideoDecoder_GetExtendedSettings(handle->serverSettings.videoDecoder, &settings);
    if (settings.crcFifoSize == 0) {
        settings.crcFifoSize = 128;
        rc = NEXUS_VideoDecoder_SetExtendedSettings(handle->serverSettings.videoDecoder, &settings);
        if (rc) return BERR_TRACE(rc);
    }
    return NEXUS_VideoDecoder_GetCrcData(handle->serverSettings.videoDecoder, pEntries, numEntries, pNumReturned);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_GetVideoInputCrcData( NEXUS_SimpleVideoDecoderHandle handle, NEXUS_VideoInputCrcData *pEntries, unsigned numEntries, unsigned *pNumReturned )
{
    NEXUS_Error rc;
    NEXUS_VideoInput videoInput;
    NEXUS_VideoInputSettings videoInputSettings;
    BDBG_OBJECT_ASSERT(handle, NEXUS_SimpleVideoDecoder);
    *pNumReturned = 0;
    if (nexus_simplevideodecoder_has_resource(handle, &rc)) {
        return NEXUS_SUCCESS;
    }
    videoInput = NEXUS_VideoDecoder_GetConnector(handle->serverSettings.videoDecoder);
    NEXUS_VideoInput_GetSettings(videoInput, &videoInputSettings);
    if (videoInputSettings.crcQueueSize == 0) {
        videoInputSettings.crcQueueSize = 128;
        rc = NEXUS_VideoInput_SetSettings(videoInput, &videoInputSettings);
        if (rc) return BERR_TRACE(rc);
    }
    return NEXUS_VideoInput_GetCrcData(videoInput, pEntries, numEntries, pNumReturned);
}

NEXUS_Error NEXUS_SimpleVideoDecoder_SetSdOverride( NEXUS_SimpleVideoDecoderHandle handle, bool enabled )
{
    if (enabled) {
        if (g_state.sdOverride.handle) {
            return BERR_TRACE(NEXUS_INVALID_PARAMETER);
        }
        g_state.sdOverride.handle = handle;
        g_state.sdOverride.videoInput = nexus_simplevideodecoder_p_getinput(g_state.sdOverride.handle);
    }
    else if (!enabled) {
        if (g_state.sdOverride.handle != handle) {
            return NEXUS_SUCCESS;
        }
        g_state.sdOverride.handle = NULL;
        g_state.sdOverride.videoInput = NULL;
    }
    for (handle=BLST_S_FIRST(&g_videoDecoders); handle; handle=BLST_S_NEXT(handle, link)) {
        NEXUS_Error rc;
        NEXUS_VideoWindowHandle window = handle->serverSettings.window[1];
        if (!window) continue;
        NEXUS_VideoWindow_RemoveAllInputs(window);
        if (g_state.sdOverride.videoInput) {
            rc = NEXUS_VideoWindow_AddInput(window, g_state.sdOverride.videoInput);
            if (rc) return BERR_TRACE(rc);
        }
        else {
            if (handle->connected) {
                rc = NEXUS_VideoWindow_AddInput(window, nexus_simplevideodecoder_p_getinput(handle));
                if (rc) return BERR_TRACE(rc);
            }
        }
    }
    return NEXUS_SUCCESS;
}
