/******************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *****************************************************************************/

#ifndef _ZIGBEE_API_H_
#define _ZIGBEE_API_H_

#include "zigbee_rf4ce_registration.h"

/******************************************************************************
    Zigbee Request functions runs from the caller thread
******************************************************************************/
typedef uint8_t NoAppropriateType_t;

/* 7.1 */
typedef enum
{
    /* Completed successfully. */
    BROADBEE_NO_ERROR,
    /* Address is out of range. */
    BROADBEE_ADDRESS_OUT_OF_RANGE,
    /* Access has been denied. */
    BROADBEE_ACCESS_DENIED,
    /* Access has taken too long time. */
    BROADBEE_ACCESS_TIME_OUT,
    /* Data is not available. */
    BROADBEE_DATA_NOT_AVAILABLE
} BroadBee_HostAccessResult_t;

/* 7.3 */

typedef struct _BroadBee_DataInHost_t
{
    /* Filename Index */
    uint8_t FilenameIndex;
    /* 32-bit address offset from the beginning of the file in Host */
    uint32_t address;
    /* Length to read in byte unit */
    uint32_t length;
    /* Pointer to actual data read from or to be written to Host */
    uint8_t *hostData;
} BroadBee_DataInHost_t;

typedef struct _BroadBee_ReadFileFromHostInd_t
{
    BroadBee_DataInHost_t dataFromHost;
    void (*BroadBee_ReadFromHostResp)( struct _BroadBee_ReadFileFromHostInd_t *readFileFromHostIndParam, BroadBee_DataInHost_t *returnData);
} BroadBee_ReadFileFromHostInd_t;

typedef struct _BroadBee_WriteFileToHostInd_t
{
    BroadBee_DataInHost_t dataToHost;
    void (*BroadBee_WriteToHostResp)( struct _BroadBee_WriteFileToHostInd_t *writeFileToHostIndParam, BroadBee_HostAccessResult_t *result);
} BroadBee_WriteFileToHostInd_t;

void BroadBee_ReadFileFromHostInd( BroadBee_ReadFileFromHostInd_t *readFileFromHostParam);
void BroadBee_WriteFileToHostInd( BroadBee_WriteFileToHostInd_t *writeFileToHostParam);

typedef struct zigbeeCallback {
#ifdef _RF4CE_
    void (*RF4CE_PairInd)(RF4CE_PairingIndParams_t *indication);
    void (*RF4CE_ZRC2_CheckValidationInd)(RF4CE_ZRC2_CheckValidationIndParams_t *indication);
    void (*RF4CE_ZRC2_ControlCommandInd)(RF4CE_ZRC2_ControlCommandIndParams_t *indication);
    void (*RF4CE_ZRC1_ControlCommandInd)(RF4CE_ZRC1_ControlCommandIndParams_t *indication);
    void (*RF4CE_ZRC1_VendorSpecificInd)(RF4CE_ZRC1_VendorSpecificIndParams_t *indication);
#endif  // _RF4CE_
    void (*SYS_EventNtfy)(SYS_EventNotifyParams_t *event);
    void (*Mail_UartRecvInd)(Mail_UartRecvIndDescr_t *indication);
#ifdef _ZBPRO_
    void (*ZBPRO_ZDO_MgmtNwkUpdateUnsolInd)(ZBPRO_ZDO_MgmtNwkUpdateUnsolIndParams_t *indication);
    void (*ZBPRO_ZDO_DeviceAnnceInd)(ZBPRO_ZDO_DeviceAnnceIndParams_t *indication);
    void (*ZBPRO_ZCL_IdentifyInd)(ZBPRO_ZCL_IdentifyIndParams_t *indication);
    void (*ZBPRO_ZCL_ProfileWideCmdReportAttributesInd)(ZBPRO_ZCL_ProfileWideCmdReportAttributesIndParams_t *indication);
    void (*ZBPRO_ZCL_GroupsCmdGetGroupMembershipResponseInd)(ZBPRO_ZCL_GroupsCmdGetGroupMembershipIndParams_t *indication);
    void (*ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseInd)(ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseIndParams_t *indication);
    void (*ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestInd)(ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestIndParams_t *indication);
    void (*ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationInd)(ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceArmInd)(ZBPRO_ZCL_SapIasAceArmIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceBypassInd)(ZBPRO_ZCL_SapIasAceBypassIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceEmergencyInd)(ZBPRO_ZCL_SapIasAceAlarmIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceFireInd)(ZBPRO_ZCL_SapIasAceAlarmIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAcePanicInd)(ZBPRO_ZCL_SapIasAceAlarmIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceGetZoneIdMapInd)(ZBPRO_ZCL_SapIasAceGetZoneIdMapIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceGetZoneInfoInd)(ZBPRO_ZCL_SapIasAceGetZoneInfoIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceGetPanelStatusInd)(ZBPRO_ZCL_SapIasAceGetPanelStatusIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceGetBypassedZoneListInd)(ZBPRO_ZCL_SapIasAceGetBypassedZoneListIndParams_t *indication);
    void (*ZBPRO_ZCL_SapIasAceGetZoneStatusInd)(ZBPRO_ZCL_SapIasAceGetZoneStatusIndParams_t *indication);
    void (*ZBPRO_ZHA_CieDeviceSetPanelStatusInd)(ZBPRO_ZHA_CieSetPanelStatusIndParams_t *indication);
#endif  // _ZBPRO_
    void (*ServerLoopbackInd)(unsigned int *tx_buffer, unsigned int *rx_buffer, unsigned int num_of_words, void (*callback)(unsigned int *tx_buffer, unsigned int *rx_buffer, unsigned int num_of_words));
    void (*BufferCompare)(unsigned int *buf, unsigned int num_of_words);
    void (*ZigbeeError)(void);
} zigbeeCallback;


#define DECLARE_SERVER_REQUEST_API_FUNCTION(name, reqType, confType)    \
    extern void server_##name(unsigned int *buf, int socket);           \
    extern void server_##name##_callback(reqType  *request, confType *conf);

#define DECLARE_NON_RPC_REQUEST_API_FUNCTION(name, reqType, confType)    \
    extern void name(reqType *request); \
    extern void non_rpc_##name##_callback(reqType  *request, confType *conf);

#define DECLARE_SERVER_INDICATION_API_FUNCTION(name, indType, respType) \
    extern void server_##name(indType *indication, int socket);         \
    extern void server_##name##_callback(unsigned int *buf, int socket);\
    extern void server_##name##_non_rpc(indType *indication, int socket);

#define DECLARE_CLIENT_REQUEST_API_FUNCTION(name, reqType, confType)    \
    extern void name(reqType *request);                                 \
    extern void client_##name##_callback(unsigned int *message_rx);

#define DECLARE_CLIENT_INDICATION_API_FUNCTION(name, indType, respType) \
    extern void client_##name(unsigned int *message_rx);                \
    extern void client_##name##_callback(indType *ind, respType *resp);

#define DECLARE_SERVER_LOCAL_REQUEST_API_FUNCTION(name, reqType, confType)      \
    extern void server_##name##_local_call(reqType *req);                       \
    extern void server_##name##_local_callback(reqType  *request, confType *conf);

#ifdef SERVER

void rf4ce_ZRC2_Set_Default_Check_Validation_Period(uint8_t pairingRef);

DECLARE_SERVER_REQUEST_API_FUNCTION(Mail_TestEngineReset, TE_ResetCommandReqDescr_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(SYS_EventNtfy, SYS_EventNotifyParams_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Get_FW_Rev_Req, Get_FW_Rev_ReqDescr_t, Get_FW_Rev_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_MAC_GetReq, MAC_GetReqDescr_t, MAC_GetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_MAC_GetReq, MAC_GetReqDescr_t, MAC_GetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_MAC_SetReq, MAC_SetReqDescr_t, MAC_SetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ResetReq, RF4CE_ResetReqDescr_t, RF4CE_StartResetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_RegisterVirtualDevice, RF4CE_RegisterVirtualDeviceReqDescr_t, RF4CE_RegisterVirtualDeviceConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_StartReq, RF4CE_StartReqDescr_t, RF4CE_StartResetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_SetSupportedDevicesReq, RF4CE_SetSupportedDevicesReqDescr_t, RF4CE_SetSupportedDevicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_NWK_DataReq, RF4CE_NWK_DataReqDescr_t, RF4CE_NWK_DataConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_NWK_GetReq, RF4CE_NWK_GetReqDescr_t, RF4CE_NWK_GetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_NWK_SetReq, RF4CE_NWK_SetReqDescr_t, RF4CE_NWK_SetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC1_GetAttributesReq, RF4CE_ZRC1_GetAttributeDescr_t, RF4CE_ZRC1_GetAttributeConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC1_SetAttributesReq, RF4CE_ZRC1_SetAttributeDescr_t, RF4CE_ZRC1_SetAttributeConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Mail_TestEngineEcho, TE_EchoCommandReqDescr_t, TE_EchoCommandConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(sysEventSubscribeHostHandler, SYS_EventHandlerMailParams_t, NoAppropriateType_t)

//DECLARE_SERVER_REQUEST_API_FUNCTION(Mail_SetEchoDelay, TE_SetEchoDelayCommandReqDescr_t, TE_SetEchoDelayCommandConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC_SetWakeUpActionCodeReq, RF4CE_ZRC_SetWakeUpActionCodeReqDescr_t, RF4CE_ZRC_SetWakeUpActionCodeConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC_GetWakeUpActionCodeReq, RF4CE_ZRC_GetWakeUpActionCodeReqDescr_t, RF4CE_ZRC_GetWakeUpActionCodeConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(RF4CE_PairInd, RF4CE_PairingIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(RF4CE_ZRC2_CheckValidationInd, RF4CE_ZRC2_CheckValidationIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(RF4CE_ZRC2_ControlCommandInd, RF4CE_ZRC2_ControlCommandIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(RF4CE_ZRC1_ControlCommandInd, RF4CE_ZRC1_ControlCommandIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC1_VendorSpecificReq, RF4CE_ZRC1_VendorSpecificReqDescr_t, RF4CE_ZRC1_VendorSpecificConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(RF4CE_ZRC1_VendorSpecificInd, RF4CE_ZRC1_VendorSpecificIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC2_SetPushButtonStimulusReq, RF4CE_ZRC2_ButtonBindingReqDescr_t, RF4CE_ZRC2_BindingConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC2_EnableBindingReq, RF4CE_ZRC2_BindingReqDescr_t, RF4CE_ZRC2_BindingConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC2_SetAttributesReq, RF4CE_ZRC2_SetAttributesReqDescr_t, RF4CE_ZRC2_SetAttributesConfParams_t)

DECLARE_SERVER_LOCAL_REQUEST_API_FUNCTION(RF4CE_ZRC2_SetAttributesReq, RF4CE_ZRC2_SetAttributesReqDescr_t, RF4CE_ZRC2_SetAttributesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC2_CheckValidationResp, RF4CE_ZRC2_CheckValidationRespDescr_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_ZRC1_TargetBindReq, RF4CE_ZRC1_BindReqDescr_t, RF4CE_ZRC1_BindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_UnpairReq, RF4CE_UnpairReqDescr_t, RF4CE_UnpairConfParams_t)

DECLARE_NON_RPC_REQUEST_API_FUNCTION(RF4CE_UnpairReq, RF4CE_UnpairReqDescr_t, RF4CE_UnpairConfParams_t)

//DECLARE_SERVER_REQUEST_API_FUNCTION(Mail_Host2Uart1, TE_Host2Uart1ReqDescr_t,   NoAppropriateType_t)

#ifdef _PHY_TEST_HOST_INTERFACE_

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Get_Caps_Req, Phy_Test_Get_Caps_ReqDescr_t, Phy_Test_Get_Caps_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Set_Channel_Req, Phy_Test_Set_Channel_ReqDescr_t, Phy_Test_Set_Channel_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Continuous_Wave_Start_Req, Phy_Test_Continuous_Wave_Start_ReqDescr_t, Phy_Test_Continuous_Wave_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Continuous_Wave_Stop_Req, Phy_Test_Continuous_Wave_Stop_ReqDescr_t, Phy_Test_Continuous_Wave_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Transmit_Start_Req, Phy_Test_Transmit_Start_ReqDescr_t, Phy_Test_Transmit_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Transmit_Stop_Req, Phy_Test_Transmit_Stop_ReqDescr_t, Phy_Test_Transmit_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Receive_Start_Req, Phy_Test_Receive_Start_ReqDescr_t, Phy_Test_Receive_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Receive_Stop_Req, Phy_Test_Receive_Stop_ReqDescr_t, Phy_Test_Receive_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Echo_Start_Req, Phy_Test_Echo_Start_ReqDescr_t, Phy_Test_Echo_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Echo_Stop_Req, Phy_Test_Echo_Stop_ReqDescr_t, Phy_Test_Echo_StartStop_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Energy_Detect_Scan_Req, Phy_Test_Energy_Detect_Scan_ReqDescr_t, Phy_Test_Energy_Detect_Scan_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Get_Stats_Req, Phy_Test_Get_Stats_ReqDescr_t, Phy_Test_Get_Stats_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Reset_Stats_Req, Phy_Test_Reset_Stats_ReqDescr_t, Phy_Test_Reset_Stats_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_Set_TX_Power_Req, Phy_Test_Set_TX_Power_ReqDescr_t, Phy_Test_Set_TX_Power_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(Phy_Test_SelectAntenna_Req, Phy_Test_Select_Antenna_ReqDescr_t, Phy_Test_Select_Antenna_ConfParams_t)


DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_Get_Diag_Caps_Req, RF4CE_Diag_Caps_ReqDescr_t, RF4CE_Diag_Caps_ConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(RF4CE_Get_Diag_Req, RF4CE_Diag_ReqDescr_t, RF4CE_Diag_ConfParams_t)

#endif  // _PHY_TEST_HOST_INTERFACE_


#ifdef _ZBPRO_

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_NWK_PermitJoiningReq, ZBPRO_NWK_PermitJoiningReqDescr_t, ZBPRO_NWK_PermitJoiningConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_NWK_LeaveReq, ZBPRO_NWK_LeaveReqDescr_t, ZBPRO_NWK_LeaveConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_NWK_GetKeyReq, ZBPRO_NWK_GetKeyReqDescr_t, ZBPRO_NWK_GetKeyConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_NWK_SetKeyReq, ZBPRO_NWK_SetKeyReqDescr_t, ZBPRO_NWK_SetKeyConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_NWK_RouteDiscoveryReq, ZBPRO_NWK_RouteDiscoveryReqDescr_t, ZBPRO_NWK_RouteDiscoveryConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_EndpointRegisterReq, ZBPRO_APS_EndpointRegisterReqDescr_t, ZBPRO_APS_EndpointRegisterConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_EndpointUnregisterReq, ZBPRO_APS_EndpointUnregisterReqDescr_t, ZBPRO_APS_EndpointRegisterConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_DataReq, ZBPRO_APS_DataReqDescr_t, ZBPRO_APS_DataConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_BindReq, ZBPRO_APS_BindUnbindReqDescr_t, ZBPRO_APS_BindUnbindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_UnbindReq, ZBPRO_APS_BindUnbindReqDescr_t, ZBPRO_APS_BindUnbindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_GetReq, ZBPRO_APS_GetReqDescr_t, ZBPRO_APS_GetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_SetReq, ZBPRO_APS_SetReqDescr_t, ZBPRO_APS_SetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_GetKeyReq, ZBPRO_APS_GetKeyReqDescr_t, ZBPRO_APS_GetKeyConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_SetKeyReq, ZBPRO_APS_SetKeyReqDescr_t, ZBPRO_APS_SetKeyConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_AddGroupReq, ZBPRO_APS_AddGroupReqDescr_t, ZBPRO_APS_AddGroupConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveGroupReq, ZBPRO_APS_RemoveGroupReqDescr_t, ZBPRO_APS_RemoveGroupConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveAllGroupsReq, ZBPRO_APS_RemoveAllGroupsReqDescr_t, ZBPRO_APS_RemoveAllGroupsConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_TransportKeyReq, ZBPRO_APS_TransportKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_UpdateDeviceReq, ZBPRO_APS_UpdateDeviceReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveDeviceReq, ZBPRO_APS_RemoveDeviceReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_RequestKeyReq, ZBPRO_APS_RequestKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_APS_SwitchKeyReq, ZBPRO_APS_SwitchKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_AddrResolvingReq, ZBPRO_ZDO_AddrResolvingReqDescr_t, ZBPRO_ZDO_AddrResolvingConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_NodeDescReq, ZBPRO_ZDO_NodeDescReqDescr_t, ZBPRO_ZDO_NodeDescConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_PowerDescReq, ZBPRO_ZDO_PowerDescReqDescr_t, ZBPRO_ZDO_PowerDescConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_SimpleDescReq, ZBPRO_ZDO_SimpleDescReqDescr_t, ZBPRO_ZDO_SimpleDescConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_ActiveEpReq, ZBPRO_ZDO_ActiveEpReqDescr_t, ZBPRO_ZDO_ActiveEpConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MatchDescReq, ZBPRO_ZDO_MatchDescReqDescr_t, ZBPRO_ZDO_MatchDescConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_DeviceAnnceReq, ZBPRO_ZDO_DeviceAnnceReqDescr_t, ZBPRO_ZDO_DeviceAnnceConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_EndDeviceBindReq, ZBPRO_ZDO_EndDeviceBindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_BindReq, ZBPRO_ZDO_BindUnbindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_UnbindReq, ZBPRO_ZDO_BindUnbindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_StartNetworkReq, ZBPRO_ZDO_StartNetworkReqDescr_t, ZBPRO_ZDO_StartNetworkConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtLeaveReq, ZBPRO_ZDO_MgmtLeaveReqDescr_t, ZBPRO_ZDO_MgmtLeaveConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtPermitJoiningReq, ZBPRO_ZDO_MgmtPermitJoiningReqDescr_t, ZBPRO_ZDO_MgmtPermitJoiningConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateReq, ZBPRO_ZDO_MgmtNwkUpdateReqDescr_t, ZBPRO_ZDO_MgmtNwkUpdateConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateUnsolResp, ZBPRO_ZDO_MgmtNwkUpdateUnsolRespDescr_t, ZBPRO_ZDO_MgmtNwkUpdateUnsolConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtLqiReq, ZBPRO_ZDO_MgmtLqiReqDescr_t, ZBPRO_ZDO_MgmtLqiConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtBindReq, ZBPRO_ZDO_MgmtBindReqDescr_t, ZBPRO_ZDO_MgmtBindConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateUnsolInd, ZBPRO_ZDO_MgmtNwkUpdateUnsolIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZDO_DeviceAnnceInd, ZBPRO_ZDO_DeviceAnnceIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_TC_NwkKeyUpdateReq, ZBPRO_TC_NwkKeyUpdateReqDescr_t, ZBPRO_TC_NwkKeyUpdateConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SetPowerSourceReq, ZBPRO_ZCL_SetPowerSourceReqDescr_t, ZBPRO_ZCL_SetPowerSourceConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdDiscoverAttrReq, ZBPRO_ZCL_ProfileWideCmdDiscoverAttrDescr_t, ZBPRO_ZCL_ProfileWideCmdDiscoverAttrConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReadAttributesReq, ZBPRO_ZCL_ProfileWideCmdReadAttrReqDescr_t, ZBPRO_ZCL_ProfileWideCmdReadAttrConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdWriteAttributesReq, ZBPRO_ZCL_ProfileWideCmdWriteAttrReqDescr_t, ZBPRO_ZCL_ProfileWideCmdWriteAttrConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdConfigureReportingReq, ZBPRO_ZCL_ProfileWideCmdConfigureReportingReqDescr_t, ZBPRO_ZCL_ProfileWideCmdConfigureReportingConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationReq, ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationReqDescr_t, ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_IdentifyCmdIdentifyReq, ZBPRO_ZCL_IdentifyCmdIdentifyReqDescr_t, ZBPRO_ZCL_IdentifyCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_IdentifyCmdIdentifyQueryReq, ZBPRO_ZCL_IdentifyCmdIdentifyQueryReqDescr_t, ZBPRO_ZCL_IdentifyCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdAddGroupReq, ZBPRO_ZCL_GroupsCmdAddGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdAddGroupConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdViewGroupReq, ZBPRO_ZCL_GroupsCmdViewGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdViewGroupConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdGetGroupMembershipReq, ZBPRO_ZCL_GroupsCmdGetGroupMembershipReqDescr_t, ZBPRO_ZCL_GroupsCmdGetGroupMembershipConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdRemoveGroupReq, ZBPRO_ZCL_GroupsCmdRemoveGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdRemoveGroupConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdRemoveAllGroupsReq, ZBPRO_ZCL_GroupsCmdRemoveAllGroupsReqDescr_t, ZBPRO_ZCL_GroupsCmdRemoveAllGroupsConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyReq, ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyReqDescr_t, ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdAddSceneReq, ZBPRO_ZCL_ScenesCmdAddSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdAddSceneConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdViewSceneReq, ZBPRO_ZCL_ScenesCmdViewSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdViewSceneConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdStoreSceneReq, ZBPRO_ZCL_ScenesCmdStoreSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdStoreSceneConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRecallSceneReq, ZBPRO_ZCL_ScenesCmdRecallSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRemoveSceneReq, ZBPRO_ZCL_ScenesCmdRemoveSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdRemoveSceneConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRemoveAllScenesReq, ZBPRO_ZCL_ScenesCmdRemoveAllScenesReqDescr_t, ZBPRO_ZCL_ScenesCmdRemoveAllScenesConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdGetSceneMembershipReq, ZBPRO_ZCL_ScenesCmdGetSceneMembershipReqDescr_t, ZBPRO_ZCL_ScenesCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdOffReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdOnReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdToggleReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdMoveToLevelReq, ZBPRO_ZCL_LevelControlCmdMoveToLevelReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdMoveReq, ZBPRO_ZCL_LevelControlCmdMoveReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdStepReq, ZBPRO_ZCL_LevelControlCmdStepReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdStopReq, ZBPRO_ZCL_LevelControlCmdStopReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_DoorLockCmdLockReq, ZBPRO_ZCL_DoorLockCmdLockUnlockReqDescr_t, ZBPRO_ZCL_DoorLockCmdLockUnlockConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_DoorLockCmdUnlockReq, ZBPRO_ZCL_DoorLockCmdLockUnlockReqDescr_t, ZBPRO_ZCL_DoorLockCmdLockUnlockConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdUpOpenReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdDownCloseReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdStopReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdGotoLiftPecentageReq, ZBPRO_ZCL_WindowCoveringLiftTiltPercentCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdGotoTiltPecentageReq, ZBPRO_ZCL_WindowCoveringLiftTiltPercentCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseReq, ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseReqDescr_t, ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceArmRespReq, ZBPRO_ZCL_SapIasAceArmRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceBypassRespReq, ZBPRO_ZCL_SapIasAceBypassRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneIdMapRespReq, ZBPRO_ZCL_SapIasAceGetZoneIdMapRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneInfoRespReq, ZBPRO_ZCL_SapIasAceGetZoneInfoRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetPanelStatusRespReq, ZBPRO_ZCL_SapIasAceGetPanelStatusRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceSetBypassedZoneListRespReq, ZBPRO_ZCL_SapIasAceSetBypassedZoneListRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneStatusRespReq, ZBPRO_ZCL_SapIasAceGetZoneStatusRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceZoneStatusChangedReq, ZBPRO_ZCL_SapIasAceZoneStatusChangedReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAcePanelStatusChangedReq, ZBPRO_ZCL_SapIasAcePanelStatusChangedReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASWDCmdStartWarningReq, ZBPRO_ZCL_IASWDCmdStartWarningReqDescr_t, ZBPRO_ZCL_IASWDCmdStartWarningConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASWDCmdSquawkReq, ZBPRO_ZCL_IASWDCmdSquawkReqDescr_t, ZBPRO_ZCL_IASWDCmdSquawkConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_IdentifyInd, ZBPRO_ZCL_IdentifyIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReportAttributesInd, ZBPRO_ZCL_ProfileWideCmdReportAttributesIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_GroupsCmdGetGroupMembershipResponseInd, ZBPRO_ZCL_GroupsCmdGetGroupMembershipIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseInd, ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestInd, ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationInd, ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceArmInd, ZBPRO_ZCL_SapIasAceArmIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceBypassInd, ZBPRO_ZCL_SapIasAceBypassIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceEmergencyInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceFireInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAcePanicInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneIdMapInd, ZBPRO_ZCL_SapIasAceGetZoneIdMapIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneInfoInd, ZBPRO_ZCL_SapIasAceGetZoneInfoIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetPanelStatusInd, ZBPRO_ZCL_SapIasAceGetPanelStatusIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetBypassedZoneListInd, ZBPRO_ZCL_SapIasAceGetBypassedZoneListIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneStatusInd, ZBPRO_ZCL_SapIasAceGetZoneStatusIndParams_t, NoAppropriateType_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveToColorReq, ZBPRO_ZCL_ColorControlCmdMoveToColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveToColorConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveColorReq, ZBPRO_ZCL_ColorControlCmdMoveColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveColorConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStepColorReq, ZBPRO_ZCL_ColorControlCmdStepColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdStepColorConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedStepHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedStepHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedStepHueConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdColorLoopSetReq, ZBPRO_ZCL_ColorControlCmdColorLoopSetReqDescr_t, ZBPRO_ZCL_ColorControlCmdColorLoopSetConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStopMoveStepReq, ZBPRO_ZCL_ColorControlCmdStopMoveStepReqDescr_t, ZBPRO_ZCL_ColorControlCmdStopMoveStepConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureReq, ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStepColorTemperatureReq, ZBPRO_ZCL_ColorControlCmdStepColorTemperatureReqDescr_t, ZBPRO_ZCL_ColorControlCmdStepColorTemperatureConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZHA_EzModeReq, ZBPRO_ZHA_EzModeReqDescr_t, ZBPRO_ZHA_EzModeConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieDeviceEnrollReq, ZBPRO_ZHA_CieEnrollReqDescr_t, ZBPRO_ZHA_CieEnrollConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieDeviceSetPanelStatusReq, ZBPRO_ZHA_CieSetPanelStatusReqDescr_t, ZBPRO_ZHA_CieSetPanelStatusConfParams_t)

DECLARE_SERVER_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieZoneSetBypassStateReq, ZBPRO_ZHA_CieZoneSetBypassStateReqDescr_t, ZBPRO_ZHA_CieZoneSetBypassStateConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(ZBPRO_ZHA_CieDeviceSetPanelStatusInd, ZBPRO_ZHA_CieSetPanelStatusIndParams_t, NoAppropriateType_t)

#endif

DECLARE_SERVER_REQUEST_API_FUNCTION(Mail_UartSendReq,   Mail_UartSendReqDescr_t,           Mail_UartSendConfParams_t)

DECLARE_SERVER_INDICATION_API_FUNCTION(Mail_UartRecvInd,   Mail_UartRecvIndDescr_t,          Mail_UartRecvRespParams_t)

#else   // SERVER

DECLARE_CLIENT_REQUEST_API_FUNCTION(Get_FW_Rev_Req, Get_FW_Rev_ReqDescr_t, Get_FW_Rev_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_MAC_GetReq, MAC_GetReqDescr_t, MAC_GetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_MAC_SetReq, MAC_SetReqDescr_t, MAC_SetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ResetReq, RF4CE_ResetReqDescr_t, RF4CE_StartResetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_SetSupportedDevicesReq, RF4CE_SetSupportedDevicesReqDescr_t, RF4CE_SetSupportedDevicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_RegisterVirtualDevice, RF4CE_RegisterVirtualDeviceReqDescr_t, RF4CE_RegisterVirtualDeviceConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_StartReq, RF4CE_StartReqDescr_t, RF4CE_StartResetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_NWK_DataReq, RF4CE_NWK_DataReqDescr_t, RF4CE_NWK_DataConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_NWK_GetReq, RF4CE_NWK_GetReqDescr_t, RF4CE_NWK_GetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_NWK_SetReq, RF4CE_NWK_SetReqDescr_t, RF4CE_NWK_SetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC1_GetAttributesReq, RF4CE_ZRC1_GetAttributeDescr_t, RF4CE_ZRC1_GetAttributeConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC1_SetAttributesReq, RF4CE_ZRC1_SetAttributeDescr_t, RF4CE_ZRC1_SetAttributeConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Mail_TestEngineEcho, TE_EchoCommandReqDescr_t, TE_EchoCommandConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(sysEventSubscribeHostHandler, SYS_EventHandlerMailParams_t, NoAppropriateType_t)

//DECLARE_CLIENT_REQUEST_API_FUNCTION(Mail_SetEchoDelay, TE_SetEchoDelayCommandReqDescr_t, TE_SetEchoDelayCommandConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC_SetWakeUpActionCodeReq, RF4CE_ZRC_SetWakeUpActionCodeReqDescr_t, RF4CE_ZRC_SetWakeUpActionCodeConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC_GetWakeUpActionCodeReq, RF4CE_ZRC_GetWakeUpActionCodeReqDescr_t, RF4CE_ZRC_GetWakeUpActionCodeConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(RF4CE_PairInd, RF4CE_PairingIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(RF4CE_ZRC2_CheckValidationInd, RF4CE_ZRC2_CheckValidationIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(RF4CE_ZRC2_ControlCommandInd, RF4CE_ZRC2_ControlCommandIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(RF4CE_ZRC1_ControlCommandInd, RF4CE_ZRC1_ControlCommandIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC1_VendorSpecificReq, RF4CE_ZRC1_VendorSpecificReqDescr_t, RF4CE_ZRC1_VendorSpecificConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(RF4CE_ZRC1_VendorSpecificInd, RF4CE_ZRC1_VendorSpecificIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC2_SetPushButtonStimulusReq, RF4CE_ZRC2_ButtonBindingReqDescr_t, RF4CE_ZRC2_BindingConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC2_EnableBindingReq, RF4CE_ZRC2_BindingReqDescr_t, RF4CE_ZRC2_BindingConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC2_SetAttributesReq, RF4CE_ZRC2_SetAttributesReqDescr_t, RF4CE_ZRC2_SetAttributesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_ZRC1_TargetBindReq, RF4CE_ZRC1_BindReqDescr_t, RF4CE_ZRC1_BindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_UnpairReq, RF4CE_UnpairReqDescr_t, RF4CE_UnpairConfParams_t)

//DECLARE_CLIENT_REQUEST_API_FUNCTION(Mail_Host2Uart1, TE_Host2Uart1ReqDescr_t,   NoAppropriateType_t)

#ifdef _PHY_TEST_HOST_INTERFACE_

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Get_Caps_Req, Phy_Test_Get_Caps_ReqDescr_t, Phy_Test_Get_Caps_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Set_Channel_Req, Phy_Test_Set_Channel_ReqDescr_t, Phy_Test_Set_Channel_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Continuous_Wave_Start_Req, Phy_Test_Continuous_Wave_Start_ReqDescr_t, Phy_Test_Continuous_Wave_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Continuous_Wave_Stop_Req, Phy_Test_Continuous_Wave_Stop_ReqDescr_t,   Phy_Test_Continuous_Wave_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Transmit_Start_Req, Phy_Test_Transmit_Start_ReqDescr_t, Phy_Test_Transmit_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Transmit_Stop_Req, Phy_Test_Transmit_Stop_ReqDescr_t, Phy_Test_Transmit_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Receive_Start_Req, Phy_Test_Receive_Start_ReqDescr_t, Phy_Test_Receive_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Receive_Stop_Req, Phy_Test_Receive_Stop_ReqDescr_t, Phy_Test_Receive_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Echo_Start_Req, Phy_Test_Echo_Start_ReqDescr_t, Phy_Test_Echo_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Echo_Stop_Req, Phy_Test_Echo_Stop_ReqDescr_t, Phy_Test_Echo_StartStop_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Energy_Detect_Scan_Req, Phy_Test_Energy_Detect_Scan_ReqDescr_t, Phy_Test_Energy_Detect_Scan_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Get_Stats_Req, Phy_Test_Get_Stats_ReqDescr_t, Phy_Test_Get_Stats_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Reset_Stats_Req, Phy_Test_Reset_Stats_ReqDescr_t, Phy_Test_Reset_Stats_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_Set_TX_Power_Req, Phy_Test_Set_TX_Power_ReqDescr_t, Phy_Test_Set_TX_Power_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(Phy_Test_SelectAntenna_Req, Phy_Test_Select_Antenna_ReqDescr_t, Phy_Test_Select_Antenna_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_Get_Diag_Caps_Req, RF4CE_Diag_Caps_ReqDescr_t, RF4CE_Diag_Caps_ConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(RF4CE_Get_Diag_Req, RF4CE_Diag_ReqDescr_t, RF4CE_Diag_ConfParams_t)

#endif  // _PHY_TEST_HOST_INTERFACE_

DECLARE_CLIENT_REQUEST_API_FUNCTION(Mail_TestEngineReset, TE_ResetCommandReqDescr_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(SYS_EventNtfy, SYS_EventNotifyParams_t, NoAppropriateType_t)

#ifdef _ZBPRO_

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_NWK_PermitJoiningReq, ZBPRO_NWK_PermitJoiningReqDescr_t, ZBPRO_NWK_PermitJoiningConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_NWK_LeaveReq, ZBPRO_NWK_LeaveReqDescr_t, ZBPRO_NWK_LeaveConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_NWK_GetKeyReq, ZBPRO_NWK_GetKeyReqDescr_t, ZBPRO_NWK_GetKeyConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_NWK_SetKeyReq, ZBPRO_NWK_SetKeyReqDescr_t, ZBPRO_NWK_SetKeyConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_NWK_RouteDiscoveryReq, ZBPRO_NWK_RouteDiscoveryReqDescr_t, ZBPRO_NWK_RouteDiscoveryConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_EndpointRegisterReq, ZBPRO_APS_EndpointRegisterReqDescr_t, ZBPRO_APS_EndpointRegisterConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_EndpointUnregisterReq, ZBPRO_APS_EndpointUnregisterReqDescr_t, ZBPRO_APS_EndpointRegisterConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_DataReq, ZBPRO_APS_DataReqDescr_t, ZBPRO_APS_DataConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_BindReq, ZBPRO_APS_BindUnbindReqDescr_t, ZBPRO_APS_BindUnbindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_UnbindReq, ZBPRO_APS_BindUnbindReqDescr_t, ZBPRO_APS_BindUnbindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_GetReq, ZBPRO_APS_GetReqDescr_t, ZBPRO_APS_GetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_SetReq, ZBPRO_APS_SetReqDescr_t, ZBPRO_APS_SetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_GetKeyReq, ZBPRO_APS_GetKeyReqDescr_t, ZBPRO_APS_GetKeyConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_SetKeyReq, ZBPRO_APS_SetKeyReqDescr_t, ZBPRO_APS_SetKeyConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_AddGroupReq, ZBPRO_APS_AddGroupReqDescr_t, ZBPRO_APS_AddGroupConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveGroupReq, ZBPRO_APS_RemoveGroupReqDescr_t, ZBPRO_APS_RemoveGroupConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveAllGroupsReq, ZBPRO_APS_RemoveAllGroupsReqDescr_t, ZBPRO_APS_RemoveAllGroupsConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_TransportKeyReq, ZBPRO_APS_TransportKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_UpdateDeviceReq, ZBPRO_APS_UpdateDeviceReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_RemoveDeviceReq, ZBPRO_APS_RemoveDeviceReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_RequestKeyReq, ZBPRO_APS_RequestKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_APS_SwitchKeyReq, ZBPRO_APS_SwitchKeyReqDescr_t, ZBPRO_APS_SecurityServicesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_AddrResolvingReq, ZBPRO_ZDO_AddrResolvingReqDescr_t, ZBPRO_ZDO_AddrResolvingConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_NodeDescReq, ZBPRO_ZDO_NodeDescReqDescr_t, ZBPRO_ZDO_NodeDescConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_PowerDescReq, ZBPRO_ZDO_PowerDescReqDescr_t, ZBPRO_ZDO_PowerDescConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_SimpleDescReq, ZBPRO_ZDO_SimpleDescReqDescr_t, ZBPRO_ZDO_SimpleDescConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_ActiveEpReq, ZBPRO_ZDO_ActiveEpReqDescr_t, ZBPRO_ZDO_ActiveEpConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MatchDescReq, ZBPRO_ZDO_MatchDescReqDescr_t, ZBPRO_ZDO_MatchDescConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_DeviceAnnceReq, ZBPRO_ZDO_DeviceAnnceReqDescr_t, ZBPRO_ZDO_DeviceAnnceConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_EndDeviceBindReq, ZBPRO_ZDO_EndDeviceBindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_BindReq, ZBPRO_ZDO_BindUnbindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_UnbindReq, ZBPRO_ZDO_BindUnbindReqDescr_t, ZBPRO_ZDO_BindConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_StartNetworkReq, ZBPRO_ZDO_StartNetworkReqDescr_t, ZBPRO_ZDO_StartNetworkConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtLeaveReq, ZBPRO_ZDO_MgmtLeaveReqDescr_t, ZBPRO_ZDO_MgmtLeaveConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtPermitJoiningReq, ZBPRO_ZDO_MgmtPermitJoiningReqDescr_t, ZBPRO_ZDO_MgmtPermitJoiningConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateReq, ZBPRO_ZDO_MgmtNwkUpdateReqDescr_t, ZBPRO_ZDO_MgmtNwkUpdateConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateUnsolResp, ZBPRO_ZDO_MgmtNwkUpdateUnsolRespDescr_t, ZBPRO_ZDO_MgmtNwkUpdateUnsolConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtLqiReq, ZBPRO_ZDO_MgmtLqiReqDescr_t, ZBPRO_ZDO_MgmtLqiConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZDO_MgmtBindReq, ZBPRO_ZDO_MgmtBindReqDescr_t, ZBPRO_ZDO_MgmtBindConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZDO_MgmtNwkUpdateUnsolInd, ZBPRO_ZDO_MgmtNwkUpdateUnsolIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZDO_DeviceAnnceInd, ZBPRO_ZDO_DeviceAnnceIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_TC_NwkKeyUpdateReq, ZBPRO_TC_NwkKeyUpdateReqDescr_t, ZBPRO_TC_NwkKeyUpdateConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SetPowerSourceReq, ZBPRO_ZCL_SetPowerSourceReqDescr_t, ZBPRO_ZCL_SetPowerSourceConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdDiscoverAttrReq, ZBPRO_ZCL_ProfileWideCmdDiscoverAttrDescr_t, ZBPRO_ZCL_ProfileWideCmdDiscoverAttrConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReadAttributesReq, ZBPRO_ZCL_ProfileWideCmdReadAttrReqDescr_t, ZBPRO_ZCL_ProfileWideCmdReadAttrConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdWriteAttributesReq, ZBPRO_ZCL_ProfileWideCmdWriteAttrReqDescr_t, ZBPRO_ZCL_ProfileWideCmdWriteAttrConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdConfigureReportingReq, ZBPRO_ZCL_ProfileWideCmdConfigureReportingReqDescr_t, ZBPRO_ZCL_ProfileWideCmdConfigureReportingConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationReq, ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationReqDescr_t, ZBPRO_ZCL_ProfileWideCmdReadReportingConfigurationConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_IdentifyCmdIdentifyReq, ZBPRO_ZCL_IdentifyCmdIdentifyReqDescr_t, ZBPRO_ZCL_IdentifyCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_IdentifyCmdIdentifyQueryReq, ZBPRO_ZCL_IdentifyCmdIdentifyQueryReqDescr_t, ZBPRO_ZCL_IdentifyCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdAddGroupReq, ZBPRO_ZCL_GroupsCmdAddGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdAddGroupConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdViewGroupReq, ZBPRO_ZCL_GroupsCmdViewGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdViewGroupConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdGetGroupMembershipReq, ZBPRO_ZCL_GroupsCmdGetGroupMembershipReqDescr_t, ZBPRO_ZCL_GroupsCmdGetGroupMembershipConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdRemoveGroupReq, ZBPRO_ZCL_GroupsCmdRemoveGroupReqDescr_t, ZBPRO_ZCL_GroupsCmdRemoveGroupConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdRemoveAllGroupsReq, ZBPRO_ZCL_GroupsCmdRemoveAllGroupsReqDescr_t, ZBPRO_ZCL_GroupsCmdRemoveAllGroupsConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyReq, ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyReqDescr_t, ZBPRO_ZCL_GroupsCmdAddGroupIfIdentifyConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdAddSceneReq, ZBPRO_ZCL_ScenesCmdAddSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdAddSceneConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdViewSceneReq, ZBPRO_ZCL_ScenesCmdViewSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdViewSceneConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdStoreSceneReq, ZBPRO_ZCL_ScenesCmdStoreSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdStoreSceneConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRecallSceneReq, ZBPRO_ZCL_ScenesCmdRecallSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRemoveSceneReq, ZBPRO_ZCL_ScenesCmdRemoveSceneReqDescr_t, ZBPRO_ZCL_ScenesCmdRemoveSceneConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdRemoveAllScenesReq, ZBPRO_ZCL_ScenesCmdRemoveAllScenesReqDescr_t, ZBPRO_ZCL_ScenesCmdRemoveAllScenesConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ScenesCmdGetSceneMembershipReq, ZBPRO_ZCL_ScenesCmdGetSceneMembershipReqDescr_t, ZBPRO_ZCL_ScenesCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdOffReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdOnReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_OnOffCmdToggleReq, ZBPRO_ZCL_OnOffCmdReqDescr_t, ZBPRO_ZCL_OnOffCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdMoveToLevelReq, ZBPRO_ZCL_LevelControlCmdMoveToLevelReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdMoveReq, ZBPRO_ZCL_LevelControlCmdMoveReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdStepReq, ZBPRO_ZCL_LevelControlCmdStepReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_LevelControlCmdStopReq, ZBPRO_ZCL_LevelControlCmdStopReqDescr_t, ZBPRO_ZCL_LevelControlCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_DoorLockCmdLockReq, ZBPRO_ZCL_DoorLockCmdLockUnlockReqDescr_t, ZBPRO_ZCL_DoorLockCmdLockUnlockConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_DoorLockCmdUnlockReq, ZBPRO_ZCL_DoorLockCmdLockUnlockReqDescr_t, ZBPRO_ZCL_DoorLockCmdLockUnlockConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdUpOpenReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdDownCloseReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdStopReq, ZBPRO_ZCL_WindowCoveringCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdGotoLiftPecentageReq, ZBPRO_ZCL_WindowCoveringLiftTiltPercentCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_WindowCoveringCmdGotoTiltPecentageReq, ZBPRO_ZCL_WindowCoveringLiftTiltPercentCmdReqDescr_t, ZBPRO_ZCL_WindowCoveringCmdConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseReq, ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseReqDescr_t, ZBPRO_ZCL_IASZoneCmdZoneEnrollResponseConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceArmRespReq, ZBPRO_ZCL_SapIasAceArmRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceBypassRespReq, ZBPRO_ZCL_SapIasAceBypassRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneIdMapRespReq, ZBPRO_ZCL_SapIasAceGetZoneIdMapRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneInfoRespReq, ZBPRO_ZCL_SapIasAceGetZoneInfoRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetPanelStatusRespReq, ZBPRO_ZCL_SapIasAceGetPanelStatusRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceSetBypassedZoneListRespReq, ZBPRO_ZCL_SapIasAceSetBypassedZoneListRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneStatusRespReq, ZBPRO_ZCL_SapIasAceGetZoneStatusRespReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAceZoneStatusChangedReq, ZBPRO_ZCL_SapIasAceZoneStatusChangedReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_SapIasAcePanelStatusChangedReq, ZBPRO_ZCL_SapIasAcePanelStatusChangedReqDescr_t, ZBPRO_ZCL_SapIasAceRespReqConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASWDCmdStartWarningReq, ZBPRO_ZCL_IASWDCmdStartWarningReqDescr_t, ZBPRO_ZCL_IASWDCmdStartWarningConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_IASWDCmdSquawkReq, ZBPRO_ZCL_IASWDCmdSquawkReqDescr_t, ZBPRO_ZCL_IASWDCmdSquawkConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_IdentifyInd, ZBPRO_ZCL_IdentifyIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_ProfileWideCmdReportAttributesInd, ZBPRO_ZCL_ProfileWideCmdReportAttributesIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_GroupsCmdGetGroupMembershipResponseInd, ZBPRO_ZCL_GroupsCmdGetGroupMembershipIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseInd, ZBPRO_ZCL_ScenesCmdGetSceneMembershipResponseIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestInd, ZBPRO_ZCL_IASZoneCmdZoneEnrollRequestIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationInd, ZBPRO_ZCL_IASZoneCmdZoneStatusChangeNotificationIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceArmInd, ZBPRO_ZCL_SapIasAceArmIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceBypassInd, ZBPRO_ZCL_SapIasAceBypassIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceEmergencyInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceFireInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAcePanicInd, ZBPRO_ZCL_SapIasAceAlarmIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneIdMapInd, ZBPRO_ZCL_SapIasAceGetZoneIdMapIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneInfoInd, ZBPRO_ZCL_SapIasAceGetZoneInfoIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetPanelStatusInd, ZBPRO_ZCL_SapIasAceGetPanelStatusIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetBypassedZoneListInd, ZBPRO_ZCL_SapIasAceGetBypassedZoneListIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZCL_SapIasAceGetZoneStatusInd, ZBPRO_ZCL_SapIasAceGetZoneStatusIndParams_t, NoAppropriateType_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveToColorReq, ZBPRO_ZCL_ColorControlCmdMoveToColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveToColorConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveColorReq, ZBPRO_ZCL_ColorControlCmdMoveColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveColorConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStepColorReq, ZBPRO_ZCL_ColorControlCmdStepColorReqDescr_t, ZBPRO_ZCL_ColorControlCmdStepColorConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveHueConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedStepHueReq, ZBPRO_ZCL_ColorControlCmdEnhancedStepHueReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedStepHueConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationReq, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationReqDescr_t, ZBPRO_ZCL_ColorControlCmdEnhancedMoveToHueAndSaturationConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdColorLoopSetReq, ZBPRO_ZCL_ColorControlCmdColorLoopSetReqDescr_t, ZBPRO_ZCL_ColorControlCmdColorLoopSetConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStopMoveStepReq, ZBPRO_ZCL_ColorControlCmdStopMoveStepReqDescr_t, ZBPRO_ZCL_ColorControlCmdStopMoveStepConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureReq, ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureReqDescr_t, ZBPRO_ZCL_ColorControlCmdMoveColorTemperatureConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZCL_ColorControlCmdStepColorTemperatureReq, ZBPRO_ZCL_ColorControlCmdStepColorTemperatureReqDescr_t, ZBPRO_ZCL_ColorControlCmdStepColorTemperatureConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZHA_EzModeReq, ZBPRO_ZHA_EzModeReqDescr_t, ZBPRO_ZHA_EzModeConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieDeviceEnrollReq, ZBPRO_ZHA_CieEnrollReqDescr_t, ZBPRO_ZHA_CieEnrollConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieDeviceSetPanelStatusReq, ZBPRO_ZHA_CieSetPanelStatusReqDescr_t, ZBPRO_ZHA_CieSetPanelStatusConfParams_t)

DECLARE_CLIENT_REQUEST_API_FUNCTION(ZBPRO_ZHA_CieZoneSetBypassStateReq, ZBPRO_ZHA_CieZoneSetBypassStateReqDescr_t, ZBPRO_ZHA_CieZoneSetBypassStateConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(ZBPRO_ZHA_CieDeviceSetPanelStatusInd, ZBPRO_ZHA_CieSetPanelStatusIndParams_t, NoAppropriateType_t)

#endif  // _ZBPRO_

DECLARE_CLIENT_REQUEST_API_FUNCTION(Mail_UartSendReq,   Mail_UartSendReqDescr_t,           Mail_UartSendConfParams_t)

DECLARE_CLIENT_INDICATION_API_FUNCTION(Mail_UartRecvInd,   Mail_UartRecvIndDescr_t,          Mail_UartRecvRespParams_t)

#endif // SERVER

#endif /*_ZIGBEE_API_H_*/

/* eof zigbee_api.h */
