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
* FILENAME: $Workfile: trunk/stack/common/Mailbox/include/bbMailAdapter.h $
*
* DESCRIPTION: declaration of the mailbox abstract module.
*
* $Revision: 3612 $
* $Date: 2014-09-17 09:29:25Z $
*
****************************************************************************************/
#ifndef _MAIL_ADAPTER_H
#define _MAIL_ADAPTER_H

/************************* INCLUDES ****************************************************/
#include "bbMailTypes.h"
#include "bbHalMailbox.h"

/************************* TYPES *******************************************************/
/**//**
 * \brief Descriptor of mailbox adapter
 */
typedef enum _MailAdapterDirection_t
{
    NO_TRANSMISSION,
    FROM_SERVER_TO_CLIENT,
    FROM_CLIENT_TO_SERVER,
} MailAdapterDirection_t;

/**//**
 * \brief mailbox adapter tx options descriptor
 */
typedef struct _MailAdapterTxOptions_t
{
    void *currentParcel;                    /*!< current parcel pointer */
    MailCheckSumLength_t txCheckSum;        /*!< Check Sum over all data except header. */
    uint16_t offset;                        /*!< transmitted bytes number */
    uint8_t reservedSpace;                  /*!< Amount of FIFO space reserved for current fragment */
    uint8_t fragmentCounter;                /*!< current fragment number */
    MailAdapterDirection_t txDirection;     /*!< direction: can be TO_CLIENT or TO_SERVER */
} MailAdapterTxOptions_t;

/**//**
 * \brief mailbox adapter rx options descriptor
 */
typedef struct _MailAdapterRxOptions_t
{
    MailCheckSumLength_t rxCheckSum;        /*!< Received Check Sum. */
    MailCheckSumLength_t rxCheckSumCalc;    /*!< Calculated Check Sum over all data except header. */
    MailWrappedReqHeader_t header;          /*!< current request header */
    uint16_t offset;                        /*!< received bytes number */
    uint16_t bytesToReceive;                /*!< bytes amount stored into hardware (for current fragment) */
    uint8_t nextFragmentNumber;             /*!< expected fragment number */
    uint8_t *params;                        /*!< Pointer to parameters of the current request */
    MailAdapterDirection_t rxDirection;     /*!< direction: can be TO_CLIENT or TO_SERVER */
    SYS_DataPointer_t dataPointer;          /*!< Pointer to the current data memory */
} MailAdapterRxOptions_t;

/**//**
 * \brief Descriptor of mailbox adapter
 */
typedef struct _MailAdapterDescriptor_t MailAdapterDescriptor_t;
typedef struct _MailAdapterDescriptor_t
{
    HAL_MailboxDescriptor_t mbFifoDescr;    /*!< shared FIFO descriptor */
    MailAdapterTxOptions_t txOptions;       /*!< tx options */
    /* TODO: add opportunity parallel request reception from different hosts */
    MailAdapterRxOptions_t rxOptions[1];    /*!< rx options */
    SYS_SchedulerTaskDescriptor_t taskDescr;/*!< task descriptor */
    Bool8_t pollPriority;                   /*!< Sets polling priority module (client or server) */
    Bool8_t readyToSendFlag;                /*!< true if FIFO is available to send something */
} MailAdapterDescriptor_t;

#endif /* _MAIL_ADAPTER_H */
/* eof bbMailAdapter.h */