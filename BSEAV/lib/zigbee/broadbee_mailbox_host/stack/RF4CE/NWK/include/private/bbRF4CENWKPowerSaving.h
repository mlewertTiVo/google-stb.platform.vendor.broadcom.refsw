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
* FILENAME: $Workfile: trunk/stack/RF4CE/NWK/include/private/bbRF4CENWKPowerSaving.h $
*
* DESCRIPTION:
*   RF4CE NWK Power Saving private functions.
*
* $Revision: 3450 $
* $Date: 2014-09-03 14:42:06Z $
*
****************************************************************************************/
#ifndef _RF4CE_NWK_POWER_SAVING_H
#define _RF4CE_NWK_POWER_SAVING_H
/************************* INCLUDES ****************************************************/
#include "bbSysTypes.h"
#include "bbSysBasics.h"
#include "bbSysTaskScheduler.h"

/************************* FUNCTIONS PROTOTYPES ****************************************/
/************************************************************************************//**
 \brief Starts Power Saving Work.

 \param[in] callRXEnable - if to call RX Enable request.
 \return Nothing.
 ****************************************************************************************/
void RF4CE_NWK_StartPowerSaving(bool callRXEnable);

/************************************************************************************//**
 \brief Ends (stops) Power Saving Work.

 \param[in] rxDuration - the duration of RX to be set on (temporarilly disable PowerSaving).
 \return Nothing.
 ****************************************************************************************/
void RF4CE_NWK_EndPowerSaving(uint32_t rxDuration);

/************************************************************************************//**
 \brief Power Saving Timeout Task Handler.

 \param[in] taskDescriptor - pointer to the current task descriptor structure.
 \return Nothing.
 ****************************************************************************************/
void RF4CE_NWK_PowerSavingTask(SYS_SchedulerTaskDescriptor_t *const taskDescriptor);

#endif /* _RF4CE_NWK_POWER_SAVING_H */