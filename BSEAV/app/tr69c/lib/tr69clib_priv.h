/******************************************************************************
 *    (c)2014 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 *****************************************************************************/
#ifndef __TR69CLIB_PRIV_H__
#define __TR69CLIB_PRIV_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tr69clib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LEN_STR 128
#define MAX_LEN_ARG 256
#define MAX_LEN_REG 64

typedef struct b_tr69c_client
{
    char name[MAX_LEN_STR];
    char pid[MAX_LEN_STR];
    char arglist[MAX_LEN_ARG];
    int connection_fd;
    pthread_t connection_id;
    BKNI_EventHandle request_event;
    BKNI_EventHandle response_event;
    BKNI_MutexHandle connection_mutex;
    void *send_data;
    int send_data_len;
    void *recv_data;
    int recv_data_len;
} *b_tr69c_client_t;

typedef struct b_tr69c_client_registry
{
    b_tr69c_client_t client;
} *b_tr69c_client_registry_t;

typedef struct b_tr69c_server
{
    int socket_fd;
    struct sockaddr_un address;
    pthread_t listener_id;
    int num_client;
    b_tr69c_client_registry_t client_registry;
} *b_tr69c_server_t;

typedef struct b_tr69c_data
{
    enum b_tr69c_type type;
    union b_tr69c_info info;
} *b_tr69c_data_t;

b_tr69c_server_t b_tr69c_server_init(void);
int b_tr69c_server_get_client(b_tr69c_client_t curr, void *send_data, int send_data_len, void *recv_data, int recv_data_len);
int b_tr69c_server_set_client(b_tr69c_client_t curr, void *send_data, int send_data_len);
int b_tr69c_get_client_registry(b_tr69c_client_registry_t *registry);
void b_tr69c_get_client_count(int *count);
void b_tr69c_server_uninit(b_tr69c_server_t tr69c_server);

#ifdef __cplusplus
}
#endif

#endif /* __TR69CLIB_PRIV_H__ */
