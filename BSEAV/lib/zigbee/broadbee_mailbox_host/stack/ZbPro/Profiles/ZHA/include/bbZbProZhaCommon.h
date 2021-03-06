/******************************************************************************
* (c) 2014 Broadcom Corporation
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
******************************************************************************/
/*****************************************************************************
*
* FILENAME: $Workfile: trunk/stack/ZbPro/Profiles/ZHA/include/bbZbProZhaCommon.h $
*
* DESCRIPTION:
*   ZHA Profile common definitions.
*
* $Revision: 7361 $
* $Date: 2015-07-08 17:17:17Z $
*
*****************************************************************************************/


#ifndef _BB_ZBPRO_ZHA_COMMON_H
#define _BB_ZBPRO_ZHA_COMMON_H


/************************* INCLUDES *****************************************************/
#include "bbZbProZhaConfig.h"
#include "bbZbProApsCommon.h"


/************************* DEFINITIONS **************************************************/
/**//**
 * \brief   Numeric value of ZHA profile identifier.
 * \par     Documentation
 *  See ZigBee Document 05-3520-29, frontpage.
 */
#define ZBPRO_ZHA_PROFILE_ID    ZBPRO_APS_PROFILE_ID_ZHA


/**//**
 * \brief   Enumeration of ZHA Devices.
 * \par     Documentation
 *  See ZigBee Document 05-3520-29, subclause 5.7, table 5.1.
 */
typedef enum _ZBPRO_ZHA_DeviceId_t
{
    /* Generic. */

    ZHA_DEVICE_ID_ONOFF_SWITCH                          = 0x0000,       /*!< On/Off Switch. */

    ZHA_DEVICE_ID_LEVEL_CONTROL_SWITCH                  = 0x0001,       /*!< Level Control Switch. */

    ZHA_DEVICE_ID_ONOFF_OUTPUT                          = 0x0002,       /*!< On/Off Output. */

    ZHA_DEVICE_ID_LEVEL_CONTROLLABLE_OUTPUT             = 0x0003,       /*!< Level Controllable Output. */

    ZHA_DEVICE_ID_SCENE_SELECTOR                        = 0x0004,       /*!< Scene Selector. */

    ZHA_DEVICE_ID_CONFIGURATION_TOOL                    = 0x0005,       /*!< Configuration Tool. */

    ZHA_DEVICE_ID_REMOTE_CONTROL                        = 0x0006,       /*!< Remote Control. */

    ZHA_DEVICE_ID_COMBINED_INTERFACE                    = 0x0007,       /*!< Combined Interface. */

    ZHA_DEVICE_ID_RANGE_EXTENDER                        = 0x0008,       /*!< Range Extender. */

    ZHA_DEVICE_ID_MAINS_POWER_OUTLET                    = 0x0009,       /*!< Mains Power Outlet. */

    ZHA_DEVICE_ID_DOOR_LOCK                             = 0x000A,       /*!< Door Lock. */

    ZHA_DEVICE_ID_DOOR_LOCK_CONTROLLER                  = 0x000B,       /*!< Door Lock Controller. */

    ZHA_DEVICE_ID_SIMPLE_SENSOR                         = 0x000C,       /*!< Simple Sensor. */

    ZHA_DEVICE_ID_CONSUMPTION_AWARENESS_DEVICE          = 0x000D,       /*!< Consumption Awareness Device. */

    /*                                         0x000E ... 0x004F           < reserved. */

    ZHA_DEVICE_ID_HOME_GATEWAY                          = 0x0050,       /*!< Home Gateway. */

    ZHA_DEVICE_ID_SMART_PLUG                            = 0x0051,       /*!< Smart Plug. */

    ZHA_DEVICE_ID_WHITE_GOODS                           = 0x0052,       /*!< White Goods. */

    ZHA_DEVICE_ID_METER_INTERFACE                       = 0x0053,       /*!< Meter Interface. */

    /*                                         0x0054 ... 0x005F           < reserved. */

    /*                                         0x0060 ... 0x00FF           < reserved. */


    /* Lighting. */

    ZHA_DEVICE_ID_ONOFF_LIGHT                           = 0x0100,       /*!< On/Off Light. */

    ZHA_DEVICE_ID_DIMMABLE_LIGHT                        = 0x0101,       /*!< Dimmable Light. */

    ZHA_DEVICE_ID_COLOR_DIMMABLE_LIGHT                  = 0x0102,       /*!< Color Dimmable Light. */

    ZHA_DEVICE_ID_ONOFF_LIGHT_SWITCH                    = 0x0103,       /*!< On/Off Light Switch. */

    ZHA_DEVICE_ID_DIMMER_SWITCH                         = 0x0104,       /*!< Dimmer Switch. */

    ZHA_DEVICE_ID_COLOR_DIMMER_SWITCH                   = 0x0105,       /*!< Color Dimmer Switch. */

    ZHA_DEVICE_ID_LIGHT_SENSOR                          = 0x0106,       /*!< Light Sensor. */

    ZHA_DEVICE_ID_OCCUPANCY_SENSOR                      = 0x0107,       /*!< Occupancy Sensor. */

    /*                                         0x0108 ... 0x01FF           < reserved. */


    /* Closures. */

    ZHA_DEVICE_ID_SHADE                                 = 0x0200,       /*!< Shade. */

    ZHA_DEVICE_ID_SHADE_CONTROLLER                      = 0x0201,       /*!< Shade Controller. */

    ZHA_DEVICE_ID_WINDOW_COVERING_DEVICE                = 0x0202,       /*!< Window Covering Device. */

    ZHA_DEVICE_ID_WINDOW_COVERING_CONTROLLER            = 0x0203,       /*!< Window Covering Controller. */

    /*                                         0x0204 ... 0x02FF           < reserved. */


    /* HVAC. */

    ZHA_DEVICE_ID_HEATING_COOLING_UNIT                  = 0x0300,       /*!< Heating/Cooling Unit. */

    ZHA_DEVICE_ID_THERMOSTAT                            = 0x0301,       /*!< Thermostat. */

    ZHA_DEVICE_ID_TEMPERATURE_SENSOR                    = 0x0302,       /*!< Temperature Sensor. */

    ZHA_DEVICE_ID_PUMP                                  = 0x0303,       /*!< Pump. */

    ZHA_DEVICE_ID_PUMP_CONTROLLER                       = 0x0304,       /*!< Pump Controller. */

    ZHA_DEVICE_ID_PRESSURE_SENSOR                       = 0x0305,       /*!< Pressure Sensor. */

    ZHA_DEVICE_ID_FLOW_SENSOR                           = 0x0306,       /*!< Flow Sensor. */

    ZHA_DEVICE_ID_MINI_SPLIT_AC                         = 0x0307,       /*!< Mini Split AC. */

    /*                                         0x0308 ... 0x03FF           < reserved. */


    /* Intruder Alarm Systems. */

    ZHA_DEVICE_ID_IAS_CONTROL_AND_INDICATING_EQUIPMENT  = 0x0400,       /*!< IAS Control and Indicating Equipment. */

    ZHA_DEVICE_ID_IAS_ANCILLARY_CONTROL_EQUIPMENT       = 0x0401,       /*!< IAS Ancillary Control Equipment. */

    ZHA_DEVICE_ID_IAS_ZONE                              = 0x0402,       /*!< IAS Zone. */

    ZHA_DEVICE_ID_IAS_WARNING_DEVICE                    = 0x0403,       /*!< IAS Warning Device. */

    /*                                         0x0404 ... 0xFFFF           < reserved. */

    ZHA_DEVICE_ID_MAX                                   = 0xFFFF,       /*!< Maximum value of ZHA Device identifier.
                                                                            This value also makes the enumeration 16-bit
                                                                            integer type. */
} ZBPRO_ZHA_DeviceId_t;

/*
 * Validate size of the Device Id data type. It must be 16-bit wide.
 */
//SYS_DbgAssertStatic(sizeof(ZBPRO_ZHA_DeviceId_t) == sizeof(ZBPRO_APS_DeviceId_t));  // short-enum


/**//**
 * \brief   Numeric value of ZHA device version.
 * \note
 *  ZHA profile doesn't define device version. Due to this reason the default APS device
 *  version is accepted for all the ZHA devices.
 * \par     Documentation
 *  See ZigBee Document 053474r20, subclause 2.3.2.5.4, table 2.40.
 */
#define ZBPRO_ZHA_DEVICE_VERSION    ZBPRO_APS_DEVICE_VERSION_DEFAULT

/*
 * Validate numeric value of ZHA device version.
 */
SYS_DbgAssertStatic(ZBPRO_ZHA_DEVICE_VERSION <= ZBPRO_APS_DEVICE_VERSION_MAX);


/************************* PROTOTYPES ***************************************************/
/**//**
 * \brief   Initializes ZHA Profile.
 * \details
 *  Call this function ones at application startup.
 * \details
 *  This function initializes one instance of ZCL Dispatcher with ZHA Profile Descriptor.
 * \note
 *  This implementation supports only one instance of ZHA profile.
 */
void ZBPRO_ZHA_Initialization(void);


#endif /* _BB_ZBPRO_ZHA_COMMON_H */
