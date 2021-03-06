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
 * FILENAME: $Workfile: trunk/stack/ZbPro/APS/include/private/bbZbProApsDataTx.h $
 *
 * DESCRIPTION:
 *   Declaration of the ZigBee PRO APS Data Service component. Transmission  part.
 *
 * $Revision: 2491 $
 * $Date: 2014-05-22 11:01:34Z $
 *
 ****************************************************************************************/

#ifndef _ZBPRO_APS_DATA_TX_H
#define _ZBPRO_APS_DATA_TX_H

/************************* INCLUDES ****************************************************/
#include "bbZbProApsCommon.h"
#include "bbZbProApsData.h"
#include "private/bbZbProApsPrivate.h"
#include "private/bbZbProApsHub.h"

/************************* TYPES *******************************************************/
/* Descriptor declaration of the static variable of the module */
typedef struct _ZbProApsDataDesc_t
{
    SYS_QueueDescriptor_t       dataReqFromNhlQueue;
    ZBPRO_APS_DataReqDescr_t    *dataReqToFill;
} ZbProApsDataDesc_t;

/************************* PROTOTYPES **************************************************/

/**//**
 * \brief Fills in the specified transceiver Tx buffer
 */
APS_PRIVATE ZbProApsHubTxFillCb_t zbProApsDataFill;

/**//**
 * \brief  Tx confirmation callback function
 */
APS_PRIVATE ZbProApsHubTxConfCb_t zbProApsDataConf;

/**//**
 * \brief Resets this module
 */
APS_PRIVATE void zbProApsDataReset(void);

/************************************************************************************//**
  \brief APS-DATA request primitive task handler function
****************************************************************************************/
APS_PRIVATE void zbProApsDataReqHandler(SYS_SchedulerTaskDescriptor_t *const taskDescriptor);

#endif /* _ZBPRO_APS_DATA_TX_H */
