/******************************************************************************
 * Copyright (C) 2018 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 ******************************************************************************/

/* base modules */
#include "bstd.h"           /* standard types */
#include "berr.h"           /* error code */
#include "bdbg.h"           /* debug interface */
#include "bkni.h"           /* kernel interface */

#include "bmuxlib_debug.h"
#include "bmuxlib_alloc.h"
#include "bmuxlib_ts.h"
#include "bmuxlib_ts_priv.h"

#include "bmuxlib_input.h"

#if BMUXLIB_TS_P_TEST_MODE
#include <stdio.h>
#endif

BDBG_MODULE(BMUXLIB_TS);
BDBG_FILE_MODULE(BMUXLIB_TS_FINISH);
BDBG_FILE_MODULE(BMUXLIB_TS_PENDING);
BDBG_FILE_MODULE(BMUXLIB_TS_QUEUED);
BDBG_FILE_MODULE(BMUXLIB_TS_TRANSPORT);
BDBG_FILE_MODULE(BMUXLIB_TS_MSP);
BDBG_FILE_MODULE(BMUXLIB_TS_MEMORY);
BDBG_FILE_MODULE(BMUXLIB_TS_STATS);

BDBG_OBJECT_ID(BMUXlib_TS_Legacy_P_Context);

static bool BMUXlib_TS_P_ValidateStartSettings(BMUXlib_TS_StartSettings *pstStartSettings, bool bUseDefaults);
static bool BMUXlib_TS_P_ValidateMuxSettings(BMUXlib_TS_MuxSettings *pstMuxSettings, BMUXlib_TS_StartSettings *pstStartSetting, bool bUseDefaults);
static bool BMUXlib_TS_P_ValidateSystemDataBitrate(BMUXlib_TS_MuxSettings *pstMuxSettings, const BMUXlib_TS_StartSettings *pstStartSettings, bool bUseDefaults);
static BERR_Code BMUXlib_TS_P_ValidateSystemDataBuffers(const BMUXlib_TS_SystemData *astSystemDataBuffer, size_t uiCount);

static void BMUXlib_TS_P_GetMemoryConfigTotal(const BMUXlib_TS_P_MemoryConfig *pstMemoryConfig, BMUXlib_TS_P_MemoryConfigTotal *pstMemoryConfigTotal);
static void BMUXlib_TS_P_Input_GetMemoryConfig(BMUXlib_TS_P_InputType eInputType, unsigned uiNumFrames, unsigned uiNumPIDs, unsigned uiNumMSPs, bool bSupportTTS, BMUXlib_TS_P_MemoryConfig *pstMemoryConfig);
static void BMUXlib_TS_P_GetMemoryConfig(const BMUXlib_TS_MuxConfig *pstMuxConfig, BMUXlib_TS_P_MemoryConfig *pstMemoryConfig);
static void BMUXlib_TS_P_GetMaxMemoryConfig(const BMUXlib_TS_MuxConfig *astMuxConfig, unsigned uiNumMuxConfig, BMUXlib_TS_P_MemoryConfig *pstMemoryConfig);
static void BMUXlib_TS_P_GetDefaultMemorySize(BMUXlib_TS_MemoryConfig *pstMemoryConfig);
static void BMUXlib_TS_P_GetBufferConfigFromMemoryConfig(const BMUXlib_TS_MemoryConfig *pstMemoryConfig, const BMUXlib_TS_P_MemoryBuffers *pstMemoryBuffers, BMUXlib_TS_P_BufferConfig *pstBufferConfig);

static BERR_Code BMUXlib_TS_P_AllocateMemory(BMUXlib_TS_Legacy_Handle hMuxTS, BMMA_Heap_Handle hMma, BMUXlib_TS_P_BufferConfig *pstMemoryConfig);
static void BMUXlib_TS_P_FreeMemory(BMUXlib_TS_Legacy_Handle hMuxTS, BMMA_Heap_Handle hMma);
static void BMUXlib_TS_P_ResetResources(BMUXlib_TS_Legacy_Handle hMuxTS);
static BERR_Code BMUXlib_TS_P_AllocateResources(BMUXlib_TS_Legacy_Handle hMuxTS, const BMUXlib_TS_P_MemoryConfig *pstMemoryConfig);
static void BMUXlib_TS_P_FreeResources(BMUXlib_TS_Legacy_Handle hMuxTS);

static BERR_Code BMUXlib_TS_P_RelocatePreQSystemData(BMUXlib_TS_Legacy_Handle hMuxTS);
static BERR_Code BMUXlib_TS_P_SetupMCPB(BMUXlib_TS_Legacy_Handle hMuxTS);
static void BMUXlib_TS_P_TeardownMCPB(BMUXlib_TS_Legacy_Handle hMuxTS);

static void BMUXlib_TS_P_Reset(BMUXlib_TS_Legacy_Handle hMuxTS);
static BERR_Code BMUXlib_TS_P_Finish(BMUXlib_TS_Legacy_Handle hMuxTS, const BMUXlib_TS_FinishSettings *pstFinishSettings);
static BERR_Code BMUXlib_TS_P_AssignTransportChannel(BMUXlib_TS_Legacy_Handle hMuxTS, unsigned uiTransportChannelIndex, unsigned uiPID, unsigned uiPIDChannelIndex, bool bIsTS, BMUXlib_TS_TransportChannelInterface *pChannelInterface);

/******************/
/* Create/Destroy */
/******************/

#define BMUXLIB_TS_P_SYSTEM_DATA_PRE_Q_COUNT 5
#define BMUXLIB_TS_P_NOMINAL_SYSTEM_DATA_BITRATE 2000000
#define BMUXLIB_TS_P_NOMINAL_PCR_INTERVAL 20
#define BMUXLIB_TS_P_NOMINAL_USERDATA_PIDS 4
#define BMUXLIB_TS_P_MAX_NRT_SPEED 4
#define BMUXLIB_TS_P_MSP_COUNT 2
#define BMUXLIB_TS_P_MIN_DESCRIPTORS_PER_FRAME 2 /* for PES and CDB */
#define BMUXLIB_TS_P_EXTRA_DESCRIPTORS_PER_MSP 2 /* partial frame and/or CDB wrap */
#define BMUXLIB_TS_P_MAX_VIDEO_FPS 60
#define BMUXLIB_TS_P_MAX_AUDIO_FPS 50

static void
BMUXlib_TS_P_GetMemoryConfigTotal(
   const BMUXlib_TS_P_MemoryConfig *pstMemoryConfig,
   BMUXlib_TS_P_MemoryConfigTotal *pstMemoryConfigTotal
)
{
   BDBG_ASSERT( pstMemoryConfig );
   BDBG_ASSERT( pstMemoryConfigTotal );

   BKNI_Memset( pstMemoryConfigTotal, 0, sizeof( *pstMemoryConfigTotal ) );
   {
      BMUXlib_TS_P_MemoryEntryType eMemoryEntryType;
      for ( eMemoryEntryType = 0; eMemoryEntryType < BMUXlib_TS_P_MemoryEntryType_eMax; eMemoryEntryType++ )
      {
         BMUXlib_TS_P_InputType eInputType;
         for ( eInputType=0; eInputType<BMUXlib_TS_P_InputType_eMax; eInputType++ )
         {
            BMUXlib_TS_P_MemoryType eMemoryType;
            for ( eMemoryType=0; eMemoryType<BMUXlib_TS_P_MemoryType_eMax; eMemoryType++ )
            {
               pstMemoryConfigTotal->astMemoryEntry[eMemoryEntryType].stMemoryConfig.stBufferInfo[eMemoryType].uiSize += pstMemoryConfig->astMemoryEntry[eMemoryEntryType][eInputType].stMemoryConfig.stBufferInfo[eMemoryType].uiSize;
            }
            pstMemoryConfigTotal->astMemoryEntry[eMemoryEntryType].uiCount += pstMemoryConfig->astMemoryEntry[eMemoryEntryType][eInputType].uiCount;
         }
      }
   }

   pstMemoryConfigTotal->stMemoryConfig = pstMemoryConfig->stMemoryConfig;
}

static void
BMUXlib_TS_P_Input_GetMemoryConfig(
   BMUXlib_TS_P_InputType eInputType,
   unsigned uiNumFrames,
   unsigned uiNumPIDs,
   unsigned uiNumMSPs,
   bool bSupportTTS,
   BMUXlib_TS_P_MemoryConfig *pstMemoryConfig
   )
{
   /************/
   /* CDB Data */
   /************/
   /* We need one PES header for each frame */
   pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader][eInputType].uiCount += uiNumFrames;
   /* We need at least 2 transport descriptors per frame for PES + CDB.
    */
   pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][eInputType].uiCount += (uiNumFrames * BMUXLIB_TS_P_MIN_DESCRIPTORS_PER_FRAME);

   /* We need then 1 additional transport descriptor per MSP for each of:
    *    1) partial frame
    *    2) cdb wrap
    */
   pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][eInputType].uiCount += ( BMUXLIB_TS_P_EXTRA_DESCRIPTORS_PER_MSP * uiNumMSPs );

   if ( BMUXlib_TS_P_InputType_eVideo == eInputType )
   {
      /****************/
      /* MTU BPP Data */
      /****************/
      /* SW7425-659:  We need to account for worst case MUX_TIMESTAMP_UPDATE BPP which is once every frame ( 1 BPP and 1 transport descriptor per frame ) */
      if ( true == bSupportTTS )
      {
         /* SW7425-5180: Use a separate MTU BPP memory resource to handle 16x consecutive BPPs to maintain CC count */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eMTUBPP][eInputType].uiCount += uiNumFrames;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][eInputType].uiCount += uiNumFrames;
      }
   }

   /*****************/
   /* LAST BPP Data */
   /*****************/
   /* Allocate LAST BPP + transport descriptor */
   pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eBPP][eInputType].uiCount += 1;
   pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][eInputType].uiCount += 1;

   /*****************/
   /* Multiple PIDs */
   /*****************/
   {
      BMUXlib_TS_P_MemoryEntryType eEntryType;

      for ( eEntryType = 0; eEntryType < BMUXlib_TS_P_MemoryEntryType_eMax; eEntryType++ )
      {
         /* We need to multiply the totals by the number of PIDs */
         pstMemoryConfig->astMemoryEntry[eEntryType][eInputType].uiCount *= uiNumPIDs;
      }
   }
}

static void
BMUXlib_TS_P_GetMemoryConfig(
   const BMUXlib_TS_MuxConfig *pstMuxConfig,
   BMUXlib_TS_P_MemoryConfig *pstMemoryConfig
)
{
   bool rc1, rc2;
   BMUXlib_TS_MuxSettings stMuxSettings, *pstMuxSettings = &stMuxSettings;
   BMUXlib_TS_StartSettings stStartSettings, *pstStartSettings = &stStartSettings;

   BDBG_ENTER( BMUXlib_TS_P_GetMemoryConfig );

   BDBG_ASSERT(pstMuxConfig);
   BDBG_ASSERT(pstMemoryConfig);

   BKNI_Memset( pstMemoryConfig, 0, sizeof( *pstMemoryConfig ) );
   /* copy the settings, so that we can override invalid settings with defaults */
   stMuxSettings = pstMuxConfig->stMuxSettings;
   stStartSettings = pstMuxConfig->stMuxStartSettings;

   /* validate settings, but only print warnings for invalid values (use defaults)
      (return code is used to indicate that a change was made to a setting) */
   /* IMPORTANT: Start settings must be validated first.  This is due to the dependency
      of the mux settings check (system data bitrate check) on PCR interval */
   rc1 = BMUXlib_TS_P_ValidateStartSettings(pstStartSettings, true);
   rc2 = BMUXlib_TS_P_ValidateMuxSettings(pstMuxSettings, pstStartSettings, true);
   if (rc1 || rc2)
      BDBG_WRN(("GetMemoryConfig:: Settings have been altered (memory calculations may not match specified settings)"));

   /* Determine number of descriptors required */
   /* MSP: 100 ms max
    * Video: 60 frames/sec max
    * Audio: ~50 frames/sec max
    * System: 1 Mbps/sec
    *    pcr: 20 pcrs/sec max (50 ms interval)
    *    system/user data: 1 Mbps/sec * sec/1000ms * 100ms/MSP * byte/8 bits * packet/188 bytes = ~ 67 packets/sec max
    *
    *    - 1 PES Header/frame
    *    - 2 Transport Descriptors/frame
    *    - 2 Additional Transport Descriptors for:
    *       - partial frame and/or
    *       - CDB wrap and/or
    *    - 1 MUX_TIMESTAMP_UPDATE BPP per A/V frame
    *    - 1 additional transport descriptor per A/V BPP
    *    - 1 BTP + transport descriptor per MSP
    *    - 1 additional transport descriptor per BTP
    *    video: = 60 frames/sec * sec/1000ms * 100ms/MSP = 6 frames/MSP
    *       --> 6 PES/MSP
    *       --> 12+6+3 Transport Descriptors/MSP
    *       --> 6 BPP
    *    audio:
    *       --> 5 PES/MSP
    *       --> 10+5+3 Transport Descriptors/MSP
    *       --> 5 BPP
    *    system:
    *       --> 20+1 TS Packets/MSP (pcr + btp)
    *       --> 67+1 Transport Descriptors/MSP (pcr + btp)
    *       --> 67 System Data Buffers/MSP
    *
    */

   {
      /* For resource allocation purposes, we need to make sure we have enough resources to schedule enough frames for the
       * worst case next execution time.  This is the MSP + service latency tolerance (SLT).  I.e. If DoMux() is called at T=0,
       * ideally, we should be executed again at T=MSP, however, because of host CPU latency, we need to tolerate up to
       * T=MSP+SLT.
       */
      unsigned uiWorstCaseNextExecutionTime = (pstStartSettings->uiServicePeriod + pstStartSettings->uiServiceLatencyTolerance);
      unsigned uiTotalMSPCount = BMUXLIB_TS_P_MSP_COUNT;

      /* SW7435-680: Account for delay of descriptors because of the mux delay.
       * E.g. if service latency tolerance is 20ms, MSP=50ms, mux_delay=70ms, then descriptors
       * sent on MSP[0] won't be released until at least 70ms later, which is MSP[2]. */
      uiTotalMSPCount += BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP(uiWorstCaseNextExecutionTime, pstStartSettings->uiServicePeriod);

      /* SW7435-535: Account for additional MSP delay because of pre-mature descriptor release work-around
       * Because we are delaying the release of descriptors, we may not get back all the descriptors until
       * the next MSP */
      uiTotalMSPCount += 1;

      if ( true == stStartSettings.bSupportTTS )
      {
         /* SW7425-659: Increase by an additional MSP count to account for latency MTU BPP is enabled in RT mode */
         uiTotalMSPCount += 1;
      }

      /* Video */
      if ( 0 != pstStartSettings->uiNumValidVideoPIDs )
      {
         /* SW7425-5180/SW7425-4707: In order to calculate the worst case number of video frames we need to have queued, we
          * need to consider the end-to-end delay (a.k.a A2PDelay, Dee) and the worst case encoder output frame rate (60fps).
          * We need to consider end-to-end delay because the encoder may generate frames at a rate faster than the frame rate
          * when the content is simple/static.
          */
         unsigned uiDurationms = (pstStartSettings->uiA2PDelay / BMUXLIB_TS_P_SCALE_MS_TO_27MHZ) + (uiWorstCaseNextExecutionTime * uiTotalMSPCount);
         unsigned uiNumFrames =  BMUXLIB_TS_P_MAX(2,BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP((BMUXLIB_TS_P_MAX_VIDEO_FPS * uiDurationms),1000));
         if ( true == pstStartSettings->bNonRealTimeMode ) uiNumFrames *= BMUXLIB_TS_P_MAX_NRT_SPEED;

         BMUXlib_TS_P_Input_GetMemoryConfig(
            BMUXlib_TS_P_InputType_eVideo,
            uiNumFrames,
            pstStartSettings->uiNumValidVideoPIDs,
            uiTotalMSPCount,
            pstStartSettings->bSupportTTS,
            pstMemoryConfig
            );
      }

      /* Audio */
      if ( 0 != pstStartSettings->uiNumValidAudioPIDs )
      {
         unsigned uiNumFrames = BMUXLIB_TS_P_MAX(2,BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP((BMUXLIB_TS_P_MAX_AUDIO_FPS * uiWorstCaseNextExecutionTime),1000)) * uiTotalMSPCount;
         if ( true == pstStartSettings->bNonRealTimeMode ) uiNumFrames *= BMUXLIB_TS_P_MAX_NRT_SPEED;

         BMUXlib_TS_P_Input_GetMemoryConfig(
            BMUXlib_TS_P_InputType_eAudio,
            uiNumFrames,
            pstStartSettings->uiNumValidAudioPIDs,
            uiTotalMSPCount,
            pstStartSettings->bSupportTTS,
            pstMemoryConfig
            );
      }

      /* System */
      {
         unsigned uiNumFrames = BMUXLIB_TS_P_MAX(2,BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP(pstMuxSettings->uiSystemDataBitRate * uiWorstCaseNextExecutionTime,1000 * 8 * 188)) * uiTotalMSPCount;
         if ( true == pstStartSettings->bNonRealTimeMode ) uiNumFrames *= BMUXLIB_TS_P_MAX_NRT_SPEED;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eSystem].uiCount = uiNumFrames;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eSystemData][BMUXlib_TS_P_InputType_eSystem].uiCount = uiNumFrames;
      }
      {
         unsigned uiNumPcrs = BMUXLIB_TS_P_MAX(2,BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP(uiWorstCaseNextExecutionTime, ( 0 != pstStartSettings->stPCRData.uiInterval ) ? pstStartSettings->stPCRData.uiInterval : BMUXLIB_TS_P_MUX_PCR_INTERVAL_DEFAULT)) * uiTotalMSPCount;
         if ( true == pstStartSettings->bNonRealTimeMode ) uiNumPcrs *= BMUXLIB_TS_P_MAX_NRT_SPEED;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket][BMUXlib_TS_P_InputType_eSystem].uiCount = uiNumPcrs;
      }

      if ( true == stStartSettings.bSupportTTS )
      /* SW7425-659: We need to send one MTU BTP with the very first system data packet to ensure a non-zero deltaATS */
      {
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eSystem].uiCount += 1;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket][BMUXlib_TS_P_InputType_eSystem].uiCount += 1;
      }

      /* SW7425-4764: Allocate a single TS Packet for use as a NULL Packet and a BPP for use as a dummy PES frame */
      {
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket][BMUXlib_TS_P_InputType_eSystem].uiCount += 1;
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eBPP][BMUXlib_TS_P_InputType_eSystem].uiCount += 1;
      }

      /* TS User Data */
      {
         /* SW7425-4340: allow for encoder delay (A2P) to allow for buffering of userdata due to diff between encode ESCR and encode PTS */
         unsigned uiDurationms = (pstStartSettings->uiA2PDelay / BMUXLIB_TS_P_SCALE_MS_TO_27MHZ) + (uiWorstCaseNextExecutionTime * uiTotalMSPCount);
         unsigned uiNumFrames =  BMUXLIB_TS_P_MAX(2,BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP((BMUXLIB_TS_P_MAX_VIDEO_FPS * uiDurationms),1000));
         unsigned uiNumPackets = uiNumFrames * BMUXLIB_TS_USERDATA_MAX_PKTS_PER_VID;
         if (true == pstStartSettings->bNonRealTimeMode) uiNumPackets *= BMUXLIB_TS_P_MAX_NRT_SPEED;

         /* Need 1 unwrap packet per PID */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataUnwrap][BMUXlib_TS_P_InputType_eSystem].uiCount = pstStartSettings->uiNumValidUserdataPIDs;
         /* 1 pending Q entry per packet */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserData][BMUXlib_TS_P_InputType_eSystem].uiCount = uiNumPackets * pstStartSettings->uiNumValidUserdataPIDs;
         /* 1 release Q entry per segment */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataReleaseQ][BMUXlib_TS_P_InputType_eSystem].uiCount = uiNumPackets * BMUXLIB_TS_USERDATA_MAX_SEGMENTS * pstStartSettings->uiNumValidUserdataPIDs;
         /* 2 PTS entries per TS packet (one for PTS, one for DTS - assume worst case of 1 PES = 1 TS packet) */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataPTS][BMUXlib_TS_P_InputType_eSystem].uiCount = 2 * uiNumPackets * pstStartSettings->uiNumValidUserdataPIDs;
         /* 1 Transport Desc per Release Q entry (NOTE: one entry already accounted for by System Data) */
         pstMemoryConfig->astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eSystem].uiCount += (uiNumPackets * (BMUXLIB_TS_USERDATA_MAX_SEGMENTS-1) * pstStartSettings->uiNumValidUserdataPIDs);
      }
   }

   {
      BMUXlib_TS_P_MemoryEntryType eMemoryEntryType;

      for ( eMemoryEntryType = 0; eMemoryEntryType < BMUXlib_TS_P_MemoryEntryType_eMax; eMemoryEntryType++ )
      {
         BMUXlib_TS_P_InputType eInputType;

         for ( eInputType=0; eInputType<BMUXlib_TS_P_InputType_eMax; eInputType++ )
         {
#define BMUXLIB_TS_P_ADD_MEMORY_SIZE(size,p,i,j,type) (p)[i][j].stMemoryConfig.stBufferInfo[type].uiSize += BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP( size * (p)[i][j].uiCount, 4096 ) * 4096

            switch ( eMemoryEntryType )
            {
               case BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor:
                  /* Transport Descriptors */
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_TransportDescriptor) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  /* Transport Descriptor Temp Array */
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXlib_TS_TransportDescriptor ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXlib_TS_P_TransportDescriptorMetaData ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_ePESHeader:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_PESHeader) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eTransportPacket:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_TSPacket) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eSystemData:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eBPP:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_BPPData) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eUserData:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_UserdataPending) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eUserDataPTS:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_UserdataPTSEntry) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eUserDataUnwrap:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_TSPacket) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eUserDataReleaseQ:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXlib_TS_P_UserdataReleaseQEntry ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eSystem);
                  break;

               case BMUXlib_TS_P_MemoryEntryType_eMTUBPP:
                  BMUXLIB_TS_P_ADD_MEMORY_SIZE(sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_MTUBPPData) ), pstMemoryConfig->astMemoryEntry,eMemoryEntryType,eInputType,BMUXlib_TS_P_MemoryType_eShared);
                  break;

               /* coverity[dead_error_begin] */
               /* SW7435-1313: Intentional dead code: catch-all for added enums */
               default:
                  BDBG_ERR(("Unsupported memory entry type (%d)", eMemoryEntryType ));
                  break;
            } /* end: switch memory entry type */

            {
               BMUXlib_TS_P_MemoryType eMemoryType;
               for ( eMemoryType=0; eMemoryType<BMUXlib_TS_P_MemoryType_eMax; eMemoryType++ )
               {
                  pstMemoryConfig->stMemoryConfig.stBufferInfo[eMemoryType].uiSize += pstMemoryConfig->astMemoryEntry[eMemoryEntryType][eInputType].stMemoryConfig.stBufferInfo[eMemoryType].uiSize;
               }
            }
         } /* end: for input type */
      } /* end: for memory entry type */
   }

   BDBG_LEAVE( BMUXlib_TS_P_GetMemoryConfig );
}

#define BMUXLIB_TS_P_ASSIGN_IF_LARGER(a,b,field) if ((a)->field < (b)->field) { ((a)->field = (b)->field); }

static void
BMUXlib_TS_P_GetMaxMemoryConfig(
   const BMUXlib_TS_MuxConfig *astMuxConfig, /* Array of possible configurations */
   unsigned uiNumMuxConfig,
   BMUXlib_TS_P_MemoryConfig *pstMemoryConfig /* The memory config required to support all possible configurations */
   )
{
   unsigned i;
   BMUXlib_TS_P_MemoryConfig *pTempMuxMemConfig;

   BDBG_ENTER( BMUXlib_TS_P_GetMaxMemoryConfig );

   BDBG_ASSERT(astMuxConfig);
   BDBG_ASSERT(uiNumMuxConfig);
   BDBG_ASSERT(pstMemoryConfig);

   BKNI_Memset( pstMemoryConfig, 0, sizeof( *pstMemoryConfig ) );

   pTempMuxMemConfig = BMUXlib_Malloc(sizeof(*pTempMuxMemConfig));
   if (!pTempMuxMemConfig) {
      BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
      return;
   }

   for ( i = 0; (NULL != astMuxConfig) && (i < uiNumMuxConfig); i++ )
   {
      BMUXlib_TS_P_GetMemoryConfig(
         &astMuxConfig[i],
         pTempMuxMemConfig
      );

      {
         BMUXlib_TS_P_MemoryEntryType eMemoryEntryType;

         for ( eMemoryEntryType = 0; eMemoryEntryType < BMUXlib_TS_P_MemoryEntryType_eMax; eMemoryEntryType++ )
         {
            BMUXlib_TS_P_InputType eInputType;

            for ( eInputType=0; eInputType<BMUXlib_TS_P_InputType_eMax; eInputType++ )
            {
               BMUXlib_TS_P_MemoryType eMemoryType;
               for ( eMemoryType=0; eMemoryType<BMUXlib_TS_P_MemoryType_eMax; eMemoryType++ )
               {
                  BMUXLIB_TS_P_ASSIGN_IF_LARGER(pstMemoryConfig, pTempMuxMemConfig, astMemoryEntry[eMemoryEntryType][eInputType].stMemoryConfig.stBufferInfo[eMemoryType].uiSize);
               }

               BMUXLIB_TS_P_ASSIGN_IF_LARGER(pstMemoryConfig, pTempMuxMemConfig, astMemoryEntry[eMemoryEntryType][eInputType].uiCount);
            }
         }
      }

      {
         BMUXlib_TS_P_MemoryType eMemoryType;
         for ( eMemoryType=0; eMemoryType<BMUXlib_TS_P_MemoryType_eMax; eMemoryType++ )
         {
            BMUXLIB_TS_P_ASSIGN_IF_LARGER(pstMemoryConfig, pTempMuxMemConfig, stMemoryConfig.stBufferInfo[eMemoryType].uiSize);
         }
      }
   }
   BKNI_Free(pTempMuxMemConfig);

   BDBG_LEAVE( BMUXlib_TS_P_GetMaxMemoryConfig );
}

/**********/
/* Memory */
/**********/
void
BMUXlib_TS_Legacy_GetMemoryConfig(
         const BMUXlib_TS_MuxConfig *pstMuxConfig,
         BMUXlib_TS_MemoryConfig *pstMemoryConfig
         )
{
   BMUXlib_TS_P_MemoryConfig *pTempMuxMemConfig;

   BDBG_ENTER( BMUXlib_TS_GetMemoryConfig );

   BDBG_ASSERT(pstMuxConfig);
   BDBG_ASSERT(pstMemoryConfig);

   BKNI_Memset( pstMemoryConfig, 0, sizeof( *pstMemoryConfig )  );

   pTempMuxMemConfig = BMUXlib_Malloc(sizeof(*pTempMuxMemConfig));
   if (!pTempMuxMemConfig) {
      BERR_TRACE(BERR_OUT_OF_SYSTEM_MEMORY);
      return;
   }

   BMUXlib_TS_P_GetMaxMemoryConfig(
      pstMuxConfig,
      1,
      pTempMuxMemConfig
      );

   pstMemoryConfig->uiSystemBufferSize = pTempMuxMemConfig->stMemoryConfig.stBufferInfo[BMUXlib_TS_P_MemoryType_eSystem].uiSize;
   pstMemoryConfig->uiSharedBufferSize = pTempMuxMemConfig->stMemoryConfig.stBufferInfo[BMUXlib_TS_P_MemoryType_eShared].uiSize;

   BKNI_Free(pTempMuxMemConfig);

   BDBG_LEAVE( BMUXlib_TS_GetMemoryConfig );
}

static void
BMUXlib_TS_P_GetDefaultMemorySize(
   BMUXlib_TS_MemoryConfig *pstMemoryConfig /* The memory size required to support all possible configurations */
   )
{
   BMUXlib_TS_MuxConfig stMuxConfig;

   BDBG_ENTER( BMUXlib_TS_P_GetDefaultMemorySize );

   BDBG_ASSERT( pstMemoryConfig );

   /* Determine sub heap allocation size */
   BMUXlib_TS_GetDefaultStartSettings(
      &stMuxConfig.stMuxStartSettings
      );

   BMUXlib_TS_GetDefaultMuxSettings(
      &stMuxConfig.stMuxSettings
      );

   /* Assume 6 audio, 1 video, 4 user data pids, NRT mode, TTS support */
   stMuxConfig.stMuxStartSettings.uiNumValidVideoPIDs = BMUXLIB_TS_MAX_VIDEO_PIDS;
   stMuxConfig.stMuxStartSettings.uiNumValidAudioPIDs = BMUXLIB_TS_MAX_AUDIO_PIDS;
   stMuxConfig.stMuxStartSettings.uiNumValidUserdataPIDs = BMUXLIB_TS_P_NOMINAL_USERDATA_PIDS;
   stMuxConfig.stMuxStartSettings.bNonRealTimeMode = true;
   stMuxConfig.stMuxSettings.uiSystemDataBitRate = BMUXLIB_TS_P_NOMINAL_SYSTEM_DATA_BITRATE;
   stMuxConfig.stMuxStartSettings.stPCRData.uiInterval = BMUXLIB_TS_P_NOMINAL_PCR_INTERVAL;
   stMuxConfig.stMuxStartSettings.bSupportTTS = true;

   BMUXlib_TS_Legacy_GetMemoryConfig(
      &stMuxConfig,
      pstMemoryConfig
      );

   BDBG_LEAVE( BMUXlib_TS_P_GetDefaultMemorySize );
}

void
BMUXlib_TS_Legacy_GetDefaultCreateSettings(
         BMUXlib_TS_CreateSettings *pCreateSettings
         )
{
   BDBG_ENTER( BMUXlib_TS_GetDefaultCreateSettings );

   BDBG_ASSERT( pCreateSettings );

   BKNI_Memset( pCreateSettings, 0, sizeof(*pCreateSettings) );

   pCreateSettings->uiSignature = BMUXLIB_TS_P_SIGNATURE_CREATESETTINGS;

   BMUXlib_TS_P_GetDefaultMemorySize(
      &pCreateSettings->stMemoryConfig
      );

   BDBG_LEAVE( BMUXlib_TS_GetDefaultCreateSettings );
   return;
}

static void
BMUXlib_TS_P_GetBufferConfigFromMemoryConfig(
   const BMUXlib_TS_MemoryConfig *pstMemoryConfig,
   const BMUXlib_TS_P_MemoryBuffers *pstMemoryBuffers,
   BMUXlib_TS_P_BufferConfig *pstBufferConfig
   )
{
   BDBG_ENTER(BMUXlib_TS_P_GetBufferConfigFromMemoryConfig);

   BKNI_Memset( pstBufferConfig, 0, sizeof( *pstBufferConfig ) );

   pstBufferConfig->stBufferInfo[BMUXlib_TS_P_MemoryType_eSystem].uiSize = pstMemoryConfig->uiSystemBufferSize;
   pstBufferConfig->stBufferInfo[BMUXlib_TS_P_MemoryType_eSystem].pBuffer = pstMemoryBuffers->pSystemBuffer;
   pstBufferConfig->stBufferInfo[BMUXlib_TS_P_MemoryType_eShared].uiSize = pstMemoryConfig->uiSharedBufferSize;
   pstBufferConfig->stBufferInfo[BMUXlib_TS_P_MemoryType_eShared].pBuffer = pstMemoryBuffers->pSharedBuffer;

   BDBG_LEAVE(BMUXlib_TS_P_GetBufferConfigFromMemoryConfig);
}

/* BMUXlib_TS_P_AllocateMemory - Allocates all system/device memory required for mux operation */
static BERR_Code
BMUXlib_TS_P_AllocateMemory(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   BMMA_Heap_Handle hMma,
   BMUXlib_TS_P_BufferConfig *pstMemoryConfig
)
{
   BERR_Code rc = BERR_SUCCESS;
   BMUXlib_TS_P_MemoryType eMemoryType;

   BDBG_ENTER( BMUXlib_TS_P_AllocateMemory );

   BDBG_OBJECT_ASSERT( hMuxTS, BMUXlib_TS_Legacy_P_Context );
   BDBG_ASSERT( hMma );
   BDBG_ASSERT( pstMemoryConfig );

   for ( eMemoryType=0; eMemoryType < BMUXlib_TS_P_MemoryType_eMax ; eMemoryType++ )
   {
      /* memory config must specify some amount of memory ... */
      if (0 == pstMemoryConfig->stBufferInfo[eMemoryType].uiSize)
      {
         BDBG_LEAVE( BMUXlib_TS_P_AllocateMemory );
         return BERR_TRACE(BERR_INVALID_PARAMETER);
      }

      /* Create sub-heap */
      BMMA_RangeAllocator_GetDefaultCreateSettings( &hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings );
      hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.size = pstMemoryConfig->stBufferInfo[eMemoryType].uiSize;

      /* Only allocate memory for the sub heap if an explicit buffer is not passed in */
      if ( NULL != pstMemoryConfig->stBufferInfo[eMemoryType].pBuffer )
      {
         hMuxTS->stSubHeap[eMemoryType].pBuffer = pstMemoryConfig->stBufferInfo[eMemoryType].pBuffer;
         /* Get virtual address/offset */
         hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.base = pstMemoryConfig->stBufferInfo[eMemoryType].uiBufferOffset;
      }
      else
      {
         hMuxTS->stSubHeap[eMemoryType].hBlock = BMMA_Alloc(
            hMma,
            hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.size,
            0,
            0
            );

         if ( NULL == hMuxTS->stSubHeap[eMemoryType].hBlock )
         {
            BDBG_ERR(("Error allocating memory"));
            BDBG_LEAVE( BMUXlib_TS_P_AllocateMemory );
            return BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
         }

         hMuxTS->stSubHeap[eMemoryType].pBuffer = BMMA_Lock( hMuxTS->stSubHeap[eMemoryType].hBlock );
         hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.base = BMMA_LockOffset( hMuxTS->stSubHeap[eMemoryType].hBlock );
      }

      /* Create BMMA Range Allocator */
      rc = BMMA_RangeAllocator_Create(
            &hMuxTS->stSubHeap[eMemoryType].hMmaRangeAllocator,
            &hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings
            );
      if ( BERR_SUCCESS != rc )
      {
         BDBG_ERR(("Error getting creating MMA range allocator"));
         BDBG_LEAVE( BMUXlib_TS_P_AllocateMemory );
         return BERR_TRACE( rc );
      }
   }


   BDBG_LEAVE( BMUXlib_TS_P_AllocateMemory );

   return BERR_TRACE( rc );
}

static void
BMUXlib_TS_P_FreeMemory(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   BMMA_Heap_Handle hMma
   )
{
   BMUXlib_TS_P_MemoryType eMemoryType;
   BMUXlib_TS_P_BufferConfig stMemoryConfigLocal;

   BDBG_ENTER( BMUXlib_TS_P_FreeMemory );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   BSTD_UNUSED( hMma );

   BMUXlib_TS_P_GetBufferConfigFromMemoryConfig( &hMuxTS->stCreateSettings.stMemoryConfig, &hMuxTS->stMemoryBuffers, &stMemoryConfigLocal );

   for ( eMemoryType=0; eMemoryType < BMUXlib_TS_P_MemoryType_eMax; eMemoryType++ )
   {
      if ( NULL != hMuxTS->stSubHeap[eMemoryType].hMmaRangeAllocator )
      {
         BMMA_RangeAllocator_Destroy( hMuxTS->stSubHeap[eMemoryType].hMmaRangeAllocator );
      }

      /* Only free memory for the sub heap if an explicit buffer was not passed in */
      if ( NULL == stMemoryConfigLocal.stBufferInfo[eMemoryType].pBuffer )
      {
         if ( NULL != hMuxTS->stSubHeap[eMemoryType].hBlock )
         {
            if ( NULL != hMuxTS->stSubHeap[eMemoryType].pBuffer )
            {
               BMMA_Unlock( hMuxTS->stSubHeap[eMemoryType].hBlock, hMuxTS->stSubHeap[eMemoryType].pBuffer );
            }
            hMuxTS->stSubHeap[eMemoryType].pBuffer = NULL;

            if ( BMUXLIB_TS_P_INVALID_OFFSET != hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.base )
            {
               BMMA_UnlockOffset( hMuxTS->stSubHeap[eMemoryType].hBlock, hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.base );
            }
            hMuxTS->stSubHeap[eMemoryType].stMmaRangeAllocatorCreateSettings.base = BMUXLIB_TS_P_INVALID_OFFSET;

            BMMA_Free( hMuxTS->stSubHeap[eMemoryType].hBlock );
            hMuxTS->stSubHeap[eMemoryType].hBlock = NULL;
         }
      }

      BKNI_Memset( &hMuxTS->stSubHeap[eMemoryType], 0, sizeof( hMuxTS->stSubHeap[eMemoryType] ) );
   }

   BDBG_LEAVE( BMUXlib_TS_P_FreeMemory );
}

static void
BMUXlib_TS_P_ResetResources(
   BMUXlib_TS_Legacy_Handle hMuxTS
   )
{
   unsigned i;

   BDBG_ENTER( BMUXlib_TS_P_ResetResources );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   /* Available Transport Descriptors */
   BMUXLIB_LIST_INIT( &hMuxTS->stTransportDescriptorFreeList );

   for ( i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount; i++ )
   {
      BMUXLIB_LIST_ADD(
               &hMuxTS->stTransportDescriptorFreeList,
               &hMuxTS->astTransportDescriptor[i]
               );
   }

   /* Initialize Transport Descriptor Pending List */
   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      BMUXLIB_LIST_INIT( &hMuxTS->stTransportDescriptorPendingList[i] );
   }

   /* PES Headers */
   BMUXLIB_LIST_INIT( &hMuxTS->stPESHeaderFreeList );

   for ( i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader].uiCount; i++ )
   {
      BMUXLIB_LIST_ADD(
               &hMuxTS->stPESHeaderFreeList,
               &hMuxTS->astPESHeader[i]
               );
   }

   /* TS Packets */
   BMUXLIB_LIST_INIT( &hMuxTS->stTSPacketFreeList);
   for ( i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount; i++ )
   {
      BMUXLIB_LIST_ADD(
               &hMuxTS->stTSPacketFreeList,
               &hMuxTS->astTSPacket[i]
               );
   }

   /* BPP Packets */
   BMUXLIB_LIST_INIT( &hMuxTS->stBPPFreeList);
   for ( i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eBPP].uiCount; i++ )
   {
      BMUXLIB_LIST_ADD(
               &hMuxTS->stBPPFreeList,
               &hMuxTS->astBPPData[i]
               );
   }

   /* MTU BPP Packets */
   BMUXLIB_LIST_INIT( &hMuxTS->stMTUBPPFreeList);
   for ( i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eMTUBPP].uiCount; i++ )
   {
      BMUXLIB_LIST_ADD(
               &hMuxTS->stMTUBPPFreeList,
               &hMuxTS->astMTUBPPData[i]
               );
   }

   /* System Data */
   BMUXLIB_LIST_INIT( &hMuxTS->stSystemDataFreeList );
   for (i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eSystemData].uiCount; i++){
      BMUXLIB_LIST_ADD(&hMuxTS->stSystemDataFreeList, &hMuxTS->astSystemData[i]);
   }

   /* Initialize User Data Free List */
   BMUXLIB_LIST_INIT( &hMuxTS->stUserdataFreeList );
   for (i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserData].uiCount; i++)
   {
      BMUXLIB_LIST_ADD(&hMuxTS->stUserdataFreeList, &hMuxTS->astUserdataPending[i]);
   }

   /* Initialize User Data Pending List */
   for ( i = 0; i < BMUXLIB_TS_MAX_USERDATA_PIDS; i++ )
   {
      BMUXLIB_LIST_INIT( &hMuxTS->stUserdataPendingList[i] );
   }

   /* Userdata PTS entries */
   BMUXLIB_LIST_INIT( &hMuxTS->stUserdataPTSFreeList );

   for (i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataPTS].uiCount; i++)
   {
      BMUXLIB_LIST_ADD(&hMuxTS->stUserdataPTSFreeList, &hMuxTS->astUserdataPTS[i]);
   }

   /* Userdata release Q free list */
   {
      uint32_t uiCount = hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataReleaseQ].uiCount;
      if ( 0 != uiCount )
      {
         hMuxTS->pUserdataReleaseQFreeList = hMuxTS->pUserdataReleaseQFreeListBase;
         BKNI_Memset(hMuxTS->pUserdataReleaseQFreeList, 0, sizeof(*hMuxTS->pUserdataReleaseQFreeList) * uiCount);
         for (i = 0; i < uiCount-1; i++)
            hMuxTS->pUserdataReleaseQFreeList[i].pNext = &hMuxTS->pUserdataReleaseQFreeList[i+1];
         hMuxTS->pUserdataReleaseQFreeList[i].pNext = NULL;
      }
   }

   /* SW7435-535,SW7435-688: Temporary fix for prematurely released transport descriptors */
   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      hMuxTS->uiPendingCompleted[i] = 0;
   }

   BDBG_LEAVE( BMUXlib_TS_P_ResetResources );
}

#define BMMA_RANGE_ALLOC( _subHeap, _bufferType, _count, _hBlock, _pvBuffer )\
      {\
         BMMA_RangeAllocator_BlockSettings stMmaRangeAllocatorBlockSettings;\
         BMMA_RangeAllocator_GetDefaultAllocationSettings( &stMmaRangeAllocatorBlockSettings );\
\
         (_pvBuffer) = NULL;\
         rc = BMMA_RangeAllocator_Alloc(\
               (_subHeap).hMmaRangeAllocator,\
               &(_hBlock),\
               sizeof( _bufferType) * (_count),\
               &stMmaRangeAllocatorBlockSettings\
               );\
\
         if ( BERR_SUCCESS == rc )\
         {\
            (_pvBuffer) = (_bufferType *) (((uint8_t*)(_subHeap).pBuffer + (BMMA_RangeAllocator_GetAllocationBase_isrsafe( _hBlock) - (_subHeap).stMmaRangeAllocatorCreateSettings.base)));\
         }\
         BERR_TRACE( rc );\
      }

#define BMMA_RANGE_FREE( _subHeap, _hBlock, _pvBuffer )\
      {\
         BMMA_RangeAllocator_Free(\
               (_subHeap).hMmaRangeAllocator,\
               _hBlock\
            );\
         (_hBlock) = NULL;\
         (_pvBuffer) = NULL;\
      }

static BERR_Code
BMUXlib_TS_P_AllocateResources(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   const BMUXlib_TS_P_MemoryConfig *pstMemoryConfig
   )
{
   BERR_Code rc = BERR_SUCCESS;
   uint32_t i;

   BDBG_ENTER( BMUXlib_TS_P_AllocateResources );
   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   hMuxTS->pstStatus->stMemoryConfig = *pstMemoryConfig;
   BMUXlib_TS_P_GetMemoryConfigTotal( &hMuxTS->pstStatus->stMemoryConfig, &hMuxTS->pstStatus->stMemoryConfigTotal );

#if BDBG_DEBUG_BUILD
   {
      BMUXlib_TS_P_MemoryEntryType eMemoryEntryType;

      static const char * const BMUXlib_TS_P_MemoryTypeLUT[BMUXlib_TS_P_MemoryEntryType_eMax] =
      {
         "eTransportDescriptor",
         "ePESHeader",
         "eTransportPacket",
         "eSystemData",
         "BPP",
         "UserData",
         "UserDataUnwrap",
         "UserDataReleaseQ",
         "UserDataPTS",
         "MTU BPP",
      };

      for ( eMemoryEntryType = 0; eMemoryEntryType < BMUXlib_TS_P_MemoryEntryType_eMax; eMemoryEntryType++ )
      {
         BDBG_MODULE_MSG( BMUXLIB_TS_MEMORY, ("Memory Type[%2d]: %5d (%7d/%7d bytes) - %s",
            eMemoryEntryType,
            (int)hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[eMemoryEntryType].uiCount,
            (int)hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[eMemoryEntryType].stMemoryConfig.stBufferInfo[0].uiSize,
            (int)hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[eMemoryEntryType].stMemoryConfig.stBufferInfo[1].uiSize,
            BMUXlib_TS_P_MemoryTypeLUT[eMemoryEntryType]
            ));
      }
      BDBG_MODULE_MSG( BMUXLIB_TS_MEMORY, ("Total Memory: %7d/%7d bytes",
         (int)hMuxTS->pstStatus->stMemoryConfigTotal.stMemoryConfig.stBufferInfo[0].uiSize,
         (int)hMuxTS->pstStatus->stMemoryConfigTotal.stMemoryConfig.stBufferInfo[0].uiSize
         ));
   }
#endif

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eSystemData].uiCount )
   {
      /* Allocate System Data List */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eSystemData].uiCount,
            hMuxTS->hSystemDataBlock,
            hMuxTS->astSystemData
            );
      if ( NULL == hMuxTS->astSystemData )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
   } /* end: allocate system data pending list */

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount )
   {
      /* Allocate Transport Descriptor Array (system memory) */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_TransportDescriptor),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount,
            hMuxTS->hTransportDescriptorBlock,
            hMuxTS->astTransportDescriptor
            );
      if ( NULL == hMuxTS->astTransportDescriptor )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
      BKNI_Memset( hMuxTS->astTransportDescriptor, 0, sizeof( *hMuxTS->astTransportDescriptor ) * hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount );

      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            BMUXlib_TS_TransportDescriptor,
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount,
            hMuxTS->hTransportDescriptorTempBlock,
            hMuxTS->astTransportDescriptorTemp
            );
      if ( NULL == hMuxTS->astTransportDescriptorTemp )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
      BKNI_Memset( hMuxTS->astTransportDescriptorTemp, 0, sizeof( *hMuxTS->astTransportDescriptorTemp ) * hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount );

      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            BMUXlib_TS_P_TransportDescriptorMetaData,
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount,
            hMuxTS->hTransportDescriptorMetaDataTempBlock,
            hMuxTS->astTransportDescriptorMetaDataTemp
            );
      if ( NULL == hMuxTS->astTransportDescriptorMetaDataTemp )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
      BKNI_Memset( hMuxTS->astTransportDescriptorMetaDataTemp, 0, sizeof( *hMuxTS->astTransportDescriptorMetaDataTemp ) * hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount );
   }

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader].uiCount )
   {
      /* Allocate PES Header Array (device memory) */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_PESHeader),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader].uiCount,
            hMuxTS->hPESHeaderBlock,
            hMuxTS->astPESHeader
            );
      if ( NULL == hMuxTS->astPESHeader )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
      }
   }

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount )
   {
      /* Allocate TS Packet Array (device memory) */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_TSPacket),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount,
            hMuxTS->hTSPacketBlock,
            hMuxTS->astTSPacket
            );
      if ( NULL == hMuxTS->astTSPacket )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
      }

      /* Pre-populate with default TS Packet */
      for (i = 0; i < hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount; i++ )
      {
         BKNI_Memcpy(
                  BMUXLIB_LIST_ENTRY_DATA(&hMuxTS->astTSPacket[i]),
                  &s_stDefaultTSPacket,
                  sizeof( BMUXlib_TS_P_TSPacket )
                  );
      }
   }

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount )
   {
      /* Allocate BPP Data Array (device memory) */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_BPPData),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eBPP].uiCount,
            hMuxTS->hBPPDataBlock,
            hMuxTS->astBPPData
            );
      if ( NULL == hMuxTS->astBPPData )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
      }

      if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eMTUBPP].uiCount )
      {
         /* Allocate MTU BPP Data Array (device memory) */
         BMMA_RANGE_ALLOC(
               hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
               BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_MTUBPPData),
               hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eMTUBPP].uiCount,
               hMuxTS->hMTUBPPDataBlock,
               hMuxTS->astMTUBPPData
               );
         if ( NULL == hMuxTS->astMTUBPPData )
         {
            BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
            return BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
         }
      }
   }

   if ( 0 != hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserData].uiCount )
   {
      /* Allocate Userdata Free List
         NOTE: This assumes that there is a fixed amount of userdata
         processed per video frame - thus, the PES count supplied by the user
         is used to allocate the necessary free list for userdata. */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_UserdataPending),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserData].uiCount,
            hMuxTS->hUserdataPendingBlock,
            hMuxTS->astUserdataPending
            );
      if ( NULL == hMuxTS->astUserdataPending )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }

      /* Allocate userdata PTS storage - need TWO per userdata PES pending */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
            BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_P_UserdataPTSEntry),
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataPTS].uiCount,
            hMuxTS->hUserdataPTSBlock,
            hMuxTS->astUserdataPTS
            );
      if ( NULL == hMuxTS->astUserdataPTS )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE(BERR_OUT_OF_DEVICE_MEMORY);
      }

      /* Allocate userdata local buffering for "unwrapping" packets
         (each userdata input has its own buffer) */
      BMMA_RANGE_ALLOC(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
            BMUXlib_TS_P_TSPacket,
            hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataUnwrap].uiCount,
            hMuxTS->hUserdataUnwrapBlock,
            hMuxTS->astUserdataUnwrap
            );
      if ( NULL == hMuxTS->astUserdataUnwrap )
      {
         BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
         return BERR_TRACE(BERR_OUT_OF_DEVICE_MEMORY);
      }

      /* userdata release Q (need one entry for each pending segment entry) ... */
      {
         uint32_t uiCount = hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eUserDataReleaseQ].uiCount;
         BMMA_RANGE_ALLOC(
               hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
               BMUXlib_TS_P_UserdataReleaseQEntry,
               uiCount,
               hMuxTS->hUserdataReleaseQFreeListBaseBlock,
               hMuxTS->pUserdataReleaseQFreeListBase
               );
         if ( NULL == hMuxTS->pUserdataReleaseQFreeListBase )
         {
            BDBG_LEAVE(BMUXlib_TS_P_AllocateResources);
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
      }
   } /* end: userdata free list allocation */

   BMUXlib_TS_P_ResetResources( hMuxTS );

   BDBG_LEAVE( BMUXlib_TS_P_AllocateResources );

   return BERR_TRACE( rc );
}

/* move any pre-queued system data into the actual system data queue */
/* SW7425-4643: This call expects hMuxTS->pstSettings->stStartSettings to be set, since
   the copying of any pre-queued system data will use the PCR PID for validation */
static BERR_Code
BMUXlib_TS_P_RelocatePreQSystemData(BMUXlib_TS_Legacy_Handle hMuxTS)
{
   BERR_Code rc = BERR_SUCCESS;
   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   /* SW7425-3958: */
   /* Use the allocated system data list: Move any system data queued prior to start to this list buffers */
   /* NOTE: This must be done AFTER ResetResources() since that call will clear the new queue (and the newly
      copied data!) */
   {
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData)* pSystemDataEntry;
      BMUXLIB_LIST_FIRST( &hMuxTS->stSystemDataPendingList, &pSystemDataEntry );
      BMUXLIB_LIST_INIT( &hMuxTS->stSystemDataPendingList );
      hMuxTS->pstStatus->bSystemDataPreQ = false;
      while ( NULL != pSystemDataEntry )
      {
         size_t uiQueuedCount;

         rc = BMUXlib_TS_P_AddSystemDataBuffers(
                  hMuxTS,
                  BMUXLIB_LIST_ENTRY_DATA(pSystemDataEntry),
                  1,
                  &uiQueuedCount
                  );
         if ( rc != BERR_SUCCESS )
         {
            return BERR_TRACE( rc );
         }

         if ( 0 == uiQueuedCount )
         {
            BDBG_ERR(("Not enough space to relocate system data pre-Q"));
            return BERR_TRACE( BERR_UNKNOWN );
         }

         pSystemDataEntry = BMUXLIB_LIST_ENTRY_NEXT( pSystemDataEntry );
      }
   }

   return BERR_TRACE(rc);
}

static BERR_Code
BMUXlib_TS_P_SetupMCPB(
   BMUXlib_TS_Legacy_Handle hMuxTS
   )
{
   BERR_Code rc = BERR_SUCCESS;
   unsigned i, uiPIDIndex;
   unsigned auiNumInputsPerTransportChannelIndex[BMUXLIB_TS_MAX_TRANSPORT_INSTANCES];

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      auiNumInputsPerTransportChannelIndex[i] = 0;
   }

   /* Check Video Inputs */
   for ( uiPIDIndex = 0; (BERR_SUCCESS == rc) && (uiPIDIndex < BMUXLIB_TS_MAX_VIDEO_PIDS) && (uiPIDIndex < hMuxTS->pstSettings->stStartSettings.uiNumValidVideoPIDs); uiPIDIndex++ )
   {
      auiNumInputsPerTransportChannelIndex[hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiTransportChannelIndex]++;
   }

   /* Check Audio Inputs */
   for ( uiPIDIndex = 0; (BERR_SUCCESS == rc) && (uiPIDIndex < BMUXLIB_TS_MAX_AUDIO_PIDS) && (uiPIDIndex < hMuxTS->pstSettings->stStartSettings.uiNumValidAudioPIDs); uiPIDIndex++ )
   {
      auiNumInputsPerTransportChannelIndex[hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiTransportChannelIndex]++;
   }

   /* Check System Inputs */
   auiNumInputsPerTransportChannelIndex[hMuxTS->pstSettings->stStartSettings.stPCRData.uiTransportChannelIndex]++;

   /* Create MCPB instance for each transport that has multiple inputs */
   for ( i = 0; ( BERR_SUCCESS == rc) && i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      if ( auiNumInputsPerTransportChannelIndex[i] > 1 )
      {
         BMUXlib_TS_MCPB_CreateSettings stCreateSettings;

         BMUXlib_TS_MCPB_GetDefaultCreateSettings( &stCreateSettings );
         stCreateSettings.hMma = hMuxTS->stCreateSettings.hMma;
         stCreateSettings.stMuxSharedMemory.hBlock =  hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
         stCreateSettings.stMuxSharedMemory.uiSize =  hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].stMmaRangeAllocatorCreateSettings.size;
         stCreateSettings.uiMaxNumInputs = auiNumInputsPerTransportChannelIndex[i];
         stCreateSettings.stOutputChannelInterface = hMuxTS->pstSettings->stStartSettings.transport.stChannelInterface[i];

         rc = BERR_TRACE( BMUXlib_TS_MCPB_Create(
            &hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB,
            &stCreateSettings
            ));
      }
   }

   return BERR_TRACE( rc );
}

static void
BMUXlib_TS_P_TeardownMCPB(
   BMUXlib_TS_Legacy_Handle hMuxTS
   )
{
   unsigned i,j;

   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      if ( NULL != hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB )
      {
         for ( j = 0; j < BMUXLIB_TS_MAX_VIDEO_PIDS + BMUXLIB_TS_MAX_AUDIO_PIDS + 1; j++ )
         {
            if ( NULL != hMuxTS->pstStatus->stOutput.stMCPB[i].ahMuxMCPBCh[j] )
            {
               BMUXlib_TS_MCPB_Channel_Close( hMuxTS->pstStatus->stOutput.stMCPB[i].ahMuxMCPBCh[j] );
               hMuxTS->pstStatus->stOutput.stMCPB[i].ahMuxMCPBCh[j] = NULL;
            }
         }

         BMUXlib_TS_MCPB_Destroy(
            hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB
            );
         hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB = NULL;
      }
   }
}

static void
BMUXlib_TS_P_FreeResources(
   BMUXlib_TS_Legacy_Handle hMuxTS
   )
{
   uint32_t i;

   BDBG_ENTER( BMUXlib_TS_P_FreeResources );

   /* cleanup input group and inputs - these are created by Start() */
   if (NULL != hMuxTS->hInputGroup)
   {
      BMUXlib_InputGroup_Stop(hMuxTS->hInputGroup);
   }

   BMUXlib_TS_P_TeardownMCPB( hMuxTS );

   {
      unsigned uiInputIndex;

      for ( uiInputIndex = 0; uiInputIndex < BMUXLIB_TS_MAX_INPUT_PIDS; uiInputIndex++ )
      {
         if ( NULL != hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput )
         {
            BMUXlib_Input_Stop( hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput );
            hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput = NULL;
         }
      }
   }

#if BDBG_DEBUG_BUILD
#if 1
   /* Verify there aren't any pending buffers */

   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      if ( true == hMuxTS->pstStatus->stOutput.stTransport[i].bActive )
      {
         if ( false == BMUXLIB_LIST_ISEMPTY( &hMuxTS->stTransportDescriptorPendingList[i] ) )
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("Transport Descriptors Pending List[%d] not empty", i));
         }
      }
   }

   /* Verify all buffers have been returned to the free lists */
   {
      size_t uiCount;

      {
         BMUXLIB_LIST_GETNUMENTRIES(
                  &hMuxTS->stTransportDescriptorFreeList,
                  &uiCount
                  );
         if ( hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount != uiCount )
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("All Transport Descriptors have not been freed (%d)", (int)(hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor].uiCount - uiCount)));
         }
      }

      {
         BMUXLIB_LIST_GETNUMENTRIES(
                  &hMuxTS->stPESHeaderFreeList,
                  &uiCount
                  );
         if ( hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader].uiCount != uiCount )
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("All PES Headers have not been freed (%d)", (int)(hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_ePESHeader].uiCount - uiCount)));
         }
      }

      {
         BMUXLIB_LIST_GETNUMENTRIES(
                  &hMuxTS->stTSPacketFreeList,
                  &uiCount
                  );
         if ( hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount != uiCount )
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("All TS Packets have not been freed (%d)", (int)(hMuxTS->pstStatus->stMemoryConfigTotal.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportPacket].uiCount - uiCount)));
         }
      }
   }
#else
#warning "Check for pending descriptors on destroy has been disabled"
#endif
#endif

   if ( NULL != hMuxTS->astMTUBPPData )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
         hMuxTS->hMTUBPPDataBlock,
         hMuxTS->astMTUBPPData
         );
   }

   if ( NULL != hMuxTS->astBPPData )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
         hMuxTS->hBPPDataBlock,
         hMuxTS->astBPPData
         );
   }

   if ( NULL != hMuxTS->astUserdataPending )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
         hMuxTS->hUserdataPendingBlock,
         hMuxTS->astUserdataPending
         );
   }

   if ( NULL != hMuxTS->pUserdataReleaseQFreeListBase )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
         hMuxTS->hUserdataReleaseQFreeListBaseBlock,
         hMuxTS->pUserdataReleaseQFreeListBase
         );
      hMuxTS->pUserdataReleaseQFreeList = NULL;
   }

   for (i = 0; i < BMUXLIB_TS_MAX_USERDATA_PIDS; i++)
   {
      {
#if BDBG_DEBUG_BUILD
         size_t uiMinUsage, uiMaxUsage, uiSize;
         BMUXLIB_LIST_GETUSAGE(&hMuxTS->stUserdataPendingList[i], &uiMinUsage, &uiMaxUsage, &uiSize);
         BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("Userdata Pending List[%d]: Max Usage: %d of %d (%d%%)", i, (int)uiMaxUsage, (int)uiSize, (int)((100*uiMaxUsage)/uiSize)));
#endif
         BMUXLIB_LIST_INIT(&hMuxTS->stUserdataPendingList[i]);
      }
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stUserdataFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("Userdata Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stUserdataFreeList);
   }

   if (NULL != hMuxTS->astUserdataPTS )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
         hMuxTS->hUserdataPTSBlock,
         hMuxTS->astUserdataPTS
         );
   }

   if (NULL != hMuxTS->astUserdataUnwrap )
   {
      BMMA_RANGE_FREE(
         hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
         hMuxTS->hUserdataUnwrapBlock,
         hMuxTS->astUserdataUnwrap
         );
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stUserdataPTSFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("Userdata PTS Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stUserdataPTSFreeList);
   }

   if ( NULL != hMuxTS->astSystemData )
   {
      BMMA_RANGE_FREE( hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem], hMuxTS->hSystemDataBlock, hMuxTS->astSystemData );
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stSystemDataPendingList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("System Data Pending List: Max Usage: %d of %d (%d%%)", (int)uiMaxUsage, (int)uiSize, (int)((100*uiMaxUsage)/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stSystemDataPendingList);
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stMTUBPPFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("MTU BPP Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stMTUBPPFreeList);
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stBPPFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("BPP Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stBPPFreeList);
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stTSPacketFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("TS Packet Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stTSPacketFreeList);
   }

   if ( NULL != hMuxTS->astTSPacket )
   {
      BMMA_RANGE_FREE(
               hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
               hMuxTS->hTSPacketBlock,
               hMuxTS->astTSPacket
               );
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stPESHeaderFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("PES Header Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stPESHeaderFreeList);
   }

   if ( NULL != hMuxTS->astPESHeader )
   {
      BMMA_RANGE_FREE(
               hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared],
               hMuxTS->hPESHeaderBlock,
               hMuxTS->astPESHeader
               );
   }

   for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
   {
      {
#if BDBG_DEBUG_BUILD
         size_t uiMinUsage, uiMaxUsage, uiSize;
         BMUXLIB_LIST_GETUSAGE(&hMuxTS->stTransportDescriptorPendingList[i], &uiMinUsage, &uiMaxUsage, &uiSize);
         BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("Transport Descriptor Pending List[%d]: Max Usage: %d of %d (%d%%)", i, (int)uiMaxUsage, (int)uiSize, (int)((100*uiMaxUsage)/uiSize)));
#endif
         BMUXLIB_LIST_INIT(&hMuxTS->stTransportDescriptorPendingList[i]);
      }
   }

   {
#if BDBG_DEBUG_BUILD
      size_t uiMinUsage, uiMaxUsage, uiSize;
      BMUXLIB_LIST_GETUSAGE(&hMuxTS->stTransportDescriptorFreeList, &uiMinUsage, &uiMaxUsage, &uiSize);
      BDBG_MODULE_MSG(BMUXLIB_TS_MEMORY, ("Transport Descriptor Free List: Max Usage: %d of %d (%d%%)", (int)(uiSize-uiMinUsage), (int)uiSize, (int)((100 * (uiSize-uiMinUsage))/uiSize)));
#endif
      BMUXLIB_LIST_INIT(&hMuxTS->stTransportDescriptorFreeList);
   }

   if ( NULL != hMuxTS->astTransportDescriptorMetaDataTemp )
   {
      BMMA_RANGE_FREE(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            hMuxTS->hTransportDescriptorMetaDataTempBlock,
            hMuxTS->astTransportDescriptorMetaDataTemp
            );
   }

   if ( NULL != hMuxTS->astTransportDescriptorTemp )
   {
      BMMA_RANGE_FREE(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            hMuxTS->hTransportDescriptorTempBlock,
            hMuxTS->astTransportDescriptorTemp
            );
   }

   if ( NULL != hMuxTS->astTransportDescriptor )
   {
      BMMA_RANGE_FREE(
            hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eSystem],
            hMuxTS->hTransportDescriptorBlock,
            hMuxTS->astTransportDescriptor
            );
   }

   BDBG_LEAVE( BMUXlib_TS_P_FreeResources );
}

static void
BMUXlib_TS_P_Reset(
   BMUXlib_TS_Legacy_Handle hMuxTS
   )
{
   unsigned i;

   BDBG_ENTER( BMUXlib_TS_P_Reset );

#ifdef BMUXLIB_TS_P_TEST_MODE
   if (NULL != hMuxTS->pstStatus->stSystemDataInfo.hCSVFile)
      fclose(hMuxTS->pstStatus->stSystemDataInfo.hCSVFile);
   if (NULL != hMuxTS->pstStatus->stSystemDataInfo.hDataFile)
      fclose(hMuxTS->pstStatus->stSystemDataInfo.hDataFile);
#endif

   BKNI_Memset( hMuxTS->pstStatus, 0, sizeof( *hMuxTS->pstStatus ) );
   BKNI_Memset( hMuxTS->aFoundPIDs, 0, BMUXLIB_TS_PID_TABLE_SIZE * sizeof(*hMuxTS->aFoundPIDs) );
   BKNI_Memset( hMuxTS->pstUserdataStatus, 0, sizeof(*hMuxTS->pstUserdataStatus) );
   BKNI_Memset( hMuxTS->pstSettings, 0, sizeof(*hMuxTS->pstSettings) );

   BMUXlib_InputGroup_Stop(hMuxTS->hInputGroup);

   for ( i = 0; i < (BMUXLIB_TS_MAX_VIDEO_PIDS + BMUXLIB_TS_MAX_AUDIO_PIDS); i++ )
   {
      BMUXlib_Input_Stop(hMuxTS->ahInput[i]);
   }

   /* Initialize System Data Pending List pre-Q */
   {
      BDBG_ASSERT( hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].stMmaRangeAllocatorCreateSettings.size > ( BMUXLIB_TS_P_SYSTEM_DATA_PRE_Q_COUNT * sizeof( BMUXlib_TS_SystemData ) ) );

      BMUXLIB_LIST_INIT( &hMuxTS->stSystemDataFreeList );
      for (i = 0; i < BMUXLIB_TS_P_SYSTEM_DATA_PRE_Q_COUNT ; i++)
      {
         BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData)*pstSystemDataBuffer = (BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData)*) ( (uint8_t *) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer + ( hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].stMmaRangeAllocatorCreateSettings.size - ( (i+1) * sizeof( BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData) ) ) ) );
         BMUXLIB_LIST_ADD(&hMuxTS->stSystemDataFreeList, pstSystemDataBuffer);
      }
      hMuxTS->pstStatus->bSystemDataPreQ = true;
   }

   BMUXlib_TS_GetDefaultMuxSettings(
            &hMuxTS->pstSettings->stMuxSettings
            );

   BMUXlib_TS_GetDefaultStartSettings(
            &hMuxTS->pstSettings->stStartSettings
            );

   BDBG_LEAVE( BMUXlib_TS_P_Reset );
}

BERR_Code
BMUXlib_TS_Legacy_Create(
         BMUXlib_TS_Legacy_Handle *phMuxTS,  /* [out] TSMuxer handle returned */
         const BMUXlib_TS_CreateSettings *pstCreateSettings
         )
{
   BERR_Code rc = BERR_UNKNOWN;
   BMUXlib_TS_Legacy_P_Context *pMuxTS;

   BDBG_ENTER( BMUXlib_TS_Create );

   BDBG_ASSERT( phMuxTS );
   BDBG_ASSERT( pstCreateSettings );
   BDBG_ASSERT( BMUXLIB_TS_P_SIGNATURE_CREATESETTINGS == pstCreateSettings->uiSignature );

   /************************/
   /* Create MUX TS Handle */
   /************************/

   /* Set the handle to NULL in case the allocation fails */
   *phMuxTS = NULL;

   if (/* verify the required hMem handle is provided ... */
      /* NOTE: if software XPT ever used via the transport abstraction, then
         this _could_ be NULL, and Malloc should be used instead */
      (NULL == pstCreateSettings->hMma)
      )
   {
      BDBG_LEAVE(BMUXlib_TS_Create);
      return BERR_TRACE( BERR_INVALID_PARAMETER );
   }

   /* Allocate MUX TS Context (system memory) */
   BMUXLIB_P_CONTEXT_ALLOCATE( BMUXlib_TS_Legacy_P_Context, pMuxTS, alloc_context_error )
   BDBG_OBJECT_SET(pMuxTS, BMUXlib_TS_Legacy_P_Context);

   /* Allocate MUX TS Status (system memory) */
   BMUXLIB_P_CONTEXT_ALLOCATE( BMUXlib_TS_P_Status, pMuxTS->pstStatus, alloc_error )
   BMUXLIB_P_CONTEXT_ALLOCATE( BMUXlib_TS_P_UserdataStatusPriv, pMuxTS->pstUserdataStatus, alloc_error )
   BMUXLIB_P_CONTEXT_ALLOCATE( BMUXlib_TS_P_Settings, pMuxTS->pstSettings, alloc_error )

   pMuxTS->aFoundPIDs = (uint32_t*) BMUXlib_Malloc(BMUXLIB_TS_PID_TABLE_SIZE * sizeof(uint32_t));
   if ( NULL == pMuxTS->aFoundPIDs ) goto alloc_error;

   BKNI_Memset( pMuxTS->aFoundPIDs, 0, BMUXLIB_TS_PID_TABLE_SIZE * sizeof(*pMuxTS->aFoundPIDs) );

   /* Copy user specified settings */
   BKNI_Memcpy( &pMuxTS->stCreateSettings, pstCreateSettings, sizeof( pMuxTS->stCreateSettings ) );

   /* Populate BMUXlib_TS_P_MemoryBuffers */
   {
      pMuxTS->stMemoryBuffers.pSystemBuffer = pMuxTS->stCreateSettings.stMemoryBuffers.pSystemBuffer;

      if ( NULL != pMuxTS->stCreateSettings.stMemoryBuffers.hSharedBufferBlock )
      {
         pMuxTS->stMemoryBuffers.pSharedBuffer = BMMA_Lock( pMuxTS->stCreateSettings.stMemoryBuffers.hSharedBufferBlock );
      }
   }

   /* Allocate sub heap */
   {
      BMUXlib_TS_P_BufferConfig stMemoryConfig;
      BKNI_Memset( &stMemoryConfig, 0, sizeof( stMemoryConfig ) );
      BMUXlib_TS_P_GetBufferConfigFromMemoryConfig( &pMuxTS->stCreateSettings.stMemoryConfig, &pMuxTS->stMemoryBuffers, &stMemoryConfig );

      rc = BMUXlib_TS_P_AllocateMemory( pMuxTS, pMuxTS->stCreateSettings.hMma, &stMemoryConfig );

      if ( BERR_SUCCESS != rc )
      {
         BMUXlib_TS_Legacy_Destroy( pMuxTS );
         BDBG_LEAVE(BMUXlib_TS_Create);
         return BERR_TRACE(rc);
      }
   }

   /* Allocate input handles */
   {
      unsigned i;
      BMUXlib_Input_CreateSettings stInputCreateSettings;
      BMUXlib_Input_GetDefaultCreateSettings( &stInputCreateSettings );

      for ( i = 0; i < BMUXLIB_TS_MAX_VIDEO_PIDS + BMUXLIB_TS_MAX_AUDIO_PIDS; i++ )
      {
         rc = BMUXlib_Input_Create( &pMuxTS->ahInput[i], &stInputCreateSettings );

         if ( BERR_SUCCESS != rc )
         {
            BMUXlib_TS_Legacy_Destroy( pMuxTS );
            BDBG_LEAVE(BMUXlib_TS_Create);
            return BERR_TRACE(rc);
         }
      }
   }

   /* Allocate input group handles */
   {
      BMUXlib_InputGroup_CreateSettings stInputGroupCreateSettings;
      BMUXlib_InputGroup_GetDefaultCreateSettings( &stInputGroupCreateSettings );
      stInputGroupCreateSettings.uiMaxInputCount = BMUXLIB_TS_MAX_VIDEO_PIDS + BMUXLIB_TS_MAX_AUDIO_PIDS;

      rc = BMUXlib_InputGroup_Create( &pMuxTS->hInputGroup, &stInputGroupCreateSettings );

      if ( BERR_SUCCESS != rc )
      {
         BMUXlib_TS_Legacy_Destroy( pMuxTS );
         BDBG_LEAVE(BMUXlib_TS_Create);
         return BERR_TRACE(rc);
      }
   }

   /* Reset State */
   BMUXlib_TS_P_Reset( pMuxTS );

   /* Provide handle to caller */
   *phMuxTS = pMuxTS;
   goto alloc_done;

alloc_error:
   BMUXlib_TS_Legacy_Destroy( pMuxTS );

alloc_context_error:
   rc = BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );

alloc_done:
   BDBG_LEAVE(BMUXlib_TS_Create);
   return BERR_TRACE( BERR_SUCCESS );
}

void
BMUXlib_TS_Legacy_Destroy(
         BMUXlib_TS_Legacy_Handle hMuxTS
         )
{
   BDBG_ENTER(BMUXlib_TS_Legacy_Destroy);

   /* the following signifies an attempt to free up something that was either
      a) not created by Create()
      b) has already been destroyed
   */
   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   /* SW7425-3642: Stop the mux if it hasn't already been stopped
      - this is necessary since Stop() now frees resources */
   if (BMUXlib_State_eStopped != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS))
   {
      BERR_Code rc = BMUXlib_TS_Legacy_Stop(hMuxTS);
      if (rc != BERR_SUCCESS)
      {
         BDBG_ERR(("Error Force Stopping Mux"));
      }
   }

   /* destroy input group */
   if ( NULL != hMuxTS->hInputGroup )
   {
      BMUXlib_InputGroup_Destroy( hMuxTS->hInputGroup );
      hMuxTS->hInputGroup = NULL;
   }
   /* destroy inputs */
   {
      unsigned i;
      BMUXlib_Input_CreateSettings stInputCreateSettings;
      BMUXlib_Input_GetDefaultCreateSettings( &stInputCreateSettings );

      for ( i = 0; i < BMUXLIB_TS_MAX_VIDEO_PIDS + BMUXLIB_TS_MAX_AUDIO_PIDS; i++ )
      {
         if ( NULL != hMuxTS->ahInput[i] )
         {
            BMUXlib_Input_Destroy( hMuxTS->ahInput[i] );
            hMuxTS->ahInput[i] = NULL;
         }
      }
   }

   BMUXlib_TS_P_FreeMemory(
      hMuxTS,
      hMuxTS->stCreateSettings.hMma
      );

   /* Unlock shared buffer (if supplied) */
   /* Populate BMUXlib_TS_P_MemoryBuffers */
   {
      if ( ( NULL != hMuxTS->stCreateSettings.stMemoryBuffers.hSharedBufferBlock )
           && ( NULL != hMuxTS->stMemoryBuffers.pSharedBuffer ) )
      {
         BMMA_Unlock( hMuxTS->stCreateSettings.stMemoryBuffers.hSharedBufferBlock, hMuxTS->stMemoryBuffers.pSharedBuffer );
      }
   }

   if ( NULL != hMuxTS->aFoundPIDs )
   {
      BKNI_Free(hMuxTS->aFoundPIDs);
      hMuxTS->aFoundPIDs = NULL;
   }

   BMUXLIB_P_CONTEXT_FREE( hMuxTS->pstSettings )
   BMUXLIB_P_CONTEXT_FREE( hMuxTS->pstUserdataStatus )
   BMUXLIB_P_CONTEXT_FREE( hMuxTS->pstStatus )

   /* the following prevents accidental reuse of the context */
   BDBG_OBJECT_DESTROY(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BMUXLIB_P_CONTEXT_FREE( hMuxTS )

   BDBG_LEAVE(BMUXlib_TS_Legacy_Destroy);
   return;
}

/****************/
/* Mux Settings */
/****************/
/* Validate Specified Mux settings
   NOTE: that validation of the system data bitrate requires access to the
   Start Settings
   If bUseDefaults set, then erroneous settings will result in a warning and
   defaults will be used

   Return will be false if settings are OK
   If bUseDefaults is set, a true return code indicates a change was made
   otherwise true indicates an erroneous setting (it is expected that the caller
   will handle the error condition if bUseDefaults is false)
*/
static bool
BMUXlib_TS_P_ValidateMuxSettings(BMUXlib_TS_MuxSettings *pstMuxSettings, BMUXlib_TS_StartSettings *pstStartSettings, bool bUseDefaults)
{
   /* NOTE: for now, system data bitrate is the only mux setting to be validated */
   return BMUXlib_TS_P_ValidateSystemDataBitrate(pstMuxSettings, pstStartSettings, bUseDefaults);
}

/* Minimum number of system data packets that occurs in a PCR interval
   (INCLUDING the PCR packet itself) - i.e. PCR + 1 other */
#define MIN_SYSD_PACKETS_PER_INTERVAL 2

/* Validate the System Data Bitrate to ensure it is sufficient for the
   current PCR interval in use
   (we specify the minimum required to be 2 packets: the PCR itself
   and one other)
   If the value is not within the allowed bounds clip to the max/min
   and give a warning
   Return will be false if settings are OK
   If bUseDefaults is set, a true return code indicates a change was made
   otherwise true indicates an erroneous setting (it is expected that the caller
   will handle the error condition if bUseDefaults is false)
   NOTE: Currently, this will always return false if bUseDefaults is false
*/
static bool
BMUXlib_TS_P_ValidateSystemDataBitrate(BMUXlib_TS_MuxSettings *pstMuxSettings, const BMUXlib_TS_StartSettings *pstStartSettings, bool bUseDefaults)
{
   uint32_t uiMinSystemDataBitrate;
   /* NOTE: here, the system data bitrate has been either set to defaults by Create, or to
      some other value by the user via SetMuxSettings().  The values set by Create (the
      defaults) are guaranteed to work. So this is only verifying user input of user-
      provided system  data bitrate vs current PCR inteval (either default or user-provided) */

   if (pstMuxSettings->uiSystemDataBitRate > BMUXLIB_TS_MAX_SYS_DATA_BR)
   {
      pstMuxSettings->uiSystemDataBitRate = BMUXLIB_TS_MAX_SYS_DATA_BR;
      BDBG_WRN(("System Data Bitrate exceeds maximum allowed (%d) ... clipping to max", pstMuxSettings->uiSystemDataBitRate));
      return bUseDefaults;
   }
   if ( 0 != pstStartSettings->stPCRData.uiInterval )
   {
      /* calculate min required for PCR interval */
      /* min bitrate required = 2 TS packets / PCR interval (ms) = 2 * 188 * 8 * 1000 / PCR interval
         = 3008000 / PCR Interval (in ms) */
      uiMinSystemDataBitrate = (MIN_SYSD_PACKETS_PER_INTERVAL * 188 * 8 * 1000) / pstStartSettings->stPCRData.uiInterval;
      if (pstMuxSettings->uiSystemDataBitRate < uiMinSystemDataBitrate)
      {
         BDBG_WRN(("System Data Bitrate (%d) insufficient for current PCR interval (%d ms)", pstMuxSettings->uiSystemDataBitRate,
            pstStartSettings->stPCRData.uiInterval));
         pstMuxSettings->uiSystemDataBitRate = uiMinSystemDataBitrate;
         BDBG_WRN(("Using minimum bitrate to provide at least 2 packets per interval: %d bps", pstMuxSettings->uiSystemDataBitRate));
         return bUseDefaults;
      }
   }
   return false;
}

BERR_Code
BMUXlib_TS_Legacy_SetMuxSettings(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_MuxSettings *pstMuxSettings
         )
{
   BMUXlib_TS_MuxSettings stSettingsCopy;
   BDBG_ENTER( BMUXlib_TS_SetMuxSettings );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstMuxSettings );
   BDBG_ASSERT( BMUXLIB_TS_P_SIGNATURE_MUXSETTINGS == pstMuxSettings->uiSignature );

   /* make a copy since Validate() may modify the settings */
   stSettingsCopy = *pstMuxSettings;
   if (BMUXlib_TS_P_ValidateMuxSettings(&stSettingsCopy, &hMuxTS->pstSettings->stStartSettings, false))
   {
      BDBG_LEAVE( BMUXlib_TS_SetMuxSettings );
      return BERR_TRACE(BERR_INVALID_PARAMETER);
   }

   /* if settings validate OK, then store them */
   hMuxTS->pstSettings->stMuxSettings = stSettingsCopy;

   /* SW7436-1363: If mux has started, then set the inputs as needed */
   if ( BMUXlib_State_eStopped != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
   {
      unsigned i,uiInputIndex;
      BMUXlib_Input_Settings stSettings;

      for ( i = 0; i < BMUXLIB_TS_MAX_VIDEO_PIDS; i++ )
      {
         uiInputIndex = hMuxTS->pstStatus->stInputIndexLUT.uiVideo[i];

         if ( uiInputIndex == 0xFFFFFFFF ) continue;

         BMUXlib_Input_GetSettings(
            hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput,
            &stSettings
         );

         stSettings.bEnable = hMuxTS->pstSettings->stMuxSettings.stInputEnable.bVideo[i];

         BMUXlib_Input_SetSettings(
            hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput,
            &stSettings
         );
      }

      for ( i = 0; i < BMUXLIB_TS_MAX_AUDIO_PIDS; i++ )
      {
         uiInputIndex = hMuxTS->pstStatus->stInputIndexLUT.uiAudio[i];

         if ( uiInputIndex == 0xFFFFFFFF ) continue;

         BMUXlib_Input_GetSettings(
            hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput,
            &stSettings
         );

         stSettings.bEnable = hMuxTS->pstSettings->stMuxSettings.stInputEnable.bAudio[i];

         BMUXlib_Input_SetSettings(
            hMuxTS->pstStatus->stInputMetaData[uiInputIndex].hInput,
            &stSettings
         );
      }
   }

   BDBG_LEAVE( BMUXlib_TS_SetMuxSettings );
   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_Legacy_GetMuxSettings(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         BMUXlib_TS_MuxSettings *pstMuxSettings
         )
{
   BDBG_ENTER( BMUXlib_TS_GetMuxSettings );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstMuxSettings );

   *pstMuxSettings = hMuxTS->pstSettings->stMuxSettings;

   BDBG_LEAVE( BMUXlib_TS_GetMuxSettings );
   return BERR_TRACE( BERR_SUCCESS );
}

/**************/
/* Start/Stop */
/**************/
/* Validate Start settings that also affect memory calculations

   If bUseDefaults set, then erroneous settings will result in a warning and
   defaults will be used

   Return will be false if settings are OK
   If bUseDefaults is set, a true return code indicates a change was made
   otherwise true indicates an erroneous setting (it is expected that the caller
   will handle the error condition if bUseDefaults is false)
*/
static bool
BMUXlib_TS_P_ValidateStartSettings(BMUXlib_TS_StartSettings *pstStartSettings, bool bUseDefaults)
{
   bool bChanged = false;
   bool bError = false;

   if ( ( BMUXlib_TS_InterleaveMode_ePTS == pstStartSettings->eInterleaveMode )
        && ( 0 != pstStartSettings->stPCRData.uiInterval ) )
   {
      BDBG_WRN(("PCR generation is not supported in PTS Interleave Mode - ignoring PCR interval"));
      pstStartSettings->stPCRData.uiInterval = 0;
      bChanged = true;
   }

   /* Interval of > 100ms is outside spec, but we do not impose specification on
      the user (they may desire a proprietary implementation */
   if (pstStartSettings->stPCRData.uiInterval > 100)
      BDBG_WRN(("PCR Interval exceeds maximum of 100ms (as specified by ISO/IEC-13818-1)"));

   /* NOTE: the following are all expected behaviour, so we do NOT output error */
   if (pstStartSettings->uiNumValidVideoPIDs > BMUXLIB_TS_MAX_VIDEO_PIDS)
   {
      BDBG_WRN(("Specified Video PID count exceeds capabilities - excess ignored"));
      pstStartSettings->uiNumValidVideoPIDs = BMUXLIB_TS_MAX_VIDEO_PIDS;
      bChanged = true;
   }

   if (pstStartSettings->uiNumValidAudioPIDs > BMUXLIB_TS_MAX_AUDIO_PIDS)
   {
      BDBG_WRN(("Specified Audio PID count exceeds capabilities - excess ignored"));
      pstStartSettings->uiNumValidAudioPIDs = BMUXLIB_TS_MAX_AUDIO_PIDS;
      bChanged = true;
   }

   if (pstStartSettings->uiNumValidUserdataPIDs > BMUXLIB_TS_MAX_USERDATA_PIDS)
   {
      BDBG_WRN(("Specified Userdata PID count exceeds capabilities - excess ignored"));
      pstStartSettings->uiNumValidUserdataPIDs = BMUXLIB_TS_MAX_USERDATA_PIDS;
      bChanged = true;
   }

   if (pstStartSettings->uiServicePeriod == 0)
   {
      pstStartSettings->uiServicePeriod = BMUXLIB_TS_P_MUX_SERVICE_PERIOD_DEFAULT;
      BDBG_WRN(("Service period of 0 ms specified. Using default of %d ms", pstStartSettings->uiServicePeriod));
      bChanged = true;
   }

   /* SW7425-4340: Add app-specified A2PDelay */
   if (pstStartSettings->uiA2PDelay < BMUXLIB_TS_MIN_A2PDELAY)
   {
      pstStartSettings->uiA2PDelay = BMUXLIB_TS_P_A2PDELAY_DEFAULT;
      BDBG_WRN(("Arrival-to-Presentation Delay of < 27000 ticks (1ms) specified. Using default of %d ms", pstStartSettings->uiA2PDelay/BMUXLIB_TS_P_SCALE_MS_TO_27MHZ));
      bChanged = true;
   }

   if (bUseDefaults)
      return bChanged;
   return bError;
}

static BERR_Code
BMUXlib_TS_P_AssignTransportChannel(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   unsigned uiTransportChannelIndex,
   unsigned uiPID,
   unsigned uiPIDChannelIndex,
   bool bIsTS,
   BMUXlib_TS_TransportChannelInterface *pChannelInterface
   )
{
   BERR_Code rc = BERR_SUCCESS;

   BKNI_Memset( pChannelInterface, 0, sizeof( *pChannelInterface ) );

   if ( NULL == hMuxTS->pstStatus->stOutput.stMCPB[uiTransportChannelIndex].hMuxMCPB )
   {
      /* Only one input assigned to this transport channel index, so it is a legacy PB channel, so use it as-is */
      *pChannelInterface = hMuxTS->pstSettings->stStartSettings.transport.stChannelInterface[uiTransportChannelIndex];
   }
   else
   {
      /* More than one inputs are assigned to this transport channel index, so it is a MCPB channel, so assign */
      BMUXlib_TS_MCPB_Channel_OpenSettings stOpenSettings;

      BMUXlib_TS_MCPB_Channel_GetDefaultOpenSettings( &stOpenSettings );
      stOpenSettings.bIsTS = bIsTS;
      stOpenSettings.uiInstance = hMuxTS->pstStatus->stOutput.stMCPB[uiTransportChannelIndex].uiNumOpenChannels++;
      stOpenSettings.uiPID = uiPID;
      stOpenSettings.uiPIDChannelIndex = uiPIDChannelIndex;

      rc = BERR_TRACE( BMUXlib_TS_MCPB_Channel_Open(
         hMuxTS->pstStatus->stOutput.stMCPB[uiTransportChannelIndex].hMuxMCPB,
         &hMuxTS->pstStatus->stOutput.stMCPB[uiTransportChannelIndex].ahMuxMCPBCh[stOpenSettings.uiInstance],
         &stOpenSettings
         ));

      if ( BERR_SUCCESS == rc )
      {
         pChannelInterface->pContext = hMuxTS->pstStatus->stOutput.stMCPB[uiTransportChannelIndex].ahMuxMCPBCh[stOpenSettings.uiInstance];
         pChannelInterface->fAddTransportDescriptors = (BMUXlib_TS_AddTransportDescriptors) BMUXlib_TS_MCPB_Channel_AddTransportDescriptors;
         pChannelInterface->fGetCompletedTransportDescriptors = (BMUXlib_TS_GetCompletedTransportDescriptors) BMUXlib_TS_MCPB_Channel_GetCompletedTransportDescriptors;
      }
   }

   return BERR_TRACE( rc );
}

/* BMUXlib_TS_Start - Configures the mux HW */
BERR_Code
BMUXlib_TS_Legacy_Start(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_StartSettings *pstStartSettings
         )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BMUXlib_TS_Start );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstStartSettings );
   BDBG_ASSERT( BMUXLIB_TS_P_SIGNATURE_STARTSETTINGS == pstStartSettings->uiSignature );

   /* verify required settings are present ... */
   if (( NULL == pstStartSettings->transport.stDeviceInterface.fGetTransportSettings)
         || (NULL == pstStartSettings->transport.stDeviceInterface.fSetTransportSettings)
         || (NULL == pstStartSettings->transport.stDeviceInterface.fGetTransportStatus))
   {
      BDBG_LEAVE( BMUXlib_TS_Start );
      return BERR_TRACE(BERR_INVALID_PARAMETER);
   }
   /* verify PCR parameters */
   if (pstStartSettings->stPCRData.uiPID == 0 && pstStartSettings->stPCRData.uiInterval != 0)
   {
      BDBG_LEAVE( BMUXlib_TS_Start );
      return BERR_TRACE(BERR_INVALID_PARAMETER);
   }

   /* Initialize state only if mux is stopped */
   if ( BMUXlib_State_eStopped == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
   {
      unsigned uiPIDIndex;
      uint32_t *pPIDTable = hMuxTS->aFoundPIDs;
      BMUXlib_Input_Handle aInputTable[BMUXLIB_TS_MAX_VIDEO_PIDS+BMUXLIB_TS_MAX_AUDIO_PIDS];
      BMUXlib_Input_StartSettings stInputStartSettings;
      BMUXlib_TS_P_BufferConfig stMemoryConfigLocal;
      uint32_t uiNumChannels = 0;

      /* NOTE: same count is used for both PendingList and MetadataPendingList */
      BKNI_Memset(hMuxTS->pstStatus->aTransportDescriptorPendingListCountTable, 0, sizeof(hMuxTS->pstStatus->aTransportDescriptorPendingListCountTable));

      {
         uint32_t uiTransportChannelIndex;

         for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
         {
            const BMUXlib_TS_TransportChannelInterface *pChannelInterface = &pstStartSettings->transport.stChannelInterface[uiTransportChannelIndex];
            hMuxTS->pstStatus->stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR = 0xFFFFFFFF;
            /* count valid channel interfaces */
            if ((NULL != pChannelInterface->fAddTransportDescriptors)
               && (NULL != pChannelInterface->fGetCompletedTransportDescriptors))
               uiNumChannels++;
         }
      }

      /* verify at least one channel is provided ... */
      if (0 == uiNumChannels)
      {
         BMUXlib_TS_P_FreeResources( hMuxTS );
         BMUXlib_TS_P_Reset( hMuxTS );    /* reset state since Expected ESCRs modified above */
         BDBG_LEAVE( BMUXlib_TS_Start );
         return BERR_TRACE(BERR_INVALID_PARAMETER);
      }

      hMuxTS->pstSettings->stStartSettings = *pstStartSettings;
      /* Validate the start settings
         Note: validate the stored copy, not the user-supplied input
         (since Validate may modify the settings if defaults used for example) */
      if (BMUXlib_TS_P_ValidateStartSettings(&hMuxTS->pstSettings->stStartSettings, false))
      {
         BMUXlib_TS_P_FreeResources( hMuxTS );
         BMUXlib_TS_P_Reset( hMuxTS );    /* reset state since Expected ESCRs modified above */
         BDBG_LEAVE( BMUXlib_TS_Start );
         return BERR_TRACE(BERR_INVALID_PARAMETER);
      }
      /* Verify the current system data bitrate is sufficient for the specified PCR interval
         => if invalid, set the system data bitrate to min required (i.e override with warning)s
         NOTE: The following must be done prior to the first use of the system data bitrate */
      if ( true == BMUXlib_TS_P_ValidateSystemDataBitrate(&hMuxTS->pstSettings->stMuxSettings,
                        (const BMUXlib_TS_StartSettings *)&hMuxTS->pstSettings->stStartSettings,
                        false) )
      {
         BDBG_ERR(("Something Broke! ... this should not happen! :)"));
      }

      if ( 0 != hMuxTS->pstSettings->stStartSettings.stPCRData.uiInterval )
      {
         /* mark the PCR PID as "in-use" */
         BMUXLIB_TS_PID_ENTRY_SET(pPIDTable, hMuxTS->pstSettings->stStartSettings.stPCRData.uiPID);
         hMuxTS->pstStatus->stPCRInfo.uiIntervalIn27Mhz = BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * hMuxTS->pstSettings->stStartSettings.stPCRData.uiInterval;
      }
      else
      {
         hMuxTS->pstStatus->stPCRInfo.uiIntervalIn27Mhz = BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * BMUXLIB_TS_P_MUX_PCR_INTERVAL_DEFAULT;
      }

      BMUXlib_TS_P_GetBufferConfigFromMemoryConfig( &hMuxTS->stCreateSettings.stMemoryConfig, &hMuxTS->stMemoryBuffers, &stMemoryConfigLocal );

      {
         BMUXlib_TS_P_TempMemoryConfig *pstTempMemoryConfig = (BMUXlib_TS_P_TempMemoryConfig *) BMUXlib_Malloc(sizeof(BMUXlib_TS_P_TempMemoryConfig));
         /* SW7425-5370: allocate memory to store the current memory config ... */
         /* NOTE: allocated in one block and then sub-allocate */
         if ( NULL != pstTempMemoryConfig )
         {
            BKNI_Memset(pstTempMemoryConfig, 0, sizeof(*pstTempMemoryConfig));
            pstTempMemoryConfig->stMuxConfig.stMuxStartSettings = hMuxTS->pstSettings->stStartSettings;
            pstTempMemoryConfig->stMuxConfig.stMuxSettings = hMuxTS->pstSettings->stMuxSettings;
            /* Get the current memory config that matches the supplied settings */
            BMUXlib_TS_P_GetMemoryConfig(&pstTempMemoryConfig->stMuxConfig, &pstTempMemoryConfig->stMemoryConfig);
            {
               BMUXlib_TS_P_MemoryType eMemoryType;

               for ( eMemoryType = 0; (eMemoryType < BMUXlib_TS_P_MemoryType_eMax) && ( BERR_SUCCESS == rc); eMemoryType++ )
               {
                  if ( stMemoryConfigLocal.stBufferInfo[eMemoryType].uiSize < pstTempMemoryConfig->stMemoryConfig.stMemoryConfig.stBufferInfo[eMemoryType].uiSize )
                  {
                     BDBG_ERR(("Not enough memory of type [%d] for requested configuration. (%d < %d)",
                        eMemoryType,
                        (int)stMemoryConfigLocal.stBufferInfo[eMemoryType].uiSize,
                        (int)pstTempMemoryConfig->stMemoryConfig.stMemoryConfig.stBufferInfo[eMemoryType].uiSize
                        ));
                     rc = BERR_TRACE( BERR_OUT_OF_DEVICE_MEMORY );
                  }
               }
            }

            if ( BERR_SUCCESS == rc )
            {
               /* Allocate resources from sub heap */
               rc = BMUXlib_TS_P_AllocateResources( hMuxTS, &pstTempMemoryConfig->stMemoryConfig );
               if ( BERR_SUCCESS == rc )
               {
                  BMUXlib_TS_P_MemoryType eMemoryType;

                  for ( eMemoryType = 0; (eMemoryType < BMUXlib_TS_P_MemoryType_eMax); eMemoryType++ )
                  {
                     BKNI_Memset(&pstTempMemoryConfig->stRangeAllocatorStatus, 0, sizeof(pstTempMemoryConfig->stRangeAllocatorStatus));
                     BMMA_RangeAllocator_GetStatus( hMuxTS->stSubHeap[eMemoryType].hMmaRangeAllocator, &pstTempMemoryConfig->stRangeAllocatorStatus);

                     BDBG_MODULE_MSG( BMUXLIB_TS_MEMORY, ("[%d] Total Required/Used (Slack): %u/%u (%d)",
                        eMemoryType,
                        (int)stMemoryConfigLocal.stBufferInfo[eMemoryType].uiSize,
                        (int)pstTempMemoryConfig->stRangeAllocatorStatus.allocatedSpace,
                        (int)(stMemoryConfigLocal.stBufferInfo[eMemoryType].uiSize - pstTempMemoryConfig->stRangeAllocatorStatus.allocatedSpace)
                        ));
                  }
               }
            }
            BKNI_Free(pstTempMemoryConfig);
         }
         else
         {
            BMUXlib_TS_P_FreeResources( hMuxTS );
            BMUXlib_TS_P_Reset( hMuxTS );    /* reset state since Expected ESCRs modified above */
            BDBG_LEAVE( BMUXlib_TS_Start );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
      }
      if (rc != BERR_SUCCESS)
      {
         BMUXlib_TS_P_FreeResources( hMuxTS );
         BMUXlib_TS_P_Reset( hMuxTS );    /* reset state since Expected ESCRs modified above */
         BDBG_LEAVE( BMUXlib_TS_Start );
         return rc;
      }

      /* SW7346-1363: Intialize input index LUT to 0xFFFFFFFF */
      {
         unsigned i;

         for ( i = 0; i < BMUXLIB_TS_MAX_VIDEO_PIDS; i++ )
         {
            hMuxTS->pstStatus->stInputIndexLUT.uiVideo[i] = 0xFFFFFFFF;
         }

         for ( i = 0; i < BMUXLIB_TS_MAX_AUDIO_PIDS; i++ )
         {
            hMuxTS->pstStatus->stInputIndexLUT.uiAudio[i] = 0xFFFFFFFF;
         }
      }

      /* Determine if we need MCPB support */
      BMUXlib_TS_P_SetupMCPB( hMuxTS );

      for ( uiPIDIndex = 0; (BERR_SUCCESS == rc) && (uiPIDIndex < BMUXLIB_TS_MAX_VIDEO_PIDS) && (uiPIDIndex < hMuxTS->pstSettings->stStartSettings.uiNumValidVideoPIDs); uiPIDIndex++ )
      {
         BMUXlib_VideoEncoderInterface *pEncoderInterface = &hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].stInputInterface;
         uint16_t uiPID = hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiPID;
         unsigned uiPIDChannelIndex = hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiPIDChannelIndex;

         BKNI_Memset( &hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs], 0, sizeof( hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs] ) );

         {
            unsigned uiTransportChannelIndex = hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiTransportChannelIndex;
            BMUXlib_TS_TransportChannelInterface *pChannelInterface = &hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen].stChannelInterface;

            rc = BERR_TRACE( BMUXlib_TS_P_AssignTransportChannel( hMuxTS, uiTransportChannelIndex, uiPID, uiPIDChannelIndex, false, pChannelInterface ) );
            if ( BERR_SUCCESS != rc ) break;

            hMuxTS->pstStatus->stInput.video[uiPIDIndex].uiTransportChannelIndex = hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen++;;

            if ( ( NULL == pEncoderInterface->fGetBufferDescriptors )
                 || ( NULL == pEncoderInterface->fGetBufferStatus )
                 || ( NULL == pEncoderInterface->fConsumeBufferDescriptors )
                 || ( NULL == pChannelInterface->fAddTransportDescriptors )
                 || ( NULL == pChannelInterface->fGetCompletedTransportDescriptors )
               )
            {
               BDBG_ERR(("Video[%d]: Bad Transport Channel, or Encoder Interface", uiPIDIndex));
               rc = BERR_TRACE(BERR_INVALID_PARAMETER);
               break;
            }
         }

         /* verify PID is not already in use by another input, or by PCR ... */
         /* SW7425-4643: see if PCR PID is already in use by video/audio
            (the mux does not support PCR on same PID as other data, due
             to hardware limitations with creation of correct CC values) */
         if (BMUXLIB_TS_PID_ENTRY_IS_SET(pPIDTable, uiPID))
         {
            if (uiPID == hMuxTS->pstSettings->stStartSettings.stPCRData.uiPID)
               BDBG_ERR(("Video[%d]: Mux does not support PCR on same PID as video input: %x (use a separate PID for PCR)",
                  uiPIDIndex, uiPID));
            else
               BDBG_ERR(("Video[%d]: PID %x is already in use", uiPIDIndex, uiPID));
            rc = BERR_TRACE(BERR_INVALID_PARAMETER);
            break;
         }

         BMUXlib_Input_GetDefaultStartSettings( &stInputStartSettings );
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput = hMuxTS->ahInput[hMuxTS->pstStatus->uiNumInputs];
         stInputStartSettings.eType = BMUXlib_Input_Type_eVideo;
         stInputStartSettings.interface.stVideo = *pEncoderInterface;
         /* SW7425-659: In MTU BPP mode, for video, use frame burst mode so we can calculate the correct PKT2PKT delta */
         stInputStartSettings.eBurstMode = hMuxTS->pstSettings->stStartSettings.bSupportTTS ? BMUXlib_Input_BurstMode_eFrame : BMUXlib_Input_BurstMode_eDescriptor;
         stInputStartSettings.pMetadata = &hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs];
         stInputStartSettings.uiMuxId = hMuxTS->stCreateSettings.uiMuxId;
         stInputStartSettings.uiTypeInstance = uiPIDIndex;
         /* by default, this input will be marked as "active" */
         rc = BERR_TRACE( BMUXlib_Input_Start( hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
                         &stInputStartSettings
                        ));
         if (BERR_SUCCESS != rc)
            break;

         if ( 0 == hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiPESStreamID )
         {
            BDBG_WRN(("Video[%d]: Stream ID is invalid. Using %02x instead", uiPIDIndex, 0xE0+uiPIDIndex));
            hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiPESStreamID = 0xE0 + uiPIDIndex;
         }

         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiInputIndex = hMuxTS->pstStatus->uiNumInputs;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiTransportChannelIndex = hMuxTS->pstStatus->stInput.video[uiPIDIndex].uiTransportChannelIndex;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPID = uiPID;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPESStreamID = hMuxTS->pstSettings->stStartSettings.video[uiPIDIndex].uiPESStreamID;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPIDIndex = uiPIDIndex;

         /* setup userdata companion video info */
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].pstUserdata = &hMuxTS->pstUserdataStatus->stUserdataVideoInfo[uiPIDIndex];

         hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stInput.video[uiPIDIndex].uiTransportChannelIndex].bActive = true;

         aInputTable[hMuxTS->pstStatus->uiNumInputs] = hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput;
         /* indicate this PID is in use, for later PID collision detection ...*/
         BMUXLIB_TS_PID_ENTRY_SET(pPIDTable, uiPID);

         /* Determine amount of Transport Descriptor Pending List Entries (Video) */
         /* (A single allocation for the lists is done once all count requirements are established) */
         hMuxTS->pstStatus->aTransportDescriptorPendingListCountTable[hMuxTS->pstStatus->stInput.video[uiPIDIndex].uiTransportChannelIndex] += BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP(hMuxTS->pstStatus->stMemoryConfig.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eVideo].uiCount, hMuxTS->pstSettings->stStartSettings.uiNumValidVideoPIDs);

         /* SW7346-1363: Set default input enable mode (video) */
         {
            BMUXlib_Input_Settings stSettings;

            BMUXlib_Input_GetSettings(
               hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
               &stSettings
            );

            stSettings.bEnable = hMuxTS->pstSettings->stMuxSettings.stInputEnable.bVideo[uiPIDIndex];

            BMUXlib_Input_SetSettings(
               hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
               &stSettings
            );

            hMuxTS->pstStatus->stInputIndexLUT.uiVideo[uiPIDIndex] = hMuxTS->pstStatus->uiNumInputs;
         }
         hMuxTS->pstStatus->uiNumInputs++;
      }  /* end: for each video input */

      for ( uiPIDIndex = 0; (BERR_SUCCESS == rc) && (uiPIDIndex < BMUXLIB_TS_MAX_AUDIO_PIDS) && (uiPIDIndex < hMuxTS->pstSettings->stStartSettings.uiNumValidAudioPIDs); uiPIDIndex++ )
      {
         BMUXlib_AudioEncoderInterface *pEncoderInterface = &hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].stInputInterface;
         uint16_t uiPID = hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiPID;
         unsigned uiPIDChannelIndex = hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiPIDChannelIndex;

         BKNI_Memset( &hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs], 0, sizeof( hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs] ) );

         {
            unsigned uiTransportChannelIndex = hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiTransportChannelIndex;
            BMUXlib_TS_TransportChannelInterface *pChannelInterface = &hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen].stChannelInterface;

            rc = BMUXlib_TS_P_AssignTransportChannel( hMuxTS, uiTransportChannelIndex, uiPID, uiPIDChannelIndex, false, pChannelInterface );
            if ( BERR_SUCCESS != rc ) break;

            hMuxTS->pstStatus->stInput.audio[uiPIDIndex].uiTransportChannelIndex = hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen++;;

            if ( ( NULL == pEncoderInterface->fGetBufferDescriptors )
                 || ( NULL == pEncoderInterface->fGetBufferStatus )
                 || ( NULL == pEncoderInterface->fConsumeBufferDescriptors )
                 || ( NULL == pChannelInterface->fAddTransportDescriptors )
                 || ( NULL == pChannelInterface->fGetCompletedTransportDescriptors )
               )
            {
               BDBG_ERR(("Audio[%d]: Bad Transport Channel, or Encoder Interface", uiPIDIndex));
               rc = BERR_TRACE(BERR_INVALID_PARAMETER);
               break;
            }
         }

         /* verify PID is not already in use by any other input, or by PCR ... */
         /* SW7425-4643: see if PCR PID is already in use by video/audio
            (the mux does not support PCR on same PID as other data, due
             to hardware limitations with creation of correct CC values) */
         if (BMUXLIB_TS_PID_ENTRY_IS_SET(pPIDTable, uiPID))
         {
            if (uiPID == hMuxTS->pstSettings->stStartSettings.stPCRData.uiPID)
               BDBG_ERR(("Audio[%d]: Mux does not support PCR on same PID as audio input: %x (use a separate PID for PCR)",
               uiPIDIndex, uiPID));
            else
               BDBG_ERR(("Audio[%d]: PID %x is already in use", uiPIDIndex, uiPID));
            rc = BERR_TRACE(BERR_INVALID_PARAMETER);
            break;
         }

         BMUXlib_Input_GetDefaultStartSettings( &stInputStartSettings );
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput = hMuxTS->ahInput[hMuxTS->pstStatus->uiNumInputs];
         stInputStartSettings.eType = BMUXlib_Input_Type_eAudio;
         stInputStartSettings.interface.stAudio = *pEncoderInterface;
         stInputStartSettings.eBurstMode = BMUXlib_Input_BurstMode_eFrame;
         stInputStartSettings.pMetadata = &hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs];
         stInputStartSettings.uiMuxId = hMuxTS->stCreateSettings.uiMuxId;
         stInputStartSettings.uiTypeInstance = uiPIDIndex;
         /* by default, this input will be marked as "active" */
         rc = BERR_TRACE( BMUXlib_Input_Start( hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
                         &stInputStartSettings
                        ));
         if (BERR_SUCCESS != rc)
            break;

         if ( 0 == hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiPESStreamID )
         {
            BDBG_WRN(("Audio[%d]: Stream ID is invalid. Using %02x instead", uiPIDIndex, 0xC0+uiPIDIndex));
            hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiPESStreamID = 0xC0 + uiPIDIndex;
         }

         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiInputIndex = hMuxTS->pstStatus->uiNumInputs;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiTransportChannelIndex = hMuxTS->pstStatus->stInput.audio[uiPIDIndex].uiTransportChannelIndex;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPID = uiPID;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPESStreamID = hMuxTS->pstSettings->stStartSettings.audio[uiPIDIndex].uiPESStreamID;
         hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].uiPIDIndex = uiPIDIndex;

         hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stInput.audio[uiPIDIndex].uiTransportChannelIndex].bActive = true;

         aInputTable[hMuxTS->pstStatus->uiNumInputs] = hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput;
         /* indicate this PID is in use, for later PID collision detection ...*/
         BMUXLIB_TS_PID_ENTRY_SET(pPIDTable, uiPID);

         /* Determine amount of Transport Descriptor Pending List Entries (Audio) */
         /* (A single allocation for the lists is done once all count requirements are established) */
         hMuxTS->pstStatus->aTransportDescriptorPendingListCountTable[hMuxTS->pstStatus->stInput.audio[uiPIDIndex].uiTransportChannelIndex] += BMUXLIB_TS_P_DIVIDE_WITH_ROUND_UP(hMuxTS->pstStatus->stMemoryConfig.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eAudio].uiCount, hMuxTS->pstSettings->stStartSettings.uiNumValidAudioPIDs);

         /* SW7346-1363: Set default input enable mode (audio) */
         {
            BMUXlib_Input_Settings stSettings;

            BMUXlib_Input_GetSettings(
               hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
               &stSettings
            );

            stSettings.bEnable = hMuxTS->pstSettings->stMuxSettings.stInputEnable.bAudio[uiPIDIndex];

            BMUXlib_Input_SetSettings(
               hMuxTS->pstStatus->stInputMetaData[hMuxTS->pstStatus->uiNumInputs].hInput,
               &stSettings
            );

            hMuxTS->pstStatus->stInputIndexLUT.uiAudio[uiPIDIndex] = hMuxTS->pstStatus->uiNumInputs;
         }

         hMuxTS->pstStatus->uiNumInputs++;
      }  /* end: for each audio input */

      if (0 != hMuxTS->pstStatus->uiNumInputs)
      {
         /* relocate the pre-queued system data to the main system data pending list,
            and check the PIDs for conflict */
         if (BERR_SUCCESS == rc)
            rc = BMUXlib_TS_P_RelocatePreQSystemData(hMuxTS);

         if (BERR_SUCCESS == rc)
         {
            {
               unsigned uiTransportChannelIndex = hMuxTS->pstSettings->stStartSettings.stPCRData.uiTransportChannelIndex;
               BMUXlib_TS_TransportChannelInterface *pChannelInterface = &hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen].stChannelInterface;
               unsigned uiPIDChannelIndex = hMuxTS->pstSettings->stStartSettings.stPCRData.uiPIDChannelIndex;

               rc = BMUXlib_TS_P_AssignTransportChannel( hMuxTS, uiTransportChannelIndex, 0, uiPIDChannelIndex, true, pChannelInterface );
               if ( BERR_SUCCESS == rc )
               {
                  hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex = hMuxTS->pstStatus->stOutput.uiNumTransportChannelsOpen++;;

                  /* check specified channel for system data is valid ... */
                  if ((NULL == pChannelInterface->fAddTransportDescriptors )
                     || (NULL == pChannelInterface->fGetCompletedTransportDescriptors ))
                  {
                     BDBG_ERR(("Bad Transport Channel provided for System Data"));
                     rc = BERR_TRACE(BERR_INVALID_PARAMETER);
                  }
               }
            }

            /* Determine amount of Transport Descriptor Pending List Entries (System) */
            /* (A single allocation for the lists is done once all count requirements are established) */
            hMuxTS->pstStatus->aTransportDescriptorPendingListCountTable[hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex] += hMuxTS->pstStatus->stMemoryConfig.astMemoryEntry[BMUXlib_TS_P_MemoryEntryType_eTransportDescriptor][BMUXlib_TS_P_InputType_eSystem].uiCount;
            hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex].bActive = true;
         }

         /* create the input group for the active inputs */
         if (BERR_SUCCESS == rc)
         {
            BMUXlib_InputGroup_StartSettings stStartSettings;
            BMUXlib_InputGroup_GetDefaultStartSettings(&stStartSettings);
            stStartSettings.uiInputCount = hMuxTS->pstStatus->uiNumInputs;
            stStartSettings.pInputTable = aInputTable;
            rc = BMUXlib_InputGroup_Start(hMuxTS->hInputGroup, &stStartSettings);
            if (BERR_SUCCESS == rc)
            {
               BMUXlib_InputGroup_Settings stSettings;

               BMUXlib_InputGroup_GetSettings(hMuxTS->hInputGroup, &stSettings);
               /* set the selector function to be used by InputGroup_GetNextInput() */
               switch ( hMuxTS->pstSettings->stStartSettings.eInterleaveMode )
               {
                  case BMUXlib_TS_InterleaveMode_ePTS:
                     stSettings.fSelector = BMUXlib_InputGroup_DescriptorSelectLowestDTS;
                     break;

                  default:
                     stSettings.fSelector = BMUXlib_InputGroup_DescriptorSelectLowestESCR;
               }

               rc = BMUXlib_InputGroup_SetSettings(hMuxTS->hInputGroup, &stSettings);
            }

            /* userdata */
            if (BERR_SUCCESS == rc)
               rc = BMUXlib_TS_P_UserdataInit(hMuxTS);

            /* TS Null Packet and Dummy PES Frame */
            if (BERR_SUCCESS == rc)
            {
               if ( 0 != BMUXLIB_LIST_COUNT(&hMuxTS->stBPPFreeList) )
               {
                  BMUXLIB_LIST_REMOVE(
                        &hMuxTS->stBPPFreeList,
                        &hMuxTS->pstStatus->pDummyPESBuffer
                  );

                  BKNI_Memcpy(
                     BMUXLIB_LIST_ENTRY_DATA(hMuxTS->pstStatus->pDummyPESBuffer),
                     &s_stDummyPESFrame,
                     sizeof( BMUXlib_TS_P_BPPData )
                  );
               }
               else
               {
                  rc = BERR_TRACE( BERR_UNKNOWN );
               }

               if ( BERR_SUCCESS == rc )
               {
                  if ( 0 != BMUXLIB_LIST_COUNT(&hMuxTS->stTSPacketFreeList) )
                  {
                     BMUXLIB_LIST_REMOVE(
                           &hMuxTS->stTSPacketFreeList,
                           &hMuxTS->pstStatus->pNullTSPacketBuffer
                           );

                     BKNI_Memcpy(
                        BMUXLIB_LIST_ENTRY_DATA(hMuxTS->pstStatus->pNullTSPacketBuffer),
                        &s_stNullTSPacket,
                        sizeof( BMUXlib_TS_P_TSPacket )
                     );
                  }
                  else
                  {
                     rc = BERR_TRACE( BERR_UNKNOWN );
                  }
               }
            }
         }
         if ( BERR_SUCCESS == rc )
         {
            /* start the mux ... */
            BMUXLIB_TS_P_SET_MUX_STATE(hMuxTS, BMUXlib_State_eStarted);
         }
         else
         {
            /* Startup failed ... cleanup anything that may have been created */
            BMUXlib_TS_P_FreeResources( hMuxTS );
            BMUXlib_TS_P_Reset( hMuxTS );
         }
      }
      else
      {
         /* not enough inputs supplied */
         BMUXlib_TS_P_FreeResources( hMuxTS );
         BMUXlib_TS_P_Reset( hMuxTS );
         rc = BERR_TRACE(BERR_INVALID_PARAMETER);
      }
   } /* end: if state = stopped */
   else
   {
      rc = BERR_TRACE(BERR_INVALID_PARAMETER);
   }

   BDBG_LEAVE( BMUXlib_TS_Start );
   return rc;
}

BERR_Code
BMUXlib_TS_Legacy_Finish(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_FinishSettings *pstFinishSettings
         )
{
   BERR_Code rc = BERR_SUCCESS;
   BDBG_ENTER( BMUXlib_TS_Finish );
   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   rc = BMUXlib_TS_P_Finish(hMuxTS, pstFinishSettings);

   BDBG_LEAVE( BMUXlib_TS_Finish );
   return rc;
}

static BERR_Code
BMUXlib_TS_P_Finish(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_FinishSettings *pstFinishSettings
         )
{
   BERR_Code rc = BERR_SUCCESS;
   BDBG_ENTER( BMUXlib_TS_P_Finish );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstFinishSettings );
   BDBG_ASSERT( BMUXLIB_TS_P_SIGNATURE_FINISHSETTINGS == pstFinishSettings->uiSignature );

   switch ( BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
   {
      case BMUXlib_State_eStarted:
         {
            hMuxTS->pstSettings->stFinishSettings = *pstFinishSettings;
            switch ( hMuxTS->pstSettings->stFinishSettings.eFinishMode )
            {
               case BMUXlib_FinishMode_ePrepareForStop:
                  BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("Finishing Input") );
                  BMUXLIB_TS_P_SET_MUX_STATE(hMuxTS, BMUXlib_State_eFinishingInput);
                  break;

               default:
                  BDBG_ERR(("Unknown finish mode specified"));
                  rc = BERR_TRACE( BERR_INVALID_PARAMETER );
                  break;
            }
         }
         break;

      case BMUXlib_State_eFinishingInput:
      case BMUXlib_State_eFinishingOutput:
      case BMUXlib_State_eFinished:
         /* do nothing if invoked from these states - already finishing! */
         rc = BERR_SUCCESS;
         break;

      default:
         rc = BERR_TRACE( BERR_NOT_SUPPORTED );
         break;
   }

   BDBG_LEAVE( BMUXlib_TS_P_Finish );
   return BERR_TRACE( rc );
}

BERR_Code
BMUXlib_TS_Legacy_Stop(
         BMUXlib_TS_Legacy_Handle hMuxTS
         )
{
   BERR_Code rc = BERR_SUCCESS;
   BDBG_ENTER( BMUXlib_TS_Legacy_Stop );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   switch ( BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
   {
      case BMUXlib_State_eStarted:
      case BMUXlib_State_eFinishingInput:
         /* coverity[unterminated_case] */
      case BMUXlib_State_eFinishingOutput:
         BDBG_MSG(("Not finished yet. Output may be corrupt!"));

         /* coverity[fallthrough] */
      case BMUXlib_State_eFinished:
         BMUXLIB_TS_P_SET_MUX_STATE(hMuxTS, BMUXlib_State_eStopped);
#if BMUXLIB_TS_P_DUMP_TRANSPORT_DESC
         {
            uint32_t uiTransportIndex;

            for ( uiTransportIndex = 0; uiTransportIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportIndex++ )
            {
               if ( NULL != hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hDescDumpFile )
               {
                  fclose( hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hDescDumpFile );
                  hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hDescDumpFile = NULL;
               }
            }
         }
#endif
#if BMUXLIB_TS_P_DUMP_TRANSPORT_PES
         {
            uint32_t uiTransportIndex;

            for ( uiTransportIndex = 0; uiTransportIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportIndex++ )
            {
               if ( NULL != hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hPESDumpFile )
               {
                  fclose( hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hPESDumpFile );
                  hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stDebug.hPESDumpFile = NULL;
               }
            }
         }
#endif
#if BMUXLIB_TS_P_DUMP_PCR
         if ( NULL != hMuxTS->pstStatus->stPCRInfo.hPCRFile )
         {
            fclose(hMuxTS->pstStatus->stPCRInfo.hPCRFile);
            hMuxTS->pstStatus->stPCRInfo.hPCRFile = NULL;
         }
#endif
         break;

      case BMUXlib_State_eStopped:
         BDBG_WRN(("Already Stopped"));
         break;

      default:
         rc = BERR_INVALID_PARAMETER;
         break;
   }

   /* Return NULL TS Packet and dummy PES frame */
   if ( NULL != hMuxTS->pstStatus->pDummyPESBuffer )
   {
      BMUXLIB_LIST_ADD(
         &hMuxTS->stBPPFreeList,
         hMuxTS->pstStatus->pDummyPESBuffer
      );
      hMuxTS->pstStatus->pDummyPESBuffer = NULL;
   }

   if ( NULL != hMuxTS->pstStatus->pNullTSPacketBuffer )
   {
      BMUXLIB_LIST_ADD(
         &hMuxTS->stTSPacketFreeList,
         hMuxTS->pstStatus->pNullTSPacketBuffer
      );
      hMuxTS->pstStatus->pNullTSPacketBuffer = NULL;
   }

   /* Reset State */
   BMUXlib_TS_P_FreeResources( hMuxTS );
   BMUXlib_TS_P_Reset( hMuxTS );

   BDBG_LEAVE( BMUXlib_TS_Legacy_Stop );
   return BERR_TRACE( rc );
}

/***************/
/* System Data */
/***************/
static BERR_Code
BMUXlib_TS_P_ValidateSystemDataBuffers(
         const BMUXlib_TS_SystemData *astSystemDataBuffer, /* Array of system data buffers */
         size_t uiCount /* Count of system data buffers in array */
         )
{
   BERR_Code rc = BERR_SUCCESS;
   unsigned i;

   for ( i = 0; i < uiCount; i++ )
   {
      if ( (0 == astSystemDataBuffer[i].uiSize)
          || (0 != ( astSystemDataBuffer[i].uiSize % 188 ))) {
         BDBG_ERR(("System buffer @%p size of %d bytes is NOT a multiple of MPEG TS packet size (188 bytes)",
                  (void *)&astSystemDataBuffer[i],
                  (int)astSystemDataBuffer[i].uiSize
                  ));

         rc = BERR_TRACE(BERR_INVALID_PARAMETER);
         break;
      }
      else if (NULL == astSystemDataBuffer[i].hBlock)
      {
         BDBG_ERR(("System buffer @%p size of %d bytes has no data (hBlock = NULL)",
                  (void *)&astSystemDataBuffer[i],
                  (int)astSystemDataBuffer[i].uiSize
                  ));

         rc = BERR_TRACE(BERR_INVALID_PARAMETER);
         break;
      }
   }

   return BERR_TRACE( rc );
}

BERR_Code
BMUXlib_TS_Legacy_AddSystemDataBuffers(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_SystemData *astSystemDataBuffer, /* Array of system data buffers */
         size_t uiCount, /* Count of system data buffers in array */
         size_t *puiQueuedCount /* Count of system data buffers queued by muxer (*puiQueuedCount <= uiCount) */
         )
{
   BERR_Code rc = BERR_SUCCESS;
   BDBG_ENTER( BMUXlib_TS_AddSystemDataBuffers );

   BDBG_ASSERT( puiQueuedCount );
   BDBG_ASSERT( astSystemDataBuffer );
   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);

   *puiQueuedCount = 0;
   rc = BMUXlib_TS_P_ValidateSystemDataBuffers( astSystemDataBuffer, uiCount );

   if ( rc == BERR_SUCCESS )
   {
      rc = BMUXlib_TS_P_AddSystemDataBuffers(hMuxTS, astSystemDataBuffer, uiCount, puiQueuedCount);
   }
   BDBG_LEAVE( BMUXlib_TS_AddSystemDataBuffers );
   return BERR_TRACE( rc );
}

BERR_Code
BMUXlib_TS_P_AddSystemDataBuffers(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         const BMUXlib_TS_SystemData *astSystemDataBuffer, /* Array of system data buffers */
         size_t uiCount, /* Count of system data buffers in array */
         size_t *puiQueuedCount /* Count of system data buffers queued by muxer (*puiQueuedCount <= uiCount) */
         )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BMUXlib_TS_P_AddSystemDataBuffers );

   *puiQueuedCount = 0;

   while ( ( uiCount != *puiQueuedCount )
           && ( false == BMUXLIB_LIST_ISEMPTY( &hMuxTS->stSystemDataFreeList ) )
         )
   {
      /* SW7425-4643: We cannot support PCR PID on same PID as system data
         for each packet in the system data buffer that is queued up, extract the PID and verify that it
         does not conflict with the PCR pid (from the start settings), or with Audio or Video PIDs
         If PCR PID is zero, start settings have not been set yet, so don't bother checking yet (this is
         being invoked as a pre-Q operation) */
      if (hMuxTS->pstSettings->stStartSettings.stPCRData.uiPID != 0)
      {
         void *pDataBase = BMMA_Lock( astSystemDataBuffer[*puiQueuedCount].hBlock );
         unsigned char *pData = (unsigned char *)((uint8_t *) pDataBase + astSystemDataBuffer[*puiQueuedCount].uiBlockOffset);
         unsigned uiLength = astSystemDataBuffer[*puiQueuedCount].uiSize;

         while ((uiLength >= 188) && (rc == BERR_SUCCESS))
         {
            uint16_t uiPID = ((uint16_t)(pData[1] & 0x1F) << 8) | pData[2];

            if (BMUXLIB_TS_PID_ENTRY_IS_SET(hMuxTS->aFoundPIDs, uiPID))
            {
               if (uiPID == hMuxTS->pstSettings->stStartSettings.stPCRData.uiPID)
                  BDBG_ERR(("Mux does not support PCR on same PID as System Data: %x (use a separate PID for PCR)", uiPID));
               else
                  BDBG_ERR(("System Data PID %x is aleady in use by Audio or Video input", uiPID));
               rc = BERR_TRACE(BERR_INVALID_PARAMETER);
            }
            uiLength -= 188;
            pData += 188;
         }
         BMMA_Unlock( astSystemDataBuffer[*puiQueuedCount].hBlock, pDataBase );
         if (rc != BERR_SUCCESS)
            break;
      }

      {
         BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_SystemData)* pSystemDataEntry;
         BMUXLIB_LIST_REMOVE( &hMuxTS->stSystemDataFreeList, &pSystemDataEntry );
         *BMUXLIB_LIST_ENTRY_DATA( pSystemDataEntry ) = astSystemDataBuffer[*puiQueuedCount];
         BMUXLIB_LIST_ADD( &hMuxTS->stSystemDataPendingList, pSystemDataEntry );
      }

#ifdef BMUXLIB_TS_P_TEST_MODE
      /* SW7425-3958: do not write data to CSV files if these entries are due to move of data from pre-Q to pending Q */
      if (false == hMuxTS->pstStatus->bSystemDataPreQ )
      {
         /* Write CSV containing info about system data: offset in data file, length, insertion interval, etc */
         /* system data is written as raw binary data to the data file */
         FILE *hCSVFile = hMuxTS->pstStatus->stSystemDataInfo.hCSVFile;
         FILE *hDataFile = hMuxTS->pstStatus->stSystemDataInfo.hDataFile;

         if (!hMuxTS->pstStatus->stSystemDataInfo.bCSVOpened)
         {
            char fname[256];
            sprintf(fname, "BMUXlib_TS_SystemData_%2.2d.csv", hMuxTS->stCreateSettings.uiMuxId);
            hCSVFile = fopen(fname, "w");
            if (NULL == hCSVFile)
               BDBG_ERR(("Error Creating System Data CSV File (%s)", fname));
            hMuxTS->pstStatus->stSystemDataInfo.hCSVFile = hCSVFile;
            sprintf(fname, "BMUXlib_TS_SystemData_%2.2d.dat", hMuxTS->stCreateSettings.uiMuxId);
            hDataFile = fopen(fname, "wb");
            if (NULL == hDataFile)
               BDBG_ERR(("Error Creating System Data File (%s)", fname));
            hMuxTS->pstStatus->stSystemDataInfo.hDataFile = hDataFile;

            hMuxTS->pstStatus->stSystemDataInfo.bCSVOpened = true;

            if (hCSVFile != NULL)
               fprintf(hCSVFile, "offset, length, insertion_interval\n");
         }
         if (hCSVFile != NULL && hDataFile != NULL)
         {
            void *pBufferBaseAddress = BMMA_Lock( astSystemDataBuffer[*puiQueuedCount].hBlock );
            fprintf(hCSVFile, "%u, %u, %u\n", (unsigned)ftell(hDataFile), astSystemDataBuffer[*puiQueuedCount].uiSize,
               astSystemDataBuffer[*puiQueuedCount].uiTimestampDelta);
            fwrite((void*)((unsigned)pBufferBaseAddress + astSystemDataBuffer[*puiQueuedCount].uiBlockOffset), sizeof(uint8_t),
               astSystemDataBuffer[*puiQueuedCount].uiSize, hDataFile);
            BMMA_Unlock( astSystemDataBuffer[*puiQueuedCount].hBlock, pBufferBaseAddress );
         }
      }
#endif
      (*puiQueuedCount)++;
   } /* end: while data to queue && pending list not full */
   BDBG_LEAVE( BMUXlib_TS_P_AddSystemDataBuffers );
   return BERR_TRACE( rc );
}

BERR_Code
BMUXlib_TS_Legacy_GetCompletedSystemDataBuffers(
         BMUXlib_TS_Legacy_Handle hMuxTS,
         size_t *puiCompletedCount /* Returns count of system data buffers fully muxed */
         )
{
   BDBG_ENTER( BMUXlib_TS_GetCompletedSystemDataBuffers );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( puiCompletedCount );

   *puiCompletedCount = BMUXLIB_TS_P_GET_SYS_DATA_COMP_CNT(hMuxTS);
   BMUXLIB_TS_P_SET_SYS_DATA_COMP_CNT(hMuxTS, 0);

   BDBG_LEAVE( BMUXlib_TS_GetCompletedSystemDataBuffers );
   return BERR_TRACE( BERR_SUCCESS );
}

/**********/
/* Status */
/**********/
void
BMUXlib_TS_Legacy_GetStatus(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   BMUXlib_TS_Status *pstStatus
   )
{
   BDBG_ENTER( BMUXlib_TS_GetStatus );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstStatus );

   *pstStatus = hMuxTS->pstStatus->stStatus;

   BDBG_LEAVE( BMUXlib_TS_GetStatus );
}

/***********/
/* Execute */
/***********/
BERR_Code
BMUXlib_TS_Legacy_DoMux(
   BMUXlib_TS_Legacy_Handle hMuxTS,
   BMUXlib_DoMux_Status *pstStatus
   )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BMUXlib_TS_DoMux );

   BDBG_OBJECT_ASSERT(hMuxTS, BMUXlib_TS_Legacy_P_Context);
   BDBG_ASSERT( pstStatus );

   BKNI_Memset( pstStatus, 0, sizeof( *pstStatus ) );

   /* Debug code to print ESCR for current execution */
   if (NULL != hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.fGetTransportStatus)
   {
      hMuxTS->pstStatus->stPreviousTransportStatus = hMuxTS->pstStatus->stTransportStatus;

      hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.fGetTransportStatus(
               hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.pContext,
               &hMuxTS->pstStatus->stTransportStatus
               );

      if ( false == hMuxTS->pstSettings->stStartSettings.bNonRealTimeMode )
      {
         if ( true == hMuxTS->pstStatus->bTransportStatusValid )
         {
            if ( hMuxTS->pstStatus->stTransportStatus.uiESCR > hMuxTS->pstStatus->stPreviousTransportStatus.uiESCR )
            {
               uint32_t uiMaxExpectedMSP = hMuxTS->pstSettings->stStartSettings.uiServiceLatencyTolerance + hMuxTS->pstStatus->stDoMuxStatus.uiNextExecutionTime;
               uint32_t uiDeltaESCR = hMuxTS->pstStatus->stTransportStatus.uiESCR - hMuxTS->pstStatus->stPreviousTransportStatus.uiESCR;
               uiDeltaESCR /= 27000;

               if ( uiDeltaESCR > uiMaxExpectedMSP )
               {
                  BDBG_WRN(("MUX Service Latency Too Large! (%d > %d ms)", uiDeltaESCR, uiMaxExpectedMSP ));
               }
            }
         }
      }

      hMuxTS->pstStatus->bTransportStatusValid = true;

      BDBG_MSG(("STC = "BDBG_UINT64_FMT", PACING = %08x",
               BDBG_UINT64_ARG(hMuxTS->pstStatus->stTransportStatus.uiSTC),
               hMuxTS->pstStatus->stTransportStatus.uiESCR
               ));
   }

#if BMUXLIB_TS_P_ENABLE_STATS
   /* SW7425-5841: Update efficiency statistics - subtract oldest stats */
   {
      BMUXlib_TS_P_DataType eDataType;
      BMUXlib_TS_P_SourceType eSourceType;

      for ( eDataType = 0; eDataType < BMUXlib_TS_P_DataType_eMax; eDataType ++ )
      {
         for ( eSourceType = 0; eSourceType < BMUXlib_TS_P_SourceType_eMax; eSourceType++ )
         {
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[eDataType][eSourceType] -= hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerDataType[eDataType] -= hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerSourceType[eSourceType] -= hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes -= hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex] = 0;
         }
      }
      hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes -= hMuxTS->pstStatus->stEfficiencyStats.uiPacketizationOverhead[hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
      hMuxTS->pstStatus->stEfficiencyStats.uiPacketizationOverhead[hMuxTS->pstStatus->stEfficiencyStats.uiIndex] = 0;

      hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs -= hMuxTS->pstStatus->stEfficiencyStats.uiTimeInMs[hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
      hMuxTS->pstStatus->stEfficiencyStats.uiTimeInMs[hMuxTS->pstStatus->stEfficiencyStats.uiIndex] = hMuxTS->pstSettings->stStartSettings.uiServicePeriod;

      if ( true == hMuxTS->pstStatus->bTransportStatusValid )
      {
         uint32_t uiDeltaESCR = hMuxTS->pstStatus->stTransportStatus.uiESCR - hMuxTS->pstStatus->stPreviousTransportStatus.uiESCR;

         hMuxTS->pstStatus->stEfficiencyStats.uiTimeInMs[hMuxTS->pstStatus->stEfficiencyStats.uiIndex] = uiDeltaESCR/27000;
      }
   }
#endif

   /* SW7425-659: Initialize the system data pkt2pkt delta */
   if ( false == hMuxTS->pstStatus->bBTPSent )
   {
      uint64_t uiPacket2PacketTimestampDelta = ((uint64_t)BMUXlib_TS_P_TSPacket_MAXSIZE * 8 * 27000000) / hMuxTS->pstSettings->stMuxSettings.uiSystemDataBitRate;

      hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.bValid = true;
      hMuxTS->pstStatus->stOutput.stTransport[hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.uiValue = uiPacket2PacketTimestampDelta;
   }

   if ( ( BMUXlib_State_eStopped != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
        && ( BMUXlib_State_eFinished != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
       )
   {
      BMUXlib_TS_P_ProcessCompletedBuffers( hMuxTS );

      if ( BMUXlib_State_eFinishingOutput == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
      {
         unsigned uiIndex;
         bool bMuxOutputComplete = true;

         /* Make sure all transport descriptors have been consumed */
         for ( uiIndex = 0; uiIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiIndex++ )
         {
            if ( true == hMuxTS->pstStatus->stOutput.stTransport[uiIndex].bActive )
            {
               if ( false == BMUXLIB_LIST_ISEMPTY( &hMuxTS->stTransportDescriptorPendingList[uiIndex] ) )
               {
                  BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("TRANSPORT[%d] has pending descriptors", uiIndex) );
                  bMuxOutputComplete = false;
                  break;
               }
            }
         }

         if ( true == bMuxOutputComplete )
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("Finished") );
            BMUXLIB_TS_P_SET_MUX_STATE(hMuxTS, BMUXlib_State_eFinished);
         }
      }
      else if ( ( BMUXlib_State_eFinishingInput == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
                || ( BMUXlib_State_eStarted == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
              )
      {
         bool bMuxInputComplete = true;

         BMUXlib_TS_P_ProcessNewBuffers( hMuxTS );

         /* Check if we've seen an EOS on all active A/V Channels */
         {
            unsigned uiIndex;

             /* Check if EOS seen on all input channels associated with this Transport Channel */
             for ( uiIndex = 0; uiIndex < hMuxTS->pstStatus->uiNumInputs ; uiIndex++ )
             {
               if ( false == hMuxTS->pstStatus->stInputMetaData[uiIndex].bEOSSeen )
               {
                  bMuxInputComplete = false;
                  break;
               }
             }
         }

         /* SW7425-998: Auto-Finish if EOS is seen on all active inputs */
         if ( BMUXlib_State_eStarted == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
         {
            if ( true == bMuxInputComplete )
            {
               BMUXlib_TS_FinishSettings stFinishSettings;

               BMUXlib_TS_GetDefaultFinishSettings( &stFinishSettings );

               BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("Auto Finish") );
               BMUXlib_TS_P_Finish(
                        hMuxTS,
                        &stFinishSettings
                        );
            }
         }

            /* SW7425-831: If we've seen an EOS on all inputs, and we're in NRT mode,
             * then we want to flush all output channels by inserting a dummy descriptor
             * with an ESCR=to the largestESCR.  That way, the XPT PB Pause logic won't
             * trigger if one output finishes before the others.
             */

         /* SW7425-999: In NRT mode, we need to insert dummy descriptors for all
          * inputs that have seen EOS to ensure the they don't cause the non-EOS
          * inputs to stall
          */
         if ( true == hMuxTS->pstSettings->stStartSettings.bNonRealTimeMode )
         {
            bMuxInputComplete &= BMUXlib_TS_P_Flush( hMuxTS, bMuxInputComplete);
         }

         if ( BMUXlib_State_eFinishingInput == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
         {
            /* SW7425-2825/SW7425-5055: Since system data is queued up until the lastESCR
             * that has been queued to the transport, we need to continue to process
             * system data until all pending descriptors have been queued to the transport.
             */
            BMUXlib_TS_P_ProcessSystemData( hMuxTS );
            {
               unsigned uiTransportChannelIndex;

               for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
               {
                  size_t uiNumEntries;

                  {
                     BMUXLIB_LIST_GETNUMENTRIES(
                        &hMuxTS->stTransportDescriptorPendingList[uiTransportChannelIndex],
                        &uiNumEntries
                        );

                     bMuxInputComplete &= (uiNumEntries == hMuxTS->pstStatus->stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued);
                  }
               }
            }

            if ( true == bMuxInputComplete )
            {
               BDBG_MODULE_MSG( BMUXLIB_TS_FINISH, ("Finishing Output") );

               BMUXLIB_TS_P_SET_MUX_STATE(hMuxTS, BMUXlib_State_eFinishingOutput);
            }
         }
      }

      if ( false == hMuxTS->pstStatus->bTransportConfigured )
      {
         BMUXlib_TS_P_ConfigureTransport( hMuxTS );

         /* SW7425-659: In NRT mode, since TS MUXlib is seeding the PACING_COUNTER,
          * we need to resample the status to ensure subsequent code
          * (E.g. Late ESCR detection) uses a valid value
          */
         if (NULL != hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.fGetTransportStatus)
         {
            hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.fGetTransportStatus(
                     hMuxTS->pstSettings->stStartSettings.transport.stDeviceInterface.pContext,
                     &hMuxTS->pstStatus->stTransportStatus
                     );

            BDBG_MSG(("STC = "BDBG_UINT64_FMT", PACING = %08x (After Config)",
                     BDBG_UINT64_ARG(hMuxTS->pstStatus->stTransportStatus.uiSTC),
                     hMuxTS->pstStatus->stTransportStatus.uiESCR
                     ));
         }
      }

      if ( true == hMuxTS->pstStatus->bTransportConfigured )
      {
         BMUXlib_TS_P_ScheduleProcessedBuffers( hMuxTS );
      }
   }

   if ( ( BMUXlib_State_eStopped != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
        && ( BMUXlib_State_eFinished != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
       )
   {
      unsigned uiTransportIndex;

      for ( uiTransportIndex = 0; uiTransportIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportIndex++ )
      {
         if ( true == hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].bActive )
         {

            size_t uiTransportPending;

            BMUXLIB_LIST_GETNUMENTRIES(
                     &hMuxTS->stTransportDescriptorPendingList[uiTransportIndex],
                     &uiTransportPending
                     );

            BDBG_MODULE_MSG(BMUXLIB_TS_PENDING, ("T[%2d]=%08x (ESCR=%08x --> %08x)",
               uiTransportIndex,
               (int)uiTransportPending,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingPending.uiLastStartingESCR,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingPending.uiNextExpectedESCR
            ));

            BDBG_MODULE_MSG(BMUXLIB_TS_QUEUED, ("T[%2d]=%08x (ESCR=%08x --> %08x, PKT2PKT=%08x (%d ms))",
               uiTransportIndex,
               (int)uiTransportPending,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingQueued.uiLastStartingESCR,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingQueued.uiNextExpectedESCR,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingQueued.uiCurrentPacket2PacketTimestampDelta,
               hMuxTS->pstStatus->stOutput.stTransport[uiTransportIndex].stTimingQueued.uiCurrentPacket2PacketTimestampDelta / 27000
            ));

            BDBG_MODULE_MSG(BMUXLIB_TS_TRANSPORT, ("T[%2d] added/completed/returned = %08x,%08x,%08x",
               uiTransportIndex,
               hMuxTS->pstStatus->stTransport[uiTransportIndex].uiDescriptorsAdded,
               hMuxTS->pstStatus->stTransport[uiTransportIndex].uiDescriptorsCompleted,
               hMuxTS->pstStatus->stTransport[uiTransportIndex].uiDescriptorsReturned
            ));
         }
      }
   }

   pstStatus->eState = BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS);

   pstStatus->uiNextExecutionTime = hMuxTS->pstSettings->stStartSettings.uiServicePeriod + hMuxTS->pstStatus->iExecutionTimeAdjustment;
   hMuxTS->pstStatus->iExecutionTimeAdjustment = 0;

   /* SW7425-3684: Update completed duration */
   if ( ( BMUXlib_State_eStopped != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
        && ( BMUXlib_State_eFinished != BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
        && ( NULL != hMuxTS->hInputGroup )
       )
   {
      BMUXlib_InputGroup_Status stStatus;

      BMUXlib_InputGroup_GetStatus(
         hMuxTS->hInputGroup,
         &stStatus
         );

      pstStatus->uiCompletedDuration = stStatus.uiDuration;
   }

#if BMUXLIB_TS_P_ENABLE_STATS
   /* SW7425-5841: Update/Calc/Print efficiency statistics - add newest stats */
   {
      BMUXlib_TS_P_DataType eDataType;
      BMUXlib_TS_P_SourceType eSourceType;

      for ( eDataType = 0; eDataType < BMUXlib_TS_P_DataType_eMax; eDataType ++ )
      {
         for ( eSourceType = 0; eSourceType < BMUXlib_TS_P_SourceType_eMax; eSourceType++ )
         {
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[eDataType][eSourceType] += hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerDataType[eDataType] += hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerSourceType[eSourceType] += hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
            hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes += hMuxTS->pstStatus->stEfficiencyStats.uiNumBytes[eDataType][eSourceType][hMuxTS->pstStatus->stEfficiencyStats.uiIndex];
         }
      }
      hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes += hMuxTS->pstStatus->stEfficiencyStats.uiPacketizationOverhead[hMuxTS->pstStatus->stEfficiencyStats.uiIndex];

      hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs += hMuxTS->pstStatus->stEfficiencyStats.uiTimeInMs[hMuxTS->pstStatus->stEfficiencyStats.uiIndex];

      hMuxTS->pstStatus->stEfficiencyStats.uiIndex++;
      hMuxTS->pstStatus->stEfficiencyStats.uiIndex %= BMUXLib_TS_P_STATS_MAX_MSP_COUNT;

      if ( ( 0 != hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs/1000 ) && ( 0 != hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes ) )
      {
         hMuxTS->pstStatus->stStatus.stAverageBitrate.uiVideo = ( hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[BMUXlib_TS_P_DataType_eCDB][BMUXlib_TS_P_SourceType_eVideo] * 8 ) / (hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs/1000);
         hMuxTS->pstStatus->stStatus.stAverageBitrate.uiAudio = ( hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[BMUXlib_TS_P_DataType_eCDB][BMUXlib_TS_P_SourceType_eAudio] * 8 ) / (hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs/1000);
         hMuxTS->pstStatus->stStatus.stAverageBitrate.uiSystemData = hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerSourceType[BMUXlib_TS_P_SourceType_eSystem] * 8 / (hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs/1000);
         hMuxTS->pstStatus->stStatus.stAverageBitrate.uiUserData = hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerSourceType[BMUXlib_TS_P_SourceType_eUserdata] * 8 / (hMuxTS->pstStatus->stEfficiencyStats.uiTotalTimeInMs/1000);
         hMuxTS->pstStatus->stStatus.uiEfficiency = (hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[BMUXlib_TS_P_DataType_eCDB][BMUXlib_TS_P_SourceType_eVideo] + hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerInput[BMUXlib_TS_P_DataType_eCDB][BMUXlib_TS_P_SourceType_eAudio] + hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerDataType[BMUXlib_TS_P_DataType_eUserdataPTS] + hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesPerDataType[BMUXlib_TS_P_DataType_eUserdataLocal]) * 100 / hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytes;
         hMuxTS->pstStatus->stStatus.uiTotalBytes = hMuxTS->pstStatus->stEfficiencyStats.uiTotalBytesWritten;
         BDBG_MODULE_MSG( BMUXLIB_TS_STATS, ("Avg. Bitrate (kbps): V=%5u, A=%3u, S=%2u, U=%3u [%3u%%] ("BDBG_UINT64_FMT")",
            hMuxTS->pstStatus->stStatus.stAverageBitrate.uiVideo/1000,
            hMuxTS->pstStatus->stStatus.stAverageBitrate.uiAudio/1000,
            hMuxTS->pstStatus->stStatus.stAverageBitrate.uiSystemData/1000,
            hMuxTS->pstStatus->stStatus.stAverageBitrate.uiUserData/1000,
            hMuxTS->pstStatus->stStatus.uiEfficiency,
            BDBG_UINT64_ARG(hMuxTS->pstStatus->stStatus.uiTotalBytes)
         ) );
      }
   }
#endif

   /* Call MCPB DoMux*( as needed */
   {
      unsigned i;
      unsigned uiEndESCR = 0;
      bool bEndESCRValid = false;

      /* Determine the smallest active queued next ESCR to send to MCPB DoMux */
      {
         uint32_t uiTransportChannelIndex;

         /* Determine the largest ESCR to use for flushing the outputs */
         for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
         {
            signed iDeltaESCR = -1;

            /* Ignore this transport channel if it is not active */
            if ( false == hMuxTS->pstStatus->stOutput.stTransport[uiTransportChannelIndex].bActive ) continue;

            if ( BMUXlib_State_eFinishingOutput == BMUXLIB_TS_P_GET_MUX_STATE(hMuxTS) )
            {
               /* When finishing output, do not use system data channel for computing uiEndESCR since PCRs are generated internally based on the input timing */
               if ( uiTransportChannelIndex == hMuxTS->pstStatus->stInput.system.uiTransportChannelIndex ) continue;
            }

            /* If a transport queue is empty, check of all associated inputs are done */
            if ( true == BMUXLIB_LIST_ISEMPTY( &hMuxTS->stTransportDescriptorPendingList[uiTransportChannelIndex] ) )
            {
               bool bAllInputsForTransportChannelHasSeenEOS = false;
               unsigned uiIndex;

               /* Ignore this transport channel, if EOS seen on all associated inputs */
               for ( uiIndex = 0; uiIndex < hMuxTS->pstStatus->uiNumInputs ; uiIndex++ )
               {
                  if ( uiTransportChannelIndex == hMuxTS->pstStatus->stInputMetaData[uiIndex].uiTransportChannelIndex )
                  {
                     bAllInputsForTransportChannelHasSeenEOS = hMuxTS->pstStatus->stInputMetaData[uiIndex].bEOSSeen;

                     if ( false == bAllInputsForTransportChannelHasSeenEOS )
                     {
                        break;
                     }
                  }
               }
               if ( true == bAllInputsForTransportChannelHasSeenEOS ) continue;
            }

            if ( true == bEndESCRValid )
            {
               iDeltaESCR = hMuxTS->pstStatus->stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiNextExpectedESCR - uiEndESCR;
            }

            if ( iDeltaESCR < 0 )
            {
               uiEndESCR = hMuxTS->pstStatus->stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiNextExpectedESCR;
               bEndESCRValid = true;
            }
         }
      }

      if ( true == bEndESCRValid )
      {
         for ( i = 0; i < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; i++ )
         {
            if ( NULL != hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB )
            {
               if (BERR_SUCCESS != BMUXlib_TS_MCPB_DoMux( hMuxTS->pstStatus->stOutput.stMCPB[i].hMuxMCPB, uiEndESCR ))
               {
                  BDBG_ERR(("Unknown error calling BMUXlib_TS_MCPB_DoMux"));
               }
            }
         }
      }
   }

   hMuxTS->pstStatus->stDoMuxStatus = *pstStatus;

   BDBG_LEAVE( BMUXlib_TS_DoMux );
   return BERR_TRACE( rc );
}
