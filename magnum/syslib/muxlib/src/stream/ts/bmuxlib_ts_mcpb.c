/******************************************************************************
 *  Copyright (C) 2018 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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

#include "bmuxlib_ts_mcpb.h"
#include "bmuxlib_list.h"
#include "bkni.h"

#include "bmuxlib_debug.h"
#include "bmuxlib_alloc.h"
#if ( BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_DESC || BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_OUTPUT )
#include <stdio.h>
#endif

#include "bmuxlib_ts_priv.h"

BDBG_MODULE(BMUXLIB_TS_MCPB);
BDBG_FILE_MODULE(BMUXLIB_TS_MCPB_STATE);
BDBG_FILE_MODULE(BMUXLIB_TS_MCPB_QUEUE);
BDBG_FILE_MODULE(BMUXLIB_TS_MCPB_FIFO);

/**************************/
/* Internal Structs/Types */
/**************************/
typedef enum BMUXlib_TS_MCPB_P_SourceType
{
   BMUXlib_TS_MCPB_P_SourceType_eInput,
   BMUXlib_TS_MCPB_P_SourceType_eFIFO,
   BMUXlib_TS_MCPB_P_SourceType_eBTP,
   BMUXlib_TS_MCPB_P_SourceType_ePESHeaderExtended,

   BMUXlib_TS_MCPB_P_SourceType_eMax
} BMUXlib_TS_MCPB_P_SourceType;

typedef struct BMUXlib_TS_MCPB_QueueEntry
{
   unsigned uiBaseESCR; /* The start ESCR for the beginning of this entry */
   unsigned uiCurrentESCR; /* The start ESCR for the remaining data in this entry */
   unsigned uiPacket2PacketDelta;
   BMUXlib_TS_TransportDescriptor stTransportDescriptor; /* Transport Descriptor */
   unsigned uiBytesConsumed;
   unsigned uiBytesLeft;
   unsigned uiChannelInstance; /* Channel from which this transport descriptor is from */
   bool bRandomAccessIndication;
   /* Debug */
   unsigned uiSerialNumber;
   unsigned uiSerialNumberEnd;
   BMUXlib_TS_MCPB_P_SourceType eSource;
   unsigned uiSourceDescriptorCount; /* Indicates how many actual source descriptors are represented by this entry */
} BMUXlib_TS_MCPB_QueueEntry;

BMUXLIB_LIST_DEFINE(BMUXlib_TS_MCPB_QueueEntry)

typedef struct BMUXlib_TS_MCPB_P_PacketFifo
{
   BMMA_Block_Handle hBlock;
   BMMA_DeviceOffset uiOffset;
   void *pBuffer;
   size_t uiSize;
   unsigned uiRead;
   unsigned uiWrite;
   unsigned uiPacketRead; /* Start of packet */
} BMUXlib_TS_MCPB_P_PacketFifo;

typedef struct BMUXlib_TS_MCPB_P_Channel_Context
{
   BMUXlib_TS_MCPB_Handle hMuxMCPB;
   BMUXlib_TS_MCPB_Channel_OpenSettings stSettings;
   BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry) stDescriptorFifo;

   struct
   {
      bool bOpened;
      bool bActive;
      unsigned uiPacketSize;
      unsigned uiCurrentESCR;
      unsigned uiCurrentPKT2PKTDelta;
      unsigned uiQueuedCount;
      unsigned uiPendingCount;
      unsigned uiCompletedCount;
      unsigned uiSkippedDescriptorCount;
      struct
      {
         unsigned uiSourceDescriptorCount; /* How many descriptors of data are represented between packetRead and write */
         BMUXlib_TS_MCPB_QueueEntry stEntry;
         BMUXlib_TS_MCPB_P_PacketFifo stFifo;
      } stTSPacketInfo;
   } stState;
} BMUXlib_TS_MCPB_P_Channel_Context;

typedef enum BMUXlib_TS_MCPB_P_ProcessNextEntry_State
{
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eGetNextDescriptor = 0,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESDescriptor,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessLastBPP,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeader,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeaderExtended,
#if 0 /* Unused */
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeaderPayload,
#endif
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessTSDescriptor,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPayload,
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor
} BMUXlib_TS_MCPB_P_ProcessNextEntry_State;

typedef struct BMUXlib_TS_MCPB_P_TransportMetadata
{
   BMUXlib_TS_MCPB_P_SourceType eSource;
   unsigned uiChannelInstance;
   unsigned uiSourceDescriptorCount;
} BMUXlib_TS_MCPB_P_TransportMetadata;

#define BMUXLIB_TS_MCPB_P_PUSI_COUNT 2
#define BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT 128

typedef struct BMUXlib_TS_MCPB_P_BufferContext
{
   BMMA_Block_Handle hPUSIBtpBlock[BMUXLIB_TS_MCPB_P_PUSI_COUNT];
   BMMA_DeviceOffset uiPUSIBtpOffset[BMUXLIB_TS_MCPB_P_PUSI_COUNT];
   void *pPUSIBtp[BMUXLIB_TS_MCPB_P_PUSI_COUNT];
   size_t uiPUSIBtpSize;

   BMMA_Block_Handle hRAIExtendedBlock[BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT];
   BMMA_DeviceOffset uiRAIExtendedOffset[BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT];
   void *pRAIExtended[BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT];
   size_t uiRAIExtendedSize;
   unsigned uiRAIExtendedReadOffset;
   unsigned uiRAIExtendedWriteOffset;
} BMUXlib_TS_MCPB_P_BufferContext;

typedef struct BMUXlib_TS_MCPB_P_Context
{
   BMUXlib_TS_MCPB_CreateSettings stSettings;

   BMUXlib_TS_MCPB_P_Channel_Context *astChannel;

   BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)** astListQueue;

   struct
   {
      void *pBuffer;
      BMMA_DeviceOffset uiOffset;
   } stMuxSharedMemory;

   BMUXlib_TS_MCPB_P_BufferContext *pstBuffer;

   struct
   {
      BMUXlib_TS_MCPB_P_ProcessNextEntry_State eState;
      BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)* hList;
      BMUXlib_TS_MCPB_QueueEntry *pstEntry;
   } stProcessEntryInfo;

   struct
   {
      BMUXLIB_P_ENTRY_TYPE( BMUXlib_TS_TransportDescriptor, astTransportDescriptors )
      BMUXLIB_P_ENTRY_TYPE( BMUXlib_TS_MCPB_P_TransportMetadata, astMetadata )
      unsigned uiSize;
      unsigned uiReadOffset; /* Points to the first descriptor that has been processed */
      unsigned uiPendingOffset; /* Points to the descriptor that is next to be queued to transport */
      unsigned uiWriteOffset; /* Points to the last descriptor that has been processed */

      bool bLastChannelInstanceValid;
      unsigned uiLastChannelInstance;
   } stOutputQueue;

   unsigned uiProcessedBytes;
   unsigned uiBytesLeftInPacket;

   /* Debug */
   unsigned uiDoMuxCount;
   unsigned uiSerialNumber;
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_DESC
   FILE *hDescDumpFile;
   bool bHeaderWritten;
#endif
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_OUTPUT
   FILE *hOutputDumpFile;
#endif
} BMUXlib_TS_MCPB_P_Context;

/***********************************/
/*     Static Prototypes           */
/***********************************/
static int BMUXlib_TS_MCPB_P_CompareDescriptorFifo(const BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)*hDescriptorFifoA, const BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)*hDescriptorFifoB);
static void BMUXlib_TS_MCPB_P_ReconcileFifoQueue(BMUXlib_TS_MCPB_Handle hMuxMCPB);
static void BMUXlib_TS_MCPB_P_ConsumeDescriptor(BMUXlib_TS_MCPB_Handle hMuxMCPB);
static void BMUXlib_TS_MCPB_P_DumpDescriptor(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_MCPB_QueueEntry *pstEntry,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor, const BMUXlib_TS_MCPB_P_TransportMetadata *pstMetadata);
static bool BMUXlib_TS_MCPB_P_InsertBuffer(BMUXlib_TS_MCPB_Handle hMuxMCPB, BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXlib_TS_MCPB_P_TransportMetadata *pstMetadata, BMUXlib_TS_MCPB_QueueEntry *pstEntry);
static bool BMUXlib_TS_MCPB_P_InsertBuffer_PESHeaderExtended(BMUXlib_TS_MCPB_Handle hMuxMCPB, BMUXlib_TS_MCPB_QueueEntry *pstEntry);
static bool BMUXlib_TS_MCPB_P_InsertBuffer_PUSIBtp(BMUXlib_TS_MCPB_Handle hMuxMCPB, BMUXlib_TS_MCPB_QueueEntry *pstEntry);
static bool BMUXlib_TS_MCPB_P_InsertPayload(BMUXlib_TS_MCPB_Handle hMuxMCPB, BMUXlib_TS_MCPB_QueueEntry *pstEntry,
   BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_ProcessNextEntry(BMUXlib_TS_MCPB_Handle hMuxMCPB, unsigned uiEndESCR);
static bool BMUXlib_TS_MCPB_P_IsPESHeader(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_IsBPP(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_IsLastBPP(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_IsNULLPacket(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_IsBTP(BMUXlib_TS_MCPB_Handle hMuxMCPB, const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static void* BMUXlib_TS_MCPB_P_GetAddress(BMUXlib_TS_MCPB_Handle hMuxMCPB, uint64_t uiBufferOffset);
static void BMUXlib_TS_MCPB_P_UpdateCurrentESCR(BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh, const BMUXlib_TS_MCPB_QueueEntry *pstEntry);
static void BMUXlib_TS_MCPB_P_ClearTransportDescriptorFlags(BMUXlib_TS_TransportDescriptor *pstTransportDescriptor);
static bool BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsViaFifo(BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor, BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *apstEntry[2]);
static bool BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsDirect(BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor, BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) ** pstListEntry);
static void BMUXlib_TS_MCPB_P_ProcessCompletedTransportDescriptors(BMUXlib_TS_MCPB_Handle hMuxMCPB);
static void BMUXlib_TS_MCPB_P_PopulateEntryFromDescriptor(BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor, BMUXlib_TS_MCPB_QueueEntry *pstEntry);
static BERR_Code BMUXlib_TS_MCPB_P_ScheduleProcessedEntries(BMUXlib_TS_MCPB_Handle hMuxMCPB);

static unsigned BMUXLIB_TS_P_FIFO_PKTDEPTH(BMUXlib_TS_MCPB_P_PacketFifo *pstFifo);
static void BMUXLIB_TS_P_FIFO_PKTCONSUME(BMUXlib_TS_MCPB_P_PacketFifo *pstFifo, size_t uiLength);
static void BMUXLIB_TS_P_FIFO_CONSUME(BMUXlib_TS_MCPB_P_PacketFifo *pstFifo, size_t uiLength);
static void BMUXLIB_TS_P_FIFO_ADD(BMUXlib_TS_MCPB_P_PacketFifo *pstFifo, const void *pBuffer, size_t uiLength) ;
static unsigned BMUXLIB_TS_P_FIFO_FREE(BMUXlib_TS_MCPB_P_PacketFifo *pstFifo);


/***********************************/
/* Device Create/Destroy Functions */
/***********************************/

void
BMUXlib_TS_MCPB_GetDefaultCreateSettings(
         BMUXlib_TS_MCPB_CreateSettings *pstSettings
         )
{
   BDBG_ASSERT( pstSettings );

   BKNI_Memset( pstSettings, 0, sizeof( *pstSettings ) );

   pstSettings->uiMaxNumInputs = 1 + 6 + 1; /* 1 video, 6 audio, 1 system channel */
}

/* BMUXlib_TS_MCPB_Create - Allocates all system/device memory required for mux operation */
BERR_Code
BMUXlib_TS_MCPB_Create(
         BMUXlib_TS_MCPB_Handle *phMuxMCPB,  /* [out] TSMuxer handle returned */
         const BMUXlib_TS_MCPB_CreateSettings *pstSettings
         )
{
   BERR_Code rc = BERR_SUCCESS;
   BMUXlib_TS_MCPB_Handle hMuxMCPB = NULL;

   BDBG_ENTER( BMUXlib_TS_MCPB_Create );

   BDBG_ASSERT( phMuxMCPB );
   BDBG_ASSERT( pstSettings );

   *phMuxMCPB = NULL;

   hMuxMCPB = BMUXlib_Malloc( sizeof( BMUXlib_TS_MCPB_P_Context ) );
   if ( NULL == hMuxMCPB )
   {
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   }

   BKNI_Memset( hMuxMCPB, 0, sizeof( *hMuxMCPB ) );

   BMUXLIB_P_CONTEXT_ALLOCATE(BMUXlib_TS_MCPB_P_BufferContext, hMuxMCPB->pstBuffer, alloc_desc_error )

   hMuxMCPB->stSettings = *pstSettings;
   hMuxMCPB->uiBytesLeftInPacket = 0;

   /* Create List Queue */
   hMuxMCPB->astListQueue = (BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)**) BMUXlib_Malloc( sizeof(BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)*) * hMuxMCPB->stSettings.uiMaxNumInputs );
   if ( NULL == hMuxMCPB->astListQueue )
   {
      BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   }

   /* Allocate input channels */
   hMuxMCPB->astChannel = BMUXlib_Malloc( hMuxMCPB->stSettings.uiMaxNumInputs * sizeof( *hMuxMCPB->astChannel ) );
   if ( NULL == hMuxMCPB->astChannel )
   {
      BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   }

   BKNI_Memset( hMuxMCPB->astChannel, 0, hMuxMCPB->stSettings.uiMaxNumInputs * sizeof( *hMuxMCPB->astChannel ) );

   /* Allocate input descriptor FIFOs */
   {
      unsigned i;

      for ( i = 0; i < hMuxMCPB->stSettings.uiMaxNumInputs; i++ )
      {
         BMUXLIB_LIST_INIT(&hMuxMCPB->astChannel[i].stDescriptorFifo);
         hMuxMCPB->astListQueue[i] = &hMuxMCPB->astChannel[i].stDescriptorFifo;
      }
   }

   /* Allocate PUSI BTP Buffer */
   hMuxMCPB->pstBuffer->uiPUSIBtpSize = BMUXlib_TS_P_TSPacket_MAXSIZE;
   {
      unsigned i;

      for ( i = 0; i < BMUXLIB_TS_MCPB_P_PUSI_COUNT; i++ )
      {
         hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] = BMMA_Alloc( hMuxMCPB->stSettings.hMma, hMuxMCPB->pstBuffer->uiPUSIBtpSize, 0, 0 );
         if ( NULL == hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
         hMuxMCPB->pstBuffer->pPUSIBtp[i] = BMMA_Lock( hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] );
         if ( NULL == hMuxMCPB->pstBuffer->pPUSIBtp[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
         hMuxMCPB->pstBuffer->uiPUSIBtpOffset[i] = BMMA_LockOffset( hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] );
         if ( BMUXLIB_TS_P_INVALID_OFFSET == hMuxMCPB->pstBuffer->uiPUSIBtpOffset[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }

         /* Initialize PUSI BTP */
         BKNI_Memcpy(
            hMuxMCPB->pstBuffer->pPUSIBtp[i],
            &s_stDefaultBTPPacket,
            sizeof( BMUXlib_TS_P_TSPacket )
         );

         BMUXlib_TS_P_BTPData_SetControlWord(
            hMuxMCPB->pstBuffer->pPUSIBtp[i],
            BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND_OFFSET,
            BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND /* MTU BTP/BPP op-code*/
            );
      }

      BMUXlib_TS_P_BTPData_SetControlWord(
         hMuxMCPB->pstBuffer->pPUSIBtp[0],
         BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_OFFSET,
         BMUXLIB_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_SET_PUSI_IN_NEXT_PKT | BMUXLIB_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_SET_ADAPTATION_FIELD_TO_3 /* SET_PUSI_IN_NEXT_PKT and SET_ADAPTATION_FIELD_TO_3 is valid */
      );

      BMUXlib_TS_P_BTPData_SetControlWord(
         hMuxMCPB->pstBuffer->pPUSIBtp[1],
         BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_OFFSET,
         BMUXLIB_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_SET_PUSI_IN_NEXT_PKT | BMUXLIB_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_SET_ADAPTATION_FIELD_TO_3 /* SET_PUSI_IN_NEXT_PKT and SET_ADAPTATION_FIELD_TO_3 is valid */
      );

      for ( i = 0; i < BMUXLIB_TS_MCPB_P_PUSI_COUNT; i++ )
      {
         BMMA_FlushCache(
            hMuxMCPB->pstBuffer->hPUSIBtpBlock[i],
            hMuxMCPB->pstBuffer->pPUSIBtp[i],
            hMuxMCPB->pstBuffer->uiPUSIBtpSize
            );
      }
   }

   /* Allocate RAI Extended Buffer */
   hMuxMCPB->pstBuffer->uiRAIExtendedSize = 184;
   hMuxMCPB->pstBuffer->uiRAIExtendedReadOffset = 0;
   hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset = 0;
   {
      unsigned i;
      for ( i = 0; i < BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT; i++ )
      {
         hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] = BMMA_Alloc( hMuxMCPB->stSettings.hMma, hMuxMCPB->pstBuffer->uiRAIExtendedSize, 0, 0 );
         if ( NULL == hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
         hMuxMCPB->pstBuffer->pRAIExtended[i] = BMMA_Lock( hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] );
         if ( NULL == hMuxMCPB->pstBuffer->pRAIExtended[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }
         hMuxMCPB->pstBuffer->uiRAIExtendedOffset[i] = BMMA_LockOffset( hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] );
         if ( BMUXLIB_TS_P_INVALID_OFFSET == hMuxMCPB->pstBuffer->uiRAIExtendedOffset[i] )
         {
            BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
            BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
            return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
         }

      }
   }

   /* Allocate Output Queue */
   hMuxMCPB->stOutputQueue.uiSize = 1024;
   BMUXLIB_P_ENTRY_ALLOCATE(
         BMUXlib_TS_MCPB_P_TransportMetadata,
         hMuxMCPB->stOutputQueue.astMetadata,
         hMuxMCPB->stOutputQueue.uiSize,
         alloc_desc_error )

   BMUXLIB_P_ENTRY_ALLOCATE(
         BMUXlib_TS_TransportDescriptor,
         hMuxMCPB->stOutputQueue.astTransportDescriptors,
         hMuxMCPB->stOutputQueue.uiSize,
         alloc_desc_error )

   /* Calculate MUXlib shared block virtual address/physical offset */
   if ( NULL == hMuxMCPB->stSettings.stMuxSharedMemory.hBlock )
   {
      BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_INVALID_PARAMETER );
   }
   hMuxMCPB->stMuxSharedMemory.pBuffer = BMMA_Lock( hMuxMCPB->stSettings.stMuxSharedMemory.hBlock );
   if ( NULL == hMuxMCPB->stMuxSharedMemory.pBuffer )
   {
      BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_INVALID_PARAMETER );
   }
   hMuxMCPB->stMuxSharedMemory.uiOffset = BMMA_LockOffset( hMuxMCPB->stSettings.stMuxSharedMemory.hBlock );
   if ( BMUXLIB_TS_P_INVALID_OFFSET == hMuxMCPB->stMuxSharedMemory.uiOffset )
   {
      BMUXlib_TS_MCPB_Destroy( hMuxMCPB );
      BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
      return BERR_TRACE( BERR_INVALID_PARAMETER );
   }

   /* Debug */
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_DESC
   {
      char fname[256];
      sprintf(fname, "BMUXlib_MCPB_TRANSPORT_DESC_%08x.csv", (unsigned) hMuxMCPB);
      hMuxMCPB->hDescDumpFile = fopen(fname, "wb");
      if ( NULL == hMuxMCPB->hDescDumpFile )
      {
         BDBG_ERR(("Error Creating Transport Descriptor Dump File (%s)", fname));
      }
      else
      {
         BDBG_WRN(("Creating Transport Descriptor Dump File (%s)", fname));
      }
   }
#endif
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_OUTPUT
   {
      char fname[256];
      sprintf(fname, "BMUXlib_MCPB_TRANSPORT_OUTPUT_%08x.bin", (unsigned) hMuxMCPB);
      hMuxMCPB->hOutputDumpFile = fopen(fname, "wb");
      if ( NULL == hMuxMCPB->hOutputDumpFile )
      {
         BDBG_ERR(("Error Creating Transport Output Dump File (%s)", fname));
      }
      else
      {
         BDBG_WRN(("Creating Transport Output Dump File (%s)", fname));
      }
   }
#endif

   *phMuxMCPB = hMuxMCPB;
   goto alloc_done;

alloc_desc_error:
   rc = BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   BMUXlib_TS_MCPB_Destroy( hMuxMCPB );

alloc_done:
   BDBG_LEAVE( BMUXlib_TS_MCPB_Create );
   return BERR_TRACE( rc );
}

/* BMUXlib_TS_MCPB_Destroy - Frees all system/device memory allocated */
void
BMUXlib_TS_MCPB_Destroy(
         BMUXlib_TS_MCPB_Handle hMuxMCPB
         )
{
   BDBG_ENTER( BMUXlib_TS_MCPB_Destroy );

#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_DESC
   if ( NULL != hMuxMCPB->hDescDumpFile )
   {
      fclose( hMuxMCPB->hDescDumpFile );
      hMuxMCPB->hDescDumpFile = NULL;
   }
#endif
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_OUTPUT
   if ( NULL != hMuxMCPB->hOutputDumpFile )
   {
      fclose( hMuxMCPB->hOutputDumpFile );
      hMuxMCPB->hOutputDumpFile = NULL;
   }
#endif

   /* Free MUXlib shared block virtual address/physical offset */
   if ( NULL != hMuxMCPB->stSettings.stMuxSharedMemory.hBlock )
   {
      if ( BMUXLIB_TS_P_INVALID_OFFSET != hMuxMCPB->stMuxSharedMemory.uiOffset )
      {
         BMMA_UnlockOffset( hMuxMCPB->stSettings.stMuxSharedMemory.hBlock, hMuxMCPB->stMuxSharedMemory.uiOffset );
         hMuxMCPB->stMuxSharedMemory.uiOffset = BMUXLIB_TS_P_INVALID_OFFSET;
      }

      if ( NULL != hMuxMCPB->stMuxSharedMemory.pBuffer )
      {
         BMMA_Unlock( hMuxMCPB->stSettings.stMuxSharedMemory.hBlock, hMuxMCPB->stMuxSharedMemory.pBuffer );
         hMuxMCPB->stMuxSharedMemory.pBuffer = NULL;
      }
   }

   /* Free Output Queue */
   BMUXLIB_P_ENTRY_FREE(
         BMUXlib_TS_TransportDescriptor,
         hMuxMCPB->stOutputQueue.astTransportDescriptors,
         hMuxMCPB->stOutputQueue.uiSize )

   BMUXLIB_P_ENTRY_FREE(
         BMUXlib_TS_MCPB_P_TransportMetadata,
         hMuxMCPB->stOutputQueue.astMetadata,
         hMuxMCPB->stOutputQueue.uiSize )

   {
      unsigned i;
      for ( i = 0; i < BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT; i++ )
      {
         if ( NULL != hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] )
         {
            if ( BMUXLIB_TS_P_INVALID_OFFSET != hMuxMCPB->pstBuffer->uiRAIExtendedOffset[i] )
            {
               BMMA_UnlockOffset( hMuxMCPB->pstBuffer->hRAIExtendedBlock[i], hMuxMCPB->pstBuffer->uiRAIExtendedOffset[i] );
               hMuxMCPB->pstBuffer->uiRAIExtendedOffset[i] = BMUXLIB_TS_P_INVALID_OFFSET;
            }

            if ( NULL != hMuxMCPB->pstBuffer->pRAIExtended[i] )
            {
               BMMA_Unlock( hMuxMCPB->pstBuffer->hRAIExtendedBlock[i], hMuxMCPB->pstBuffer->pRAIExtended[i] );
               hMuxMCPB->pstBuffer->pRAIExtended[i] = NULL;
            }

            BMMA_Free( hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] );
            hMuxMCPB->pstBuffer->hRAIExtendedBlock[i] = NULL;
         }
      }
   }

   {
      unsigned i;
      for ( i = 0; i < BMUXLIB_TS_MCPB_P_PUSI_COUNT ; i++ )
      {
         if ( NULL != hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] )
         {
            if ( BMUXLIB_TS_P_INVALID_OFFSET != hMuxMCPB->pstBuffer->uiPUSIBtpOffset[i] )
            {
               BMMA_UnlockOffset( hMuxMCPB->pstBuffer->hPUSIBtpBlock[i], hMuxMCPB->pstBuffer->uiPUSIBtpOffset[i] );
               hMuxMCPB->pstBuffer->uiPUSIBtpOffset[i] = BMUXLIB_TS_P_INVALID_OFFSET;
            }

            if ( NULL != hMuxMCPB->pstBuffer->pPUSIBtp[i] )
            {
               BMMA_Unlock( hMuxMCPB->pstBuffer->hPUSIBtpBlock[i], hMuxMCPB->pstBuffer->pPUSIBtp[i] );
               hMuxMCPB->pstBuffer->pPUSIBtp[i] = NULL;
            }

            BMMA_Free( hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] );
            hMuxMCPB->pstBuffer->hPUSIBtpBlock[i] = NULL;
         }
      }
   }

   /* Allocate input descriptor FIFOs */
   {
      unsigned i;
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntry;

      for ( i = 0; i < hMuxMCPB->stSettings.uiMaxNumInputs; i++ )
      {
         while(!BMUXLIB_LIST_ISEMPTY(&hMuxMCPB->astChannel[i].stDescriptorFifo))
         {
            BMUXLIB_LIST_REMOVE(&hMuxMCPB->astChannel[i].stDescriptorFifo, &pstListEntry);
            BKNI_Free(pstListEntry);
         }
      }
   }

   if ( NULL != hMuxMCPB->astChannel )
   {
      BMUXLIB_P_CONTEXT_FREE(hMuxMCPB->pstBuffer)

      BKNI_Free( hMuxMCPB->astChannel );
      hMuxMCPB->astChannel = NULL;
   }

   if ( NULL != hMuxMCPB->astListQueue )
   {
      BKNI_Free( hMuxMCPB->astListQueue );
   }

   BKNI_Free( hMuxMCPB );

   BDBG_LEAVE( BMUXlib_TS_MCPB_Destroy );
}


/********************************/
/* Channel Open/Close Functions */
/********************************/
void
BMUXlib_TS_MCPB_Channel_GetDefaultOpenSettings(
   BMUXlib_TS_MCPB_Channel_OpenSettings *pstSettings
         )
{
   BDBG_ASSERT( pstSettings );

   BKNI_Memset( pstSettings, 0, sizeof( *pstSettings ) );
}

BERR_Code
BMUXlib_TS_MCPB_Channel_Open(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   BMUXlib_TS_MCPB_Channel_Handle *phMuxMCPBCh,
   const BMUXlib_TS_MCPB_Channel_OpenSettings *pstSettings
   )
{
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh = NULL;

   BDBG_ENTER( BMUXlib_TS_MCPB_Channel_Open );

   BDBG_ASSERT( hMuxMCPB );
   BDBG_ASSERT( phMuxMCPBCh );
   BDBG_ASSERT( pstSettings );

   *phMuxMCPBCh = NULL;

   if ( pstSettings->uiInstance >= hMuxMCPB->stSettings.uiMaxNumInputs )
   {
      BDBG_ERR(("Invalid Channel Instance"));
      BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
      return BERR_TRACE( BERR_UNKNOWN );
   }

   if ( true == hMuxMCPB->astChannel[pstSettings->uiInstance].stState.bOpened )
   {
      BDBG_ERR(("Channel is already open!"));
      BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
      return BERR_TRACE( BERR_UNKNOWN );
   }

   hMuxMCPBCh = &hMuxMCPB->astChannel[pstSettings->uiInstance];
   BKNI_Memset( &hMuxMCPBCh->stState, 0, sizeof( hMuxMCPBCh->stState ) );
   hMuxMCPBCh->hMuxMCPB = hMuxMCPB;
   hMuxMCPBCh->stSettings = *pstSettings;

   hMuxMCPBCh->stState.bOpened = true;
   hMuxMCPBCh->stState.bActive = true;
   hMuxMCPBCh->stState.uiPacketSize = hMuxMCPBCh->stSettings.bIsTS ? BMUXlib_TS_P_TSPacket_MAXSIZE : BMUXlib_TS_P_BPPData_MAXSIZE;

   /* If TS, allocate FIFO to aggregate descriptors into multiples of TS Packets */
   if ( true == hMuxMCPBCh->stSettings.bIsTS )
   {
      /* Allocate a buffer to handle up to 8 userdata PIDs each having 1504/bytes per frame. Assume 50 fps and 200 ms of buffering */
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize = 1504 * 50/5 * 8;

      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock = BMMA_Alloc( hMuxMCPB->stSettings.hMma, hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize, 0, 0 );
      if ( NULL == hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock )
      {
         BMUXlib_TS_MCPB_Channel_Close( hMuxMCPBCh );
         BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer = BMMA_Lock( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock );
      if ( NULL == hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer )
      {
         BMUXlib_TS_MCPB_Channel_Close( hMuxMCPBCh );
         BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset = BMMA_LockOffset( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock );
      if ( BMUXLIB_TS_P_INVALID_OFFSET == hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset )
      {
         BMUXlib_TS_MCPB_Channel_Close( hMuxMCPBCh );
         BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
         return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
      }
   }

   *phMuxMCPBCh = hMuxMCPBCh;

   BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Open );
   return BERR_TRACE( BERR_SUCCESS );
}

void
BMUXlib_TS_MCPB_Channel_Close(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh
   )
{
   BDBG_ENTER( BMUXlib_TS_MCPB_Channel_Close );

   BDBG_ASSERT( hMuxMCPBCh );

   /* If TS, free FIFO used to aggregate descriptors */
   if ( true == hMuxMCPBCh->stSettings.bIsTS )
   {
      if ( NULL != hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock )
      {
         if ( BMUXLIB_TS_P_INVALID_OFFSET != hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset )
         {
            BMMA_UnlockOffset( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock, hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset );
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset = BMUXLIB_TS_P_INVALID_OFFSET;
         }

         if ( NULL != hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer )
         {
            BMMA_Unlock( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock, hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer );
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer = NULL;
         }

         BMMA_Free( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock );
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock = NULL;
      }
   }

   hMuxMCPBCh->stState.bOpened = false;
   hMuxMCPBCh->stState.bActive = false;

   BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_Close );
}

/*********************/
/* Private Functions */
/*********************/

static int BMUXlib_TS_MCPB_P_CompareDescriptorFifo(
   const BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)*hDescriptorFifoA,
   const BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)*hDescriptorFifoB
   )
{
   int result;
   BDBG_ENTER( BMUXlib_TS_MCPB_P_CompareDescriptorFifo );

   BDBG_ASSERT( hDescriptorFifoA );
   BDBG_ASSERT( hDescriptorFifoB );

   {
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntryA = NULL;
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntryB = NULL;

      if ( false == BMUXLIB_LIST_ISEMPTY( hDescriptorFifoA ) )
      {
         BMUXLIB_LIST_FIRST( hDescriptorFifoA, &pstListEntryA );
      }

      if ( false == BMUXLIB_LIST_ISEMPTY( hDescriptorFifoB ) )
      {
         BMUXLIB_LIST_FIRST( hDescriptorFifoB, &pstListEntryB );
      }

      if ( ( NULL == pstListEntryA ) && ( NULL == pstListEntryB ) )
      {
         return 0;
      }
      else if ( NULL == pstListEntryA )
      {
         return 1;
      }
      else if ( NULL == pstListEntryB )
      {
         return -1;
      }
      else
      {
         BMUXlib_TS_MCPB_QueueEntry *pstEntryA = BMUXLIB_LIST_ENTRY_DATA(pstListEntryA);
         BMUXlib_TS_MCPB_QueueEntry *pstEntryB = BMUXLIB_LIST_ENTRY_DATA(pstListEntryB);

         int32_t iDeltaESCR = pstEntryA->uiCurrentESCR - pstEntryB->uiCurrentESCR;

         if ( iDeltaESCR < 0 )
         {
            result = -1;
         }
         else if ( iDeltaESCR > 0 )
         {
            result = 1;
         }
         else
         {
            result = 0;
         }
      }
   }

   BDBG_ENTER( BMUXlib_TS_MCPB_P_CompareDescriptorFifo );

   return result;
}

static void* BMUXlib_TS_MCPB_P_GetAddress(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   uint64_t uiBufferOffset
   )
{
   if ( ( uiBufferOffset >= hMuxMCPB->stMuxSharedMemory.uiOffset )
        && ( ( uiBufferOffset < ( hMuxMCPB->stMuxSharedMemory.uiOffset + hMuxMCPB->stSettings.stMuxSharedMemory.uiSize ) ) ) )
   {
      return (void*) ( ((uint8_t *) hMuxMCPB->stMuxSharedMemory.pBuffer) + ( (unsigned) ( uiBufferOffset - hMuxMCPB->stMuxSharedMemory.uiOffset ) ) );
   }

   return NULL;
}

static bool
BMUXlib_TS_MCPB_P_IsPESHeader(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   /* Detect PES Header */
   BDBG_ASSERT( pstTransportDescriptor );

   if ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
   {
      uint8_t *auiBuffer = (uint8_t *) BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPB, pstTransportDescriptor->uiBufferOffset );

      if ( auiBuffer == NULL ) return false;

      if ( ( auiBuffer[0] == 0x00 )
           && ( auiBuffer[1] == 0x00 )
           && ( auiBuffer[2] == 0x01 )
           )
      {
         return true;
      }
   }

   return false;
}

static bool
BMUXlib_TS_MCPB_P_IsBPP(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   /* Detect MTU BPP */
   BDBG_ASSERT( pstTransportDescriptor );
   BDBG_ASSERT( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptor->uiBufferOffset );
   {
      uint8_t *auiBuffer = (uint8_t *) BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPB, pstTransportDescriptor->uiBufferOffset );

      if ( auiBuffer == NULL ) return false;

      if ( ( auiBuffer[0] == 0x00 )
           && ( auiBuffer[1] == 0x00 )
           && ( auiBuffer[2] == 0x01 )
           && ( auiBuffer[10] == 'B' )
           && ( auiBuffer[11] == 'R' )
           && ( auiBuffer[12] == 'C' )
           && ( auiBuffer[13] == 'M' ) )
      {
         return true;
      }
   }

   return false;
}


static bool
BMUXlib_TS_MCPB_P_IsLastBPP(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   /* Detect LAST BPP */
   BDBG_ASSERT( pstTransportDescriptor );
   BDBG_ASSERT( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptor->uiBufferOffset );
   {
      uint8_t *auiBuffer = (uint8_t *) BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPB, pstTransportDescriptor->uiBufferOffset );

      if ( auiBuffer == NULL ) return false;

      if ( ( auiBuffer[0] == 0x00 )
           && ( auiBuffer[1] == 0x00 )
           && ( auiBuffer[2] == 0x01 )
           && ( auiBuffer[10] == 'B' )
           && ( auiBuffer[11] == 'R' )
           && ( auiBuffer[12] == 'C' )
           && ( auiBuffer[13] == 'M' )
           && ( auiBuffer[30] == 0x00 )
           && ( auiBuffer[31] == 0x00 )
           && ( auiBuffer[32] == 0x00 )
           && ( auiBuffer[33] == BMUXlib_TS_P_LAST_COMMAND ) )
      {
         return true;
      }
   }

   return false;
}

static bool
BMUXlib_TS_MCPB_P_IsNULLPacket(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   /* Detect NULL Packet*/
   BDBG_ASSERT( pstTransportDescriptor );
   BDBG_ASSERT( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptor->uiBufferOffset );
   {
      uint8_t *auiBuffer = (uint8_t *) BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPB, pstTransportDescriptor->uiBufferOffset );

      if ( auiBuffer == NULL ) return false;

      if ( ( BMUXlib_TS_P_TSPacket_MAXSIZE == pstTransportDescriptor->uiBufferLength )
           && ( auiBuffer[0] == 0x47 )
           && ( auiBuffer[1] == 0x1F )
           && ( auiBuffer[2] == 0xFF )
           && ( auiBuffer[3] == 0x00 ) )
      {
         return true;
      }
   }

   return false;
}

static bool
BMUXlib_TS_MCPB_P_IsBTP(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   /* Detect NULL Packet*/
   BDBG_ASSERT( pstTransportDescriptor );
   BDBG_ASSERT( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptor->uiBufferOffset );
   {
      uint8_t *auiBuffer = (uint8_t *) BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPB, pstTransportDescriptor->uiBufferOffset );

      if ( auiBuffer == NULL ) return false;

      if ( ( BMUXlib_TS_P_TSPacket_MAXSIZE == pstTransportDescriptor->uiBufferLength )
           && ( auiBuffer[0] == 0x47 )
           && ( auiBuffer[3] == 0x20 )
           && ( auiBuffer[4] == 0xb7 )
           && ( auiBuffer[5] == 0x82 )
           && ( auiBuffer[6] == 0x2d )
           && ( auiBuffer[7] == 0x00 )
           && ( auiBuffer[8] == 'B' )
           && ( auiBuffer[9] == 'R' )
           && ( auiBuffer[10] == 'C' )
           && ( auiBuffer[11] == 'M' ) )
      {
         return true;
      }
   }

   return false;
}

static bool BMUXlib_TS_MCPB_P_InsertBuffer(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXlib_TS_MCPB_P_TransportMetadata *pstMetadata,
   BMUXlib_TS_MCPB_QueueEntry *pstEntry /* If non-null, indicates this buffer is from an input source */
   )
{
   unsigned uiWriteOffsetTemp;

   BDBG_ASSERT( hMuxMCPB );
   BDBG_ASSERT( pstTransportDescriptor );
   BDBG_ASSERT( pstMetadata );

   /* Check if descriptor is available */
   uiWriteOffsetTemp = hMuxMCPB->stOutputQueue.uiWriteOffset + 1;
   uiWriteOffsetTemp %= hMuxMCPB->stOutputQueue.uiSize;

   if ( uiWriteOffsetTemp == hMuxMCPB->stOutputQueue.uiReadOffset )
   {
      return false;
   }

   /* See if we need to push the previous packet out due to a channel instance change */
   if ( ( true == hMuxMCPB->stOutputQueue.bLastChannelInstanceValid )
        && ( hMuxMCPB->stOutputQueue.uiLastChannelInstance != pstMetadata->uiChannelInstance ) )
   {
      /* Set PPPP flag */
      pstTransportDescriptor->stTsMuxDescriptorConfig.bPushPreviousPartialPacket = true;

      /* Reset bytes left in packet */
      hMuxMCPB->uiBytesLeftInPacket = hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize;
   }
   else if ( 0 == hMuxMCPB->uiBytesLeftInPacket )
   {
      /* Reset bytes left in packet */
      hMuxMCPB->uiBytesLeftInPacket = hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize;
   }

   hMuxMCPB->stOutputQueue.uiLastChannelInstance = pstMetadata->uiChannelInstance;
   hMuxMCPB->stOutputQueue.bLastChannelInstanceValid = true;

   if ( NULL != pstEntry )
   {
      /* Reduce buffer length to account for packet boundaries */
      if ( ( false == pstTransportDescriptor->stTsMuxDescriptorConfig.bPushPartialPacket )
           && ( pstTransportDescriptor->uiBufferLength > hMuxMCPB->uiBytesLeftInPacket )
           && ( ( pstTransportDescriptor->uiBufferLength % hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize ) != hMuxMCPB->uiBytesLeftInPacket ) )
      {
         if ( ( pstTransportDescriptor->uiBufferLength - hMuxMCPB->uiBytesLeftInPacket ) >= hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize )
         {
            pstTransportDescriptor->uiBufferLength = ( ( pstTransportDescriptor->uiBufferLength - hMuxMCPB->uiBytesLeftInPacket ) / hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize ) * hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize;
         }
         else
         {
            pstTransportDescriptor->uiBufferLength = 0;
         }
         pstTransportDescriptor->uiBufferLength += hMuxMCPB->uiBytesLeftInPacket;
      }

      /* Increment the source descriptor count if we're sending all the remaining bytes */
      if ( pstTransportDescriptor->uiBufferLength == pstEntry->uiBytesLeft )
      {
         pstMetadata->uiSourceDescriptorCount += pstEntry->uiSourceDescriptorCount;
      }
      BDBG_ASSERT( pstTransportDescriptor->uiBufferLength <= pstEntry->uiBytesLeft );
   }

   /* Include skipped descriptors in the source descriptor count */
   pstMetadata->uiSourceDescriptorCount += hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiSkippedDescriptorCount;
   hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiSkippedDescriptorCount = 0;

   /* Insert descriptor into output queue */
   *hMuxMCPB->stOutputQueue.astTransportDescriptors[hMuxMCPB->stOutputQueue.uiWriteOffset] = *pstTransportDescriptor;
   *hMuxMCPB->stOutputQueue.astMetadata[hMuxMCPB->stOutputQueue.uiWriteOffset] = *pstMetadata;

   hMuxMCPB->stOutputQueue.uiWriteOffset = uiWriteOffsetTemp;

   /* Update bytes left in packet */
   if ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bPushPartialPacket )
   {
      /* If the descriptor has PPP set, then packet will be padded as needed, so the bytes left can be set to 0 */
      hMuxMCPB->uiBytesLeftInPacket = 0;
   }
   else if ( false == pstTransportDescriptor->stTsMuxDescriptorConfig.bInsertHostDataAsBtp )
   {
      hMuxMCPB->uiBytesLeftInPacket -= pstTransportDescriptor->uiBufferLength % hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiPacketSize;
   }

   return true;
}

static void
BMUXlib_TS_MCPB_P_DumpDescriptor(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   const BMUXlib_TS_MCPB_QueueEntry *pstEntry,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   const BMUXlib_TS_MCPB_P_TransportMetadata *pstMetadata
   )
{
#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_DESC
   if ( NULL != hMuxMCPB->hDescDumpFile )
   {
      if ( false == hMuxMCPB->bHeaderWritten )
      {
         /* Write Header */
         fprintf( hMuxMCPB->hDescDumpFile, "Iter,Queued,Pending" );
         fprintf( hMuxMCPB->hDescDumpFile, ",S/N Start,S/N End,Ch I,currESCR I,Addr I,Size I,ESCR I,ESCR Valid I,PKT2PKT I,PKT2PKT Valid I,RAI I,Read I,Left I,Type I,descCount I" );
         fprintf( hMuxMCPB->hDescDumpFile, ",Addr O,Size O,ESCR O,ESCR Valid O,PKT2PKT O,PKT2PKT Valid O,RAI O,PID O,PID Valid O,HDI O,HDI BTP O,PPPP O,PPP O,Left O" );
         fprintf( hMuxMCPB->hDescDumpFile, "\n" );
         hMuxMCPB->bHeaderWritten = true;
      }


      {
         unsigned uiPending;
         unsigned uiQueued;

         if ( hMuxMCPB->stOutputQueue.uiPendingOffset >= hMuxMCPB->stOutputQueue.uiReadOffset )
         {
            uiPending = hMuxMCPB->stOutputQueue.uiPendingOffset - hMuxMCPB->stOutputQueue.uiReadOffset;
         }
         else
         {
            uiPending = hMuxMCPB->stOutputQueue.uiSize - hMuxMCPB->stOutputQueue.uiReadOffset;
            uiPending += hMuxMCPB->stOutputQueue.uiPendingOffset;
         }

         if ( hMuxMCPB->stOutputQueue.uiWriteOffset >= hMuxMCPB->stOutputQueue.uiPendingOffset )
         {
            uiQueued = hMuxMCPB->stOutputQueue.uiWriteOffset - hMuxMCPB->stOutputQueue.uiPendingOffset;
         }
         else
         {
            uiQueued = hMuxMCPB->stOutputQueue.uiSize - hMuxMCPB->stOutputQueue.uiPendingOffset;
            uiQueued += hMuxMCPB->stOutputQueue.uiWriteOffset;
         }

         fprintf( hMuxMCPB->hDescDumpFile, "%u,%u,%u",
            hMuxMCPB->uiDoMuxCount,
            uiPending,
            uiQueued
            );
      }

      fprintf( hMuxMCPB->hDescDumpFile, ",%u,%u,%u,%u,%p,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
         pstEntry->uiSerialNumber,
         pstEntry->uiSerialNumberEnd,
         pstEntry->uiChannelInstance,
         pstEntry->uiCurrentESCR,
         pstEntry->stTransportDescriptor.pBufferAddress,
         pstEntry->stTransportDescriptor.uiBufferLength,
         pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
         pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid,
         pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
         pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid,
         pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bRandomAccessIndication,
         pstEntry->uiBytesConsumed,
         pstEntry->uiBytesLeft,
         pstMetadata->eSource,
         pstMetadata->uiSourceDescriptorCount
         );

      fprintf( hMuxMCPB->hDescDumpFile, ",%p,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
         pstTransportDescriptor->pBufferAddress,
         pstTransportDescriptor->uiBufferLength,
         pstTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid,
         pstTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bRandomAccessIndication,
         pstTransportDescriptor->stTsMuxDescriptorConfig.uiPidChannelNo,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bPidChannelValid,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bHostDataInsertion,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bInsertHostDataAsBtp,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bPushPreviousPartialPacket,
         pstTransportDescriptor->stTsMuxDescriptorConfig.bPushPartialPacket,
         hMuxMCPB->uiBytesLeftInPacket
         );

      fprintf( hMuxMCPB->hDescDumpFile, "\n");
   }
#else
   BSTD_UNUSED( hMuxMCPB );
   BSTD_UNUSED( pstEntry );
   BSTD_UNUSED( pstTransportDescriptor );
   BSTD_UNUSED( pstMetadata );
#endif
}

static void
BMUXlib_TS_MCPB_P_ClearTransportDescriptorFlags (
   BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   pstTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = false;
   pstTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = false;
   pstTransportDescriptor->stTsMuxDescriptorConfig.bRandomAccessIndication = false;
}

static void
BMUXlib_TS_MCPB_P_ConsumeDescriptor(
   BMUXlib_TS_MCPB_Handle hMuxMCPB
   )
{
   BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntry = NULL;
   BMUXlib_TS_MCPB_QueueEntry *pstEntry = NULL;

   BMUXLIB_LIST_REMOVE( hMuxMCPB->stProcessEntryInfo.hList, &pstListEntry );
   BDBG_ASSERT(pstListEntry);
   pstEntry = BMUXLIB_LIST_ENTRY_DATA(pstListEntry);
   hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiQueuedCount-= pstEntry->uiSourceDescriptorCount;
   hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPendingCount += pstEntry->uiSourceDescriptorCount;
   BKNI_Free(pstListEntry);

   {
      size_t uiNumEntries;
      BMUXLIB_LIST_GETNUMENTRIES( hMuxMCPB->stProcessEntryInfo.hList, &uiNumEntries );
      BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_QUEUE, ("[%p] [%d] baseESCR = %08x, currentESCR = %08x (%5d bytes left) - Done (%3d entries)", (void *)hMuxMCPB->stProcessEntryInfo.hList, pstEntry->uiChannelInstance, pstEntry->uiBaseESCR, pstEntry->uiCurrentESCR, pstEntry->uiBytesLeft, (int)uiNumEntries ));
   }
}

static bool
BMUXlib_TS_MCPB_P_InsertBuffer_PESHeaderExtended(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   BMUXlib_TS_MCPB_QueueEntry *pstEntry
   )
{
   bool bResult = false;
   unsigned uiIndex = 0;
   unsigned uiBufferLength;

   BMUXlib_TS_TransportDescriptor stTransportDescriptor;
   BMUXlib_TS_MCPB_P_TransportMetadata stMetadata;
   void *pBufferAddress;

   /* Insert RAI Prefix */
   BDBG_ASSERT( hMuxMCPB );

   BKNI_Memset( &stTransportDescriptor, 0, sizeof( stTransportDescriptor ) );
   BKNI_Memset( &stMetadata, 0, sizeof( stMetadata ) );

   stTransportDescriptor = pstEntry->stTransportDescriptor;

   stTransportDescriptor.uiBufferOffset = hMuxMCPB->pstBuffer->uiRAIExtendedOffset[hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset];
   pBufferAddress = hMuxMCPB->pstBuffer->pRAIExtended[hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset];

   /* TODO: Only stuff the PES header if the next ES frame is less than 184-19 bytes */
   /* TODO: Add optional PCR insertion on video PID */
   /* Always stuff the PES header's adaptation field so we don't need to worry about ES frames smaller than a packet length.
    * This will cause the TS packet to only contain the PES header w/o any ES. The ES data will follow in a separate TS packet */
   stTransportDescriptor.uiBufferLength = 184;

   /* Set the adaptation length */
   ((uint8_t*)pBufferAddress)[uiIndex++] = stTransportDescriptor.uiBufferLength - pstEntry->stTransportDescriptor.uiBufferLength - 1;

   /* Set the RAI */
   if ( true == pstEntry->bRandomAccessIndication )
   {
      ((uint8_t*)pBufferAddress)[uiIndex++] = 0x40;
   }
   else
   {
      ((uint8_t*)pBufferAddress)[uiIndex++] = 0x00;
   }

   /* Add the Adaption Field Stuffing Bytes */
   while ( uiIndex < ( stTransportDescriptor.uiBufferLength - pstEntry->stTransportDescriptor.uiBufferLength ) )
   {
      ((uint8_t*)pBufferAddress)[uiIndex++] = 0xFF;
   }

   BKNI_Memcpy(&((uint8_t*)pBufferAddress)[uiIndex], BMUXlib_TS_MCPB_P_GetAddress(hMuxMCPB, pstEntry->stTransportDescriptor.uiBufferOffset), pstEntry->stTransportDescriptor.uiBufferLength );
   uiIndex += pstEntry->stTransportDescriptor.uiBufferLength;

   BDBG_ASSERT( uiIndex == stTransportDescriptor.uiBufferLength );
   stMetadata.eSource = BMUXlib_TS_MCPB_P_SourceType_ePESHeaderExtended;
   stMetadata.uiChannelInstance = pstEntry->uiChannelInstance;
   stMetadata.uiSourceDescriptorCount = pstEntry->uiSourceDescriptorCount;

   uiBufferLength = stTransportDescriptor.uiBufferLength;
   BMMA_FlushCache( hMuxMCPB->pstBuffer->hRAIExtendedBlock[hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset], pBufferAddress, uiBufferLength );

   bResult = BMUXlib_TS_MCPB_P_InsertBuffer( hMuxMCPB, &stTransportDescriptor, &stMetadata, NULL );

   if ( true == bResult )
   {
      /* Since each PES header is preceded with a PUSI BTP, we expect the bytes inserted to be the same as what we sent */
      BDBG_ASSERT( uiBufferLength == stTransportDescriptor.uiBufferLength );
      BMUXlib_TS_MCPB_P_DumpDescriptor( hMuxMCPB, pstEntry, &stTransportDescriptor, &stMetadata );
      BMUXlib_TS_MCPB_P_ClearTransportDescriptorFlags( &pstEntry->stTransportDescriptor );
      pstEntry->uiBytesLeft = 0;
      BMUXlib_TS_MCPB_P_ConsumeDescriptor( hMuxMCPB );

      hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset++;
      hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset %= BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT;
      if ( hMuxMCPB->pstBuffer->uiRAIExtendedWriteOffset == hMuxMCPB->pstBuffer->uiRAIExtendedReadOffset )
      {
         BDBG_ERR(("RAI Extended Overflow"));
      }
   }

   return bResult;
}

static bool
BMUXlib_TS_MCPB_P_InsertBuffer_PUSIBtp(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   BMUXlib_TS_MCPB_QueueEntry *pstEntry
   )
{
   bool bResult = false;
   unsigned uiBufferLength;

   BMUXlib_TS_TransportDescriptor stTransportDescriptor;
   BMUXlib_TS_MCPB_P_TransportMetadata stMetadata;

   /* Insert PUSI BTP */
   BDBG_ASSERT( hMuxMCPB );

   BKNI_Memset( &stTransportDescriptor, 0, sizeof( stTransportDescriptor ) );
   BKNI_Memset( &stMetadata, 0, sizeof( stMetadata ) );

   stTransportDescriptor = pstEntry->stTransportDescriptor;

   if ( true == pstEntry->bRandomAccessIndication )
   {
      stTransportDescriptor.uiBufferOffset = hMuxMCPB->pstBuffer->uiPUSIBtpOffset[0];
   }
   else
   {
      stTransportDescriptor.uiBufferOffset = hMuxMCPB->pstBuffer->uiPUSIBtpOffset[1];
   }
   stTransportDescriptor.uiBufferLength = hMuxMCPB->pstBuffer->uiPUSIBtpSize;

   stTransportDescriptor.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
   stTransportDescriptor.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = 0;

   stTransportDescriptor.stTsMuxDescriptorConfig.bHostDataInsertion = true;
   stTransportDescriptor.stTsMuxDescriptorConfig.bPushPreviousPartialPacket = true;

   /* Reset bytes left in packet */
   hMuxMCPB->uiBytesLeftInPacket = hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize;

   stTransportDescriptor.stTsMuxDescriptorConfig.bInsertHostDataAsBtp = true;

   stMetadata.eSource = BMUXlib_TS_MCPB_P_SourceType_eBTP;
   stMetadata.uiChannelInstance = pstEntry->uiChannelInstance;

   uiBufferLength = stTransportDescriptor.uiBufferLength;
   bResult = BMUXlib_TS_MCPB_P_InsertBuffer( hMuxMCPB, &stTransportDescriptor, &stMetadata, NULL );

   if ( true == bResult )
   {
      /* Since each BTP has PPPP set, we expect the bytes inserted to be the same as what we sent */
      BDBG_ASSERT( uiBufferLength == stTransportDescriptor.uiBufferLength );
      BMUXlib_TS_MCPB_P_DumpDescriptor( hMuxMCPB, pstEntry, &stTransportDescriptor, &stMetadata );
      BMUXlib_TS_MCPB_P_ClearTransportDescriptorFlags( &pstEntry->stTransportDescriptor );
   }

   return bResult;
}

static bool
BMUXlib_TS_MCPB_P_InsertPayload(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   BMUXlib_TS_MCPB_QueueEntry *pstEntry,
   BMUXlib_TS_TransportDescriptor *pstTransportDescriptor
   )
{
   BMUXlib_TS_MCPB_P_TransportMetadata stMetadata;

   /* Set Metadata */
   BKNI_Memset( &stMetadata, 0, sizeof( stMetadata ) );
   stMetadata.eSource = pstEntry->eSource;
   stMetadata.uiChannelInstance = pstEntry->uiChannelInstance;

   if ( true == BMUXlib_TS_MCPB_P_InsertBuffer( hMuxMCPB, pstTransportDescriptor, &stMetadata, pstEntry ) )
   {
      BMUXlib_TS_MCPB_P_DumpDescriptor( hMuxMCPB, pstEntry, pstTransportDescriptor, &stMetadata );
      /* Update the state */
      BMUXlib_TS_MCPB_P_ClearTransportDescriptorFlags( &pstEntry->stTransportDescriptor );

      /* Update the bytes left/consumed */
      pstEntry->uiBytesConsumed += pstTransportDescriptor->uiBufferLength;
      pstEntry->uiBytesLeft -= pstTransportDescriptor->uiBufferLength;
      hMuxMCPB->uiProcessedBytes += pstTransportDescriptor->uiBufferLength;

      /* Calculate the current ESCR */
      pstEntry->uiCurrentESCR = pstEntry->uiBaseESCR;
      pstEntry->uiCurrentESCR += ( pstEntry->uiBytesConsumed / hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize ) * pstEntry->uiPacket2PacketDelta;
      pstEntry->uiCurrentESCR += ( ( pstEntry->uiBytesConsumed % hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize ) * pstEntry->uiPacket2PacketDelta ) / hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize;

      /* Remove the descriptor from the FIFO if there are no more bytes left */
      if ( 0 == pstEntry->uiBytesLeft )
      {
         BMUXlib_TS_MCPB_P_ConsumeDescriptor( hMuxMCPB );
      }

      return true;
   }

   return false;
}

static void
BMUXlib_TS_MCPB_P_ReconcileFifoQueue(
   BMUXlib_TS_MCPB_Handle hMuxMCPB
   )
{
   {
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntry = NULL;
      BMUXlib_TS_MCPB_QueueEntry *pstEntry = NULL;

      if ( false == BMUXLIB_LIST_ISEMPTY(hMuxMCPB->astListQueue[0]) )
      {
         BMUXLIB_LIST_FIRST( hMuxMCPB->astListQueue[0], &pstListEntry );
         pstEntry = BMUXLIB_LIST_ENTRY_DATA(pstListEntry);
      }

      if ( ( NULL == pstEntry ) /* Empty List */
           || ( false == hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stSettings.bIsTS ) /* Not TS Input */
           || ( 0 == ( hMuxMCPB->uiProcessedBytes % hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize ) ) /* Processed multiple of packet size bytes */
         )
      {
         /* Do a bubble-sort of the list (the list is less than 10 elements, so should be relatively fast) */
         unsigned i,j;
         for ( i = 0; i < (hMuxMCPB->stSettings.uiMaxNumInputs-1); i++ )
         {
            for ( j = 0; j < (hMuxMCPB->stSettings.uiMaxNumInputs-i-1); j++ )
            {
               if ( BMUXlib_TS_MCPB_P_CompareDescriptorFifo(hMuxMCPB->astListQueue[j], hMuxMCPB->astListQueue[j+1]) > 0 )
               {
                  BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry) *hTempList;

                  hTempList = hMuxMCPB->astListQueue[j+1];
                  hMuxMCPB->astListQueue[j+1] = hMuxMCPB->astListQueue[j];
                  hMuxMCPB->astListQueue[j] = hTempList;
               }
            }
         }
         hMuxMCPB->uiProcessedBytes = 0;
      }
   }
}

static bool
BMUXlib_TS_MCPB_P_ProcessNextEntry(
   BMUXlib_TS_MCPB_Handle hMuxMCPB,
   unsigned uiEndESCR
   )
{
   bool bResult = true;
   BMUXlib_TS_MCPB_P_ProcessNextEntry_State ePreviousState;
   BDBG_ENTER( BMUXlib_TS_MCPB_P_ProcessNextEntry );

   BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("*** Enter ***") );
   do
   {
      ePreviousState = hMuxMCPB->stProcessEntryInfo.eState;

      switch( hMuxMCPB->stProcessEntryInfo.eState )
      {
         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eGetNextDescriptor:
         {
            BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry) *hList = hMuxMCPB->astListQueue[0];
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eGetNextDescriptor") );
            BKNI_Memset( &hMuxMCPB->stProcessEntryInfo, 0, sizeof( hMuxMCPB->stProcessEntryInfo ) );

            /* Queue all descriptors up until ESCR >= endPacingCounter value */
            if ( false == BMUXLIB_LIST_ISEMPTY( hList ) )
            {
               BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstListEntry;
               BMUXlib_TS_MCPB_QueueEntry *pstEntry;
               size_t uiNumEntries;
               signed iESCRDelta;

               BMUXLIB_LIST_GETNUMENTRIES( hList, &uiNumEntries );

               BMUXLIB_LIST_FIRST( hList, &pstListEntry );
               pstEntry = BMUXLIB_LIST_ENTRY_DATA(pstListEntry);

               iESCRDelta = uiEndESCR - pstEntry->uiCurrentESCR;

               if ( ( iESCRDelta < 0 )
                    || ( ( 0 == iESCRDelta ) && ( 0 != pstEntry->uiBytesLeft ) && ( 0 != pstEntry->uiPacket2PacketDelta ) ) )
               {
                  /* We need to wait before processing this descriptor */
                  bResult = false;
                  break;
               }

               hMuxMCPB->stProcessEntryInfo.hList = hList;
               hMuxMCPB->stProcessEntryInfo.pstEntry = pstEntry;

               BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_QUEUE, ("[%p] [%d] baseESCR = %08x, currentESCR = %08x (%5d bytes left) - Next (%3d entries)", (void *)hMuxMCPB->stProcessEntryInfo.hList, pstEntry->uiChannelInstance, pstEntry->uiBaseESCR, pstEntry->uiCurrentESCR, pstEntry->uiBytesLeft, (int)uiNumEntries ));

               if ( false == hMuxMCPB->astChannel[hMuxMCPB->stProcessEntryInfo.pstEntry->uiChannelInstance].stSettings.bIsTS )
               {
                  hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESDescriptor;
               }
               else
               {
                  hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessTSDescriptor;
               }
            }
            else
            {
               BDBG_WRN(("no fifos available"));
            }
         }
         break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESDescriptor:
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessPESDescriptor") );
            if ( true == BMUXlib_TS_MCPB_P_IsPESHeader( hMuxMCPB, &hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor ) )
            {
               if ( true == BMUXlib_TS_MCPB_P_IsLastBPP( hMuxMCPB, &hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor ) )
               {
                  /* Handle LAST BPP */
                  hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessLastBPP;
               }
               else if ( true == BMUXlib_TS_MCPB_P_IsBPP( hMuxMCPB, &hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor ) )
               {
                  /* Handle BPP */
                  hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor;
               }
               else
               {
                  if ( ( true == hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid )
                            && ( ( 0 == hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta ) ) )
                  {
                     /* Handle Unknown PES */
                     hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor;
                  }
                  else
                  {
                     /* Handle PES Header */
                     hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeader;
                  }
               }
            }
            else
            {
               /* Handle CDB Data */
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPayload;
            }
            break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessLastBPP:
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessLastBPP") );
            /* Deactivate this input */
            hMuxMCPB->astChannel[hMuxMCPB->stProcessEntryInfo.pstEntry->uiChannelInstance].stState.bActive = false;

            hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor;
            break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor:
         {
            /* Skip Descriptor */
            BMUXlib_TS_MCPB_QueueEntry *pstEntry = hMuxMCPB->stProcessEntryInfo.pstEntry;
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eSkipDescriptor") );

            hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiSkippedDescriptorCount++;
            pstEntry->uiBytesLeft = 0;
            BMUXlib_TS_MCPB_P_ConsumeDescriptor( hMuxMCPB );
            hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor;
         }
            break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeader:
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessPESHeader") );
            /* Insert PUSI BTP */
            if ( true == BMUXlib_TS_MCPB_P_InsertBuffer_PUSIBtp( hMuxMCPB, hMuxMCPB->stProcessEntryInfo.pstEntry ) )
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeaderExtended;
            }
            break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeaderExtended:
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessPESHeaderExtended") );
            if ( true == BMUXlib_TS_MCPB_P_InsertBuffer_PESHeaderExtended( hMuxMCPB, hMuxMCPB->stProcessEntryInfo.pstEntry ) )
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor;
            }
            break;

#if 0 /* Unused */
         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPESHeaderPayload:
         {
            BMUXlib_TS_MCPB_QueueEntry *pstEntry = hMuxMCPB->stProcessEntryInfo.pstEntry;
            BMUXlib_TS_TransportDescriptor stTransportDescriptor;

            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessPESHeaderPayload") );

            BKNI_Memset( &stTransportDescriptor, 0, sizeof( stTransportDescriptor ) );

            stTransportDescriptor = pstEntry->stTransportDescriptor;

            stTransportDescriptor.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
            stTransportDescriptor.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = pstEntry->uiPacket2PacketDelta;

            if ( true == BMUXlib_TS_MCPB_P_InsertPayload( hMuxMCPB, pstEntry, &stTransportDescriptor ) )
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor;
            }
         }
            break;
#endif
         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessTSDescriptor:
         {
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessTSDescriptor") );

            if ( true == BMUXlib_TS_MCPB_P_IsNULLPacket( hMuxMCPB, &hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor ) )
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor;
            }
            else if ( true == BMUXlib_TS_MCPB_P_IsBTP( hMuxMCPB, &hMuxMCPB->stProcessEntryInfo.pstEntry->stTransportDescriptor ) )
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eSkipDescriptor;
            }
            else
            {
               hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPayload;
            }
         }
            break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eProcessPayload:
         {
            BMUXlib_TS_MCPB_QueueEntry *pstEntry = hMuxMCPB->stProcessEntryInfo.pstEntry;
            BMUXLIB_LIST_TYPE(BMUXlib_TS_MCPB_QueueEntry)* hNextList = hMuxMCPB->astListQueue[1];

            {
               BMUXlib_TS_TransportDescriptor stTransportDescriptor;

               BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eProcessPayload") );

               BKNI_Memset( &stTransportDescriptor, 0, sizeof( stTransportDescriptor ) );

               stTransportDescriptor = pstEntry->stTransportDescriptor;

               stTransportDescriptor.uiBufferOffset = pstEntry->stTransportDescriptor.uiBufferOffset + pstEntry->uiBytesConsumed;

               /* Calculate numBytes to send */
               /* Peek at next descriptor */
               if ( false == BMUXLIB_LIST_ISEMPTY( hNextList ) )
               {
                  BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *pstNextListEntry;
                  BMUXlib_TS_MCPB_QueueEntry *pstNextEntry;
                  unsigned uiNextESCR;
                  signed iESCRDelta;

                  BMUXLIB_LIST_FIRST( hNextList, &pstNextListEntry );
                  pstNextEntry = BMUXLIB_LIST_ENTRY_DATA(pstNextListEntry);
                  uiNextESCR = pstNextEntry->uiCurrentESCR;
                  iESCRDelta = uiEndESCR - uiNextESCR;

                  if ( iESCRDelta < 0 )
                  {
                     uiNextESCR = uiEndESCR;
                  }

                  /* Calculate the number of bytes to send such that we don't overlap with the start of the next entry */
                  if ( 0 != pstEntry->uiPacket2PacketDelta )
                  {
                     stTransportDescriptor.uiBufferLength = ( ( ( uiNextESCR - pstEntry->uiCurrentESCR ) / pstEntry->uiPacket2PacketDelta ) + 1 ) * hMuxMCPB->astChannel[pstEntry->uiChannelInstance].stState.uiPacketSize;

                     /* Don't send more than the descriptor actually contains */
                     if ( stTransportDescriptor.uiBufferLength > pstEntry->uiBytesLeft )
                     {
                        stTransportDescriptor.uiBufferLength = pstEntry->uiBytesLeft;
                     }
                  }
                  else
                  {
                     stTransportDescriptor.uiBufferLength = pstEntry->uiBytesLeft;
                  }
               }
               else
               {
                  /* There's isn't a next descriptor, so send the remaining bytes in this descriptor and push it out */
                  stTransportDescriptor.uiBufferLength = pstEntry->uiBytesLeft;
                  stTransportDescriptor.stTsMuxDescriptorConfig.bPushPartialPacket = true;
               }

               stTransportDescriptor.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
               stTransportDescriptor.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = pstEntry->uiPacket2PacketDelta;

               if ( true == BMUXlib_TS_MCPB_P_InsertPayload( hMuxMCPB, pstEntry, &stTransportDescriptor ) )
               {
                  hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor;
               }
            }
         }
         break;

         case BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eDoneWithDescriptor:
         {
            BMUXlib_TS_MCPB_QueueEntry *pstEntry = hMuxMCPB->stProcessEntryInfo.pstEntry;
            size_t uiNumEntries;
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("eDoneWithDescriptor") );
            BMUXLIB_LIST_GETNUMENTRIES( hMuxMCPB->stProcessEntryInfo.hList, &uiNumEntries );
            if ( 0 != pstEntry->uiBytesLeft )
            {
               BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_QUEUE, ("[%p] [%d] baseESCR = %08x, currentESCR = %08x (%5d bytes left) - Re-Q (%3d entries)", (void *)hMuxMCPB->stProcessEntryInfo.hList, pstEntry->uiChannelInstance, pstEntry->uiBaseESCR, pstEntry->uiCurrentESCR, pstEntry->uiBytesLeft, (int)uiNumEntries ));
            }
            BMUXlib_TS_MCPB_P_ReconcileFifoQueue( hMuxMCPB );
            hMuxMCPB->stProcessEntryInfo.eState = BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eGetNextDescriptor;
         }
         break;
      }
   } while ( ( BMUXlib_TS_MCPB_P_ProcessNextEntry_State_eGetNextDescriptor != hMuxMCPB->stProcessEntryInfo.eState ) && ( ePreviousState != hMuxMCPB->stProcessEntryInfo.eState ) );

   BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_STATE, ("--- Leave ---") );

   BDBG_LEAVE( BMUXlib_TS_MCPB_P_ProcessNextEntry );

   return bResult;
}

static BERR_Code
BMUXlib_TS_MCPB_P_ScheduleProcessedEntries(
   BMUXlib_TS_MCPB_Handle hMuxMCPB
   )
{
   BMUXlib_TS_TransportChannelInterface *pTransportInterface = NULL;

   BDBG_ENTER( BMUXlib_TS_MCPB_P_ScheduleProcessedEntries );

   BDBG_ASSERT( hMuxMCPB );

   pTransportInterface = &hMuxMCPB->stSettings.stOutputChannelInterface;

   while ( hMuxMCPB->stOutputQueue.uiPendingOffset != hMuxMCPB->stOutputQueue.uiWriteOffset )
   {
      size_t uiNumTransportDescriptors = 1;
      size_t uiNumTransportDescriptorsQueued;

      pTransportInterface->fAddTransportDescriptors(
         pTransportInterface->pContext,
         hMuxMCPB->stOutputQueue.astTransportDescriptors[hMuxMCPB->stOutputQueue.uiPendingOffset],
         uiNumTransportDescriptors,
         &uiNumTransportDescriptorsQueued
         );

#if BMUXLIB_TS_MCPB_P_DUMP_TRANSPORT_OUTPUT
      if ( NULL != hMuxMCPB->hOutputDumpFile )
      {
         unsigned uiCount = uiNumTransportDescriptorsQueued;
         unsigned i = hMuxMCPB->stOutputQueue.uiPendingOffset;

         while ( 0 != uiCount )
         {
            fwrite( hMuxMCPB->stOutputQueue.astTransportDescriptors[i].pBufferAddress, 1, hMuxMCPB->stOutputQueue.astTransportDescriptors[i].uiBufferLength, hMuxMCPB->hOutputDumpFile );
            i++;
            i %= hMuxMCPB->stOutputQueue.uiSize;
            uiCount--;
         }
      }
#endif

      hMuxMCPB->stOutputQueue.uiPendingOffset += uiNumTransportDescriptorsQueued;
      hMuxMCPB->stOutputQueue.uiPendingOffset %= hMuxMCPB->stOutputQueue.uiSize;

      if ( uiNumTransportDescriptorsQueued != uiNumTransportDescriptors ) break;
   }

   BDBG_LEAVE( BMUXlib_TS_MCPB_P_ScheduleProcessedEntries );

   return BERR_TRACE( BERR_SUCCESS );
}

static unsigned BMUXLIB_TS_P_FIFO_FREE(
   BMUXlib_TS_MCPB_P_PacketFifo *pstFifo
   )
{
   BDBG_ASSERT( pstFifo );

   if ( pstFifo->uiWrite >= pstFifo->uiPacketRead )
   {
      return pstFifo->uiRead + ( pstFifo->uiSize - pstFifo->uiWrite );
   }
   else
   {
      return pstFifo->uiRead - pstFifo->uiWrite;
   }
}

static void BMUXLIB_TS_P_FIFO_ADD(
   BMUXlib_TS_MCPB_P_PacketFifo *pstFifo,
   const void *pBuffer,
   size_t uiLength
   )
{
   unsigned uiBytesCopied = 0;
   unsigned uiBytesLeft = uiLength;

   BDBG_ASSERT( BMUXLIB_TS_P_FIFO_FREE( pstFifo ) >= uiLength );

   while ( 0 != uiBytesLeft )
   {
      unsigned uiContiguousBytesAvailable = 0;
      unsigned uiBytesToCopy = 0;
      if ( pstFifo->uiWrite >= pstFifo->uiRead )
      {
         uiContiguousBytesAvailable = pstFifo->uiSize - pstFifo->uiWrite;
      }
      else
      {
         uiContiguousBytesAvailable = pstFifo->uiRead - pstFifo->uiWrite;
      }

      if ( uiContiguousBytesAvailable >= uiBytesLeft )
      {
         uiBytesToCopy = uiBytesLeft;
      }
      else
      {
         uiBytesToCopy = uiContiguousBytesAvailable;
      }

      BDBG_ASSERT( uiBytesToCopy <= uiBytesLeft );

      BKNI_Memcpy( (void*) ( ( (uint8_t *) pstFifo->pBuffer ) + pstFifo->uiWrite ), (void*) ( ( (uint8_t *) pBuffer ) + uiBytesCopied ), uiBytesToCopy );

      uiBytesLeft -= uiBytesToCopy;
      uiBytesCopied += uiBytesToCopy;
      pstFifo->uiWrite += uiBytesToCopy;
      pstFifo->uiWrite %= pstFifo->uiSize;
   }
}

static void BMUXLIB_TS_P_FIFO_CONSUME(
   BMUXlib_TS_MCPB_P_PacketFifo *pstFifo,
   size_t uiLength
   )
{
   BDBG_ASSERT( pstFifo );
   pstFifo->uiRead += uiLength;
   pstFifo->uiRead %= pstFifo->uiSize;
}

static void BMUXLIB_TS_P_FIFO_PKTCONSUME(
   BMUXlib_TS_MCPB_P_PacketFifo *pstFifo,
   size_t uiLength
   )
{
   BDBG_ASSERT( pstFifo );
   pstFifo->uiPacketRead += uiLength;
   pstFifo->uiPacketRead %= pstFifo->uiSize;
}

static unsigned BMUXLIB_TS_P_FIFO_PKTDEPTH(
   BMUXlib_TS_MCPB_P_PacketFifo *pstFifo
   )
{
   BDBG_ASSERT( pstFifo );

   if ( pstFifo->uiWrite >= pstFifo->uiPacketRead )
   {
      return pstFifo->uiWrite - pstFifo->uiPacketRead;
   }
   else
   {
      return pstFifo->uiWrite + ( pstFifo->uiSize - pstFifo->uiPacketRead );
   }
}

static void
BMUXlib_TS_MCPB_P_ProcessCompletedTransportDescriptors(
   BMUXlib_TS_MCPB_Handle hMuxMCPB
   )
{
   size_t uiCompleted;
   BMUXlib_TS_TransportChannelInterface *pTransportInterface = NULL;

   BDBG_ENTER( BMUXlib_TS_MCPB_P_ProcessCompletedTransportDescriptors );

   BDBG_ASSERT( hMuxMCPB );

   pTransportInterface = &hMuxMCPB->stSettings.stOutputChannelInterface;

   if ( ( NULL != pTransportInterface->pContext )
        && ( NULL != pTransportInterface->fGetCompletedTransportDescriptors ) )
   {
      /* Get the completed descriptors from the output transport channel */
      pTransportInterface->fGetCompletedTransportDescriptors(
               pTransportInterface->pContext,
               &uiCompleted
               );

      /* Release the resources for the completed */
      while ( 0 != uiCompleted )
      {
         BMUXlib_TS_MCPB_P_TransportMetadata *pstMetadata = NULL;
         BMUXlib_TS_TransportDescriptor *pstTransportDescriptor = NULL;

         pstMetadata = hMuxMCPB->stOutputQueue.astMetadata[hMuxMCPB->stOutputQueue.uiReadOffset];
         pstTransportDescriptor = hMuxMCPB->stOutputQueue.astTransportDescriptors[hMuxMCPB->stOutputQueue.uiReadOffset];

         /* Reclaim any internal buffers */
         switch ( pstMetadata->eSource )
         {
            case BMUXlib_TS_MCPB_P_SourceType_eInput:
               hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiCompletedCount += pstMetadata->uiSourceDescriptorCount;
               break;

            case BMUXlib_TS_MCPB_P_SourceType_eFIFO:
               hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiCompletedCount += pstMetadata->uiSourceDescriptorCount;
               BDBG_ASSERT( 0 != hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo.uiSize );
               BDBG_ASSERT( 0 == ( pstTransportDescriptor->uiBufferLength % 188 ) );
               BMUXLIB_TS_P_FIFO_CONSUME( &hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo, pstTransportDescriptor->uiBufferLength );
               BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] Finished descriptor "BDBG_UINT64_FMT" %d bytes %d source descriptors",
                  hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo.uiRead,
                  hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo.uiPacketRead,
                  hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo.uiWrite,
                  (int)hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.stTSPacketInfo.stFifo.uiSize,
                  BDBG_UINT64_ARG(pstTransportDescriptor->uiBufferOffset),
                  (int)pstTransportDescriptor->uiBufferLength,
                  pstMetadata->uiSourceDescriptorCount
               ));
               break;

            case BMUXlib_TS_MCPB_P_SourceType_eBTP:
               /* Do Nothing */
               break;

            case BMUXlib_TS_MCPB_P_SourceType_ePESHeaderExtended:
               hMuxMCPB->astChannel[pstMetadata->uiChannelInstance].stState.uiCompletedCount += pstMetadata->uiSourceDescriptorCount;
               hMuxMCPB->pstBuffer->uiRAIExtendedReadOffset++;
               hMuxMCPB->pstBuffer->uiRAIExtendedReadOffset %= BMUXLIB_TS_MCPB_P_RAI_EXTENDED_COUNT;
               break;

            default:
               /* Unknown Source Type */
               break;
         }

         /* Reclaim transport descriptor and metadata */
         hMuxMCPB->stOutputQueue.uiReadOffset++;
         hMuxMCPB->stOutputQueue.uiReadOffset %= hMuxMCPB->stOutputQueue.uiSize;
         uiCompleted--;
      }
   }

   BDBG_LEAVE( BMUXlib_TS_MCPB_P_ProcessCompletedTransportDescriptors );
}

/**********************/
/* Run-time Functions */
/**********************/
static void
BMUXlib_TS_MCPB_P_PopulateEntryFromDescriptor(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXlib_TS_MCPB_QueueEntry *pstEntry
   )
{
   BDBG_ASSERT( pstEntry );
   BKNI_Memset( pstEntry, 0, sizeof( *pstEntry ) );

   /* Set the current ESCR value */
   if ( ( NULL != pstTransportDescriptor )
        && ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid ) )
   {
      pstEntry->uiBaseESCR = pstTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
   }
   else
   {
      pstEntry->uiBaseESCR = hMuxMCPBCh->stState.uiCurrentESCR;
   }

   {
      signed iDelta;

      iDelta = pstEntry->uiBaseESCR - hMuxMCPBCh->stState.uiCurrentESCR;
      if ( iDelta < 0 )
      {
         BDBG_WRN(("Warning: The actual ESCR (%08x) is less than the computed ESCR (%08x)", pstEntry->uiBaseESCR, hMuxMCPBCh->stState.uiCurrentESCR ));
      }
   }

   /* Set the current PKT2PKT delta */
   if ( ( NULL != pstTransportDescriptor )
        && ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid ) )
   {
      pstEntry->uiPacket2PacketDelta = pstTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta;
   }
   else
   {
      pstEntry->uiPacket2PacketDelta = hMuxMCPBCh->stState.uiCurrentPKT2PKTDelta;
   }

   /* Set the current ESCR for this entry equal to the base ESCR */
   pstEntry->uiCurrentESCR = pstEntry->uiBaseESCR;

   /* Set the channel index */
   pstEntry->uiChannelInstance = hMuxMCPBCh->stSettings.uiInstance;

   /* Copy the transport descriptor */
   if ( NULL != pstTransportDescriptor )
   {
      pstEntry->stTransportDescriptor = *pstTransportDescriptor;
      pstEntry->bRandomAccessIndication = pstTransportDescriptor->stTsMuxDescriptorConfig.bRandomAccessIndication;
      /* Set the number of bytes left */
      pstEntry->uiBytesLeft = pstEntry->stTransportDescriptor.uiBufferLength;
   }

   /* Set the PID */
   pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.uiPidChannelNo = hMuxMCPBCh->stSettings.uiPIDChannelIndex;
   pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bPidChannelValid = true;

   /* Set the Host Data Insertion Flag */
   pstEntry->stTransportDescriptor.stTsMuxDescriptorConfig.bHostDataInsertion = hMuxMCPBCh->stSettings.bIsTS;

   if ( NULL != pstTransportDescriptor )
   {
      /* DEBUG: Set descriptor serial number */
      pstEntry->uiSerialNumber = hMuxMCPBCh->hMuxMCPB->uiSerialNumber++;
      pstEntry->uiSerialNumberEnd = pstEntry->uiSerialNumber;
   }
}

static void BMUXlib_TS_MCPB_P_UpdateCurrentESCR(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_MCPB_QueueEntry *pstEntry
   )
{

   /* Update channel state */
   hMuxMCPBCh->stState.uiCurrentESCR = pstEntry->uiBaseESCR;
   hMuxMCPBCh->stState.uiCurrentPKT2PKTDelta = pstEntry->uiPacket2PacketDelta;

   /* Calculate the next ESCR by using the currentESCR, currentPKT2PKTDelta, and buffer size */
   hMuxMCPBCh->stState.uiCurrentESCR += ( pstEntry->stTransportDescriptor.uiBufferLength / hMuxMCPBCh->stState.uiPacketSize ) * hMuxMCPBCh->stState.uiCurrentPKT2PKTDelta; /* Number of Packets */
   hMuxMCPBCh->stState.uiCurrentESCR += ( ( pstEntry->stTransportDescriptor.uiBufferLength % hMuxMCPBCh->stState.uiPacketSize ) * hMuxMCPBCh->stState.uiCurrentPKT2PKTDelta ) / hMuxMCPBCh->stState.uiPacketSize; /* Include fractional Packet */
}

static bool
BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsDirect(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) ** pstListEntry
   )
{
   BMUXlib_TS_MCPB_QueueEntry *pstEntry = NULL;
   BDBG_ASSERT(pstListEntry);

   *pstListEntry = (BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)*) BMUXlib_Malloc(sizeof(BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)));

   if (!*pstListEntry)
   {
      BDBG_WRN(("descriptor free list is empty"));
      return false;
   }
   BKNI_Memset(*pstListEntry, 0, sizeof(BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)));
   pstEntry =  BMUXLIB_LIST_ENTRY_DATA(*pstListEntry);
   BKNI_Memset( pstEntry, 0, sizeof( BMUXlib_TS_MCPB_QueueEntry ) );

   BMUXlib_TS_MCPB_P_PopulateEntryFromDescriptor( hMuxMCPBCh, pstTransportDescriptor, pstEntry );

   pstEntry->uiSourceDescriptorCount = 1;
   pstEntry->eSource = BMUXlib_TS_MCPB_P_SourceType_eInput;

   BMUXlib_TS_MCPB_P_UpdateCurrentESCR( hMuxMCPBCh, pstEntry );

   return true;
}

static bool
BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsViaFifo(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *apstEntry[2]
   )
{
   BDBG_ASSERT(apstEntry);
   apstEntry[0] = NULL;
   apstEntry[1] = NULL;

   /* If FIFO is doesn't contain pending data and the descriptor length is a multiple of the packetSize, then
    * bypass the FIFO and send this descriptor directly as-is.
    */
   if ( ( 0 == BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ) )
        && ( 0 == ( pstTransportDescriptor->uiBufferLength % hMuxMCPBCh->stState.uiPacketSize ) ) )
   {
      return BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsDirect( hMuxMCPBCh, pstTransportDescriptor, &apstEntry[0] );
   }

   /* We need to copy the descriptor data to the FIFO */

   /* Check to make sure there's enough room in the FIFO */
   if ( pstTransportDescriptor->uiBufferLength >= BMUXLIB_TS_P_FIFO_FREE( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ) )
   {
      BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] FIFO is full (need: %d, free: %d)",
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiRead,
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead,
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite,
         (int)hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize,
         (int)pstTransportDescriptor->uiBufferLength,
         BMUXLIB_TS_P_FIFO_FREE( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo )
         ));
      return false;
   }

   /* If this is the 1st descriptor being copied to the FIFO, we want to generate an entry using the input descriptor info */
   if ( 0 == BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ) )
   {
      /* Populate a temporary entry with the descriptor info */
      BMUXlib_TS_MCPB_P_PopulateEntryFromDescriptor( hMuxMCPBCh, pstTransportDescriptor, &hMuxMCPBCh->stState.stTSPacketInfo.stEntry );
   }

   /* Copy the data to the FIFO, making sure to handle the FIFO wrap */
   BMUXLIB_TS_P_FIFO_ADD( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo, BMUXlib_TS_MCPB_P_GetAddress( hMuxMCPBCh->hMuxMCPB, pstTransportDescriptor->uiBufferOffset ), pstTransportDescriptor->uiBufferLength );
   BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] Adding "BDBG_UINT64_FMT" %d bytes",
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiRead,
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead,
      hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite,
      (int)hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize,
      BDBG_UINT64_ARG(pstTransportDescriptor->uiBufferOffset),
      (int)pstTransportDescriptor->uiBufferLength
      ));
   /* Bump up the FIFOs source descriptor count */
   hMuxMCPBCh->stState.stTSPacketInfo.uiSourceDescriptorCount++;
   /* Bump up the serial number end for this entry */
   hMuxMCPBCh->stState.stTSPacketInfo.stEntry.uiSerialNumberEnd++;
   hMuxMCPBCh->hMuxMCPB->uiSerialNumber++;

   /* If the FIFO packet depth is a multiple of packetSize, we can send out a descriptor (or two if FIFO wraps) */
   if ( 0 == ( BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ) % hMuxMCPBCh->stState.uiPacketSize ) )
   {
      unsigned i;

      BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] Sending packet %d bytes",
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiRead,
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead,
         hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite,
         (int)hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize,
         BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo )
      ));
      for ( i = 0; (i < 2) && (0 != BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo )); i++ )
      {
         BMUXlib_TS_MCPB_QueueEntry *pstEntry;
         apstEntry[i] = (BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)*) BMUXlib_Malloc(sizeof(BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)));
         BDBG_ASSERT(apstEntry[i]);
         BKNI_Memset(apstEntry[i], 0, sizeof(BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry)));
         pstEntry =  BMUXLIB_LIST_ENTRY_DATA(apstEntry[i]);

         if ( 0 == i )
         {
            *pstEntry = hMuxMCPBCh->stState.stTSPacketInfo.stEntry;
         }
         else
         {
            BMUXlib_TS_MCPB_P_PopulateEntryFromDescriptor( hMuxMCPBCh, NULL, pstEntry );
         }

         /* TODO: What do with the descriptor serial number in the log? */

         /* Set Source Type to FIFO */
         pstEntry->eSource = BMUXlib_TS_MCPB_P_SourceType_eFIFO;

         /* Set Buffer Address */
         pstEntry->stTransportDescriptor.uiBufferOffset = hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiOffset + hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead;

         /* Override buffer address/length/bytes left */
         if ( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead < hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite )
         {
            pstEntry->stTransportDescriptor.uiBufferLength = hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite - hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead;
            pstEntry->uiSourceDescriptorCount = hMuxMCPBCh->stState.stTSPacketInfo.uiSourceDescriptorCount;
            hMuxMCPBCh->stState.stTSPacketInfo.uiSourceDescriptorCount = 0;
         }
         else
         {
            /* handle FIFO wrap */
            pstEntry->stTransportDescriptor.uiBufferLength = hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize - hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead;
         }

         /* Flush the cache */
         BMMA_FlushCache( hMuxMCPBCh->stState.stTSPacketInfo.stFifo.hBlock, (void*) ( ( (uint8_t *) hMuxMCPBCh->stState.stTSPacketInfo.stFifo.pBuffer ) + hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead ), pstEntry->stTransportDescriptor.uiBufferLength );

         BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] Sending descriptor[%d] "BDBG_UINT64_FMT" %d bytes for packet %d bytes %d source descriptors",
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiRead,
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead,
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite,
            (int)hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize,
            i,
            BDBG_UINT64_ARG(pstEntry->stTransportDescriptor.uiBufferOffset),
            (int)pstEntry->stTransportDescriptor.uiBufferLength,
            BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ),
            pstEntry->uiSourceDescriptorCount
         ));

         /* Set Bytes Left */
         pstEntry->uiBytesLeft = pstEntry->stTransportDescriptor.uiBufferLength;
         BMUXLIB_TS_P_FIFO_PKTCONSUME( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo, pstEntry->stTransportDescriptor.uiBufferLength );
         BMUXlib_TS_MCPB_P_UpdateCurrentESCR( hMuxMCPBCh, pstEntry );

         BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_FIFO, ("[read:%6d pktread:%6d write:%6d size:%6d] Sent    descriptor[%d] "BDBG_UINT64_FMT" %d bytes for packet %d bytes %d source descriptors",
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiRead,
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiPacketRead,
            hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiWrite,
            (int)hMuxMCPBCh->stState.stTSPacketInfo.stFifo.uiSize,
            i,
            BDBG_UINT64_ARG(pstEntry->stTransportDescriptor.uiBufferOffset),
            (int)pstEntry->stTransportDescriptor.uiBufferLength,
            BMUXLIB_TS_P_FIFO_PKTDEPTH( &hMuxMCPBCh->stState.stTSPacketInfo.stFifo ),
            pstEntry->uiSourceDescriptorCount
         ));
      }
   }

   return true;
}

BERR_Code
BMUXlib_TS_MCPB_Channel_AddTransportDescriptors(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   const BMUXlib_TS_TransportDescriptor *astTransportDescriptors, /* Array of pointers to transport descriptors */
   size_t uiCount, /* Count of descriptors in array */
   size_t *puiQueuedCount /* Count of descriptors queued (*puiQueuedCount <= uiCount) */
   )
{
   BDBG_ENTER( BMUXlib_TS_MCPB_Channel_AddTransportDescriptors );

   BDBG_ASSERT( hMuxMCPBCh );
   BDBG_ASSERT( astTransportDescriptors );
   BDBG_ASSERT( puiQueuedCount );

   *puiQueuedCount = 0;

   while ( *puiQueuedCount != uiCount )
   {
      BMUXLIB_LIST_ENTRY_TYPE(BMUXlib_TS_MCPB_QueueEntry) *apstEntry[2] = { NULL, NULL };
      const BMUXlib_TS_TransportDescriptor *pstTransportDescriptor = &astTransportDescriptors[*puiQueuedCount];
      unsigned i;

      if ( false == ( hMuxMCPBCh->stSettings.bIsTS
                      ? BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsViaFifo( hMuxMCPBCh, pstTransportDescriptor, apstEntry )
                      : BMUXlib_TS_MCPB_P_Channel_AddTransportDescriptorsDirect( hMuxMCPBCh, pstTransportDescriptor, &apstEntry[0] ) ) )
      {
         BDBG_WRN(("Only (%d/%d) queued (%d)", (int)*puiQueuedCount, (int)uiCount, hMuxMCPBCh->stSettings.bIsTS));
         break;
      }

      for ( i = 0; i < 2; i ++ )
      {
         if ( apstEntry[i] != NULL )
         {
            /* Insert the entry into the descriptor FIFO */
            BMUXLIB_LIST_ADD( &hMuxMCPBCh->stDescriptorFifo, apstEntry[i] );
            BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_QUEUE, ("[%p] Adding baseESCR=%08x (%d bytes)", (void *)&hMuxMCPBCh->stDescriptorFifo, BMUXLIB_LIST_ENTRY_DATA(apstEntry[0])->uiBaseESCR, BMUXLIB_LIST_ENTRY_DATA(apstEntry[i])->uiBytesLeft));
         }
      }

      (*puiQueuedCount)++;
   }

   hMuxMCPBCh->stState.uiQueuedCount += *puiQueuedCount;

   BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_AddTransportDescriptors );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_MCPB_Channel_GetCompletedTransportDescriptors(
   BMUXlib_TS_MCPB_Channel_Handle hMuxMCPBCh,
   size_t *puiCompletedCount /* Count of descriptors completed */
   )
{
   BDBG_ENTER( BMUXlib_TS_MCPB_Channel_GetCompletedTransportDescriptors );

   BDBG_ASSERT( hMuxMCPBCh );
   BDBG_ASSERT( puiCompletedCount );

   BMUXlib_TS_MCPB_P_ProcessCompletedTransportDescriptors( hMuxMCPBCh->hMuxMCPB );

   *puiCompletedCount = hMuxMCPBCh->stState.uiCompletedCount;
   BDBG_ASSERT( hMuxMCPBCh->stState.uiPendingCount >= hMuxMCPBCh->stState.uiCompletedCount );

   hMuxMCPBCh->stState.uiPendingCount -= hMuxMCPBCh->stState.uiCompletedCount;
   hMuxMCPBCh->stState.uiCompletedCount = 0;

   /* If only pending descriptors are left, then release them, also */
   if ( hMuxMCPBCh->stState.uiSkippedDescriptorCount == hMuxMCPBCh->stState.uiPendingCount )
   {
      *puiCompletedCount += hMuxMCPBCh->stState.uiSkippedDescriptorCount;
      hMuxMCPBCh->stState.uiPendingCount -= hMuxMCPBCh->stState.uiSkippedDescriptorCount;
      hMuxMCPBCh->stState.uiSkippedDescriptorCount = 0;
   }

   BDBG_LEAVE( BMUXlib_TS_MCPB_Channel_GetCompletedTransportDescriptors );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_MCPB_DoMux(
         BMUXlib_TS_MCPB_Handle hMuxMCPB,
         unsigned uiEndESCR
         )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BMUXlib_TS_MCPB_DoMux );

   BDBG_ASSERT( hMuxMCPB );

   hMuxMCPB->uiDoMuxCount++;

   /* Descriptors may have been added to the FIFOs since the last call to DoMux,
    * so we need to re-sort the priority queue before proceeding */
   BMUXlib_TS_MCPB_P_ReconcileFifoQueue( hMuxMCPB );

   while ( 1 )
   {
      /* Make sure there's input data to process */
      if ( true == BMUXLIB_LIST_ISEMPTY( hMuxMCPB->astListQueue[0] ) ) break;
      if ( false == BMUXlib_TS_MCPB_P_ProcessNextEntry( hMuxMCPB, uiEndESCR ) ) break;
   }

   /* Send pending descriptors to output transport channel */
   BMUXlib_TS_MCPB_P_ScheduleProcessedEntries( hMuxMCPB );

   {
      unsigned i;
      for( i = 0; i < hMuxMCPB->stSettings.uiMaxNumInputs; i++ )
      {
         BDBG_MODULE_MSG( BMUXLIB_TS_MCPB_QUEUE, ("[%p] Queued: %d Pending: %d", (void *)&hMuxMCPB->astChannel[i].stDescriptorFifo, hMuxMCPB->astChannel[i].stState.uiQueuedCount, hMuxMCPB->astChannel[i].stState.uiPendingCount ));
      }
   }
   BDBG_LEAVE( BMUXlib_TS_MCPB_DoMux );
   return BERR_TRACE( rc );
}
