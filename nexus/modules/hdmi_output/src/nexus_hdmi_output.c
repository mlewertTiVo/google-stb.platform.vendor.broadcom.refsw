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

#include "nexus_hdmi_output_module.h"
#include "priv/nexus_hdmi_output_priv.h"
#include "priv/nexus_i2c_priv.h"
#include "priv/nexus_core_video.h"
#include "priv/nexus_core_audio.h"
#include "priv/nexus_core.h"
#include "bhdm.h"
#include "bhdm_edid.h"
#include "bhdm_hdcp.h"
#include "bhdm_scdc.h"
#include "bhdm_auto_i2c.h"
#include "bhdm_monitor.h"
#include "priv/nexus_hdmi_output_mhl_priv.h"

#if NEXUS_HAS_HDMI_INPUT
#include "priv/nexus_hdmi_input_priv.h"
#endif

BDBG_MODULE(nexus_hdmi_output);

#if NEXUS_NUM_HDMI_OUTPUTS

#define NEXUS_HDMI_OUTPUT_4K_PIXEL_CLOCK_RATE 296703	/* 4K p30/25 */

static NEXUS_HdmiOutput g_hdmiOutputs[NEXUS_NUM_HDMI_OUTPUTS];

static NEXUS_VideoFormat NEXUS_HdmiOutput_P_GetPreferredFormat(NEXUS_HdmiOutputHandle output);

static void NEXUS_HdmiOutput_P_RxSenseTimerExpiration(void *pContext);

static void NEXUS_HdmiOutput_P_ScrambleCallback(void *pContext);
static void NEXUS_HdmiOutput_P_AvRateChangeCallback(void *pContext);

static void NEXUS_HdmiOutput_P_HotplugCallback(void *pContext);
static void NEXUS_HdmiOutput_P_HotPlug_isr(void *context, int param, void *data) ;

static void NEXUS_HdmiOutput_P_StopTimers(NEXUS_HdmiOutputHandle hdmiOutput) ;

static void NEXUS_HdmiOutput_P_GetTmdsSignals(
    NEXUS_HdmiOutputHandle hdmiOutput) ;

static NEXUS_Error NEXUS_HdmiOutput_P_GetSupportedFormats(
    NEXUS_HdmiOutputHandle output, bool *supportedVideoFormats) ;

#if BDBG_DEBUG_BUILD
static const char *NEXUS_HdmiOutput_P_ColorSpace_Text[NEXUS_ColorSpace_eMax] =
{
    BDBG_STRING("Auto"),
    BDBG_STRING("RGB"),
    BDBG_STRING("YCbCr 4:2:2"),
    BDBG_STRING("YCbCr 4:4:4"),
    BDBG_STRING("YCbCr 4:2:0")
} ;

static const char NEXUS_HdmiOutput_P_InvalidEdid[] = BDBG_STRING("***** Unable to read an EDID;  EDID data is invalid *****") ;
#endif


#define HDMI_MAX_SCRAMBLE_RETRY 5

void NEXUS_HdmiOutputModule_Print(void)
{
#if BDBG_DEBUG_BUILD
    NEXUS_HdmiOutputStatus *hdmiOutputStatus ;
    int i;

    hdmiOutputStatus = BKNI_Malloc(sizeof(NEXUS_HdmiOutputStatus)) ;
    if (hdmiOutputStatus == NULL)
    {
        BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        return ;
    }

    for (i=0 ; i < NEXUS_NUM_HDMI_OUTPUTS; i++)
    {
        NEXUS_HdmiOutput_GetStatus(&g_hdmiOutputs[i], hdmiOutputStatus) ;
        BDBG_LOG(("HDMI %d:%s%s%s",i,
            g_hdmiOutputs[i].opened ? "o" : "-",
            g_hdmiOutputs[i].videoConnected ? "v" : "-",
            g_hdmiOutputs[i].hdcpStarted ? "E" : "-"));

        if (g_hdmiOutputs[i].videoConnected)
        {
            BDBG_LOG((" video: s:%p d:%p",g_hdmiOutputs[i].videoConnector.source,g_hdmiOutputs[i].videoConnector.destination));
            BDBG_LOG((" audio: t:%d o:%p md:%p p:%lu",g_hdmiOutputs[i].audioConnector.objectType,(void *)g_hdmiOutputs[i].audioConnector.pObjectHandle,(void *)g_hdmiOutputs[i].audioConnector.pMixerData,(unsigned long)g_hdmiOutputs[i].audioConnector.port));
        }


        /* hardware status */
        BDBG_LOG(("rxAttached: %c   rxPowered: %c",
            hdmiOutputStatus->connected ? 'Y' : 'N',
            hdmiOutputStatus->rxPowered ? 'Y' : 'N')) ;

        BDBG_LOG(("txPower  Clock: %c CH2: %c CH1: %c CH0: %c",
            g_hdmiOutputs[i].txHwStatus.clockPower ? 'Y' : 'N',
            g_hdmiOutputs[i].txHwStatus.channelPower[2] ? 'Y' : 'N',
            g_hdmiOutputs[i].txHwStatus.channelPower[1] ? 'Y' : 'N',
            g_hdmiOutputs[i].txHwStatus.channelPower[0] ? 'Y' : 'N')) ;

        BDBG_LOG(("Total RxSense Changes:   %d",
            g_hdmiOutputs[i].txHwStatus.rxSenseCounter)) ;
        BDBG_LOG(("Total HP Changes:        %d",
            g_hdmiOutputs[i].txHwStatus.hotplugCounter)) ;


        BDBG_LOG(("HDMI Settings:")) ;

        BDBG_LOG(("   ColorSpace: %s",
            NEXUS_HdmiOutput_P_ColorSpace_Text[g_hdmiOutputs[i].settings.colorSpace])) ;

        BDBG_LOG(("   ColorDepth: %d ", g_hdmiOutputs[i].settings.colorDepth)) ;

        BDBG_LOG(("   Matrix Coefficient ID: %d  Override: %s",
            g_hdmiOutputs[i].settings.matrixCoefficients,
            g_hdmiOutputs[i].settings.overrideMatrixCoefficients ? "Yes" : "No")) ;

        BDBG_LOG(("   Color Range:           %d  Override: %s",
            g_hdmiOutputs[i].settings.colorRange,
            g_hdmiOutputs[i].settings.overrideColorRange ? "Yes" : "No")) ;

        if (!g_hdmiOutputs[i].txHwStatus.hotplugInterruptEnabled)
        {
            BDBG_ERR(("EXCESSIVE HP INTRs were detected; HP interrupt has been DISABLED")) ;
        }
        BDBG_LOG((" ")) ;
    }
    BKNI_Free(hdmiOutputStatus) ;
#endif
}

NEXUS_Error NEXUS_HdmiOutputModule_Standby_priv(bool enabled, const NEXUS_StandbySettings *pSettings)
{
#if NEXUS_POWER_MANAGEMENT
    int i;
    NEXUS_HdmiOutput *handle;
    BERR_Code rc = BERR_SUCCESS;

    BSTD_UNUSED(pSettings);

    for (i=0; i<NEXUS_NUM_HDMI_OUTPUTS; i++)
    {
        handle = &g_hdmiOutputs[i];

        /* if the handle not opened continue */
        if (!handle->opened) continue ;

        if (enabled)
        {
            NEXUS_HdmiOutput_P_StopTimers(handle) ;

            if (pSettings->mode == NEXUS_StandbyMode_eDeepSleep)
            {
                NEXUS_UnregisterEvent(handle->hotplugEventCallback);
                NEXUS_UnregisterEvent(handle->scrambleEventCallback);
                NEXUS_UnregisterEvent(handle->avRateChangeEventCallback);

#ifdef NEXUS_P_MHL_SUPPORT
                if (handle->mhlStandbyEventCallback)
                {
                    NEXUS_UnregisterEvent(handle->mhlStandbyEventCallback);
                }
#endif
                NEXUS_HdmiOutput_P_UninitHdcp(handle);
                BHDM_Close(handle->hdmHandle);
                handle->resumeFromS3 = true;
            }
            else
            {
                BHDM_StandbySettings standbySettings;

                BHDM_GetDefaultStandbySettings(&standbySettings);
                rc = BHDM_Standby(handle->hdmHandle, &standbySettings);
                handle->resumeFromS3 = false;
            }
        }
        else
        {
            if (handle->resumeFromS3)
            {
                BKNI_EventHandle hdmEvent;

#ifdef NEXUS_P_MHL_SUPPORT
                BKNI_EventHandle mhlStandbyEvent;
#endif

                BREG_I2C_Handle i2cRegHandle =
                    NEXUS_I2c_GetRegHandle(handle->openSettings.i2c, NEXUS_MODULE_SELF) ;

                handle->hdmSettings.bResumeFromS3 = true;

                rc = BHDM_Open(&handle->hdmHandle, g_pCoreHandles->chp, g_pCoreHandles->reg, g_pCoreHandles->bint, i2cRegHandle, &handle->hdmSettings);
                if (rc) goto err;

                 rc = NEXUS_HdmiOutput_P_InitHdcp(handle);
                 if (rc) goto err;

                 handle->hotplugEventCallback =
                    NEXUS_RegisterEvent(handle->notifyHotplugEvent, NEXUS_HdmiOutput_P_HotplugCallback, handle);
                 if ( NULL == handle->hotplugEventCallback )
                    goto err ;

                 rc = BHDM_InstallHotplugChangeCallback(handle->hdmHandle, NEXUS_HdmiOutput_P_HotPlug_isr, handle, 0);
                 if (rc) goto err;

#ifdef NEXUS_P_MHL_SUPPORT
                rc = BHDM_GetEventHandle(handle->hdmHandle, BHDM_EventMhlStandby, &mhlStandbyEvent);
                if (rc) goto err;

                if (mhlStandbyEvent)
                {
                    handle->mhlStandbyEventCallback =
                        NEXUS_RegisterEvent(mhlStandbyEvent, NEXUS_HdmiOutput_P_MhlStandbyCallback, handle);

                     rc = BHDM_MHL_InstallStandbyCallback(handle->hdmHandle, NEXUS_HdmiOutput_P_MhlStandby_isr, handle, 0);
                     if (rc) goto err;
                }
#endif

                 rc = BHDM_GetEventHandle(handle->hdmHandle, BHDM_EventScramble, &hdmEvent);
                 if (rc) goto err;

                 handle->scrambleEventCallback =
                    NEXUS_RegisterEvent(hdmEvent, NEXUS_HdmiOutput_P_ScrambleCallback, handle);

                 rc = BHDM_GetEventHandle(handle->hdmHandle, BHDM_EventAvRateChange, &hdmEvent);
                 if (rc) goto err;

                 handle->avRateChangeEventCallback =
                    NEXUS_RegisterEvent(hdmEvent, NEXUS_HdmiOutput_P_AvRateChangeCallback, handle);

            }
            else
            {
                rc = BHDM_Resume(handle->hdmHandle);
            }

            handle->lastRxState  = NEXUS_HdmiOutputState_eMax ;
            handle->rxState  = NEXUS_HdmiOutputState_eMax ;
            handle->lastHotplugState_isr = NEXUS_HdmiOutputState_eMax ;
            NEXUS_HdmiOutput_P_HotplugCallback(handle);

            {
                uint8_t deviceAttached ;

                rc = BHDM_RxDeviceAttached(handle->hdmHandle, &deviceAttached);
                BERR_TRACE(rc) ;

                if (!deviceAttached) /* rx device is REMOVED */
                {
                    handle->rxState =
                    handle->lastRxState =
                        NEXUS_HdmiOutputState_eDisconnected ;
                }
            }
        }
err:
        if (rc) { return BERR_TRACE(rc); }
    }
#else
    BSTD_UNUSED(enabled);
    BSTD_UNUSED(pSettings);
#endif
    return NEXUS_SUCCESS;
}

void NEXUS_HdmiOutput_GetDefaultOpenSettings( NEXUS_HdmiOutputOpenSettings *pSettings )
{
    BHDM_Settings hdmDefaultSettings ;

    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
    BHDM_GetDefaultSettings(&hdmDefaultSettings) ;

    pSettings->spd.deviceType = hdmDefaultSettings.eSpdSourceDevice ;
    BKNI_Memcpy(pSettings->spd.vendorName, hdmDefaultSettings.SpdVendorName, BAVC_HDMI_SPD_IF_VENDOR_LEN) ;
    BKNI_Memcpy(pSettings->spd.description, hdmDefaultSettings.SpdDescription, BAVC_HDMI_SPD_IF_DESC_LEN) ;

    pSettings->maxEdidRetries = 3;
    pSettings->powerPollingInterval = 500;

    pSettings->maxRxSenseRetries = 20 ;
    pSettings->rxSenseInterval = 50 ;

    pSettings->hotplugChangeThreshold = 0 ;

    pSettings->manualTmdsControl = false ;

}


NEXUS_HdmiOutputHandle NEXUS_HdmiOutput_Open( unsigned index, const NEXUS_HdmiOutputOpenSettings *pSettings )
{
    BERR_Code errCode;
    BREG_I2C_Handle i2cRegHandle;
    NEXUS_HdmiOutput *pOutput;
    BKNI_EventHandle hdmEvent;
    static const uint8_t defaultVendorName[BAVC_HDMI_SPD_IF_VENDOR_LEN+1]  = "Broadcom";
    static const uint8_t defaultDescription[BAVC_HDMI_SPD_IF_DESC_LEN+1] = "STB Refsw Design";
    uint8_t *ptr ;
    uint8_t deviceAttached ;
    NEXUS_HdmiOutputHandle master = NULL;
    const char *nexusEnv ;

    if (index >= NEXUS_ALIAS_ID && index-NEXUS_ALIAS_ID < NEXUS_NUM_HDMI_OUTPUTS) {
        BDBG_MSG(("%d aliasing %d(%p)", index, index-NEXUS_ALIAS_ID, (void *)&g_hdmiOutputs[index-NEXUS_ALIAS_ID]));
        index -= NEXUS_ALIAS_ID;
        master = &g_hdmiOutputs[index];
        if (!master->opened) {
            BDBG_ERR(("cannot alias %d because it is not opened", index));
            return NULL;
        }
        /* HdmiOutput is a read-only alias, so we can allow multiple */
    }

    if ( index >= NEXUS_NUM_HDMI_OUTPUTS )
    {
        BDBG_ERR(("HDMI output %u not supported on this chipset", index));
        return NULL;
    }

    if (!master) {
        if ( NULL == pSettings || NULL == pSettings->i2c )
        {
            BDBG_ERR(("I2C Handle must be provided for HDMI output"));
            return NULL;
        }
        pOutput = &g_hdmiOutputs[index];
        if ( pOutput->opened )
        {
            BDBG_ERR(("HDMI output %u already opened", index));
            return NULL;
        }
    }
    else {
        pOutput = BKNI_Malloc(sizeof(*pOutput));
    }
	BKNI_Memset(pOutput, 0, sizeof(*pOutput));

    NEXUS_OBJECT_INIT(NEXUS_HdmiOutput, pOutput);

    if (master) {
        pOutput->alias.master = master;
        return pOutput;
    }

    i2cRegHandle = NEXUS_I2c_GetRegHandle(pSettings->i2c, NEXUS_MODULE_SELF);
    if ( NULL == i2cRegHandle )
    {
        BDBG_ERR(("Invalid I2C Handle"));
        return NULL;
    }

    BHDM_GetDefaultSettings(&pOutput->hdmSettings);
    pOutput->openSettings = *pSettings;
    pOutput->hdcpFailureCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->hdcpStateChangedCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->hdcpSuccessCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->hdcpReceiverIdListReadyCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->hotplugCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->rxStatusCallback = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->notifyDisplay = NEXUS_TaskCallback_Create(pOutput, NULL);
    pOutput->sampleRate = BAVC_AudioSamplingRate_e48k;
    pOutput->audioBits = BAVC_AudioBits_e16;
    pOutput->audioFormat = BAVC_AudioFormat_ePCM;
    pOutput->audioNumChannels = 2;

    /* default NEXUS_HdmiOutputSettings */
    pOutput->settings.preFormatChangeAvMuteDelay = 100;
    pOutput->settings.postFormatChangeAvMuteDelay = 100;

    /* Set nexus colorSpace to match magnum default */
    pOutput->settings.colorSpace =
        NEXUS_P_ColorSpace_FromMagnum_isrsafe(pOutput->hdmSettings.stVideoSettings.eColorSpace) ;
    pOutput->settings.colorSpace = NEXUS_ColorSpace_eAuto ;

    pOutput->settings.overrideMatrixCoefficients = pOutput->hdmSettings.overrideDefaultColorimetry ;

    pOutput->settings.colorDepth = 0; /* 0 =  Auto Select */
    pOutput->settings.audioDitherEnabled = true;
    pOutput->settings.audioBurstType = NEXUS_SpdifOutputBurstType_ePause;
    pOutput->settings.audioBurstPadding = 0;

    BDBG_ASSERT(pSettings->spd.deviceType < NEXUS_HdmiSpdSourceDeviceType_eMax);

    BDBG_CASSERT(NEXUS_HDMI_SPD_VENDOR_NAME_MAX == BAVC_HDMI_SPD_IF_VENDOR_LEN) ;
    BDBG_CASSERT(NEXUS_HDMI_SPD_DESCRIPTION_MAX == BAVC_HDMI_SPD_IF_DESC_LEN) ;

    /* update Source Device Info Type */
    pOutput->hdmSettings.eSpdSourceDevice = pSettings->spd.deviceType;

    /* update Source Device Info Vendor Name */
    if ( pSettings->spd.vendorName[0] )
        ptr = (uint8_t *) pSettings->spd.vendorName ;
    else
        ptr = (uint8_t *) defaultVendorName ;
    BKNI_Memcpy(&pOutput->hdmSettings.SpdVendorName, ptr, sizeof(pOutput->hdmSettings.SpdVendorName));

    /* update Source Device Info Description */
    if ( pSettings->spd.description[0] )
        ptr = (uint8_t *) &pSettings->spd.description ;
    else
        ptr = (uint8_t *) defaultDescription ;
    BKNI_Memcpy(pOutput->hdmSettings.SpdDescription, ptr, sizeof(pOutput->hdmSettings.SpdDescription));


#if BDBG_DEBUG_BUILD
    nexusEnv = NEXUS_GetEnv("hdmi_bypass_edid") ;
    if ((!NEXUS_StrCmp(nexusEnv, "y")) || (!NEXUS_StrCmp(nexusEnv, "Y")))
    {
        /***************************************************************/
        /* FOR DEBUG/TEST ONLY - Override EDID Checking  */
        /* set to true to disable EDID checking */
        pOutput->hdmSettings.BypassEDIDChecking = true;
    }
    else {
        pOutput->hdmSettings.BypassEDIDChecking = pSettings->bypassEdidChecking;
    }

    nexusEnv = NEXUS_GetEnv("hdmi_use_debug_edid") ;
    if ((!NEXUS_StrCmp(nexusEnv, "y")) || (!NEXUS_StrCmp(nexusEnv, "Y")))
    {
        pOutput->hdmSettings.UseDebugEdid = true;
    }
#endif

    BDBG_MSG(("Application control of TMDS Signals: %s",
        pSettings->manualTmdsControl ? "Yes (not recommended)" : "No")) ;

    /* Setup connectors */
    NEXUS_VIDEO_OUTPUT_INIT(&pOutput->videoConnector, NEXUS_VideoOutputType_eHdmi, pOutput);
    NEXUS_AUDIO_OUTPUT_INIT(&pOutput->audioConnector, NEXUS_AudioOutputType_eHdmi, pOutput);
    BKNI_Snprintf(pOutput->audioConnectorName, sizeof(pOutput->audioConnectorName), "HDMI Output %d", index);
    pOutput->audioConnector.pName = pOutput->audioConnectorName;
    pOutput->audioConnector.port = NEXUS_AudioOutputPort_eMai;

    pOutput->hdmSettings.hTMR = g_pCoreHandles->tmr ;

    pOutput->hdmSettings.eCoreId = index ;

    /* set Hot Plug Settings */
    pOutput->hdmSettings.HotplugDetectThreshold = pSettings->hotplugChangeThreshold ;

    pOutput->hdmSettings.bResumeFromS3 = false;

    errCode = BHDM_Open(&pOutput->hdmHandle,
        g_pCoreHandles->chp, g_pCoreHandles->reg, g_pCoreHandles->bint, i2cRegHandle, &pOutput->hdmSettings);
    if ( errCode )
    {
        errCode = BERR_TRACE(errCode);
        return NULL;
    }

    /* Register for Hot Plug Events */
      errCode = BKNI_CreateEvent(&pOutput->notifyHotplugEvent) ;
     if (errCode) goto err_event ;

     pOutput->hotplugEventCallback =
        NEXUS_RegisterEvent(pOutput->notifyHotplugEvent, NEXUS_HdmiOutput_P_HotplugCallback, pOutput);
     if ( NULL == pOutput->hotplugEventCallback )
        goto err_event;

    /* Register for Scramble Events */
    errCode = BHDM_GetEventHandle(pOutput->hdmHandle, BHDM_EventScramble, &hdmEvent);
    if (errCode)
        goto err_event;

    pOutput->scrambleEventCallback =
        NEXUS_RegisterEvent(hdmEvent, NEXUS_HdmiOutput_P_ScrambleCallback, pOutput);
    if ( NULL == pOutput->scrambleEventCallback )
        goto err_event;


     errCode = BHDM_GetEventHandle(pOutput->hdmHandle, BHDM_EventAvRateChange, &hdmEvent);
     if (errCode) goto err_event ;

     pOutput->avRateChangeEventCallback =
        NEXUS_RegisterEvent(hdmEvent, NEXUS_HdmiOutput_P_AvRateChangeCallback, pOutput);
    if ( NULL == pOutput->avRateChangeEventCallback )
        goto err_event;


#ifdef NEXUS_P_MHL_SUPPORT
	errCode = NEXUS_HdmiOutput_P_OpenMhl(pOutput);
	 if (errCode)
        goto err_event;
#endif

    errCode = BHDM_RxDeviceAttached(pOutput->hdmHandle, &deviceAttached) ;
    BERR_TRACE(errCode);

    if (deviceAttached)
    {
        /* Update Rx supported hdcp version - use hdcp 2.2 if support */
        errCode = BHDM_HDCP_GetHdcpVersion(pOutput->hdmHandle, &pOutput->eHdcpVersion);
        /* default to HDCP 1.x if cannot read HDCP Version */
        if (errCode != BERR_SUCCESS) {
            pOutput->eHdcpVersion = BHDM_HDCP_Version_e1_1;
        }
    }

    /* If no SAGE support for HDCP 2.2 or 2.2 disabled, downgrade to 1.1 */
#if ! (NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT))
    if (pOutput->eHdcpVersion == BHDM_HDCP_Version_e2_2)
    {
            pOutput->eHdcpVersion = BHDM_HDCP_Version_e1_1 ;
    }
#endif

    errCode = NEXUS_HdmiOutput_P_InitHdcp(pOutput);
    if ( errCode )
    {
        errCode = BERR_TRACE(errCode);
        goto err_hdcp;
    }

    errCode = BHDM_InstallHotplugChangeCallback(
        pOutput->hdmHandle, NEXUS_HdmiOutput_P_HotPlug_isr, pOutput, 0);

    pOutput->rxState = NEXUS_HdmiOutputState_eNone ;
    pOutput->lastRxState = NEXUS_HdmiOutputState_eMax ;

    NEXUS_HdmiOutput_SelectVideoSettingsPriorityTable_priv() ;

    /* force a hot plug if a device is attached */
    if (deviceAttached)
    {
        NEXUS_HdmiOutput_P_HotplugCallback(pOutput);
    }

    pOutput->opened = true;
    NEXUS_OBJECT_REGISTER(NEXUS_VideoOutput, &pOutput->videoConnector, Open);
    NEXUS_OBJECT_REGISTER(NEXUS_AudioOutput, &pOutput->audioConnector, Open);

    return pOutput;

err_hdcp:

    if (pOutput->notifyHotplugEvent != NULL) {
        BKNI_DestroyEvent(pOutput->notifyHotplugEvent) ;
        pOutput->notifyHotplugEvent = NULL ;
    }

    if (pOutput->hotplugEventCallback != NULL) {
        NEXUS_UnregisterEvent(pOutput->hotplugEventCallback);
        pOutput->hotplugEventCallback = NULL;
    }

    if (pOutput->scrambleEventCallback != NULL) {
        NEXUS_UnregisterEvent(pOutput->scrambleEventCallback);
        pOutput->scrambleEventCallback = NULL;
    }

    if (pOutput->avRateChangeEventCallback != NULL) {
        NEXUS_UnregisterEvent(pOutput->avRateChangeEventCallback);
        pOutput->avRateChangeEventCallback = NULL;
    }

    if (pOutput->hdcpFailureCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->hdcpFailureCallback);
        pOutput->hdcpFailureCallback = NULL;
    }

    if (pOutput->hdcpStateChangedCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->hdcpStateChangedCallback);
        pOutput->hdcpStateChangedCallback = NULL;
    }

    if (pOutput->hdcpSuccessCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->hdcpSuccessCallback);
        pOutput->hdcpSuccessCallback = NULL;
    }

    if (pOutput->hdcpReceiverIdListReadyCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->hdcpReceiverIdListReadyCallback);
        pOutput->hdcpReceiverIdListReadyCallback = NULL;
    }

    if (pOutput->hotplugCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->hotplugCallback);
        pOutput->hotplugCallback = NULL;
    }

    if (pOutput->rxStatusCallback != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->rxStatusCallback);
        pOutput->rxStatusCallback = NULL;
    }

    if (pOutput->notifyDisplay != NULL) {
        NEXUS_TaskCallback_Destroy(pOutput->notifyDisplay);
        pOutput->notifyDisplay = NULL;
    }

err_event:

    BHDM_Close(pOutput->hdmHandle);
    return NULL;
}


static void NEXUS_HdmiOutput_P_StopTimers(NEXUS_HdmiOutputHandle hdmiOutput )
{
    NEXUS_OBJECT_ASSERT(NEXUS_HdmiOutput, hdmiOutput);

    if ( hdmiOutput->powerTimer )
    {
        NEXUS_CancelTimer(hdmiOutput->powerTimer);
        hdmiOutput->powerTimer  = NULL ;
    }

    if (hdmiOutput->connectTimer)
    {
        NEXUS_CancelTimer(hdmiOutput->connectTimer);
        hdmiOutput->connectTimer = NULL;
    }
}

static void NEXUS_HdmiOutput_P_Finalizer( NEXUS_HdmiOutputHandle hdmiOutput )
{
    NEXUS_OBJECT_ASSERT(NEXUS_HdmiOutput, hdmiOutput);

    if (hdmiOutput->alias.master) {
        NEXUS_OBJECT_DESTROY(NEXUS_HdmiOutput, hdmiOutput);
        BKNI_Free(hdmiOutput);
        return;
    }
    else {
        /* close slaves */
        unsigned i;
        for (i=0;i<NEXUS_NUM_HDMI_OUTPUTS;i++) {
            if (g_hdmiOutputs[i].opened && g_hdmiOutputs[i].alias.master == hdmiOutput) {
                /* slave is no longer callable by client */
                NEXUS_OBJECT_UNREGISTER(NEXUS_HdmiOutput, &g_hdmiOutputs[i], Close);
                NEXUS_HdmiOutput_Close(&g_hdmiOutputs[i]);
            }
        }
    }

#if NEXUS_HAS_HDMI_INPUT
    NEXUS_HdmiOutput_SetRepeaterInput(hdmiOutput, NULL);
#endif

    if ( hdmiOutput->audioConnector.pMixerData )
    {
        BDBG_ERR(("Audio connector is still active.  Please call NEXUS_AudioOutput_Shutdown() first."));
    }
    if ( hdmiOutput->videoConnector.destination )
    {
        BDBG_ERR(("Video connector is still active.  Please call NEXUS_VideoOutput_Shutdown() first."));
    }

    NEXUS_HdmiOutput_P_StopTimers(hdmiOutput) ;

    NEXUS_UnregisterEvent(hdmiOutput->hotplugEventCallback);
    NEXUS_UnregisterEvent(hdmiOutput->scrambleEventCallback);
    NEXUS_UnregisterEvent(hdmiOutput->avRateChangeEventCallback);
    NEXUS_HdmiOutput_P_UninitHdcp(hdmiOutput);

    if (hdmiOutput->notifyHotplugEvent != NULL)
    {
        BKNI_DestroyEvent(hdmiOutput->notifyHotplugEvent) ;
    }

#ifdef NEXUS_P_MHL_SUPPORT
	NEXUS_HdmiOutput_P_CloseMhl(hdmiOutput);
#endif

    BHDM_Close(hdmiOutput->hdmHandle);

    NEXUS_TaskCallback_Destroy(hdmiOutput->hdcpFailureCallback);
    NEXUS_TaskCallback_Destroy(hdmiOutput->hdcpStateChangedCallback);
    NEXUS_TaskCallback_Destroy(hdmiOutput->hdcpSuccessCallback);
    NEXUS_TaskCallback_Destroy(hdmiOutput->hdcpReceiverIdListReadyCallback);
    NEXUS_TaskCallback_Destroy(hdmiOutput->hotplugCallback);
     NEXUS_TaskCallback_Destroy(hdmiOutput->rxStatusCallback);
    NEXUS_TaskCallback_Destroy(hdmiOutput->notifyDisplay);

    if (hdmiOutput->crc.queue) {
        BKNI_Free(hdmiOutput->crc.queue);
    }

    /* memset of zero will also wipe out BDBG_OBJECT */
    BKNI_Memset(hdmiOutput, 0, sizeof(*hdmiOutput));
}

static void NEXUS_HdmiOutput_P_Release( NEXUS_HdmiOutputHandle hdmiOutput )
{
    if (!IS_ALIAS(hdmiOutput)) {
        NEXUS_OBJECT_UNREGISTER(NEXUS_VideoOutput, &hdmiOutput->videoConnector, Close);
        NEXUS_OBJECT_UNREGISTER(NEXUS_AudioOutput, &hdmiOutput->audioConnector, Close);
    }
    return;
}

NEXUS_OBJECT_CLASS_MAKE_WITH_RELEASE(NEXUS_HdmiOutput, NEXUS_HdmiOutput_Close);

/**
Summary:
Get current settings
**/
void NEXUS_HdmiOutput_GetSettings(
    NEXUS_HdmiOutputHandle output,
    NEXUS_HdmiOutputSettings *pSettings    /* [out] Settings */
    )
{

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    *pSettings = output->settings;
}

static BAVC_HDMI_DRM_DescriptorId NEXUS_P_HdmiOutputDrmStaticMetadataType_ToMagnum_isrsafe(NEXUS_HdmiDynamicRangeMasteringStaticMetadataType type)
{
    switch (type) {
    case NEXUS_HdmiDynamicRangeMasteringStaticMetadataType_e1:         return BAVC_HDMI_DRM_DescriptorId_eType1;
    default: return BAVC_HDMI_DRM_DescriptorId_eMax;
    }
}

#if !BDBG_NO_MSG
static const char * eotfStrings[] =
{
    "sdr",
    "gmhdr",
    "pqhdr",
    "future",
    NULL
};

static const char * metadataTypeStrings[] =
{
    "1",
    NULL
};

static void NEXUS_HdmiOutput_P_PrintType1DrmStaticMetadata(const NEXUS_HdmiType1DynamicRangeMasteringStaticMetadata * pMetadata)
{
    const NEXUS_MasteringDisplayColorVolume * pMdcv;
    const NEXUS_ContentLightLevel * pCll;

    pMdcv = &pMetadata->masteringDisplayColorVolume;
    pCll = &pMetadata->contentLightLevel;

    BDBG_MSG(("      green: (%d, %d)", pMdcv->greenPrimary.x, pMdcv->greenPrimary.y));
    BDBG_MSG(("      blue: (%d, %d)", pMdcv->bluePrimary.x, pMdcv->bluePrimary.y));
    BDBG_MSG(("      red: (%d, %d)", pMdcv->redPrimary.x, pMdcv->redPrimary.y));
    BDBG_MSG(("      white: (%d, %d)", pMdcv->whitePoint.x, pMdcv->whitePoint.y));
    BDBG_MSG(("      luma: (%d, %d)", pMdcv->luminance.max, pMdcv->luminance.min));
    BDBG_MSG(("      cll: %d", pCll->max));
    BDBG_MSG(("      fal: %d", pCll->maxFrameAverage));
}

static void NEXUS_HdmiOutput_P_PrintType1DrmStaticMetadataChanges(const NEXUS_HdmiType1DynamicRangeMasteringStaticMetadata * pOldMetadata, const NEXUS_HdmiType1DynamicRangeMasteringStaticMetadata * pNewMetadata)
{
    const NEXUS_MasteringDisplayColorVolume * pOldMdcv;
    const NEXUS_ContentLightLevel * pOldCll;
    const NEXUS_MasteringDisplayColorVolume * pNewMdcv;
    const NEXUS_ContentLightLevel * pNewCll;

    pOldMdcv = &pOldMetadata->masteringDisplayColorVolume;
    pOldCll = &pOldMetadata->contentLightLevel;
    pNewMdcv = &pNewMetadata->masteringDisplayColorVolume;
    pNewCll = &pNewMetadata->contentLightLevel;

    BDBG_MSG(("      green: (%d, %d) -> (%d, %d)",
        pOldMdcv->greenPrimary.x, pOldMdcv->greenPrimary.y,
        pNewMdcv->greenPrimary.x, pNewMdcv->greenPrimary.y));
    BDBG_MSG(("      blue: (%d, %d) -> (%d, %d)",
        pOldMdcv->bluePrimary.x, pOldMdcv->bluePrimary.y,
        pNewMdcv->bluePrimary.x, pNewMdcv->bluePrimary.y));
    BDBG_MSG(("      red: (%d, %d) -> (%d, %d)",
        pOldMdcv->redPrimary.x, pOldMdcv->redPrimary.y,
        pNewMdcv->redPrimary.x, pNewMdcv->redPrimary.y));
    BDBG_MSG(("      white: (%d, %d) -> (%d, %d)",
        pOldMdcv->whitePoint.x, pOldMdcv->whitePoint.y,
        pNewMdcv->whitePoint.x, pNewMdcv->whitePoint.y));
    BDBG_MSG(("      luma: (%d, %d) -> (%d, %d)",
        pOldMdcv->luminance.max, pOldMdcv->luminance.min,
        pNewMdcv->luminance.max, pNewMdcv->luminance.min));
    BDBG_MSG(("      cll: %d -> %d",
        pOldCll->max,
        pNewCll->max));
    BDBG_MSG(("      fal: %d -> %d",
        pOldCll->maxFrameAverage,
        pNewCll->maxFrameAverage));
}

static void NEXUS_HdmiOutput_P_PrintDrmInfoFrameChanges(const NEXUS_HdmiDynamicRangeMasteringInfoFrame * pOldInfoFrame, const NEXUS_HdmiDynamicRangeMasteringInfoFrame * pNewInfoFrame)
{
    BDBG_MSG(("    DRMInfoFrame")) ;
    BDBG_MSG(("      eotf: %s -> %s",
        eotfStrings[pOldInfoFrame->eotf],
        eotfStrings[pNewInfoFrame->eotf]));
    BDBG_MSG(("      type: %s -> %s",
        metadataTypeStrings[pOldInfoFrame->metadata.type],
        metadataTypeStrings[pNewInfoFrame->metadata.type]));
    if (pOldInfoFrame->metadata.type != pNewInfoFrame->metadata.type)
    {
        BDBG_MSG(("      [old]"));
        switch (pOldInfoFrame->metadata.type)
        {
            case NEXUS_HdmiDynamicRangeMasteringStaticMetadataType_e1:
                NEXUS_HdmiOutput_P_PrintType1DrmStaticMetadata(&pOldInfoFrame->metadata.typeSettings.type1);
                break;
            default:
                break;
        }
        BDBG_MSG(("      [new]"));
        switch (pNewInfoFrame->metadata.type)
        {
            case NEXUS_HdmiDynamicRangeMasteringStaticMetadataType_e1:
                NEXUS_HdmiOutput_P_PrintType1DrmStaticMetadata(&pNewInfoFrame->metadata.typeSettings.type1);
                break;
            default:
                break;
        }
    }
    else
    {
        switch (pNewInfoFrame->metadata.type)
        {
            case NEXUS_HdmiDynamicRangeMasteringStaticMetadataType_e1:
                NEXUS_HdmiOutput_P_PrintType1DrmStaticMetadataChanges(
                    &pOldInfoFrame->metadata.typeSettings.type1,
                    &pNewInfoFrame->metadata.typeSettings.type1);
                break;
            default:
                break;
        }
    }
}
#endif

static void NEXUS_HdmiOutput_P_DrmInfoFrame_ToMagnum(BAVC_HDMI_DRMInfoFrame * magnum, const NEXUS_HdmiDynamicRangeMasteringInfoFrame * nexus)
{
    magnum->eEOTF = NEXUS_P_VideoEotf_ToMagnum_isrsafe(nexus->eotf);
    magnum->eDescriptorId = NEXUS_P_HdmiOutputDrmStaticMetadataType_ToMagnum_isrsafe(nexus->metadata.type);
    switch (nexus->metadata.type)
    {
        case NEXUS_HdmiDynamicRangeMasteringStaticMetadataType_e1:
            magnum->Type1.DisplayPrimaries[0].X = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.greenPrimary.x;
            magnum->Type1.DisplayPrimaries[0].Y = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.greenPrimary.y;
            magnum->Type1.DisplayPrimaries[1].X = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.bluePrimary.x;
            magnum->Type1.DisplayPrimaries[1].Y = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.bluePrimary.y;
            magnum->Type1.DisplayPrimaries[2].X = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.redPrimary.x;
            magnum->Type1.DisplayPrimaries[2].Y = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.redPrimary.y;
            magnum->Type1.WhitePoint.X = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.whitePoint.x;
            magnum->Type1.WhitePoint.Y = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.whitePoint.y;
            magnum->Type1.DisplayMasteringLuminance.Max = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.luminance.max;
            magnum->Type1.DisplayMasteringLuminance.Min = nexus->metadata.typeSettings.type1.masteringDisplayColorVolume.luminance.min;
            magnum->Type1.MaxContentLightLevel = nexus->metadata.typeSettings.type1.contentLightLevel.max;
            magnum->Type1.MaxFrameAverageLightLevel = nexus->metadata.typeSettings.type1.contentLightLevel.maxFrameAverage;
            break;
        default:
            break;
    }
}

NEXUS_Error NEXUS_HdmiOutput_SetSettings( NEXUS_HdmiOutputHandle output, const NEXUS_HdmiOutputSettings *pSettings )
{
    NEXUS_Error errCode = NEXUS_SUCCESS ;
    bool audioChanged = false;
    BHDM_Settings hdmSettings ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    if (IS_ALIAS(output)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);

    NEXUS_TaskCallback_Set(output->hotplugCallback, &pSettings->hotplugCallback);
    NEXUS_TaskCallback_Set(output->rxStatusCallback, &pSettings->hdmiRxStatusChanged);
#ifdef NEXUS_P_MHL_SUPPORT
    NEXUS_TaskCallback_Set(output->mhlStandbyCallback, &pSettings->mhlStandbyCallback);
#endif

    BHDM_GetHdmiSettings(output->hdmHandle, &hdmSettings) ;
        hdmSettings.bEnableScdcMonitoring = pSettings->hdmiRxScdcMonitoring ;
    BHDM_SetHdmiSettings(output->hdmHandle, &hdmSettings) ;

    if (pSettings->overrideMatrixCoefficients)
    {
        NEXUS_MatrixCoefficients matrixCoefficients = pSettings->matrixCoefficients;
        NEXUS_VideoFormat outputFormat = pSettings->outputFormat;
        if ((matrixCoefficients == NEXUS_MatrixCoefficients_eUnknown) ||
            (matrixCoefficients == NEXUS_MatrixCoefficients_eHdmi_RGB) ||
            (matrixCoefficients == NEXUS_MatrixCoefficients_eDvi_Full_Range_RGB) ||
            (matrixCoefficients == NEXUS_MatrixCoefficients_eHdmi_Full_Range_YCbCr))
        {
            BDBG_WRN(("Hdmi output NEXUS_MatrixCoefficients=%d, it is to be deprecated soon", matrixCoefficients));
        }
        if (NEXUS_VideoFormat_eUnknown != outputFormat)
        {
            if (((outputFormat >= NEXUS_VideoFormat_e1080i) && (outputFormat <= NEXUS_VideoFormat_e720p30hz) && /* HD */
                 (matrixCoefficients != NEXUS_MatrixCoefficients_eItu_R_BT_709) && (matrixCoefficients != NEXUS_MatrixCoefficients_eXvYCC_709)) ||
                ((outputFormat >= NEXUS_VideoFormat_e3840x2160p24hz) && (outputFormat <= NEXUS_VideoFormat_e4096x2160p60hz) && /* UHD */
                 (matrixCoefficients != NEXUS_MatrixCoefficients_eItu_R_BT_709) && (matrixCoefficients != NEXUS_MatrixCoefficients_eXvYCC_709) &&
                 (matrixCoefficients != NEXUS_MatrixCoefficients_eItu_R_BT_2020_NCL) && (matrixCoefficients != NEXUS_MatrixCoefficients_eItu_R_BT_2020_CL)) ||
                ((outputFormat == NEXUS_VideoFormat_eNtsc) && /* NTSC */
                 (matrixCoefficients != NEXUS_MatrixCoefficients_eSmpte_170M) && (matrixCoefficients != NEXUS_MatrixCoefficients_eXvYCC_601)) ||
                ((outputFormat == NEXUS_VideoFormat_ePal) && /* PAL */
                 (matrixCoefficients != NEXUS_MatrixCoefficients_eItu_R_BT_470_2_BG) && (matrixCoefficients != NEXUS_MatrixCoefficients_eXvYCC_601)))
                BDBG_MSG(("Hdmi output bad combination of displayFmt %d and matrixCoefficients %d", outputFormat, matrixCoefficients));
        }
    }

     /* See if audio settings have changed */
    if ( output->settings.audioDitherEnabled != pSettings->audioDitherEnabled
    ||   output->settings.audioBurstType != pSettings->audioBurstType
    ||   output->settings.audioBurstPadding != pSettings->audioBurstPadding
    ||   BKNI_Memcmp(&output->settings.audioChannelStatusInfo, &pSettings->audioChannelStatusInfo, sizeof(pSettings->audioChannelStatusInfo)) )
    {
        audioChanged = true;
    }

    /* when overriding the default colorimetry */
    /* make sure colorspaces (pixel encodings) and color matrices are consistent */
    if (pSettings->overrideMatrixCoefficients)
    {
        bool rgbMatrix ;

        rgbMatrix =
            (pSettings->matrixCoefficients == NEXUS_MatrixCoefficients_eDvi_Full_Range_RGB)
            || (pSettings->matrixCoefficients == NEXUS_MatrixCoefficients_eHdmi_RGB)
            || (pSettings->colorSpace == NEXUS_ColorSpace_eRgb); /* new algorithm, use colorSpace */

        if (rgbMatrix && (pSettings->colorSpace != NEXUS_ColorSpace_eRgb))
        {
            BDBG_WRN(("Invalid Colorspace %d for override of matrixCoefficients %d;  Settings not applied",
                pSettings->colorSpace, pSettings->matrixCoefficients)) ;
            errCode = NEXUS_INVALID_PARAMETER ;
            BERR_TRACE(errCode);
            goto done ;
        }
    }

    /* YCbCr 4:2:2 is always 12 bits */
    /* Refer to HDMI 1.4 section 6.2.4 paragraph 4 */
    if ((pSettings->colorSpace == NEXUS_ColorSpace_eYCbCr422)
    && ((pSettings->colorDepth != 0 /* Auto */) && pSettings->colorDepth != 12))
    {
        BDBG_WRN(("Invalid Colordepth %d for Colorspace YCbCr 4:2:2  Settings not applied ",
            pSettings->colorDepth)) ;
        goto done ;
    }

    BHDM_GetDRMInfoFramePacket(output->hdmHandle, &hdmSettings.stDRMInfoFrame) ;
        NEXUS_HdmiOutput_P_DrmInfoFrame_ToMagnum(&hdmSettings.stDRMInfoFrame, &pSettings->dynamicRangeMasteringInfoFrame);
    BHDM_SetDRMInfoFramePacket(output->hdmHandle, &hdmSettings.stDRMInfoFrame) ;

    if ((pSettings->colorDepth  != output->settings.colorDepth)
    || (pSettings->colorSpace != output->settings.colorSpace)

    || (pSettings->overrideMatrixCoefficients != output->settings.overrideMatrixCoefficients)
    || (pSettings->matrixCoefficients != output->settings.matrixCoefficients)

    || (pSettings->overrideColorRange != output->settings.overrideColorRange)
    || (pSettings->colorRange != output->settings.colorRange))
    {
#if BDBG_DEBUG_BUILD
        BDBG_MSG(("SetSettings")) ;
        BDBG_MSG(("   ColorSpace from %s to %s",
            NEXUS_HdmiOutput_P_ColorSpace_Text[output->settings.colorSpace],
            NEXUS_HdmiOutput_P_ColorSpace_Text[pSettings->colorSpace])) ;

        BDBG_MSG(("   ColorDepth from %d bit to %d bit ",
            output->settings.colorDepth, pSettings->colorDepth)) ;

        BDBG_MSG(("   Override Matrix Coefficient Setting from %s to %s",
            output->settings.overrideMatrixCoefficients ? "Yes" : "No",
            pSettings->overrideMatrixCoefficients ? "Yes" : "No")) ;

        BDBG_MSG(("   Matrix Coefficients ID from %d to %d",
            output->settings.matrixCoefficients, pSettings->matrixCoefficients)) ;

        BDBG_MSG(("   Override Color Range Setting from %s to %s",
            output->settings.overrideColorRange ? "Yes" : "No",
            pSettings->overrideColorRange ? "Yes" : "No")) ;

        BDBG_MSG(("   Color Range from %d to %d",
            output->settings.colorRange, pSettings->colorRange)) ;
#endif

        /* if there are two rapid calls to SetSettings, the second must not clear a pending formatChangeUpdate */
        output->formatChangeUpdate = true;
    }

    if (BKNI_Memcmp(&output->settings.dynamicRangeMasteringInfoFrame, &pSettings->dynamicRangeMasteringInfoFrame, sizeof(output->settings.dynamicRangeMasteringInfoFrame)))
    {
        /* if there are two rapid calls to SetSettings, the second must not clear a pending formatChangeUpdate */
        output->formatChangeUpdate = true;
#if !BDBG_NO_MSG
        BDBG_MSG(("SetSettings")) ;
        NEXUS_HdmiOutput_P_PrintDrmInfoFrameChanges(&output->settings.dynamicRangeMasteringInfoFrame, &pSettings->dynamicRangeMasteringInfoFrame);
#endif
    }

    /* save previous copy of outputSettings */
    output->previousSettings = output->settings;

    /* save new settings passed in */
    output->settings = *pSettings;

    if (output->crc.size != pSettings->crcQueueSize) {
        void *new_ptr = NULL, *old_ptr;

        /* defer the free until after critical section */
        old_ptr = output->crc.queue;
        /* queue size of 1 is treated same as 0 because it can't hold anything */
        if (pSettings->crcQueueSize > 1) {
            new_ptr = BKNI_Malloc(pSettings->crcQueueSize * sizeof(output->crc.queue[0]));
            if (!new_ptr) {
                return BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
            }
        }

        /* must synchronize with ISR, so set state in CS */
        BKNI_EnterCriticalSection();
        output->crc.queue = new_ptr;
        output->crc.size = pSettings->crcQueueSize>1?pSettings->crcQueueSize:0;
        output->crc.wptr = output->crc.rptr = 0; /* flush */
        BKNI_LeaveCriticalSection();

        if (old_ptr) {
            BKNI_Free(old_ptr);
        }
    }

    /* TODO: should the display be reconfigured, even if HdmiOutput is not on? */
    /* Save settings and return if not connected */
    if ( output->rxState != NEXUS_HdmiOutputState_ePoweredOn )
    {
        BDBG_MSG(("Tx/Rx is not powered (%d)... new settings will be applied when powered",
			output->rxState)) ;
        errCode = NEXUS_SUCCESS ;
        goto done ;

    }


    /* See NEXUS_VideoOutput_P_SetHdmiSettings for how HdmiOutputSettings are applied to VDC */

    /* After saving the settings, notify the Display module that new HdmiOutputSettings must be applied to VDC.
    A delay of one context switch is required to keep HdmiOutput and Display in separate synchronization contexts. */
    NEXUS_TaskCallback_Fire(output->notifyDisplay);  /* NEXUS_VideoOutput_P_SetHdmiSettings */
    if ( output->notifyAudioEvent && audioChanged )
    {
         BKNI_SetEvent(output->notifyAudioEvent);
    }
    errCode = NEXUS_SUCCESS ;

done :
    return errCode ;
}




void NEXUS_HdmiOutput_ReenableHotplugInterrupt(NEXUS_HdmiOutputHandle output)
{
    BHDM_ReenableHotplugInterrupt(output->hdmHandle) ;
}


NEXUS_Error NEXUS_HdmiOutput_GetStatus( NEXUS_HdmiOutputHandle output, NEXUS_HdmiOutputStatus *pStatus )
{
    uint16_t supported3DFormats[BFMT_VideoFmt_eMaxCount];
    BHDM_EDID_AudioDescriptor supportedAudioFormats[BAVC_AudioFormat_eMaxCount];
    BHDM_EDID_ColorimetryDataBlock colorimetryDB ;
    NEXUS_Error errCode;
    BHDM_EDID_RxVendorSpecificDB vendorDb;
    int i;
    uint8_t deviceAttached ;
    uint8_t RxSense;
    BHDM_Video_Settings stVideoSettings ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(output);
    BDBG_CASSERT(sizeof(NEXUS_HdmiOutputMonitorRange) == sizeof(BHDM_EDID_MonitorRange));
    BKNI_Memset(pStatus, 0, sizeof(*pStatus));
    pStatus->txHardwareStatus.hotplugInterruptEnabled = true ;
    BKNI_Memset(supported3DFormats, 0, sizeof (supported3DFormats));

    /* get index/coreId of the HDMI Handle */
    pStatus->index = (unsigned) output->hdmSettings.eCoreId ;

    /* Check for device */
    errCode = BHDM_RxDeviceAttached(output->hdmHandle, &deviceAttached) ;
    if ((errCode) || (!deviceAttached))
    {
        pStatus->connected = false ;
        goto done ;
    }
    pStatus->connected = true ;

    /* do not report status until ready */
    if (output->rxState == NEXUS_HdmiOutputState_eMax)
    {
        BDBG_MSG(("HdmiOutput not initialized ")) ;
        goto done ;
    }

    /* Get current RxSense; could still be reported OFF if power was just enabled */
    errCode = BHDM_GetReceiverSense(output->hdmHandle, &RxSense) ;
    pStatus->rxPowered = RxSense ;

    /* HDMI must be connected to Nexus Display (i.e. powered) to get a valid RxSense value */
    /* update the last receiver sense value only when a valid value can be read */
    if ((!RxSense) && (!errCode) && (output->videoConnected))
    {
        BDBG_MSG(("Attached Rx may be powered down...")) ;
        output->lastReceiverSense = RxSense ;
    }

    if (output->invalidEdid)
    {
        if (!output->invalidEdidReported)
        {
            BDBG_WRN(("%s", NEXUS_HdmiOutput_P_InvalidEdid)) ;
            output->invalidEdidReported = true ;
            BDBG_WRN(("Invalid EDID; Report 640x480 as preferred video format")) ;
        }

        pStatus->preferredVideoFormat = NEXUS_VideoFormat_eVesa640x480p60hz ;

        /* no further checking is required; cannot determine any addl support */
        goto done ;
    }
    else
    {
        /*** HDMI Device ***/
        errCode = BHDM_EDID_IsRxDeviceHdmi(output->hdmHandle, &vendorDb, &pStatus->hdmiDevice);
        if ( !errCode )  /* save the physical address if successfully retrieved from the EDID */
        {
            /* Copy over the HDMI DB specific data. */
            pStatus->physicalAddressA = vendorDb.PhysAddr_A;
            pStatus->physicalAddressB = vendorDb.PhysAddr_B;
            pStatus->physicalAddressC = vendorDb.PhysAddr_C;
            pStatus->physicalAddressD = vendorDb.PhysAddr_D;
        }

        /*** Receiver Name ***/
        errCode = BHDM_EDID_GetMonitorName(output->hdmHandle, (uint8_t *)pStatus->monitorName);
        if ( errCode ) /* set the Monitor Name to NULL if error retrieving monitor name from the EDID */
        {
            pStatus->monitorName[0] = 0;
            errCode = BERR_TRACE(errCode);
            /* Keep going - other parts of the EDID may be valid */
        }

        /*** Monitor Range Info ***/
        errCode = BHDM_EDID_GetMonitorRange(output->hdmHandle,(BHDM_EDID_MonitorRange *) &pStatus->monitorRange);
        if ( errCode )
        {
            BDBG_WRN(("Unable to get Monitor h/v range information; errCode: %d", errCode)) ;
            /* Keep going - other parts of the EDID may be valid */
        }

        /*** Preferred format ***/
        pStatus->preferredVideoFormat = NEXUS_HdmiOutput_P_GetPreferredFormat(output);


        /*** Supported Video Formats ***/
        errCode = NEXUS_HdmiOutput_P_GetSupportedFormats(output, pStatus->videoFormatSupported) ;

        /*** Supported Colorimetries ***/
        /* These structures are just copies of one another, this should keep them in sync */
        BDBG_CASSERT(sizeof(NEXUS_HdmiOutputMonitorColorimetry) == sizeof(BHDM_EDID_ColorimetryDataBlock));
        errCode = BHDM_EDID_GetColorimetryDB(output->hdmHandle, &colorimetryDB) ;
        if (!errCode ) /* save supported colorimetries if successfully retrieved from the EDID */
        {
            BKNI_Memcpy(&pStatus->monitorColorimetry, &colorimetryDB,
                sizeof(NEXUS_HdmiOutputMonitorColorimetry)) ;
        }

        /*** Get Supported 3D Formats ***/
        errCode = BHDM_EDID_GetSupported3DFormats(output->hdmHandle, supported3DFormats);
        if (!errCode )  /* save supported 3D formats if successfully retrieved from the EDID  */
        {
	        /* add supported 3D formats */
	        for (i=0; i < BFMT_VideoFmt_eMaxCount; i++)
	        {
	            if (supported3DFormats[i])
	            {
	                NEXUS_VideoFormat nexusFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(i);
	                if ( nexusFormat != NEXUS_VideoFormat_eUnknown)
	                    pStatus->hdmi3DFormatsSupported[nexusFormat] = supported3DFormats[i];
	            }
	        }
        }


        /*** Get Supported audio formats ***/
        if ( pStatus->hdmiDevice )
        {
            errCode = BHDM_EDID_GetSupportedAudioFormats(output->hdmHandle, supportedAudioFormats);
            if ( !errCode ) /* save supported audio formats if successfully retrieved from EDID */
            {
                for ( i = 0; i < BAVC_AudioFormat_eMaxCount; i++ )
                {
                    if ( supportedAudioFormats[i].Supported )
                    {
                        switch ( i )
                        {
                        case BAVC_AudioFormat_ePCM:
                            pStatus->maxAudioPcmChannels = supportedAudioFormats[i].AudioChannels;
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_ePcm] = true;
                            break;
                        case BAVC_AudioFormat_eMP3:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eMp3] = true;
                            break;
                        case BAVC_AudioFormat_eMPEG1:
                        case BAVC_AudioFormat_eMPEG2:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eMpeg] = true;
                            break;
                        case BAVC_AudioFormat_eAAC:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eAac] = true;
                            break;
                        case BAVC_AudioFormat_eDTSHD:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eDtsHd] = true;
                            /* Fall Through (DTS-HD Implies DTS by the spec) */
                        case BAVC_AudioFormat_eDTS:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eDts] = true;
                            break;
                        case BAVC_AudioFormat_eDDPlus:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eAc3Plus] = true;
                            /* Fall Through (AC3+ Implies AC3 by the spec) */
                        case BAVC_AudioFormat_eAC3:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eAc3] = true;
                            break;
                        case BAVC_AudioFormat_eMATMLP:
                            pStatus->audioCodecSupported[NEXUS_AudioCodec_eMlp] = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

        /* Save auto lipsync audio/video latency info from the attached Rx */
        pStatus->autoLipsyncInfo.audioLatency = vendorDb.Audio_Latency;
        pStatus->autoLipsyncInfo.videoLatency = vendorDb.Video_Latency;
        pStatus->autoLipsyncInfo.interlacedAudioLatency = vendorDb.Interlaced_Audio_Latency;
        pStatus->autoLipsyncInfo.interlacedVideoLatency = vendorDb.Interlaced_Video_Latency;
    }

    errCode = BHDM_GetVideoSettings(output->hdmHandle, &stVideoSettings) ;
    if (errCode)
    {
        BDBG_ERR(("Unexpected error %d getting  HDMI Video Settings", errCode)) ;
        errCode = NEXUS_UNKNOWN ;
        goto done ;
    }

    pStatus->colorSpace =
        NEXUS_P_ColorSpace_FromMagnum_isrsafe(stVideoSettings.eColorSpace) ;

    (void) BHDM_GetHdmiSettings(output->hdmHandle, &output->hdmSettings);
    pStatus->videoFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(output->hdmSettings.eInputVideoFmt);
    pStatus->aspectRatio = NEXUS_P_AspectRatio_FromMagnum_isrsafe(output->hdmSettings.eAspectRatio);

    switch (output->hdmSettings.eAudioFormat) {
    case BAVC_AudioFormat_ePCM: pStatus->audioFormat = NEXUS_AudioCodec_ePcm; break;
    case BAVC_AudioFormat_eAC3: pStatus->audioFormat = NEXUS_AudioCodec_eAc3; break;
    case BAVC_AudioFormat_eMPEG1: pStatus->audioFormat = NEXUS_AudioCodec_eMpeg; break;
    case BAVC_AudioFormat_eMP3: pStatus->audioFormat = NEXUS_AudioCodec_eMp3; break;
    case BAVC_AudioFormat_eMPEG2: pStatus->audioFormat = NEXUS_AudioCodec_eMpeg; break;
    case BAVC_AudioFormat_eAAC: pStatus->audioFormat = NEXUS_AudioCodec_eAacPlus; break;
    case BAVC_AudioFormat_eDTS: pStatus->audioFormat = NEXUS_AudioCodec_eDts; break;
    case BAVC_AudioFormat_eDDPlus: pStatus->audioFormat = NEXUS_AudioCodec_eAc3Plus; break;
    case BAVC_AudioFormat_eWMAPro: pStatus->audioFormat = NEXUS_AudioCodec_eWmaPro; break;

    case BAVC_AudioFormat_eAVS:
    case BAVC_AudioFormat_eATRAC:
    case BAVC_AudioFormat_eOneBit:
    case BAVC_AudioFormat_eDTSHD:
    case BAVC_AudioFormat_eMATMLP:
    case BAVC_AudioFormat_eDST:

    default:
        /* If the codec is actually supported by our silicon, please extend NEXUS_AudioCodec. If not, this warning is sufficient. */
        BDBG_WRN(("Unable to convert HDMI audio info frame codec %d to NEXUS_HdmiOutputStatus.audioFormat",
            output->hdmSettings.eAudioFormat));
        break;
    }


    switch (output->hdmSettings.eAudioSamplingRate) {
    case BAVC_AudioSamplingRate_e32k :
        pStatus->audioSamplingRate = 32000 ; break ;
    case BAVC_AudioSamplingRate_e44_1k :
        pStatus->audioSamplingRate = 44100 ; break ;
    case BAVC_AudioSamplingRate_e48k :
        pStatus->audioSamplingRate = 48000 ; break ;
    case BAVC_AudioSamplingRate_e88_2k :
        pStatus->audioSamplingRate = 88200 ; break ;
    case BAVC_AudioSamplingRate_e96k:
        pStatus->audioSamplingRate = 96000 ; break ;
    case BAVC_AudioSamplingRate_e176_4k:
        pStatus->audioSamplingRate = 176400 ; break ;
    case BAVC_AudioSamplingRate_e192k:
        pStatus->audioSamplingRate = 192000 ; break ;
    default: break;
    }

    switch (output->hdmSettings.eAudioBits) {
    case BAVC_AudioBits_e16: pStatus->audioSamplingSize = 16; break;
    case BAVC_AudioBits_e20: pStatus->audioSamplingSize = 20; break;
    case BAVC_AudioBits_e24: pStatus->audioSamplingSize = 24; break;
    default: break;
    }

    switch (output->hdmSettings.stAudioInfoFrame.ChannelCount) {
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e2Channels: pStatus->audioChannelCount = 2; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e3Channels: pStatus->audioChannelCount = 3; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e4Channels: pStatus->audioChannelCount = 4; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e5Channels: pStatus->audioChannelCount = 5; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e6Channels: pStatus->audioChannelCount = 6; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e7Channels: pStatus->audioChannelCount = 7; break;
    case BAVC_HDMI_AudioInfoFrame_ChannelCount_e8Channels: pStatus->audioChannelCount = 8; break;
    default: break;
    }

    {
        BHDM_MONITOR_Status txStatus ;

        errCode = BHDM_MONITOR_GetHwStatusTx(output->hdmHandle, &txStatus) ;
        if (!errCode)
        {
            /* hardware status */
            output->txHwStatus.clockPower = txStatus.EnabledTMDS_Clock;
            output->txHwStatus.channelPower[2] = txStatus.EnabledTMDS_CH2 ;
            output->txHwStatus.channelPower[1] = txStatus.EnabledTMDS_CH1 ;
            output->txHwStatus.channelPower[0] = txStatus.EnabledTMDS_CH0 ;

            output->txHwStatus.hotplugCounter = txStatus.TotalHotPlugChanges ;  /* total since device opened */
            output->txHwStatus.rxSenseCounter = txStatus.TotalRxSenseChanges ;    /* total since device opened */

            output->txHwStatus.hotplugInterruptEnabled = !txStatus.TxHotPlugInterruptDisabled ;
            output->txHwStatus.unstableFormatDetectedCounter = txStatus.UnstableFormatDetectedCounter ;

            BKNI_Memcpy(&pStatus->txHardwareStatus, &output->txHwStatus,
                sizeof(NEXUS_HdmiOutputTxHardwareStatus)) ;
        }
    }

#if BHDM_HAS_HDMI_20_SUPPORT
    {
        BHDM_ScrambleConfig stScrambleConfig ;
        BHDM_SCDC_GetScrambleConfiguration(output->hdmHandle, &stScrambleConfig) ;
            pStatus->txHardwareStatus.scrambling = stScrambleConfig.txScrambleEnable ;
            pStatus->rxHardwareStatus.descrambling = stScrambleConfig.rxStatusFlags_Scramble ;
    }
#endif

done:
    return 0;
}

NEXUS_Error NEXUS_HdmiOutput_GetBasicEdidData( NEXUS_HdmiOutputHandle output, NEXUS_HdmiOutputBasicEdidData *pData )
{
    NEXUS_Error errCode;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(output);
    BDBG_ASSERT(NULL != pData);

    /* These structures are just copies of one another, this should keep them in sync */
    BDBG_CASSERT(sizeof(NEXUS_HdmiOutputBasicEdidData) == sizeof(BHDM_EDID_BasicData));

    errCode = BHDM_EDID_GetBasicData(output->hdmHandle,(BHDM_EDID_BasicData *)pData);
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }

    return BERR_SUCCESS;
}

NEXUS_Error NEXUS_HdmiOutput_GetEdidBlock( NEXUS_HdmiOutputHandle output, unsigned blockNum, NEXUS_HdmiOutputEdidBlock *pBlock )
{
    NEXUS_Error errCode;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(output);
    BDBG_ASSERT(NULL != pBlock);

    errCode = BHDM_EDID_ReadNthBlock(output->hdmHandle, blockNum, pBlock->data, sizeof(pBlock->data));
    if ( errCode )
    {
        return BERR_TRACE(errCode);
    }

    return BERR_SUCCESS;
}


static NEXUS_Error NEXUS_HdmiOutput_P_GetSupportedFormats(
    NEXUS_HdmiOutputHandle output,
    bool *nexusSupportedVideoFormats)
{
    BERR_Code errCode = BERR_SUCCESS ;
    bool *magnumSupported444VideoFormats ;
    bool *magnumSupported420VideoFormats ;
    uint8_t i ;

    /* Get all supported video formats (yCbCrRgb444 yCbCr420 */
    BKNI_Memset(nexusSupportedVideoFormats, 0,
        sizeof (bool) * NEXUS_VideoFormat_eMax) ;

    /******************************/
    /* Check for YCbCr 4:4:4, RGB Support */
    /******************************/
    magnumSupported444VideoFormats = BKNI_Malloc(sizeof(bool) * BFMT_VideoFmt_eMaxCount) ;
    if (magnumSupported444VideoFormats == NULL)
    {
        errCode = BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        goto done ;
    }

    BKNI_Memset(magnumSupported444VideoFormats, 0, sizeof (bool) * BFMT_VideoFmt_eMaxCount) ;

    errCode = BHDM_EDID_GetSupportedVideoFormats(output->hdmHandle, magnumSupported444VideoFormats) ;
    if ( errCode )
    {
        BDBG_WRN(("Unable to determine supported video formats")) ;
        /* Keep going - support for other formats can be determined */
    }
    else
    {
        for ( i = 0; i < BFMT_VideoFmt_eMaxCount; i++ )
        {
            NEXUS_VideoFormat nexusFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(i);
            if ( nexusFormat != NEXUS_VideoFormat_eUnknown )
            {
                if (magnumSupported444VideoFormats[i])
                    nexusSupportedVideoFormats[nexusFormat] = true ;
            }
        }
    }
    BKNI_Free(magnumSupported444VideoFormats) ;


    /******************************/
    /* Check for YCbCr 4:2:0 Support         */
    /******************************/
    magnumSupported420VideoFormats = BKNI_Malloc(sizeof(bool) * BFMT_VideoFmt_eMaxCount) ;
    if (magnumSupported420VideoFormats == NULL)
    {
        errCode = BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        goto done ;
    }
    BKNI_Memset(magnumSupported420VideoFormats, 0, sizeof (bool) * BFMT_VideoFmt_eMaxCount) ;

    errCode = BHDM_EDID_GetSupported420VideoFormats(output->hdmHandle, magnumSupported420VideoFormats) ;
    if ( errCode )
    {
        BDBG_WRN(("Unable to determine supported 420 video formats")) ;
        /* Keep going - support for other formats can be determined */
    }
    else
    {
        for ( i = 0; i < BFMT_VideoFmt_eMaxCount; i++ )
        {
            NEXUS_VideoFormat nexusFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(i);
            if ( nexusFormat != NEXUS_VideoFormat_eUnknown )
            {
                if (magnumSupported420VideoFormats[i])
                    nexusSupportedVideoFormats[nexusFormat] = true ;
            }
        }
    }
    BKNI_Free(magnumSupported420VideoFormats) ;

done:
   return (errCode)  ;

}


NEXUS_Error NEXUS_HdmiOutput_GetEdidData(
    NEXUS_HdmiOutputHandle output, NEXUS_HdmiOutputEdidData *pEdid)
{
    NEXUS_Error errCode;
    BFMT_VideoFmt magnumFormat;
    BHDM_EDID_HDRStaticDB hdrdb ;
    BHDM_EDID_VideoCapabilityDataBlock videoCapabilityDB ;
    bool errorFound = false ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(output);
    BDBG_ASSERT(NULL != pEdid);

    BKNI_Memset(pEdid, 0, sizeof(NEXUS_HdmiOutputEdidData)) ;
    if (output->invalidEdid)
    {
        if (!output->invalidEdidReported)
        {
            BDBG_WRN(("%s", NEXUS_HdmiOutput_P_InvalidEdid)) ;
            output->invalidEdidReported = true ;
        }
    }
    else
    {
        /* BASIC EDID data */
        /* These structures are just copies of one another, this should keep them in sync */
        BDBG_CASSERT(sizeof(NEXUS_HdmiOutputBasicEdidData) == sizeof(BHDM_EDID_BasicData));
        errCode = BHDM_EDID_GetBasicData(output->hdmHandle, (BHDM_EDID_BasicData *) &pEdid->basicData);
        if ( errCode ) { errorFound = true ; BERR_TRACE(errCode); }

        /* convert magnum to nexus video format */
        magnumFormat = pEdid->basicData.preferredVideoFormat ;
        pEdid->basicData.preferredVideoFormat =
           NEXUS_P_VideoFormat_FromMagnum_isrsafe(magnumFormat) ;


        /* Video Capability Data Block - Monitor's Capabilities */
        errCode = BHDM_EDID_GetVideoCapabilityDB(output->hdmHandle, &videoCapabilityDB) ;
        if ( errCode )
        {
            errorFound = true ;
            errCode = BERR_TRACE(errCode);
        }
        else if (videoCapabilityDB.valid) /* save supported colorimetries if successfully retrieved from the EDID */
        {
            pEdid->videoCapabilitydb.valid = true ;
            pEdid->videoCapabilitydb.selectableRgb = videoCapabilityDB.bQuantizationSelectatbleRGB ;
            pEdid->videoCapabilitydb.selectableYcc = videoCapabilityDB.bQuantizationSelectatbleYCC ;
        }


        /* HDMI VSDB */
        /* These structures are just copies of one another, this should keep them in sync */
        BDBG_CASSERT(sizeof(NEXUS_HdmiOutputEdidRxHdmiVsdb) == sizeof(BHDM_EDID_RxVendorSpecificDB));
        errCode = BHDM_EDID_GetHdmiVsdb(output->hdmHandle, (BHDM_EDID_RxVendorSpecificDB *) &pEdid->hdmiVsdb) ;
        if ( errCode ) { errorFound = true ; BERR_TRACE(errCode); }


        /* HDMI Forum VSDB (HF-VSDB) */
        /* These structures are just copies of one another, this should keep them in sync */
        BDBG_CASSERT(sizeof(NEXUS_HdmiOutputEdidRxHdmiForumVsdb) == sizeof(BHDM_EDID_RxHfVsdb));
        errCode = BHDM_EDID_GetHdmiForumVsdb(output->hdmHandle,
            (BHDM_EDID_RxHfVsdb *) &pEdid->hdmiForumVsdb) ;
        if ( errCode ) { errorFound = true ; BERR_TRACE(errCode); }

        errCode = BHDM_EDID_GetHdrStaticMetadatadb(output->hdmHandle, &hdrdb) ;
        if ( errCode ) { errorFound = true ; BERR_TRACE(errCode); }
        pEdid->hdrdb.valid = hdrdb.valid ;
        BDBG_CASSERT(NEXUS_VideoEotf_eMax == (NEXUS_VideoEotf) BHDM_EDID_HdrDbEotfSupport_eMax) ;
        BKNI_Memcpy(&pEdid->hdrdb.eotfSupported, &hdrdb.bEotfSupport, NEXUS_VideoEotf_eMax) ;

        errCode = NEXUS_HdmiOutput_P_GetSupportedFormats(output, pEdid->videoFormatSupported) ;
        if ( errCode ) { errorFound = true ; BERR_TRACE(errCode); }
    }

    if (errorFound)
        errCode = NEXUS_UNKNOWN ;
    else
        errCode = NEXUS_SUCCESS ;
    return errCode ;
}


NEXUS_Error NEXUS_HdmiOutput_GetVideoFormatSupport(
    NEXUS_HdmiOutputHandle output,
    NEXUS_VideoFormat nexusFormat,
    NEXUS_HdmiOutputEdidVideoFormatSupport *pVideoFormatSupport    /* [out] */
)
{
    NEXUS_Error errCode = NEXUS_SUCCESS ;
    BFMT_VideoFmt magnumFormat ;

    bool *magnumSupported444VideoFormats ;
    bool *magnumSupported420VideoFormats ;
    uint16_t *magnumSupported3dVideoFormats ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(output);
    BDBG_ASSERT(NULL != pVideoFormatSupport);


    BKNI_Memset(pVideoFormatSupport, 0, sizeof(NEXUS_HdmiOutputEdidVideoFormatSupport)) ;

    if (nexusFormat == NEXUS_VideoFormat_eUnknown)
    {
        errCode = BERR_TRACE(NEXUS_UNKNOWN) ;
        goto done ;
    }

    errCode = NEXUS_P_VideoFormat_ToMagnum_isrsafe(nexusFormat, &magnumFormat) ;
    if ( errCode )
    {
        errCode = BERR_TRACE(errCode);
        goto done ;
    }


    /* YCbCr 4:4:4, RGB */
    magnumSupported444VideoFormats = BKNI_Malloc(sizeof(bool) * BFMT_VideoFmt_eMaxCount) ;
    if (magnumSupported444VideoFormats == NULL)
    {
        errCode = BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        goto done ;
    }

    BKNI_Memset(magnumSupported444VideoFormats, 0, sizeof (bool) * BFMT_VideoFmt_eMaxCount) ;
    errCode = BHDM_EDID_GetSupportedVideoFormats(output->hdmHandle, magnumSupported444VideoFormats) ;
    if ( errCode == NEXUS_SUCCESS )
    {
        if (magnumSupported444VideoFormats[magnumFormat])
        {
            pVideoFormatSupport->yCbCr444rgb444 = true ;
            /* all HDMI Rx are required to support both YCbCr 422 and YCbCr 444 */
            pVideoFormatSupport->yCbCr422 = true ;
        }
    }
    BKNI_Free(magnumSupported444VideoFormats) ;


    /* YCbCr 4:2:0 */
    magnumSupported420VideoFormats = BKNI_Malloc(sizeof(bool) * BFMT_VideoFmt_eMaxCount) ;
    if (magnumSupported420VideoFormats == NULL)
    {
        errCode = BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        goto done ;
    }

    BKNI_Memset(magnumSupported420VideoFormats, 0, sizeof (bool) * BFMT_VideoFmt_eMaxCount) ;
    errCode = BHDM_EDID_GetSupported420VideoFormats(output->hdmHandle, magnumSupported420VideoFormats) ;
    if ( errCode == NEXUS_SUCCESS ) {
        if (magnumSupported420VideoFormats[magnumFormat])
            pVideoFormatSupport->yCbCr420 = true ;
    }
    BKNI_Free(magnumSupported420VideoFormats) ;


    /* Get supported 3D Formats */
    magnumSupported3dVideoFormats = BKNI_Malloc(sizeof(uint16_t) * BFMT_VideoFmt_eMaxCount) ;
    if (magnumSupported3dVideoFormats == NULL)
    {
        errCode = BERR_TRACE(NEXUS_OUT_OF_DEVICE_MEMORY) ;
        goto done ;
    }
    BKNI_Memset(magnumSupported3dVideoFormats, 0, sizeof (uint16_t) * BFMT_VideoFmt_eMaxCount) ;
    errCode = BHDM_EDID_GetSupported3DFormats(output->hdmHandle, magnumSupported3dVideoFormats);
    if ( errCode == NEXUS_SUCCESS ) {
        /* add supported 3D formats */
        pVideoFormatSupport->_3d.framePacking =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_FramePacking ;

        pVideoFormatSupport->_3d.fieldAlternative =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_FieldAlternative ;

        pVideoFormatSupport->_3d.lineAlternative =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_LineAlternative ;

        pVideoFormatSupport->_3d.sideBySideFull=
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_SideBySideFull ;

        pVideoFormatSupport->_3d.lDepth =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_LDepth ;

        pVideoFormatSupport->_3d.lDepthGraphics =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_LDepthGraphics;

        pVideoFormatSupport->_3d.topAndBottom =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_TopAndBottom;

        pVideoFormatSupport->_3d.sideBySideHalfHorizontal =
            magnumSupported3dVideoFormats[magnumFormat]  & NEXUS_HdmiOutput_3DStructure_SideBySideHalfHorizontal ;

        pVideoFormatSupport->_3d.sideBySideHalfQuincunx =
            magnumSupported3dVideoFormats[magnumFormat] & NEXUS_HdmiOutput_3DStructure_SideBySideHalfQuincunx ;
    }
    BKNI_Free(magnumSupported3dVideoFormats) ;

done:
    return errCode ;
}


void NEXUS_HdmiOutput_GetDefaultVideoSettings(
    NEXUS_HdmiOutputVideoSettings *pSettings
)
{
	pSettings->videoFormat = NEXUS_VideoFormat_e720p ;
	pSettings->colorDepth = 8 ;
	pSettings->colorSpace = NEXUS_ColorSpace_eYCbCr444 ;
}


NEXUS_VideoOutput NEXUS_HdmiOutput_GetVideoConnector( NEXUS_HdmiOutputHandle output )
{
    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    if (IS_ALIAS(output)) return NULL;
    return &output->videoConnector;
}

NEXUS_AudioOutputHandle NEXUS_HdmiOutput_GetAudioConnector( NEXUS_HdmiOutputHandle output )
{
    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);
    if (IS_ALIAS(output)) return NULL;
    return &output->audioConnector;
}

static NEXUS_VideoFormat NEXUS_HdmiOutput_P_GetPreferredFormat(NEXUS_HdmiOutputHandle output)
{
    NEXUS_Error errCode;
    BFMT_VideoFmt magnumFormat;
    NEXUS_VideoFormat nexusFormat;
    BHDM_EDID_DetailTiming detailedTiming ;
    uint8_t nativeFormat, videoIdCode;
    unsigned detailTimingNum = 1;

    magnumFormat = BFMT_VideoFmt_eDVI_640x480p;     /* Default to safe mode */

get_next_detailed_timing_from_edid:
    /* first detailed EDID block contains preferred format */
    (void) BHDM_EDID_GetDetailTiming(output->hdmHandle, detailTimingNum, &detailedTiming, &magnumFormat);

    /* if the 1st/current detailed (preferred) format is not supported, */
    /*     check if the next detailed timing format is supported */
    if (!BFMT_SUPPORT_HDMI(magnumFormat))
    {
        detailTimingNum++;
        goto get_next_detailed_timing_from_edid ;
    }

    /* Translate the first selection */
    nexusFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(magnumFormat);

    /* If preferred format is VESA, check for HD format */
    if (( BFMT_IS_VESA_MODE(magnumFormat) && (magnumFormat != BFMT_VideoFmt_eDVI_640x480p))
    || NEXUS_VideoFormat_eUnknown == nexusFormat )
    {
        errCode = BHDM_EDID_GetVideoDescriptor(output->hdmHandle, detailTimingNum, &videoIdCode, &magnumFormat, &nativeFormat);
        if (errCode)
        {
            goto done ;
        }
        BDBG_MSG(("Using CEA-861 Video ID %d (HD) instead of Preferred VESA format", videoIdCode));
    }

    /* Translate */
    nexusFormat = NEXUS_P_VideoFormat_FromMagnum_isrsafe(magnumFormat);
    if ( NEXUS_VideoFormat_eUnknown == nexusFormat )
    {
        BDBG_WRN(("Unknown preferred format %d, defaulting to NTSC", magnumFormat));
        nexusFormat =  NEXUS_VideoFormat_eNtsc;
        goto done ;
    }

done:
    return nexusFormat;
}


static void NEXUS_HdmiOutput_P_RxRemoved(void *pContext)
{
    NEXUS_HdmiOutputHandle output = (NEXUS_HdmiOutputHandle) pContext;
    BERR_Code errCode ;
    uint8_t deviceAttached ;

    /* if already waiting to connect,
         cancel request to connect since the Rx has been removed */
    if (output->connectTimer)
    {
        NEXUS_CancelTimer(output->connectTimer) ;
        output->connectTimer = NULL ;
    }

    /* if no device is attached, cancel RxSense polling if active */
    errCode = BHDM_RxDeviceAttached(output->hdmHandle, &deviceAttached) ;
    BERR_TRACE(errCode) ;
    if (!deviceAttached)
    {
        output->rxState = NEXUS_HdmiOutputState_eDisconnected ;

        /* reset cached edid settings */
        output->edidHdmiDevice = false ;
        BKNI_Memset(&output->edidVendorSpecificDB, 0, sizeof(BHDM_EDID_RxVendorSpecificDB)) ;

        if (output->powerTimer)
        {
            NEXUS_CancelTimer(output->powerTimer) ;
            output->powerTimer = NULL ;
        }
    }

    /* Connected -> disconnected or Connected -> powered down.  Disable output and fire callback */
    if ((!output->tmdsDataEnabled)
    &&  (output->videoConnected))
    {
        BDBG_MSG(("TMDS data lines have already been disabled...")) ;
        /* continue through to disable the TMDS clock if data has already been disabled */
    }

    /* disable TMDS data lines data and notify app of disconnect */
    if (!output->openSettings.manualTmdsControl)
    {
        NEXUS_HdmiOutput_P_SetTmdsSignalData(output, false) ;
    }
    else
    {
        /* debug message for manual control by App */
        BDBG_MSG(("NEXUS_HdmiOutput_P_RxRemoved: Expected TMDS DATA OFF")) ;
    }

#if NEXUS_HAS_HDMI_INPUT
#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
    /* Update hdcp2.x RxCaps settings */
    if (output->hdmiInput)
    {
        /* Update RxCaps */
        NEXUS_Module_Lock(g_NEXUS_hdmiOutputModuleSettings.modules.hdmiInput);
        errCode = NEXUS_HdmiInput_UpdateHdcp2xRxCaps_priv(output->hdmiInput, false);
        NEXUS_Module_Unlock(g_NEXUS_hdmiOutputModuleSettings.modules.hdmiInput);
        if (errCode != NEXUS_SUCCESS)
        {
            errCode = BERR_TRACE(errCode);
            return;
        }
    }
#endif
#endif

    /* notify Nexus Display of the cable removal to disable the HDMI Output */
    output->formatChangeUpdate = true ;
    NEXUS_TaskCallback_Fire(output->notifyDisplay) ;

    NEXUS_HdmiOutput_P_HdcpNotifyHotplug(output) ;

    /* notify the app of the cable removal */
    NEXUS_TaskCallback_Fire(output->hotplugCallback) ;

#if BHDM_HAS_HDMI_20_SUPPORT
    BHDM_SCDC_DisableScrambleTx(output->hdmHandle) ;
#endif


}


static void NEXUS_HdmiOutput_P_HotplugTimerExpiration(void *pContext)
{

    NEXUS_HdmiOutputHandle output = (NEXUS_HdmiOutputHandle)pContext;

    output->connectTimer = NULL ;

    /*
    ** after a hot plug (cable connected) event, some Rx Devices may require
    ** extra time before it can be determined if the device is powered.
    ** re-schedule the connectTimer until we have waited at least
    ** output->openSettings.rxSenseInterval * output->openSettings.maxRxSenseRetries
    ** by default this is 50ms * 20 = 1000ms (1 second)
     */
    if (output->rxState == NEXUS_HdmiOutputState_eRxSenseCheck)
    {
        BERR_Code errCode ;
        uint8_t rxSense ;

        errCode = BHDM_GetReceiverSense(output->hdmHandle, &rxSense);
        if (errCode != BERR_SUCCESS)
        {
            BDBG_WRN(("Unable to get Receiver Sense")) ;
        }
        output->lastReceiverSense = rxSense ;

        /* If in RxSense check state; we are waiting to determine if attached Rx is ON ;
            do not set State to OFF until RxSense check has been completed
        */

        if (rxSense)
        {
            output->rxState = NEXUS_HdmiOutputState_ePoweredOn ;
            output->lastHotplugState_isr = NEXUS_HdmiOutputState_eNone ;
        }
        else if (output->checkRxSenseCount < output->openSettings.maxRxSenseRetries)
        {
            output->connectTimer = NEXUS_ScheduleTimer(output->openSettings.rxSenseInterval,
                NEXUS_HdmiOutput_P_HotplugTimerExpiration, output) ;
            output->checkRxSenseCount++ ;

            BDBG_MSG(("Check for RxSense try %2d of %2d; Elapsed time: %4dms",
                output->checkRxSenseCount, output->openSettings.maxRxSenseRetries,
                output->checkRxSenseCount * output->openSettings.rxSenseInterval)) ;
            goto done ;
        }
        else
        {
            BDBG_WRN(("RxSense checking is finished; Rx is powered down...")) ;
            /* get the updated TMDS Signals */
            NEXUS_HdmiOutput_P_GetTmdsSignals (output) ;
            output->rxState = NEXUS_HdmiOutputState_ePoweredDown ;
        }
    }

    /* the RxSense Check has timed out
        so the Rx device has been removed (DEVICE REMOVED)
        OR the Rx device has been powered ON
        OR the Rx device has been powered OFF
    */


    if ( output->rxState == NEXUS_HdmiOutputState_eDisconnected)
    {
        NEXUS_HdmiOutput_P_RxRemoved(output) ;
        goto done ;
    }

    else if ( output->rxState == NEXUS_HdmiOutputState_ePoweredOn )
    {
        /* notify Nexus Display of the cable insertion to re-enable HDMI Output */
        output->formatChangeUpdate = true ;
        NEXUS_TaskCallback_Fire(output->notifyDisplay);

        if (output->notifyAudioEvent)
        {
             BKNI_SetEvent(output->notifyAudioEvent);
        }

        NEXUS_HdmiOutput_P_CheckHdcpVersion(output);

        {
#if NEXUS_HAS_HDMI_INPUT
#if NEXUS_HAS_SAGE && defined(NEXUS_HAS_HDCP_2X_SUPPORT)
            if (output->hdmiInput && (output->eHdcpVersion == BHDM_HDCP_Version_e2_2))
            {
                NEXUS_Error rc;

                /* Update RxCaps */
                NEXUS_Module_Lock(g_NEXUS_hdmiOutputModuleSettings.modules.hdmiInput);
                rc = NEXUS_HdmiInput_UpdateHdcp2xRxCaps_priv(output->hdmiInput, true);
                NEXUS_Module_Unlock(g_NEXUS_hdmiOutputModuleSettings.modules.hdmiInput);
                if (rc != NEXUS_SUCCESS)
                {
                    rc = BERR_TRACE(rc);
                    return;
                }
            }
#endif
#endif
        }
    }
    else if ( output->rxState == NEXUS_HdmiOutputState_ePoweredDown )
    {
        /* Disconnected -> powered down - treat as no event */
        BDBG_MSG(("Receiver powered down... at line %d", __LINE__));

        if (!output->openSettings.manualTmdsControl)
            NEXUS_HdmiOutput_P_SetTmdsSignalData(output, false) ;
        else
        {
            /* debug message for manual control by App */
            BDBG_MSG(("NEXUS_HdmiOutput_P_HotplugTimerExpiration: Expected TMDS OFF")) ;
        }

        /* enable Rx Sense polling to check if/when a HDMI Rx is powered back on...
        a small number of receivers may not generate a Hotplug when powered on */
        if (!output->powerTimer
        &&   output->openSettings.powerPollingInterval)
        {
            BDBG_MSG(("Start Rx Sense polling...")) ;
            output->powerTimer =
                NEXUS_ScheduleTimer(output->openSettings.powerPollingInterval,
                NEXUS_HdmiOutput_P_RxSenseTimerExpiration, output);

            if (!output->powerTimer)
            {
                BERR_TRACE(NEXUS_NOT_INITIALIZED) ;
                goto done ;
            }
        }
        else
        {
            BDBG_WRN(("RxSense polling timer is already running!")) ;
        }

#if BHDM_HAS_HDMI_20_SUPPORT
        /* make sure all Auto I2c channels are disabled */
        BKNI_EnterCriticalSection() ;
            BHDM_AUTO_I2C_SetChannels_isr(output->hdmHandle, false) ;
        BKNI_LeaveCriticalSection() ;
#endif
    }

    /* Only notify hdcplib of CONNECT event for HDCP 1.x to update
    internal hdcplib state. For HDCP2.x, the state is updated through SAGE indications */
    if ((output->eHdcpVersion == BHDM_HDCP_Version_e1_0)
    || (output->eHdcpVersion == BHDM_HDCP_Version_e1_1))
    {
        NEXUS_HdmiOutput_P_HdcpNotifyHotplug(output) ;
    }

    /* notify application of hotplug status change */
    NEXUS_TaskCallback_Fire(output->hotplugCallback);

done:
    return ;
}


static void NEXUS_HdmiOutput_P_ReadEdid(NEXUS_HdmiOutputHandle hdmiOutput)
{

    BERR_Code errCode ;
    unsigned int i = 1 ;

    hdmiOutput->invalidEdid = true ;
    hdmiOutput->invalidEdidReported = false ;

    do
    {
         errCode = BHDM_EDID_Initialize(hdmiOutput->hdmHandle);
        if (errCode == BERR_SUCCESS)
        {
            hdmiOutput->invalidEdid = false ;
            break ;
        }

        /* Rx Device has been removed .  Abort */
        if (errCode == BHDM_NO_RX_DEVICE)
        {
            BDBG_WRN(("Device Disconnected while trying to read EDID")) ;
            hdmiOutput->rxState = NEXUS_HdmiOutputState_eDisconnected ;
             goto done ;
        }

        /*
        this delay should really not be needed, but allow some time
        between EDID read attempts in case the Rx is still settling
        */
        BKNI_Sleep(100) ;
        if (i < hdmiOutput->openSettings.maxEdidRetries)
        {
            BDBG_WRN(("Read EDID Try %d of %d",
            i + 1, hdmiOutput->openSettings.maxEdidRetries)) ;
        }
    } while ( i++ <  hdmiOutput->openSettings.maxEdidRetries ) ;

    if ( i >= hdmiOutput->openSettings.maxEdidRetries )
    {
        BDBG_ERR(("Unable to read EDID after %d attempts", hdmiOutput->openSettings.maxEdidRetries ));
        goto done ;
    }

    /* cache EDID settings */
    errCode = BHDM_EDID_IsRxDeviceHdmi(hdmiOutput->hdmHandle,
        &hdmiOutput->edidVendorSpecificDB, &hdmiOutput->edidHdmiDevice) ;
    if (errCode)
    {
        BDBG_ERR(("Error determining Rx Device type")) ;
        goto done ;
    }

done:
    return ;
}


static void NEXUS_HdmiOutput_P_ScrambleCallback(void *pContext)
{
    NEXUS_HdmiOutputHandle hdmiOutput = (NEXUS_HdmiOutputHandle) pContext ;
    NEXUS_HdmiOutputStatus hdmiOutputStatus ;
    BHDM_SCDC_StatusControlData scdc ;

    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);

    BKNI_Memset(&scdc, 0, sizeof(BHDM_SCDC_StatusControlData)) ;

    NEXUS_HdmiOutput_GetStatus(hdmiOutput, &hdmiOutputStatus) ;

#if BHDM_HAS_HDMI_20_SUPPORT
    BHDM_SCDC_ReadStatusControlData(hdmiOutput->hdmHandle, &scdc) ;
    hdmiOutput->rxHwStatus.descrambling = scdc.RxScramblerStatus ;

    /* Some early HDMI 2.0 TVs do not enable deScrambling after being configured to do so */
    /* sometimes the re-write of the Scramble bit will work and sometimes it does not */
    /* Hot Plug or switching inputs and back to HDMI port sometimes helps */
    if (hdmiOutputStatus.txHardwareStatus.scrambling != hdmiOutputStatus.rxHardwareStatus.descrambling)
    {
        BDBG_ERR(("Mismatch in Scramble Status for STB/Tx= %s vs. %s/Rx= %s",
            hdmiOutputStatus.txHardwareStatus.scrambling ? "Yes" : "No",
            hdmiOutputStatus.monitorName, hdmiOutputStatus.rxHardwareStatus.descrambling ? "Yes" : "No")) ;

        /*************************/
        /* Possible No Display due to Rx */
        /*************************/

        /* an attempt to reconfigure the scrambling can be done here */
        /* BUT an endless loop could occur, if the Rx does not switch to scrambling */

        /* if rescramble config has been tried MAX times, give up retrying and notify the app */
        if (++hdmiOutput->retryScrambleCount > HDMI_MAX_SCRAMBLE_RETRY)
        {
            BDBG_ERR(("*** Mismatch in Rx/Tx scramble config; Probable <No Signal> on display ***")) ;
            goto notifyApp ;
        }


        BDBG_WRN(("Attempting to reset HDMI scrambling configuration; retry scramble conifg %d of %d...",
			hdmiOutput->retryScrambleCount, HDMI_MAX_SCRAMBLE_RETRY)) ;
        NEXUS_HdmiOutput_ResetScrambling(hdmiOutput) ;

        return ;

    }

notifyApp:
    BDBG_LOG(("HDMI Link Scramble Status: %s",
        hdmiOutputStatus.txHardwareStatus.scrambling ? "***Scrambling***" : "Normal video")) ;
        hdmiOutput->retryScrambleCount = 0 ;

    /* notify application of HDMI Rx Status Change event */
    NEXUS_TaskCallback_Fire(hdmiOutput->rxStatusCallback);

#endif
}


static void NEXUS_HdmiOutput_P_AvRateChangeCallback(void *pContext)
{
    NEXUS_HdmiOutputHandle hdmiOutput = (NEXUS_HdmiOutputHandle) pContext ;
    BHDM_Settings hdmiSettings ;

    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);

    BHDM_GetHdmiSettings(hdmiOutput->hdmHandle, &hdmiSettings) ;
	hdmiSettings.bForceEnableDisplay = true ;
    BHDM_EnableDisplay(hdmiOutput->hdmHandle, &hdmiSettings) ;
}


static void NEXUS_HdmiOutput_P_HotplugCallback(void *pContext)
{
    NEXUS_HdmiOutputHandle output = (NEXUS_HdmiOutputHandle)pContext;
    bool local_forceDisconnect ;
    NEXUS_HdmiOutputState local_lastHotplugState ;
    uint8_t deviceAttached;
    BERR_Code errCode ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);

    BKNI_EnterCriticalSection();
        local_forceDisconnect = output->forceDisconnect_isr ;   /* force removal of HDMI */
        local_lastHotplugState = output->lastHotplugState_isr;  /* last isr state: removed, connected, powered down  */
        output->forceDisconnect_isr = false ;
    BKNI_LeaveCriticalSection();

    BDBG_MSG(("forceDisconnect_isr %s", local_forceDisconnect ? "Yes" : "No")) ;
    BDBG_MSG(("lastHotplugState_isr %d", local_lastHotplugState)) ;

    /* force a disconnect of HDMI device if cable is removed  */
    errCode = BHDM_RxDeviceAttached(output->hdmHandle, &deviceAttached);
    if (errCode) { BERR_TRACE(errCode) ; return ;}

    if ((local_forceDisconnect) && (!deviceAttached))
    {
        BDBG_MSG(("Hot Plug request to disable TMDS lines %d", local_lastHotplugState)) ;

        if (local_lastHotplugState == NEXUS_HdmiOutputState_eDisconnected)
            output->lastRxState = NEXUS_HdmiOutputState_eDisconnected ;
        else
            output->lastRxState = NEXUS_HdmiOutputState_ePoweredOn ;

        NEXUS_HdmiOutput_P_RxRemoved(output) ;
        return ;
    }

    /*
        DEVICE is CONNECTED
        After a HotPlug, Initialization (lastRxState) or Resume (lastRxState)
        schedule a connection event for later...
        Use the time to allow the Rx to settle and to ensure an accurate RxSense reading
      */
    if ((local_lastHotplugState == NEXUS_HdmiOutputState_eRxSenseCheck)
    || (output->lastRxState == NEXUS_HdmiOutputState_eMax))  /* force initial HP processing */
    {
        output->rxState = NEXUS_HdmiOutputState_eRxSenseCheck ;
        if (!output->connectTimer)
        {
            /*  turn on the TMDS Clock after the cable is connected
                  and wait for  Rx to be ready */
            BDBG_MSG(("Schedule RxSense check for %dms ",
                output->openSettings.maxRxSenseRetries
                * output->openSettings.rxSenseInterval)) ;

            if (!output->openSettings.manualTmdsControl)
            {
                NEXUS_HdmiOutput_P_SetTmdsSignalClock(output, true) ;
            }
            else
            {
                /* debug message for manual control by App */
                BDBG_MSG(("NEXUS_HdmiOutput_P_HotplugCallback: Expected TMDS ON")) ;
            }

            output->connectTimer = NEXUS_ScheduleTimer(output->openSettings.rxSenseInterval,
               NEXUS_HdmiOutput_P_HotplugTimerExpiration, output) ;

            output->checkRxSenseCount = 0  ;
        }
        else
        {
            /* report debug message to check if events are occurring consecutively */
            BDBG_WRN(("Connected event already pending...")) ;
        }

        /* EDID is not dependent on RxSense; it should be available to be read now */
        NEXUS_HdmiOutput_P_ReadEdid(output) ;

        /* if checking for RxSense after a hot plug etc,
            wait until *after* RxSense detection to notify the app of hp event */

        if (output->rxState != NEXUS_HdmiOutputState_eRxSenseCheck)
        {
            NEXUS_TaskCallback_Fire(output->hotplugCallback) ;
        }

        return ;
    }


    if (output->rxState == NEXUS_HdmiOutputState_ePoweredOn )
    {
        BDBG_MSG(("Already powered and connected... ")) ;
    }
    else if (output->rxState == NEXUS_HdmiOutputState_ePoweredDown )
    {
        /* Disconnected -> powered down - treat as no event */
        NEXUS_HdmiOutput_P_RxRemoved(output) ;
        BDBG_MSG(("Receiver powered down, no hotplug event."));
    }

    else if (output->rxState == NEXUS_HdmiOutputState_eDisconnected)
    {
        /* Disconnected -> powered down - treat as no event */
        NEXUS_HdmiOutput_P_RxRemoved(output) ;
        BDBG_MSG(("Receiver not connected..."));
    }

    else if (output->rxState == NEXUS_HdmiOutputState_eMax)
        /* Initialization State */
        /* do nothing */ ;
    else
    {
        /* should never get here */
        BDBG_WRN((" HdmiOutputState (%d) not handled...", output->rxState)) ;
        BERR_TRACE(NEXUS_UNKNOWN) ;
    }

}


static void NEXUS_HdmiOutput_P_HotPlug_isr(void *context, int param, void *data)
{
    NEXUS_HdmiOutputHandle hdmiOutput = context;
    bool deviceAttached =  * (bool *) data ;

    BSTD_UNUSED(param);
    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);

    if (deviceAttached) {
        hdmiOutput->lastHotplugState_isr = NEXUS_HdmiOutputState_eRxSenseCheck ;
    }
    else
    {
        hdmiOutput->lastHotplugState_isr = NEXUS_HdmiOutputState_eDisconnected ;
        hdmiOutput->forceDisconnect_isr = true ;
    }

    BKNI_SetEvent_isr(hdmiOutput->notifyHotplugEvent) ;

    BDBG_MSG(("HotPlug isr state: '%s'",
		deviceAttached ? "RxSense Check" : "Disconnected")) ;
}


NEXUS_HdmiOutputState NEXUS_HdmiOutput_P_GetState(NEXUS_HdmiOutputHandle output)
{
    BERR_Code errCode;
    uint8_t rxSense=0, deviceAttached=1;
    NEXUS_HdmiOutputState outputState ;

    errCode = BHDM_RxDeviceAttached(output->hdmHandle, &deviceAttached);
    BERR_TRACE(errCode) ;

    if (!deviceAttached) /* rx device is REMOVED */
    {
        output->lastRxState = output->rxState ;
        output->rxState = NEXUS_HdmiOutputState_eDisconnected ;
        outputState = NEXUS_HdmiOutputState_eDisconnected ;
        goto done ;
    }


    /* rx Device is CONNECTED  */

#if NEXUS_HAS_SECURITY
    /* as long as encryption is enabled; no need to check for receiver sense */
    if (NEXUS_HdmiOutput_P_GetCurrentHdcplibState(output) == BHDCPlib_State_eEncryptionEnabled)
    {
        outputState = NEXUS_HdmiOutputState_ePoweredOn ;
        goto done ;
    }
#endif

    errCode = BHDM_GetReceiverSense(output->hdmHandle, &rxSense);
    if (errCode != BERR_SUCCESS)
    {
        outputState = output->lastRxState ;
        BDBG_WRN(("Unable to get Receiver Sense")) ;
        BERR_TRACE(errCode) ;
        goto done ;
    }

    /* If in RxSense check state; we are waiting to determine if attached Rx is ON ;
              do not set State  to OFF until RxSense check has been completed
           */
    if (output->lastState == NEXUS_HdmiOutputState_eRxSenseCheck)
    {
        if (rxSense)
            outputState = NEXUS_HdmiOutputState_ePoweredOn ;
        else
            outputState = NEXUS_HdmiOutputState_eRxSenseCheck ;
    }

    else if (rxSense)
        outputState = NEXUS_HdmiOutputState_ePoweredOn ;
    else
        outputState = NEXUS_HdmiOutputState_ePoweredDown;

done :
    if (deviceAttached)
    {
        BDBG_MSG(("Attached Rx Powered? %s",
            outputState == NEXUS_HdmiOutputState_ePoweredOn ? "Yes" : "No")) ;
    }
    else
    {
        BDBG_MSG(("No Rx Device Attached")) ;
    }


    BDBG_MSG(("HDMI Output connected to Display? %s ",
        output->videoConnected  ? "Yes" : "No")) ;
#if 0
    output->rxState = outputState ;
#endif
    return outputState ;
}




static void NEXUS_HdmiOutput_P_RxSenseTimerExpiration(void *pContext)
{
    NEXUS_HdmiOutputHandle output = pContext;
    uint8_t receiverSense ;
    BERR_Code errCode ;

    BDBG_OBJECT_ASSERT(output, NEXUS_HdmiOutput);

    output->powerTimer = NULL ; /* clear timer used */

    errCode = BHDM_GetReceiverSense(output->hdmHandle, &receiverSense) ;
    if (receiverSense != output->lastReceiverSense)
    {
        BDBG_MSG(("rxState: %d;  RxSense change from %s to %s",
            output->rxState,
            output->lastReceiverSense ? "ON" : "OFF",
            receiverSense ? "ON" : "OFF")) ;

        output->lastReceiverSense = receiverSense ;

        output->rxState = receiverSense ?
              NEXUS_HdmiOutputState_ePoweredOn
            : NEXUS_HdmiOutputState_ePoweredDown ;

        if (receiverSense)
        {
            /* Rx detected to be ON...
                make sure all TMDS data lines are ENABLED if under nexus control */
            if (!output->openSettings.manualTmdsControl)
                NEXUS_HdmiOutput_P_SetTmdsSignalData(output, true) ;
            else
            {
                /* debug message for manual control by App */
                BDBG_MSG(("NEXUS_HdmiOutput_P_HotplugCallback: Expected TMDS ON")) ;
            }

            /* notify application of hotplug - only for status */
            NEXUS_TaskCallback_Fire(output->hotplugCallback);

            /* allow display to re-enable HDMI output */
            NEXUS_TaskCallback_Fire(output->notifyDisplay);
        }
        else
        {
            /* Rx detected to be OFF...
                make sure all TMDS data lines are DISABLED if under nexus control */
            if (!output->openSettings.manualTmdsControl)
                NEXUS_HdmiOutput_P_SetTmdsSignalData(output, false) ;
            else
            {
                /* debug message for manual control by App */
                BDBG_MSG(("NEXUS_HdmiOutput_P_HotplugCallback: Expected TMDS Data OFF")) ;
            }

#if NEXUS_HAS_SECURITY
            /* make sure current HDCP link is disabled */
            {
                BHDCPlib_State hdcpState;
                hdcpState = NEXUS_HdmiOutput_P_GetCurrentHdcplibState(output);
                if ((hdcpState != BHDCPlib_State_eUnauthenticated)
                &&  (hdcpState != BHDCPlib_State_eUnPowered))
                {
                    BDBG_LOG(("Disable HDCP Authentication: current state: %d", hdcpState)) ;
                    errCode = NEXUS_HdmiOutput_DisableHdcpAuthentication(output);
                    if (errCode) { errCode = BERR_TRACE(errCode) ; }
                }
            }
#endif

        }
    }


    /* Re-arm timer */
    output->powerTimer =
        NEXUS_ScheduleTimer(output->openSettings.powerPollingInterval,
            NEXUS_HdmiOutput_P_RxSenseTimerExpiration, pContext);
}


/* VDC -> HDM callback info for rate changes */
void NEXUS_HdmiOutput_VideoRateChange_isr(
    NEXUS_HdmiOutputHandle handle,
    BAVC_VdcDisplay_Info *pDisplayInfo
    )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    BDBG_ASSERT(NULL != pDisplayInfo);

    BDBG_MSG(("HDMI Video Rate Change"));
    BHDM_AudioVideoRateChangeCB_isr(handle->hdmHandle, BHDM_Callback_Type_eVideoChange, pDisplayInfo);
}

/* RAP -> HDM callback for sample rate changes */
void NEXUS_HdmiOutput_AudioSampleRateChange_isr(
    NEXUS_HdmiOutputHandle handle,
    BAVC_AudioSamplingRate sampleRate
    )
{
    BAVC_Audio_Info audioInfo;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    handle->sampleRate = sampleRate;
    audioInfo.eAudioSamplingRate = sampleRate;

    BDBG_MSG(("HDMI Audio Sample Rate Change %d", sampleRate));
    if ( handle->tmdsDataEnabled )
    {
        BHDM_AudioVideoRateChangeCB_isr(handle->hdmHandle, BHDM_Callback_Type_eAudioChange, &audioInfo);
    }
}

bool NEXUS_HdmiOutput_GetEotf_priv(
    NEXUS_HdmiOutputHandle hdmiOutput,
    NEXUS_VideoEotf *pEotf
)
{
    *pEotf = hdmiOutput->settings.dynamicRangeMasteringInfoFrame.eotf;

    return true;
}

/* Returns false if the format is not supported, true if it is */
bool NEXUS_HdmiOutput_GetColorimetry_priv(
    NEXUS_HdmiOutputHandle hdmiOutput,
    const NEXUS_HdmiOutputColorimetryParameters *parameters,
    BAVC_MatrixCoefficients *pColorimetry
)
{
    BERR_Code errCode;
    BHDM_TxSupport platformHdmiOutputSupport ;
    BFMT_VideoFmt magnumVideoFormat ;
    uint8_t supported;
    BHDM_EDID_ColorimetryParams edidParameters ;

    const BFMT_VideoInfo *pVideoFormatInfo ;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);

    if (hdmiOutput->invalidEdid)
    {
        if (!hdmiOutput->invalidEdidReported)
        {
            BDBG_WRN(("%s", NEXUS_HdmiOutput_P_InvalidEdid)) ;
            hdmiOutput->invalidEdidReported = true ;
        }
     }

    errCode = NEXUS_P_VideoFormat_ToMagnum_isrsafe(parameters->format, &magnumVideoFormat) ;
    if (errCode) {
        magnumVideoFormat = BFMT_VideoFmt_eNTSC; /* don't proceed with uninitialized value. */
    }
    pVideoFormatInfo = BFMT_GetVideoFormatInfoPtr(magnumVideoFormat) ;

    edidParameters.eVideoFmt = magnumVideoFormat ;
    edidParameters.xvYccEnabled = parameters->xvYccEnabled ;

    /* get the preferred colorimetry  */
    errCode = BHDM_EDID_GetPreferredColorimetry(hdmiOutput->hdmHandle,
        &edidParameters, pColorimetry) ;

    if ( errCode )
    {
        return false;
    }

    errCode = BHDM_GetTxSupportStatus(hdmiOutput->hdmHandle, &platformHdmiOutputSupport) ;
    if (errCode)
    {
        BDBG_ERR(("Unable to determine platform support")) ;
        return false ;
    }

    switch (hdmiOutput->settings.colorSpace)
    {
    case NEXUS_ColorSpace_eAuto :
        /* User override of default colorspace */
        /* do nothing */
        break ;

    case NEXUS_ColorSpace_eRgb :
    case NEXUS_ColorSpace_eYCbCr444 :
        /* all platforms support RGB 444 and YCbCr 444 output */
        /* select colorimetry based on format */
        break ;

    case NEXUS_ColorSpace_eYCbCr422 :
        if  (!platformHdmiOutputSupport.YCbCr422)
        {
            BDBG_WRN(("Platform does not support YCbCr 4:2:2")) ;
            return false ; /* unsupported */
        }
        break ;

    case NEXUS_ColorSpace_eYCbCr420 :
        if  (!platformHdmiOutputSupport.YCbCr420)
        {
            BDBG_WRN(("Platform does not support YCbCr 4:2:0")) ;
            return false ; /* unsupported */
        }
        break ;

    default :
        BDBG_WRN(("Unknown/Unsupported Color Space: %d", hdmiOutput->settings.colorSpace)) ;
        return false ;
        break ;
    }

    errCode = BHDM_EDID_VideoFmtSupported(hdmiOutput->hdmHandle, magnumVideoFormat, &supported) ;
    if ( errCode )
    {
        if (!hdmiOutput->invalidEdidReported)
        {
            BDBG_WRN(("Unable to determine if %s (%d) is supported",
                pVideoFormatInfo->pchFormatStr, magnumVideoFormat)) ;
            hdmiOutput->invalidEdidReported = true ;
        }
        return false;
    }

    BDBG_MSG(("Format (%d) %s  Preferred Colorimetry: %d  Supported: %s",
        magnumVideoFormat, pVideoFormatInfo->pchFormatStr, *pColorimetry,
        supported ? "Yes" : "No")) ;
    return supported;
}

/* Set the notifyAudioEvent */
void NEXUS_HdmiOutput_SetNotifyAudioEvent_priv(
    NEXUS_HdmiOutputHandle handle,
    BKNI_EventHandle notifyAudioEvent
    )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    NEXUS_ASSERT_MODULE();
    handle->notifyAudioEvent = notifyAudioEvent;
}

/* Connect video and set format parameters */
NEXUS_Error NEXUS_HdmiOutput_SetDisplayParams_priv(
    NEXUS_HdmiOutputHandle handle,
    BFMT_VideoFmt format,
    BAVC_MatrixCoefficients colorimetry,
    BFMT_AspectRatio aspectRatio,
    bool masterMode,
    const NEXUS_CallbackDesc *notifyDisplay
    )
{
    BERR_Code errCode;
    BAVC_HDMI_AviInfoFrame hdmAviInfoFrameSettings;
    uint8_t deviceAttached ;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    /* whether or not we can set HDMI Display Parameters,
        inform HdmiOutput module is now connected to the Nexus Display */
    handle->videoConnected = true;


    /* set the HDMI transfer mode accordingly */
    errCode = BHDM_SetHdmiDataTransferMode(handle->hdmHandle, masterMode);
    if (errCode) {
        return BERR_TRACE(errCode);
    }


    if (notifyDisplay)
        NEXUS_TaskCallback_Set(handle->notifyDisplay, notifyDisplay);

   /* if no device is attached, settings cannot be applied; simply return */
    errCode = BHDM_RxDeviceAttached(handle->hdmHandle, &deviceAttached) ;
    BERR_TRACE(errCode) ;

    if (!deviceAttached)
    {
        return BERR_SUCCESS ;
    }

    BHDM_GetHdmiSettings(handle->hdmHandle, &handle->hdmSettings);

    handle->hdmSettings.eOutputFormat = handle->edidHdmiDevice ?
        BHDM_OutputFormat_eHDMIMode : BHDM_OutputFormat_eDVIMode;
    handle->hdmSettings.eAspectRatio = aspectRatio;
    handle->hdmSettings.eInputVideoFmt = format;
    handle->hdmSettings.eColorimetry = colorimetry;

    /* Aspect ratio change only, simply update AVI info frame is enough
        no need to go through BHDM_EnableDisplay */
    if (handle->aspectRatioChangeOnly && handle->tmdsDataEnabled)
    {
        /* Save the new aspect ratio in HDMI settings */
        errCode = BHDM_SetHdmiSettings(handle->hdmHandle, &handle->hdmSettings);
        if (errCode) return BERR_TRACE(errCode);

        /* When updating AVI InfoFrame packets, the aspect ratio from hdmi settings is used,
            not the setting in AviInfoFrameSettings is used. */
        BHDM_GetAVIInfoFramePacket(handle->hdmHandle, &hdmAviInfoFrameSettings);
        BHDM_SetAVIInfoFramePacket(handle->hdmHandle, &hdmAviInfoFrameSettings);

        /* Reset aspectRatioChangeOnly flag after checking */
        handle->aspectRatioChangeOnly = false;
    }
    else
    {
        BDBG_MSG(("EnableDisplay: Format= %d  ColorDepth= %d, ColorSpace= %d  eColorMatrix: %d",
            handle->hdmSettings.eInputVideoFmt,
            handle->hdmSettings.stVideoSettings.eBitsPerPixel,
            handle->hdmSettings.stVideoSettings.eColorSpace,
            handle->hdmSettings.eColorimetry)) ;
        errCode = BHDM_EnableDisplay(handle->hdmHandle, &handle->hdmSettings);
        /* get the updated TMDS Signals */
        NEXUS_HdmiOutput_P_GetTmdsSignals (handle) ;

        if ( errCode )
        {
            return BERR_TRACE(errCode);
        }

        (void) NEXUS_HdmiOutput_SetAudioParams_priv(handle, handle->audioBits,
            handle->sampleRate, handle->audioFormat, handle->audioNumChannels);

        if ( !handle->powerTimer
        && handle->openSettings.powerPollingInterval )
        {
            handle->powerTimer =
                NEXUS_ScheduleTimer(handle->openSettings.powerPollingInterval,
                    NEXUS_HdmiOutput_P_RxSenseTimerExpiration, handle);

            if (!handle->powerTimer)
            {
                return BERR_TRACE(NEXUS_NOT_INITIALIZED) ;
            }
        }

    }

    return BERR_SUCCESS;
}

/* Disconnect video */
NEXUS_Error NEXUS_HdmiOutput_Disconnect_priv(
    NEXUS_HdmiOutputHandle handle
    )
{
    BERR_Code errCode = BERR_SUCCESS ;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    handle->videoConnected = false;
    handle->lastRxState = NEXUS_HdmiOutputState_eMax;
    NEXUS_TaskCallback_Set(handle->notifyDisplay, NULL);

    errCode = BHDM_DisableDisplay(handle->hdmHandle);
    if (errCode)
        BERR_TRACE(errCode) ;
    NEXUS_HdmiOutput_P_GetTmdsSignals(handle) ;

    if ( handle->powerTimer )
    {
        NEXUS_CancelTimer(handle->powerTimer);
        handle->powerTimer = NULL ;
    }

    return errCode ;
}

/* Audio can only start if video is connected */
bool NEXUS_HdmiOutput_VideoIsConnected_priv(
    NEXUS_HdmiOutputHandle handle
    )
{
    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    return handle->videoConnected;
}

/* Sanity checks for the audio module */
bool NEXUS_HdmiOutput_AudioFormatSupported_priv(
    NEXUS_HdmiOutputHandle handle,
    BAVC_AudioFormat format,
    unsigned numChannels    /* PCM only */
    )
{
    BERR_Code errCode;
    BHDM_EDID_AudioDescriptor audioFormats[BAVC_AudioFormat_eMaxCount] ;

    NEXUS_ASSERT_MODULE();
    errCode = BHDM_EDID_GetSupportedAudioFormats(handle->hdmHandle, audioFormats);
    if ( errCode )
    {
        return false;
    }

    if ( !audioFormats[format].Supported )
    {
        return false;
    }
    else if ( format == BAVC_AudioFormat_ePCM )
    {
        /* Hardware can only output stereo or 5.1 currently */
        switch ( numChannels )
        {
        case 2:
        case 6:
            if ( audioFormats[BAVC_AudioFormat_ePCM].AudioChannels < numChannels )
            {
                return false;
            }
            break;
        default:
            /* Not supported */
            return false;
        }
    }

    return true;
}

/* Set audio format information */
NEXUS_Error NEXUS_HdmiOutput_SetAudioParams_priv(
    NEXUS_HdmiOutputHandle handle,
    BAVC_AudioBits bitsPerSample,
    BAVC_AudioSamplingRate sampleRate,
    BAVC_AudioFormat format,
    unsigned numChannels    /* PCM only */
    )
{
    NEXUS_Error errCode;
    BAVC_HDMI_AudioInfoFrame audioInfoFrame;

    NEXUS_ASSERT_MODULE();

    handle->sampleRate = sampleRate;
    handle->audioBits = bitsPerSample;
    handle->audioFormat = format;
    handle->audioNumChannels = numChannels;

    BHDM_GetHdmiSettings(handle->hdmHandle, &handle->hdmSettings);

    if ( handle->hdmSettings.eOutputFormat != BHDM_OutputFormat_eHDMIMode )
    {
        BDBG_MSG(("Not an HDMI Device"));
        return BERR_SUCCESS;
    }

    /* Don't enable TMDS on audio changes, only set this if we actually have a change. */
    if ( handle->tmdsDataEnabled )
    {
        errCode = BHDM_GetAudioInfoFramePacket(handle->hdmHandle, &audioInfoFrame);
        if ( errCode )
        {
            return BERR_TRACE(errCode);
        }

        if ( format == BAVC_AudioFormat_ePCM )
        {
            /* PCM should specify the number of PCM channels */
            switch ( numChannels )
            {
            default:
                BDBG_WRN(("Invalid number of PCM channels.  Defaulting to 2"));
                /* coverity[unterminated_default] */
            case 2:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e2Channels;
                break;
            case 3:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e3Channels;
                break;
            case 4:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e4Channels;
                break;
            case 5:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e5Channels;
                break;
            case 6:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e6Channels;
                break;
            case 7:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e7Channels;
                break;
            case 8:
                audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_e8Channels;
                break;
            }
        }
        else
        {
            /* Compressed should always use refer to stream header */
            audioInfoFrame.ChannelCount = BAVC_HDMI_AudioInfoFrame_ChannelCount_eReferToStreamHeader;
        }

        if ( format == BAVC_AudioFormat_ePCM )
        {
            switch ( numChannels )
            {
            case 6:
                audioInfoFrame.SpeakerAllocation = BHDM_ChannelAllocation_e5_1;
                break;
            case 8:
                audioInfoFrame.SpeakerAllocation = BHDM_ChannelAllocation_e3_4_1;
                break;
            default:
                BDBG_WRN(("Unexpected PCM channel count %u", numChannels));
                /* Fall through */
            case 2:
                audioInfoFrame.SpeakerAllocation = BHDM_ChannelAllocation_eStereo;
                break;
            }
        }
        else
        {
            audioInfoFrame.SpeakerAllocation = BHDM_ChannelAllocation_eStereo;
        }
        audioInfoFrame.DownMixInhibit = 0;  /* default */
        audioInfoFrame.LevelShift = 0;  /* default */

        /* Set the audio infoframe with the new settings; update will occur in BHDM_EnableDisplay */
        BKNI_Memcpy(&handle->hdmSettings.stAudioInfoFrame, &audioInfoFrame,
            sizeof(BAVC_HDMI_AudioInfoFrame)) ;

        handle->hdmSettings.eAudioSamplingRate = sampleRate;
        handle->hdmSettings.eAudioBits = bitsPerSample;
        handle->hdmSettings.eAudioFormat = format ;

        /* update the HDMI output ;
        ** changes in SR will affect the Clock Recovery Packet
        */
        BDBG_MSG(("EnableDisplay:  Sample Rate %d", handle->hdmSettings.eAudioSamplingRate));
        errCode = BHDM_EnableDisplay(handle->hdmHandle, &handle->hdmSettings);

        NEXUS_HdmiOutput_P_GetTmdsSignals (handle) ;
        if ( errCode )
        {
            return BERR_TRACE(errCode);
        }

        if ( !handle->powerTimer
        && handle->openSettings.powerPollingInterval )
        {
            handle->powerTimer =
                NEXUS_ScheduleTimer(handle->openSettings.powerPollingInterval,
                    NEXUS_HdmiOutput_P_RxSenseTimerExpiration, handle);

            if (!handle->powerTimer)
            {
                return BERR_TRACE(NEXUS_NOT_INITIALIZED) ;
            }
        }
    }
    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_HdmiOutput_P_Shutdown(void)
{
    int i;

    for ( i = 0; i < NEXUS_NUM_HDMI_OUTPUTS; i++ )
    {
        if ( g_hdmiOutputs[i].opened )
        {
            BDBG_ERR(("Force closing HDMI output %p", (void *)&g_hdmiOutputs[i]));
            NEXUS_HdmiOutput_Close(&g_hdmiOutputs[i]);
        }
    }
    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_HdmiOutput_SetAVMute(NEXUS_HdmiOutputHandle hdmiOutput, bool mute)
{
    BERR_Code errCode;

    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(hdmiOutput);

    /* store users value */
    hdmiOutput->avMuteSetting = mute;

    mute = mute || hdmiOutput->formatChangeMute;

    /* AvMute is HDMI concept only; no meaning in DVI mode */
    /* check for DVI and use black screen?? vs HDMI (AvMute) */
    errCode = BHDM_SetAvMute(hdmiOutput->hdmHandle, mute) ;
    if (errCode) return BERR_TRACE(errCode);

    return NEXUS_SUCCESS;
}


/* use NEXUS_HdmiOutout_GetVideoFormatSupport vs NEXUS_HdmiOutput_VideoFormatSupported */
/* for additional support information */
bool NEXUS_HdmiOutput_VideoFormatSupported(NEXUS_HdmiOutputHandle handle, NEXUS_VideoFormat format)
{
    BERR_Code errCode;
    BFMT_VideoFmt videoFmt;
    uint8_t supported;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);

    errCode = NEXUS_P_VideoFormat_ToMagnum_isrsafe(format, &videoFmt);
    if (errCode)
    {
        supported = false;
        errCode = BERR_TRACE(errCode) ;
        goto done ;
    }

    errCode = BHDM_EDID_VideoFmtSupported(handle->hdmHandle, videoFmt, &supported);
    if (errCode)
    {
        supported = false;
        errCode = BERR_TRACE(errCode) ;
        goto done ;
    }

    done :
        return supported;
}

NEXUS_Error NEXUS_HdmiOutput_P_PreFormatChange_priv(NEXUS_HdmiOutputHandle hdmiOutput, bool aspectRatioChangeOnly)
{
    NEXUS_Error rc;
    NEXUS_HdmiOutputState state;
    BHDM_Status hdmiStatus;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);

    hdmiOutput->aspectRatioChangeOnly = aspectRatioChangeOnly;
    if (aspectRatioChangeOnly)
        goto done;

    /* Check for device */
    state = NEXUS_HdmiOutput_P_GetState(hdmiOutput);

    BDBG_MSG(("PreFormatChange hdcp? %c; current state: %s",
        hdmiOutput->hdcpStarted ? 'y' : 'n',
        state == NEXUS_HdmiOutputState_ePoweredOn
            ? "powered" : "disconnect/unpowered"));

    /* if device is POWERED and HDCP is currently ENABLED */
    if ((state == NEXUS_HdmiOutputState_ePoweredOn)
    && hdmiOutput->hdcpStarted)
    {
        hdmiOutput->hdcpRequiredPostFormatChange = true ;

        rc = NEXUS_HdmiOutput_DisableHdcpEncryption(hdmiOutput);
        if (rc) return BERR_TRACE(rc);

        rc = BHDM_GetHdmiStatus(hdmiOutput->hdmHandle, &hdmiStatus);
        if (rc) return BERR_TRACE(rc);
        /* remember the pixel rate to determine if re-authentication or enable encryption is required */
        hdmiOutput->pixelClkRatePreFormatChange = hdmiStatus.pixelClockRate;

        BDBG_MSG(("%s pixelClkRate = %d",
            __FUNCTION__, hdmiOutput->pixelClkRatePreFormatChange)) ;
    }

    hdmiOutput->formatChangeMute = true;

    /* send AvMute to HDMI Receiver to minimize video flash/audio pops, etc.
       due to format change, color space, aspect ratio changes, etc.
       mute is only required if hdmiOutput is connected to the display */
    if (hdmiOutput->videoConnected)
    {
        rc = NEXUS_HdmiOutput_SetAVMute(hdmiOutput, hdmiOutput->avMuteSetting);
        if (rc) return BERR_TRACE(rc);

        /* Give receiver time to process the AvMute packet  */
        BKNI_Sleep(hdmiOutput->settings.preFormatChangeAvMuteDelay);
    }

done:
    return 0;
}


NEXUS_Error NEXUS_HdmiOutput_P_PostFormatChange_priv(NEXUS_HdmiOutputHandle hdmiOutput)
{
    NEXUS_Error rc = NEXUS_SUCCESS ;
    BHDM_Status hdmiStatus;

    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(hdmiOutput, NEXUS_HdmiOutput);
    BDBG_MSG(("PostFormatChange"));

    /* Give receiver time to finish processing format change before unmuting */
    BKNI_Sleep(hdmiOutput->settings.postFormatChangeAvMuteDelay);

    hdmiOutput->formatChangeMute = false;

    /* Now unmute */
    rc = NEXUS_HdmiOutput_SetAVMute(hdmiOutput, hdmiOutput->avMuteSetting);
    if (rc) {rc = BERR_TRACE(rc); goto done ;}

    if (hdmiOutput->hdcpRequiredPostFormatChange)
    {
        hdmiOutput->hdcpRequiredPostFormatChange = false ;

        BDBG_MSG(("PostFormatChange HDCP Restart required")) ;

        rc = BHDM_GetHdmiStatus(hdmiOutput->hdmHandle, &hdmiStatus);
        if (rc) {rc = BERR_TRACE(rc); goto done ;}

        BDBG_MSG(("%s pixelClkRate = %d", __FUNCTION__, hdmiStatus.pixelClockRate));
        /* Restart Hdcp Authentication when switching from/to a high clock rate format (297Mhz and up) */

        if ((hdmiOutput->pixelClkRatePreFormatChange != hdmiStatus.pixelClockRate)
        || (hdmiOutput->pixelClkRatePreFormatChange >= NEXUS_HDMI_OUTPUT_4K_PIXEL_CLOCK_RATE)
        || (hdmiStatus.pixelClockRate >= NEXUS_HDMI_OUTPUT_4K_PIXEL_CLOCK_RATE))
        {
            BDBG_WRN(("%s: Restart HDCP authentication after format change", __FUNCTION__));
            rc = NEXUS_HdmiOutput_StartHdcpAuthentication(hdmiOutput);
        }
        else {
            rc = NEXUS_HdmiOutput_EnableHdcpEncryption(hdmiOutput);
        }
        if (rc) {rc = BERR_TRACE(rc); goto done ;}

    }

done :
    return rc ;
}


NEXUS_Error NEXUS_HdmiOutput_GetDisplaySettings_priv(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiOutputDisplaySettings *stHdmiOutputDisplaySettings
)
{
#if NEXUS_HAS_HDMI_1_3
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    BKNI_Memset(stHdmiOutputDisplaySettings, 0, sizeof(NEXUS_HdmiOutputDisplaySettings)) ;
    stHdmiOutputDisplaySettings->colorDepth = handle->settings.colorDepth ;
    stHdmiOutputDisplaySettings->colorSpace = handle->settings.colorSpace ;
    stHdmiOutputDisplaySettings->eColorimetry = handle->settings.matrixCoefficients ;
    stHdmiOutputDisplaySettings->overrideMatrixCoefficients = handle->settings.overrideMatrixCoefficients ;
    stHdmiOutputDisplaySettings->colorRange = handle->settings.colorRange ;
    stHdmiOutputDisplaySettings->overrideColorRange = handle->settings.overrideColorRange ;
#else
    BSTD_UNUSED(handle);
    BSTD_UNUSED(stHdmiOutputDisplaySettings) ;
#endif

    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_HdmiOutput_SetDisplaySettings_priv(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiOutputDisplaySettings *pstDisplaySettings
)
{
    BHDM_Video_Settings stVideoSettings;
    BERR_Code rc ;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    BDBG_MSG(("SetDisplaySettings ")) ;
    BDBG_MSG(("  Color> Space: %d  Range: %d Override (%s) Depth: %d Colorimetry: %d Override (%s)",
		pstDisplaySettings->colorSpace,
		pstDisplaySettings->colorRange, pstDisplaySettings->overrideColorRange ? "Yes" : "No",
		pstDisplaySettings->colorDepth,
		pstDisplaySettings->eColorimetry, pstDisplaySettings->overrideMatrixCoefficients ? "Yes" : "No")) ;

    handle->settings.colorSpace = pstDisplaySettings->colorSpace ;

#if NEXUS_HAS_HDMI_1_3
    handle->settings.colorDepth = pstDisplaySettings->colorDepth ;

    handle->settings.matrixCoefficients = pstDisplaySettings->eColorimetry ;
    handle->settings.overrideMatrixCoefficients = pstDisplaySettings->overrideMatrixCoefficients ;

    handle->settings.colorRange = pstDisplaySettings->colorRange ;
    handle->settings.overrideColorRange = pstDisplaySettings->overrideColorRange ;
#endif

    rc = BHDM_GetVideoSettings(handle->hdmHandle, &stVideoSettings) ;
    if (rc) return BERR_TRACE(rc);

        stVideoSettings.eColorSpace =
        NEXUS_P_ColorSpace_ToMagnum_isrsafe(pstDisplaySettings->colorSpace) ;

        #if NEXUS_HAS_HDMI_1_3
        stVideoSettings.eBitsPerPixel = NEXUS_P_HdmiColorDepth_ToMagnum_isrsafe(pstDisplaySettings->colorDepth) ;
        stVideoSettings.eColorRange = NEXUS_P_ColorRange_ToMagnum_isrsafe(pstDisplaySettings->colorRange) ;
        #endif

    rc = BHDM_SetVideoSettings(handle->hdmHandle, &stVideoSettings) ;
    if (rc) return BERR_TRACE(rc);

    return NEXUS_SUCCESS;
}


void NEXUS_HdmiOutput_ReadFormatChangeStatus_priv(NEXUS_HdmiOutputHandle handle, bool *pFormatChangeUpdate)
{
    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    *pFormatChangeUpdate = handle->formatChangeUpdate;

    /* clear status */
    handle->formatChangeUpdate = false;
    return;
}


void NEXUS_HdmiOutput_GetVendorSpecificInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiVendorSpecificInfoFrame *pVendorSpecificInfoFrame)
{
    BAVC_HDMI_VendorSpecificInfoFrame avcInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    BDBG_CASSERT(NEXUS_HdmiVendorSpecificInfoFrame_HDMIVideoFormat_eMax == (NEXUS_HdmiVendorSpecificInfoFrame_HDMIVideoFormat)BAVC_HDMI_VSInfoFrame_HDMIVideoFormat_eMax);
    BDBG_CASSERT(NEXUS_HdmiVendorSpecificInfoFrame_HDMIVIC_eMax == (NEXUS_HdmiVendorSpecificInfoFrame_HDMIVIC)BAVC_HDMI_VSInfoFrame_HDMIVIC_eMax);
    BDBG_CASSERT(NEXUS_HdmiVendorSpecificInfoFrame_3DStructure_eMax == (NEXUS_Video3DStructure)BAVC_HDMI_VSInfoFrame_3DStructure_eMax);
    BDBG_CASSERT(NEXUS_HdmiVendorSpecificInfoFrame_3DExtData_eMax == (NEXUS_Video3DSubSample)BAVC_HDMI_VSInfoFrame_3DExtData_eMax);

    BHDM_GetVendorSpecificInfoFrame(handle->hdmHandle, &avcInfoFrame);

    BKNI_Memset(pVendorSpecificInfoFrame, 0, sizeof(*pVendorSpecificInfoFrame)); /* memset makes it future proof */

    pVendorSpecificInfoFrame->ieeeRegId[0] = avcInfoFrame.uIEEE_RegId[0];
    pVendorSpecificInfoFrame->ieeeRegId[1] = avcInfoFrame.uIEEE_RegId[1];
    pVendorSpecificInfoFrame->ieeeRegId[2] = avcInfoFrame.uIEEE_RegId[2];
    pVendorSpecificInfoFrame->hdmiVideoFormat =
            (NEXUS_HdmiVendorSpecificInfoFrame_HDMIVideoFormat) avcInfoFrame.eHdmiVideoFormat;
    pVendorSpecificInfoFrame->hdmiVIC =
            (NEXUS_HdmiVendorSpecificInfoFrame_HDMIVIC) avcInfoFrame.eHdmiVic;
    pVendorSpecificInfoFrame->hdmi3DStructure =
            (NEXUS_HdmiVendorSpecificInfoFrame_3DStructure) avcInfoFrame.e3DStructure;
    pVendorSpecificInfoFrame->hdmi3DExtData =
            (NEXUS_HdmiVendorSpecificInfoFrame_3DExtData) avcInfoFrame.e3DExtData;

    return  ;
}


NEXUS_Error NEXUS_HdmiOutput_SetVendorSpecificInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    const NEXUS_HdmiVendorSpecificInfoFrame *pVendorSpecificInfoFrame
    )
{
    BERR_Code rc = BERR_SUCCESS ;
    BAVC_HDMI_VendorSpecificInfoFrame avcInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (IS_ALIAS(handle)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);

    /* payload length must not include the IEEE Reg ID or checksum byte  */
    /* i.e. payload length < BAVC_HDMI_PB_LENGTH - 3 byte IEEE RegID - 1 byte checksum */

    avcInfoFrame.uIEEE_RegId[0] = pVendorSpecificInfoFrame->ieeeRegId[0];
    avcInfoFrame.uIEEE_RegId[1] = pVendorSpecificInfoFrame->ieeeRegId[1];
    avcInfoFrame.uIEEE_RegId[2] = pVendorSpecificInfoFrame->ieeeRegId[2];
    avcInfoFrame.eHdmiVideoFormat =
        (BAVC_HDMI_VSInfoFrame_HDMIVideoFormat) pVendorSpecificInfoFrame->hdmiVideoFormat;
    avcInfoFrame.eHdmiVic =
        (BAVC_HDMI_VSInfoFrame_HDMIVIC) pVendorSpecificInfoFrame->hdmiVIC;
    avcInfoFrame.e3DStructure =
        (BAVC_HDMI_VSInfoFrame_3DStructure) pVendorSpecificInfoFrame->hdmi3DStructure;
    avcInfoFrame.e3DExtData =
        (BAVC_HDMI_VSInfoFrame_3DExtData) pVendorSpecificInfoFrame->hdmi3DExtData;

    rc = BHDM_SetVendorSpecificInfoFrame(handle->hdmHandle, &avcInfoFrame);
    if (rc) return BERR_TRACE(rc);

    return NEXUS_SUCCESS ;
}


NEXUS_Error NEXUS_HdmiOutput_GetAviInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiAviInfoFrame *pAviInfoFrame
)
{
    BERR_Code rc = BERR_SUCCESS;
    BAVC_HDMI_AviInfoFrame avcAviInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Colorspace) == sizeof(BAVC_HDMI_AviInfoFrame_Colorspace));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ActiveInfo) == sizeof(BAVC_HDMI_AviInfoFrame_ActiveInfo));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_BarInfo) == sizeof(BAVC_HDMI_AviInfoFrame_BarInfo));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Colorimetry) == sizeof(BAVC_HDMI_AviInfoFrame_Colorimetry));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_PictureAspectRatio) == sizeof(BAVC_HDMI_AviInfoFrame_PictureAspectRatio));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ActiveFormatAspectRatio) == sizeof(BAVC_HDMI_AviInfoFrame_ActiveFormatAspectRatio));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Scaling) == sizeof(BAVC_HDMI_AviInfoFrame_Scaling));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ITContent) == sizeof(BAVC_HDMI_AviInfoFrame_ITContent));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ExtendedColorimetry) == sizeof(BAVC_HDMI_AviInfoFrame_ExtendedColorimetry));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_RGBQuantizationRange) == sizeof(BAVC_HDMI_AviInfoFrame_RGBQuantizationRange));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ContentType) == sizeof(BAVC_HDMI_AviInfoFrame_ContentType));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_YccQuantizationRange) == sizeof(BAVC_HDMI_AviInfoFrame_YccQuantizationRange));

    rc = BHDM_GetAVIInfoFramePacket(handle->hdmHandle, &avcAviInfoFrame);

    pAviInfoFrame->bOverrideDefaults = avcAviInfoFrame.bOverrideDefaults;

    pAviInfoFrame->eColorSpace =
        (NEXUS_HdmiAviInfoFrame_Colorspace) avcAviInfoFrame.ePixelEncoding;
    pAviInfoFrame->eActiveInfo =
        (NEXUS_HdmiAviInfoFrame_ActiveInfo) avcAviInfoFrame.eActiveInfo;
    pAviInfoFrame->eBarInfo =
        (NEXUS_HdmiAviInfoFrame_BarInfo) avcAviInfoFrame.eBarInfo;
    pAviInfoFrame->eScanInfo =
        (NEXUS_HdmiAviInfoFrame_ScanInfo) avcAviInfoFrame.eScanInfo;
    pAviInfoFrame->eColorimetry=
        (NEXUS_HdmiAviInfoFrame_Colorimetry) avcAviInfoFrame.eColorimetry;

    pAviInfoFrame->ePictureAspectRatio =
        (NEXUS_HdmiAviInfoFrame_PictureAspectRatio) avcAviInfoFrame.ePictureAspectRatio;
    pAviInfoFrame->eActiveFormatAspectRatio=
        (NEXUS_HdmiAviInfoFrame_ActiveFormatAspectRatio) avcAviInfoFrame.eActiveFormatAspectRatio;
    pAviInfoFrame->eScaling=
        (NEXUS_HdmiAviInfoFrame_Scaling) avcAviInfoFrame.eScaling;

    pAviInfoFrame->videoIdCode = avcAviInfoFrame.VideoIdCode;
    pAviInfoFrame->pixelRepeat = avcAviInfoFrame.PixelRepeat;

#if NEXUS_HAS_HDMI_1_3
    pAviInfoFrame->eITContent=
        (NEXUS_HdmiAviInfoFrame_ITContent) avcAviInfoFrame.eITContent;
    pAviInfoFrame->eExtendedColorimetry=
        (NEXUS_HdmiAviInfoFrame_ExtendedColorimetry) avcAviInfoFrame.eExtendedColorimetry;
    pAviInfoFrame->eRGBQuantizationRange=
        (NEXUS_HdmiAviInfoFrame_RGBQuantizationRange) avcAviInfoFrame.eRGBQuantizationRange;
    pAviInfoFrame->eContentType =
        (NEXUS_HdmiAviInfoFrame_ContentType) avcAviInfoFrame.eContentType;
    pAviInfoFrame->eYccQuantizationRange =
        (NEXUS_HdmiAviInfoFrame_YccQuantizationRange) avcAviInfoFrame.eYccQuantizationRange;
#endif

    pAviInfoFrame->topBarEndLineNumber = avcAviInfoFrame.TopBarEndLineNumber;
    pAviInfoFrame->bottomBarStartLineNumber = avcAviInfoFrame.BottomBarStartLineNumber;
    pAviInfoFrame->leftBarEndPixelNumber = avcAviInfoFrame.LeftBarEndPixelNumber;
    pAviInfoFrame->rightBarEndPixelNumber = avcAviInfoFrame.RightBarEndPixelNumber;

    return NEXUS_SUCCESS;
}


NEXUS_Error NEXUS_HdmiOutput_SetAviInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    const NEXUS_HdmiAviInfoFrame *pAviInfoFrame
    )
{
    BERR_Code rc = BERR_SUCCESS;
    BAVC_HDMI_AviInfoFrame avcAviInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (IS_ALIAS(handle)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Colorspace) == sizeof(BAVC_HDMI_AviInfoFrame_Colorspace));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ActiveInfo) == sizeof(BAVC_HDMI_AviInfoFrame_ActiveInfo));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_BarInfo) == sizeof(BAVC_HDMI_AviInfoFrame_BarInfo));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Colorimetry) == sizeof(BAVC_HDMI_AviInfoFrame_Colorimetry));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_PictureAspectRatio) == sizeof(BAVC_HDMI_AviInfoFrame_PictureAspectRatio));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ActiveFormatAspectRatio) == sizeof(BAVC_HDMI_AviInfoFrame_ActiveFormatAspectRatio));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_Scaling) == sizeof(BAVC_HDMI_AviInfoFrame_Scaling));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ITContent) == sizeof(BAVC_HDMI_AviInfoFrame_ITContent));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ExtendedColorimetry) == sizeof(BAVC_HDMI_AviInfoFrame_ExtendedColorimetry));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_RGBQuantizationRange) == sizeof(BAVC_HDMI_AviInfoFrame_RGBQuantizationRange));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_ContentType) == sizeof(BAVC_HDMI_AviInfoFrame_ContentType));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAviInfoFrame_YccQuantizationRange) == sizeof(BAVC_HDMI_AviInfoFrame_YccQuantizationRange));


    /* If set, all settings are to be used in generating AVI InfoFrame. Else, some of the fields will be
        automatically generated instead (e.g. color space, colorimetry, etc.) */
    avcAviInfoFrame.bOverrideDefaults = pAviInfoFrame->bOverrideDefaults;

    avcAviInfoFrame.ePixelEncoding =
        (BAVC_HDMI_AviInfoFrame_Colorspace) pAviInfoFrame->eColorSpace;
    avcAviInfoFrame.eActiveInfo =
        (BAVC_HDMI_AviInfoFrame_ActiveInfo) pAviInfoFrame->eActiveInfo;
    avcAviInfoFrame.eBarInfo =
        (BAVC_HDMI_AviInfoFrame_BarInfo) pAviInfoFrame->eBarInfo;
    avcAviInfoFrame.eScanInfo =
        (BAVC_HDMI_AviInfoFrame_ScanInfo) pAviInfoFrame->eScanInfo;
    avcAviInfoFrame.eColorimetry =
        (BAVC_HDMI_AviInfoFrame_Colorimetry) pAviInfoFrame->eColorimetry;

    avcAviInfoFrame.ePictureAspectRatio =
        (BAVC_HDMI_AviInfoFrame_PictureAspectRatio) pAviInfoFrame->ePictureAspectRatio;
    avcAviInfoFrame.eActiveFormatAspectRatio =
        (BAVC_HDMI_AviInfoFrame_ActiveFormatAspectRatio) pAviInfoFrame->eActiveFormatAspectRatio;
    avcAviInfoFrame.eScaling =
        (BAVC_HDMI_AviInfoFrame_Scaling) pAviInfoFrame->eScaling;

    avcAviInfoFrame.VideoIdCode = pAviInfoFrame->videoIdCode;
    avcAviInfoFrame.PixelRepeat = pAviInfoFrame->pixelRepeat;

#if NEXUS_HAS_HDMI_1_3
    avcAviInfoFrame.eITContent =
        (BAVC_HDMI_AviInfoFrame_ITContent) pAviInfoFrame->eITContent;
    avcAviInfoFrame.eExtendedColorimetry =
        (BAVC_HDMI_AviInfoFrame_ExtendedColorimetry) pAviInfoFrame->eExtendedColorimetry;
    avcAviInfoFrame.eRGBQuantizationRange =
        (BAVC_HDMI_AviInfoFrame_RGBQuantizationRange) pAviInfoFrame->eRGBQuantizationRange;
    avcAviInfoFrame.eContentType =
        (BAVC_HDMI_AviInfoFrame_ContentType) pAviInfoFrame->eContentType;
    avcAviInfoFrame.eYccQuantizationRange =
        (BAVC_HDMI_AviInfoFrame_YccQuantizationRange) pAviInfoFrame->eYccQuantizationRange;
#endif

    avcAviInfoFrame.TopBarEndLineNumber = pAviInfoFrame->topBarEndLineNumber;
    avcAviInfoFrame.BottomBarStartLineNumber = pAviInfoFrame->bottomBarStartLineNumber;
    avcAviInfoFrame.LeftBarEndPixelNumber = pAviInfoFrame->leftBarEndPixelNumber;
    avcAviInfoFrame.RightBarEndPixelNumber = pAviInfoFrame->rightBarEndPixelNumber;

    rc = BHDM_SetAVIInfoFramePacket(handle->hdmHandle, &avcAviInfoFrame);
    if (rc) return BERR_TRACE(rc);

    return NEXUS_SUCCESS;
}


NEXUS_Error NEXUS_HdmiOutput_GetAudioInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiAudioInfoFrame *pAudioInfoFrame
)
{
    BERR_Code rc = BERR_SUCCESS;
    BAVC_HDMI_AudioInfoFrame avcAudioInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_ChannelCount) == sizeof(BAVC_HDMI_AudioInfoFrame_ChannelCount));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_CodingType) == sizeof(BAVC_HDMI_AudioInfoFrame_CodingType));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_SampleSize) == sizeof(BAVC_HDMI_AudioInfoFrame_SampleSize));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_SampleFrequency) == sizeof(BAVC_HDMI_AudioInfoFrame_SampleFrequency));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_DownMixInhibit) == sizeof(BAVC_HDMI_AudioInfoFrame_DownMixInhibit));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_LevelShift) == sizeof(BAVC_HDMI_AudioInfoFrame_LevelShift));

    rc = BHDM_GetAudioInfoFramePacket(handle->hdmHandle, &avcAudioInfoFrame);
    if (rc) return BERR_TRACE(rc);

    pAudioInfoFrame->bOverrideDefaults = avcAudioInfoFrame.bOverrideDefaults;
    pAudioInfoFrame->eChannelCount =
        (NEXUS_HdmiAudioInfoFrame_ChannelCount) avcAudioInfoFrame.ChannelCount;
    pAudioInfoFrame->eCodingType =
        (NEXUS_HdmiAudioInfoFrame_CodingType) avcAudioInfoFrame.CodingType;
    pAudioInfoFrame->eSampleSize =
        (NEXUS_HdmiAudioInfoFrame_SampleSize) avcAudioInfoFrame.SampleSize;
    pAudioInfoFrame->eSampleFrequency =
        (NEXUS_HdmiAudioInfoFrame_SampleFrequency) avcAudioInfoFrame.SampleFrequency;

    pAudioInfoFrame->speakerAllocation = avcAudioInfoFrame.SpeakerAllocation;
    pAudioInfoFrame->eDownMixInhibit =
        (NEXUS_HdmiAudioInfoFrame_DownMixInhibit) avcAudioInfoFrame.DownMixInhibit;
    pAudioInfoFrame->eLevelShift=
        (NEXUS_HdmiAudioInfoFrame_LevelShift) avcAudioInfoFrame.LevelShift;

    return NEXUS_SUCCESS;
}


NEXUS_Error NEXUS_HdmiOutput_SetAudioInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    const NEXUS_HdmiAudioInfoFrame *pAudioInfoFrame
)
{
    BERR_Code rc = BERR_SUCCESS;
    BAVC_HDMI_AudioInfoFrame avcAudioInfoFrame;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (IS_ALIAS(handle)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_ChannelCount) == sizeof(BAVC_HDMI_AudioInfoFrame_ChannelCount));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_CodingType) == sizeof(BAVC_HDMI_AudioInfoFrame_CodingType));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_SampleSize) == sizeof(BAVC_HDMI_AudioInfoFrame_SampleSize));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_SampleFrequency) == sizeof(BAVC_HDMI_AudioInfoFrame_SampleFrequency));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_DownMixInhibit) == sizeof(BAVC_HDMI_AudioInfoFrame_DownMixInhibit));
    BDBG_CASSERT(sizeof(NEXUS_HdmiAudioInfoFrame_LevelShift) == sizeof(BAVC_HDMI_AudioInfoFrame_LevelShift));


    avcAudioInfoFrame.bOverrideDefaults = pAudioInfoFrame->bOverrideDefaults;
    avcAudioInfoFrame.ChannelCount =
        (BAVC_HDMI_AudioInfoFrame_ChannelCount) pAudioInfoFrame->eChannelCount;
    avcAudioInfoFrame.CodingType =
        (BAVC_HDMI_AudioInfoFrame_CodingType) pAudioInfoFrame->eCodingType;
    avcAudioInfoFrame.SampleSize =
        (BAVC_HDMI_AudioInfoFrame_SampleSize) pAudioInfoFrame->eSampleSize;
    avcAudioInfoFrame.SampleFrequency =
        (BAVC_HDMI_AudioInfoFrame_SampleFrequency) pAudioInfoFrame->eSampleFrequency;

    avcAudioInfoFrame.SpeakerAllocation = pAudioInfoFrame->speakerAllocation;
    avcAudioInfoFrame.DownMixInhibit=
        (BAVC_HDMI_AudioInfoFrame_DownMixInhibit) pAudioInfoFrame->eDownMixInhibit;
    avcAudioInfoFrame.LevelShift=
        (BAVC_HDMI_AudioInfoFrame_LevelShift) pAudioInfoFrame->eLevelShift;

    rc = BHDM_SetAudioInfoFramePacket(handle->hdmHandle, &avcAudioInfoFrame);
    if (rc) return BERR_TRACE(rc);

    return NEXUS_SUCCESS;
}

NEXUS_Error NEXUS_HdmiOutput_GetSpdInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiSpdInfoFrame *pSpdInfoFrame
)
{
    BERR_Code rc = BERR_SUCCESS ;
    BHDM_Settings hdmiSettings;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);

    BDBG_CASSERT(sizeof(NEXUS_HdmiSpdSourceDeviceType) == sizeof(BAVC_HDMI_SpdInfoFrame_SourceType));

    BHDM_GetHdmiSettings(handle->hdmHandle, &hdmiSettings);
    BKNI_Memcpy(pSpdInfoFrame->vendorName, hdmiSettings.SpdVendorName, NEXUS_HDMI_SPD_VENDOR_NAME_MAX);
    pSpdInfoFrame->vendorName[NEXUS_HDMI_SPD_VENDOR_NAME_MAX] = '\0';
    BKNI_Memcpy(pSpdInfoFrame->description, hdmiSettings.SpdDescription, NEXUS_HDMI_SPD_DESCRIPTION_MAX);
    pSpdInfoFrame->description[NEXUS_HDMI_SPD_DESCRIPTION_MAX] = '\0';
    pSpdInfoFrame->deviceType = hdmiSettings.eSpdSourceDevice;

    return rc;
}

NEXUS_Error NEXUS_HdmiOutput_SetSpdInfoFrame(
    NEXUS_HdmiOutputHandle handle,
    const NEXUS_HdmiSpdInfoFrame *pSpdInfoFrame
)
{
    BERR_Code rc = BERR_SUCCESS ;
    BHDM_Settings hdmiSettings;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (IS_ALIAS(handle)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);

    BDBG_CASSERT(sizeof(NEXUS_HdmiSpdSourceDeviceType) == sizeof(BAVC_HDMI_SpdInfoFrame_SourceType));

    BHDM_GetHdmiSettings(handle->hdmHandle, &hdmiSettings);
    BKNI_Memcpy(hdmiSettings.SpdVendorName, pSpdInfoFrame->vendorName, NEXUS_HDMI_SPD_VENDOR_NAME_MAX);
    BKNI_Memcpy(hdmiSettings.SpdDescription, pSpdInfoFrame->description, NEXUS_HDMI_SPD_DESCRIPTION_MAX);
    hdmiSettings.eSpdSourceDevice = pSpdInfoFrame->deviceType;
    rc = BHDM_SetHdmiSettings(handle->hdmHandle, &hdmiSettings);
    if (rc) return BERR_TRACE(rc);

    rc = BHDM_SetSPDInfoFramePacket(handle->hdmHandle);
    if (rc) return BERR_TRACE(rc);

    return rc;
}

NEXUS_Error NEXUS_HdmiOutput_P_SetTmdsSignalData(
    NEXUS_HdmiOutputHandle handle,
    bool tmdsDataEnable)
{
    BERR_Code rc = BERR_SUCCESS ;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    BDBG_MSG(("Set NEXUS_HdmiOutput_P_SetTmdsSignalData : %s",
        tmdsDataEnable ? "ON" : "OFF"));

    rc = BHDM_EnableTmdsData(handle->hdmHandle, tmdsDataEnable) ;
    if (rc) return BERR_TRACE(rc);

    /* get the updated TMDS Signals */
    NEXUS_HdmiOutput_P_GetTmdsSignals (handle) ;

    return NEXUS_SUCCESS ;
}


NEXUS_Error NEXUS_HdmiOutput_P_SetTmdsSignalClock(
    NEXUS_HdmiOutputHandle handle,
    bool tmdsClockEnable)
{
    BERR_Code rc = BERR_SUCCESS ;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);

    BDBG_MSG(("NEXUS_HdmiOutput_P_SetTmdsSignalClock : %s",
        tmdsClockEnable ? "ON" : "OFF"));

    rc = BHDM_EnableTmdsClock(handle->hdmHandle, tmdsClockEnable) ;
    if (rc) return BERR_TRACE(rc);

    /* for power management, the clock will be forced off if a device is not connected */
    /* get the updated TMDS Signals */
    NEXUS_HdmiOutput_P_GetTmdsSignals (handle) ;

    return NEXUS_SUCCESS ;
}


NEXUS_Error NEXUS_HdmiOutput_SetTmdsSignal(
    NEXUS_HdmiOutputHandle handle,
    const NEXUS_HdmiOutputTmdsSignalSettings *tmdsSettings)
 {
    BERR_Code rc = BERR_SUCCESS ;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (IS_ALIAS(handle)) return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    if (!handle->openSettings.manualTmdsControl)
    {
        BDBG_ERR(("App did not request control of TMDS lines; see openSettings.manualTmdsControl")) ;
        return NEXUS_NOT_INITIALIZED ;
    }

    rc = NEXUS_HdmiOutput_P_SetTmdsSignalData(handle, tmdsSettings->data) ;
    if (rc) return BERR_TRACE(rc);

    rc = NEXUS_HdmiOutput_P_SetTmdsSignalClock(handle, tmdsSettings->clock) ;
    if (rc) return BERR_TRACE(rc);

    return NEXUS_SUCCESS ;
}


void NEXUS_HdmiOutput_GetTmdsSignal(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiOutputTmdsSignalSettings *tmdsSettings)
{

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);
    tmdsSettings->data = handle->tmdsDataEnabled ;
    tmdsSettings->clock = handle->tmdsClockEnabled ;
    return;
}


static void NEXUS_HdmiOutput_P_GetTmdsSignals (NEXUS_HdmiOutputHandle hdmiOutput)
{
    BHDM_Status hdmiStatus ;

    BHDM_GetHdmiStatus(hdmiOutput->hdmHandle, &hdmiStatus) ;
    hdmiOutput->tmdsClockEnabled = hdmiStatus.tmds.clockEnabled ;
    hdmiOutput->tmdsDataEnabled = hdmiStatus.tmds.dataEnabled ;

    BDBG_MSG(("Retrieved TMDS Signals Clock: %d; Data: %d",
        hdmiOutput->tmdsClockEnabled, hdmiOutput->tmdsDataEnabled)) ;
}

NEXUS_Error NEXUS_HdmiOutput_GetSupportedVideoInfo(
    NEXUS_HdmiOutputHandle handle,
    NEXUS_HdmiOutputSupportedVideoInfo *pSupportedVideoInfo
)
{
    BERR_Code rc = BERR_SUCCESS;
    BHDM_EDID_VideoDescriptorInfo stHdmSupportedVideoInfo ;
    int i=0;

    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);
    BKNI_Memset(pSupportedVideoInfo, 0, sizeof(*pSupportedVideoInfo));

    rc = BHDM_EDID_GetSupportedVideoInfo(handle->hdmHandle, &stHdmSupportedVideoInfo);
    if (rc) return BERR_TRACE(rc);

    pSupportedVideoInfo->numSupportedVideoDescriptors = stHdmSupportedVideoInfo.numDescriptors;

    /* ensure we don't overrun the nexus struct */
#define MAX_NUM_SUPPORTED_VIDEO_DESC (sizeof(pSupportedVideoInfo->supportedVideoIDCode)/sizeof(pSupportedVideoInfo->supportedVideoIDCode[0]))

    if (pSupportedVideoInfo->numSupportedVideoDescriptors > MAX_NUM_SUPPORTED_VIDEO_DESC) {
        pSupportedVideoInfo->numSupportedVideoDescriptors = MAX_NUM_SUPPORTED_VIDEO_DESC;
    }

    for (i=0; i < pSupportedVideoInfo->numSupportedVideoDescriptors; i++)
    {
        pSupportedVideoInfo->supportedVideoIDCode[i] =
            stHdmSupportedVideoInfo.VideoIDCode[i] ;
    }

    return NEXUS_SUCCESS;
}


NEXUS_Error NEXUS_HdmiOutput_GetCrcData( NEXUS_HdmiOutputHandle handle, NEXUS_HdmiOutputCrcData *pData, unsigned numEntries, unsigned *pNumEntriesReturned )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    RESOLVE_ALIAS(handle);
    *pNumEntriesReturned = 0;
    if (pData == NULL) {
        BERR_TRACE(NEXUS_INVALID_PARAMETER);
        return 0;
    }
    /* no critical section needed for this type of producer/consumer */
    while (*pNumEntriesReturned < numEntries && handle->crc.wptr != handle->crc.rptr && handle->crc.queue) {
        pData[*pNumEntriesReturned] = handle->crc.queue[handle->crc.rptr];
        (*pNumEntriesReturned)++;
        if (++handle->crc.rptr == handle->crc.size) {
            handle->crc.rptr = 0;
        }
    }
    return 0;
}

void NEXUS_HdmiOutput_P_Vsync_isr( void *context )
{
    NEXUS_HdmiOutputHandle handle = context;
    BDBG_OBJECT_ASSERT(handle, NEXUS_HdmiOutput);
    if (handle->crc.queue) {
        NEXUS_HdmiOutputCrcData *pData;
        BERR_Code rc;
        BHDM_CrcData crc;

        rc = BHDM_GetCrcValue_isr(handle->hdmHandle, &crc);
        if (rc) return;

        pData = &handle->crc.queue[handle->crc.wptr];
        pData->crc = crc.crc;
        if (++handle->crc.wptr == handle->crc.size) {
            handle->crc.wptr = 0;
        }
        if (handle->crc.wptr == handle->crc.rptr) {
            BDBG_WRN(("HDMI Tx CRC overflow"));
        }
    }
}

#endif /* #if NEXUS_NUM_HDMI_OUTPUTS */
