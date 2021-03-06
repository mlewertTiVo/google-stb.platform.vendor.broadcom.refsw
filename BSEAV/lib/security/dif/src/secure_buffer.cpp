/******************************************************************************
 *    (c)2015 Broadcom Corporation
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
 * Module Description:
 *
 * DRM Integration Framework
 *
 *****************************************************************************/
#include "sage_srai.h"

#include "streamer.h"

BDBG_MODULE(secure_buffer);
#undef LOGE
#undef LOGW
#undef LOGD
#undef LOGV
#define LOGE BDBG_ERR
#define LOGW BDBG_WRN
#define LOGD BDBG_MSG
#define LOGV BDBG_MSG

#ifdef USE_SECURE_PLAYBACK
using namespace dif_streamer;

SecureBuffer::~SecureBuffer()
{
    if (m_dmaHandle != NULL) {
        if (m_dmaJob) {
            NEXUS_DmaJob_Destroy(m_dmaJob);
            m_dmaJob = NULL;
        }
        NEXUS_Dma_Close(m_dmaHandle);
        m_dmaHandle = NULL;
    }

    if (!m_givenBuffer) {
        SRAI_Memory_Free(m_data);
        m_data = NULL;
    }
}

bool SecureBuffer::Initialize()
{
    m_dmaHandle = NULL;
    m_dmaJob = NULL;

    if (m_data == NULL) {
        uint8_t *pBuf = NULL;

        pBuf = SRAI_Memory_Allocate(m_size,
                SRAI_MemoryType_SagePrivate);

        if (pBuf == NULL) {
            LOGE(("%s: failed to allocate secure memory", __FUNCTION__));
            return false;
        }
        m_data = pBuf;
    }

    return true;
}

void SecureBuffer::Copy(uint32_t offset, uint8_t* dataToCopy, size_t size)
{
    PrivateCopy(m_data + offset, dataToCopy, size, true);
}

void SecureBuffer::Copy(uint32_t offset, IBuffer* bufToCopy, size_t size)
{
    if(bufToCopy->IsSecure()) {
        PrivateCopy(m_data + offset, bufToCopy->GetPtr(), size, false);
    } else {
        PrivateCopy(m_data + offset, bufToCopy->GetPtr(), size, true);
    }
}

void SecureBuffer::PrivateCopy(void *pDest, const void *pSrc, size_t nSize, bool flush)
{
    NEXUS_Error rc;

    LOGV(("%s: dest:%p, src:%p, size:%d", __FUNCTION__, pDest, pSrc, nSize));

    if (m_dmaJob == NULL) {
        LOGD(("%s: setting up DmaJob", __FUNCTION__));
        m_dmaHandle = NEXUS_Dma_Open(NEXUS_ANY_ID, NULL);
        if (m_dmaHandle == NULL) {
            LOGE(("%s: Failed to NEXUS_Dma_Open !!!", __FUNCTION__));
            return;
        }

        NEXUS_DmaJobSettings dmaJobSettings;
        NEXUS_DmaJob_GetDefaultSettings(&dmaJobSettings);
        dmaJobSettings.completionCallback.callback = NULL;
        dmaJobSettings.bypassKeySlot = NEXUS_BypassKeySlot_eGR2R;
        m_dmaJob = NEXUS_DmaJob_Create(m_dmaHandle, &dmaJobSettings);

        if (m_dmaJob == NULL) {
            LOGE(("%s: Failed to NEXUS_DmaJob_Create !!!", __FUNCTION__));
            return;
        }
    }

    NEXUS_DmaJobBlockSettings blockSettings;
    NEXUS_DmaJob_GetDefaultBlockSettings(&blockSettings);
    blockSettings.pSrcAddr = pSrc;
    blockSettings.pDestAddr = pDest;
    blockSettings.blockSize = nSize;
    blockSettings.cached = false;

    if (flush)
        NEXUS_FlushCache(blockSettings.pSrcAddr, blockSettings.blockSize);

    rc = NEXUS_DmaJob_ProcessBlocks(m_dmaJob, &blockSettings, 1);
    if (rc == NEXUS_DMA_QUEUED) {
        for (;;) {
            NEXUS_DmaJobStatus status;
            rc = NEXUS_DmaJob_GetStatus(m_dmaJob, &status);
            if (rc != NEXUS_SUCCESS) {
                LOGE(("%s: DmaJob_GetStatus err=%d", __FUNCTION__, rc));
                return;
            }
            if (status.currentState == NEXUS_DmaJobState_eComplete ) {
                break;
            }
            BKNI_Delay(1);
        }
    }
    else {
        LOGE(("%s: error in dma transfer, err:%d", __FUNCTION__, rc));
        return;
    }

    if (flush)
        NEXUS_FlushCache(blockSettings.pSrcAddr, blockSettings.blockSize);

    return;
}
#endif
