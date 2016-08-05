/***************************************************************************
 * Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <tzioc_ioctls.h>

#include "libtzioc.h"

int _tzioc_ioctl_client_open(
    struct tzioc_client *pClient)
{
    struct tzioc_ioctl_client_open_data clientOpenData;
    int err = 0;

    strncpy(clientOpenData.name, pClient->name, TZIOC_CLIENT_NAME_LEN_MAX);
    clientOpenData.msgQ = (uint32_t)pClient->msgQ;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_CLIENT_OPEN,
        (void *)&clientOpenData);

    if (err)
        return -EIO;

    if (clientOpenData.retVal)
        return clientOpenData.retVal;

    pClient->hKlient   = clientOpenData.hClient;
    pClient->id        = clientOpenData.id;
    pClient->smemStart = clientOpenData.smemStart;
    pClient->smemSize  = clientOpenData.smemSize;
    return 0;
}

int _tzioc_ioctl_client_close(
    struct tzioc_client *pClient)
{
    struct tzioc_ioctl_client_close_data clientCloseData;
    int err = 0;

    clientCloseData.hClient = pClient->hKlient;
    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_CLIENT_CLOSE,
        (void *)&clientCloseData);

    if (err)
        return -EIO;

    if (clientCloseData.retVal)
        return clientCloseData.retVal;

    return 0;
}

int _tzioc_ioctl_client_getid(
    struct tzioc_client *pClient,
    const char *pName,
    uint32_t ulPid,
    uint8_t *pId)
{
    struct tzioc_ioctl_client_getid_data clientGetIdData;
    int err = 0;

    clientGetIdData.hClient = pClient->hKlient;
    strncpy(clientGetIdData.name, pName, TZIOC_CLIENT_NAME_LEN_MAX);
    clientGetIdData.pid = ulPid;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_CLIENT_GETID,
        (void *)&clientGetIdData);

    if (err)
        return -EIO;

    if (clientGetIdData.retVal)
        return clientGetIdData.retVal;

    *pId = clientGetIdData.id;
    return 0;
}

int _tzioc_ioctl_msg_send(
    struct tzioc_client *pClient,
    struct tzioc_msg_hdr *pHdr)
{
    struct tzioc_ioctl_msg_send_data msgSendData;
    int err = 0;

    msgSendData.hClient = pClient->hKlient;
    memcpy(&msgSendData.hdr, pHdr, sizeof(*pHdr));
    msgSendData.payloadAddr = (uint32_t)TZIOC_MSG_PAYLOAD(pHdr);

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_MSG_SEND,
        (void *)&msgSendData);

    if (err)
        return -EIO;

    if (msgSendData.retVal)
        return msgSendData.retVal;

    return 0;
}

int _tzioc_ioctl_map_paddr(
    struct tzioc_client *pClient,
    uint32_t ulPaddr,
    uint32_t ulSize,
    uint32_t ulFlags,
    uint32_t *pulVaddr)
{
    struct tzioc_ioctl_map_paddr_data mapPaddrData;
    int err = 0;

    mapPaddrData.hClient = pClient->hKlient;
    mapPaddrData.paddr = ulPaddr;
    mapPaddrData.size  = ulSize;
    mapPaddrData.flags = ulFlags;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_MAP_PADDR,
        (void *)&mapPaddrData);

    if (err)
        return -EIO;

    if (mapPaddrData.retVal)
        return mapPaddrData.retVal;

    *pulVaddr = mapPaddrData.vaddr;
    return 0;
}

int _tzioc_ioctl_unmap_paddr(
    struct tzioc_client *pClient,
    uint32_t ulPaddr,
    uint32_t ulSize)
{
    struct tzioc_ioctl_unmap_paddr_data unmapPaddrData;
    int err = 0;

    unmapPaddrData.hClient = pClient->hKlient;
    unmapPaddrData.paddr = ulPaddr;
    unmapPaddrData.size  = ulSize;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_UNMAP_PADDR,
        (void *)&unmapPaddrData);

    if (err)
        return -EIO;

    if (unmapPaddrData.retVal)
        return unmapPaddrData.retVal;

    return 0;
}

int _tzioc_ioctl_map_paddrs(
    struct tzioc_client *pClient,
    uint8_t ucCount,
    struct tzioc_mem_region *pRegions)
{
    struct tzioc_ioctl_map_paddrs_data mapPaddrsData;
    int err = 0;

    mapPaddrsData.hClient = pClient->hKlient;
    mapPaddrsData.count = ucCount;
    mapPaddrsData.regionsAddr = (uint32_t)pRegions;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_MAP_PADDRS,
        (void *)&mapPaddrsData);

    if (err)
        return -EIO;

    if (mapPaddrsData.retVal)
        return mapPaddrsData.retVal;

    return 0;
}

int _tzioc_ioctl_unmap_paddrs(
    struct tzioc_client *pClient,
    uint8_t ucCount,
    struct tzioc_mem_region *pRegions)
{
    struct tzioc_ioctl_unmap_paddrs_data unmapPaddrsData;
    int err = 0;

    unmapPaddrsData.hClient = pClient->hKlient;
    unmapPaddrsData.count = ucCount;
    unmapPaddrsData.regionsAddr = (uint32_t)pRegions;

    err = ioctl(
        pClient->fd,
        TZIOC_IOCTL_UNMAP_PADDRS,
        (void *)&unmapPaddrsData);

    if (err)
        return -EIO;

    if (unmapPaddrsData.retVal)
        return unmapPaddrsData.retVal;

    return 0;
}
