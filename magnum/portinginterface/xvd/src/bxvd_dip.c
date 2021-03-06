/***************************************************************************
 *     Copyright (c) 2003-2013, Broadcom Corporation
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
#include "bstd.h"
#include "bkni.h"          /* For malloc */
#include "bmem.h"
#include "bxvd.h"
#include "bxvd_platform.h"
#include "bxvd_priv.h"
#include "bxvd_dip.h"

#if (BCHP_CHIP == 7401 || BCHP_CHIP == 7403)
#include "bchp_decode_ip_shim.h"
#elif ((BCHP_CHIP != 7445) && (BCHP_CHIP != 7145) && (BCHP_CHIP != 7250) && (BCHP_CHIP != 7268) && (BCHP_CHIP != 7271) && \
       (BCHP_CHIP != 7364) && (BCHP_CHIP != 73625) && (BCHP_CHIP != 7366) && (BCHP_CHIP != 74295) && (BCHP_CHIP != 7439) && \
       (BCHP_CHIP != 74371) && (BCHP_CHIP != 75525) && (BCHP_CHIP != 75635) && (BCHP_CHIP != 75845) && \
       (BCHP_CHIP != 7586) &&  (BCHP_CHIP != 73465))

#include "bchp_decode_ip_shim_0.h"
#endif

BDBG_MODULE(BXVD_DIP);
BDBG_FILE_MODULE(BXVD_DIPCTL);
BDBG_FILE_MODULE(BXVD_DIPDBG);

#if !BXVD_P_RUL_DONE_MASK_64_BITS

/* 32 bit VDC RUL Done interrupt mask support */

#define BXVD_DIP_TEST_MULTI_INTERRUPT_BITS_SET( stDisplayInfo, stIntrSettings)  \
{                                                                               \
   uint32_t uiTempBits = stDisplayInfo.vsync_parity &                           \
      ( stIntrSettings.stRULIDMasks_0.ui32BottomFieldRULIDMask |                \
        stIntrSettings.stRULIDMasks_0.ui32TopFieldRULIDMask |                   \
        stIntrSettings.stRULIDMasks_0.ui32ProgressiveFieldRULIDMask );          \
                                                                                \
   /* Check if more than one vsync polarity bit is set */                       \
   if (uiTempBits & (uiTempBits - 1))                                           \
   {                                                                            \
      BDBG_WRN(("Multiple bits set in the vsync polarity bit field (0x%08x)",   \
                stDisplayInfo.vsync_parity));                                   \
   }                                                                            \
}

#define BXVD_DIP_IS_POLARITY_BOTTOM(stDisplayInfo, stIntrSettings)                       \
   (stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32BottomFieldRULIDMask)

#define BXVD_DIP_IS_POLARITY_TOP(stDisplayInfo, stIntrSettings)                       \
   (stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32TopFieldRULIDMask)

#define BXVD_DIP_IS_POLARITY_FRAME(stDisplayInfo, stIntrSettings)                             \
   (stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32ProgressiveFieldRULIDMask)

#define BXVD_DIP_DBG_WRN_BAD_VSYNC(stDisplayInfo)                                       \
   BDBG_WRN(("Unrecognized VSYNC polarity type (0x%08x)", stDisplayInfo.vsync_parity));

#else

/* 64 bit VDC RUL Done interrupt mask support */

#define BXVD_DIP_TEST_MULTI_INTERRUPT_BITS_SET( stDisplayInfo, stIntrSettings)          \
{                                                                                       \
   uint32_t uiTempBits = stDisplayInfo.vsync_parity &                                   \
            ( stIntrSettings.stRULIDMasks_0.ui32BottomFieldRULIDMask |                  \
              stIntrSettings.stRULIDMasks_0.ui32TopFieldRULIDMask |                     \
              stIntrSettings.stRULIDMasks_0.ui32ProgressiveFieldRULIDMask );            \
                                                                                        \
   uint32_t uiTempBits1 = stDisplayInfo.vsync_parity_1 &                                \
      ( stIntrSettings.stRULIDMasks_1.ui32BottomFieldRULIDMask |                        \
        stIntrSettings.stRULIDMasks_1.ui32TopFieldRULIDMask |                           \
        stIntrSettings.stRULIDMasks_1.ui32ProgressiveFieldRULIDMask );                  \
                                                                                        \
   /* Check if more than one vsync polarity bit is set */                               \
   if ((uiTempBits & (uiTempBits - 1)) ||                                               \
       (uiTempBits1 & (uiTempBits1 - 1)) ||                                             \
       ((uiTempBits != 0 ) && (uiTempBits1 != 0)))                                      \
   {                                                                                    \
      BDBG_WRN(("Multiple bits set in the vsync polarity bit field (0x%08x), (0x%08x)", \
                stDisplayInfo.vsync_parity, stDisplayInfo.vsync_parity_1));             \
   }                                                                                    \
}

#define BXVD_DIP_IS_POLARITY_BOTTOM(stDisplayInfo, stIntrSettings)                           \
   ((stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32BottomFieldRULIDMask) || \
    (stDisplayInfo.vsync_parity_1 & stIntrSettings.stRULIDMasks_1.ui32BottomFieldRULIDMask))

#define BXVD_DIP_IS_POLARITY_TOP(stDisplayInfo, stIntrSettings)                           \
   ((stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32TopFieldRULIDMask) || \
    (stDisplayInfo.vsync_parity_1 & stIntrSettings.stRULIDMasks_1.ui32TopFieldRULIDMask))

#define BXVD_DIP_IS_POLARITY_FRAME(stDisplayInfo, stIntrSettings)                                  \
   ((stDisplayInfo.vsync_parity & stIntrSettings.stRULIDMasks_0.ui32ProgressiveFieldRULIDMask ) || \
    (stDisplayInfo.vsync_parity_1 & stIntrSettings.stRULIDMasks_1.ui32ProgressiveFieldRULIDMask ))

#define BXVD_DIP_DBG_WRN_BAD_VSYNC(stDisplayInfo)                        \
   BDBG_WRN(("Unrecognized VSYNC polarity type (0x%08x), (0x%08x)",      \
             stDisplayInfo.vsync_parity, stDisplayInfo.vsync_parity_1));

#endif

#define BXVD_DIP_P_MEASURE_LATENCY 0
#define LATENCY_FILE_OUTPUT 1

#if BXVD_DIP_P_MEASURE_LATENCY
#include <stdio.h>
#endif

typedef struct BXVD_DisplayInterruptProvider_P_DisplayInterruptHandler
{
      BXDM_DisplayInterruptHandler_isr fCallback;
      void *pPrivateContext;
} BXVD_DisplayInterruptProvider_P_DisplayInterruptHandler;

typedef struct BXVD_DisplayInterruptProvider_P_ChannelContext
{
      BXVD_DisplayInterruptProvider_P_ChannelSettings stChannelSettings;

      BXVD_DisplayInterruptProvider_P_InterruptSettings stInterruptSettings;
      uint32_t uiRegMaskCurrent_0;
      uint32_t uiRegMaskCurrent_1;
      bool bInterruptSettingsValid;

      /* BINT Callback Handle */
      BINT_CallbackHandle hPictureDataReadyInterruptCallback;
      BXDM_DisplayInterruptInfo stDisplayInterruptInfo;

      /* Application Callback */
      BXVD_DisplayInterruptProvider_P_DisplayInterruptHandler stDisplayInterruptHandler;
} BXVD_DisplayInterruptProvider_P_ChannelContext;

#if BXVD_DIP_P_MEASURE_LATENCY

#define BXVD_DIP_MAX_TIME_SAMPLES 2048

/* DM's persistent state between BXVD_StartDecode and
 * BXVD_StopDecode */

static uint32_t guiSampleIndex;
static uint32_t gauiTimeSample[BXVD_DIP_MAX_TIME_SAMPLES];


void BXVD_DIP_P_SampleTime(BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh)
{
   uint32_t uiTemp;
   BXVD_Handle hXvd = hXvdDipCh->stChannelSettings.hXvd;
   BXVD_ChannelHandle hCh = hXvd->ahChannel[0];
   BXVD_STC eStc;

   if (guiSampleIndex >= BXVD_DIP_MAX_TIME_SAMPLES)
   {
      BKNI_Printf("\nBXVD_DIP_P_SampleTime too many samples\n");
   }
   else
   {
#if (BCHP_CHIP == 7445)
      BDBG_ERR(("NEED TO ADD STC Read Macro in BXVD_DIP_P_SampleTime"));
#else
      if (hXvd->uDecoderInstance == 0)
      {
	 BXVD_GetSTCSource(hCh, &eStc);
         if (BXVD_STC_eZero == eStc  )
         {
            uiTemp = BREG_Read32(hXvd->hReg, BCHP_DECODE_IP_SHIM_0_STC0_REG);
         }
         else
         {
            uiTemp = BREG_Read32(hXvd->hReg, BCHP_DECODE_IP_SHIM_0_STC1_REG);
         }
      }
      else
#endif
      {
         uiTemp = 0xFFFFFFFF;
      }

      gauiTimeSample[guiSampleIndex] = uiTemp;
      guiSampleIndex++;
   }
}

static void BXVD_DIP_S_PrintSampleTimes(void *pXvdDipCh, uint32_t uiStcFromDecoder)
{
#if LATENCY_FILE_OUTPUT
   FILE *outfd;
#endif
   int32_t iNum45kTicks;
   int32_t iUsecs;

   BSTD_UNUSED(pXvdDipCh);

#if LATENCY_FILE_OUTPUT
   outfd = fopen("timeresults.txt", "a+");
   if (outfd == NULL)
   {
      BKNI_Printf("BXVD_DIP_S_PrintSampleTimes is unable to open output file\n");
      return;
   }
#endif
   /* Sampled STC, AVD STC snapshot and latency between the two. */
   iNum45kTicks = (int)gauiTimeSample[0] - (int)uiStcFromDecoder;
   iUsecs = ( iNum45kTicks * 1000 * 1000 ) / 45000;

#if !LATENCY_FILE_OUTPUT
   BKNI_Printf("%u,%u,%d,",
         uiStcFromDecoder,
         gauiTimeSample[0],
         iUsecs
         );
#else
   fprintf(outfd, "%u,%u,%d,", uiStcFromDecoder, gauiTimeSample[0], iUsecs);
#endif

   /* The elapse time for the DM ISR routine. */
   iNum45kTicks = gauiTimeSample[2] - gauiTimeSample[0];
   iUsecs = ( iNum45kTicks * 1000 * 1000 ) / 45000;
#if !LATENCY_FILE_OUTPUT
   BKNI_Printf("%u,", iUsecs );
#else
   fprintf(outfd, "%u,", iUsecs );
#endif
   /* Execution time for callbacks. */
   iNum45kTicks = gauiTimeSample[2] - gauiTimeSample[1];
   iUsecs = ( iNum45kTicks * 1000 * 1000 ) / 45000;
#if !LATENCY_FILE_OUTPUT
   BKNI_Printf("%u ", iUsecs );
   BKNI_Printf("\n");
#else
   fprintf(outfd, "%u\n", iUsecs );
   fflush(outfd);
   fclose(outfd);
#endif
}
#endif

BERR_Code
BXVD_DisplayInterruptProvider_P_GetDefaultChannelSettings(
         BXVD_DisplayInterruptProvider_P_ChannelSettings *pstXvdDipChSettings
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_GetDefaultChannelSettings );

   BDBG_ASSERT( pstXvdDipChSettings );

   BKNI_Memset( pstXvdDipChSettings, 0, sizeof( BXVD_DisplayInterruptProvider_P_ChannelSettings ) );

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_GetDefaultChannelSettings );
   return BERR_TRACE(BERR_SUCCESS);
}

void
BXVD_DisplayInterruptProvider_S_PictureDataReady_isr(
         void* pXvdDipCh,
         int iParam2
         )
{
   BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh = (BXVD_DisplayInterruptProvider_P_ChannelHandle) pXvdDipCh;

   BXVD_P_DisplayInfo stDisplayInfo;
   BXVD_P_DisplayInfo stUpdateDisplayInfo;

   uint32_t i;

#if BDBG_DEBUG_BUILD
   BXVD_DisplayInterrupt eDisplayInterrupt = ( BXVD_DisplayInterrupt ) iParam2;

   BDBG_ASSERT( eDisplayInterrupt == hXvdDipCh->stChannelSettings.eDisplayInterrupt );
#else
   BSTD_UNUSED(iParam2);
#endif

   BDBG_ASSERT( hXvdDipCh );

#if BXVD_DIP_P_MEASURE_LATENCY
   guiSampleIndex = 0;
   BKNI_Memset( gauiTimeSample, 0, (sizeof( uint32_t) * BXVD_DIP_MAX_TIME_SAMPLES) );

   BXVD_DIP_P_SampleTime(hXvdDipCh);
#endif

   /*************************************/
   /* Update the Display Interrupt Info */
   /*************************************/

   for ( i = 0; i < 2; i++ )
   {
      BXVD_P_READ_DISPLAY_INFO(hXvdDipCh, stDisplayInfo);

      /* Extract Interrupt Polarity */
      BXVD_DIP_TEST_MULTI_INTERRUPT_BITS_SET(stDisplayInfo, hXvdDipCh->stInterruptSettings);

      if (BXVD_DIP_IS_POLARITY_BOTTOM(stDisplayInfo, hXvdDipCh->stInterruptSettings))
      {
         hXvdDipCh->stDisplayInterruptInfo.eInterruptPolarity = BAVC_Polarity_eBotField ;
      }
      else if (BXVD_DIP_IS_POLARITY_TOP(stDisplayInfo, hXvdDipCh->stInterruptSettings))
      {
         hXvdDipCh->stDisplayInterruptInfo.eInterruptPolarity = BAVC_Polarity_eTopField ;
      }
      else if (BXVD_DIP_IS_POLARITY_FRAME(stDisplayInfo, hXvdDipCh->stInterruptSettings))
      {
         hXvdDipCh->stDisplayInterruptInfo.eInterruptPolarity = BAVC_Polarity_eFrame ;
      }
      else
      {
         /* We default to TopField for the vsync polarity if we don't
          * recognize the polarity type */
         BXVD_DIP_DBG_WRN_BAD_VSYNC(stDisplayInfo);

         hXvdDipCh->stDisplayInterruptInfo.eInterruptPolarity = BAVC_Polarity_eTopField ;
      }

      /* Verify that the AVD FW hasn't updated the STC and Vsync Parity, meaning something is keeping the
         PDR ISR from being serviced in a timely manner */

      BXVD_P_READ_DISPLAY_INFO(hXvdDipCh, stUpdateDisplayInfo);

      if (!BXVD_P_IS_DISPLAY_INFO_EQUAL(stUpdateDisplayInfo, stDisplayInfo))
      {
#if !B_REFSW_SYSTEM_MODE_CLIENT
         BDBG_ERR(("AVD Picture Data Ready interrupt not processed in time!"));
#else
         BDBG_MSG(("AVD Picture Data Ready interrupt not processed in time!"));
#endif
      }
      else
      {
         break;
      }
   }

   /* Extract STC snapshot(s) */
   BDBG_ASSERT( BXVD_P_STC_MAX == hXvdDipCh->stDisplayInterruptInfo.uiSTCCount );

   BXVD_P_SAVE_DIP_INFO_STC(hXvdDipCh, stDisplayInfo);

   /* Increment Interrupt Count */
   hXvdDipCh->stDisplayInterruptInfo.stInterruptCount.uiValue++;
   hXvdDipCh->stDisplayInterruptInfo.stInterruptCount.bValid = true;

   /* Execute the application's Picture Data Ready callback */
   if( NULL != hXvdDipCh->stDisplayInterruptHandler.fCallback )
   {
#if BXVD_DIP_P_MEASURE_LATENCY
      BXVD_DIP_P_SampleTime(hXvdDipCh);
#endif

#if BDBG_DEBUG_BUILD
      {
         /*
          * If enabled, print out a debug message every vsync.
          */
         char cPolarity;
         switch( hXvdDipCh->stDisplayInterruptInfo.eInterruptPolarity )
         {
            case BAVC_Polarity_eTopField:    cPolarity='T';    break;
            case BAVC_Polarity_eBotField:    cPolarity='B';    break;
            case BAVC_Polarity_eFrame:       cPolarity='F';    break;
            default:                         cPolarity='u';    break;
         }

         BDBG_MODULE_MSG( BXVD_DIPDBG, ("0x%0*lx %d.%d %c stc:%08x(%d) %d",
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh,
                  hXvdDipCh->stChannelSettings.hXvd->uDecoderInstance,
                  hXvdDipCh->stChannelSettings.eDisplayInterrupt,
                  cPolarity,
                  hXvdDipCh->stDisplayInterruptInfo.astSTC[0].uiValue,
                  hXvdDipCh->stDisplayInterruptInfo.astSTC[0].bValid,
                  hXvdDipCh->stDisplayInterruptInfo.stInterruptCount.uiValue
                  ));
      }
#endif

      hXvdDipCh->stDisplayInterruptHandler.fCallback(
               hXvdDipCh->stDisplayInterruptHandler.pPrivateContext,
               &hXvdDipCh->stDisplayInterruptInfo
               );
#if BXVD_DIP_P_MEASURE_LATENCY
      BXVD_DIP_P_SampleTime(hXvdDipCh);
#endif
   }

#if BXVD_DIP_P_MEASURE_LATENCY
#if !BXVD_P_FW_HIM_API

   BXVD_DIP_S_PrintSampleTimes(hXvdDipCh, stDisplayInfo.stc_snapshot);
#else
   BXVD_DIP_S_PrintSampleTimes(hXvdDipCh, stDisplayInfo.stc_snapshot[0]);
#endif

#endif

   return;
}

static BERR_Code
BXVD_DisplayInterruptProvider_S_SetupInterrupts(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
         )
{
   BERR_Code rc;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_S_SetupInterrupts );

   BDBG_ASSERT( hXvdDipCh );

#if !BXVD_POLL_FW_MBX
   if ( hXvdDipCh->hPictureDataReadyInterruptCallback ) {
      rc = BINT_DisableCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
      if (rc != BERR_SUCCESS)
      {
         return BERR_TRACE(rc);
      }
   } else {
      rc = BINT_CreateCallback(
               &hXvdDipCh->hPictureDataReadyInterruptCallback,
               hXvdDipCh->stChannelSettings.hInterrupt,
               hXvdDipCh->stChannelSettings.interruptId,
               BXVD_DisplayInterruptProvider_S_PictureDataReady_isr,
               ( void* ) hXvdDipCh,
               hXvdDipCh->stChannelSettings.eDisplayInterrupt
               );

      if (rc != BERR_SUCCESS)
      {
         return BERR_TRACE(rc);
      }
   }

   rc = BINT_EnableCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
   if (rc != BERR_SUCCESS )
   {
      return BERR_TRACE(rc);
   }
#endif
   BDBG_LEAVE( BXVD_DisplayInterruptProvider_S_SetupInterrupts );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code BXVD_DisplayInterruptProvider_P_EnableInterrupts(
   BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
   )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_EnableInterrupts );

   BDBG_ASSERT( hXvdDipCh );

   if ( hXvdDipCh->hPictureDataReadyInterruptCallback )
   {
      rc = BINT_EnableCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_SetupInterrupts );

   return BERR_TRACE( rc );
}

BERR_Code BXVD_DisplayInterruptProvider_P_DisableInterrupts(
   BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
   )
{
   BERR_Code rc = BERR_SUCCESS;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_DisaableInterrupts );

   BDBG_ASSERT( hXvdDipCh );

   if ( hXvdDipCh->hPictureDataReadyInterruptCallback )
   {
      rc = BINT_DisableCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_DisableInterrupts );

   return BERR_TRACE( rc );
}

static BERR_Code
BXVD_DisplayInterruptProvider_S_TeardownInterrupts(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
         )
{
   BERR_Code rc;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_S_TeardownInterrupts );

   BDBG_ASSERT( hXvdDipCh );

   if ( hXvdDipCh->hPictureDataReadyInterruptCallback ) {
      rc = BINT_DisableCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
      if (rc != BERR_SUCCESS)
      {
         return BERR_TRACE(rc);
      }

      rc = BINT_DestroyCallback( hXvdDipCh->hPictureDataReadyInterruptCallback );
      if (rc != BERR_SUCCESS )
      {
         return BERR_TRACE(rc);
      }

      hXvdDipCh->hPictureDataReadyInterruptCallback = NULL;
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_S_TeardownInterrupts );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BXVD_DisplayInterruptProvider_P_OpenChannel(
         BXVD_DisplayInterruptProvider_P_ChannelHandle *phXvdDipCh,
         const BXVD_DisplayInterruptProvider_P_ChannelSettings *pstXvdDipChSettings
         )
{
   BERR_Code rc;
   BXVD_DisplayInterruptProvider_P_ChannelContext *pXvdDipCh = NULL;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_OpenChannel );

   BDBG_ASSERT( phXvdDipCh );
   BDBG_ASSERT( pstXvdDipChSettings );

   /* Set the handle to NULL in case the allocation fails */
   *phXvdDipCh = NULL;

   pXvdDipCh = ( BXVD_DisplayInterruptProvider_P_ChannelContext* ) BKNI_Malloc( sizeof( BXVD_DisplayInterruptProvider_P_ChannelContext ) );
   if ( NULL == pXvdDipCh )
   {
      return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   }

   /* Zero out the newly allocated context */
   BKNI_Memset( ( void * ) pXvdDipCh, 0, sizeof( BXVD_DisplayInterruptProvider_P_ChannelContext ) );

   /* Allocate STC snapshot array */
   pXvdDipCh->stDisplayInterruptInfo.uiSTCCount = BXVD_P_STC_MAX;
   pXvdDipCh->stDisplayInterruptInfo.astSTC = ( BXDM_QualifiedValue * ) BKNI_Malloc( sizeof ( BXDM_QualifiedValue ) * pXvdDipCh->stDisplayInterruptInfo.uiSTCCount );
   if ( NULL == pXvdDipCh->stDisplayInterruptInfo.astSTC )
   {
      BXVD_DisplayInterruptProvider_P_CloseChannel( pXvdDipCh );
      return BERR_TRACE( BERR_OUT_OF_SYSTEM_MEMORY );
   }

   /* TODO: Validate the channel settings */
   pXvdDipCh->stChannelSettings = *pstXvdDipChSettings;

   /* Install Picture Data Ready Callback Handler */
   rc = BXVD_DisplayInterruptProvider_S_SetupInterrupts( pXvdDipCh );
   if ( BERR_SUCCESS != rc )
   {
      BXVD_DisplayInterruptProvider_P_CloseChannel( pXvdDipCh );
      return BERR_TRACE(rc);
   }

   *phXvdDipCh = pXvdDipCh;

   BDBG_MODULE_MSG( BXVD_DIPCTL, ("Create: hDip:0x%0*lx Decoder:%d hXvd:0x%0*lx eDisplayInterrupt:%d interruptId:%08x",
                  BXVD_P_DIGITS_IN_LONG, (long)pXvdDipCh,
                  pXvdDipCh->stChannelSettings.hXvd->uDecoderInstance,
                  BXVD_P_DIGITS_IN_LONG, (long)pXvdDipCh->stChannelSettings.hXvd,
                  pXvdDipCh->stChannelSettings.eDisplayInterrupt,
                  pXvdDipCh->stChannelSettings.interruptId
                  ));

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_OpenChannel );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BXVD_DisplayInterruptProvider_P_CloseChannel(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_CloseChannel );

   if ( NULL != hXvdDipCh )
   {
      BDBG_MODULE_MSG( BXVD_DIPCTL, ("Destroy: h0x%0*lx Decoder:%d",
                           BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh,
                           hXvdDipCh->stChannelSettings.hXvd->uDecoderInstance ));

      BXVD_DisplayInterruptProvider_S_TeardownInterrupts( hXvdDipCh );

      BKNI_Free( hXvdDipCh->stDisplayInterruptInfo.astSTC );
      BKNI_Free( hXvdDipCh );
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_CloseChannel );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BXVD_DisplayInterruptProvider_P_ProcessWatchdog(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh
         )
{
   BERR_Code rc;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_ProcessWatchdog );

   BDBG_ASSERT( hXvdDipCh );

   rc = BXVD_DisplayInterruptProvider_S_SetupInterrupts( hXvdDipCh );
   if ( BERR_SUCCESS != rc )
   {
      return BERR_TRACE( rc );
   }

   if ( true == hXvdDipCh->bInterruptSettingsValid )
   {
      /* clear pending BVNF interrupts since they are edge-triggered */
      BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                   hXvdDipCh->stChannelSettings.uiInterruptClearRegister,
                   hXvdDipCh->uiRegMaskCurrent_0);

      /* Restore mask register */
      BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                   hXvdDipCh->stChannelSettings.uiInterruptMaskRegister,
                   hXvdDipCh->uiRegMaskCurrent_0);

      if ( hXvdDipCh->stChannelSettings.uiInterruptClearRegister_1 != 0)
      {
         /* clear pending BVNF interrupts since they are edge-triggered */
         BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                      hXvdDipCh->stChannelSettings.uiInterruptClearRegister_1,
                      hXvdDipCh->uiRegMaskCurrent_1);

         BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                      hXvdDipCh->stChannelSettings.uiInterruptMaskRegister_1,
                      hXvdDipCh->uiRegMaskCurrent_1);
      }

      rc = BXVD_P_HostCmdSendConfig(hXvdDipCh->stChannelSettings.hXvd,
                                    hXvdDipCh->stChannelSettings.eDisplayInterrupt,
                                    hXvdDipCh->uiRegMaskCurrent_0,
                                    hXvdDipCh->uiRegMaskCurrent_1);
      if ( BERR_SUCCESS != rc )
      {
         return BERR_TRACE( rc );
      }
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_ProcessWatchdog );

   return BERR_TRACE( BERR_SUCCESS );
}

#if !B_REFSW_MINIMAL /* SWSTB-461 */
BERR_Code
BXVD_DisplayInterruptProvider_P_GetDefaultInterruptSettings(
         BXVD_DisplayInterruptProvider_P_InterruptSettings *pstXvdDipIntSettings
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_GetDefaultInterruptSettings );

   BDBG_ASSERT( pstXvdDipIntSettings );

   BKNI_Memset( pstXvdDipIntSettings, 0, sizeof( BXVD_DisplayInterruptProvider_P_InterruptSettings ) );

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_GetDefaultInterruptSettings );

   return BERR_TRACE(BERR_SUCCESS);
}
#endif

BERR_Code
BXVD_DisplayInterruptProvider_P_SetInterruptConfiguration(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh,
         const BXVD_DisplayInterruptProvider_P_InterruptSettings *pstXvdDipIntSettings
         )
{
   BERR_Code rc;

   uint32_t uiRegMaskNew_0, uiRegMaskNew_1;

   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_SetInterruptConfiguration );

   BDBG_ASSERT( hXvdDipCh );
   BDBG_ASSERT( pstXvdDipIntSettings );

   uiRegMaskNew_0 = ( pstXvdDipIntSettings->stRULIDMasks_0.ui32ProgressiveFieldRULIDMask |
                    pstXvdDipIntSettings->stRULIDMasks_0.ui32TopFieldRULIDMask |
                    pstXvdDipIntSettings->stRULIDMasks_0.ui32BottomFieldRULIDMask );

   uiRegMaskNew_1 = ( pstXvdDipIntSettings->stRULIDMasks_1.ui32ProgressiveFieldRULIDMask |
                    pstXvdDipIntSettings->stRULIDMasks_1.ui32TopFieldRULIDMask |
                    pstXvdDipIntSettings->stRULIDMasks_1.ui32BottomFieldRULIDMask );

   if ( ( false == hXvdDipCh->bInterruptSettingsValid )
        || ( hXvdDipCh->uiRegMaskCurrent_0 != uiRegMaskNew_0 )
        || ( hXvdDipCh->uiRegMaskCurrent_1 != uiRegMaskNew_1 ))
   {
      hXvdDipCh->bInterruptSettingsValid = true;
      hXvdDipCh->stInterruptSettings = *pstXvdDipIntSettings;
      hXvdDipCh->uiRegMaskCurrent_0 = uiRegMaskNew_0;
      hXvdDipCh->uiRegMaskCurrent_1 = uiRegMaskNew_1;

      /* clear pending BVNF interrupts since they are edge-triggered */
      BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                   hXvdDipCh->stChannelSettings.uiInterruptClearRegister,
                   hXvdDipCh->uiRegMaskCurrent_0);

      BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                   hXvdDipCh->stChannelSettings.uiInterruptMaskRegister,
                   hXvdDipCh->uiRegMaskCurrent_0);

      BDBG_MODULE_MSG( BXVD_DIPCTL, ("hDip:0x%0*lx SetInterrupt Decoder:%d hXvd:0x%0*lx eDisplayInterrupt:%d mask0:%08x mask1:%08x",
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh,
                  hXvdDipCh->stChannelSettings.hXvd->uDecoderInstance,
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh->stChannelSettings.hXvd,
                  hXvdDipCh->stChannelSettings.eDisplayInterrupt,
                  hXvdDipCh->uiRegMaskCurrent_0,
                  hXvdDipCh->uiRegMaskCurrent_0));

      if ( hXvdDipCh->stChannelSettings.uiInterruptClearRegister_1 != 0 )
      {
         /* clear pending BVNF interrupts since they are edge-triggered */
         BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                      hXvdDipCh->stChannelSettings.uiInterruptClearRegister_1,
                      hXvdDipCh->uiRegMaskCurrent_1);

         BREG_Write32(hXvdDipCh->stChannelSettings.hRegister,
                      hXvdDipCh->stChannelSettings.uiInterruptMaskRegister_1,
                      hXvdDipCh->uiRegMaskCurrent_1);
      }

      rc = BXVD_P_HostCmdSendConfig(hXvdDipCh->stChannelSettings.hXvd,
                                    hXvdDipCh->stChannelSettings.eDisplayInterrupt,
                                    hXvdDipCh->uiRegMaskCurrent_0,
                                    hXvdDipCh->uiRegMaskCurrent_1);
      if ( BERR_SUCCESS != rc )
      {
         return BERR_TRACE( rc );
      }
   }

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_SetInterruptConfiguration );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BXVD_DisplayInterruptProvider_P_GetInterruptConfiguration(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh,
         BXVD_DisplayInterruptProvider_P_InterruptSettings *pstXvdDipIntSettings
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_P_GetInterruptConfiguration );

   BDBG_ASSERT( hXvdDipCh );
   BDBG_ASSERT( pstXvdDipIntSettings );

   *pstXvdDipIntSettings = hXvdDipCh->stInterruptSettings;

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_P_GetInterruptConfiguration );
   return BERR_TRACE(BERR_SUCCESS);
}

BERR_Code
BXVD_DisplayInterruptProvider_InstallCallback_DisplayInterrupt(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh,
         BXDM_DisplayInterruptHandler_isr fCallback,
         void *pPrivateContext
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_InstallCallback_DisplayInterrupt );

   BDBG_ASSERT( hXvdDipCh );

   hXvdDipCh->stDisplayInterruptHandler.fCallback = fCallback;
   hXvdDipCh->stDisplayInterruptHandler.pPrivateContext = pPrivateContext;

   BDBG_MODULE_MSG( BXVD_DIPCTL, ("hDip:0x%0*lx Install DIH Decoder:%d hXvd:0x%0*lx eDisplayInterrupt:%d hDih:0x%0*lx cb:0x%0*lx",
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh,
                  hXvdDipCh->stChannelSettings.hXvd->uDecoderInstance,
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh->stChannelSettings.hXvd,
                  hXvdDipCh->stChannelSettings.eDisplayInterrupt,
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh->stDisplayInterruptHandler.pPrivateContext,
                  BXVD_P_DIGITS_IN_LONG, (long)hXvdDipCh->stDisplayInterruptHandler.fCallback
                  ));

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_InstallCallback_DisplayInterrupt );

   return BERR_TRACE( BERR_SUCCESS );
}

BERR_Code
BXVD_DisplayInterruptProvider_GetDisplayInterruptInfo_isr(
         BXVD_DisplayInterruptProvider_P_ChannelHandle hXvdDipCh,
         BXDM_DisplayInterruptInfo *pstXvdDisplayInterruptInfo
         )
{
   BDBG_ENTER( BXVD_DisplayInterruptProvider_GetDisplayInterruptInfo_isr );

   BDBG_ASSERT( hXvdDipCh );
   BDBG_ASSERT( pstXvdDisplayInterruptInfo );

   *pstXvdDisplayInterruptInfo = hXvdDipCh->stDisplayInterruptInfo;

   BDBG_LEAVE( BXVD_DisplayInterruptProvider_GetDisplayInterruptInfo_isr );

   return BERR_TRACE( BERR_SUCCESS );
}
