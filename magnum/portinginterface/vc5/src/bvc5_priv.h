/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 *  This program is the proprietary software of Broadcom and/or its licensors,
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
 ******************************************************************************/
#ifndef BVC5_PRIV_H__
#define BVC5_PRIV_H__

#include "bkni.h"
#include "bkni_multi.h"

#include "bvc5_fence_priv.h"
#include "bvc5_client_priv.h"
#include "bvc5_scheduler_state_priv.h"
#include "bvc5_scheduler_priv.h"
#include "bvc5_hardware_priv.h"
#include "bvc5_bin_pool_priv.h"
#include "bvc5_internal_job_priv.h"
#include "bvc5_perf_counters_priv.h"
#include "bvc5_event_monitor_priv.h"
#include "bvc5_os_priv.h"

#if defined(BVC5_HARDWARE_SIMPENROSE)
#include "bvc5_simpenrose_priv.h"
#endif

/* Latency for lockup condition check */
#define BVC5_WATCHDOG_TIMEOUT          20
#define BVC5_POWERDOWN_TIMEOUT         (100 * BVC5_WATCHDOG_TIMEOUT)
#define BVC5_TIMEOUTS_FOR_STALL_CHECK  50
#define BVC5_PURGE_LIMIT               100

#define BVC5_MAX_CORES                 8

/* We will stop issuing bin jobs when the binner is this many jobs ahead of the renderer */
#define BVC5_MAX_BINNER_RUNAHEAD       15

#define BVC5_CACHE_FLUSH_CORE (\
      BVC5_CACHE_CLEAR_SIC\
   |  BVC5_CACHE_CLEAR_SUC\
   |  BVC5_CACHE_CLEAR_L1TD\
   |  BVC5_CACHE_CLEAR_L1TC\
   |  BVC5_CACHE_CLEAR_VCD\
   |  BVC5_CACHE_CLEAR_L2C\
   |  BVC5_CACHE_FLUSH_L2T)

#define BVC5_CACHE_CLEAN_CORE (\
      BVC5_CACHE_CLEAN_L1TD\
   |  BVC5_CACHE_CLEAN_L2T)

#define BVC5_CACHE_FLUSH_HUB (BVC5_CACHE_FLUSH_L3C | BVC5_CACHE_CLEAR_GCA)
#define BVC5_CACHE_CLEAN_HUB (BVC5_CACHE_CLEAN_L3C)
#define BVC5_CACHE_FLUSH_ALL (BVC5_CACHE_FLUSH_CORE | BVC5_CACHE_FLUSH_HUB)
#define BVC5_CACHE_CLEAN_ALL (BVC5_CACHE_CLEAN_CORE | BVC5_CACHE_CLEAN_HUB)

#if V3D_VER_AT_LEAST(4,1,34,0)
typedef struct BVC5_P_TileState
{
   BMMA_Block_Handle          hTileState;
   BMMA_DeviceOffset          uiTileStateOffset;
   uint32_t                   uiTileStateSize;
} BVC5_P_TileState;
#endif


typedef struct BVC5_P_Handle
{
   BCHP_Handle                hChp;
   BREG_Handle                hReg;

   BMMA_Heap_Handle           hMMAHeap;
   BMMA_Heap_Handle           hSecureMMAHeap;
   uint64_t                   ulDbgHeapOffset;
   unsigned                   uDbgHeapSize;

   BCHP_MemoryInfo            sMemInfo;

   BINT_Handle                hInt;
   BVC5_OpenParameters        sOpenParams;

   BKNI_MutexHandle     hModuleMutex;              /* used to protect the worker queue and the posting of messages         */

   /* Records if the SAGE put us in secure mode */
   bool                 bSecure;

   /* Current hardware unit states */
   BVC5_P_CoreState     psCoreStates[BVC5_MAX_CORES];
   uint32_t             uiNumCores;

   /* Current TFU status */
   BVC5_P_TFUState      sTFUState;

   /* Counters */
   uint32_t             uiNextClientId;            /* counter for client ids                                               */

   /* Scheduler control */
   bool                 bSchedulerRunning;         /* used in termination                                                  */
   BKNI_EventHandle     hSchedulerSyncEvent;       /* created by the caller and used to sync thread creation/destruction   */
   BKNI_EventHandle     hSchedulerTerminateEvent;  /* signaled when the worker can shutdown                                */

   /* Interrupt handling */
   BKNI_EventHandle     hSchedulerWakeEvent;
   uint32_t             uiInterruptReason;         /* TODO multi-core: per core interrupt reason                           */
   uint32_t             uiTFUInterruptReason;      /* Interrupt raised by TFU?                                             */
   uint32_t             uiPurgeCounter;            /* When it reaches BVC5_PURGE_LIMIT, try to reduce bin memory           */

   /* Fences */
   BVC5_FenceArrayHandle     hFences;           /* holds the fence pool                                                 */

   BVC5_ClientMapHandle      hClientMap;        /* holds a map of all the current clients                               */

   BVC5_P_SchedulerState     sSchedulerState;

   /* Bin memory management */
   BVC5_BinPoolHandle         hBinPool;
   BVC5_BinPoolHandle         hSecureBinPool;

   /* Standby */
   bool                       bEnterStandby;     /* Standby has been requested; stop issuing work to hardware  */
   bool                       bInStandby;        /* Requested standby status                                   */

   /* Performance counters */
   BVC5_P_PerfCounters        sPerfCounters;

   /* Event monitor */
   BVC5_P_EventMonitor        sEventMonitor;
   BKNI_MutexHandle           hEventMutex;       /* used to protect the event queue                            */

   /* Callbacks */
   BVC5_Callbacks             sCallbacks;        /* Callbacks to higher layer e.g. Nexus                       */

#if defined(BVC5_HARDWARE_SIMPENROSE)
   BVC5_SimpenroseHandle      hSimpenrose;
#endif

   BINT_CallbackHandle        callback_intctl;
   BINT_CallbackHandle        callback_hub_intctl;

   BMMA_Block_Handle          hGMPTable;
   BMMA_DeviceOffset          uiGMPOffset;

   bool                       bLockupReported;

   bool                       bToggling;         /* Set when security toggle code is invoked                   */

   /* MMU Bin Memory support */
   int64_t                    iUnsecureBinTranslation;
   int64_t                    iSecureBinTranslation;
   BMMA_Block_Handle          hMmuSafePage;
   BMMA_DeviceOffset          uiMmuSafePageOffset;

#if V3D_VER_AT_LEAST(4,1,34,0)
   BVC5_P_TileState           sTileState[2];             /* 0 = unsecure, 1 = secure */
#endif

   bool                       bCollectLoadStats;         /* Do we want to gather load statistics for each client?                */
   uint32_t                   uiCyclesPerUs;             /* Used for both load statistics and event monitoring                   */
} BVC5_P_Handle;

BVC5_BinPoolHandle BVC5_P_GetBinPool(
   BVC5_Handle hVC5
);

BMMA_Heap_Handle BVC5_P_GetMMAHeap(
   BVC5_Handle hVC5
);

uint32_t BVC5_P_TranslateBinAddress(
   BVC5_Handle hVC5,
   uint64_t    uiAddr,
   bool        bSecure
);

#if V3D_VER_AT_LEAST(4,1,34,0)
bool BVC5_P_AllocTileState(
   BVC5_P_TileState *psTileState,
   BMMA_Heap_Handle hMMAHeap,
   uint32_t size
);

void BVC5_P_FreeTileState(
   BVC5_P_TileState *psTileState
);
#endif

/* SoC hardware quirks that the userspace drivers need to know about, these
 * must match the definitions in libs/platform/v3d_driver_api.h
 */
#define BVC5_P_SOC_QUIRK_HWBCM_7260_81 (1L << 0)

#endif /* BVC5_PRIV_H__ */
