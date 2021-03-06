/***************************************************************************
*     (c)2004-2014 Broadcom Corporation
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
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* API Description:
*   API name: Frontend
*    Generic Frontend APIs.
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/

#include "nexus_frontend_module.h"
#include "priv/nexus_frontend_mtsif_priv.h"
#include "priv/nexus_core.h"
#if NEXUS_HAS_MXT
#include "bmxt.h"
#endif

#include "nexus_frontend_device_impl.h"

BDBG_MODULE(nexus_frontend);

#define BDBG_MSG_TRACE(x) /*BDBG_MSG(x)*/

#define MAX_PB_PER_MTSIF_FRONTEND 4

struct NEXUS_FrontendDeviceList g_frontendDeviceList;

struct NEXUS_FrontendList g_frontendList;

struct NEXUS_TunerList g_tunerList;

/***************************************************************************
Summary:
    Return a list of capabilities for a given frontend object.
Description:
    This call returns a list of capabilities for a frontend object.  Because
    many frontends support multiple types of modulation schemes, this
    allows the application to distinguish the capabilities of one tuner
    versus another.  If a tuner has multiple capabilities, only one of
    the modes may be used at any one time.
See Also:
    NEXUS_Frontend_Get
 ***************************************************************************/
void NEXUS_Frontend_GetCapabilities(
    NEXUS_FrontendHandle handle,
    NEXUS_FrontendCapabilities *pCapabilities   /* [out] */
    )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != pCapabilities);

    *pCapabilities = handle->capabilities;
}

NEXUS_Error NEXUS_Frontend_GetInputBandStatus(NEXUS_FrontendHandle handle, NEXUS_InputBandStatus *pStatus)
{
    NEXUS_FrontendUserParameters userParams;
    NEXUS_FrontendDeviceHandle device;
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if (NEXUS_Frontend_P_CheckDeviceOpen(handle)) {
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    device = handle->pGenericDeviceHandle;
    NEXUS_Frontend_GetUserParameters(handle, &userParams);

#if NEXUS_HAS_MXT
{
    BMXT_InputBandStatus ibStatus;
    NEXUS_Error rc;
    rc = BMXT_GetInputBandStatus(device->mtsifConfig.mxt, &ibStatus);
    if (rc) {
        return BERR_TRACE(rc);
    }
    pStatus->syncCount = ibStatus.syncCount[userParams.param1];
}
    return NEXUS_SUCCESS;
#else
    BSTD_UNUSED(pStatus);
    return NEXUS_NOT_SUPPORTED;
#endif
}

NEXUS_Error NEXUS_Frontend_ReadSoftDecisions( NEXUS_FrontendHandle handle, NEXUS_FrontendSoftDecision *pDecisions, size_t length, size_t *pNumRead )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != handle->pDeviceHandle);
    BDBG_ASSERT(NULL != pDecisions);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->readSoftDecisions )
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_ReadSoftDecisions(handle->pParentFrontend, pDecisions, length, pNumRead);
        }
        else if (handle->getSoftDecisions)
        {
            /* impl Read with Get */
            int rc;
            rc = NEXUS_Frontend_GetSoftDecisions(handle, pDecisions, length);
            if (rc) return BERR_TRACE(rc);
            *pNumRead = length;
            return 0;
        }
        else
        {
            return BERR_TRACE(NEXUS_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->readSoftDecisions(handle->pDeviceHandle, pDecisions, length, pNumRead);
    }
}

NEXUS_Error NEXUS_Frontend_GetSoftDecisions( NEXUS_FrontendHandle handle, NEXUS_FrontendSoftDecision *pDecisions, size_t length )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != handle->pDeviceHandle);
    BDBG_ASSERT(NULL != pDecisions);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getSoftDecisions )
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_GetSoftDecisions(handle->pParentFrontend, pDecisions, length);
        }
        else if (handle->readSoftDecisions)
        {
            /* impl Get with Read */
            while (length) {
                size_t numRead;
                int rc;
                rc = NEXUS_Frontend_ReadSoftDecisions(handle, pDecisions, length, &numRead);
                if (rc) return BERR_TRACE(rc);
                length -= numRead;
            }
            return 0;
        }
        else
        {
            return BERR_TRACE(NEXUS_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->getSoftDecisions(handle->pDeviceHandle, pDecisions, length);
    }
}

NEXUS_Error NEXUS_Frontend_GetUserParameters( NEXUS_FrontendHandle handle, NEXUS_FrontendUserParameters *pParams )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != pParams);
    *pParams = handle->userParameters;
    if (handle->userParameters.isMtsif) {
        NEXUS_FRONTEND_USER_PARAM1_SET_INPUT_BAND(pParams->param1, handle->mtsif.inputBand);
        NEXUS_FRONTEND_USER_PARAM1_SET_MTSIF_TX(pParams->param1, handle->mtsif.txOut);
        NEXUS_FRONTEND_USER_PARAM1_SET_DAISYCHAIN_MTSIF_TX(pParams->param1, handle->mtsif.daisyTxOut);
    }
    return BERR_SUCCESS;
}

NEXUS_Error NEXUS_Frontend_SetUserParameters( NEXUS_FrontendHandle handle, const NEXUS_FrontendUserParameters *pParams )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != pParams);
    handle->userParameters = *pParams;
    if (pParams->isMtsif) {
        handle->mtsif.inputBand = NEXUS_FRONTEND_USER_PARAM1_GET_INPUT_BAND(pParams->param1);
        handle->mtsif.txOut = NEXUS_FRONTEND_USER_PARAM1_GET_MTSIF_TX(pParams->param1);
        handle->mtsif.daisyTxOut = NEXUS_FRONTEND_USER_PARAM1_GET_DAISYCHAIN_MTSIF_TX(pParams->param1);
    }
    return BERR_SUCCESS;
}

#if NEXUS_HAS_MXT
typedef struct NEXUS_FrontendHostMtsifConfig {
    struct {
        unsigned demodPb; /* PB number used on demod */
        bool connected; /* if connection specified in NEXUS_ParserBandSettings has been established (via tune) */
        NEXUS_FrontendDeviceMtsifConfig *deviceConfig; /* device that is feeding the host PB */
    } hostPbSettings[NEXUS_NUM_PARSER_BANDS];
} NEXUS_FrontendHostMtsifConfig;
static struct NEXUS_FrontendHostMtsifConfig g_NEXUS_Frontend_P_HostMtsifConfig;
#endif

void NEXUS_Frontend_P_Init(void)
{
#if NEXUS_HAS_MXT
    BKNI_Memset(&g_NEXUS_Frontend_P_HostMtsifConfig, 0, sizeof(g_NEXUS_Frontend_P_HostMtsifConfig));
#endif
}

void NEXUS_Frontend_GetDefaultAcquireSettings( NEXUS_FrontendAcquireSettings *pSettings )
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

static void NEXUS_Frontend_UninstallCallbacks_priv(NEXUS_FrontendHandle handle)
{
    NEXUS_FrontendDeviceHandle deviceHandle = NULL;
    BDBG_ASSERT(NULL != handle);

    deviceHandle = NEXUS_Frontend_GetDevice(handle);
    if(deviceHandle && (deviceHandle->openFailed || deviceHandle->abortThread)) goto done;

    if(handle->uninstallCallbacks) {
        handle->uninstallCallbacks(handle->pDeviceHandle);
    }
    else if ( handle->pParentFrontend && handle->pParentFrontend->uninstallCallbacks ) {
        NEXUS_Frontend_UninstallCallbacks_priv(handle->pParentFrontend);
    }

done:
    return;
}

NEXUS_Error NEXUS_Frontend_GetBertStatus(NEXUS_FrontendHandle handle, NEXUS_FrontendBertStatus *pStatus)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getBertStatus )
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_GetBertStatus(handle->pParentFrontend, pStatus);
        }
        else
        {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->getBertStatus(handle->pDeviceHandle, pStatus);
    }
}

NEXUS_FrontendHandle NEXUS_Frontend_P_Create(void *pDeviceHandle)
{
    NEXUS_FrontendHandle handle;

    BDBG_ASSERT(NULL != pDeviceHandle);
    handle = BKNI_Malloc(sizeof(NEXUS_Frontend));
    if (!handle) {
        BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
        return NULL;
    }

    NEXUS_OBJECT_INIT(NEXUS_Frontend, handle);
    handle->pDeviceHandle = pDeviceHandle;
    /* must be append for implicit index to work */
    BLST_SQ_INSERT_TAIL(&g_frontendList.frontends, handle, link);
    NEXUS_OBJECT_REGISTER(NEXUS_Frontend, handle, Create);

    handle->connector = NEXUS_FrontendConnector_Create();
    if (!handle->connector) {
        goto error;
    }

    return handle;

error:
    NEXUS_Frontend_Close(handle);
    return NULL;
}

NEXUS_FrontendHandle NEXUS_Frontend_Acquire( const NEXUS_FrontendAcquireSettings *pSettings )
{
    NEXUS_FrontendHandle frontend;
    unsigned index;
    for (frontend = BLST_SQ_FIRST(&g_frontendList.frontends), index = 0; frontend; frontend = BLST_SQ_NEXT(frontend, link), index++) {
        BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);
        if (frontend->acquired) continue;
        switch (pSettings->mode) {
        case NEXUS_FrontendAcquireMode_eByCapabilities:
            if ((!pSettings->capabilities.qam || frontend->capabilities.qam) &&
                (!pSettings->capabilities.vsb || frontend->capabilities.vsb) &&
                (!pSettings->capabilities.ofdm || frontend->capabilities.ofdm) &&
                (!pSettings->capabilities.outOfBand || frontend->capabilities.outOfBand) &&
                (!pSettings->capabilities.ifd || frontend->capabilities.ifd) &&
                (!pSettings->capabilities.satellite || frontend->capabilities.satellite))
            {
                frontend->acquired = true;
                NEXUS_OBJECT_REGISTER(NEXUS_FrontendConnector, frontend->connector, Acquire);
                return frontend;
            }
            break;
        case NEXUS_FrontendAcquireMode_eByIndex:
            if (pSettings->index == NEXUS_ANY_ID || pSettings->index == index) {
                frontend->acquired = true;
                NEXUS_OBJECT_REGISTER(NEXUS_FrontendConnector, frontend->connector, Acquire);
                return frontend;
            }
            break;
        default:
            BERR_TRACE(NEXUS_INVALID_PARAMETER); return NULL;
        }
    }
    return NULL;
}

void NEXUS_Frontend_Release( NEXUS_FrontendHandle frontend )
{
    NEXUS_FrontendDeviceHandle deviceHandle = NULL;
    BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);

    NEXUS_Frontend_UninstallCallbacks_priv(frontend);


    deviceHandle = NEXUS_Frontend_GetDevice(frontend);
    if(deviceHandle == NULL) goto done;

    if(!deviceHandle->openFailed && !deviceHandle->abortThread){
        NEXUS_Frontend_Untune(frontend);
    }
    if(frontend->acquired){
        frontend->acquired = false;
        NEXUS_OBJECT_UNREGISTER(NEXUS_FrontendConnector, frontend->connector, Release);
    }
done:
    return;
}

/***************************************************************************
Summary:
    Close a frontend handle
***************************************************************************/

static void NEXUS_Frontend_P_Release(NEXUS_FrontendHandle handle)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    if(handle->acquired)
        NEXUS_Frontend_Release(handle);

    NEXUS_OBJECT_UNREGISTER(NEXUS_Frontend, handle, Destroy);
}

void NEXUS_Frontend_P_Destroy(NEXUS_FrontendHandle handle)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BLST_SQ_REMOVE(&g_frontendList.frontends, handle, NEXUS_Frontend, link);
    if (handle->connector) {
        NEXUS_FrontendConnector_Destroy(handle->connector);
    }
    BDBG_OBJECT_DESTROY(handle, NEXUS_Frontend);
    BKNI_Free(handle);
    return;
}

static void NEXUS_Frontend_P_Finalizer(NEXUS_FrontendHandle handle)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != handle->close);
    if ( handle->numExtensions > 0 )
    {
        BDBG_ERR(("Can not close a frontend with open extensions."));
        BDBG_ASSERT(handle->numExtensions == 0);
    }

#if (0==(NEXUS_HOST_CONTROL_EXTERNAL_FRONTEND_STANDBY+NEXUS_HOST_CONTROL_INTERNAL_FRONTEND_STANDBY))
    /* These standby use cases need to separate untune and close. Otherwise... */
    /* untune before closing. may result in power savings. */
    {
        NEXUS_FrontendDeviceHandle deviceHandle = NULL;
        deviceHandle = NEXUS_Frontend_GetDevice(handle);

        if(deviceHandle && !deviceHandle->openFailed && !deviceHandle->abortThread){
            NEXUS_Frontend_Untune(handle);
        }
    }
#endif

#ifdef NEXUS_FRONTEND_CUSTOMER_EXTENSION
    NEXUS_Frontend_P_UninitExtension(handle);
#endif
    if(handle->pGenericDeviceHandle && (handle->pGenericDeviceHandle->openPending || handle->pGenericDeviceHandle->openFailed))
    BDBG_WRN(("NEXUS_Frontend_P_Finalizer: Application aborted prematurely on the previous run. Waiting for device initialization thread to exit."));

    handle->close(handle);

    goto done; /* This bypasses a compile warning if the earlier standby block is not compiled. */

done:
    return;
}

NEXUS_OBJECT_CLASS_MAKE_WITH_RELEASE(NEXUS_Frontend, NEXUS_Frontend_Close);

void NEXUS_Frontend_Untune( NEXUS_FrontendHandle handle )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        BDBG_ERR(("Device open failed. Cannot untune."));
        return;
    }

#if 0
    /* if app has disconnected mtsif, this will release mapping */
    NEXUS_Frontend_ReapplyTransportSettings(handle);
#endif

    if ( handle->untune )
    {
        handle->untune(handle->pDeviceHandle);
    }
    else if ( handle->pParentFrontend && handle->pParentFrontend->untune )
    {
        NEXUS_Frontend_Untune(handle->pParentFrontend);
    }
    else
    {
        /* This is a simple way to reset the app callback. In the future, we may want
                a chip-specific way of reseting state. */
        if ( handle->capabilities.qam )
        {
            NEXUS_FrontendQamSettings settings;
            NEXUS_Frontend_GetDefaultQamSettings(&settings);
            (void)NEXUS_Frontend_TuneQam(handle, &settings);
        }
        else if ( handle->capabilities.vsb )
        {
            NEXUS_FrontendVsbSettings settings;
            NEXUS_Frontend_GetDefaultVsbSettings(&settings);
            (void)NEXUS_Frontend_TuneVsb(handle, &settings);
        }
        else if ( handle->capabilities.satellite )
        {
            NEXUS_FrontendSatelliteSettings settings;
            NEXUS_Frontend_GetDefaultSatelliteSettings(&settings);
            settings.frequency=950000000UL;
            (void)NEXUS_Frontend_TuneSatellite(handle, &settings);
        }
        else if ( handle->capabilities.ofdm )
        {
            NEXUS_FrontendOfdmSettings settings;
            NEXUS_Frontend_GetDefaultOfdmSettings(&settings);
            (void)NEXUS_Frontend_TuneOfdm(handle, &settings);
        }
        else if ( handle->capabilities.analog )
        {
            NEXUS_FrontendAnalogSettings settings;
            NEXUS_Frontend_GetDefaultAnalogSettings(&settings);
            (void)NEXUS_Frontend_TuneAnalog(handle, &settings);
        }
        else if ( handle->capabilities.outOfBand )
        {
            NEXUS_FrontendOutOfBandSettings settings;
            NEXUS_Frontend_GetDefaultOutOfBandSettings(&settings);
            (void)NEXUS_Frontend_TuneOutOfBand(handle, &settings);
        }
        else if ( handle->capabilities.upstream )
        {
            /* no untune for upstream*/
        }
        else
        {
            BDBG_ERR(("Unable to untune. No tuner capabilities found."));
        }
    }
}

/***************************************************************************
Summary:
    Reset status values for a frontend
***************************************************************************/
void NEXUS_Frontend_ResetStatus(NEXUS_FrontendHandle handle)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        BDBG_ERR(("Device open failed. Cannot reset status."));
        return;
    }

    NEXUS_Time_Get(&handle->resetStatusTime);
    if ( handle->resetStatus )
    {
        handle->resetStatus(handle->pDeviceHandle);
    }
    else if ( handle->pParentFrontend )
    {
        NEXUS_Frontend_ResetStatus(handle->pParentFrontend);
    }
}

/***************************************************************************
Summary:
    Get the Synchronous lock status of a frontend.
 ***************************************************************************/
NEXUS_Error NEXUS_Frontend_GetFastStatus(NEXUS_FrontendHandle handle, NEXUS_FrontendFastStatus *pStatus)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getFastStatus )
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_GetFastStatus(handle->pParentFrontend, pStatus);
        }
        else
        {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->getFastStatus(handle->pDeviceHandle, pStatus);
    }
}

/***************************************************************************
Summary:
    Retrieve the chip family id, chip id, chip version and firmware version.
 ***************************************************************************/
void NEXUS_Frontend_GetType(NEXUS_FrontendHandle handle,NEXUS_FrontendType *pType)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        BDBG_ERR(("Device open failed. Cannot get type."));
        BERR_TRACE(NEXUS_NOT_INITIALIZED);
        return;
    }

    if ( NULL == handle->getType )
    {
        if ( handle->pParentFrontend )
        {
            NEXUS_Frontend_GetType(handle->pParentFrontend, pType);
        }
        else
        {
            BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        handle->getType(handle->pDeviceHandle, pType);
    }
}

void NEXUS_Frontend_GetDefaultSpectrumSettings(NEXUS_FrontendSpectrumSettings *pSettings)
{
    BDBG_ASSERT(NULL != pSettings);
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}


NEXUS_Error NEXUS_Frontend_RequestSpectrumData(NEXUS_FrontendHandle handle, const NEXUS_FrontendSpectrumSettings *settings)
{
    BDBG_ASSERT(NULL != handle);

    if (!settings || !settings->data) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }
    if (!NEXUS_P_CpuAccessibleAddress(settings->data)) {
        /* this pointer must have driver-side mapping */
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->requestSpectrumData )
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_RequestSpectrumData(handle->pParentFrontend, settings);
        }
        else
        {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->requestSpectrumData(handle->pDeviceHandle, settings);
    }
}

void NEXUS_Frontend_P_GetDefaultExtensionSettings(NEXUS_FrontendExtensionSettings *pSettings )
{
    BDBG_ASSERT(NULL != pSettings);
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

/* Functions to override.  The close function is required, all other functions are optional.
    If you do not want to extend a function, leave it in the default state and the parent's handler will be called instead. */
NEXUS_FrontendHandle NEXUS_Frontend_P_CreateExtension(const NEXUS_FrontendExtensionSettings *pSettings )
{
    NEXUS_Frontend *pFrontend;

    BDBG_ASSERT(NULL != pSettings);
    BDBG_ASSERT(NULL != pSettings->close);
    BDBG_ASSERT(NULL != pSettings->parent);
    NEXUS_ASSERT_MODULE();

    pFrontend = NEXUS_Frontend_P_Create(pSettings->pDeviceHandle);
    if ( NULL == pFrontend )
    {
        (void)BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
        return NULL;
    }

    if (pSettings->parent->registerExtension) {
        (*pSettings->parent->registerExtension)(pSettings->parent, pFrontend);
    }

    pFrontend->capabilities = pSettings->parent->capabilities;
    pFrontend->userParameters = pSettings->parent->userParameters;
    pFrontend->pParentFrontend = pSettings->parent;
    pFrontend->close = pSettings->close;
    pFrontend->tuneAnalog = pSettings->tuneAnalog;
    pFrontend->tuneOutOfBand = pSettings->tuneOutOfBand;
    pFrontend->tuneQam = pSettings->tuneQam;
    pFrontend->tuneUpstream = pSettings->tuneUpstream;
    pFrontend->tuneVsb = pSettings->tuneVsb;
    pFrontend->tuneSatellite = pSettings->tuneSatellite;
    pFrontend->getDiseqcSettings = pSettings->getDiseqcSettings;
    pFrontend->setDiseqcSettings = pSettings->setDiseqcSettings;
    pFrontend->sendDiseqcMessage = pSettings->sendDiseqcMessage;
    pFrontend->getDiseqcReply = pSettings->getDiseqcReply;
    pFrontend->sendDiseqcAcw = pSettings->sendDiseqcAcw;
    pFrontend->resetDiseqc = pSettings->resetDiseqc;
    pFrontend->tuneOfdm = pSettings->tuneOfdm;
    pFrontend->untune = pSettings->untune;

    pSettings->parent->numExtensions++;

    return pFrontend;
}

void NEXUS_Frontend_P_DestroyExtension(NEXUS_FrontendHandle handle)
{
    NEXUS_ASSERT_MODULE();
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);
    BDBG_ASSERT(NULL != handle->pParentFrontend);
    BDBG_ASSERT(handle->pParentFrontend->numExtensions > 0);
    handle->pParentFrontend->numExtensions--;

    if (handle->pParentFrontend->registerExtension) {
        (*handle->pParentFrontend->registerExtension)(handle->pParentFrontend, NULL);
    }

    NEXUS_Frontend_P_Destroy(handle);
}

#include "priv/nexus_transport_priv.h"

#if NEXUS_HAS_MXT
NEXUS_Error NEXUS_Frontend_P_InitMtsifConfig(NEXUS_FrontendDeviceMtsifConfig *pConfig, const BMXT_Settings *mxtSettings)
{
    BMXT_ParserConfig parserConfig;
    unsigned i;
    NEXUS_Error rc;
    BMXT_Handle mxt = pConfig->mxt;

    if (mxt==NULL) {
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }
    /* TODO: we could move BMXT_Open() here as well, but some platforms (e.g. 7366) would need to pass in an extra BREG handle to this function */

    BKNI_Memset(pConfig, 0, sizeof(NEXUS_FrontendDeviceMtsifConfig));
    pConfig->mxt = mxt;
    pConfig->numDemodIb = BMXT_GetNumResources(mxtSettings->chip, mxtSettings->chipRev, BMXT_ResourceType_eInputBand);
    pConfig->numDemodPb = BMXT_GetNumResources(mxtSettings->chip, mxtSettings->chipRev, BMXT_ResourceType_eParser);

    for (i=0; i<pConfig->numDemodPb; i++) {
        rc = BMXT_GetParserConfig(pConfig->mxt, i, &parserConfig);
        if (rc) { return BERR_TRACE(rc); }

        /* turn off demod parsers */
        parserConfig.Enable = false;

        /* cache other settings */
        pConfig->demodPbSettings[i].errorInputIgnore = parserConfig.ErrorInputIgnore;
        pConfig->demodPbSettings[i].timestampMode = (unsigned)parserConfig.TsMode;
        pConfig->demodPbSettings[i].dssMode = parserConfig.DssMode;
        pConfig->demodPbSettings[i].mtsifTxSel = parserConfig.mtsifTxSelect;

        rc = BMXT_SetParserConfig(pConfig->mxt, i, &parserConfig);
        if (rc) { return BERR_TRACE(rc); }
    }

    return NEXUS_SUCCESS;
}

#if NEXUS_HAS_TSMF
static NEXUS_Error NEXUS_Frontend_P_SetTsmfInput(void *mxt, unsigned tsmfInput, unsigned tsmfIndex, const NEXUS_TsmfSettings *tsmfSettings)
{
    NEXUS_Error rc;
    BDBG_CASSERT(sizeof(NEXUS_TsmfFieldVerifyConfig) == sizeof(BMXT_TSMFFldVerifyConfig));
    BDBG_CASSERT(NEXUS_TsmfVersionChangeMode_eFrameChangeVer == (NEXUS_TsmfVersionChangeMode)BMXT_TSMFVersionChgMode_eFrameChgVer);
    BDBG_ASSERT(tsmfSettings->sourceType==NEXUS_TsmfSourceType_eMtsif);

    rc = BMXT_TSMF_SetFldVerifyConfig(mxt, tsmfIndex, (const BMXT_TSMFFldVerifyConfig *)&(tsmfSettings->fieldVerifyConfig));
    if (rc) {rc = BERR_TRACE(rc);} /* keep going */

    if (tsmfSettings->enabled) {
        BMXT_TSMFInputSel inputSelect = (BMXT_TSMFInputSel) tsmfInput;
        BDBG_MSG(("TSMF: IB%u -> TSMF%u (rel %u)", tsmfInput, tsmfIndex, tsmfSettings->relativeTsNum));

        if (tsmfSettings->semiAutomaticMode) {
            rc = BMXT_TSMF_EnableSemiAutoMode(mxt, inputSelect, tsmfIndex, tsmfSettings->slotMapLo, tsmfSettings->slotMapHi, tsmfSettings->relativeTsNum);
            if (rc) { return BERR_TRACE(rc); }
        }
        else {
            rc = BMXT_TSMF_EnableAutoMode(mxt, inputSelect, tsmfIndex, tsmfSettings->relativeTsNum);
            if (rc) { return BERR_TRACE(rc); }
        }
    }
    else {
        BDBG_MSG(("TSMF: TSMF%u disabled", tsmfIndex));
        rc = BMXT_TSMF_DisableTsmf(mxt, tsmfIndex);
        if (rc) { return BERR_TRACE(rc); }
    }
    return NEXUS_SUCCESS;
}

static NEXUS_Error NEXUS_Frontend_P_SetTsmfOutput(void *mxt, unsigned tsmfOutput, unsigned tsmfIndex, bool enabled)
{
    NEXUS_Error rc;
    BDBG_MSG(("TSMF: TSMF%u -> PB%u (enable %u)", tsmfIndex, tsmfOutput, enabled));
    rc = BMXT_TSMF_SetParserConfig(mxt, tsmfOutput, tsmfIndex, enabled);
    if (rc) { return BERR_TRACE(rc); }
    return NEXUS_SUCCESS;
}
#endif

static void NEXUS_Frontend_P_MxtTimer(void* context)
{
    NEXUS_FrontendDeviceMtsifConfig *pConfig = context;
    BMXT_Handle mxt = pConfig->mxt;
    unsigned i;
    uint32_t status = BMXT_ReadIntStatusRegister(mxt, BMXT_IntReg_eFE_INTR_STATUS0);

    BDBG_MSG_TRACE(("NEXUS_Frontend_P_MxtTimer: status %#x", status));
    if (status) {
        unsigned lengthError = 0, transportError = 0; /* remap frontend PB to backend PB */
        /* bits 0~15 are length error for parser 0 through 15. bits 16~31 are transport error for parser 0 through 15 */
        for (i=0; i<16; i++) {
            if (i<pConfig->numDemodPb) {
                if ((status>>i)&0x1) {
                    lengthError |= 1 << pConfig->demodPbSettings[i].virtualNum;
                }
                if ((status>>(i+16))&0x1) {
                    transportError |= 1 << pConfig->demodPbSettings[i].virtualNum;
                }
            }
        }
        NEXUS_Module_Lock(g_NEXUS_frontendModuleSettings.transport);
        NEXUS_ParserBand_P_MtsifErrorStatus_priv(lengthError, transportError);
        NEXUS_Module_Unlock(g_NEXUS_frontendModuleSettings.transport);
    }

    pConfig->timer = NEXUS_ScheduleTimer(1000, NEXUS_Frontend_P_MxtTimer, pConfig);
}

#if NEXUS_DUMP_MTSIF
static void NEXUS_Frontend_P_DumpMtsifConfig(const NEXUS_FrontendDeviceMtsifConfig *pConfig);
#endif

/* configure the demod transport core to route a given frontend to one or more parser bands on host */
static NEXUS_Error NEXUS_Frontend_P_ConfigMtsifConfig(NEXUS_FrontendHandle frontend, bool enabled)
{
    unsigned i;
    unsigned demodIb, demodPb, demodMtsifTx, demodDaisyMtsifTx;
    NEXUS_FrontendUserParameters userParams;
    NEXUS_FrontendDeviceMtsifConfig *pConfig = &frontend->pGenericDeviceHandle->mtsifConfig;
    NEXUS_FrontendDeviceMtsifConfig *pChainedConfig = frontend->pGenericDeviceHandle->chainedConfig; /* points to a chained MTSIF config, if it exists */
    BMXT_Handle mxt = pConfig->mxt;
    NEXUS_Error rc = NEXUS_SUCCESS;
    struct NEXUS_MtsifParserBandSettings *mtsifConnections = NULL;
    unsigned num;
    bool setTimer = false;
    mtsifConnections = BKNI_Malloc(sizeof(*mtsifConnections)*MAX_PB_PER_MTSIF_FRONTEND);
    if (!mtsifConnections) { BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY); goto done; }

    NEXUS_Frontend_GetUserParameters(frontend, &userParams);
    demodIb = frontend->mtsif.inputBand;
    demodMtsifTx = frontend->mtsif.txOut;
    demodDaisyMtsifTx = frontend->mtsif.daisyTxOut;

    if (!userParams.isMtsif) {
        rc = NEXUS_SUCCESS; goto done;
    }
    if (mxt==NULL) {
        rc = BERR_TRACE(NEXUS_INVALID_PARAMETER); goto done;
    }
    if (pConfig->numDemodIb==0 || pConfig->numDemodIb>BMXT_MAX_NUM_INPUT_BANDS || pConfig->numDemodPb==0 || pConfig->numDemodPb>BMXT_MAX_NUM_PARSERS) {
        rc = BERR_TRACE(NEXUS_INVALID_PARAMETER); goto done;
    }

    /* one device but two mtsif configs. this is a special case that requires setting the pointers to the correct config for this frontend handle */
    if (userParams.pParam2) { /* TODO: use a nexus_frontend_mtsif_priv.h macro to handle this case */
        if (frontend->pGenericDeviceHandle->chainedConfig==NULL) {
            rc = BERR_TRACE(NEXUS_INVALID_PARAMETER); goto done;
        }
        if (frontend->pGenericDeviceHandle->chainedConfig->mxt==NULL) {
            rc = BERR_TRACE(NEXUS_INVALID_PARAMETER); goto done;
        }

        /* swap configs */
        pChainedConfig = pConfig;
        pConfig = frontend->pGenericDeviceHandle->chainedConfig;
        mxt = pConfig->mxt;

        /* HAB/RPC-chained, but not MTSIF-chained */
        if (!pConfig->slave && !pChainedConfig->slave) {
            pChainedConfig = NULL;
        }
    }

    /* 1. find all host PBs whose MTSIF source is equal to this frontend handle. we must map those PBs to this frontend */
    if (enabled) {
        NEXUS_Module_Lock(g_NEXUS_frontendModuleSettings.transport);
        NEXUS_ParserBand_GetMtsifConnections_priv(frontend->connector, mtsifConnections, MAX_PB_PER_MTSIF_FRONTEND, &num);
        NEXUS_Module_Unlock(g_NEXUS_frontendModuleSettings.transport);
    }
    else {
        num = 0;
    }

    for (i=0; i<num; i++) {
        unsigned hostPb = mtsifConnections[i].index;
        const NEXUS_ParserBandSettings *pSettings = &mtsifConnections[i].settings;

        /* 2. check all current connections for existing connections or conflicts */
        for (demodPb=0; demodPb<pConfig->numDemodPb; demodPb++) {
            if (pConfig->demodPbSettings[demodPb].enabled) {
                if (pConfig->demodPbSettings[demodPb].virtualNum==hostPb) {
                    if (pConfig->demodPbSettings[demodPb].inputBandNum==demodIb) {
                        /* specified FE-> PB connection already exists due to tuning again without untune */
                        BDBG_MSG(("MTSIF exist:      IB%u -> PB%u -> PB%u (TX%u, %p:%p)", demodIb, demodPb, hostPb, pConfig->demodPbSettings[demodPb].mtsifTxSel, (void *)frontend, (void *)mxt));
                        break;
                    }
                    else {
                        /* mark the existing connection as NULL so we override it below
                           typical example: FE0 -> PB0 then FE1 -> PB0. FE0 is still tuned, but data not routed anywhere */
                        pConfig->demodPbSettings[demodPb].enabled = false;
                        pConfig->demodPbSettings[demodPb].connector = NULL;
                        BDBG_MSG(("MTSIF override:   IB%u -> PB%u -> PB%u (TX%u, %p:%p)", pConfig->demodPbSettings[demodPb].inputBandNum, demodPb, hostPb, demodMtsifTx, (void *)frontend, (void *)mxt));
                    }
                }
            }
        }

        if (demodPb < pConfig->numDemodPb) { /* connection already exists. skip over picking a free demod PB */
            if (pConfig->demodPbSettings[demodPb].mtsifTxSel != demodMtsifTx) { /* allow MTSIF_TX change on-the-fly */
                goto set_mapping;
            }
            goto apply_settings;
        }

        /* 3. pick a free demod PB */
        for (demodPb=0; demodPb<pConfig->numDemodPb; demodPb++) {
            /* coverity [dead_error_condition:false] */
            if (pConfig->demodPbSettings[demodPb].enabled==false) {
                BMXT_ParserConfig parserConfig;

                /* if chained, then the chainedConfig adds more restrictions to which PB we can pick */
                if (pChainedConfig) {
                    unsigned k;
                    if (!pConfig->slave) {
                        /* master has to avoid physical PBs that are in use by slave as virtual PBs.
                           could also do this by adding .enabledPassthrough to demodPbSettings instead */
                        for (k=0; k<pChainedConfig->numDemodPb; k++) {
                            if (pChainedConfig->demodPbSettings[k].enabled &&
                                pChainedConfig->demodPbSettings[k].virtualNum==demodPb)
                            {
                                goto next;
                            }
                        }
                    }
                    else {
                        /* slave has to check if virtual PB is in use by master as physical PB and error out */
                        if (pChainedConfig->demodPbSettings[hostPb].enabled) {
                            BDBG_ERR(("Unable to route data IB%u -> PB%u -> PB%u -> PB%u because PB%u is in use by chain master", demodIb, demodPb, hostPb, hostPb, hostPb));
                            demodPb = pConfig->numDemodPb; /* no way to get around this for now */
                            break;
                        }
                    }
                }

set_mapping:
                /* 4a. map demod IB->demod PB */
                BMXT_GetParserConfig(mxt, demodPb, &parserConfig);
                pConfig->demodPbSettings[demodPb].inputBandNum = parserConfig.InputBandNumber = demodIb;
                pConfig->demodPbSettings[demodPb].enabled = parserConfig.Enable = true;
                pConfig->demodPbSettings[demodPb].connector = frontend->connector;
                if (pChainedConfig && !pConfig->slave) {
                    pConfig->demodPbSettings[demodPb].mtsifTxSel = parserConfig.mtsifTxSelect = 0; /* master's own PBs are always routed through TX0 */
                }
                else {
                    pConfig->demodPbSettings[demodPb].mtsifTxSel = parserConfig.mtsifTxSelect = demodMtsifTx; /* otherwise, honor the TX selection */
                }
                BMXT_SetParserConfig(mxt, demodPb, &parserConfig);

                /* if slave, then we need to change the passthrough routing on master */
                if (pConfig->slave) {
                    BMXT_Handle mxt_master;
                    BDBG_ASSERT(pChainedConfig); /* if slave, then by definition, pChainedConfig is non-NULL */
                    mxt_master = pChainedConfig->mxt;
                    BMXT_GetParserConfig(mxt_master, hostPb, &parserConfig);
                    BDBG_ASSERT(parserConfig.Enable == false);
                    pChainedConfig->demodPbSettings[hostPb].mtsifTxSel = parserConfig.mtsifTxSelect = demodDaisyMtsifTx;
                    BMXT_SetParserConfig(mxt_master, hostPb, &parserConfig);
                }

                /* 4b. set virtual PB num of demod PB equal to host PB num */
                BMXT_GetParserConfig(mxt, demodPb, &parserConfig);
                pConfig->demodPbSettings[demodPb].virtualNum = parserConfig.virtualParserNum = hostPb;
                BMXT_SetParserConfig(mxt, demodPb, &parserConfig);
                BDBG_MSG(("MTSIF connect:    IB%u -> PB%u -> PB%u (TX%u, %p:%p) %s", demodIb, demodPb, hostPb, parserConfig.mtsifTxSelect,
                    (void *)frontend, (void *)mxt, pChainedConfig?(pConfig->slave?"(chain slave) ":"(chain master)"):""));

                if ( (g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].connected) && /* if this hostPb is already being fed */
                    (g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].deviceConfig != pConfig) )
                {
                    NEXUS_FrontendDeviceMtsifConfig *otherConfig = g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].deviceConfig;
                    BMXT_Handle otherMxt;
                    unsigned otherDemodPb;
                    BDBG_ASSERT(otherConfig);

                    otherMxt = otherConfig->mxt;
                    otherDemodPb = g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].demodPb;

                    BMXT_GetParserConfig(otherMxt, otherDemodPb, &parserConfig);
                    otherConfig->demodPbSettings[otherDemodPb].enabled = parserConfig.Enable = false;
                    BMXT_SetParserConfig(otherMxt, otherDemodPb, &parserConfig);
                    BDBG_WRN(("MTSIF conflict: host PB%u is already receiving data from another MTSIF device (%p). Forcing disable of demod PB%u (IB%u -> PB%u -> PB%u)",
                        hostPb, (void*)otherMxt, otherDemodPb, parserConfig.InputBandNumber, otherDemodPb, parserConfig.virtualParserNum));
                }

                g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].demodPb = demodPb;
                g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].connected = true;
                g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].deviceConfig = pConfig;
                break;
            }
next:
            BSTD_UNUSED(0);
        }

apply_settings:
        /* did not find free demod PB */
        if (demodPb>=pConfig->numDemodPb) {
            BDBG_ERR(("No demod PB available to connect: %p:%p IB%u -> PB? -> PB%u", (void *)frontend, (void *)mxt, demodIb, hostPb));
            rc = BERR_TRACE(NEXUS_NOT_AVAILABLE); goto done;
        }

        /* grab the settings of the output PB on the host, and apply them to the demod PB */
        if ( (pConfig->demodPbSettings[demodPb].errorInputIgnore != pSettings->teiIgnoreEnabled) ||
             (pConfig->demodPbSettings[demodPb].timestampMode != (NEXUS_IS_DSS_MODE(pSettings->transportType) ? BMXT_ParserTimestampMode_eBinary : BMXT_ParserTimestampMode_eMod300)) ||
             (pConfig->demodPbSettings[demodPb].dssMode != NEXUS_IS_DSS_MODE(pSettings->transportType)) )
        {
            BMXT_ParserConfig mxtParserConfig;
            BDBG_MSG(("MTSIF PB settings: demod PB%u <- host PB%u (DSS %u)", demodPb, hostPb, NEXUS_IS_DSS_MODE(pSettings->transportType)));
            BMXT_GetParserConfig(mxt, demodPb, &mxtParserConfig);
            /* translate NEXUS_ParserBandSettings to BMXT_ParserConfig.
               note the ParserBandSettings have already been applied to host; we'll just apply them to the demod as well.
               some may be needed, some not. */
            mxtParserConfig.ErrorInputIgnore = pSettings->teiIgnoreEnabled;
            mxtParserConfig.TsMode = NEXUS_IS_DSS_MODE(pSettings->transportType) ? BMXT_ParserTimestampMode_eBinary : BMXT_ParserTimestampMode_eMod300;
            mxtParserConfig.DssMode = NEXUS_IS_DSS_MODE(pSettings->transportType);
            /* ignore allPass and acceptNullPackets for demod. demod is always in allpass + acceptnull mode. */
            BMXT_SetParserConfig(mxt, demodPb, &mxtParserConfig);

            if (pConfig->demodPbSettings[demodPb].dssMode != NEXUS_IS_DSS_MODE(pSettings->transportType)) {
                BMXT_InputBandConfig mxtInputBandConfig;
                BDBG_MSG(("MTSIF IB settings: demod IB%u <- host PB%u (DSS %u)", demodIb, hostPb, NEXUS_IS_DSS_MODE(pSettings->transportType)));
                BMXT_GetInputBandConfig(mxt, demodIb, &mxtInputBandConfig);
                mxtInputBandConfig.DssMode = mxtParserConfig.DssMode;
                BMXT_SetInputBandConfig(mxt, demodIb, &mxtInputBandConfig);
            }

            pConfig->demodPbSettings[demodPb].errorInputIgnore = mxtParserConfig.ErrorInputIgnore;
            pConfig->demodPbSettings[demodPb].timestampMode = mxtParserConfig.TsMode;
            pConfig->demodPbSettings[demodPb].dssMode = mxtParserConfig.DssMode;
        }
        if (pSettings->teiError.callback || pSettings->lengthError.callback) {
            setTimer = true;
        }

#if NEXUS_HAS_TSMF
        if (!mtsifConnections[i].tsmf.valid) continue;
        /* set input: connect this frontend to all (could be more than one) TSMF whose input matches this frontend */
        NEXUS_Frontend_P_SetTsmfInput(mxt, demodIb, mtsifConnections[i].tsmf.hwIndex, &mtsifConnections[i].tsmf.settings);

        /* set output: connect this TSMF to all demod PB that are connected to host PBs whose input is this TSMF (yes, it's complicated...) */
        demodPb = g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[mtsifConnections[i].index].demodPb; /* demod TSMF output is the demod PB, not the host PB */
        NEXUS_Frontend_P_SetTsmfOutput(mxt, demodPb, mtsifConnections[i].tsmf.hwIndex, mtsifConnections[i].tsmf.settings.enabled);
#endif

    }

    /* find any previously-established connections that must now be disconnected. disconnects are only implicitly requested, so we must search */
    for (demodPb=0; demodPb<pConfig->numDemodPb; demodPb++) {

        if (!pConfig->demodPbSettings[demodPb].enabled) { continue; }
        if (pConfig->demodPbSettings[demodPb].connector != frontend->connector) {
            continue; /* if PB is being fed by a different frontend, then the new connection has already been established, so no need to disconnect the old one */
        }

        for (i=0; i<num; i++) {
            if (mtsifConnections[i].index == pConfig->demodPbSettings[demodPb].virtualNum) { break; } /* connection is still valid */
        }
        if (i == num) {
            /* connection no longer valid, so disconnect */
            BMXT_ParserConfig parserConfig;
            unsigned hostPb = pConfig->demodPbSettings[demodPb].virtualNum;
            BMXT_GetParserConfig(mxt, demodPb, &parserConfig);
            pConfig->demodPbSettings[demodPb].enabled = parserConfig.Enable = false;
            pConfig->demodPbSettings[demodPb].connector = NULL;
            BMXT_SetParserConfig(mxt, demodPb, &parserConfig);
            BDBG_MSG(("MTSIF disconnect: IB%u -> PB%u -> PB%u (%p:%p)", demodIb, demodPb, hostPb, (void *)frontend, (void *)mxt));
            g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].demodPb = 0;
            g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].connected = false;
            g_NEXUS_Frontend_P_HostMtsifConfig.hostPbSettings[hostPb].deviceConfig = 0;
#if NEXUS_HAS_TSMF
            /* disable/reset the parser's TSMF config. otherwise, a new frontend -> host_PB connection may inadvertently get TSMF-filtered */
            BMXT_TSMF_SetParserConfig(mxt, demodPb, 0 /* don't care */, false);
#endif
        }
    }

    if (setTimer) {
        if (!pConfig->timer) {
            pConfig->timer = NEXUS_ScheduleTimer(1000, NEXUS_Frontend_P_MxtTimer, pConfig);
        }
    }
    else {
        if (pConfig->timer) {
            NEXUS_CancelTimer(pConfig->timer);
            pConfig->timer = NULL;
        }
    }

#if NEXUS_DUMP_MTSIF
    NEXUS_Frontend_P_DumpMtsifConfig(pConfig);
    if (pChainedConfig) {
        NEXUS_Frontend_P_DumpMtsifConfig(pChainedConfig);
    }
#endif

done:
    if (mtsifConnections) { BKNI_Free(mtsifConnections); }
    return rc;
}

NEXUS_Error NEXUS_Frontend_P_SetMtsifConfig(NEXUS_FrontendHandle frontend)
{
    NEXUS_Error rc;
    BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);
    rc = NEXUS_Frontend_P_ConfigMtsifConfig(frontend, true);
    if (rc) return BERR_TRACE(rc);
    return 0;
}

void NEXUS_Frontend_P_UnsetMtsifConfig(NEXUS_FrontendHandle frontend)
{

    BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);
    (void)NEXUS_Frontend_P_ConfigMtsifConfig(frontend, false);
}
#else
NEXUS_Error NEXUS_Frontend_P_InitMtsifConfig(NEXUS_FrontendDeviceMtsifConfig *pConfig, const void *mxtSettings)
{
    BSTD_UNUSED(pConfig);
    BSTD_UNUSED(mxtSettings);
    return 0;
}
NEXUS_Error NEXUS_Frontend_P_SetMtsifConfig(NEXUS_FrontendHandle frontend)
{
    BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);
    return 0;
}

void NEXUS_Frontend_P_UnsetMtsifConfig(NEXUS_FrontendHandle frontend)
{
    BDBG_OBJECT_ASSERT(frontend, NEXUS_Frontend);
}
#endif /* NEXUS_HAS_MXT */

#if NEXUS_DUMP_MTSIF
void NEXUS_Frontend_P_PostDumpMtsifConfig(void *context)
{
#if NEXUS_HAS_MXT
    unsigned i;
    NEXUS_FrontendDeviceMtsifConfig *pConfig = context;
    BMXT_Handle mxt = pConfig->mxt;
    BMXT_InputBandStatus ibStatus;
    BDBG_MSG(("PostDumpMtsifConfig %p (%s)", (void *)mxt, pConfig->slave?"slave":"master"));

    /* IB sync count */
    for (i=0; i<2; i++) {
        BMXT_GetInputBandStatus(mxt, &ibStatus);
        BDBG_MSG(("demod IB: %02x %02x %02x %02x   %02x %02x %02x %02x   %02x %02x %02x %02x   %02x %02x %02x %02x",
            ibStatus.syncCount[0], ibStatus.syncCount[1], ibStatus.syncCount[2], ibStatus.syncCount[3],
            ibStatus.syncCount[4], ibStatus.syncCount[5], ibStatus.syncCount[6], ibStatus.syncCount[7],
            ibStatus.syncCount[8], ibStatus.syncCount[9], ibStatus.syncCount[10], ibStatus.syncCount[11],
            ibStatus.syncCount[12], ibStatus.syncCount[13], ibStatus.syncCount[14], ibStatus.syncCount[15]));
    }
#else
    BSTD_UNUSED(context);
#endif
}

/* debug function to dump all known info */
void NEXUS_Frontend_P_DumpMtsifConfig(const NEXUS_FrontendDeviceMtsifConfig *pConfig)
{
#if NEXUS_HAS_MXT && NEXUS_NUM_PARSER_BANDS
    BMXT_Handle mxt = pConfig->mxt;
    BMXT_ParserConfig parserConfig;
    unsigned i;
    const unsigned numDemodPb = pConfig->numDemodPb>16 ? 16:pConfig->numDemodPb;

    BDBG_ASSERT(pConfig);
    BDBG_ASSERT(mxt);

    BDBG_MSG(("DumpMtsifConfig %p (%s)", (void *)mxt, pConfig->slave?"slave":"master"));
    /* print out real and cached demod PB settings */
    for (i=0; i<numDemodPb; i++) {
        BMXT_GetParserConfig(mxt, i, &parserConfig);
        BDBG_MSG(("demod PB[%2u]: en %u:%u, IB %2u:%2u, virt %2u:%2u, TX %u", i,
            parserConfig.Enable, pConfig->demodPbSettings[i].enabled,
            parserConfig.InputBandNumber, pConfig->demodPbSettings[i].inputBandNum,
            parserConfig.virtualParserNum, pConfig->demodPbSettings[i].virtualNum,
            parserConfig.mtsifTxSelect));
    }

#if 0
    /* print out host PB settings */
    {
        struct NEXUS_MtsifParserBandSettings mtsifConnections[MAX_PB_PER_MTSIF_FRONTEND];
        unsigned i, num;

        NEXUS_Module_Lock(g_NEXUS_frontendModuleSettings.transport);
        NEXUS_ParserBand_GetMtsifConnections_priv(frontend->connector, mtsifConnections, MAX_PB_PER_MTSIF_FRONTEND, &num);
        NEXUS_Module_Unlock(g_NEXUS_frontendModuleSettings.transport);

        for (i=0; i<num; i++) {
            BDBG_MSG(("host PB[%2u]: %u:%#lx", mtsifConnections[i].index, mtsifConnections[i].settings.sourceType,
                mtsifConnections[i].settings.sourceType==NEXUS_ParserBandSourceType_eMtsif ? mtsifConnections[i].settings.sourceTypeSettings.mtsif : NULL));
        }
    }
#endif

    /* print MTSIF status */
    {
        BMXT_MtsifStatus mtsifStatus;
        BMXT_GetMtsifStatus(mxt, &mtsifStatus);
        for (i=0; i<2; i++) {
            if (mtsifStatus.tx[i].enabled) {
                BDBG_MSG(("MTSIF_TX[%u]: enabled, polarity %u, ifWidth %u", i, mtsifStatus.tx[i].clockPolarity, mtsifStatus.tx[i].interfaceWidth));
            }
        }
        for (i=0; i<1; i++) {
            if (mtsifStatus.rx[i].enabled) {
                BDBG_MSG(("MTSIF_RX[%u]: enabled, polarity %u, ifWidth %u", i, mtsifStatus.rx[i].clockPolarity, mtsifStatus.rx[i].interfaceWidth));
            }
        }
    }

    #if 0
    /* print the IB sync count later since it's only useful after the Tune */
    NEXUS_ScheduleTimer(4000, NEXUS_Frontend_P_PostDumpMtsifConfig, pConfig);
    #endif
#else
    BSTD_UNUSED(pConfig);
#endif
}
#endif

NEXUS_Error NEXUS_Frontend_ReapplyTransportSettings(NEXUS_FrontendHandle handle)
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    /* call the device-specific function because the cached MTSIF-config struct is per-device */
    if (NULL == handle->reapplyTransportSettings) {
        if (handle->pParentFrontend) {
            return NEXUS_Frontend_ReapplyTransportSettings(handle->pParentFrontend);
        }
        else {
            return BERR_TRACE(BERR_SUCCESS);
        }
    }
    else {
        return handle->reapplyTransportSettings(handle->pDeviceHandle);
    }
}

NEXUS_Error NEXUS_Frontend_WriteRegister(NEXUS_FrontendHandle handle, unsigned address, uint32_t value )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if (NULL == handle->writeRegister) {
        if (handle->pParentFrontend) {
            return NEXUS_Frontend_WriteRegister(handle->pParentFrontend, address, value);
        }
        else {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else {
        return handle->writeRegister(handle->pDeviceHandle, address, value);
    }
}

NEXUS_Error NEXUS_Frontend_ReadRegister(NEXUS_FrontendHandle handle, unsigned address,  uint32_t *value )
{
    BDBG_OBJECT_ASSERT(handle, NEXUS_Frontend);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if (NULL == handle->readRegister) {
        if (handle->pParentFrontend) {
            return NEXUS_Frontend_ReadRegister(handle->pParentFrontend, address, value);
        }
        else {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else {
        return handle->readRegister(handle->pDeviceHandle, address, value);
    }
}

NEXUS_FrontendHandle NEXUS_Frontend_OpenStub( unsigned index )
{
    BSTD_UNUSED(index);
    BERR_TRACE(NEXUS_NOT_SUPPORTED);
    return NULL;
}

NEXUS_Error NEXUS_Frontend_Standby_priv(NEXUS_FrontendHandle handle, bool enabled, const NEXUS_StandbySettings *pSettings)
{
#if NEXUS_POWER_MANAGEMENT
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if(handle->standby)
    {
        return handle->standby(handle->pDeviceHandle, enabled, pSettings);
    }
    else if ( handle->pParentFrontend && handle->pParentFrontend->standby ) {
        return NEXUS_Frontend_Standby_priv(handle->pParentFrontend, enabled, pSettings);
    }
    else
    {
        /* Frontend does not have a standby api. This is a valid case */
        return NEXUS_SUCCESS;
    }
#else
    BSTD_UNUSED(handle);
    BSTD_UNUSED(enabled);
    BSTD_UNUSED(pSettings);
    return NEXUS_SUCCESS;
#endif
}

NEXUS_Error NEXUS_Frontend_GetTemperature(NEXUS_FrontendHandle handle, NEXUS_FrontendTemperature *pTemp)
{
    NEXUS_OBJECT_ASSERT(NEXUS_Frontend, handle);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getTemperature)
    {
        if ( handle->pParentFrontend )
        {
            return NEXUS_Frontend_GetTemperature(handle->pParentFrontend, pTemp);
        }
        else
        {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->getTemperature(handle->pDeviceHandle, pTemp);
    }
}

NEXUS_Error NEXUS_Frontend_TransmitDebugPacket(NEXUS_FrontendHandle handle, NEXUS_FrontendDebugPacketType type, const uint8_t *pBuffer, size_t size)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_Frontend_P_CheckDeviceOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->transmitDebugPacket)
    {
        if (handle->pParentFrontend)
        {
            return NEXUS_Frontend_TransmitDebugPacket(handle->pParentFrontend, type, pBuffer, size);
        }
        else
        {
            return BERR_TRACE(BERR_NOT_SUPPORTED);
        }
    }
    else
    {
        return handle->transmitDebugPacket(handle->pDeviceHandle, type, pBuffer, size);
    }
}

NEXUS_FrontendConnectorHandle NEXUS_Frontend_GetConnector( NEXUS_FrontendHandle handle )
{
    NEXUS_OBJECT_ASSERT(NEXUS_Frontend, handle);
    return handle->connector;
}

#if defined(NEXUS_FRONTEND_DOCSIS) && defined(NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT)
NEXUS_Error NEXUS_FrontendDevice_P_GetDocsisLnaDeviceAgcValue(NEXUS_FrontendDeviceHandle handle,
                                                              uint32_t *agcValue)
{
    BDBG_ASSERT(NULL != handle);
    if ( NULL == handle->getDocsisLnaDeviceAgcValue)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->getDocsisLnaDeviceAgcValue(handle->pDevice, agcValue);
    }
}
NEXUS_Error NEXUS_FrontendDevice_P_SetHostChannelLockStatus(NEXUS_FrontendDeviceHandle handle,
                                                            unsigned channelNum,bool locked)
{
    BDBG_ASSERT(NULL != handle);
    if ( NULL == handle->getDocsisLnaDeviceAgcValue)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->setHostChannelLockStatus(handle->pDevice,channelNum,locked);
    }
}
#endif

NEXUS_Error NEXUS_FrontendDevice_P_GetInternalGain(NEXUS_FrontendDeviceHandle handle, const NEXUS_GainParameters *params, NEXUS_InternalGainSettings *pSettings)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getInternalGain)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->getInternalGain(handle->pDevice, params, pSettings);
    }
}

NEXUS_Error NEXUS_FrontendDevice_P_SetExternalGain(NEXUS_FrontendDeviceHandle handle, const NEXUS_ExternalGainSettings *pSettings)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->setExternalGain)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->setExternalGain(handle->pDevice, pSettings);
    }
}

void NEXUS_FrontendDevice_GetDefaultLinkSettings(NEXUS_FrontendDeviceLinkSettings *pSettings)
{
    BDBG_ASSERT(NULL != pSettings);
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
    pSettings->rfInput = NEXUS_FrontendDeviceRfInput_eDaisy;
}

NEXUS_FrontendDeviceHandle NEXUS_Frontend_GetDevice(NEXUS_FrontendHandle handle)
{
    BDBG_ASSERT(NULL != handle);

    if(NULL == handle->pGenericDeviceHandle)
    {
        BDBG_WRN(("Device handle not initialized."));
        return NULL;
    }
    else
    {
        return handle->pGenericDeviceHandle;
    }

}

NEXUS_Error NEXUS_FrontendDevice_Link(NEXUS_FrontendDeviceHandle parentHandle, NEXUS_FrontendDeviceHandle childHandle, const NEXUS_FrontendDeviceLinkSettings *pSettings)
{
    NEXUS_Error rc=NEXUS_SUCCESS;

    BLST_D_INSERT_HEAD(&parentHandle->deviceChildList, childHandle, link);
    childHandle->parent = parentHandle;
    if (pSettings) {
        childHandle->linkSettings = *pSettings;

        if (pSettings->mtsif==NEXUS_FrontendDeviceMtsifOutput_eDaisy) {
            parentHandle->chainedConfig = &childHandle->mtsifConfig;
            childHandle->chainedConfig = &parentHandle->mtsifConfig;
            childHandle->mtsifConfig.slave = true;
        }
    }

    return rc;
}

void NEXUS_FrontendDevice_Unlink(NEXUS_FrontendDeviceHandle parentHandle,NEXUS_FrontendDeviceHandle childHandle)
{
    NEXUS_FrontendDeviceHandle tempChildHandle = NULL;
    if(childHandle){
        BLST_D_REMOVE(&parentHandle->deviceChildList, childHandle, link);
    }
    else {
        /* Remove all the children of this device. */
        for ( tempChildHandle = BLST_D_FIRST(&parentHandle->deviceChildList); NULL != tempChildHandle; tempChildHandle = BLST_D_NEXT(tempChildHandle, link) )
        {
            BLST_D_REMOVE(&parentHandle->deviceChildList, tempChildHandle, link);
            tempChildHandle->parent = NULL;
        }
    }

    /* Remove the device from its parent's list of children. */
    if(parentHandle->parent != NULL){
        BLST_D_REMOVE(&parentHandle->parent->deviceChildList, parentHandle, link);
    }
}

void  NEXUS_FrontendDevice_Close(NEXUS_FrontendDeviceHandle handle)
{
    BDBG_ASSERT(NULL != handle);

    if ( NULL == handle->close)
    {
        BERR_TRACE(BERR_NOT_SUPPORTED);
        return;
    }
    else
    {
        handle->close(handle->pDevice);
    }
}
NEXUS_Error NEXUS_FrontendDevice_GetStatus(NEXUS_FrontendDeviceHandle handle, NEXUS_FrontendDeviceStatus *pStatus)
{
    if(handle != NULL){
        if ( NULL == handle->getStatus)
        {
            BDBG_WRN(("Device status not supported for this frontend device."));
            return NEXUS_SUCCESS;
        }
        else
        {
            return handle->getStatus(handle->pDevice, pStatus);
        }
    }
    else{
        /* This is to handle the scenario where the device api is not supported. */
        BKNI_Memset(pStatus, 0, sizeof(*pStatus));
        return NEXUS_SUCCESS;
    }
}

NEXUS_Error NEXUS_FrontendDevice_P_Standby(NEXUS_FrontendDeviceHandle handle, const NEXUS_StandbySettings *pSettings)
{
    BDBG_ASSERT(NULL != handle);

    if((handle->mode != NEXUS_StandbyMode_eDeepSleep) && (NEXUS_FrontendDevice_P_CheckOpen(handle))){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->standby)
    {
        BDBG_WRN(("Device standby not supported for this frontend device."));
        return NEXUS_SUCCESS;
    }
    else
    {
        return handle->standby(handle->pDevice, pSettings);
    }
}

void NEXUS_FrontendDevice_GetDefaultRecalibrateSettings(NEXUS_FrontendDeviceRecalibrateSettings *pSettings)
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

NEXUS_Error NEXUS_FrontendDevice_Recalibrate(NEXUS_FrontendDeviceHandle handle, const NEXUS_FrontendDeviceRecalibrateSettings *pSettings)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->recalibrate)
    {
        BDBG_WRN(("Device recalibration not supported for this frontend device."));
        return NEXUS_SUCCESS;
    }
    else
    {
         return handle->recalibrate(handle->pDevice, pSettings);
    }
}

void NEXUS_FrontendDevice_GetCapabilities(NEXUS_FrontendDeviceHandle handle, NEXUS_FrontendDeviceCapabilities *pCapabilities)
{
    BDBG_ASSERT(NULL != handle);

    if (!handle->nonblocking.getCapabilities) {
        if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
            BDBG_ERR(("Device open failed. Cannot get device capabilities."));
            BERR_TRACE(NEXUS_NOT_INITIALIZED);
            return;
        }
    }

    if ( NULL == handle->getCapabilities)
    {
        BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        handle->getCapabilities(handle->pDevice, pCapabilities);
    }
}

NEXUS_Error NEXUS_FrontendDevice_GetSatelliteCapabilities(NEXUS_FrontendDeviceHandle handle, NEXUS_FrontendSatelliteCapabilities *pCapabilities)
{
    BDBG_ASSERT(NULL != handle);

    if (!handle->nonblocking.getSatelliteCapabilities) {
        if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
            BDBG_ERR(("Device open failed. Cannot get device satellite capabilities."));
            return BERR_TRACE(NEXUS_NOT_INITIALIZED);
        }
    }

    if ( NULL == handle->getSatelliteCapabilities)
    {
        return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    }
    else
    {
        return handle->getSatelliteCapabilities(handle->pDevice, pCapabilities);
    }
}

void NEXUS_FrontendDevice_GetTunerCapabilities(NEXUS_FrontendDeviceHandle handle, unsigned tunerIndex, NEXUS_FrontendCapabilities *pCapabilities)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        BDBG_ERR(("Device open failed. Cannot get device tuner capabilities.."));
        BERR_TRACE(NEXUS_NOT_INITIALIZED);
        return;
    }

    if ( NULL == handle->getTunerCapabilities)
    {
        BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        handle->getTunerCapabilities(handle->pDevice, tunerIndex, pCapabilities);
    }
}

NEXUS_Error NEXUS_FrontendDevice_LinkAmplifier(NEXUS_FrontendDeviceHandle handle, NEXUS_AmplifierHandle amplifierHandle)
{
    NEXUS_Error rc = NEXUS_SUCCESS;
    BDBG_ASSERT(NULL != handle);
    BDBG_ASSERT(NULL != amplifierHandle);
#if NEXUS_AMPLIFIER_SUPPORT
    handle->amplifier = amplifierHandle;
#endif
    return rc;
}

#if NEXUS_AMPLIFIER_SUPPORT
NEXUS_Error NEXUS_FrontendDevice_P_GetAmplifierStatus(NEXUS_FrontendDeviceHandle handle, NEXUS_AmplifierStatus *pStatus)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getAmplifierStatus)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->getAmplifierStatus(handle->pDevice, pStatus);
    }
}

NEXUS_Error NEXUS_FrontendDevice_P_SetAmplifierStatus(NEXUS_FrontendDeviceHandle handle, const NEXUS_AmplifierStatus *pStatus)
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->setAmplifierStatus)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->setAmplifierStatus(handle->pDevice, pStatus);
    }
}
#endif

NEXUS_Error NEXUS_FrontendDevice_P_CheckOpen(NEXUS_FrontendDeviceHandle handle)
{
    NEXUS_Error rc = NEXUS_SUCCESS;

iterate:
    if(handle->openFailed || handle->abortThread){
        rc = BERR_TRACE(NEXUS_NOT_INITIALIZED); goto done;
    }
    else if(handle->openPending){
        rc = BKNI_Sleep(50);
        if (rc) {rc = BERR_TRACE(rc); goto done;}
        goto iterate;
    } else if (handle->delayedInitializationRequired) {
        if (handle->delayedInit(handle) != NEXUS_SUCCESS) {
            rc = BERR_TRACE(NEXUS_NOT_INITIALIZED); goto done;
        }
        handle->delayedInitializationRequired = false;
    }
done:
    if(rc && (handle->openPending || handle->openFailed )){
        BDBG_WRN(("NEXUS_FrontendDevice_P_CheckOpen: Application aborted prematurely. Waiting for device initialization thread to exit."));
        handle->abortThread = true;
        NEXUS_FrontendDevice_Unlink(handle, NULL);
    }
    return rc;
}

NEXUS_Error NEXUS_Frontend_P_CheckDeviceOpen(NEXUS_FrontendHandle handle)
{
    NEXUS_Error rc = NEXUS_SUCCESS;
    NEXUS_FrontendDeviceHandle deviceHandle = NULL;

    deviceHandle = NEXUS_Frontend_GetDevice(handle);
    if(deviceHandle == NULL) goto done;

    rc = NEXUS_FrontendDevice_P_CheckOpen(deviceHandle);
done:
    return rc;
}

BDBG_FILE_MODULE(nexus_frontend_proc);
void NEXUS_FrontendModule_P_Print(void)
{
#if BDBG_DEBUG_BUILD
    NEXUS_FrontendHandle frontend;
    NEXUS_FrontendDeviceHandle device;
    unsigned index;
    NEXUS_Error errCode;
    bool handleFound = false;
    NEXUS_FrontendDeviceHandle tempHandle = NULL, deviceHandle = NULL;
    NEXUS_FrontendCapabilities frontendCapabilities;
    NEXUS_FrontendQamStatus *pQamStatus = NULL;
    NEXUS_FrontendSatelliteStatus *pSatStatus = NULL;
    NEXUS_FrontendVsbStatus *pVsbStatus = NULL;
    NEXUS_FrontendOfdmStatus *pOfdmStatus = NULL;

    pQamStatus = BKNI_Malloc(sizeof(*pQamStatus));
    if (NULL == pQamStatus) {BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY); goto done;}
    pSatStatus = BKNI_Malloc(sizeof(*pSatStatus));
    if (NULL == pSatStatus) {BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY); goto done;}
    pVsbStatus = BKNI_Malloc(sizeof(*pVsbStatus));
    if (NULL == pVsbStatus) {BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY); goto done;}
    pOfdmStatus = BKNI_Malloc(sizeof(*pOfdmStatus));
    if (NULL == pOfdmStatus) {BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY); goto done;}

    BLST_D_INIT(&g_frontendDeviceList.deviceList);

    /* TODO: frontend list should be moved per device */
    for (frontend = BLST_SQ_FIRST(&g_frontendList.frontends), index = 0; frontend; frontend = BLST_SQ_NEXT(frontend, link), index++) {
        handleFound = false;
        deviceHandle = NEXUS_Frontend_GetDevice(frontend);
         if(deviceHandle != NULL){
            for (tempHandle = BLST_D_FIRST(&g_frontendDeviceList.deviceList); tempHandle; tempHandle = BLST_D_NEXT(tempHandle, node)) {
                if(tempHandle == deviceHandle){
                    handleFound = true;
                }

            }
            if(!handleFound){
                BLST_D_INSERT_HEAD(&g_frontendDeviceList.deviceList, deviceHandle, node);

            }
        }
        BDBG_MODULE_LOG(nexus_frontend_proc, ("frontend %d: %p, acquired %c", index, (void *)frontend, frontend->acquired?'y':'n'));
        NEXUS_Frontend_GetCapabilities(frontend, &frontendCapabilities);
        if (frontend->acquired && frontendCapabilities.qam) {
            NEXUS_FrontendQamStatus *pStatus = pQamStatus;
            int modulation;
            BDBG_ASSERT(pStatus);

            BDBG_MODULE_LOG(nexus_frontend_proc, (" Cable/QAM"));
            /* must use sync call to for status, can't modify the application async callback setup */
            errCode = NEXUS_Frontend_GetQamStatus(frontend, pStatus);
            if (errCode) {
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    Error reading status, errCode=%d", errCode));
                continue;
            }
            modulation = 0;
            if (pStatus->settings.mode != NEXUS_FrontendQamMode_eAuto_64_256) {
                modulation = 16 << pStatus->settings.mode; /* use as multiple by 2 */
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    freq=%d hz, modulation=Qam-%d, annex=%c",
                    pStatus->settings.frequency, modulation, pStatus->settings.annex+'A'));
            }
            else {
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    freq=%dhz, modulation=auto, annex=%c",
                    pStatus->settings.frequency, pStatus->settings.annex+'A'));
            }
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    lockStatus=%s, snr=%ddB (est), fecCorrected=%d, fecUncorrected=%d",
                (pStatus->fecLock)? "locked": "unlocked",
                pStatus->snrEstimate/100, pStatus->fecCorrected, pStatus->fecUncorrected));
        }
        if (frontend->acquired && frontendCapabilities.satellite) {
            NEXUS_FrontendSatelliteStatus *pStatus = pSatStatus;
            int modulation;
            static const char *satModulation[] = {"dss", "dcii", "qpskTurbo", "8pskTubro", "turbo", "qpskLdpc", "8pskLdpc", "ldpc", "blindAcquistion", "unknown" };
            BDBG_ASSERT(pStatus);

            /* must use sync call to for status, can't modify the application async callback setup */
            BDBG_MODULE_LOG(nexus_frontend_proc, (" Satellite"));
            errCode = NEXUS_Frontend_GetSatelliteStatus(frontend, pStatus);
            if (errCode) {
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    Error reading status, errCode=%d", errCode));
                continue;
            }
            modulation = (pStatus->mode < (sizeof(satModulation)/sizeof(char *))? pStatus->mode: ((sizeof(satModulation)/sizeof(char *))-1));
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    freq=%dhz, modulation=%s, codeRate=%d/%d %s",
                pStatus->settings.frequency, satModulation[modulation],
                pStatus->settings.codeRate.numerator, pStatus->settings.codeRate.denominator,
                (pStatus->settings.bertEnable)? "bert":""));
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    lockStatus=%s, snr=%ddB (est), fecCorrected=%d, fecUncorrected=%d",
                (pStatus->demodLocked)? "locked": "unlocked",
                pStatus->snrEstimate/100, pStatus->fecCorrected, pStatus->fecUncorrected));
        }
        if (frontend->acquired && frontendCapabilities.vsb) {
            NEXUS_FrontendVsbStatus *pStatus = pVsbStatus;
            BDBG_ASSERT(pStatus);

            /* must use sync call to for status, can't modify the application async callback setup */
            BDBG_MODULE_LOG(nexus_frontend_proc, (" ATSC/VSB"));
            errCode = NEXUS_Frontend_GetVsbStatus(frontend, pStatus);
            if (errCode) {
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    Error reading status, errCode=%d", errCode));
                continue;
            }
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    freq=%dhz, modulation=%d",
                pStatus->settings.frequency, (pStatus->settings.mode == NEXUS_FrontendVsbMode_e8)? 8: 16));
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    lockStatus=%s, snr=%ddB (est), fecCorrected=%d, fecUncorrected=%d",
                (pStatus->fecLock)? "locked": "unlocked",
                pStatus->snrEstimate/100, pStatus->fecCorrected, pStatus->fecUncorrected));
        }
        if (frontend->acquired && frontendCapabilities.ofdm) {
            NEXUS_FrontendOfdmStatus *pStatus = pOfdmStatus;
            int mode, modulation;
            static const char *ofdmModulation[] = {"qpsk", "Qam-16", "Qam-64", "Qam-256", "dqpsk", "unknown" };
            static const char *ofdmMode[] = {"DVB-T", "DVB-T2", "DVB-C2", "ISDBT", "unknown" };
            BDBG_ASSERT(pStatus);

            /* must use sync call to for status, can't modify the application async callback setup */
            errCode = NEXUS_Frontend_GetOfdmStatus(frontend, pStatus);
            if (errCode) {
                BDBG_MODULE_LOG(nexus_frontend_proc, (" OFDM"));
                BDBG_MODULE_LOG(nexus_frontend_proc, ("    Error reading status, errCode=%d", errCode));
                pStatus = NULL;
                continue;
            }
            mode = (pStatus->settings.mode < (sizeof(ofdmMode)/sizeof(char *)))? pStatus->settings.mode: ((sizeof(ofdmMode)/sizeof(char *))-1);
            modulation = (pStatus->modulation < (sizeof(ofdmModulation)/sizeof(char *)))? pStatus->modulation: ((sizeof(ofdmModulation)/sizeof(char *)-1));
            BDBG_MODULE_LOG(nexus_frontend_proc, (" %s", ofdmMode[mode]));
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    freq=%dhz, modulation=%s",
                pStatus->settings.frequency, ofdmModulation[modulation]));
            BDBG_MODULE_LOG(nexus_frontend_proc, ("    lockStatus=%s, snr=%ddB, fecCorrected=%d, fecUncorrected=%d",
                (pStatus->fecLock)? "locked": "unlocked",
                pStatus->snr/100, pStatus->fecCorrectedBlocks, pStatus->fecUncorrectedBlocks));
        }
    }

    for (device = BLST_D_FIRST(&g_frontendDeviceList.deviceList); device; device = BLST_D_NEXT(device, link)) {
        BDBG_MODULE_LOG(nexus_frontend_proc, ("device %#x: %p", device->familyId, (void *)device));

        if (device->parent) {
            BDBG_MODULE_LOG(nexus_frontend_proc, ("  parent %#x: %p", device->parent->familyId, (void *)device->parent));
        }
#if NEXUS_HAS_MXT
        {
            unsigned j;
            NEXUS_FrontendDeviceMtsifConfig *pConfig = &device->mtsifConfig;
            for (j=0; j<pConfig->numDemodPb; j++) {
                if (pConfig->demodPbSettings[j].enabled) {
                    /* lookup frontend using connector */
                    for (frontend = BLST_SQ_FIRST(&g_frontendList.frontends); frontend; frontend = BLST_SQ_NEXT(frontend, link)) {
                        if (frontend->connector == pConfig->demodPbSettings[j].connector) break;
                    }
                    BDBG_MODULE_LOG(nexus_frontend_proc, ("  frontend %p -> IB%u -> PB%u -> PB%u",
                        (void *)frontend,
                        pConfig->demodPbSettings[j].inputBandNum,
                        j,
                        pConfig->demodPbSettings[j].virtualNum));
                }
            }
        }
#endif
    }
done:
    for (tempHandle = BLST_D_FIRST(&g_frontendDeviceList.deviceList); tempHandle; tempHandle = BLST_D_NEXT(tempHandle, node)) {
        BLST_D_REMOVE(&g_frontendDeviceList.deviceList, tempHandle, node);
    }

    if (pQamStatus) { BKNI_Free(pQamStatus); }
    if (pSatStatus) { BKNI_Free(pSatStatus); }
    if (pVsbStatus) { BKNI_Free(pVsbStatus); }
    if (pOfdmStatus) { BKNI_Free(pOfdmStatus); }
#endif
}


#if NEXUS_HAS_SPI
#include "nexus_spi.h"
#endif

NEXUS_Error NEXUS_FrontendDevice_Probe(const NEXUS_FrontendDeviceOpenSettings *pSettings, NEXUS_FrontendProbeResults *pResults)
{
    NEXUS_Error rc = NEXUS_SUCCESS;
    unsigned chipId=0;

    /* Read chipId based on i2c (if provided), SPI (if provided), or chip ID (if neither provided).
     * For i2c/spi, read two bytes. If the resulting id is below the lowest known frontend, read one more byte,
     * e.g. for 45216.
     */
    if (pSettings->i2cDevice) {
        BREG_I2C_Handle i2cHandle;
        uint8_t buf[5];
        uint8_t subAddr;

        i2cHandle = NEXUS_I2c_GetRegHandle(pSettings->i2cDevice, NULL);
        BDBG_MSG(("i2c handle: %p, i2cAddress: 0x%x",(void*)i2cHandle,pSettings->i2cAddress));
        buf[0]= 0x0;
        subAddr = 0x1;
        BREG_I2C_WriteNoAddr(i2cHandle, pSettings->i2cAddress, (uint8_t *)&subAddr, 1);
        BREG_I2C_ReadNoAddr(i2cHandle, pSettings->i2cAddress, buf, 1);
        chipId = buf[0];

        subAddr = 0x2;
        BREG_I2C_WriteNoAddr(i2cHandle, pSettings->i2cAddress, (uint8_t *)&subAddr, 1);
        BREG_I2C_ReadNoAddr(i2cHandle, pSettings->i2cAddress, buf, 1);
        chipId = (chipId << 8) | buf[0];

        if (chipId < 0x3000) {
            subAddr = 0x3;
            BREG_I2C_WriteNoAddr(i2cHandle, pSettings->i2cAddress, (uint8_t *)&subAddr, 1);
            BREG_I2C_ReadNoAddr(i2cHandle, pSettings->i2cAddress, buf, 1);
            chipId = (chipId << 8) | buf[0];
        }

        BDBG_MSG(("chip family ID = 0x%04x", chipId));
    }
#if NEXUS_HAS_SPI
    else if (pSettings->spiDevice) {
        uint8_t wData[2], rData[8];
        NEXUS_Error rc;

        BDBG_MSG(("Probing for 45216 at SPI %p",(void*)pSettings->spiDevice));

        wData[0] = 0x40;
        wData[1] = 0x00;
#if DEBUG_SPI_READS
        {
            int i;
            for (i=0; i < 2; i++) {
                BDBG_MSG(("wData[%d]: 0x%02x",i,wData[i]));
            }
        }
#endif

        rc = NEXUS_Spi_Read(pSettings->spiDevice, wData, rData, 8);
        if(rc) {rc = BERR_TRACE(rc);}

#if DEBUG_SPI_READS
        {
            int i;
            for (i=0; i < 8; i++) {
                BDBG_MSG(("rData[%d]: 0x%02x",i,rData[i]));
            }
        }
#endif

        chipId = (rData[3] << 8) | (rData[4]);
        if (chipId < 0x3000) {
            chipId = (chipId << 8) | (rData[5]);
        }

        BDBG_MSG(("chip family ID = 0x%04x", chipId));

    }
#endif
    else {
        BCHP_Info chipInfo;
        BCHP_GetInfo(g_pCoreHandles->chp, &chipInfo);
        chipId = chipInfo.familyId;
    }

    if (chipId == 0xffff)
        chipId = 0;
    pResults->chip.familyId = chipId;

    return rc;
}

void NEXUS_FrontendDevice_GetDefaultOpenSettings(NEXUS_FrontendDeviceOpenSettings *pSettings)
{
    int i;
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
    for (i=0; i < NEXUS_MAX_MTSIF; i++) {
        pSettings->mtsif[i].enabled = true;
    }
}

NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_Open(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings)
{
    unsigned i;
    NEXUS_FrontendProbeResults probe;

    if (!NEXUS_FrontendDevice_Probe(pSettings, &probe)) {
        for (i=0;g_frontends[i].chipid;i++) {
            BDBG_MSG(("NEXUS_FrontendDevice_Open: %x %x | %d %d | %p",g_frontends[i].chipid,probe.chip.familyId,g_frontends[i].chipid,probe.chip.familyId,g_frontends[i].openDevice));
            if (g_frontends[i].chipid == probe.chip.familyId) {
                return g_frontends[i].openDevice(index, pSettings);
            }
        }
    }
    BERR_TRACE(NEXUS_NOT_SUPPORTED);
    return NULL;
}

void NEXUS_FrontendDevice_GetDefaultSettings(
    NEXUS_FrontendDeviceSettings *pSettings   /* [out] */
    )
{
    BDBG_ASSERT(NULL != pSettings);
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

NEXUS_Error NEXUS_FrontendDevice_GetSettings(
    NEXUS_FrontendDeviceHandle handle,
    NEXUS_FrontendDeviceSettings *pSettings    /* [out] */
    )
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getSettings)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->getSettings(handle->pDevice, pSettings);
    }
}

NEXUS_Error NEXUS_FrontendDevice_SetSettings(
    NEXUS_FrontendDeviceHandle handle,
    const NEXUS_FrontendDeviceSettings *pSettings
    )
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->setSettings)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->setSettings(handle->pDevice, pSettings);
    }
}

NEXUS_Error NEXUS_FrontendDevice_GetAmplifierStatus(
    NEXUS_FrontendDeviceHandle handle,
    NEXUS_FrontendDeviceAmplifierStatus *pStatus
    )
{
    BDBG_ASSERT(NULL != handle);

    if(NEXUS_FrontendDevice_P_CheckOpen(handle)){
        return BERR_TRACE(NEXUS_NOT_INITIALIZED);
    }

    if ( NULL == handle->getDeviceAmplifierStatus)
    {
        return BERR_TRACE(BERR_NOT_SUPPORTED);
    }
    else
    {
        return handle->getDeviceAmplifierStatus(handle->pDevice, pStatus);
    }
}

void NEXUS_Frontend_GetDefaultOpenSettings(NEXUS_FrontendChannelSettings *pSettings)
{
    BKNI_Memset(pSettings, 0, sizeof(*pSettings));
}

NEXUS_FrontendHandle NEXUS_Frontend_Open(const NEXUS_FrontendChannelSettings *pSettings)
{
    unsigned i;

    if (pSettings->device->familyId) {
        for (i=0;g_frontends[i].chipid;i++) {
            if (g_frontends[i].chipid == pSettings->device->familyId) {
                return g_frontends[i].openChannel(pSettings);
            }
        }
    }
    BERR_TRACE(NEXUS_NOT_SUPPORTED);
    return NULL;
}
