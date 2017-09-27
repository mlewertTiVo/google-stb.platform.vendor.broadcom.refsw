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

/*******************************************************************************
 *
 * DESCRIPTION:
 *      implementation of mailbox client module
 *
*******************************************************************************/

/************************* INCLUDES ****************************************************/
#include "private/bbMailPrivateClient.h"
#include "private/bbMailPrivateAdapter.h"

/************************* STATIC FUNCTIONS PROTOTYPES *********************************/
#ifdef _HOST_
INLINE void freePendingTableEntry(MailPendingAPICall_t *const entry);
#endif  // _HOST_
INLINE void mailClientStartTimer(const uint32_t timeout);
#ifdef _HOST_
INLINE void mailClientSetParcelState(MailPendingAPICall_t *const postponedCall, const MailState_t state);
#endif  // _HOST_
INLINE void mailClientCompouseHeaders(MailFifoHeader_t *const fifoHeader, MailWrappedReqHeader_t *const header,
                                      uint16_t fId, uint8_t uId, uint8_t *const parcel);

INLINE void mailClientCancelParcelSending(MailPendingAPICall_t *const postponedCall);
#ifndef _HOST_
static MailPendingAPICall_t *mailClientFindOldestOneWayEmpty(void);
static MailPendingAPICall_t *mailClientFindPendingTableEntryByUid(uint8_t uId);
#else   // _HOST_
static MailPendingAPICall_t *findAppropriatePendingTableEntry(const uint8_t uId);
#endif  // _HOST_
static MailPendingAPICall_t *mailClientFillPostponeParcel(const uint16_t fId, uint8_t *const req);
static void mailClientSendParcel(MailPendingAPICall_t *const postponedCall);
static bool mailClientQueuePollCommon(bool isServiceQueue);
static void mailClientAckTimerFired(SYS_TimeoutTaskServiceField_t *const timeoutService);
#ifdef _HOST_
static MailPendingAPICall_t *mailClientFindEmptyPendingTableEntryWithUid(uint8_t *uId);
#endif  // _HOST_

/************************* IMPLEMENTATION **********************************************/
static MailClientDescriptor_t clientMemory;
/************************************************************************************//**
    \brief initializes mailbox client module
    \param[in] mail - mailbox descriptor.
****************************************************************************************/
void mailClientInit()
{
    SYS_QueueResetQueue(&clientMemory.parcelQueue);
#ifdef _HOST_
    pthread_mutex_init(&clientMemory.parcelQueueMutex, NULL);
    pthread_mutex_init(&clientMemory.pendingTableMutex, NULL);
#endif  // _HOST_
    SYS_QueueResetQueue(&clientMemory.serviceMessageQueue);
    clientMemory.uIdCounter = 10U;
    memset(clientMemory.pendingTable, 0U,
           sizeof(clientMemory.pendingTable[0]) * MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS);
    clientMemory.ackTimer.callback = mailClientAckTimerFired;
}

#ifdef _HOST_
/************************************************************************************//**
    \brief Clear given entry of postponed table.
    \param[in] entry - pointer to entry to be cleared
****************************************************************************************/
INLINE void freePendingTableEntry(MailPendingAPICall_t *const entry)
{
    pthread_mutex_lock(&clientMemory.pendingTableMutex);
    entry->fId = INCORRECT_REQ_ID;
    pthread_mutex_unlock(&clientMemory.pendingTableMutex);
}
#endif  // _HOST_

/************************************************************************************//**
    \brief Cancel sending procedure and frees payload if possible.
    \param[in] mail - mailbox descriptor.
    \param[in] postponedCall - pointer to entry to be cleared
****************************************************************************************/
INLINE void mailClientCancelParcelSending(MailPendingAPICall_t *const postponedCall)
{
    const MailServiceFunctionInfo_t *const reqInfo = Mail_ServiceGetFunctionInfo(postponedCall->fId);

    if (MAIL_PARCEL_AWAIT_FOR_SEND == postponedCall->state)
    {
#ifdef _HOST_
        pthread_mutex_lock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_
        SYS_QueueRemoveQueueElement(&clientMemory.parcelQueue, &postponedCall->elem);
#ifdef _HOST_
        pthread_mutex_unlock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_
        mailAdapterCancelTx((uint8_t *)&postponedCall->params);
    }

    if (NULL == postponedCall->callback
            && MAIL_INVALID_OFFSET != reqInfo->reqDataPointerOffset)
    {
        SYS_DataPointer_t *const dataPointer = (SYS_DataPointer_t *)(
                (uint8_t *)&postponedCall->params + reqInfo->reqDataPointerOffset);
        if (SYS_CheckPayload(dataPointer))
            SYS_FreePayload(dataPointer);
    }
}

/************************************************************************************//**
    \brief Starts timer for acknowledgment feature.
    \param[in] mail - mailbox descriptor.
    \param[in] timeout - delay in ms
****************************************************************************************/
INLINE void mailClientStartTimer(const uint32_t timeout)
{
    if (!SYS_TimeoutRemain(&clientMemory.ackTimer.service))
    {
        clientMemory.ackTimer.timeout = timeout;
        SYS_TimeoutSignalStart(&clientMemory.ackTimer, TIMEOUT_TASK_ONE_SHOT_MODE);
    }
}

#ifdef _HOST_
/************************************************************************************//**
    \brief Set state and timeout.
    \param[in] postponedCall - parcel buffer pointer.
    \param[in] state - new state.
****************************************************************************************/
INLINE void mailClientSetParcelState(MailPendingAPICall_t *const postponedCall, const MailState_t state)
{
    postponedCall->state = state;
    switch (state)
    {
        case MAIL_PARCEL_AWAIT_FOR_SEND:
        case MAIL_ACK_AWAIT_FOR_SEND:
        case MAIL_ACK_NOT_NEEDED:
        case MAIL_ACK_SENT:
        case MAIL_WAITING_FOR_CONF:
            /* These states can't be expired and due timestamp should be as far as possible */
            postponedCall->dueTimestamp = HAL_GetSystemTime() - 1;
            break;

        case MAIL_WAITING_FOR_ACK:
            postponedCall->dueTimestamp = HAL_GetSystemTime() + MAIL_CLIENT_DELIVERY_TIME_MS;
            break;
        default:
            break;
    }
}
#endif  // _HOST_

/************************************************************************************//**
    \brief Find first empty entry in postponed call table.
    \param[in] client - client module descriptor.

    \return Pointer to the found entry.
****************************************************************************************/
MailPendingAPICall_t *mailClientFindEmptyPendingTableEntry(void)
{
    for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
#ifndef _HOST_
        if (MAIL_ENTRY_IDLE == clientMemory.pendingTable[i].state)
#else   // _HOST_
        if (INCORRECT_REQ_ID == clientMemory.pendingTable[i].fId)
#endif  // _HOST_
            return &clientMemory.pendingTable[i];
    return NULL;
}

#ifndef _HOST_
static MailPendingAPICall_t *mailClientFindOldestOneWayEmpty(void)
{
    uint8_t maxCnt = 0;
    MailPendingAPICall_t *oldestEntry = NULL;
    MailPendingAPICall_t *const sendingInProgress =
        GET_PARENT_BY_FIELD(MailPendingAPICall_t, elem, SYS_QueueGetQueueHead(&clientMemory.parcelQueue));

    for (int16_t i = MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS - 1; i >= 0; --i)
    {
        MailPendingAPICall_t *const seek = &clientMemory.pendingTable[i];
        if ((MAIL_PARCEL_AWAIT_FOR_SEND == seek->state || MAIL_WAITING_FOR_ACK == seek->state)
                && NULL == seek->callback
                && maxCnt <= seek->shipmentsCounter
                && sendingInProgress != seek)
        {
            maxCnt = seek->shipmentsCounter;
            oldestEntry = seek;
        }
    }
    return oldestEntry;
}
#endif  // _HOST_

#ifdef _HOST_
/************************************************************************************//**
    \brief Find an empty entry in the postponed call table with unique uId.
    \param[out] uid - non-conflicting uid

    \return Pointer to the found entry.
****************************************************************************************/
static MailPendingAPICall_t *mailClientFindEmptyPendingTableEntryWithUid(uint8_t *uId)
{
    MailPendingAPICall_t *ret = NULL;
    bool gotUId;

    SYS_DbgAssert(NULL != uId, MAILCLIENT_FIND_EMPTY_PENDING_TABLE_ENTRY_NULL);

    do {
        *uId = clientMemory.uIdCounter++;
        gotUId = true;

        for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
        {
            if (INCORRECT_REQ_ID != clientMemory.pendingTable[i].fId)
            {
                if (clientMemory.pendingTable[i].uId == *uId)
                {
                    SYS_DbgLogId(MAILCLIENT_FIND_EMPTY_PENDING_TABLE_ENTRY_CONFLICT);
                    gotUId = false;
                }
            }
            else
            {
                ret = &clientMemory.pendingTable[i];
            }
        }
    } while (!gotUId);

    return ret;
}
#endif  // _HOST_

#ifdef _HOST_
int mailClientAvailablePendingTableEntry()
{
    int available = 0;
    for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
        if (INCORRECT_REQ_ID == clientMemory.pendingTable[i].fId)
            available++;
    return available;
}
#endif  // _HOST_

#ifndef _HOST_
static MailPendingAPICall_t *mailClientFindPendingTableEntryByUid(uint8_t uId)
{
    for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
        if (MAIL_ENTRY_IDLE != clientMemory.pendingTable[i].state
                && uId == clientMemory.pendingTable[i].uId)
            return &clientMemory.pendingTable[i];
    return NULL;
}
#endif  // _HOST_

#ifdef _HOST_
/************************************************************************************//**
    \brief Find first empty entry in postponed call table.
    \param[in] client - client module descriptor.

    \return Pointer to the found entry.
****************************************************************************************/
static MailPendingAPICall_t *findAppropriatePendingTableEntry(const uint8_t uId)
{
    for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
        if (INCORRECT_REQ_ID != clientMemory.pendingTable[i].fId
                && uId == clientMemory.pendingTable[i].uId)
        {
            if (MAIL_WAITING_FOR_CONF == clientMemory.pendingTable[i].state
                    || MAIL_WAITING_FOR_ACK == clientMemory.pendingTable[i].state)
                return &clientMemory.pendingTable[i];
            else
            {
                SYS_DbgLogId(MAILCLIENT_UNEXPECTED_ANSWER_HOST);
                return NULL;
            }
        }
    return NULL;
}
#endif  // _HOST_

/************************************************************************************//**
    \brief Helper function. Puts a passed wrapper into the packing queue (the client's queue).
    \param[in] mail - mailbox descriptor.
    \param[in] fId - number of invoked wrapper.
    \param[in] req - request pointer.
****************************************************************************************/
static MailPendingAPICall_t *mailClientFillPostponeParcel(const uint16_t fId, uint8_t *const req)
{
#ifndef _HOST_

    const MailServiceFunctionInfo_t *const reqInfo = Mail_ServiceGetFunctionInfo(fId);

    uint8_t uId = clientMemory.uIdCounter++;
    while (NULL != mailClientFindPendingTableEntryByUid(uId))
        uId = clientMemory.uIdCounter++;

    MailPendingAPICall_t *entry = mailClientFindEmptyPendingTableEntry();
    if (NULL == entry)
    {
        entry = mailClientFindOldestOneWayEmpty();
        if (entry)
        {
            mailClientCancelParcelSending(entry);
            entry->state = MAIL_ENTRY_IDLE;
        }
    }

    if (entry)
    {
  #ifdef _HOST_
        pthread_mutex_lock(&clientMemory.pendingTableMutex);
  #endif  // _HOST_
        entry->state = MAIL_ENTRY_ALLOCATED;
        entry->originalReq = req;
        entry->fId = fId;
        entry->uId = uId;
        entry->shipmentsCounter = 0;
        entry->callback = (MAIL_INVALID_OFFSET != reqInfo->reqCallbackOffset) ?
                          *(ConfirmCall_t *)(req + reqInfo->reqCallbackOffset) :
                          NULL;
        memcpy(&entry->params, req + reqInfo->reqParametersOffset, reqInfo->reqParametersLength);
  #ifdef _HOST_
        pthread_mutex_lock(&clientMemory.pendingTableMutex);
  #endif  // _HOST_
        return entry;
    }

    if (MAIL_INVALID_OFFSET != reqInfo->reqCallbackOffset)
    {
        SYS_DbgHalt(MAILCLIENT_NOT_ENOUGH_MEMORY_TO_SEND_REQUEST);
    }

    if (MAIL_INVALID_OFFSET != reqInfo->reqDataPointerOffset)
    {
        SYS_DataPointer_t *const dataPointer = (SYS_DataPointer_t *)(
                (uint8_t *)req + reqInfo->reqParametersOffset + reqInfo->reqDataPointerOffset);
        if (SYS_CheckPayload(dataPointer))
            SYS_FreePayload(dataPointer);
    }
    return NULL;

#else   // _HOST_

    uint8_t uId;
    MailPendingAPICall_t *const postponedCall = mailClientFindEmptyPendingTableEntryWithUid(&uId);
    const MailServiceFunctionInfo_t *const reqInfo = Mail_ServiceGetFunctionInfo(fId);
    if (postponedCall)
    {
        pthread_mutex_lock(&clientMemory.pendingTableMutex);
        postponedCall->originalReq = req;
        postponedCall->fId = fId;
        postponedCall->uId = uId;
        postponedCall->callback = (MAIL_INVALID_OFFSET != reqInfo->reqCallbackOffset) ?
                                  *(ConfirmCall_t *)(req + reqInfo->reqCallbackOffset) :
                                  NULL;
        memcpy(&postponedCall->params, req + reqInfo->reqParametersOffset, reqInfo->reqParametersLength);
        pthread_mutex_unlock(&clientMemory.pendingTableMutex);
    }
    else if (MAIL_INVALID_OFFSET != reqInfo->reqCallbackOffset)
    {
        SYS_DbgHalt(MAILCLIENT_NOT_ENOUGH_MEMORY_TO_SEND_REQUEST_HOST);
    }
    else if (MAIL_INVALID_OFFSET != reqInfo->reqDataPointerOffset)
    {
        SYS_DataPointer_t *const dataPointer = (SYS_DataPointer_t *)(
                (uint8_t *)req + reqInfo->reqParametersOffset + reqInfo->reqDataPointerOffset);
        if (SYS_CheckPayload(dataPointer))
            SYS_FreePayload(dataPointer);
    }
    return postponedCall;

#endif  // _HOST_
}

/************************************************************************************//**
    \brief Checks service state.
    \param[in] mail - mailbox descriptor.
    \return true if service is busy otherwise false.
****************************************************************************************/
bool mailClientIsBusy(void)
{
    return !(SYS_QueueIsEmpty(&clientMemory.parcelQueue)
             && SYS_QueueIsEmpty(&clientMemory.serviceMessageQueue));
}

/************************************************************************************//**
    \brief serializes the received wrapper
    \param[in] mail - mailbox descriptor.
    \param[in] fId - number of invoked wrapper.
    \param[in] req - request pointer.
****************************************************************************************/
void mailClientSerialize(const uint16_t fId, uint8_t *const req)
{
    SYS_DbgAssertComplex(req, MAILCLIENT_MAILCLIENTSERIALIZE_0);
    MailPendingAPICall_t *const postponedCall = mailClientFillPostponeParcel(fId, req);

#ifndef _HOST_
    mailClientSendParcel(postponedCall);
#else   // _HOST_
    pthread_mutex_lock(&clientMemory.parcelQueueMutex);
    SYS_QueuePutQueueElementToTail(&clientMemory.parcelQueue, &postponedCall->elem);
    mailClientSetParcelState(postponedCall, MAIL_PARCEL_AWAIT_FOR_SEND);
    pthread_mutex_unlock(&clientMemory.parcelQueueMutex);
    extern void mailAdapterTryToSend();
    mailAdapterTryToSend();
#endif  // _HOST_
    return;
}

static void mailClientSendParcel(MailPendingAPICall_t *const postponedCall)
{
    if (postponedCall)
    {
        const bool wasBusy = mailClientIsBusy();
#ifdef _HOST_
        pthread_mutex_lock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_
        SYS_DbgAssertComplex(!SYS_QueueFindParentElement(&clientMemory.parcelQueue, &postponedCall->elem), SYS_QUEUE_PUT_MAIL_CLIENT);
        SYS_QueuePutQueueElementToTail(&clientMemory.parcelQueue, &postponedCall->elem);
#ifdef _HOST_
        pthread_mutex_unlock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_
#ifndef _HOST_
        postponedCall->state = MAIL_PARCEL_AWAIT_FOR_SEND;
#else
        mailClientSetParcelState(postponedCall, MAIL_PARCEL_AWAIT_FOR_SEND);
#endif  // _HOST_
        if (!wasBusy)
            mailClientQueuePollCommon(false);
    }
}

INLINE void mailClientCompouseHeaders(MailFifoHeader_t *const fifoHeader, MailWrappedReqHeader_t *const header,
                                      uint16_t fId, uint8_t uId, uint8_t *const parcel)
{
    const MailServiceFunctionInfo_t *const reqInfo = Mail_ServiceGetFunctionInfo(fId);

    /* compose fifo header */
    {
        fifoHeader->msgId = reqInfo->id;
        fifoHeader->isFragment = 0;
        fifoHeader->fragmentNumber = 0;
        fifoHeader->msgType.version = MAILBOX_API_VERSION;
        fifoHeader->msgType.subSystem = UART;
        fifoHeader->msgType.isConfirm = (TE_MAILBOX_ACK_FID == reqInfo->id) ? 1 : 0;

#ifdef MAILBOX_HOST_SIDE
        fifoHeader->msgType.fromStackSide = 0;
#else
        fifoHeader->msgType.fromStackSide = 1;
#endif
    }

    /* compose wrapped header */
    {
        header->uId                  = uId;
        header->paramLength          = reqInfo->reqParametersLength;
        if (MAIL_INVALID_OFFSET != reqInfo->reqDataPointerOffset)
        {
            SYS_DataPointer_t *const dataPointer = (SYS_DataPointer_t *)(parcel + reqInfo->reqDataPointerOffset);
            header->paramLength     -= sizeof(SYS_DataPointer_t);
            header->dataLength       = SYS_GetPayloadSize(dataPointer);
            SYS_DbgAssertComplex(header->paramLength, MAILCLIENT_QUEUEPOLL_0);
        }
        else
            header->dataLength       = 0;
    }
}

/************************************************************************************//**
    \brief This function called by the mailbox adapter when the adapter checks
        the client's sending queue.
    \param[in] mail - mailbox descriptor.

    \return true if the server's queue is not empty and false otherwise.
****************************************************************************************/
bool mailClientQueuePoll(void)
{
    return /* mailClientQueuePollCommon(mail, true) */
        /* || */ mailClientQueuePollCommon(false);
}

static bool mailClientQueuePollCommon(bool isServiceQueue)
{
    SYS_QueueDescriptor_t *const queueToPoll = (isServiceQueue) ?
            &clientMemory.serviceMessageQueue :
            &clientMemory.parcelQueue;

    if (!SYS_QueueIsEmpty(queueToPoll))
    {
        MailFifoHeader_t fifoHeader;
        MailWrappedReqHeader_t header;
        MailPendingAPICall_t *const postponedCall = GET_PARENT_BY_FIELD(MailPendingAPICall_t, elem,
                SYS_QueueGetQueueHead(queueToPoll));
        uint8_t *const parcel = (isServiceQueue) ?
                                (uint8_t *)&postponedCall->serviceParcel :
                                (uint8_t *)&postponedCall->params;

        mailClientCompouseHeaders(&fifoHeader, &header, postponedCall->fId, postponedCall->uId, parcel);

        if (mailAdapterSend(&fifoHeader, &header, parcel))
        {
#ifdef _HOST_
            pthread_mutex_lock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_
            SYS_QueueRemoveHeadElement(queueToPoll);
#ifdef _HOST_
            pthread_mutex_unlock(&clientMemory.parcelQueueMutex);
#endif  // _HOST_

#ifndef _HOST_
            postponedCall->state = MAIL_WAITING_FOR_ACK;
            postponedCall->shipmentsCounter++;
            postponedCall->dueTimestamp = HAL_GetSystemTime() + MAIL_CLIENT_DELIVERY_TIME_MS;
            mailClientStartTimer(MAIL_CLIENT_DELIVERY_TIME_MS);
#else   // _HOST_
            mailClientSetParcelState(postponedCall, MAIL_WAITING_FOR_ACK);
            //mailClientStartTimer(mail, MAIL_CLIENT_DELIVERY_TIME_MS);
#endif  // _HOST_
        }
        return true;
    }
    return false;
}

/************************************************************************************//**
    \brief Helper function. Parses the parcel header and allocates memory for a parcel.
    \param[in] adapter - adapter module descriptor.
    \param[in] header - request header pointer.
****************************************************************************************/
uint8_t *mailClientGetMemory(MailFifoHeader_t *const fifoHeader, MailWrappedReqHeader_t *const header)
{
#ifndef _HOST_

    MailPendingAPICall_t *const postponedCall = mailClientFindPendingTableEntryByUid(header->uId);
    if (NULL == postponedCall)
    {
        if (TE_MAILBOX_ACK_FID != fifoHeader->msgId)
        {
            SYS_DbgLogId(MAILCLIENT_FIND_APPROPRIATE_PENDING_TABLE_ENTRY_0);
        }
        return NULL;
    }

    if (MAIL_WAITING_FOR_CONF != postponedCall->state
            && MAIL_WAITING_FOR_ACK != postponedCall->state)
    {
        SYS_DbgLogId(MAILCLIENT_UNEXPECTED_ANSWER);
        return NULL;
    }

    return (TE_MAILBOX_ACK_FID == fifoHeader->msgId) ?
           (uint8_t *)&postponedCall->serviceParcel :
           (uint8_t *)&postponedCall->params;

#else   // _HOST_

    MailPendingAPICall_t *const postponedCall = findAppropriatePendingTableEntry(header->uId);
    if (postponedCall)
        return (TE_MAILBOX_ACK_FID == fifoHeader->msgId) ?
               (uint8_t *)&postponedCall->serviceParcel :
               (uint8_t *)&postponedCall->params;

    if (TE_MAILBOX_ACK_FID != fifoHeader->msgId)
        SYS_DbgLogId(MAILCLIENT_FIND_APPROPRIATE_PENDING_TABLE_ENTRY_0_HOST);

    return NULL;

#endif  // _HOST_
}

/************************************************************************************//**
    \brief This function should be invoked by the adapter when it has a parcel
        for the mailbox client.
    \param[out] fifoHeader - FIFO header pointer.
    \param[out] header - request header pointer.
    \param[out] confirm - confirmation parameters pointer.
****************************************************************************************/
void mailClientDataInd(MailFifoHeader_t *const fifoHeader, MailWrappedReqHeader_t *const header, uint8_t *confirm)
{
    (void)header;
    SYS_DbgAssertComplex(NULL != confirm, MAILCLIENT_DATAIND_CONFIRM_SHALL_BE_NOT_NULL);

    switch (fifoHeader->msgId)
    {
        case TE_MAILBOX_ACK_FID:
        {
            MailPendingAPICall_t *const postponedCall = GET_PARENT_BY_FIELD(MailPendingAPICall_t,
                    serviceParcel , confirm);
            SYS_DbgAssertComplex(MAIL_SUCCESSFUL_RECEIVED == postponedCall->serviceParcel.conf.ack.status,
                                 MAILCLIENT_DATAIND_0);

#ifdef _HOST_
            mailClientCancelParcelSending(postponedCall);
#endif  // _HOST_
#if defined(RF4CE_ZRC_WAKEUP_ACTION_CODE_SUPPORT) && !defined(_HOST_)
            RF4CE_PMSetHostWakingUp(false);
#endif
#ifndef _HOST_
            mailClientCancelParcelSending(postponedCall);
            postponedCall->state = postponedCall->callback ? MAIL_WAITING_FOR_CONF : MAIL_ENTRY_IDLE;
            break;
#else   // _HOST_
            if (!postponedCall->callback)
                freePendingTableEntry(postponedCall);
            else
                mailClientSetParcelState(postponedCall, MAIL_WAITING_FOR_CONF);
            break;
#endif  // _HOST_
        }
        default:
        {
            MailPendingAPICall_t *const postponedCall = GET_PARENT_BY_FIELD(MailPendingAPICall_t, params, confirm);
#ifdef _DEBUG_COMPLEX_
            {
                SYS_DbgAssertLog(MAIL_WAITING_FOR_ACK != postponedCall->state, MAILCLIENT_DATAIND__CONFIRM_OR_RESPONSE_RECEIVED_BEFORE_ACK);
                const MailServiceFunctionInfo_t *const reqInfo = Mail_ServiceGetFunctionInfo(fifoHeader->msgId);
                SYS_DbgAssertComplex(MAIL_INVALID_OFFSET != reqInfo->reqCallbackOffset,
                                     MAILCLIENT_DATAIND_COMMAND_DO_NOT_HAVE_RESPONSE_OR_CONFIRM);
                SYS_DbgAssertComplex(NULL != postponedCall->callback, MAILCLIENT_DATAIND_CALLBACK_SHALL_BE_NOT_NULL);
            }
#endif /* _DEBUG_COMPLEX_ */

            /* call application callback */
            Mail_CallbackHandler(fifoHeader->msgId, postponedCall->callback, postponedCall->originalReq, confirm);

            mailClientCancelParcelSending(postponedCall);
#ifndef _HOST_
            postponedCall->state = MAIL_ENTRY_IDLE;
#else   // _HOST_
            freePendingTableEntry(postponedCall);
#endif  // _HOST_
        }
    }
}

static void mailClientAckTimerFired(SYS_TimeoutTaskServiceField_t *const timeoutService)
{
    (void)timeoutService;
    const MailPendingAPICall_t *nextToExpire = NULL;
    for (uint8_t i = 0; i < MAIL_CLIENT_MAX_AMOUNT_PENDING_CALLS; ++i)
    {
        MailPendingAPICall_t *const entry = &clientMemory.pendingTable[i];
#ifndef _HOST_
        if (MAIL_WAITING_FOR_ACK == entry->state)
#else   // _HOST_
        if (INCORRECT_REQ_ID != entry->fId)
#endif  // _HOST_
        {
#ifdef _HOST_
            if (MAIL_WAITING_FOR_ACK != entry->state)
                continue;
#endif  // _HOST_
            if (HAL_GetSystemTime() >= entry->dueTimestamp)
            {
#ifndef _HOST_
                SYS_DbgAssertLog(TE_ASSERT_LOGID_FID == entry->fId || TE_HELLO_FID == entry->fId,
                                 MAILCLIENT__ACK_TIMEOUT_EXPIRED__RETRANSMISSION_INITIATED);
#endif  // _HOST_
                mailClientSendParcel(entry);
            }
            else if (NULL == nextToExpire
                     || nextToExpire->dueTimestamp > entry->dueTimestamp)
                nextToExpire = entry;
        }
    }

    if (NULL != nextToExpire)
    {
        const SYS_Time_t currentTime = HAL_GetSystemTime();
        const uint32_t timeout = (nextToExpire->dueTimestamp > currentTime) ?
                                 nextToExpire->dueTimestamp - currentTime : 0;
        mailClientStartTimer(timeout);
    }
}

/* eof bbMailClient.c */