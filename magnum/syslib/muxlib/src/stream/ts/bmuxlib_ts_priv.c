/***************************************************************************
 *     Copyright (c) 2003-2014, Broadcom Corporation
 *     All Rights Reserved
 *     Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 * [File Description:]
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 ***************************************************************************/

/* base modules */
#include "bstd.h"           /* standard types */
#include "berr.h"           /* error code */
#include "bdbg.h"           /* debug interface */
#include "bkni.h"           /* kernel interface */

#include "blst_slist.h"

#include "bmuxlib_ts.h"
#include "bmuxlib_ts_priv.h"

BDBG_MODULE(BMUXLIB_TS_PRIV);
BDBG_FILE_MODULE(BMUXLIB_TS_OUTPUT_DESC);
BDBG_FILE_MODULE(BMUXLIB_TS_OUTPUT_DESC_ERROR);
BDBG_FILE_MODULE(BMUXLIB_TS_EOS);
BDBG_FILE_MODULE(BMUXLIB_TS_SYSDATA);
BDBG_FILE_MODULE(BMUXLIB_TS_PCR);
BDBG_FILE_MODULE(BMUXLIB_TS_MTU);
BDBG_FILE_MODULE(BMUXLIB_TS_UD_RQ);    /* release Q debug */

const BMUXlib_TS_P_PESHeader s_stDefaultPESHeader =
{
 {
  0x00,0x00,0x01, /* Start Code */
  0xE0, /* Stream ID */
  0x00, 0x00, /* Packet Length */
  0x80, 0x00, /* Extension w/ PTS+DTS invalid */
  0x0A, /* PES Header Data Length */
  0xFF, /* PTS[32:30] */
  0xFF, 0xFF, /* PTS[29:15] */
  0xFF, 0xFF, /* PTS[14:00] */
  0xFF, /* DTS[32:30] */
  0xFF, 0xFF, /* DTS[29:15] */
  0xFF, 0xFF  /* DTS[14:00] */
 },
};

const BMUXlib_TS_P_BPPData s_stDummyPESFrame =
{
   {
      0x00,0x00,0x01, /* Start Code */
      0xFF, /* Dummy Stream ID */
      0x00, 0xB2, /* Packet Length */
      0x80, 0x00, /* Extension w/ PTS+DTS invalid */
      0x0A, /* PES Header Data Length */
      0xFF, /* PTS[32:30] */
      0xFF, 0xFF, /* PTS[29:15] */
      0xFF, 0xFF, /* PTS[14:00] */
      0xFF, /* DTS[32:30] */
      0xFF, 0xFF, /* DTS[29:15] */
      0xFF, 0xFF, /* DTS[14:00] */
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF
   }
};

const BMUXlib_TS_P_TSPacket s_stNullTSPacket =
{
   {
      0x47, /* Sync Byte */
      0x1F, 0xFF, /* PUSI/PID */
      0x00, /* Scrambling/Adaptation/CC */
      /* Adaptation/Payload */
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
   }
};

const BMUXlib_TS_P_TSPacket s_stDefaultTSPacket =
{
 {
    0x47, /* Sync Byte */
    0x00, 0x00, /* PUSI/PID */
    0x00, /* Scrambling/Adaptation/CC */
    /* Adaptation/Payload */
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF

 },
};

const BMUXlib_TS_P_BPPData s_stDefaultBPPData =
{
 {
    0x00,0x00,0x01, /* Start Code */
    0xE0, /* Stream ID */
    0x00, 0xB2, /* Packet Length (184 - 6 bytes)*/
    0x81, 0x01, /* Extension w/ PES Extension Flag Valid */
    0x14, /* PES Header Data Length (29 - 9 bytes) */
    0x80, /* PES Extension Flags w/ PES Private Data Valid */
    'B','R','C','M', /* "BRCM" signature for BPP */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /* Stuffing */
    0xFF,0xFF,0xFF, /* More Stuffing */
    0x00, /* Sub-Stream ID */
    0x00,0x00,0x00,0x00, /* BPP command */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00
 },
};

const BMUXlib_TS_P_TSPacket s_stDefaultBTPPacket =
{
 {
    0x47, /* Sync Byte */
    0x00, 0x00, /* PUSI/PID */
    0x20, /* Scrambling/Adaptation/CC */
    /* Adaptation/Payload */
    0xb7, /* Adaptation field length (183, the remainder of the packet) */
    0x82,
    0x2D, /* Transport private data length */
    0x00, /* Align Byte */
    'B','R','C','M', /* "BRCM" signature for BTP */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 1 - BTP Command */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 2 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 3 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 4 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 5 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 6 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 7 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 8 */
    0xFF,0xFF,0xFF,0xFF, /* Control Word 9 */
    0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
 },
};

/*************************/
/* Private Mux Functions */
/*************************/
BERR_Code
BMUXlib_TS_P_ConfigureTransport(
         BMUXlib_TS_Handle hMuxTS
         )
{
   bool bReadyToConfigure = false;

   BDBG_ENTER( BMUXlib_TS_P_ConfigureTransport );

   hMuxTS->status.stStartSettings.transport.stDeviceInterface.fGetTransportSettings(
            hMuxTS->status.stStartSettings.transport.stDeviceInterface.pContext,
            &hMuxTS->status.stTransportSettings);

   hMuxTS->status.stTransportSettings.uiMuxDelay = hMuxTS->status.stStartSettings.uiServiceLatencyTolerance;
   hMuxTS->status.stTransportSettings.uiMuxDelay += hMuxTS->status.stStartSettings.uiServicePeriod;


   /* Handle NRT/AFAP mode */
   if ( true == hMuxTS->status.stStartSettings.bNonRealTimeMode )
   {
      /* Seed the pacing counter based on the smallest initial ESCR seen */
      if ( true == hMuxTS->status.bFirstESCRValid )
      {
         hMuxTS->status.stTransportSettings.bNonRealTimeMode = true;
         hMuxTS->status.stTransportSettings.stNonRealTimeSettings.uiPacingCounter = hMuxTS->status.uiFirstESCR;
         hMuxTS->status.stTransportSettings.stNonRealTimeSettings.uiPacingCounter -= ( hMuxTS->status.stTransportSettings.uiMuxDelay * 27000 );

         bReadyToConfigure = true;
      }
   }
   else
   {
      bReadyToConfigure = true;
   }

   if ( true == bReadyToConfigure )
   {
      hMuxTS->status.stStartSettings.transport.stDeviceInterface.fSetTransportSettings(
               hMuxTS->status.stStartSettings.transport.stDeviceInterface.pContext,
               &hMuxTS->status.stTransportSettings);

      hMuxTS->status.bTransportConfigured = true;
   }

   BDBG_LEAVE( BMUXlib_TS_P_ConfigureTransport );

   return BERR_TRACE( BERR_SUCCESS );
}

/* TODO: define "VideoSettings struct" to increase code modularity */
BERR_Code
BMUXlib_TS_P_ProcessCompletedBuffers(
   BMUXlib_TS_Handle hMuxTS
   )
{
   size_t auiInputDescriptorsConsumed[BMUXLIB_TS_MAX_INPUT_PIDS];

   BDBG_ENTER( BMUXlib_TS_P_ProcessCompletedBuffers );

   BKNI_Memset(
            auiInputDescriptorsConsumed,
            0,
            sizeof( size_t ) * BMUXLIB_TS_MAX_INPUT_PIDS
            );

   /* Get Completed Transport Descriptors */
   {
      size_t uiCompleted;
      BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
      BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
      uint32_t uiTransportChannelIndex;

      for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
      {
         BMUXlib_TS_TransportChannelInterface *pTransportInterface = &hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stChannelInterface;

         if ( ( NULL != pTransportInterface->pContext )
              && ( NULL != pTransportInterface->fGetCompletedTransportDescriptors )
            )
         {
            pTransportInterface->fGetCompletedTransportDescriptors(
                     pTransportInterface->pContext,
                     &uiCompleted
                     );

            hMuxTS->status.stTransport[uiTransportChannelIndex].uiDescriptorsCompleted += uiCompleted;

            uiCompleted += hMuxTS->uiPendingCompleted[uiTransportChannelIndex];

            while ( 0 != uiCompleted )
            {
#if 1
               BMUXlib_TS_TransportStatus stTransportStatus;

               BERR_TRACE( BMUXlib_List_GetHead(
                        hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex],
                        (void**) &pstCurrentTransportDescriptorMetaData
                        ) );

               BERR_TRACE( BMUXlib_List_GetHead(
                        hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex],
                        (void**) &pstCurrentTransportDescriptor
                        ) );

               hMuxTS->status.stStartSettings.transport.stDeviceInterface.fGetTransportStatus(
                        hMuxTS->status.stStartSettings.transport.stDeviceInterface.pContext,
                        &stTransportStatus
                        );

               /* SW7435-535: Temporary fix for prematurely released transport descriptors */
               if ( true == pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
               {
                  if ( ( ( stTransportStatus.uiESCR < pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp )
                         && ( ( pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp - stTransportStatus.uiESCR ) < 0x80000000 ) )
                       || ( ( stTransportStatus.uiESCR > pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp )
                           && ( ( stTransportStatus.uiESCR - pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp ) > 0x80000000 ) ) )
                  {
                     if ( ( pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp - stTransportStatus.uiESCR ) < 0x80000000 )
                     {
#if 0
                        BDBG_ERR(("Descriptor (type:%d) returned prematurely (0x%08x > 0x%08x) %d ms ([%d]%d pending)",
                           pstCurrentTransportDescriptorMetaData->eDataType,
                           pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
                           stTransportStatus.uiESCR,
                           (pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp - stTransportStatus.uiESCR)/27000,
                           uiTransportChannelIndex,
                           uiCompleted
                        ));
#endif
                        break;
                     }
                  }
               }
#endif

               BERR_TRACE( BMUXlib_List_Remove(
                        hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex],
                        (void**) &pstCurrentTransportDescriptorMetaData
                        ) );

               BERR_TRACE( BMUXlib_List_Remove(
                        hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex],
                        (void**) &pstCurrentTransportDescriptor
                        ) );

               /* Reclaim any internal buffers */
               switch ( pstCurrentTransportDescriptorMetaData->eDataType )
               {
                  case BMUXlib_TS_P_DataType_eNULL:
                     /* Do nothing */
                     break;

                  case BMUXlib_TS_P_DataType_eCDB:
                     /* Do nothing */
                     break;

                  case BMUXlib_TS_P_DataType_ePESHeader:
                     /* Put PES Header back on Free List */
                     BERR_TRACE( BMUXlib_List_Add(
                              hMuxTS->hPESHeaderFreeList,
                              (void*) pstCurrentTransportDescriptorMetaData->pBufferAddress
                              ) );
                     break;

                  case BMUXlib_TS_P_DataType_ePCRPacket:
                     /* Put PCR Packet back on Free List (TS) */
                     BERR_TRACE( BMUXlib_List_Add(
                              hMuxTS->hTSPacketFreeList,
                              (void*) pstCurrentTransportDescriptorMetaData->pBufferAddress
                              ) );
                     break;

                  case BMUXlib_TS_P_DataType_eBPP:
                     BDBG_MSG(("BPP Consumed by TS Muxer"));
                     if ( 0 != pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount )
                     {
                        auiInputDescriptorsConsumed[pstCurrentTransportDescriptorMetaData->uiInputIndex] += pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount;
                     }

                     BERR_TRACE( BMUXlib_List_Add(
                              hMuxTS->hBPPFreeList,
                              (void*) pstCurrentTransportDescriptorMetaData->pBufferAddress
                              ) );
                     break;

                  case BMUXlib_TS_P_DataType_eUserdataPTS:
                  {
                     BERR_Code rc;
                     /* return the userdata PTS entry to the free list */
                     rc = BERR_TRACE(BMUXlib_List_Add(hMuxTS->hUserdataPTSFreeList, (void *)pstCurrentTransportDescriptorMetaData->pBufferAddress));
                     /* there should always be room for the PTS entry on the free list */
                     BDBG_ASSERT(BERR_SUCCESS == rc);
                     break;
                  }

                  case BMUXlib_TS_P_DataType_eUserdataLocal:
                     /* used to identify use of local buffer for unwrapping split packet */
                     hMuxTS->status.stUserdataInfo[pstCurrentTransportDescriptorMetaData->uiInputIndex].bUnwrapInUse = false;
                     break;

                  case BMUXlib_TS_P_DataType_eMTUBPP:
                     BDBG_MSG(("MTU BPP Consumed by TS Muxer"));
                     if ( 0 != pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount )
                     {
                        auiInputDescriptorsConsumed[pstCurrentTransportDescriptorMetaData->uiInputIndex] += pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount;
                     }

                     BERR_TRACE( BMUXlib_List_Add(
                              hMuxTS->hMTUBPPFreeList,
                              (void*) pstCurrentTransportDescriptorMetaData->pBufferAddress
                              ) );
                     break;

                  case BMUXlib_TS_P_DataType_eMax:
                     BDBG_ERR(("Unknown data type"));
                     break;
               }

               switch ( pstCurrentTransportDescriptorMetaData->eSourceType )
               {
                  case BMUXlib_TS_P_SourceType_eAudio:
                  case BMUXlib_TS_P_SourceType_eVideo:
                     if ( BMUXlib_TS_P_DataType_eCDB == pstCurrentTransportDescriptorMetaData->eDataType )
                     {
                        if ( ( BMUXLIB_TS_P_INVALID_OFFSET != pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset ) && ( NULL != pstCurrentTransportDescriptorMetaData->hBufferBaseBlock ) )
                        {
                           BMMA_UnlockOffset( pstCurrentTransportDescriptorMetaData->hBufferBaseBlock, pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset );
                           pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset = BMUXLIB_TS_P_INVALID_OFFSET;
                           pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = NULL;
                        }
                        auiInputDescriptorsConsumed[pstCurrentTransportDescriptorMetaData->uiInputIndex] += pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount;
                     }
                     break;

                  case BMUXlib_TS_P_SourceType_eSystem:
                     {
                        unsigned i;

                        for ( i = 0; i < pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount; i++ )
                        {
                           if ( ( BMUXLIB_TS_P_INVALID_OFFSET != pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset ) && ( NULL != pstCurrentTransportDescriptorMetaData->hBufferBaseBlock ) )
                           {
                              BMMA_UnlockOffset( pstCurrentTransportDescriptorMetaData->hBufferBaseBlock, pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset );
                              pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset = BMUXLIB_TS_P_INVALID_OFFSET;
                              pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = NULL;
                           }
                           hMuxTS->status.uiSystemDataPendingListReadOffset++;
                           hMuxTS->status.uiSystemDataPendingListReadOffset %= hMuxTS->status.uiSystemDataMaxCount;
                        }
                     }
                     BMUXLIB_TS_P_GET_SYS_DATA_COMP_CNT(hMuxTS) += pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount;
                     break;

                  case BMUXlib_TS_P_SourceType_eUserdata:
                     /* SW7425-3250: Sequence ID and desc counts passed back from transport are added to release Q */
                     BDBG_MODULE_MSG(BMUXLIB_TS_UD_RQ, ("UD[%d]: Returning desc from XPT: %d bytes, Seq: %d, count: %d",  pstCurrentTransportDescriptorMetaData->uiInputIndex, pstCurrentTransportDescriptor->uiBufferLength,
                        pstCurrentTransportDescriptorMetaData->uiSequenceID, pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount));
                     BMUXlib_TS_P_Userdata_AddToReleaseQ(hMuxTS, pstCurrentTransportDescriptorMetaData->uiInputIndex, pstCurrentTransportDescriptor->uiBufferLength,
                        pstCurrentTransportDescriptorMetaData->uiSequenceID, pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount);
                     break;

                  case BMUXlib_TS_P_SourceType_eUnknown:
                  case BMUXlib_TS_P_SourceType_eMax:
                     /* Unknown Source Type */
                     break;
               }

               if ( ( pstCurrentTransportDescriptorMetaData->eDataType < BMUXlib_TS_P_DataType_eMax ) && ( pstCurrentTransportDescriptorMetaData->eSourceType < BMUXlib_TS_P_SourceType_eMax ) )
               {
                  /* SW7425-5841: Update stats */
                  hMuxTS->status.stEfficiencyStats.uiNumBytes[pstCurrentTransportDescriptorMetaData->eDataType][pstCurrentTransportDescriptorMetaData->eSourceType][hMuxTS->status.stEfficiencyStats.uiIndex] += pstCurrentTransportDescriptor->uiBufferLength;
                  switch ( pstCurrentTransportDescriptorMetaData->eSourceType )
                  {
                     case BMUXlib_TS_P_SourceType_eAudio:
                     case BMUXlib_TS_P_SourceType_eVideo:
                        switch ( pstCurrentTransportDescriptorMetaData->eDataType )
                        {
                           case BMUXlib_TS_P_DataType_eCDB:
                              hMuxTS->status.stEfficiencyStats.uiFrameSize[pstCurrentTransportDescriptorMetaData->eSourceType] += pstCurrentTransportDescriptor->uiBufferLength;
                              break;
                           case BMUXlib_TS_P_DataType_ePESHeader:
                           {
                              unsigned uiPacketizationOverhead = 0;

                              /* Account for padding at the end of each frame */
                              if ( 0 != ( hMuxTS->status.stEfficiencyStats.uiFrameSize[pstCurrentTransportDescriptorMetaData->eSourceType] % 184 ) )
                              {
                                 uiPacketizationOverhead += 184 - (hMuxTS->status.stEfficiencyStats.uiFrameSize[pstCurrentTransportDescriptorMetaData->eSourceType] % 184);
                              }
                              /* Account for 4 byte TS Header per 184 bytes of CDB */
                              uiPacketizationOverhead += ( ( hMuxTS->status.stEfficiencyStats.uiFrameSize[pstCurrentTransportDescriptorMetaData->eSourceType] + 183 ) / 184 ) * 4;

                              hMuxTS->status.stEfficiencyStats.uiPacketizationOverhead[hMuxTS->status.stEfficiencyStats.uiIndex] += uiPacketizationOverhead;
                              hMuxTS->status.stEfficiencyStats.uiTotalBytesWritten += uiPacketizationOverhead;

                              /* Reset frame size count */
                              hMuxTS->status.stEfficiencyStats.uiFrameSize[pstCurrentTransportDescriptorMetaData->eSourceType] = pstCurrentTransportDescriptor->uiBufferLength;
                           }
                              break;
                           default:
                              break;
                        }
                        break;
                     default:
                        break;
                  }

                  hMuxTS->status.stEfficiencyStats.uiTotalBytesWritten += pstCurrentTransportDescriptor->uiBufferLength;
               }

               BERR_TRACE( BMUXlib_List_Add(
                        hMuxTS->hTransportDescriptorMetaDataFreeList,
                        pstCurrentTransportDescriptorMetaData
                        ) );

               BERR_TRACE( BMUXlib_List_Add(
                        hMuxTS->hTransportDescriptorFreeList,
                        pstCurrentTransportDescriptor
                        ) );

               hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued--;

               uiCompleted--;
               hMuxTS->status.stTransport[uiTransportChannelIndex].uiDescriptorsReturned++;
            } /* end: while descriptors remain */
            hMuxTS->uiPendingCompleted[uiTransportChannelIndex] = uiCompleted;
         } /* end: if interface valid */
      } /* end: for each transport channel */
   }

   /* Return completed descriptors back to encoders */
   {
      unsigned uiInputIndex;

      /* Return completed video descriptors */
      for ( uiInputIndex = 0; uiInputIndex < BMUXLIB_TS_MAX_INPUT_PIDS; uiInputIndex++ )
      {
         if ( 0 != auiInputDescriptorsConsumed[uiInputIndex] )
         {
            BMUXlib_Input_ConsumeDescriptors( hMuxTS->status.stInputMetaData[uiInputIndex].hInput, auiInputDescriptorsConsumed[uiInputIndex] );
         }
      }
   }

   /* free up any userdata that may have been returned from transport */
   BMUXlib_TS_P_Userdata_ProcessReleaseQueues(hMuxTS);

   BDBG_LEAVE( BMUXlib_TS_P_ProcessCompletedBuffers );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_CalculateNextESCR(
         uint32_t uiCurrentESCR,
         size_t uiLength,
         uint32_t uiPacket2PacketDeltaTimestamp,
         uint32_t *puiNextESCR,
         unsigned uiPacketSize
         )
{
   /* Set next expected ESCR value */
   uint64_t uiNextESCR = uiPacket2PacketDeltaTimestamp;

   BDBG_ASSERT( puiNextESCR );

   uiNextESCR *= (uiLength + (uiPacketSize - 1))/uiPacketSize;

   uiNextESCR += uiCurrentESCR;

   *puiNextESCR = (uiNextESCR & 0xFFFFFFFF);

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_AddTransportDescriptor(
   BMUXlib_TS_Handle hMuxTS,
   unsigned uiTransportChannelIndex,
   BMUXlib_TS_TransportDescriptor *pstTransportDescriptor,
   BMUXlib_TS_P_TransportDescriptorMetaData *pstTransportDescriptorMetaData
   )
{

   BDBG_ASSERT( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptor->uiBufferOffset );

   if ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
   {
      hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR = pstTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
      hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiLastStartingESCR = pstTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
   }

   if ( true == pstTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid )
   {
      hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiCurrentPacket2PacketTimestampDelta = pstTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta;
   }

   BMUXlib_TS_P_CalculateNextESCR(
      hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR,
      pstTransportDescriptor->uiBufferLength,
      hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiCurrentPacket2PacketTimestampDelta,
      &hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR,
      ( uiTransportChannelIndex == hMuxTS->status.stInput.system.uiTransportChannelIndex ) ? 188 : 184
   );

   if ( ( NULL != pstTransportDescriptorMetaData->hBufferBaseBlock )
        && ( NULL != pstTransportDescriptorMetaData->pBufferAddress ) )
   {
      BMMA_FlushCache(
         pstTransportDescriptorMetaData->hBufferBaseBlock,
         (void*) pstTransportDescriptorMetaData->pBufferAddress,
         pstTransportDescriptor->uiBufferLength
         );
   }

   /* SW7425-296: Coalesce contiguous buffer descriptors */
   if ( false == BMUXlib_List_IsEmpty( hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex] ) )
   {
      BMUXlib_TS_TransportDescriptor **astTransportDescriptors[2];
      BMUXlib_TS_P_TransportDescriptorMetaData **astTransportDescriptorsMetadata[2];
      const void *pDescriptors[2];
      size_t uiNumTransportDescriptors[2];

      /* Get the previous pending descriptor */
      BMUXlib_List_GetEntries(
               hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex],
               &pDescriptors[0],
               &uiNumTransportDescriptors[0],
               &pDescriptors[1],
               &uiNumTransportDescriptors[1]
               );

      astTransportDescriptors[0] = (BMUXlib_TS_TransportDescriptor **) pDescriptors[0];
      astTransportDescriptors[1] = (BMUXlib_TS_TransportDescriptor **) pDescriptors[1];

      BMUXlib_List_GetEntries(
               hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex],
               &pDescriptors[0],
               &uiNumTransportDescriptors[0],
               &pDescriptors[1],
               &uiNumTransportDescriptors[1]
               );

      astTransportDescriptorsMetadata[0] = (BMUXlib_TS_P_TransportDescriptorMetaData **) pDescriptors[0];
      astTransportDescriptorsMetadata[1] = (BMUXlib_TS_P_TransportDescriptorMetaData **) pDescriptors[1];

      if ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued < ( uiNumTransportDescriptors[0] + uiNumTransportDescriptors[1] ) )
      {
         BMUXlib_TS_TransportDescriptor *pstPreviousTransportDescriptor;
         BMUXlib_TS_P_TransportDescriptorMetaData *pstPreviousTransportDescriptorMetaData;
         unsigned uiQueueIndex = 0;

         if ( 0 != uiNumTransportDescriptors[1] )
         {
            uiQueueIndex = 1;
         }

         pstPreviousTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) astTransportDescriptors[uiQueueIndex][uiNumTransportDescriptors[uiQueueIndex] - 1];
         pstPreviousTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) astTransportDescriptorsMetadata[uiQueueIndex][uiNumTransportDescriptors[uiQueueIndex] - 1];

         if ( ( ( (unsigned) pstPreviousTransportDescriptor->uiBufferOffset + pstPreviousTransportDescriptor->uiBufferLength ) == (unsigned) pstTransportDescriptor->uiBufferOffset )
              && ( BMUXlib_TS_P_DataType_eCDB == pstTransportDescriptorMetaData->eDataType )
              && ( pstPreviousTransportDescriptorMetaData->eDataType == pstTransportDescriptorMetaData->eDataType )
              && ( pstPreviousTransportDescriptorMetaData->eSourceType == pstTransportDescriptorMetaData->eSourceType )
              && ( pstPreviousTransportDescriptorMetaData->uiInputIndex == pstTransportDescriptorMetaData->uiInputIndex )
              )
         {
            /* We have contiguous descriptors, so coalesce them */
            pstPreviousTransportDescriptor->uiBufferLength += pstTransportDescriptor->uiBufferLength;
            pstPreviousTransportDescriptorMetaData->uiSourceDescriptorCount += pstTransportDescriptorMetaData->uiSourceDescriptorCount;

            BDBG_MSG(("[%d][%d][%d] Coalescing descriptor w/ length = %d (@%p %d)",
               pstTransportDescriptorMetaData->eDataType,
               pstTransportDescriptorMetaData->eSourceType,
               pstTransportDescriptorMetaData->uiInputIndex,
               pstTransportDescriptor->uiBufferLength,
               pstPreviousTransportDescriptor->uiBufferOffset,
               pstPreviousTransportDescriptor->uiBufferLength
               ));

            if ( ( BMUXLIB_TS_P_INVALID_OFFSET != pstTransportDescriptorMetaData->uiBufferBaseOffset ) && ( NULL != pstTransportDescriptorMetaData->hBufferBaseBlock ) )
            {
               BMMA_UnlockOffset( pstTransportDescriptorMetaData->hBufferBaseBlock, pstTransportDescriptorMetaData->uiBufferBaseOffset );
               pstTransportDescriptorMetaData->uiBufferBaseOffset = BMUXLIB_TS_P_INVALID_OFFSET;
               pstTransportDescriptorMetaData->hBufferBaseBlock = NULL;
            }

            /* Don't add the new descriptor to the pending queue, instead, return them to the free list */
            BERR_TRACE( BMUXlib_List_Add(
                     hMuxTS->hTransportDescriptorMetaDataFreeList,
                     pstTransportDescriptorMetaData
                     ) );

            BERR_TRACE( BMUXlib_List_Add(
                     hMuxTS->hTransportDescriptorFreeList,
                     pstTransportDescriptor
                     ) );

            return BERR_TRACE( BERR_SUCCESS );
         }
      }
   }

   BERR_TRACE( BMUXlib_List_Add(
            hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex],
            pstTransportDescriptorMetaData
            ) );

   return BERR_TRACE( BMUXlib_List_Add(
            hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex],
            pstTransportDescriptor
            ) );
}

typedef struct BMUXlib_P_PESInfo
{
      BMUXlib_TS_P_SourceType eSourceType;
      unsigned uiTransportChannelIndex;
      uint8_t uiPESStreamID;

      uint64_t uiPTS;
      uint64_t uiDTS;
      uint32_t uiESCR;
      uint16_t uiTicksPerBit;
      int16_t iSHR;

      unsigned uiFrameSize;

      bool bSourceTypeValid;
      bool bTransportChannelIndexValid;

      bool bPTSValid;
      bool bDTSValid;
      bool bESCRValid;
      bool bTicksPerBitValid;
      bool bSHRValid;
      bool bFrameSizeValid;

      bool bRAI;
} BMUXlib_P_PESInfo;

const BMUXlib_TS_P_SourceType BMUXLIB_TS_P_InputDescriptorTypeLUT[BMUXlib_Input_Type_eMax] =
{
   BMUXlib_TS_P_SourceType_eVideo, /* BMUXlib_Input_Type_eVideo */
   BMUXlib_TS_P_SourceType_eAudio, /* BMUXlib_Input_Type_eAudio */
};

BERR_Code
BMUXlib_TS_P_PopulatePESInfoFromInputDescriptor(
         const BMUXlib_Input_Descriptor *pstDescriptor,
         BMUXlib_P_PESInfo *pstPESInfo,
         bool bSupportTTS
        )
{
   BDBG_ASSERT( pstDescriptor );
   BDBG_ASSERT( pstPESInfo );

   BKNI_Memset(
            pstPESInfo,
            0,
            sizeof( BMUXlib_P_PESInfo )
            );

   /* Common */
   pstPESInfo->bPTSValid = BMUXLIB_INPUT_DESCRIPTOR_IS_PTS_VALID( pstDescriptor );
   pstPESInfo->uiPTS = BMUXLIB_INPUT_DESCRIPTOR_PTS ( pstDescriptor );

   pstPESInfo->bESCRValid = BMUXLIB_INPUT_DESCRIPTOR_IS_ESCR_VALID( pstDescriptor );
   pstPESInfo->uiESCR = BMUXLIB_INPUT_DESCRIPTOR_ESCR( pstDescriptor );

   pstPESInfo->bTicksPerBitValid = BMUXLIB_INPUT_DESCRIPTOR_IS_TICKS_PER_BIT_VALID ( pstDescriptor );
   pstPESInfo->uiTicksPerBit = BMUXLIB_INPUT_DESCRIPTOR_TICKS_PER_BIT( pstDescriptor );

   pstPESInfo->bSHRValid = BMUXLIB_INPUT_DESCRIPTOR_IS_SHR_VALID( pstDescriptor );
   pstPESInfo->iSHR = BMUXLIB_INPUT_DESCRIPTOR_SHR( pstDescriptor );

   pstPESInfo->bRAI = BMUXLIB_INPUT_DESCRIPTOR_IS_KEYFRAME( pstDescriptor );

   pstPESInfo->eSourceType = BMUXLIB_TS_P_InputDescriptorTypeLUT[BMUXLIB_INPUT_DESCRIPTOR_TYPE( pstDescriptor )];
   pstPESInfo->bSourceTypeValid = true;

   switch ( pstPESInfo->eSourceType )
   {
      case BMUXlib_TS_P_SourceType_eVideo:
         pstPESInfo->bDTSValid = BMUXLIB_INPUT_DESCRIPTOR_IS_DTS_VALID( pstDescriptor );
         pstPESInfo->uiDTS = BMUXLIB_INPUT_DESCRIPTOR_VIDEO_DTS( pstDescriptor );

         if ( true == bSupportTTS )
         {
            /* SW7425-659: In Support TTS, for video, frame burst mode is enabled so the frame size is valid */

            pstPESInfo->bFrameSizeValid = true;
            pstPESInfo->uiFrameSize = BMUXLIB_INPUT_DESCRIPTOR_FRAMESIZE( pstDescriptor );
            /* We add 13 bytes to payload size to account for the PES Header */
            pstPESInfo->uiFrameSize += BMUXLIB_TS_P_PES_HEADER_PAYLOAD_SIZE;
         }
         break;

      case BMUXlib_TS_P_SourceType_eAudio:
         pstPESInfo->bFrameSizeValid = true;
         pstPESInfo->uiFrameSize = BMUXLIB_INPUT_DESCRIPTOR_FRAMESIZE( pstDescriptor );
         /* We add 13 bytes to payload size to account for the Optional PES Header */
         pstPESInfo->uiFrameSize += BMUXLIB_TS_P_PES_HEADER_PAYLOAD_SIZE;
         break;

      default:
         break;
   }

   return BERR_TRACE( BERR_SUCCESS );
}

void
BMUXlib_TS_P_UpdateTotalPacketTimestampDelta(
   BMUXlib_TS_Handle hMuxTS
   )
{
   /* 1/((1/A)+(1/B)) = 1/((B+A)/AB) = AB/(B+A) */
   /* 1/((1/A)-1/B) = 1/((B-A)/AB) = AB/(B-A) */

   /* Calculate total packet 2 packet timestamp delta using absolute math */
   {
      unsigned uiTransportIndex;
      uint64_t uiTotalPacket2PacketTimestampDelta64 = 0;

      for ( uiTransportIndex = 0; uiTransportIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportIndex++ )
      {
         if ( true == hMuxTS->status.stOutput.stTransport[uiTransportIndex].stPacket2PacketDelta.bValid )
         {
            if ( 0 == uiTotalPacket2PacketTimestampDelta64 )
            {
               uiTotalPacket2PacketTimestampDelta64 = hMuxTS->status.stOutput.stTransport[uiTransportIndex].stPacket2PacketDelta.uiValue;
            }
            else if ( 0 != hMuxTS->status.stOutput.stTransport[uiTransportIndex].stPacket2PacketDelta.uiValue )
            {
               uiTotalPacket2PacketTimestampDelta64 = ( uiTotalPacket2PacketTimestampDelta64 * hMuxTS->status.stOutput.stTransport[uiTransportIndex].stPacket2PacketDelta.uiValue )/( hMuxTS->status.stOutput.stTransport[uiTransportIndex].stPacket2PacketDelta.uiValue + uiTotalPacket2PacketTimestampDelta64 );
            }
         }
      }

      hMuxTS->status.uiTotalPacket2PacketTimestampDelta = uiTotalPacket2PacketTimestampDelta64;
   }

   hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 = ((hMuxTS->status.uiTotalPacket2PacketTimestampDelta / 300) << 9) | (hMuxTS->status.uiTotalPacket2PacketTimestampDelta % 300);
}

typedef struct BMUXlib_P_BPPInfo
{
      bool bSourceTypeValid;
      bool bTransportChannelIndexValid;
      bool bSkippedDescriptorsValid;
      bool bInputIndexValid;
      bool bESCRValid;
      bool bPacket2PacketDeltaValid;
      bool bDataTypeValid;

      BMUXlib_TS_P_SourceType eSourceType;
      unsigned uiTransportChannelIndex;
      unsigned uiSkippedDescriptors;
      unsigned uiInputIndex;
      uint32_t uiESCR; /* 27 Mhz */
      uint32_t uiPacket2PacketDelta; /* 27 Mhz */

      void *pBuffer;
      size_t uiLength;

      BMUXlib_TS_P_DataType eDataType;
} BMUXlib_P_BPPInfo;

BERR_Code
BMUXlib_TS_P_InsertBPP(
         BMUXlib_TS_Handle hMuxTS,
         BMUXlib_P_BPPInfo *pstBPPInfo
         )
{
   {
      /* Allocate Transport Descriptor */
      BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
      BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
      void *pEntry = NULL;

      BERR_TRACE( BMUXlib_List_Remove(
               hMuxTS->hTransportDescriptorMetaDataFreeList,
               &pEntry
               ) );

      pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

      /* Populate Transport Meta Data */
      BKNI_Memset(
               pstCurrentTransportDescriptorMetaData,
               0,
               sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
               );

      pstCurrentTransportDescriptorMetaData->eDataType = pstBPPInfo->bDataTypeValid ? pstBPPInfo->eDataType : BMUXlib_TS_P_DataType_eBPP;
      pstCurrentTransportDescriptorMetaData->eSourceType = pstBPPInfo->eSourceType;

      if ( true == pstBPPInfo->bSkippedDescriptorsValid )
      {
         if ( 0 != pstBPPInfo->uiSkippedDescriptors )
         {
            pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount += pstBPPInfo->uiSkippedDescriptors;
            BDBG_ASSERT( pstBPPInfo->bInputIndexValid );
            pstCurrentTransportDescriptorMetaData->uiInputIndex = pstBPPInfo->uiInputIndex;
         }
      }

      BERR_TRACE( BMUXlib_List_Remove(
               hMuxTS->hTransportDescriptorFreeList,
               &pEntry
               ) );

      pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;

      /* Populate Transport Descriptor */
      BKNI_Memset(
               pstCurrentTransportDescriptor,
               0,
               sizeof( BMUXlib_TS_TransportDescriptor )
               );

      /* Set ESCR */
      pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = pstBPPInfo->bESCRValid;
      if ( true == pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
      {
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = pstBPPInfo->uiESCR;
      }

      /* Set Packet 2 Packet Timestamp Delta */
      pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = pstBPPInfo->bPacket2PacketDeltaValid;
      if ( true == pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid )
      {
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = pstBPPInfo->uiPacket2PacketDelta;
      }

      /* Set Buffer Info */
      pstCurrentTransportDescriptorMetaData->pBufferAddress = pstBPPInfo->pBuffer;
      pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
      pstCurrentTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
      pstCurrentTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
      pstCurrentTransportDescriptor->uiBufferLength = pstBPPInfo->uiLength;

      BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
         hMuxTS,
         pstBPPInfo->uiTransportChannelIndex,
         pstCurrentTransportDescriptor,
         pstCurrentTransportDescriptorMetaData
         ) );
   }

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_InsertPESHeader(
         BMUXlib_TS_Handle hMuxTS,
         BMUXlib_P_PESInfo *pstPESInfo
         )
{
   BDBG_ASSERT( true == pstPESInfo->bTransportChannelIndexValid );

   if ( false == BMUXlib_List_IsEmpty(
                 hMuxTS->hPESHeaderFreeList
                 )
      )
   {
      void *pEntry = NULL;
      /* Allocate PES Header */
      BMUXlib_TS_P_PESHeader *pstCurrentPESHeader = NULL;

      BERR_TRACE( BMUXlib_List_Remove(
               hMuxTS->hPESHeaderFreeList,
               &pEntry
               ) );

      pstCurrentPESHeader = (BMUXlib_TS_P_PESHeader *) pEntry;

      /* Populate PES Header */
      BKNI_Memcpy(
               pstCurrentPESHeader->data,
               &s_stDefaultPESHeader,
               sizeof( BMUXlib_TS_P_PESHeader )
               );

      /* Set Stream ID */
      BMUXlib_TS_P_PESHeader_SetStreamID(
               pstCurrentPESHeader->data,
               pstPESInfo->uiPESStreamID
               );

      /* Set PTS */
      /* TODO: Optimize by not sending PTS portion of PES Header if PTS doesn't exist */
      if ( true == pstPESInfo->bPTSValid )
      {
         BMUXlib_TS_P_PESHeader_SetPTS(
                  pstCurrentPESHeader->data,
                  pstPESInfo->uiPTS
                  );
      }

      /* Set DTS */
      /* TODO: Optimize by not sending DTS portion of PES Header if DTS doesn't exist */
      if ( ( true == pstPESInfo->bDTSValid )
           && ( true == pstPESInfo->bPTSValid )
           && ( pstPESInfo->uiPTS != pstPESInfo->uiDTS )
         )
      {
         BMUXlib_TS_P_PESHeader_SetDTS(
                  pstCurrentPESHeader->data,
                  pstPESInfo->uiDTS
                  );
      }

      /* Set Length */
      if ( ( true == pstPESInfo->bFrameSizeValid )
           && ( BMUXlib_TS_P_SourceType_eVideo != pstPESInfo->eSourceType ) )
      {
         BMUXlib_TS_P_PESHeader_SetLength(
                  pstCurrentPESHeader->data,
                  pstPESInfo->uiFrameSize
                  );
      }

      {
         /* Allocate Transport Descriptor */
         BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
         BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
         void *pEntry = NULL;

         BERR_TRACE( BMUXlib_List_Remove(
                  hMuxTS->hTransportDescriptorMetaDataFreeList,
                  &pEntry
                  ) );

         pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

         /* Populate Transport Meta Data */
         BKNI_Memset(
                  pstCurrentTransportDescriptorMetaData,
                  0,
                  sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
                  );

         pstCurrentTransportDescriptorMetaData->eDataType = BMUXlib_TS_P_DataType_ePESHeader;
         pstCurrentTransportDescriptorMetaData->eSourceType = pstPESInfo->eSourceType;

         BERR_TRACE( BMUXlib_List_Remove(
                  hMuxTS->hTransportDescriptorFreeList,
                  &pEntry
                  ) );

         pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;

         /* Populate Transport Descriptor */
         BKNI_Memset(
                  pstCurrentTransportDescriptor,
                  0,
                  sizeof( BMUXlib_TS_TransportDescriptor )
                  );

         /* Set Buffer Info */
         pstCurrentTransportDescriptorMetaData->pBufferAddress = pstCurrentPESHeader;
         pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
         pstCurrentTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
         pstCurrentTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
         pstCurrentTransportDescriptor->uiBufferLength = sizeof( BMUXlib_TS_P_PESHeader );

         /* Set ESCR */
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = pstPESInfo->bESCRValid;
         if ( true == pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
         {
            pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = pstPESInfo->uiESCR;
         }

         /* Set Packet 2 Packet Timestamp Delta */
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = pstPESInfo->bTicksPerBitValid;
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid &= pstPESInfo->bSHRValid;
         if ( true == pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid )
         {
            uint64_t uiTempPacket2PacketTimestampDelta = pstPESInfo->uiTicksPerBit;
            /* PES header represents about 19 bytes = 19/ 184, AND for an I-frame there is
               an extra 2 byte adaptation header in the TS packet to allow for RAI
               => 21 bytes / 184 bytes worst case => almost 12 % overhead */
            /* NOTE: This packet-to-packet delta is used for all packets that get created by the transport for this
               data block */
            uiTempPacket2PacketTimestampDelta *= 184 * 8 * 99;
            uiTempPacket2PacketTimestampDelta /= 100;

            if ( true == pstPESInfo->bDTSValid )
            {
               pstCurrentTransportDescriptorMetaData->stDtsInfo.bValid = pstPESInfo->bDTSValid;
               pstCurrentTransportDescriptorMetaData->stDtsInfo.uiDtsIn27Mhz = pstPESInfo->uiDTS * 300;
            }
            else
            {
               pstCurrentTransportDescriptorMetaData->stDtsInfo.bValid = pstPESInfo->bPTSValid;
               pstCurrentTransportDescriptorMetaData->stDtsInfo.uiDtsIn27Mhz = pstPESInfo->uiPTS * 300;
            }

            BDBG_ASSERT( pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid == pstCurrentTransportDescriptorMetaData->stDtsInfo.bValid );

            if ( pstPESInfo->iSHR > 0 )
            {
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = uiTempPacket2PacketTimestampDelta >> pstPESInfo->iSHR;
            }
            else
            {
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = uiTempPacket2PacketTimestampDelta << -pstPESInfo->iSHR;
            }

            /* SW7425-659: Calculate actual PKT2PKT_DELTA using frame length */
            if ( ( true == pstPESInfo->bFrameSizeValid )
                 && ( true == pstPESInfo->bESCRValid ) )
            {
               /* We need to add 6 bytes extra for the PES header and 2 for the RAI adaptation for I-frames */
               unsigned uiTotalPackets = ( (pstPESInfo->uiFrameSize + 6) + 2 + (184 - 1))/184;
               uint64_t uiTotalTicks = 0;
               unsigned uiPacket2PacketTimestampDelta = 0;

               if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
               {
               /* SW7425-659: Even though the MTU BPP packet's PKT2PKT delta is 0, the PACING_COUNTER
                * still increments after processing an MTU BPP packet.  The root cause is TBD, but to
                * be safe, we include a possible MTU BPP packet in the PKT2PKT calculation.
                */
                  if ( ( BMUXlib_TS_P_SourceType_eVideo == pstPESInfo->eSourceType )
                       || ( false == hMuxTS->status.bBPPSentForVideo ) )
                  {
                     uiTotalPackets+=BMUXlib_TS_P_MTUBPPData_NUMCONSECUTIVEPACKETS; /* Include MTU BPP packet in front of frame */
                  }
               }

               /* SW7425-659: The frame size includes the PES header payload size, which we DO NOT want
                * to include for the purposes of calculating the ending ESCR since we want the ending ESCR
                * to be the same as what the encoder expects to be the end of the frame.  I.e. including
                * the PES header payload size may incorrectly make the ending ESCR larger than the next
                * frame's ESCR. So the ending ESCR value is really the time we need to finish sending the
                * last bit of this frame such that endingESCR(n) <= ESCR(n+1)
                */
               uiTotalTicks = pstPESInfo->uiTicksPerBit;
               uiTotalTicks *= ( ( pstPESInfo->uiFrameSize - BMUXLIB_TS_P_PES_HEADER_PAYLOAD_SIZE ) * 8);

               if ( pstPESInfo->iSHR > 0 )
               {
                  uiTotalTicks >>= pstPESInfo->iSHR;
               }
               else
               {
                  uiTotalTicks <<= -pstPESInfo->iSHR;
               }

               /* SW7435-1133: Check to make sure the frames arrives prior to its DTS
                * If not, then adjust the uiTotalTicks to ensure the frame arrives on time
                * */
               if ( ( true == pstPESInfo->bDTSValid ) || ( true == pstPESInfo->bPTSValid ) )
               {
                  uint64_t uiTotalTicksUsingDTS = 0;
                  uint32_t uiDtsIn27Mhz = 0;

                  if ( true == pstPESInfo->bDTSValid )
                  {
                     uiDtsIn27Mhz = (uint32_t) (pstPESInfo->uiDTS * 300);
                  }
                  else if ( true == pstPESInfo->bPTSValid )
                  {
                     uiDtsIn27Mhz = (uint32_t) (pstPESInfo->uiPTS * 300);
                  }

                  if ( uiDtsIn27Mhz > pstPESInfo->uiESCR )
                  {
                     uiTotalTicksUsingDTS = uiDtsIn27Mhz - pstPESInfo->uiESCR;
                  }
                  else
                  {
                     uiTotalTicksUsingDTS = (0xFFFFFFFF - pstPESInfo->uiESCR) + 1 + uiDtsIn27Mhz;
                  }

                  if ( uiTotalTicks > uiTotalTicksUsingDTS )
                  {
                     uiTotalTicks = uiTotalTicksUsingDTS;
                  }
               }

               /* SW7435-1133: Account for possible A/V packet interleave */
               {
                  unsigned uiTransportChannelIndex;

                  for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
                  {
                     if ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bActive )
                      {
                         /* Keep track of the worst case packet interleave conflict in the deltaESCR */
                        uiTotalPackets++;
                      }
                  }
               }

               uiPacket2PacketTimestampDelta = uiTotalTicks / uiTotalPackets;

               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = uiPacket2PacketTimestampDelta;
            }

            if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
            {
               /* SW7425-659: Insert MUX_TIMESTAMP_UPDATE BPP for ATS OFFSET support */

               if ( ( false == hMuxTS->status.stOutput.stTransport[pstPESInfo->uiTransportChannelIndex].stPacket2PacketDelta.bValid )
                    || ( pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta !=
                       hMuxTS->status.stOutput.stTransport[pstPESInfo->uiTransportChannelIndex].stPacket2PacketDelta.uiValue ) )
               {
#if BDBG_DEBUG_BUILD
                  unsigned uiPreviousPacket2PacketTimestampDelta = hMuxTS->status.stOutput.stTransport[pstPESInfo->uiTransportChannelIndex].stPacket2PacketDelta.uiValue;
#endif
                  hMuxTS->status.stOutput.stTransport[pstPESInfo->uiTransportChannelIndex].stPacket2PacketDelta.bValid = true;
                  hMuxTS->status.stOutput.stTransport[pstPESInfo->uiTransportChannelIndex].stPacket2PacketDelta.uiValue = pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta;

                  BMUXlib_TS_P_UpdateTotalPacketTimestampDelta(
                     hMuxTS
                     );

                  BDBG_MODULE_MSG( BMUXLIB_TS_MTU, ("Transport[%d] UPD (ESCR=%08x, PKT2PKT=(%08x-->%08x), TOTAL=(%08x-->%08x (%06x Base + %03x Ext)))",
                     pstPESInfo->uiTransportChannelIndex,
                     pstPESInfo->uiESCR,
                     uiPreviousPacket2PacketTimestampDelta,
                     pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
                     hMuxTS->status.uiPreviousPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 >> 9,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 & 0x1FF
                     ));
               }
               else
               {
                  BDBG_MODULE_MSG( BMUXLIB_TS_MTU, ("Transport[%d]     (ESCR=%08x)",
                     pstPESInfo->uiTransportChannelIndex,
                     pstPESInfo->uiESCR
                     ));
               }

               if ( true == pstPESInfo->bESCRValid )
               {
                  if ( ( hMuxTS->status.uiPreviousESCR > pstPESInfo->uiESCR )
                       && ( ( hMuxTS->status.uiPreviousESCR - pstPESInfo->uiESCR ) < 0x80000000 ) )
                  {
                     BDBG_MODULE_ERR( BMUXLIB_TS_MTU, ("Out of order ESCR!"));
                     BDBG_ASSERT(0);
                  }
                  hMuxTS->status.uiPreviousESCR = pstPESInfo->uiESCR;
               }

               /* Insert MUX_TIMESTAMP_UPDATE BPP on video channel only if the total bitrate has changed */
               if ( ( ( BMUXlib_TS_P_SourceType_eVideo == pstPESInfo->eSourceType )
                      /* SW7425-659: To prevent deltaATS=0 for any packets sent before the first video frame, we send
                       * an MTU BPP if a BPP for video hasn't been sent, yet.
                       */
                      || ( false == hMuxTS->status.bBPPSentForVideo ) )
                    && ( hMuxTS->status.uiPreviousPacket2PacketTimestampDelta != hMuxTS->status.uiTotalPacket2PacketTimestampDelta ) )
               {
                  unsigned i;

                  BMUXlib_P_BPPInfo stBPPInfo;

                  BKNI_Memset( &stBPPInfo, 0, sizeof( BMUXlib_P_BPPInfo ) );

                  stBPPInfo.uiTransportChannelIndex = pstPESInfo->uiTransportChannelIndex;
                  stBPPInfo.bTransportChannelIndexValid = true;

                  stBPPInfo.eSourceType = pstPESInfo->eSourceType;
                  stBPPInfo.bSourceTypeValid = true;

                  stBPPInfo.uiESCR = pstPESInfo->uiESCR;
                  stBPPInfo.bESCRValid = true;

                  stBPPInfo.uiPacket2PacketDelta = 0;
                  stBPPInfo.bPacket2PacketDeltaValid = true;

                  stBPPInfo.bDataTypeValid = true;
                  stBPPInfo.eDataType = BMUXlib_TS_P_DataType_eMTUBPP;

                  BERR_TRACE( BMUXlib_List_Remove(
                           hMuxTS->hMTUBPPFreeList,
                           &stBPPInfo.pBuffer
                           ) );

                  BKNI_Memcpy(
                           stBPPInfo.pBuffer,
                           &s_stDefaultBPPData,
                           sizeof ( BMUXlib_TS_P_BPPData )
                           );

                  stBPPInfo.uiLength = sizeof ( BMUXlib_TS_P_BPPData );

                  /* SW7425-659: MTU BPP parsing only works if SCD is enabled for the PID.  Typically, SCD is only
                   * enabled for the video PID, so if we are sending a BPP, we should send it on the video PID, only.
                   * However, if there isn't a video PID, then just use the audio PID.
                   */
                  BMUXlib_TS_P_BPPData_SetStreamID(
                              stBPPInfo.pBuffer,
                              ( 0 == hMuxTS->status.stStartSettings.uiNumValidVideoPIDs ) ? pstPESInfo->uiPESStreamID : hMuxTS->status.stStartSettings.video[0].uiPESStreamID
                              );

                  BMUXlib_TS_P_BPPData_SetControlWord(
                     stBPPInfo.pBuffer,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND_OFFSET,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND
                     );

                  BMUXlib_TS_P_BPPData_SetControlWord(
                     stBPPInfo.pBuffer,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_OFFSET,
                     0x2 /* Only PKT2PKT_DELTA_TIMESTAMP in BPP is valid */
                     );

                  BMUXlib_TS_P_BPPData_SetControlWord(
                     stBPPInfo.pBuffer,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_PKT2PKT_DELTA_TIMESTAMP_OFFSET,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300
                     );

                  /* SW7425-5180: Ensure the MTU BPP doesn't cause a CC error in resulting TS by
                   * sending the 16 MTU BPP consecutive packets so that the 4-bit CC count wraps.
                   */
                  for ( i = 1; i < BMUXlib_TS_P_MTUBPPData_NUMCONSECUTIVEPACKETS; i++ )
                  {
                     BKNI_Memcpy(
                              (uint8_t*) stBPPInfo.pBuffer + ( sizeof ( BMUXlib_TS_P_BPPData ) * i),
                              stBPPInfo.pBuffer,
                              sizeof ( BMUXlib_TS_P_BPPData )
                              );

                     stBPPInfo.uiLength += sizeof ( BMUXlib_TS_P_BPPData );
                  }

                  if ( BERR_SUCCESS != BMUXlib_TS_P_InsertBPP(
                           hMuxTS,
                           &stBPPInfo
                           )
                     )
                  {
                     BDBG_ERR(("Transport descriptors not available.  Will be processed next time."));
                     BDBG_ASSERT(0);
                  }

                  /* Adjust transport descriptor to not set ESCR, since ESCR is set by the BPP packet */
                  pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = false;

                  BDBG_MODULE_MSG( BMUXLIB_TS_MTU, ("Transport[%d] BPP (ESCR=%08x,                                TOTAL=(%08x-->%08x (%06x Base + %03x Ext)))",
                     pstPESInfo->uiTransportChannelIndex,
                     pstPESInfo->uiESCR,
                     hMuxTS->status.uiPreviousPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 >> 9,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 & 0x1FF
                     ));

                  hMuxTS->status.uiPreviousPacket2PacketTimestampDelta = hMuxTS->status.uiTotalPacket2PacketTimestampDelta;

                  if ( BMUXlib_TS_P_SourceType_eVideo == pstPESInfo->eSourceType )
                  {
                     hMuxTS->status.bBPPSentForVideo = true;
                  }
               }
            }
         }

         /* Set Random Access Indicator */
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bRandomAccessIndication = pstPESInfo->bRAI;

         BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
            hMuxTS,
            pstPESInfo->uiTransportChannelIndex,
            pstCurrentTransportDescriptor,
            pstCurrentTransportDescriptorMetaData
            ) );
      }
   }
   else
   {
      return BERR_TRACE( BERR_UNKNOWN );
   }

   return BERR_TRACE( BERR_SUCCESS );
}


typedef struct BMUXlib_P_TransportInfo
{
      BMUXlib_TS_P_SourceType eSourceType;
      unsigned uiTransportChannelIndex;

      BMMA_Block_Handle hBlock;
      unsigned uiBlockOffset;
      size_t uiLength;

      unsigned uiSkippedDescriptors;
      unsigned uiInputIndex;

      bool bSourceTypeValid;
      bool bTransportChannelIndexValid;
      bool bBufferValid;
      bool bLengthValid;
      bool bSkippedDescriptorsValid;
      bool bInputIndexValid;
} BMUXlib_P_TransportInfo;

BERR_Code
BMUXlib_TS_P_PopulateTransportInfoFromInputDescriptor(
         const BMUXlib_Input_Descriptor *pstDescriptor,
         BMUXlib_P_TransportInfo *pstTransportInfo
        )
{
   BDBG_ASSERT( pstDescriptor );
   BDBG_ASSERT( pstTransportInfo );

   BKNI_Memset(
            pstTransportInfo,
            0,
            sizeof( BMUXlib_P_TransportInfo )
            );

   pstTransportInfo->uiLength = BMUXLIB_INPUT_DESCRIPTOR_LENGTH( pstDescriptor );
   pstTransportInfo->bLengthValid = true;

   pstTransportInfo->hBlock = BMUXLIB_INPUT_DESCRIPTOR_BLOCK( pstDescriptor );
   pstTransportInfo->uiBlockOffset = BMUXLIB_INPUT_DESCRIPTOR_OFFSET( pstDescriptor );
   pstTransportInfo->bBufferValid = true;

   pstTransportInfo->eSourceType = BMUXLIB_TS_P_InputDescriptorTypeLUT[BMUXLIB_INPUT_DESCRIPTOR_TYPE( pstDescriptor )];
   pstTransportInfo->bSourceTypeValid = true;

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_InsertTransportDescriptor (
         BMUXlib_TS_Handle hMuxTS,
         BMUXlib_P_TransportInfo *pstTransportInfo
         )
{
   /* Allocate Transport Descriptor */
   BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
   BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
   void *pEntry = NULL;

   BDBG_ASSERT( pstTransportInfo );
   BDBG_ASSERT( pstTransportInfo->bSourceTypeValid );
   BDBG_ASSERT( pstTransportInfo->bTransportChannelIndexValid );
   BDBG_ASSERT( pstTransportInfo->bBufferValid );
   BDBG_ASSERT( pstTransportInfo->bLengthValid );

   BERR_TRACE( BMUXlib_List_Remove(
            hMuxTS->hTransportDescriptorMetaDataFreeList,
            &pEntry
            ) );

   pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

   /* Populate Transport Meta Data */
   BKNI_Memset(
            pstCurrentTransportDescriptorMetaData,
            0,
            sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
            );

   pstCurrentTransportDescriptorMetaData->eDataType = BMUXlib_TS_P_DataType_eCDB;
   pstCurrentTransportDescriptorMetaData->eSourceType = pstTransportInfo->eSourceType;
   pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount = 1;

   if ( true == pstTransportInfo->bSkippedDescriptorsValid )
   {
      pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount += pstTransportInfo->uiSkippedDescriptors;
   }

   if ( true == pstTransportInfo->bInputIndexValid )
   {
      pstCurrentTransportDescriptorMetaData->uiInputIndex = pstTransportInfo->uiInputIndex;
   }
   else
   {
      if ( ( BMUXlib_TS_P_SourceType_eAudio == pstTransportInfo->eSourceType )
           || ( BMUXlib_TS_P_SourceType_eVideo == pstTransportInfo->eSourceType ) )
      {
         BDBG_ERR(("uiInputIndex required for audio/video types"));
         BDBG_ASSERT(0);
      }
   }

   BERR_TRACE( BMUXlib_List_Remove(
            hMuxTS->hTransportDescriptorFreeList,
            &pEntry
            ) );

   pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;

   /* Populate Transport Descriptor */
   BKNI_Memset(
            pstCurrentTransportDescriptor,
            0,
            sizeof( BMUXlib_TS_TransportDescriptor )
            );

   /* Set Buffer Info */
   pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset = BMMA_LockOffset( pstTransportInfo->hBlock );
   pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = pstTransportInfo->hBlock;
   pstCurrentTransportDescriptor->uiBufferOffset = pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset + pstTransportInfo->uiBlockOffset;
   pstCurrentTransportDescriptor->uiBufferLength = pstTransportInfo->uiLength;

   BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
      hMuxTS,
      pstTransportInfo->uiTransportChannelIndex,
      pstCurrentTransportDescriptor,
      pstCurrentTransportDescriptorMetaData
      ) );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_InsertNULLTransportDescriptor (
         BMUXlib_TS_Handle hMuxTS,
         unsigned uiTransportChannelIndex,
         uint32_t uiESCR,
         BMUXlib_TS_P_DataType eDataType,
         BMUXlib_TS_P_SourceType eSourceType,
         unsigned uiInputIndex
         )
{
   /* Allocate Transport Descriptor */
   BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
   BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
   void *pEntry = NULL;

   BDBG_ASSERT( hMuxTS );

   BERR_TRACE( BMUXlib_List_Remove(
            hMuxTS->hTransportDescriptorMetaDataFreeList,
            &pEntry
            ) );

   pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

   /* Populate Transport Meta Data */
   BKNI_Memset(
            pstCurrentTransportDescriptorMetaData,
            0,
            sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
            );

   pstCurrentTransportDescriptorMetaData->eDataType = eDataType;
   pstCurrentTransportDescriptorMetaData->eSourceType = eSourceType;
   if ( BMUXlib_TS_P_DataType_eCDB == pstCurrentTransportDescriptorMetaData->eDataType )
   {
      pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount = 1;
      pstCurrentTransportDescriptorMetaData->uiInputIndex = uiInputIndex;
   }

   BERR_TRACE( BMUXlib_List_Remove(
            hMuxTS->hTransportDescriptorFreeList,
            &pEntry
            ) );

   pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;

   /* Populate Transport Descriptor */
   BKNI_Memset(
            pstCurrentTransportDescriptor,
            0,
            sizeof( BMUXlib_TS_TransportDescriptor )
            );

   /* Set Buffer Info */
   pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = true;
   pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = uiESCR;

   pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
   pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = 0;

   if ( uiTransportChannelIndex == hMuxTS->status.stInput.system.uiTransportChannelIndex )
   {
      pstCurrentTransportDescriptorMetaData->pBufferAddress = hMuxTS->status.pNullTSPacketBuffer;
      pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
      pstCurrentTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
      pstCurrentTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
      pstCurrentTransportDescriptor->uiBufferLength = sizeof( BMUXlib_TS_P_TSPacket );
   }
   else
   {
      pstCurrentTransportDescriptorMetaData->pBufferAddress = hMuxTS->status.pDummyPESBuffer;
      pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
      pstCurrentTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
      pstCurrentTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
      pstCurrentTransportDescriptor->uiBufferLength = sizeof( BMUXlib_TS_P_BPPData );
   }

   BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
      hMuxTS,
      uiTransportChannelIndex,
      pstCurrentTransportDescriptor,
      pstCurrentTransportDescriptorMetaData
      ) );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_ProcessSystemData(
         BMUXlib_TS_Handle hMuxTS
         )
{
   BMUXlib_TS_P_TSPacket *pstPCRPacket = NULL;
   BMUXlib_TS_TransportDescriptor *pstCurrentTransportDescriptor = NULL;
   BMUXlib_TS_P_TransportDescriptorMetaData *pstCurrentTransportDescriptorMetaData = NULL;
   uint32_t uiLastPendingESCR = 0xFFFFFFFF;
   bool bLastPendingESCRValid = false;

   BDBG_ENTER( BMUXlib_TS_P_ProcessSystemData );

   BDBG_ASSERT( hMuxTS );

   if ( BMUXlib_List_IsFull ( hMuxTS->hTransportDescriptorPendingList[hMuxTS->status.stInput.system.uiTransportChannelIndex] ) )
   {
      if ( false == hMuxTS->status.stStartSettings.bNonRealTimeMode )
      {
         if ( false == hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].bStalled )
         {
            BDBG_ERR(("TRANSPORT[%d] Stalled...ignoring SYSTEM[%d]", hMuxTS->status.stInput.system.uiTransportChannelIndex, 0));
         }
      }

      hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].bStalled = true;

      if ( false == hMuxTS->status.stStartSettings.bNonRealTimeMode )
      {
         return BERR_TRACE( BERR_UNKNOWN );
      }
      else
      {
         return BERR_TRACE( BERR_SUCCESS );
      }
   }
   else
   {
      hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].bStalled = false;
   }

   /* Determine how many PCRs we need to generate based on the
    * largest of the last pending ESCRs in the audio/video data */
    /* FIXME: This should be max of (last pending ESCR, starting ESCR + mux service period) */
   {
      uint32_t uiTransportChannelIndex;
      for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
      {
         if ( ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bActive )
              && ( false == BMUXlib_List_IsEmpty(
                     hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex]
                  )
              )
              && ( uiTransportChannelIndex != hMuxTS->status.stInput.system.uiTransportChannelIndex )
            )
         {
            if ( /* Use largest ESCR */
                 ( ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR > uiLastPendingESCR )
                      && ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR - uiLastPendingESCR ) < 0x80000000 ) )
                   || ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR < uiLastPendingESCR )
                        && ( ( uiLastPendingESCR - hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR) > 0x80000000 ) ) )
                 || ( false == bLastPendingESCRValid )
               )
               {
                  uiLastPendingESCR = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR;
                  bLastPendingESCRValid = true;
               }
         }
      }
   }

   if ( ( true == bLastPendingESCRValid )
        && ( true == hMuxTS->status.stPCRInfo.bInitialized )
      )
   {
      /* Ticks per packet for system data
         We use 188 bytes, because we are always sending complete TS packets on the system data channel */
      uint64_t uiPacket2PacketTimestampDelta = ((uint64_t)BMUXlib_TS_P_TSPacket_MAXSIZE * 8 * 27000000) / hMuxTS->status.stMuxSettings.uiSystemDataBitRate;

      while ( ( false == BMUXlib_List_IsEmpty(
               hMuxTS->hTransportDescriptorFreeList
               )
           )
           &&
           ( false == BMUXlib_List_IsEmpty(
                    hMuxTS->hTSPacketFreeList
                    )
           )
           &&
           ( false == BMUXlib_List_IsFull(
                    hMuxTS->hTransportDescriptorPendingList[hMuxTS->status.stInput.system.uiTransportChannelIndex]
                    )
           )
           /* FIXME: In here make sure we use max of these last pending ESCRs and starting ESCR + Mux service period to ensure we have enough time to insert
              userdata even if video frames are very short */
           && ((((hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF) < uiLastPendingESCR) /* Next PCR's ESCR is smaller than last pending ESCR */
                 && ((uiLastPendingESCR - (hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF) < 0x80000000))) /* But not too small (E.g. ESCR = 0xFFFFFFFF and next PCR = 0x00000000, we don't want to schedule it yet */
               || (((hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF) >= uiLastPendingESCR) /* Next PCR's ESCR is larger than the last pending ESCR */
                 && (((hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF) - uiLastPendingESCR) > 0x80000000)) /* But much larger (E.g. ESCR = 0x00000000 and next PCR = 0xFFFFFFFF, we DO want to schedule it now */
           )
         )

      {
         void *pEntry = NULL;
         /* calculate Next PCR's expected ESCR value for future reference */
         uint64_t uiNextESCR = hMuxTS->status.stPCRInfo.uiESCR + (BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * hMuxTS->status.stStartSettings.stPCRData.uiInterval);

         if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
         {
            /* SW7425-659: Check for resource availability for MUX_TIMESTAMP_UPDATE BTP */

            unsigned uiNumTransportDescriptorsFree;
            unsigned uiNumTSPacketsFree;
            unsigned uiNumPendingFree;

            BMUXlib_List_GetNumEntries(
                     hMuxTS->hTransportDescriptorFreeList,
                     &uiNumTransportDescriptorsFree
                     );

            BMUXlib_List_GetNumEntries(
                     hMuxTS->hTSPacketFreeList,
                     &uiNumTSPacketsFree
                     );

            BMUXlib_List_GetNumFree(
                     hMuxTS->hTransportDescriptorPendingList[hMuxTS->status.stInput.system.uiTransportChannelIndex],
                     &uiNumPendingFree
                     );

            if ( ( uiNumTransportDescriptorsFree < 2 ) || ( uiNumTSPacketsFree < 2 ) || ( uiNumPendingFree < 2 ) )
            {
               /* We don't have room for MUX_TIMESTAMP_UPDATE BTP and a PCR, so wait until the next iteration */
               break;
            }
         }

         /* Handle MOD300 wrap */
         hMuxTS->status.stPCRInfo.uiNextESCR = BMUXLIB_TS_P_MOD300_WRAP(uiNextESCR);

         /* Insert PCR */
         BERR_TRACE( BMUXlib_List_Remove(
                  hMuxTS->hTSPacketFreeList,
                  &pEntry
                  ) );

         pstPCRPacket = (BMUXlib_TS_P_TSPacket *) pEntry;

#if 0
         BDBG_ERR(("PCR --> %08x/%08x "BDBG_UINT64_FMT" ("BDBG_UINT64_FMT") "BDBG_UINT64_FMT" @%p",
            (unsigned)(hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF), uiLastPendingESCR,
            BDBG_UINT64_ARG(hMuxTS->status.stPCRInfo.uiESCR), BDBG_UINT64_ARG(hMuxTS->status.stPCRInfo.uiBase),
            BDBG_UINT64_ARG(((uint64_t)hMuxTS->status.stPCRInfo.uiBase)*300+hMuxTS->status.stPCRInfo.uiExtension),pstPCRPacket ));
#endif

         /* Populate the PCR packet */
         BKNI_Memcpy(
                  pstPCRPacket->data,
                  &s_stDefaultTSPacket,
                  sizeof( BMUXlib_TS_P_TSPacket )
                  );

         BMUXlib_TS_P_TSPacket_SetPID(
                  pstPCRPacket->data,
                  hMuxTS->status.stStartSettings.stPCRData.uiPID
                  );

         BMUXlib_TS_P_TSPacket_SetAdaptationPresent(
                  pstPCRPacket->data,
                  true
                  );

         BMUXlib_TS_P_TSPacket_SetAdaptationLength(
                  pstPCRPacket->data,
                  183 /* Always 183 for TS packets that contain only adaptation and no payload */
                  );

         BMUXlib_TS_P_TSPacket_SetPCRPresent(
                  pstPCRPacket->data,
                  true
                  );

         BMUXlib_TS_P_TSPacket_SetPCRBase(
                  pstPCRPacket->data,
                  hMuxTS->status.stPCRInfo.uiBase
                  );

         BMUXlib_TS_P_TSPacket_SetPCRExtension(
                  pstPCRPacket->data,
                  hMuxTS->status.stPCRInfo.uiExtension
                  );

         BERR_TRACE( BMUXlib_List_Remove(
                  hMuxTS->hTransportDescriptorMetaDataFreeList,
                  &pEntry
                  ) );

         pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

         /* Populate Transport Meta Data */
         BKNI_Memset(
                  pstCurrentTransportDescriptorMetaData,
                  0,
                  sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
                  );

         pstCurrentTransportDescriptorMetaData->eDataType = BMUXlib_TS_P_DataType_ePCRPacket;
         pstCurrentTransportDescriptorMetaData->eSourceType = BMUXlib_TS_P_SourceType_eSystem;

         BERR_TRACE( BMUXlib_List_Remove(
                  hMuxTS->hTransportDescriptorFreeList,
                  &pEntry
                  ) );

         pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;
         BDBG_ASSERT(pstCurrentTransportDescriptor);

         /* Populate Transport Descriptor */
         /* coverity[deref_ptr_in_call] */
         BKNI_Memset(
                  pstCurrentTransportDescriptor,
                  0,
                  sizeof( BMUXlib_TS_TransportDescriptor )
                  );

         /* Set ESCR */
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = true;
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF;

         /* Set Packet 2 Packet Timestamp Delta */
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
         pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = uiPacket2PacketTimestampDelta;

         /* Set Buffer Info */
         pstCurrentTransportDescriptorMetaData->pBufferAddress = (void *) pstPCRPacket;
         pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
         pstCurrentTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
         pstCurrentTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
         pstCurrentTransportDescriptor->uiBufferLength = sizeof( BMUXlib_TS_P_TSPacket );

         if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
         {
            /* SW7425-659: Insert MUX_TIMESTAMP_UPDATE BTP for ATS OFFSET */

            if ( ( false == hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.bValid )
                 || ( uiPacket2PacketTimestampDelta !=
                    hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.uiValue )
                 || ( false == hMuxTS->status.bBTPSent )
               )
            {
#if BDBG_DEBUG_BUILD
               unsigned uiPreviousPacket2PacketTimestampDelta = hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.uiValue;
#endif
               hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.bValid = true;
               hMuxTS->status.stOutput.stTransport[hMuxTS->status.stInput.system.uiTransportChannelIndex].stPacket2PacketDelta.uiValue = uiPacket2PacketTimestampDelta;

               BMUXlib_TS_P_UpdateTotalPacketTimestampDelta(
                  hMuxTS
                  );

               BDBG_MODULE_MSG( BMUXLIB_TS_MTU, ("Transport[%d] UPD (ESCR=%08x, PKT2PKT=(%08x-->%08x), TOTAL=(%08x-->%08x (%06x Base + %03x Ext)))",
                  hMuxTS->status.stInput.system.uiTransportChannelIndex,
                  (uint32_t) (hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF),
                  uiPreviousPacket2PacketTimestampDelta,
                  (uint32_t) uiPacket2PacketTimestampDelta,
                  hMuxTS->status.uiPreviousPacket2PacketTimestampDelta,
                  hMuxTS->status.uiTotalPacket2PacketTimestampDelta,
                  hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 >> 9,
                  hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 & 0x1FF
                  ));

               /* SW7425-659: Send MTU BTP one time to ensure the system data's deltaATS is non-zero
                * before the first A/V frame has been muxed
                */
               if ( false == hMuxTS->status.bBTPSent )
               {
                  BMUXlib_TS_P_TSPacket *pBTPBuffer = NULL;
                  BMUXlib_TS_P_TransportDescriptorMetaData *pstBTPTransportDescriptorMetaData = NULL;
                  BMUXlib_TS_TransportDescriptor *pstBTPTransportDescriptor = NULL;


                  /* Insert MUX_TIMESTAMP_UPDATE BTP */
                  BERR_TRACE( BMUXlib_List_Remove(
                           hMuxTS->hTSPacketFreeList,
                           &pEntry
                           ) );

                  pBTPBuffer = (BMUXlib_TS_P_TSPacket *) pEntry;

                  /* Populate BTP packet */
                  BKNI_Memcpy(
                           pBTPBuffer->data,
                           &s_stDefaultBTPPacket,
                           sizeof( BMUXlib_TS_P_TSPacket )
                           );

                  BMUXlib_TS_P_BTPData_SetControlWord(
                     pBTPBuffer->data,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND_OFFSET,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_COMMAND /* MTU BTP/BPP op-code*/
                     );

                  BMUXlib_TS_P_BTPData_SetControlWord(
                     pBTPBuffer->data,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_TIMESTAMP_CTRL_OFFSET,
                     0x2 /* Only PKT2PKT_DELTA_TIMESTAMP in BPP is valid */
                     );

                  BMUXlib_TS_P_BTPData_SetControlWord(
                     pBTPBuffer->data,
                     BMUXlib_TS_P_MUX_TIMESTAMP_UPDATE_PKT2PKT_DELTA_TIMESTAMP_OFFSET,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300
                     );

                  BERR_TRACE( BMUXlib_List_Remove(
                           hMuxTS->hTransportDescriptorMetaDataFreeList,
                           &pEntry
                           ) );

                  pstBTPTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

                  /* Populate Transport Meta Data */
                  BKNI_Memset(
                           pstBTPTransportDescriptorMetaData,
                           0,
                           sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
                           );

                  pstBTPTransportDescriptorMetaData->eDataType = BMUXlib_TS_P_DataType_ePCRPacket;
                  pstBTPTransportDescriptorMetaData->eSourceType = BMUXlib_TS_P_SourceType_eSystem;

                  BERR_TRACE( BMUXlib_List_Remove(
                           hMuxTS->hTransportDescriptorFreeList,
                           &pEntry
                           ) );

                  pstBTPTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;
                  BDBG_ASSERT(pstBTPTransportDescriptor);

                  /* Populate Transport Descriptor */
                  /* coverity[deref_ptr_in_call] */
                  BKNI_Memset(
                           pstBTPTransportDescriptor,
                           0,
                           sizeof( BMUXlib_TS_TransportDescriptor )
                           );

                  /* Set ESCR */
                  pstBTPTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = true;
                  pstBTPTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF;

                  /* Set Packet 2 Packet Timestamp Delta */
                  pstBTPTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
                  pstBTPTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = 0;

                  /* Set Buffer Info */
                  pstBTPTransportDescriptorMetaData->pBufferAddress = (void *) pBTPBuffer->data;
                  pstBTPTransportDescriptorMetaData->hBufferBaseBlock = hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].hBlock;
                  pstBTPTransportDescriptor->uiBufferOffset = (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress - (unsigned) hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].pBuffer;
                  pstBTPTransportDescriptor->uiBufferOffset += hMuxTS->stSubHeap[BMUXlib_TS_P_MemoryType_eShared].uiBufferOffset;
                  pstBTPTransportDescriptor->uiBufferLength = sizeof( BMUXlib_TS_P_TSPacket );

                  BDBG_MODULE_MSG( BMUXLIB_TS_MTU, ("Transport[%d] BTP (ESCR=%08x,                                TOTAL=(%08x-->%08x (%06x Base + %03x Ext)))",
                     hMuxTS->status.stInput.system.uiTransportChannelIndex,
                     (uint32_t) (hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF),
                     hMuxTS->status.uiPreviousPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDelta,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 >> 9,
                     hMuxTS->status.uiTotalPacket2PacketTimestampDeltaMod300 & 0x1FF
                     ));

                  hMuxTS->status.uiPreviousPacket2PacketTimestampDelta = hMuxTS->status.uiTotalPacket2PacketTimestampDelta;

                  BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
                     hMuxTS,
                     hMuxTS->status.stInput.system.uiTransportChannelIndex,
                     pstBTPTransportDescriptor,
                     pstBTPTransportDescriptorMetaData
                     ) );

                  /* Adjust transport descriptor to not set ESCR, since ESCR is set by the BTP packet */
                  pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = false;

                  hMuxTS->status.bBTPSent = true;
               }
            }
         }

         BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
            hMuxTS,
            hMuxTS->status.stInput.system.uiTransportChannelIndex,
            pstCurrentTransportDescriptor,
            pstCurrentTransportDescriptorMetaData
            ) );

#if BMUXLIB_TS_P_DUMP_PCR
         if ( NULL == hMuxTS->status.stPCRInfo.hPCRFile )
         {
            char fname[256];
            sprintf(fname, "BMUXlib_OUTPUT_PCR.csv");
            hMuxTS->status.stPCRInfo.hPCRFile = fopen(fname, "wb");
            if ( NULL == hMuxTS->status.stPCRInfo.hPCRFile )
            {
               BDBG_ERR(("Error Creating PCR Dump File (%s)", fname));
            }
            fprintf(
               hMuxTS->status.stPCRInfo.hPCRFile,
               "escr,pkt2pkt_delta,pcr_base,pcr_ext,desc_addr,buffer_addr,length\n"
               );
         }

         if ( hMuxTS->status.stPCRInfo.hPCRFile )
         {
            fprintf(
               hMuxTS->status.stPCRInfo.hPCRFile,
               "%u,%u,%llu,%u,%u,%u,%u\n",
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
               hMuxTS->status.stPCRInfo.uiBase,
               hMuxTS->status.stPCRInfo.uiExtension,
               (unsigned) pstCurrentTransportDescriptor,
               (unsigned) pstCurrentTransportDescriptorMetaData->pBufferAddress,
               pstCurrentTransportDescriptor->uiBufferLength
               );
         }
#endif

         /* system data starting ESCR = ESCR of PCR + delta(PCR) */
            hMuxTS->status.stSystemDataInfo.uiESCR = ( hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF );
         hMuxTS->status.stSystemDataInfo.uiESCR += uiPacket2PacketTimestampDelta;   /* only one packet sent for PCR */

         /* After inserting the PCR, we want to schedule as many userdata and system data buffer packets in between that we can */
         /* determine how many packets we can fit between PCRs ...
            (-1 to account for the PCR packet itself) */
         hMuxTS->status.stSystemDataInfo.uiPacketsUntilNextPCR =
                  ((hMuxTS->status.stPCRInfo.uiNextESCR - hMuxTS->status.stPCRInfo.uiESCR) / uiPacket2PacketTimestampDelta) - 1;
         if ((0 == hMuxTS->status.stSystemDataInfo.uiPacketsUntilNextPCR) &&
             (false == BMUXlib_List_IsEmpty(hMuxTS->hSystemDataPendingList)))
            BDBG_WRN(("Insufficient System Data bitrate to insert system data packets"));

         /* if there is userdata, schedule it after the PCR ... */
         BMUXlib_TS_P_Userdata_SchedulePackets(hMuxTS);

         /* if there is still space, schedule system data ... */
         while ( ( hMuxTS->status.stSystemDataInfo.uiPacketsUntilNextPCR != 0 ) /* We have space in between PCR packets for more system data */
                    && ( false == BMUXlib_List_IsEmpty( /* We have transport descriptors available */
                             hMuxTS->hTransportDescriptorFreeList
                             )
                       )
                    && ( false == BMUXlib_List_IsEmpty( /* We have system data available */
                             hMuxTS->hSystemDataPendingList
                             )
                         )
                    && ( false == BMUXlib_List_IsFull(
                             hMuxTS->hTransportDescriptorPendingList[hMuxTS->status.stInput.system.uiTransportChannelIndex]
                         )
                       )
                  )
         {
            BMUXlib_TS_P_SystemDataInfo *pSystemDataInfo = &hMuxTS->status.stSystemDataInfo;
            uint32_t uiNextPCRESCR = hMuxTS->status.stPCRInfo.uiNextESCR;
            uint32_t uiPacketsNeeded;
            uint32_t uiPacketsToMux;
            BMUXlib_TS_SystemData *pstSystemDataBuffer = NULL;
            void *pEntry = NULL;
            uint32_t uiMinInsertionTime = pSystemDataInfo->uiESCR;

            BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Curr ESCR: %x, Next PCR: %x", pSystemDataInfo->uiESCR, uiNextPCRESCR));

            BMUXlib_List_GetHead(
                     hMuxTS->hSystemDataPendingList,
                     &pEntry
                     );

            pstSystemDataBuffer = (BMUXlib_TS_SystemData *) pEntry;

            /* SW7425-1188: Pace system data insertion according to specified timestamp delta */
            /* if first system data packet, insert it straight away regardless of its timestamp delta */
            if (pSystemDataInfo->bLastScheduledESCRValid)
            {
               uiMinInsertionTime = pSystemDataInfo->uiLastScheduledESCR +
                     (pstSystemDataBuffer->uiTimestampDelta * BMUXLIB_TS_P_SCALE_MS_TO_27MHZ);

               BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Delta = %x, Min Insertion = %x, Last Sched = %x",
                  pstSystemDataBuffer->uiTimestampDelta, uiMinInsertionTime, pSystemDataInfo->uiLastScheduledESCR));

               /* check if min insertion time < next PCR insertion time (accounting for wrap) */
               if ((int32_t)(uiNextPCRESCR - uiMinInsertionTime) > 0)
               {
                  /* see if the insertion of this data needs to be delayed within the current PCR interval */
                  if ((int32_t)(pSystemDataInfo->uiESCR - uiMinInsertionTime) < 0)
                  {
                     /* time to insertion point of this system data is min insertion time - current time */
                     uint32_t uiTimeToInsertionPoint = uiMinInsertionTime - pSystemDataInfo->uiESCR;
                     /* number of packets required to reach the desired insertion point (rounded
                        up to the next packet boundary) */
                     uint32_t uiPacketsToInsertionPoint = uiTimeToInsertionPoint / uiPacket2PacketTimestampDelta;

                     BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Packets to Insertion = %d (packets to PCR = %d), Time to insertion = %x, dP2P = %x",
                           uiPacketsToInsertionPoint, pSystemDataInfo->uiPacketsUntilNextPCR, uiTimeToInsertionPoint, uiPacket2PacketTimestampDelta));

                     if (uiPacketsToInsertionPoint >= pSystemDataInfo->uiPacketsUntilNextPCR)
                     {
                        /* no room for this data in this interval */
                        break; /* no need to update system data ESCR - it will get reset next interval based on PCR */
                     }
                     /* this is effectively inserting a "gap" between the PCR and the outgoing system data */
                     pSystemDataInfo->uiPacketsUntilNextPCR -= uiPacketsToInsertionPoint;
                     pSystemDataInfo->uiESCR += (uiPacketsToInsertionPoint * uiPacket2PacketTimestampDelta);
                     BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Updated: Packets to next PCR = %d, sys data ESCR = %x",
                        pSystemDataInfo->uiPacketsUntilNextPCR, pSystemDataInfo->uiESCR));
                  }
                  else
                  {
                     /* else, min insertion time <= current time
                        => this insertion needs to be scheduled now:
                        * incomplete system data from previous insertion, OR
                        * data that did not fit in previous interval, OR
                        * specified timestamp delta was smaller than the previous system data length, OR
                        * data that was to be scheduled at same time as PCR
                        So, insert this data now */
                     uiMinInsertionTime = pSystemDataInfo->uiESCR;    /* current insertion time */
                  }
               }
               else
               {
                  /* do not insert this system data yet - not time to insert it yet ... wait */
                  BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Waiting for system data insertion"));
                  break;
               }
            }

            pSystemDataInfo->uiLastScheduledESCR = uiMinInsertionTime;
            pSystemDataInfo->bLastScheduledESCRValid = true;
            /* ------------- end: SW7425-1188 ------------- */

            /* Compute how many packets we need to mux the next system data buffer */
            uiPacketsNeeded = ( pstSystemDataBuffer->uiSize / BMUXlib_TS_P_TSPacket_MAXSIZE ) - ( pSystemDataInfo->uiPacketsMuxedSoFar );
            uiPacketsToMux = ( uiPacketsNeeded < pSystemDataInfo->uiPacketsUntilNextPCR ) ? ( uiPacketsNeeded ) : pSystemDataInfo->uiPacketsUntilNextPCR;
            BDBG_MODULE_MSG(BMUXLIB_TS_SYSDATA, ("Packets Needed = %d, Packets to Mux = %d", uiPacketsNeeded, uiPacketsToMux));

            BERR_TRACE( BMUXlib_List_Remove(
                     hMuxTS->hTransportDescriptorMetaDataFreeList,
                     &pEntry
                     ) );

            pstCurrentTransportDescriptorMetaData = (BMUXlib_TS_P_TransportDescriptorMetaData *) pEntry;

            /* Populate Transport Meta Data */
            BKNI_Memset(
                     pstCurrentTransportDescriptorMetaData,
                     0,
                     sizeof( BMUXlib_TS_P_TransportDescriptorMetaData )
                     );

            pstCurrentTransportDescriptorMetaData->eDataType = BMUXlib_TS_P_DataType_eCDB;
            pstCurrentTransportDescriptorMetaData->eSourceType = BMUXlib_TS_P_SourceType_eSystem;

            BERR_TRACE( BMUXlib_List_Remove(
                     hMuxTS->hTransportDescriptorFreeList,
                     &pEntry
                     ) );

            pstCurrentTransportDescriptor = (BMUXlib_TS_TransportDescriptor *) pEntry;
            BDBG_ASSERT(pstCurrentTransportDescriptor);

            /* Populate Transport Descriptor */
            /* coverity[deref_ptr_in_call] */
            BKNI_Memset(
                     pstCurrentTransportDescriptor,
                     0,
                     sizeof( BMUXlib_TS_TransportDescriptor )
                     );

            /* Set ESCR */
            pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid = true;
            pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp = pSystemDataInfo->uiESCR;

            /* Set Packet 2 Packet Timestamp Delta */
            {
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid = true;
               pstCurrentTransportDescriptor->stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta = uiPacket2PacketTimestampDelta;
            }

            /* Set Buffer Info */
            pstCurrentTransportDescriptorMetaData->hBufferBaseBlock = pstSystemDataBuffer->hBlock;
            pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset = BMMA_LockOffset( pstSystemDataBuffer->hBlock );
            pstCurrentTransportDescriptor->uiBufferOffset = pstCurrentTransportDescriptorMetaData->uiBufferBaseOffset + pstSystemDataBuffer->uiBlockOffset + pSystemDataInfo->uiPacketsMuxedSoFar * BMUXlib_TS_P_TSPacket_MAXSIZE;
            pstCurrentTransportDescriptor->uiBufferLength = uiPacketsToMux * BMUXlib_TS_P_TSPacket_MAXSIZE;

            BERR_TRACE( BMUXlib_TS_P_AddTransportDescriptor(
               hMuxTS,
               hMuxTS->status.stInput.system.uiTransportChannelIndex,
               pstCurrentTransportDescriptor,
               pstCurrentTransportDescriptorMetaData
            ) );

            /* Compute the ESCR of the next system data packet */
            /* coverity[check_after_deref] */
            if ( NULL != pstCurrentTransportDescriptor )
            {
               pSystemDataInfo->uiESCR += ( uiPacket2PacketTimestampDelta * uiPacketsToMux );
            }
            BDBG_ASSERT((int32_t)((uint32_t)(hMuxTS->status.stPCRInfo.uiNextESCR & 0xFFFFFFFF) - pSystemDataInfo->uiESCR) >= 0);

            /* Bookkeeping */
            pSystemDataInfo->uiPacketsMuxedSoFar += uiPacketsToMux;
            pSystemDataInfo->uiPacketsUntilNextPCR -= uiPacketsToMux;
            if ( pSystemDataInfo->uiPacketsMuxedSoFar == ( pstSystemDataBuffer->uiSize / BMUXlib_TS_P_TSPacket_MAXSIZE ) )
            {
               BERR_TRACE( BMUXlib_List_Remove(
                        hMuxTS->hSystemDataPendingList,
                        (void**) &pstSystemDataBuffer
                        ) );
               pSystemDataInfo->uiPacketsMuxedSoFar = 0;
               pstCurrentTransportDescriptorMetaData->uiSourceDescriptorCount = 1;
            }
         } /* end: while system data to schedule */
         /* Set next ESCR and calculate relevant PCRBase/Extension */
         hMuxTS->status.stPCRInfo.uiESCR = hMuxTS->status.stPCRInfo.uiNextESCR;
         hMuxTS->status.stPCRInfo.uiBase = BMUXLIB_TS_P_GET_PCR_BASE(hMuxTS->status.stPCRInfo.uiESCR);
         hMuxTS->status.stPCRInfo.uiExtension = BMUXLIB_TS_P_GET_PCR_EXT(hMuxTS->status.stPCRInfo.uiESCR);
      } /* end: while PCRs to insert */
   } /* end: if PCR and last pending ESCR valid */

   BDBG_LEAVE( BMUXlib_TS_P_ProcessSystemData );

   return BERR_TRACE( BERR_SUCCESS );
}

void
BMUXlib_TS_S_StallTransport(
   BMUXlib_TS_Handle hMuxTS,
   unsigned uiTransportChannelIndex,
   bool bStalled
   )
{
   hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bStalled = bStalled;
}

BERR_Code
BMUXlib_TS_P_ProcessNewBuffers(
   BMUXlib_TS_Handle hMuxTS
   )
{
   BMUXlib_Input_Handle hSelectedInput = NULL;
   uint32_t uiInputIndex;
   BMUXlib_InputGroup_Status stStatus;
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BMUXlib_TS_P_ProcessNewBuffers );

   /* Reset all inputs to be available if resources are available */
   for ( uiInputIndex = 0; uiInputIndex < hMuxTS->status.uiNumInputs; uiInputIndex++ )
   {
      if ( false == hMuxTS->status.stInputMetaData[uiInputIndex].bEOSSeen )
      {
         uint32_t uiTransportChannelIndex = hMuxTS->status.stInputMetaData[uiInputIndex].uiTransportChannelIndex;

         if ( true == BMUXlib_List_IsFull( hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex] ) )
         {
            /* disable this input (do not consider this input for selection until the next MSP) */
            BMUXlib_TS_S_StallTransport( hMuxTS, uiTransportChannelIndex, true );
            rc = BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hMuxTS->status.stInputMetaData[uiInputIndex].hInput, false);
            if (rc != BERR_SUCCESS)
               /* this should always succeed */
               return rc;

            if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
            {
               /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                * order, if an input is stalled, we don't want to process any inputs.
                */
               return BERR_SUCCESS;
            }
         }
         else
         {
            /* enable this input */
            BMUXlib_TS_S_StallTransport( hMuxTS, uiTransportChannelIndex, false );
            rc = BMUXlib_Input_ProcessNewDescriptors( hMuxTS->status.stInputMetaData[uiInputIndex].hInput );
            if (rc != BERR_SUCCESS)
               return rc;
            rc = BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hMuxTS->status.stInputMetaData[uiInputIndex].hInput, true);
            if (rc != BERR_SUCCESS)
               /* this should always succeed */
               return rc;
         }
      }
   }

   while ( false == BMUXlib_List_IsEmpty( /* We have transport descriptors available */
                  hMuxTS->hTransportDescriptorFreeList
                  ) )
   {
      BMUXlib_InputGroup_GetStatus(
         hMuxTS->status.hInputGroup,
         &stStatus);

      if ( 0 == stStatus.uiNumActiveInputs )
      {
         break;
      }


      /* In NRT mode, on startup, we want to wait for all inputs to have data to prevent starting an input prematurely
       * and then getting data on the other input whose transmission time is in the past */
      {
         BMUXlib_InputGroup_Settings stSettings;
         BMUXlib_InputGroup_GetSettings(hMuxTS->status.hInputGroup, &stSettings);

         if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
         {
            /* SW7425-659: To ensure MUX_TIMESTAMP_UPDATE is sent synchronously with bitrate changes, we need
             * to wait for all inputs before processing any inputs to prevent out of order ESCR processing
             * across inputs.
             */
            stSettings.bWaitForAllInputs = true;
         }
         else
         {
            stSettings.bWaitForAllInputs = ( true == hMuxTS->status.stStartSettings.bNonRealTimeMode ) && ( false == hMuxTS->status.bAllInputsReady );
         }

         rc = BMUXlib_InputGroup_SetSettings(hMuxTS->status.hInputGroup, &stSettings);
         if (rc != BERR_SUCCESS)
            return rc;

         BMUXlib_InputGroup_GetNextInput(hMuxTS->status.hInputGroup, &hSelectedInput);

         if ( ( true == stSettings.bWaitForAllInputs )
              && ( NULL != hSelectedInput ) )
         {
            hMuxTS->status.bAllInputsReady = true;
         }
      }

      if ( NULL != hSelectedInput )
      {
         BMUXlib_Input_Descriptor stDescriptor;
         BMUXlib_TS_P_InputMetaData *pInputMetadata;
         /* don't process userdata until determined otherwise
            (i.e. only on video and only on start of frame) */
         bool bProcessUserdata = false;
         bool bDescAvail;
         BMUXlib_Input_CreateSettings stSettings;

         /* fetch the source metadata pointer for this input */
         BMUXlib_Input_GetCreateSettings(hSelectedInput, &stSettings);
         pInputMetadata = (BMUXlib_TS_P_InputMetaData *)stSettings.pMetadata;
         BDBG_ASSERT(pInputMetadata != NULL);

         /* NOTE: The following should not fail, since an input would not be selected if there
            is no descriptor available */
         bDescAvail = BMUXlib_Input_PeekAtNextDescriptor( hSelectedInput, &stDescriptor );
         BDBG_ASSERT(bDescAvail == true);

         /* NOTE: The following processing order relies on the fact that the first descriptor with
          * a valid ESCR *and* PTS is either an Audio or Video descriptor. */

         /* We want to initialize the first PCR value and ESCR based on the first A/V buffer we're muxing */
         if ( BMUXLIB_INPUT_DESCRIPTOR_IS_ESCR_VALID( &stDescriptor ) )
         {
            if ( ( false == hMuxTS->status.stPCRInfo.bInitialized )
                 && ( BMUXLIB_INPUT_DESCRIPTOR_IS_PTS_VALID( &stDescriptor ) )
               )
            {
               /* SW7435-1829: We are using the ESCR/PTS of the first input descriptor to seed the PCR value */
               uint32_t uiTargetESCR = BMUXLIB_INPUT_DESCRIPTOR_ESCR( &stDescriptor );
               uint64_t uiTargetPTSin27Mhz = BMUXLIB_INPUT_DESCRIPTOR_PTS( &stDescriptor ) * 300;
               uint32_t uiPCRInterval = BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * hMuxTS->status.stStartSettings.stPCRData.uiInterval;

               /* determine the desired "pre-offset" prior to the first media
                  set the PCRs to go out 3 PCR intervals prior to the first media, unless this exceeds 1 MSP
                  (we'd run out of storage if we exceed 1 service period)
                  NOTE: This is done to ensure we have at least 2 PCRs *prior* to the media to allow time
                  for decoder lock to PCRs (if this is not the case, the first GOP will likely be lost) */
               uint32_t uiMaxDiff = 3 * uiPCRInterval;
               if (uiMaxDiff > (BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * hMuxTS->status.stStartSettings.uiServicePeriod))
                  uiMaxDiff = (BMUXLIB_TS_P_SCALE_MS_TO_27MHZ * hMuxTS->status.stStartSettings.uiServicePeriod);

               /* We compute the final target ESCR by subtracting the "pre-offset" */
               uiTargetESCR -= uiMaxDiff;

               {
                  /* determine the distance (offset) between the PTS and the Target ESCR (modulo-32-bits) */
                  uint32_t uiOffset = (uint32_t)(uiTargetPTSin27Mhz & 0xFFFFFFFF) - uiTargetESCR;

                  hMuxTS->status.stPCRInfo.uiESCR = BMUXLIB_TS_P_MOD300_WRAP(uiTargetPTSin27Mhz - uiOffset);
               }

               BDBG_MODULE_MSG(BMUXLIB_TS_PCR, ("Using initial ESCR/PTS (%08x/%03x%08x) --> %03x%08x",
                  BMUXLIB_INPUT_DESCRIPTOR_ESCR( &stDescriptor ),
                  (uint32_t) (uiTargetPTSin27Mhz >> 32), (uint32_t) (uiTargetPTSin27Mhz & 0xFFFFFFFF),
                  (uint32_t) (hMuxTS->status.stPCRInfo.uiESCR >> 32), (uint32_t) (hMuxTS->status.stPCRInfo.uiESCR & 0xFFFFFFFF)));

               hMuxTS->status.stPCRInfo.uiBase = BMUXLIB_TS_P_GET_PCR_BASE(hMuxTS->status.stPCRInfo.uiESCR);
               hMuxTS->status.stPCRInfo.uiExtension = BMUXLIB_TS_P_GET_PCR_EXT(hMuxTS->status.stPCRInfo.uiESCR);
               hMuxTS->status.stPCRInfo.bInitialized = true;

               BDBG_MODULE_MSG(BMUXLIB_TS_PCR, ("Initial PCR Base/Extension: "BDBG_UINT64_FMT"/%02x", BDBG_UINT64_ARG(hMuxTS->status.stPCRInfo.uiBase), hMuxTS->status.stPCRInfo.uiExtension));
            }

            if ( false == hMuxTS->status.bFirstESCRValid )
            {
               /* TODO: We should probably wait for ALL inputs to ensure the smallest ESCR is chosen */
               hMuxTS->status.uiFirstESCR = BMUXLIB_INPUT_DESCRIPTOR_ESCR( &stDescriptor );
               hMuxTS->status.bFirstESCRValid = true;
            }
         } /* end: if ESCR valid */

         /* SW7425-75: Ignore metadata */
         if ( BMUXLIB_INPUT_DESCRIPTOR_IS_METADATA( &stDescriptor ) )
         {
            /* extract video frame rate code for userdata timing */
            if (BMUXlib_Input_Type_eVideo == BMUXLIB_INPUT_DESCRIPTOR_TYPE(&stDescriptor))
            {
               /* ensure the metatdata base address is valid, if not, ignore this descriptor */
               if (NULL == BMUXLIB_INPUT_DESCRIPTOR_BLOCK(&stDescriptor))
               {
                  BDBG_WRN(("Metadata base block not set: Ignoring video metadata descriptor ..."));
               }
               else
               {
                  /* read the data unit type to determine if we can process this metadata type */
                  if (BAVC_VideoMetadataType_eCommon == BMUXLIB_INPUT_DESCRIPTOR_VIDEO_DATA_UNIT_TYPE(&stDescriptor))
                  {
                     void *pMetadataBase = BMMA_Lock( BMUXLIB_INPUT_DESCRIPTOR_BLOCK(&stDescriptor) );
                     BAVC_VideoMetadataDescriptor *pMetadata = (BAVC_VideoMetadataDescriptor *)BMUXLIB_INPUT_DESCRIPTOR_VIRTUAL_ADDRESS(pMetadataBase, &stDescriptor);

                     pInputMetadata->eSourceFrameRateCode = (0 != (pMetadata->uiMetadataFlags & BAVC_VIDEOMETADATADESCRIPTOR_FLAGS_FRAMERATE_VALID))?
                        pMetadata->stFrameRate.eFrameRateCode: BAVC_FrameRateCode_eUnknown;

                     BMMA_Unlock( BMUXLIB_INPUT_DESCRIPTOR_BLOCK(&stDescriptor), pMetadataBase );
                  }
                  /* else: ignore unrecognized metadata type */
               }
            } /* end: video metadata */
            else if ( BMUXlib_Input_Type_eAudio == BMUXLIB_INPUT_DESCRIPTOR_TYPE(&stDescriptor) )
            {
               /* ensure the metatdata base address is valid, if not, ignore this descriptor */
               if (NULL == BMUXLIB_INPUT_DESCRIPTOR_BLOCK(&stDescriptor))
               {
                  BDBG_WRN(("Metadata base address not set: Ignoring audio metadata descriptor ..."));
               }
            } /* end: audio metadata */
            pInputMetadata->uiMetadataDescriptorsSkipped++;
         }
         else /* not a metadata descriptor */
         {
            /* Determine if required resources are available to process this input descriptor */
            unsigned uiNumTransportDescriptorsRequired = 0;
            unsigned uiNumBPPRequired = 0;
            uint32_t uiNumTransportDescriptorsFree;
            uint32_t uiNumBPPFree = 0;
            size_t uiSpaceAvailable;

            if ( BMUXLIB_INPUT_DESCRIPTOR_IS_EMPTYFRAME( &stDescriptor ) )
            {
               /* Empty frames just need a NULL descriptor to be sent */
               uiNumTransportDescriptorsRequired++;
            }
            else
            {
               if ( BMUXLIB_INPUT_DESCRIPTOR_IS_FRAMESTART( &stDescriptor ) )
               {
                  /* Frame Start requires a PES Header and Transport Descriptor */
                  uiNumTransportDescriptorsRequired++;

                  if ( true == BMUXlib_List_IsEmpty(
                                hMuxTS->hPESHeaderFreeList
                                )
                     )
                  {
                     BDBG_MSG(("PES descriptor not available.  Will be processed next time."));
                     /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                      * order, if an input is stalled, we don't want to process any inputs.
                      */
                     BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

                     if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                     {
                        /* SW7425-659: Check resource availability for MUX_TIMESTAMP_UPDATE BPP */

                        break;
                     }

                     continue;
                  }

                  if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                  {
                     /* SW7425-659: Check resource availability for MUX_TIMESTAMP_UPDATE BPP */

                     /* Frame Start requires a BPP and Transport Descriptor */
                     uiNumTransportDescriptorsRequired+=1;
                     uiNumBPPRequired+=1;
                  }

                  /* SW7425-4974: check for sufficient userdata resources */
                  if (!BMUXLIB_P_Userdata_CheckResources(hMuxTS))
                  {
                     BDBG_MSG(("Insufficient userdata resources.  Will be processed next time."));
                     BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);
                     continue;
                  }
               } /* end: is framestart */

               if ( 0 != BMUXLIB_INPUT_DESCRIPTOR_LENGTH( &stDescriptor ) )
               {
                  /* Frame Data requires only a Transport Descriptor */
                  uiNumTransportDescriptorsRequired++;
               }
            }

            if ( BMUXLIB_INPUT_DESCRIPTOR_IS_EOS( &stDescriptor ) )
            {
               /* EOS requires a BPP and a Transport Descriptor */
               uiNumTransportDescriptorsRequired++;
               uiNumBPPRequired++;
            }


            /* VERIFY at enough transport descriptors are available */
            BMUXlib_List_GetNumEntries(
                     hMuxTS->hTransportDescriptorFreeList,
                     &uiNumTransportDescriptorsFree
                     );

            if ( uiNumTransportDescriptorsFree < uiNumTransportDescriptorsRequired )
            {
               BDBG_MSG(("Transport descriptors not available.  Will be processed next time."));

               BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

               if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
               {
                  /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                   * order, if an input is stalled, we don't want to process any inputs.
                   */

                  break;
               }

               continue;
            }

            BMUXlib_List_GetNumFree( hMuxTS->hTransportDescriptorPendingList[pInputMetadata->uiTransportChannelIndex], &uiSpaceAvailable );
            if ( uiSpaceAvailable < uiNumTransportDescriptorsRequired )
            {
               BDBG_MSG(("Space not available in output.  Will be processed next time."));
               BMUXlib_TS_S_StallTransport( hMuxTS, pInputMetadata->uiTransportChannelIndex, true );
               BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

               if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
               {
                  /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                   * order, if an input is stalled, we don't want to process any inputs.
                   */

                  break;
               }

               continue;
            }

            BMUXlib_List_GetNumEntries(
               hMuxTS->hBPPFreeList,
               &uiNumBPPFree
               );

            if ( uiNumBPPFree < uiNumBPPRequired )
            {
               BDBG_MSG(("BPP not available.  Will be processed next time."));
               BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

               if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
               {
                  /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                   * order, if an BPP is not available, we don't want to process any inputs.
                   */

                  break;
               }

               continue;
            }


            /* Check to see if we need to insert a PES Header */
            if ( BMUXLIB_INPUT_DESCRIPTOR_IS_EMPTYFRAME( &stDescriptor ) )
            {
               BDBG_ASSERT( BMUXLIB_INPUT_DESCRIPTOR_IS_ESCR_VALID( &stDescriptor ) );

               /* Insert NULL */
               /* Queue the null descriptor to flush the output */
               if ( BERR_SUCCESS != BERR_TRACE(BMUXlib_TS_P_InsertNULLTransportDescriptor(
                        hMuxTS,
                        pInputMetadata->uiTransportChannelIndex,
                        BMUXLIB_INPUT_DESCRIPTOR_ESCR( &stDescriptor ),
                        BMUXlib_TS_P_DataType_eCDB,
                        BMUXLIB_TS_P_InputDescriptorTypeLUT[BMUXLIB_INPUT_DESCRIPTOR_TYPE( &stDescriptor )],
                        pInputMetadata->uiInputIndex
                        )))
               {
                  BDBG_ERR(("Transport descriptors not available.  Will be processed next time."));
                  BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

                  if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                  {
                     /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                      * order, if an input is stalled, we don't want to process any inputs.
                      */

                     break;
                  }

                  continue;
               }
            }
            else
            {
               if ( BMUXLIB_INPUT_DESCRIPTOR_IS_FRAMESTART( &stDescriptor ) )
               {
                  BMUXlib_P_PESInfo stPESInfo;

                  BMUXlib_TS_P_PopulatePESInfoFromInputDescriptor(
                           &stDescriptor,
                           &stPESInfo,
                           hMuxTS->status.stStartSettings.bSupportTTS
                           );

                  stPESInfo.uiTransportChannelIndex = pInputMetadata->uiTransportChannelIndex;
                  stPESInfo.bTransportChannelIndexValid = true;

                  stPESInfo.uiPESStreamID = pInputMetadata->uiPESStreamID;

                  /* Check to see if the following resources are available for inserting the PES header
                   *    1) Transport Descriptor
                   *    2) PES Header
                   */
                  if ( BERR_SUCCESS != BMUXlib_TS_P_InsertPESHeader(
                           hMuxTS,
                           &stPESInfo
                           )
                     )
                  {
                     BDBG_ERR(("PES descriptors not available.  Will be processed next time."));
                     BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

                     if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                     {
                        /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                         * order, if an input is stalled, we don't want to process any inputs.
                         */

                        break;
                     }

                     continue;
                  }

                  bProcessUserdata = BMUXlib_TS_P_Userdata_FindTargetPTS(hMuxTS, pInputMetadata, &stDescriptor);

                  if ( ( true == stPESInfo.bESCRValid )
                       && ( true == stPESInfo.bTicksPerBitValid )
                       && ( true == stPESInfo.bSHRValid )
                     )
                  {
                     /* ESCR Discontinuity Detection */
                     {
                           unsigned uiESCRDelta;

                           if ( hMuxTS->status.stOutput.stTransport[pInputMetadata->uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR > stPESInfo.uiESCR )
                           {
                              uiESCRDelta = hMuxTS->status.stOutput.stTransport[pInputMetadata->uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR - stPESInfo.uiESCR;
                           }
                           else
                           {
                              uiESCRDelta = stPESInfo.uiESCR - hMuxTS->status.stOutput.stTransport[pInputMetadata->uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR;
                           }

                           if ( ( 0xFFFFFFFF != hMuxTS->status.stOutput.stTransport[pInputMetadata->uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR )
                                 && ( uiESCRDelta > 27000000 * 5 )
                                 && ( uiESCRDelta < ( 0xFFFFFFFF - (27000000 * 5 ) ) ) )
                           {
                              BDBG_WRN(("ESCR Discontinuity Detected on Transport[%d]. (eESCR=%08x, aESCR=%08x, delta=%u (%u seconds)",
                                 pInputMetadata->uiTransportChannelIndex,
                                 hMuxTS->status.stOutput.stTransport[pInputMetadata->uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR,
                                 stPESInfo.uiESCR,
                                 uiESCRDelta,
                                 uiESCRDelta / 27000000
                                 ));
                           }
                     }
                  }
                  /* SW7425-4340 - Check if current timing exceeds specified A2PDelay */
                  /* NOTE: For now, this check is only valid if there are active userdata
                     PIDs.  In the future, we may require this check at any time */
                  if ((hMuxTS->status.stStartSettings.uiNumValidUserdataPIDs != 0)
                     && (true == stPESInfo.bESCRValid) && (true == stPESInfo.bDTSValid))
                  {
                     /* uiDTS is 64 bits - 33 bits @ 90KHZ
                        uiESCR is 32 bits @ 27Mhz
                        Convert uiDTS to 27Mhz (42 bits @ 27Mhz) - then trucate to 32 bits
                        32 bits @ 27MHz => Max delta of 159 seconds */
                     uint32_t uiESCRDTSDelta;
                     uint32_t uiDTSScaled = stPESInfo.uiDTS * 300;  /* scale to 27Mhz and truncate to 32 bits */
                     if (stPESInfo.uiESCR > uiDTSScaled)
                        uiESCRDTSDelta = stPESInfo.uiESCR - uiDTSScaled;
                     else
                        uiESCRDTSDelta = uiDTSScaled - stPESInfo.uiESCR;

                     if (uiESCRDTSDelta > hMuxTS->status.stStartSettings.uiA2PDelay)
                     {
                        /* Hysteresis the warning message - if increases by 200ms output another warning */
                        if (uiESCRDTSDelta > (hMuxTS->status.uiLastESCRDTSDelta + (200 * BMUXLIB_TS_P_SCALE_MS_TO_27MHZ)))
                        {
                           BDBG_WRN(("ESCR / DTS delta (%d ms) exceeds A2PDelay (%d ms) - ESCR=%08x , DTS="BDBG_UINT64_FMT,
                              uiESCRDTSDelta / BMUXLIB_TS_P_SCALE_MS_TO_27MHZ,
                              hMuxTS->status.stStartSettings.uiA2PDelay / BMUXLIB_TS_P_SCALE_MS_TO_27MHZ,
                              stPESInfo.uiESCR, BDBG_UINT64_ARG(stPESInfo.uiDTS)));
                           hMuxTS->status.uiLastESCRDTSDelta = uiESCRDTSDelta;
                        }
                     }
                  }
               } /* end: is framestart */

               /* Check to see if there is CDB data to mux */
               if ( 0 != BMUXLIB_INPUT_DESCRIPTOR_LENGTH( &stDescriptor ) )
               {
                  BMUXlib_P_TransportInfo stTransportInfo;

                  BMUXlib_TS_P_PopulateTransportInfoFromInputDescriptor(
                           &stDescriptor,
                           &stTransportInfo
                           );

                  stTransportInfo.uiTransportChannelIndex = pInputMetadata->uiTransportChannelIndex;
                  stTransportInfo.bTransportChannelIndexValid = true;

                  stTransportInfo.uiSkippedDescriptors = pInputMetadata->uiMetadataDescriptorsSkipped;
                  stTransportInfo.bSkippedDescriptorsValid = true;

                  stTransportInfo.uiInputIndex = pInputMetadata->uiInputIndex;
                  stTransportInfo.bInputIndexValid = true;

                  if ( BERR_SUCCESS != BMUXlib_TS_P_InsertTransportDescriptor(
                           hMuxTS,
                           &stTransportInfo
                           )
                     )
                  {
                     BDBG_ERR(("Transport descriptors not available!"));
                     BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

                     if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                     {
                        /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                         * order, if an input is stalled, we don't want to process any inputs.
                         */

                        break;
                     }

                     continue;
                  }

                  pInputMetadata->uiMetadataDescriptorsSkipped = 0;
               }
            }

            if ( BMUXLIB_INPUT_DESCRIPTOR_IS_EOS( &stDescriptor ) )
            {
               /* Insert LAST BPP */
               BMUXlib_P_BPPInfo stBPPInfo;

               BKNI_Memset( &stBPPInfo, 0, sizeof( BMUXlib_P_BPPInfo ) );

               stBPPInfo.uiTransportChannelIndex = pInputMetadata->uiTransportChannelIndex;
               stBPPInfo.bTransportChannelIndexValid = true;

               stBPPInfo.eSourceType = BMUXLIB_TS_P_InputDescriptorTypeLUT[BMUXLIB_INPUT_DESCRIPTOR_TYPE( &stDescriptor )];
               stBPPInfo.bSourceTypeValid = true;

               stBPPInfo.uiSkippedDescriptors = 1;
               stBPPInfo.bSkippedDescriptorsValid = true;

               stBPPInfo.uiInputIndex = pInputMetadata->uiInputIndex;
               stBPPInfo.bInputIndexValid = true;

               stBPPInfo.bESCRValid = BMUXLIB_INPUT_DESCRIPTOR_IS_ESCR_VALID( &stDescriptor );
               stBPPInfo.uiESCR = BMUXLIB_INPUT_DESCRIPTOR_ESCR( &stDescriptor );

               stBPPInfo.bPacket2PacketDeltaValid = true;
               stBPPInfo.uiPacket2PacketDelta = 0;

               BERR_TRACE( BMUXlib_List_Remove(
                        hMuxTS->hBPPFreeList,
                        &stBPPInfo.pBuffer
                        ) );

               BKNI_Memcpy(
                        stBPPInfo.pBuffer,
                        &s_stDefaultBPPData,
                        sizeof ( BMUXlib_TS_P_BPPData )
                        );

               BMUXlib_TS_P_BPPData_SetCommand(
                  stBPPInfo.pBuffer,
                  0x82); /* "LAST" BTP/BPP op-code*/

               BMUXlib_TS_P_BPPData_SetStreamID(
                           stBPPInfo.pBuffer,
                           pInputMetadata->uiPESStreamID
                           );

               stBPPInfo.uiLength = sizeof ( BMUXlib_TS_P_BPPData );

               if ( BERR_SUCCESS != BMUXlib_TS_P_InsertBPP(
                        hMuxTS,
                        &stBPPInfo
                        )
                  )
               {
                  BDBG_ERR(("Transport descriptors not available.  Will be processed next time."));
                  BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);

                  if ( true == hMuxTS->status.stStartSettings.bSupportTTS )
                  {
                     /* SW7425-659: Since MUX_TIMESTAMP_UPDATE BTP/BPP requires descriptors to be processed in ESCR sequential
                      * order, if an input is stalled, we don't want to process any inputs.
                      */

                     break;
                  }

                  continue;
               }

               BDBG_MODULE_MSG( BMUXLIB_TS_EOS, ("%s[%d] EOS Seen",
                  ( BMUXLIB_INPUT_DESCRIPTOR_TYPE( &stDescriptor ) == BMUXlib_Input_Type_eAudio ) ? "AUDIO" : "VIDEO",
                     pInputMetadata->uiInputIndex));

               pInputMetadata->bEOSSeen = true;
               BMUXlib_InputGroup_ActivateInput(hMuxTS->status.hInputGroup, hSelectedInput, false);
            } /* end: EOS Seen */
         } /* end: else not metadata */

         if (bProcessUserdata)
            if (BERR_SUCCESS != BMUXlib_TS_P_Userdata_ProcessInputs( hMuxTS ))
               BDBG_ERR(("Unable to process userdata"));

         /* Process System Data (Generate PCR and schedule system data)
            Also schedules userdata */
         BMUXlib_TS_P_ProcessSystemData( hMuxTS );

         /* discard the descriptor that was "peeked" at previously
            (this should not fail) */
         bDescAvail = BMUXlib_Input_GetNextDescriptor( hSelectedInput, &stDescriptor );
         BDBG_ASSERT(bDescAvail == true);
      } /* end: if valid input selected */
      else
      {
         /* no input ready to process */
         break;
      }
   } /* end: while descriptors to process */

   BDBG_LEAVE( BMUXlib_TS_P_ProcessNewBuffers );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BMUXlib_TS_P_ScheduleProcessedBuffers(
   BMUXlib_TS_Handle hMuxTS
   )
{
   unsigned uiTotalDescriptorsToQueue = 0;
   uint32_t uiTransportChannelIndex;
   uint32_t uiWorstCaseESCRSlip = 0;

   BDBG_ENTER( BMUXlib_TS_P_ScheduleProcessedBuffers );

   for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
   {
      if ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bActive )
       {
          /* Keep track of the worst case packet interleave conflict in the deltaESCR */
          uiWorstCaseESCRSlip += hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiCurrentPacket2PacketTimestampDelta;
       }
   }

   for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
   {
      BMUXlib_TS_TransportChannelInterface *pTransportInterface = &hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stChannelInterface;


      if ( ( NULL != pTransportInterface->pContext )
           && ( NULL != pTransportInterface->fAddTransportDescriptors )
         )
      {
         const void *pDescriptors[2];
         const BMUXlib_TS_TransportDescriptor **astTransportDescriptors[2];
         size_t uiNumTransportDescriptors[2];
         const BMUXlib_TS_P_TransportDescriptorMetaData **astTransportDescriptorsMetaData[2];
         size_t uiNumTransportDescriptorsMetaData[2];
         uint32_t uiTotalQueued = 0;
         uint32_t i;

         BMUXlib_List_GetEntries(
                  hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex],
                  &pDescriptors[0],
                  &uiNumTransportDescriptors[0],
                  &pDescriptors[1],
                  &uiNumTransportDescriptors[1]
                  );

         astTransportDescriptors[0] = (const BMUXlib_TS_TransportDescriptor **) pDescriptors[0];
         astTransportDescriptors[1] = (const BMUXlib_TS_TransportDescriptor **) pDescriptors[1];

         BMUXlib_List_GetEntries(
                  hMuxTS->hTransportDescriptorMetaDataPendingList[uiTransportChannelIndex],
                  &pDescriptors[0],
                  &uiNumTransportDescriptorsMetaData[0],
                  &pDescriptors[1],
                  &uiNumTransportDescriptorsMetaData[1]
                  );

         astTransportDescriptorsMetaData[0] = (const BMUXlib_TS_P_TransportDescriptorMetaData **) pDescriptors[0];
         astTransportDescriptorsMetaData[1] = (const BMUXlib_TS_P_TransportDescriptorMetaData **) pDescriptors[1];

         /* Ignore the descriptors that are queued */
         {
            uint32_t uiQueuedPreviously = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued;

            for ( i = 0; ( i < 2 ) && ( 0 != uiQueuedPreviously ) ; i++ )
            {
               if ( uiQueuedPreviously < uiNumTransportDescriptors[i] )
               {
                  astTransportDescriptors[i] += uiQueuedPreviously;
                  astTransportDescriptorsMetaData[i] += uiQueuedPreviously;
                  uiNumTransportDescriptors[i] -= uiQueuedPreviously;
                  uiQueuedPreviously -= uiQueuedPreviously;
               }
               else
               {
                  astTransportDescriptors[i] = NULL;
                  astTransportDescriptorsMetaData[i] = NULL;
                  uiQueuedPreviously -= uiNumTransportDescriptors[i];
                  uiNumTransportDescriptors[i] = 0;
               }
            }

            if (0 != uiQueuedPreviously)
            {
               BDBG_ERR(("Mismatch between queued transport descriptors and total descriptors"));
            }
         }

         {
            unsigned j;

            uiTotalDescriptorsToQueue = 0;

            for ( i = 0; i < 2; i++ )
            {
               for ( j = 0; j < uiNumTransportDescriptors[i]; j++)
               {
                  hMuxTS->astTransportDescriptorTemp[uiTotalDescriptorsToQueue] = *astTransportDescriptors[i][j];
                  hMuxTS->astTransportDescriptorMetaDataTemp[uiTotalDescriptorsToQueue] = *astTransportDescriptorsMetaData[i][j];
                  uiTotalDescriptorsToQueue++;
               }
            }

            if ( 0 != uiTotalDescriptorsToQueue )
            {
               pTransportInterface->fAddTransportDescriptors(
                                       pTransportInterface->pContext,
                                       hMuxTS->astTransportDescriptorTemp,
                                       uiTotalDescriptorsToQueue,
                                       &uiTotalQueued
                                       );

               hMuxTS->status.stTransport[uiTransportChannelIndex].uiDescriptorsAdded += uiTotalQueued;
            }

            /* SW7425-4707: If all descriptors aren't queued, check to see if any descriptors will be late in the next MSP and print a warning */
            if ( ( false == hMuxTS->status.stStartSettings.bNonRealTimeMode )
                 && ( uiTotalDescriptorsToQueue != uiTotalQueued ) )
            {
               unsigned uiLateCount = 0;
               unsigned i;
               uint32_t uiNextPacingCounter;
               BMUXlib_TS_TransportStatus stTransportStatus;

               hMuxTS->status.stStartSettings.transport.stDeviceInterface.fGetTransportStatus(
                        hMuxTS->status.stStartSettings.transport.stDeviceInterface.pContext,
                        &stTransportStatus
                        );

               uiNextPacingCounter = stTransportStatus.uiESCR + hMuxTS->status.stStartSettings.uiServicePeriod * 27000;

               /* Enumerate through all deferred descriptors and count how many may be late in the next MSP */
               for (i = uiTotalQueued; i < uiTotalDescriptorsToQueue; i++ )
               {
                  int32_t iDeltaPacketPacing = hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp - uiNextPacingCounter;

                  if ( ( true == hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
                       && ( iDeltaPacketPacing < 0 ) )
                  {
                     if ( 0 == uiLateCount )
                     {
                        hMuxTS->status.iExecutionTimeAdjustment = (iDeltaPacketPacing - (27000 - 1)) /27000;
                        if ( hMuxTS->status.iExecutionTimeAdjustment < (signed) -hMuxTS->status.stStartSettings.uiServicePeriod )
                        {
                           BDBG_ERR(("Frames will be late in the next MSP.  Check that the transport queue depth is large enough."));
                           hMuxTS->status.iExecutionTimeAdjustment = (signed) -hMuxTS->status.stStartSettings.uiServicePeriod;
                        }
                     }
                     uiLateCount++;
                  }
               }

               if ( 0 != uiLateCount )
               {
                  BDBG_WRN(("Adjusting the next execution time by %d ms to prevent %u deferred frames from being late", hMuxTS->status.iExecutionTimeAdjustment, uiLateCount));
               }
            }

            hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued += uiTotalQueued;
         }

         if ( 0 != uiTotalQueued )
         {
            for ( i = 0; i < uiTotalQueued; i++ )
            {
               hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsSent++;

               if ( true == hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
               {
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiNextExpectedESCR = hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiLastStartingESCR = hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
               }

               if ( true == hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid )
               {
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiCurrentPacket2PacketTimestampDelta = hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta;
               }

               BMUXlib_TS_P_CalculateNextESCR(
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiNextExpectedESCR,
                  hMuxTS->astTransportDescriptorTemp[i].uiBufferLength,
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiCurrentPacket2PacketTimestampDelta,
                  &hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingQueued.uiNextExpectedESCR,
                  ( uiTransportChannelIndex == hMuxTS->status.stInput.system.uiTransportChannelIndex ) ? 188 : 184
               );

               if ( true == hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid )
               {
                  BMUXlib_TS_TransportStatus stTransportStatus;

                  hMuxTS->status.stStartSettings.transport.stDeviceInterface.fGetTransportStatus(
                           hMuxTS->status.stStartSettings.transport.stDeviceInterface.pContext,
                           &stTransportStatus
                           );

                  /* SW7435-1133: We want to make sure the previous frame was sent prior to its DTS (endingESCR[n] < DTS[n])
                   *    For each FRAME_START, we need to remember the PACING_COUNTER, ESCR, PKT2PKT delta, DTS.
                   *    Then we need to accumulate the total number of bytes that are part of the frame.
                   *    Then compute the endingESCR[n].  Compare the endingESCR to both the DTS and next ESCR
                   */

                  if ( ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.bValid )
                       && ( 9*32 < hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiLengthOfFrame ) ) /* SW7435-1133: Ignore late frames when the frame size is less than the number of bytes that can get stuck in the CABAC on 7425/7435 (9 JWORDS) */
                  {
                     uint32_t uiEndOfTransmission = 0;
                     int32_t iDeltaESCRPacingCounter = 0;
                     {
                        /* Verify the previous frame did not arrive late */
                        uint32_t uiElapsedESCR = 0;
                        uint32_t uiStartOfTransmission = 0;

                        {
                           /* Calculate the elapsed ESCR for the frame */
                           unsigned uiNumPackets;
                           uiNumPackets = ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiLengthOfFrame + 183 ) / 184;

                           BDBG_ASSERT( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid );
                           uiElapsedESCR = uiNumPackets *  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta;
                        }

                        /* Determine the start of transmission (the larger of ESCR or PacingCounter) */
                        iDeltaESCRPacingCounter = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp - hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiPacingCounterIn27Mhz;

                        if ( iDeltaESCRPacingCounter >= 0 ) uiStartOfTransmission = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp;
                        else uiStartOfTransmission = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiPacingCounterIn27Mhz;

                        uiEndOfTransmission = uiStartOfTransmission + uiElapsedESCR;
                     }

                      /* Check to see if the frame arrived late */
                     if ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.bDtsValid )
                     {
                        int32_t iDeltaDtsEndESCR;

                        iDeltaDtsEndESCR = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiDtsIn27Mhz - uiEndOfTransmission;

                        if ( iDeltaDtsEndESCR < 0 )
                        {
                           BDBG_MODULE_WRN( BMUXLIB_TS_OUTPUT_DESC_ERROR, ("%08x: TRANSPORT[%d] Late Frame (Underflow) (endESCR=%08x > DTS=%08x) [%d ms] [%d ms]",
                              (unsigned) hMuxTS,
                              uiTransportChannelIndex,
                              uiEndOfTransmission,
                              hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiDtsIn27Mhz,
                              iDeltaDtsEndESCR/27000,
                              iDeltaESCRPacingCounter/27000
                           ));
                        }
                     }
                  }

                  BKNI_Memset( &hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection, 0, sizeof( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection ) );
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.stTsMuxDescriptorConfig = hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.bDtsValid = hMuxTS->astTransportDescriptorMetaDataTemp[i].stDtsInfo.bValid;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiDtsIn27Mhz = hMuxTS->astTransportDescriptorMetaDataTemp[i].stDtsInfo.uiDtsIn27Mhz;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiPacingCounterIn27Mhz = stTransportStatus.uiESCR;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiLengthOfFrame += hMuxTS->astTransportDescriptorTemp[i].uiBufferLength;
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.bValid = true;
               }
               else
               {
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stUnderflowDetection.uiLengthOfFrame += hMuxTS->astTransportDescriptorTemp[i].uiBufferLength;
               }

               BDBG_MODULE_MSG( BMUXLIB_TS_OUTPUT_DESC, ("TRANSPORT[%d] @%08llx (%08x), ts=%08x (%d), delta=%08x (%d), RAI=%d",
                        uiTransportChannelIndex,
                        hMuxTS->astTransportDescriptorTemp[i].uiBufferOffset,
                        hMuxTS->astTransportDescriptorTemp[i].uiBufferLength,
                        hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
                        hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid,
                        hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
                        hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid,
                        hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bRandomAccessIndication
               ));

#if BMUXLIB_TS_P_DUMP_TRANSPORT_DESC
               if ( NULL == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile )
               {
                  char fname[256];
#if BMUXLIB_TS_P_TEST_MODE
                  /* don't use handle in filename if in test mode ... it will change run-to-run
                     making it impossible to automate usage of the resulting file */
                  sprintf(fname, "BMUXlib_TRANSPORT_DESC_%02d.csv", uiTransportChannelIndex);
#else
                  sprintf(fname, "BMUXlib_TRANSPORT_DESC_%02d_%08x.csv", uiTransportChannelIndex, (unsigned) hMuxTS);
#endif
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hDescDumpFile = fopen(fname, "wb");
                  if ( NULL == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hDescDumpFile )
                  {
                     BDBG_ERR(("Error Creating Transport Descriptor Dump File (%s)", fname));
                  }
                  else
                  {
                     BDBG_WRN(("Creating Transport Descriptor Dump File (%s)", fname));
                  }
               }

               if ( NULL != hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hDescDumpFile )
               {
                  if ( false == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.bPrintedHeader )
                  {
                     fprintf( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hDescDumpFile,
                              "channel,count,pacing(27Mhz),address,length,escr(27Mhz),escr valid,pkt2pkt,pk2pkt valid,rai,queued,pending,delayed\n"
                              );

                     hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.bPrintedHeader = true;
                  }

                  fprintf( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hDescDumpFile,
                           "%10u,%10u,%10u,%p,%10u,%10u,%d,%10u,%d,%d,%d,%d,%d\n",
                           uiTransportChannelIndex,
                           hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsSent,
                           hMuxTS->status.stTransportStatus.uiESCR,
                           hMuxTS->astTransportDescriptorTemp[i].pBufferAddress,
                           hMuxTS->astTransportDescriptorTemp[i].uiBufferLength,
                           hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiNextPacketPacingTimestamp,
                           hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bNextPacketPacingTimestampValid,
                           hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.uiPacket2PacketTimestampDelta,
                           hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bPacket2PacketTimestampDeltaValid,
                           hMuxTS->astTransportDescriptorTemp[i].stTsMuxDescriptorConfig.bRandomAccessIndication,
                           uiTotalQueued,
                           hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].uiDescriptorsQueued,
                           uiTotalDescriptorsToQueue - uiTotalQueued
                           );
               }
#endif
#if BMUXLIB_TS_P_DUMP_TRANSPORT_PES
               if ( NULL == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile )
               {
                  char fname[256];
                  sprintf(fname, "BMUXlib_TRANSPORT_%02d.%s", uiTransportChannelIndex, ( uiTransportChannelIndex == hMuxTS->status.stInput.system.uiTransportChannelIndex ) ? "ts" : "pes");
                  hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile = fopen(fname, "wb");
                  if ( NULL == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile )
                  {
                     BDBG_ERR(("Error Creating Transport PES/TS Dump File (%s)", fname));
                  }
               }

               if ( NULL != hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile )
               {
                  fwrite( hMuxTS->astTransportDescriptorTemp[i].pBufferAddress,
                           1,
                           hMuxTS->astTransportDescriptorTemp[i].uiBufferLength,
                           hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stDebug.hPESDumpFile
                           );
               }
#endif
            }
            BDBG_MSG(("TRANSPORT[%d] <-- %d", uiTransportChannelIndex,  uiTotalQueued));
         }
      }
   }

   BDBG_LEAVE( BMUXlib_TS_P_ScheduleProcessedBuffers );

   return BERR_TRACE( BERR_SUCCESS );
}

/* BMUXlib_TS_P_Flush -
 * inserts a dummy transport descriptor on each channel that has an ESCR equal
 * to the largest most recent ESCR on each channel */
bool
BMUXlib_TS_P_Flush(
         BMUXlib_TS_Handle hMuxTS,
         bool bFlushSystemData
         )
{
   BMUXlib_InputGroup_Status stStatus;
   uint32_t uiTransportChannelIndexWithLargestESCR = 0;
   unsigned uiLastPendingESCR = 0;
   bool bLastPendingESCRValid = false;

   BDBG_ENTER( BMUXlib_TS_P_Flush );

   {
      uint32_t uiTransportChannelIndex;

      /* Determine the largest ESCR to use for flushing the outputs */
      for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
      {
         if ( ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bActive )
              && ( false == BMUXlib_List_IsEmpty( hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex] ) )
              && ( ( ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR > uiLastPendingESCR )
                       && ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR - uiLastPendingESCR ) < 0x80000000 ) )
                     || ( ( hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR < uiLastPendingESCR )
                          && ( ( uiLastPendingESCR - hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR ) > 0x80000000 ) )
                   )
                   || ( false == bLastPendingESCRValid )
                 )
            )
         {
            uiTransportChannelIndexWithLargestESCR = uiTransportChannelIndex;
            uiLastPendingESCR = hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].stTimingPending.uiNextExpectedESCR;
            bLastPendingESCRValid = true;
         }
      }

      if ( false == bLastPendingESCRValid )
      {
         BDBG_MODULE_MSG( BMUXLIB_TS_EOS, ("No pending descriptors, so flush not needed") );
         return true;
      }

      if ( ( ( hMuxTS->status.stTransportStatus.uiESCR > uiLastPendingESCR )
             && ( ( hMuxTS->status.stTransportStatus.uiESCR - uiLastPendingESCR ) < 0x80000000 ) )
           || ( ( hMuxTS->status.stTransportStatus.uiESCR < uiLastPendingESCR )
                && ( ( uiLastPendingESCR - hMuxTS->status.stTransportStatus.uiESCR ) > 0x80000000 ) ) )
      {
         BDBG_MODULE_MSG( BMUXLIB_TS_EOS, ("Current ESCR is larger than the last pending ESCR, so skipping NULL"));
         return true;
      }

      BMUXlib_InputGroup_GetStatus(hMuxTS->status.hInputGroup, &stStatus);

      /* Process each transport channel */
      for ( uiTransportChannelIndex = 0; uiTransportChannelIndex < BMUXLIB_TS_MAX_TRANSPORT_INSTANCES; uiTransportChannelIndex++ )
      {
         if ( ( 0 == stStatus.uiNumActiveInputs ) /* Process all transport channels if all inputs are done */
              || ( ( uiTransportChannelIndex != uiTransportChannelIndexWithLargestESCR ) /* Do not process the transport channel with the largest ESCR */
                 && ( ( uiTransportChannelIndex != hMuxTS->status.stInput.system.uiTransportChannelIndex ) /* Do not process the system data transport channel */
                      || ( true == bFlushSystemData ) ) ) /* but DO process the system data channel it if finishing input */
            )
         {
            if ( true == hMuxTS->status.stOutput.stTransport[uiTransportChannelIndex].bActive ) /* Process transport channels that are active */
            {
               bool bAllInputsForTransportChannelHasSeenEOS = false;

               unsigned uiIndex;

               /* Check if EOS seen on all input channels associated with this Transport Channel */
               for ( uiIndex = 0; uiIndex < hMuxTS->status.uiNumInputs ; uiIndex++ )
               {
                  if ( uiTransportChannelIndex == hMuxTS->status.stInputMetaData[uiIndex].uiTransportChannelIndex )
                  {
                     bAllInputsForTransportChannelHasSeenEOS = hMuxTS->status.stInputMetaData[uiIndex].bEOSSeen;

                     if ( false == bAllInputsForTransportChannelHasSeenEOS )
                     {
                        break;
                     }
                  }
               }

               if ( ( uiTransportChannelIndex == hMuxTS->status.stInput.system.uiTransportChannelIndex )
                    && ( true == bFlushSystemData ) )
               {
                  bAllInputsForTransportChannelHasSeenEOS = true;
               }

               if ( true == bAllInputsForTransportChannelHasSeenEOS )
               {
                  if ( ( true == BMUXlib_List_IsEmpty( hMuxTS->hTransportDescriptorFreeList ) ) /* There is an available descriptor */
                       || ( true == BMUXlib_List_IsFull( hMuxTS->hTransportDescriptorPendingList[uiTransportChannelIndex]) ) /* There is room to insert the entry */
                     )
                  {
                     BDBG_MODULE_WRN( BMUXLIB_TS_EOS, ("Not enough resources to insert NULL entry on transport[%d]", uiTransportChannelIndex));
                     return false;
                  }
                  else
                  {
                     BERR_Code rc;

                     /* Queue the null descriptor to flush the output */
                     rc = BERR_TRACE(BMUXlib_TS_P_InsertNULLTransportDescriptor(
                              hMuxTS,
                              uiTransportChannelIndex,
                              uiLastPendingESCR,
                              BMUXlib_TS_P_DataType_eNULL,
                              BMUXlib_TS_P_SourceType_eUnknown,
                              0
                              ));

                     BDBG_MODULE_MSG( BMUXLIB_TS_EOS, ("Inserting NULL descriptor (escr=%08x) on transport[%d]", uiLastPendingESCR, uiTransportChannelIndex));

                     BDBG_ASSERT( BERR_SUCCESS == rc );
                  }
               }
            }
         }
      }
   }

   BDBG_LEAVE( BMUXlib_TS_P_Flush );

   return true;
}
