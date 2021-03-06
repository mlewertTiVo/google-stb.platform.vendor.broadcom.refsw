/******************************************************************************
 * (c) 2016 Broadcom Corporation
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
 *
 *****************************************************************************/
#include "bip_player_impl.h"
#include <strings.h>
#include "nexus_timebase.h"
#include "nexus_video_window.h"
#include "nexus_video_adj.h"

BDBG_MODULE( bip_player );
BDBG_OBJECT_ID_DECLARE( BIP_Player );

/* Forward declarations. */
static BIP_Status processPlayerState_locked( BIP_PlayerHandle hPlayer, BIP_Arb_ThreadOrigin threadOrigin);

static void playerPrintNexusStatus(
    BIP_PlayerHandle hPlayer
    )
{
    NEXUS_Error nrc;

    if ( hPlayer == NULL ) return;

    /* Playback Stats. */
    if ( hPlayer->hPlayback )
    {
        NEXUS_PlaybackStatus pbStatus;
        NEXUS_PlaypumpStatus ppStatus;

        nrc = NEXUS_Playback_GetStatus( hPlayer->hPlayback, &pbStatus );
        BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_Playback_GetStatus() failed" ), error_play, nrc, nrc );

        BDBG_WRN(( "PB: state=%s fifo=%u/%u played=%lld dataErrors=%d indexErrors=%d posFirst=%d posLast=%d posRead=%d posCurrent=%d",
                    pbStatus.state == NEXUS_PlaybackState_eStopped ? "Stopped":
                    pbStatus.state == NEXUS_PlaybackState_ePlaying ? "Playing":
                    pbStatus.state == NEXUS_PlaybackState_ePaused ? "Paused":
                    pbStatus.state == NEXUS_PlaybackState_eTrickMode ? "Trickmode":
                    pbStatus.state == NEXUS_PlaybackState_eAborted ? "Aborted": "Unknown",
                    pbStatus.fifoDepth, pbStatus.fifoSize,
                    pbStatus.bytesPlayed, pbStatus.dataErrors, pbStatus.indexErrors,
                    pbStatus.first, pbStatus.last, pbStatus.readPosition, pbStatus.position
                 ));
        nrc = NEXUS_Playpump_GetStatus( hPlayer->hPlaypump, &ppStatus );
        BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_Playpump_GetStatus() failed" ), error_play, nrc, nrc );

        BDBG_WRN(( "PP: state=%s syncErrors=%u pacingTsRangeError=%u streamErrors=%d",
                    ppStatus.started?"Started":"Stopped", ppStatus.syncErrors, ppStatus.pacingTsRangeError, ppStatus.streamErrors));
error_play:
        ; /* empty statement for label */
    }

    /* Playpump Stats. */
    if ( hPlayer->hPlaypump)
    {
        NEXUS_PlaypumpStatus ppStatus;

        nrc = NEXUS_Playpump_GetStatus( hPlayer->hPlaypump, &ppStatus );
        BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_Playpump_GetStatus() failed" ), error_pp, nrc, nrc );

        BDBG_WRN(( "PP1: state=%s FIFO depth/size=%u/%u bytesPlayed=%llu syncErrors=%u pacingTsRangeError=%u streamErrors=%d",
                    ppStatus.started?"Started":"Stopped", ppStatus.fifoDepth, ppStatus.fifoSize, ppStatus.bytesPlayed,
                    ppStatus.syncErrors, ppStatus.pacingTsRangeError, ppStatus.streamErrors));
error_pp:
        ; /* empty statement for label */
    }
    if ( hPlayer->hPlaypump2)
    {
        NEXUS_PlaypumpStatus ppStatus;

        nrc = NEXUS_Playpump_GetStatus( hPlayer->hPlaypump2, &ppStatus );
        BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_Playpump_GetStatus() failed" ), error_pp2, nrc, nrc );

        BDBG_WRN(( "PP2: state=%s FIFO depth/size=%u/%u bytesPlayed=%llu syncErrors=%u pacingTsRangeError=%u streamErrors=%d",
                    ppStatus.started?"Started":"Stopped", ppStatus.fifoDepth, ppStatus.fifoSize, ppStatus.bytesPlayed,
                    ppStatus.syncErrors, ppStatus.pacingTsRangeError, ppStatus.streamErrors));
error_pp2:
        ; /* empty statement for label */
    }

    /* StcChannel Stats. */
    if (hPlayer->hStcChannel)
    {
        NEXUS_StcChannelSettings settings;

        NEXUS_StcChannel_GetSettings(hPlayer->hStcChannel, &settings);
        if (settings.mode == NEXUS_StcChannelMode_eAuto)
        {
            BDBG_WRN(( "Stc: mode=%s behavior=%s transportType=%s offsetThreshold=%u",
                        "Auto", BIP_ToStr_NEXUS_StcChannelAutoModeBehavior(settings.modeSettings.Auto.behavior),
                        BIP_ToStr_NEXUS_TransportType(settings.modeSettings.Auto.transportType), settings.modeSettings.Auto.offsetThreshold));
        }
        if (settings.mode == NEXUS_StcChannelMode_ePcr)
        {
            BDBG_WRN(( "Stc: mode=%s hPidChannel=%p maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s offsetThreshold=%u",
                        "Pcr", settings.modeSettings.pcr.pidChannel, settings.modeSettings.pcr.maxPcrError, settings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                        settings.modeSettings.pcr.disableTimestampCorrection?"Y":"N", settings.modeSettings.Auto.offsetThreshold));
        }
    }
    if (hPlayer->hSimpleStcChannel)
    {
        NEXUS_SimpleStcChannelSettings settings;

        NEXUS_SimpleStcChannel_GetSettings( hPlayer->hSimpleStcChannel, &settings );
        if (settings.mode == NEXUS_StcChannelMode_eAuto)
        {
            BDBG_WRN(( "Stc: mode=%s behavior=%s transportType=%s offsetThreshold=%u",
                        "Auto", BIP_ToStr_NEXUS_StcChannelAutoModeBehavior(settings.modeSettings.Auto.behavior),
                        BIP_ToStr_NEXUS_TransportType(settings.modeSettings.Auto.transportType), settings.modeSettings.Auto.offsetThreshold));
        }
        if (settings.mode == NEXUS_StcChannelMode_ePcr)
        {
            BDBG_WRN(( "Stc: mode=%s hPidChannel=%p maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s offsetThreshold=%u",
                        "Pcr", settings.modeSettings.pcr.pidChannel, settings.modeSettings.pcr.maxPcrError, settings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                        settings.modeSettings.pcr.disableTimestampCorrection?"Y":"N", settings.modeSettings.Auto.offsetThreshold));
        }
        if (settings.modeSettings.highJitter.mode != NEXUS_SimpleStcChannelHighJitterMode_eNone)
        {
            BDBG_WRN(( "Stc: mode=%s threshold=%u",
                        (settings.modeSettings.highJitter.mode == NEXUS_SimpleStcChannelHighJitterMode_eDirect ? "Direct" :
                         settings.modeSettings.highJitter.mode == NEXUS_SimpleStcChannelHighJitterMode_eTtsPacing ? "TtsPacing" : "PcrPacing"),
                        settings.modeSettings.highJitter.threshold ));
        }
    }

    /* PidChannel Stats. */
    if (hPlayer->hVideoPidChannel)
    {
        NEXUS_PidChannelStatus pidStatus;
        {
            nrc = NEXUS_PidChannel_GetStatus( hPlayer->hVideoPidChannel, &pidStatus);
            BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_PidChannel_GetStatus() failed" ), error_pid, nrc, nrc );

            BDBG_WRN(("VPidCh: pid=0x%x hwIndex=%u transportType=%u enabled=%s playback=%s pbIdx=%u parserBandIdx=%u ccErrs=%u",
                        pidStatus.pid, pidStatus.pidChannelIndex,
                        pidStatus.transportType, pidStatus.enabled?"Y":"N",
                        pidStatus.playback?"Y":"N", pidStatus.playbackIndex,
                        pidStatus.continuityCountErrors
                        ));
            BDBG_WRN(("VPidChRave: ccErrs=%u cdbOverflowErrors=%u itbOverflowErrors=%u emulationByteRemovalErrors=%u teiErrors=%u pusiErrors=%u xcBufOverflowErrors=%u",
                        pidStatus.raveStatus.continuityCountErrors,
                        pidStatus.raveStatus.cdbOverflowErrors,
                        pidStatus.raveStatus.itbOverflowErrors,
                        pidStatus.raveStatus.emulationByteRemovalErrors,
                        pidStatus.raveStatus.teiErrors,
                        pidStatus.raveStatus.pusiErrors,
                        pidStatus.xcBufferStatus.overflowErrors
                     ));
        }
        if (hPlayer->hPrimaryAudioPidChannel)
        {
            nrc = NEXUS_PidChannel_GetStatus( hPlayer->hPrimaryAudioPidChannel, &pidStatus);
            BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_PidChannel_GetStatus() failed" ), error_pid, nrc, nrc );

            BDBG_WRN(("APidCh: pid=0x%x hwIndex=%u transportType=%u enabled=%s playback=%s pbIdx=%u parserBandIdx=%u ccErrs=%u",
                        pidStatus.pid, pidStatus.pidChannelIndex,
                        pidStatus.transportType, pidStatus.enabled?"Y":"N",
                        pidStatus.playback?"Y":"N", pidStatus.playbackIndex,
                        pidStatus.continuityCountErrors
                        ));
            BDBG_WRN(("APidChRave: ccErrs=%u cdbOverflowErrors=%u itbOverflowErrors=%u emulationByteRemovalErrors=%u teiErrors=%u pusiErrors=%u xcBufOverflowErrors=%u",
                        pidStatus.raveStatus.continuityCountErrors,
                        pidStatus.raveStatus.cdbOverflowErrors,
                        pidStatus.raveStatus.itbOverflowErrors,
                        pidStatus.raveStatus.emulationByteRemovalErrors,
                        pidStatus.raveStatus.teiErrors,
                        pidStatus.raveStatus.pusiErrors,
                        pidStatus.xcBufferStatus.overflowErrors
                     ));
        }
error_pid:
        ; /* empty statement for label */
    }

    {
        NEXUS_VideoDecoderStatus videoStats;
        if (hPlayer->hSimpleVideoDecoder)
        {
            nrc = NEXUS_SimpleVideoDecoder_GetStatus( hPlayer->hSimpleVideoDecoder, &videoStats );
            BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_SimpleVideoDecoder_GetStatus() failed" ), error_vstats, nrc, nrc );
        }
        else if (hPlayer->hVideoDecoder)
        {
            nrc = NEXUS_VideoDecoder_GetStatus( hPlayer->hVideoDecoder, &videoStats );
            BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_VideoDecoder_GetStatus() failed" ), error_vstats, nrc, nrc );
        }
        if (hPlayer->hSimpleVideoDecoder || hPlayer->hVideoDecoder)
        {
            BDBG_WRN(("Vdec: started=%s srcSize=%u x %u frameRate=%u interlaced=%s tsm=%s firstPtsPassed=%s",
                        videoStats.started?"Y":"N",
                        videoStats.source.width, videoStats.source.height,
                        videoStats.frameRate, videoStats.interlaced?"Y":"N",
                        videoStats.tsm?"Y":"N", videoStats.firstPtsPassed?"Y":"N"
                        ));
            BDBG_WRN(("Vdec: cdbFifo=%u/%u qDepth=%u decoded=%u displayed=%u picRcvd=%u iFrDisplayed=%u decoderErrs=%u decoderOverflows=%u decoderDrops=%u displayErrs=%u displayDrops=%u displayUnderflows=%u ptsErrs=%u watchdogs=%u bytesDecoded=%lld",
                        videoStats.fifoDepth, videoStats.fifoSize, videoStats.queueDepth,
                        videoStats.numDecoded, videoStats.numDisplayed,
                        videoStats.numPicturesReceived,
                        videoStats.numIFramesDisplayed,
                        videoStats.numDecodeErrors,
                        videoStats.numDecodeOverflows,
                        videoStats.numDecodeDrops,

                        videoStats.numDisplayErrors,
                        videoStats.numDisplayDrops,
                        videoStats.numDisplayUnderflows,
                        videoStats.ptsErrorCount,
                        videoStats.numWatchdogs,
                        videoStats.numBytesDecoded
                        ));
        }
error_vstats:
        ; /* empty statement for label */

        {
            NEXUS_AudioDecoderStatus audioStats;
            if (hPlayer->hSimplePrimaryAudioDecoder)
            {
                nrc = NEXUS_SimpleAudioDecoder_GetStatus( hPlayer->hSimplePrimaryAudioDecoder, &audioStats );
                BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_SimpleAudioDecoder_GetStatus() failed" ), error_astats, nrc, nrc );
            }
            else if (hPlayer->hPrimaryAudioDecoder)
            {
                nrc = NEXUS_AudioDecoder_GetStatus( hPlayer->hPrimaryAudioDecoder, &audioStats );
                BIP_CHECK_GOTO((nrc==NEXUS_SUCCESS), ( "NEXUS_AudioDecoder_GetStatus() failed" ), error_astats, nrc, nrc );
            }
            if (hPlayer->hSimplePrimaryAudioDecoder || hPlayer->hPrimaryAudioDecoder)
            {
                BDBG_WRN(("Adec: started=%s tsm=%s locked=%s cdbFifoSz=%u/%u sampleRate=%d frDecoded=%u frErrs=%u fifoOverflows=%u fifoUnderflows=%u bytesDecoded=%lld watchdogs=%u",
                            audioStats.started?"Y":"N",
                            audioStats.tsm?"Y":"N", audioStats.locked?"Y":"N",
                            audioStats.fifoDepth, audioStats.fifoSize,
                            audioStats.sampleRate, audioStats.framesDecoded,
                            audioStats.frameErrors, audioStats.numFifoOverflows,
                            audioStats.numFifoUnderflows, audioStats.numBytesDecoded, audioStats.numWatchdogs
                         ));
            }
        }
error_astats:
        ; /* empty statement for label */
    }
}

static void playerPrintStatus(
    BIP_PlayerHandle hPlayer
    )
{
    BDBG_WRN(("Player: state=%s subState=%s playRate=%d dataAvailabilityModel=%s clockRecoveryMode=%s playerMode=%s",
                BIP_ToStr_BIP_PlayerState(hPlayer->state),
                BIP_ToStr_BIP_PlayerSubState(hPlayer->subState),
                hPlayer->playRate,
                BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel),
                BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode),
                BIP_ToStr_BIP_PlayerMode(hPlayer->mode)
             ));
    playerPrintNexusStatus(hPlayer);
}

static void  closeAllPidChannels(
    BIP_PlayerHandle hPlayer
    )
{
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Close All PidChannel !" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    if (hPlayer->useNexusPlaypump && hPlayer->hPlaypump)
    {
        NEXUS_Playpump_CloseAllPidChannels(hPlayer->hPlaypump);
    }
    else if (hPlayer->hPlayback)
    {
        NEXUS_Playback_CloseAllPidChannels(hPlayer->hPlayback);
    }
    hPlayer->hPrimaryAudioPidChannel = NULL;
    hPlayer->hVideoPidChannel = NULL;
    hPlayer->hPcrPidChannel = NULL;
} /* closeAllPidChannels */

static void closePidChannel(
    BIP_PlayerHandle        hPlayer,
    NEXUS_PidChannelHandle  hPidChannel
    )
{
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: closing hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPidChannel));
    if (hPlayer->useNexusPlaypump)
    {
        if (NEXUS_Playpump_ClosePidChannel(hPlayer->hPlaypump, hPidChannel) != NEXUS_SUCCESS)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: NEXUS_Playpump_ClosePidChannel Failed for hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPidChannel));
        }
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Playpump hPidChannel=%p Closed" BIP_MSG_PRE_ARG, hPlayer, hPidChannel));
    }
    else
    {
        NEXUS_Playback_ClosePidChannel(hPlayer->hPlayback, hPidChannel);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Playback hPidChannel=%p Closed" BIP_MSG_PRE_ARG, hPlayer, hPidChannel));
    }
}

static void closeAndRemoveAllPidChannelsFromTrackList(
    BIP_PlayerHandle    hPlayer
    )
{
    BIP_PlayerTrackListEntryHandle hTrackEntry;

    /* Check if new track matches either both Id & type or just one of them of an existing track and take the correct action. */
    for (
            hTrackEntry = BLST_Q_FIRST( &hPlayer->trackListHead);
            hTrackEntry;
            hTrackEntry = BLST_Q_FIRST( &hPlayer->trackListHead)
        )
    {
        BLST_Q_REMOVE( &hPlayer->trackListHead, hTrackEntry, trackListNext );
        BDBG_ASSERT( hTrackEntry->hPidChannel );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: closing hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hTrackEntry->hPidChannel));
        closePidChannel(hPlayer, hTrackEntry->hPidChannel);
        B_Os_Free(hTrackEntry);
    }
}

static BIP_Status closeAndRemovePidChannelFromTrackList(
    BIP_PlayerHandle        hPlayer,
    NEXUS_PidChannelHandle  hPidChannel
    )
{
    BIP_PlayerTrackListEntryHandle hTrackEntry;

    /* Check if new track matches either both Id & type or just one of them of an existing track and take the correct action. */
    for (
            hTrackEntry = BLST_Q_FIRST( &hPlayer->trackListHead);
            hTrackEntry && hPidChannel == hTrackEntry->hPidChannel;
            hTrackEntry = BLST_Q_NEXT( hTrackEntry, trackListNext)
        )
    {
        closePidChannel(hPlayer, hPidChannel);

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: closed hPidChannel=%p, remove remove & destroy hTrackEntry=%p" BIP_MSG_PRE_ARG, hPlayer, hPidChannel, hTrackEntry));
        BLST_Q_REMOVE( &hPlayer->trackListHead, hTrackEntry, trackListNext );
        B_Os_Free(hTrackEntry);
        return (BIP_SUCCESS);
    }
    return (BIP_INF_NOT_AVAILABLE);
}

static BIP_Status openAndAddPidChannelToTrackList(
    BIP_PlayerHandle                            hPlayer,
    unsigned                                    trackId,
    const BIP_PlayerOpenPidChannelSettings      *pSettings,
    NEXUS_PidChannelHandle                      *phPidChannel
    )
{
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    BIP_PlayerTrackListEntryHandle hTrackEntry;

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: trackId 0x%x" BIP_MSG_PRE_ARG, hPlayer, trackId));
    /* Check if new track matches either both Id & type or just one of them of an existing track and take the correct action. */
    for (
         hTrackEntry = BLST_Q_FIRST( &hPlayer->trackListHead);
         hTrackEntry && trackId == hTrackEntry->trackId;
         hTrackEntry = BLST_Q_NEXT( hTrackEntry, trackListNext)
        )
    {
        /* Duplicate track, return its pidChannel. */
        *phPidChannel = hTrackEntry->hPidChannel;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Duplicate entry: (trackId 0x%x), returning existing hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, trackId, *phPidChannel));
        bipStatus = BIP_SUCCESS;
        return (bipStatus);
    }

    /* We haven't found this trackId in current list, create a new entry, open an associated PidChannel and store its settings. */
    {
        NEXUS_Error nrc;

        hTrackEntry = B_Os_Calloc( 1, sizeof(BIP_PlayerTrackListEntry));
        BIP_CHECK_GOTO( (hTrackEntry), ( "Failed to allocate memory (%d bytes) for TrackEntry Object", sizeof(BIP_PlayerTrackListEntry) ), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus );

        /* If caller wants to open an allPass channel, get the hw index associated w/ the all pass channel. */
        if ( hPlayer->playerSettings.playbackSettings.playpumpSettings.allPass )
        {
            nrc = NEXUS_Playpump_GetAllPassPidChannelIndex(hPlayer->hPlaypump, (unsigned *)&pSettings->pidSettings.pidSettings.pidSettings.pidChannelIndex);
            BIP_CHECK_GOTO(( nrc == NEXUS_SUCCESS ), ( "NEXUS_Playpump_GetAllPassPidChannelIndex Failed" ), error, BIP_ERR_NEXUS, bipStatus );
        }

        if (hPlayer->streamInfo.usePlaypump)
        {
            hTrackEntry->hPidChannel = NEXUS_Playpump_OpenPidChannel(hPlayer->hPlaypump, trackId, &pSettings->pidSettings.pidSettings);
            BIP_CHECK_GOTO(( hTrackEntry->hPidChannel ), ( "NEXUS_Playpump_OpenPidChannel Failed" ), error, BIP_ERR_NEXUS, bipStatus );
        }
        else
        {
            hTrackEntry->hPidChannel = NEXUS_Playback_OpenPidChannel(hPlayer->hPlayback, trackId, &pSettings->pidSettings);
            BIP_CHECK_GOTO(( hTrackEntry->hPidChannel ), ( "NEXUS_Playback_OpenPidChannel Failed" ), error, BIP_ERR_NEXUS, bipStatus );
        }

        hTrackEntry->trackId = trackId;
        hTrackEntry->settings = *pSettings;
        *phPidChannel = hTrackEntry->hPidChannel;
        BLST_Q_INSERT_TAIL( &hPlayer->trackListHead, hTrackEntry, trackListNext );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Added track entry=%p: (trackId 0x%x), returning hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hTrackEntry, trackId, *phPidChannel));
        bipStatus = BIP_SUCCESS;
    }

error:
    if ( bipStatus != BIP_SUCCESS )
    {
        if ( hTrackEntry ) B_Os_Free( hTrackEntry );
    }
    return bipStatus;
} /* openAndAddPidChannelToTrackList */

static void unPrepare(
    BIP_PlayerHandle hPlayer
    )
{
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Un-preparing .." BIP_MSG_PRE_ARG, hPlayer));

    if (hPlayer->hVideoPidChannel && hPlayer->hVideoDecoder)
    {
        NEXUS_VideoDecoderSettings  videoDecoderSettings;

        /* Reset the Video PtsOffset that may have been set due to high jitter settings. */
        NEXUS_VideoDecoder_GetSettings(hPlayer->hVideoDecoder, &videoDecoderSettings);
        videoDecoderSettings.ptsOffset = 0;
        if ( NEXUS_VideoDecoder_SetSettings(hPlayer->hVideoDecoder, &videoDecoderSettings) != NEXUS_SUCCESS)
        {
            BDBG_WRN((BIP_MSG_PRE_FMT "hPlayer=%p: NEXUS_VideoDecoder_SetSettings Failed to Reset the Video Decoder PTS value" BIP_MSG_PRE_ARG, hPlayer));
        }
    }

    if (hPlayer->hPrimaryAudioPidChannel && hPlayer->hPrimaryAudioDecoder)
    {
        NEXUS_AudioDecoderSettings  audioDecoderSettings;

        /* Reset the Audio PtsOffset that may have been set due to high jitter settings. */
        NEXUS_AudioDecoder_GetSettings(hPlayer->hPrimaryAudioDecoder, &audioDecoderSettings);
        audioDecoderSettings.ptsOffset = 0;
        if (NEXUS_AudioDecoder_SetSettings(hPlayer->hPrimaryAudioDecoder, &audioDecoderSettings) != NEXUS_SUCCESS)
        {
            BDBG_WRN((BIP_MSG_PRE_FMT "hPlayer=%p: NEXUS_AudioDecoder_SetSettings Failed to Reset the Audio Decoder PTS value" BIP_MSG_PRE_ARG, hPlayer));
        }
    }

    if (hPlayer->hSimpleStcChannel)
    {
        hPlayer->hSimpleStcChannel = NULL;
    }

    if (hPlayer->hStcChannel)
    {
        hPlayer->hStcChannel = NULL;
    }

    /* Now close all the PidChannels that we had opened. */
    closeAndRemoveAllPidChannelsFromTrackList(hPlayer);
    closeAllPidChannels(hPlayer);

    if (hPlayer->lockedTimebase != NEXUS_Timebase_eInvalid)
    {
        NEXUS_Timebase_Close(hPlayer->lockedTimebase);
        hPlayer->lockedTimebase = NEXUS_Timebase_eInvalid;
    }

    if (hPlayer->freeRunTimebase != NEXUS_Timebase_eInvalid)
    {
        NEXUS_Timebase_Close(hPlayer->freeRunTimebase);
        hPlayer->freeRunTimebase = NEXUS_Timebase_eInvalid;
    }

    if (hPlayer->hPlayback)
    {
        NEXUS_Playback_Destroy(hPlayer->hPlayback);
        hPlayer->hPlayback = NULL;
    }
    if (!hPlayer->prepareSettings.hPlaypump && hPlayer->hPlaypump)
    {
        NEXUS_Playpump_Close(hPlayer->hPlaypump);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: hPlaypump=%p Closed!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump));
        hPlayer->hPlaypump = NULL;
    }
    if (!hPlayer->prepareSettings.hPlaypump2 && hPlayer->hPlaypump2)
    {
        NEXUS_Playpump_Close(hPlayer->hPlaypump2);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: hPlaypump2=%p Closed!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump2));
        hPlayer->hPlaypump2 = NULL;
    }
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: unPrepare done!" BIP_MSG_PRE_ARG, hPlayer));
} /* unPrepare */

static void disconnectPlayer(
    BIP_PlayerHandle hPlayer
    )
{
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );

    if (hPlayer->dlnaFlags.hDtcpIpHostIp)
    {
        BIP_String_Destroy(hPlayer->dlnaFlags.hDtcpIpHostIp);
        hPlayer->dlnaFlags.hDtcpIpHostIp = NULL;
    }
    if (hPlayer->dlnaFlags.hDtcpIpPort)
    {
        BIP_String_Destroy(hPlayer->dlnaFlags.hDtcpIpPort);
        hPlayer->dlnaFlags.hDtcpIpPort = NULL;
    }
    if (hPlayer->hAdditionalHeaders)
    {
        BIP_String_Destroy(hPlayer->hAdditionalHeaders);
        hPlayer->hAdditionalHeaders = NULL;
    }
    if (hPlayer->hNetworkInterfaceName)
    {
        BIP_String_Destroy(hPlayer->hNetworkInterfaceName);
        hPlayer->hNetworkInterfaceName = NULL;
    }
    if ( hPlayer->getStatusFromServerState.hAdditionalHeaders )
    {
        BIP_String_Destroy(hPlayer->getStatusFromServerState.hAdditionalHeaders);
        hPlayer->getStatusFromServerState.hAdditionalHeaders = NULL;
    }
    /* Only close DMA handle if app didn't provide it via the connectSettings */
    if (!hPlayer->connectSettings.hDma && hPlayer->hDma)
    {
        NEXUS_Dma_Close(hPlayer->hDma);
        hPlayer->hDma = NULL;
    }
    if (hPlayer->hMediaInfo)
    {
        BIP_MediaInfo_Destroy(hPlayer->hMediaInfo);
        hPlayer->hMediaInfo = NULL;
    }
    if (hPlayer->hHttpResponse)
    {
        BIP_HttpResponse_Destroy(hPlayer->hHttpResponse, hPlayer);
        hPlayer->hHttpResponse = NULL;
    }
    if ( hPlayer->hApiTimer )
    {
        BIP_Timer_Destroy( hPlayer->hApiTimer );
        hPlayer->hApiTimer = NULL;
    }
    if (hPlayer->pbipState.hUrl)
    {
        BIP_String_Destroy(hPlayer->pbipState.hUrl);
        hPlayer->pbipState.hUrl = NULL;
    }
    if ( hPlayer->hUrl )
    {
        BIP_Url_Destroy(hPlayer->hUrl);
        hPlayer->hUrl = NULL;
    }

    if (hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders)
    {
        free(hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders);
        hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders = NULL;
    }

    if (hPlayer->pbipState.hPlaybackIp)
    {
        B_Error brc;
        brc = B_PlaybackIp_Close(hPlayer->pbipState.hPlaybackIp);
        BDBG_ASSERT(brc == B_ERROR_SUCCESS);
        hPlayer->pbipState.hPlaybackIp = NULL;
    }
    hPlayer->mediaProbeComplete = false;
    hPlayer->dtcpIpEnabled = false;
    hPlayer->sslEnabled = false;
    BKNI_Memset(&hPlayer->dlnaFlags, 0, sizeof(hPlayer->dlnaFlags));

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Done!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
}

static BIP_PlayerProtocol mapSchemeToBipProtocol(
    char *scheme
    )
{
    BIP_PlayerProtocol protocol;

    if (!strcasecmp(scheme,"udp")) {
        protocol = BIP_PlayerProtocol_eSimpleUdp;
    }
    else if (!strcasecmp(scheme,"rtp")) {
        protocol = BIP_PlayerProtocol_eRtp;
    }
    else if (!strcasecmp(scheme,"rtsp")) {
        protocol = BIP_PlayerProtocol_eRtsp;
    }
    else if (!strcasecmp(scheme,"https")) {
        /* HTTPs is treated as HTTP at the Player level. The SSL/TLS portion is handled by the BIP Socket or PBIP librar. */
        protocol = BIP_PlayerProtocol_eSimpleHttp;
    }
    else if (!strcasecmp(scheme,"http")) {
        protocol = BIP_PlayerProtocol_eSimpleHttp;
    }
    else {
        BDBG_ERR(( BIP_MSG_PRE_FMT "Incorrect or not yet supported URL Scheme=%s" BIP_MSG_PRE_ARG, scheme));
        protocol = BIP_PlayerProtocol_eMax;
    }
    return protocol;
}

static B_PlaybackIpProtocol mapSchemeToPBipProtocol(
    char *scheme
    )
{
    B_PlaybackIpProtocol protocol;

    if (!strcasecmp(scheme,"udp")) {
        protocol = B_PlaybackIpProtocol_eUdp;
    }
    else if (!strcasecmp(scheme,"rtp")) {
        protocol = B_PlaybackIpProtocol_eRtp;
    }
    else if (!strcasecmp(scheme,"rtsp")) {
        protocol = B_PlaybackIpProtocol_eRtsp;
    }
    else if (!strcasecmp(scheme,"https")) {
        protocol = B_PlaybackIpProtocol_eHttps;
    }
    else if (!strcasecmp(scheme,"http")) {
        protocol = B_PlaybackIpProtocol_eHttp;
    }
    else {
        BDBG_ERR(( BIP_MSG_PRE_FMT "Incorrect or not yet supported URL Scheme=%s" BIP_MSG_PRE_ARG, scheme));
        protocol = B_PlaybackIpProtocol_eMax;
    }
    return protocol;
}

static void pbipCallbackViaArbTimer(
    void *appCtx,
    int   param
    )
{
    BIP_PlayerHandle        hPlayer = appCtx;
    B_PlaybackIpEventIds    eventId = param;
    BIP_Status              completionStatus;

    BDBG_ASSERT(hPlayer);
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer: %p, eventId=%d" BIP_MSG_PRE_ARG, hPlayer, eventId ));

    B_Mutex_Lock(hPlayer->hStateMutex);

    /* Update player's sub-state for end of stream & error callback cases. */
    if (eventId == B_PlaybackIpEvent_eServerEndofStreamReached)
    {
        hPlayer->reachedEndOfStream = true;
    }
    else if (eventId == B_PlaybackIpEvent_eClientBeginOfStream)
    {
        hPlayer->reachedBeginningOfStream = true;
    }
    else if (eventId == B_PlaybackIpEvent_eErrorDuringStreamPlayback)
    {
        hPlayer->receivedNetworkError = true;
    }
    else if (eventId == B_PlaybackIpEvent_eSeekComplete)
    {
        hPlayer->seekCompleted = true;
    }
    processPlayerState_locked( (BIP_PlayerHandle) hPlayer, BIP_Arb_ThreadOrigin_eTimer);
    B_Mutex_Unlock( hPlayer->hStateMutex );

    /* Tell ARB to do any deferred work. */
    completionStatus = BIP_Arb_DoDeferred( NULL, BIP_Arb_ThreadOrigin_eTimer );
    BDBG_ASSERT( completionStatus == BIP_SUCCESS );
} /* pbipCallbackViaArbTimer */

static void playbackCallbackViaArbTimer(
    void *appCtx,
    int   param
    )
{
    BIP_Status          completionStatus;
    BIP_PlayerHandle    hPlayer = appCtx;

    BDBG_ASSERT(hPlayer);
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );

    BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer: %p" BIP_MSG_PRE_ARG, hPlayer ));
    B_Mutex_Lock(hPlayer->hStateMutex);

    /* Update player's sub-state for end of stream & error callback cases. */
    if (param == 0)
    {
        hPlayer->reachedEndOfStream = true;
    }
    else
    {
        hPlayer->receivedNetworkError = true;
    }
    processPlayerState_locked( (BIP_PlayerHandle) hPlayer, BIP_Arb_ThreadOrigin_eTimer);

    B_Mutex_Unlock( hPlayer->hStateMutex );

    /* Tell ARB to do any deferred work. */
    completionStatus = BIP_Arb_DoDeferred( NULL, BIP_Arb_ThreadOrigin_eTimer );
    BDBG_ASSERT( completionStatus == BIP_SUCCESS );
} /* playbackCallbackViaArbTimer */

static void pbipCallback(
    void *appCtx,
    B_PlaybackIpEventIds eventId
    )
{
    BIP_PlayerHandle hPlayer = appCtx;
    BIP_CallbackDesc callbackDesc;
    BIP_Status       brc;

    BDBG_ASSERT( hPlayer );
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Received eventId=%d from PBIP, state=%s" BIP_MSG_PRE_ARG, hPlayer, eventId, BIP_PLAYER_STATE(hPlayer->state) ));
    /* Note: we got callback from PBIP. We dont run Player's state in this callback context. */
    /* This is done because player's state machine may call destroy PBIP context and we can't do that in the PBIP callback itself. */
    /* Instead, we queue up a function with ARB logic that will be run thru ARB's timer context. */
    if ( hPlayer )
    {
        callbackDesc.callback = &pbipCallbackViaArbTimer;
        callbackDesc.context = hPlayer;
        callbackDesc.param = eventId;
        BIP_Arb_AddDeferredCallback( NULL, &callbackDesc);

        brc = BIP_Arb_DoDeferred( NULL, BIP_Arb_ThreadOrigin_eUnknown);
        BDBG_ASSERT( brc == BIP_SUCCESS );
    }
}

static void errorCallbackFromPlayback(
    void *appCtx,
    int param
    )
{
    BIP_PlayerHandle    hPlayer = appCtx;
    BIP_Status          brc;
    BIP_CallbackDesc    callbackDesc;

    BSTD_UNUSED(param);
    BDBG_ASSERT( hPlayer );

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Received error Callback from Nexus Playback" BIP_MSG_PRE_ARG, hPlayer ));

    /* Note: we got callback from Playback. We dont run Player's state in this callback context. This is done because player's state machine may */
    /* destroy Playback context and we can't do that in the Playback callback itself. */
    /* Instead, we queue up a function with ARB logic that will be run thru ARB's timer context. */
    if ( hPlayer )
    {
        callbackDesc.callback = &playbackCallbackViaArbTimer;
        callbackDesc.context = hPlayer;
        callbackDesc.param = 1; /* 0 for EOS & 1 for error */
        BIP_Arb_AddDeferredCallback( NULL, &callbackDesc);

        brc = BIP_Arb_DoDeferred( NULL, BIP_Arb_ThreadOrigin_eUnknown);
        BDBG_ASSERT( brc == BIP_SUCCESS );
    }
} /* errorCallbackFromPlayback */

static void endOfStreamCallbackFromPlayback(
    void *appCtx,
    int param
    )
{
    BIP_PlayerHandle    hPlayer = appCtx;
    BIP_Status          brc;
    BIP_CallbackDesc    callbackDesc;

    BSTD_UNUSED(param);
    BDBG_ASSERT( hPlayer );

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Received endOfStream Callback from Nexus Playback" BIP_MSG_PRE_ARG, hPlayer));

    /* Note: we got callback from Playback. We dont run Player's state in this callback context. This is done because player's state machine may */
    /* destroy Playback context and we can't do that in the Playback callback itself. */
    /* Instead, we queue up a function with ARB logic that will be run thru ARB's timer context. */
    if ( hPlayer )
    {
        callbackDesc.callback = &playbackCallbackViaArbTimer;
        callbackDesc.context = hPlayer;
        callbackDesc.param = 0; /* 0 for EOS & 1 for error */
        BIP_Arb_AddDeferredCallback( NULL, &callbackDesc);

        brc = BIP_Arb_DoDeferred( NULL, BIP_Arb_ThreadOrigin_eUnknown);
        BDBG_ASSERT( brc == BIP_SUCCESS );
    }
} /* endOfStreamCallbackFromPlayback */

static void asyncApiCallbackFromDtcpIpClient(
    void *pContext,
    int param
    )
{
    BIP_PlayerHandle    hPlayer = (BIP_PlayerHandle) pContext;

    BSTD_UNUSED(param);

    BDBG_MSG(( BIP_MSG_PRE_FMT "Enter: hPlayer %p --------------------> "  BIP_MSG_PRE_ARG, hPlayer ));
    processPlayerState( hPlayer, 0, BIP_Arb_ThreadOrigin_eTimer);
    BDBG_MSG(( BIP_MSG_PRE_FMT "Exit: hPlayer %p: <-------------------- " BIP_MSG_PRE_ARG, hPlayer ));

} /* asyncApiCallbackFromDtcpIpClient */

static void processPlayerStateFromApiTimerCallback(
    void *pContext
    )
{
    BIP_PlayerHandle    hPlayer = (BIP_PlayerHandle) pContext;
    BDBG_MSG(( BIP_MSG_PRE_FMT "Enter: hPlayer %p --------------------> "  BIP_MSG_PRE_ARG, hPlayer ));

    B_Mutex_Lock(hPlayer->hStateMutex);
    if (hPlayer->hApiTimer)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hSocket %p: Got BIP_Timer callback, marking timer as self-destructed" BIP_MSG_PRE_ARG, hPlayer ));
        hPlayer->hApiTimer = NULL;   /* Indicate timer not active. */
    }
    B_Mutex_Unlock(hPlayer->hStateMutex);

    processPlayerState( hPlayer, 0, BIP_Arb_ThreadOrigin_eTimer);
    BDBG_MSG(( BIP_MSG_PRE_FMT "Exit: hPlayer %p: <-------------------- " BIP_MSG_PRE_ARG, hPlayer ));
} /* processPlayerStateFromApiTimerCallback */

static void processPlayerStateFromPauseTimerCallback(
    void *pContext
    )
{
    BIP_PlayerHandle    hPlayer = (BIP_PlayerHandle) pContext;
    BDBG_MSG(( BIP_MSG_PRE_FMT "Enter: hPlayer %p --------------------> "  BIP_MSG_PRE_ARG, hPlayer ));

    B_Mutex_Lock(hPlayer->hStateMutex);
    if (hPlayer->pauseState.hPauseTimer )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hSocket %p: Got BIP_Timer callback, marking timer as self-destructed" BIP_MSG_PRE_ARG, hPlayer ));
        hPlayer->pauseState.hPauseTimer = NULL;   /* Indicate timer not active. */
    }
    processPlayerState_locked(hPlayer, BIP_Arb_ThreadOrigin_eTimer);

    B_Mutex_Unlock(hPlayer->hStateMutex);

    BDBG_MSG(( BIP_MSG_PRE_FMT "Exit: hPlayer %p: <-------------------- " BIP_MSG_PRE_ARG, hPlayer ));
} /* processPlayerStateFromPauseTimerCallback */

static BIP_Status processDisconnectedState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    BSTD_UNUSED( threadOrigin );

    /* Not much to do here as the all the cleanup must be done by the previous state before it transitions to the Disconnected state. */
    completionStatus = BIP_SUCCESS;
    if (BIP_Arb_IsBusy(hPlayer->disconnectApi.hArb))
    {
        BIP_Arb_CompleteRequest( hPlayer->disconnectApi.hArb, completionStatus );
    }
    else if (BIP_Arb_IsBusy(hPlayer->destroyApi.hArb))
    {
        BIP_Arb_CompleteRequest( hPlayer->destroyApi.hArb, completionStatus );
    }

    hPlayer->disconnecting = false;
    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Disconnected!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    return (completionStatus);
} /* processDisconnectedState_locked */

static BIP_Status parseDtcpIpContentTypeHeader(
    BIP_PlayerHandle    hPlayer,
    char                *pRespHeaders
    )
{
    BIP_Status completionStatus = BIP_SUCCESS;
    char *pTmp;
    char *pHost, *pPort;

    /*
     * Content-Type: application/x-dtcp1 ; DTCP1HOST=<host> ; DTCP1PORT=<port> ;
     */
    if ( strcasestr(pRespHeaders, DLNA_DTCP_IP_CONTENT_TYPE_HEADER) == NULL)
    {
        BDBG_ERR(( BIP_MSG_PRE_FMT "hPlayer=%p: %s Header is not present in the response even tthough DLNA.ORG_FLAGS sets the link protection flag!" BIP_MSG_PRE_ARG, hPlayer ));
        return (BIP_ERR_PLAYER_MISSING_CONTENT_TYPE);
    }
    if ( (pHost = strcasestr(pRespHeaders, DLNA_DTCP_IP_HOST_NAME)) != NULL)
    {
        pHost += strlen(DLNA_DTCP_IP_HOST_NAME);
        pTmp = strstr(pHost, ";");
        BIP_CHECK_GOTO(( pTmp ), ( "Missing ; before DTCPPORT1 sub-element"), error, BIP_ERR_PLAYER_MISSING_CONTENT_TYPE, completionStatus );
        hPlayer->dlnaFlags.hDtcpIpHostIp = BIP_String_CreateFromCharN(pHost, (pTmp - pHost));
        BIP_CHECK_GOTO(( hPlayer->dlnaFlags.hDtcpIpHostIp ), ( "BIP_String_CreateFromCharN() Failed"), error, BIP_ERR_PLAYER_MISSING_CONTENT_TYPE, completionStatus );
    }

    if ( (pPort = strcasestr(pRespHeaders, DLNA_DTCP_IP_PORT_NAME)) != NULL)
    {
        pPort += strlen(DLNA_DTCP_IP_PORT_NAME);
        pTmp = strstr(pPort, ";");
        BIP_CHECK_GOTO(( pTmp ), ( "Missing ; before ContentFormat sub-element"), error, BIP_ERR_PLAYER_MISSING_CONTENT_TYPE, completionStatus );
        hPlayer->dlnaFlags.hDtcpIpPort = BIP_String_CreateFromCharN(pPort, (pTmp - pPort));
        BIP_CHECK_GOTO(( hPlayer->dlnaFlags.hDtcpIpPort ), ( "BIP_String_CreateFromCharN() Failed"), error, BIP_ERR_PLAYER_MISSING_CONTENT_TYPE, completionStatus );
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: DTCP/IP Host=%s Port=%s" BIP_MSG_PRE_ARG, hPlayer,
                BIP_String_GetString(hPlayer->dlnaFlags.hDtcpIpHostIp), BIP_String_GetString(hPlayer->dlnaFlags.hDtcpIpPort)));
error:
    return (completionStatus);
} /* parseDtcpIpContentTypeHeader */

static BIP_Status parseContentFeatureHeader(
    BIP_PlayerHandle    hPlayer,
    char                *pRespHeaders
    )
{
    BIP_Status completionStatus = BIP_SUCCESS;
    char *pTmp;

    if (pRespHeaders == NULL) return (BIP_SUCCESS); /* Nothing to parse */

    if ( strcasestr(pRespHeaders, DLNA_CONTENT_FEATURES_HEADER_NAME) == NULL )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: ContentFeatures.dlna.org Header is not present in the response!" BIP_MSG_PRE_ARG, hPlayer ));
        return (BIP_SUCCESS);
    }
    /* Search for DLNA.ORG_OP sub-element. */
    if ( (pTmp = strcasestr(pRespHeaders, DLNA_ORG_OP_ELEMENT)) != NULL)
    {
        pTmp += strlen(DLNA_ORG_OP_ELEMENT);
        /* Per DLNA Spec, OP sub-element contains two char, so parse them. */
        if (pTmp[0] == '1') hPlayer->dlnaFlags.timeBasedSeekSupported = true;
        if (pTmp[1] == '1') hPlayer->dlnaFlags.byteBasedSeekSupported = true;
    }

    /* Search for DLNA.ORG_FLAGS sub-element. */
    if ( (pTmp = strcasestr(pRespHeaders, DLNA_ORG_FLAGS_ELEMENT)) != NULL)
    {
        char dlnaOrgFlagsString[16];
        unsigned dlnaOrgFlags;

        pTmp += strlen(DLNA_ORG_FLAGS_ELEMENT);
        /* Per DLNA Spec, only first 8 char are currently valid. */
        strncpy(dlnaOrgFlagsString, pTmp, 8);
        dlnaOrgFlagsString[8] = '\0';
        dlnaOrgFlags = strtoul(dlnaOrgFlagsString, NULL, 16);
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_SENDER_PACED) hPlayer->dlnaFlags.senderPacingSupported = true;
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_LINK_PROTECTED_CONTENT) hPlayer->dlnaFlags.linkProtectionSupported = true;
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_So_INCREASING) hPlayer->dlnaFlags.s0Increasing = true;
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_Sn_INCREASING) hPlayer->dlnaFlags.sNIncreasing = true;
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_LIMITED_OPERATIONS_TIME_SEEK) hPlayer->dlnaFlags.timeBasedSeekSupported = true;
        if (dlnaOrgFlags & DLNA_ORG_FLAGS_LIMITED_OPERATIONS_BYTE_SEEK) hPlayer->dlnaFlags.byteBasedSeekSupported = true;

        if (hPlayer->dlnaFlags.linkProtectionSupported) { hPlayer->dtcpIpEnabled = true; }
        {
            if (getenv("disableDtcpIp") != NULL)
            {
                hPlayer->dtcpIpEnabled = false;
                BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: DtcpIp Support is disabled as dtcpIpEnabled environment variable is set!" BIP_MSG_PRE_ARG, hPlayer ));
            }
        }
    }

    /* Search for DLNA.ORG_PS sub-element. */
    if ( strcasestr(pRespHeaders, DLNA_ORG_PLAYSPEED_ELEMENT) != NULL)
    {
        hPlayer->dlnaFlags.playSpeedSupported = true;
    }
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: ContentFeatures.dlna.org Flags: timeSeek=%s byteSeek=%s senderPacing=%s dtcpIp=%s s0Increasing=%s sNIncreasing=%s!" BIP_MSG_PRE_ARG, hPlayer,
                hPlayer->dlnaFlags.timeBasedSeekSupported?"Y":"N",
                hPlayer->dlnaFlags.byteBasedSeekSupported?"Y":"N",
                hPlayer->dlnaFlags.senderPacingSupported?"Y":"N",
                hPlayer->dlnaFlags.linkProtectionSupported ?"Y":"N",
                hPlayer->dlnaFlags.s0Increasing?"Y":"N",
                hPlayer->dlnaFlags.sNIncreasing?"Y":"N"
                ));
    return (completionStatus);
} /* parseContentFeatureHeader */

/* TODO: remove this code once HttpRequest fully supports API to parse availableSeekRange Header */
#include <inttypes.h>   /* For PRIu64 printf format. */
/*****************************************************************************
 *  Local function to parse an "npt-time" from a "TimeSeekRange.dlna.org"                                        .
 *  header.
 *****************************************************************************/
static BIP_Status BIP_HttpRequest_ParseNptTime(
    const char     *pCh,            /* Pointer to string to parse (might not be null-terminated). */
    size_t          lenCh,          /* Length of string that pCh points to. */
    size_t         *pIdx,           /* [in/out] Current index in to pCh. */
    uint64_t       *pNptTimeInMs    /* Caller's variable to receive the result. */
    )
{
    BIP_Status            rc = BIP_SUCCESS;

    size_t       idx = *pIdx;

    uint64_t     parsedValue;

    uint64_t     hoursValue      = 0;
    uint64_t     minutesValue    = 0;
    uint64_t     secondsValue    = 0;
    uint64_t     msecondsValue   = 0;

    /* Now we should be at the start of an "npt-start-time". The format might be either
     * <seconds>[.<fraction>] or <hours>:<minutes>:<seconds>[.fraction>].   We won't know
     * which format until after we parse the first integer. */
    parsedValue=0;
    for ( ; isdigit(pCh[idx]) ; idx++)
    {
        uint64_t val;

        val = pCh[idx] - '0';
        /* BDBG_MSG(( BIP_MSG_PRE_FMT "parsedValue: %"PRIu64"  (BIP_U64_MAX-val)/10 : %"PRIu64"" BIP_MSG_PRE_ARG, parsedValue, (BIP_U64_MAX-val)/10 )); */
        if (parsedValue > ((BIP_U64_MAX-val)/10)) {           /* Check for integer overflow. */
            goto overflow;
        }
        parsedValue = (parsedValue*10) + val;
    }
    BDBG_MSG(( BIP_MSG_PRE_FMT "parsedValue: %"PRIu64"" BIP_MSG_PRE_ARG, parsedValue ));

    /* If we've come to a period, we must have this format: "<seconds>[.<fraction>]". */
    if (pCh[idx] == '.')
    {
        size_t    decimalPointIdx = idx++;
        uint32_t decimalFactor = 100;

        BDBG_MSG(( BIP_MSG_PRE_FMT "Found '.', npt format is \"<seconds>[.<fraction>]\"" BIP_MSG_PRE_ARG));
        BDBG_MSG(( BIP_MSG_PRE_FMT "idx=%u pCh[idx]=%c" BIP_MSG_PRE_ARG, idx, pCh[idx]));

        secondsValue = parsedValue;    /* parsedValue is seconds. */

        /* We have seconds, get the fractional seconds. */
        parsedValue=0;
        for ( ; isdigit(pCh[idx]) ; idx++)
        {
            uint64_t val;

            if (idx - decimalPointIdx >3) {break;}     /* Only allow 3 digits after decimal point. */

            val = pCh[idx] - '0';
            BDBG_MSG(( BIP_MSG_PRE_FMT "parsedValue: %"PRIu64"  (BIP_U64_MAX-val)/10 : %"PRIu64"" BIP_MSG_PRE_ARG, parsedValue, (BIP_U64_MAX-val)/10 ));
            if (parsedValue > (BIP_U64_MAX-(val*decimalFactor))) {       /* Check for integer overflow. */
                goto overflow;
            }
            parsedValue = (parsedValue) + val*decimalFactor;
            decimalFactor /= 10;
        }

        msecondsValue = parsedValue;      /* parsedValue is in milliseconds, add them to result. */
        BDBG_MSG(( BIP_MSG_PRE_FMT "msecondsValue: %"PRIu64"" BIP_MSG_PRE_ARG, msecondsValue ));
    }
    /* If we've come to a colon, we must have this format: "<hours>:<minutes>:<seconds>[.fraction>]". */
    else if (pCh[idx] == ':')
    {
        size_t    colonIdx = idx++;
        size_t    decimalPointIdx;

        BDBG_MSG(( BIP_MSG_PRE_FMT "Found ':', npt format is \"<hours>:<minutes>:<seconds>[.fraction>]\"" BIP_MSG_PRE_ARG));

        hoursValue = parsedValue;

        /* We already have the hours, not get the minutes. */
        parsedValue=0;
        for ( ; isdigit(pCh[idx]) ; idx++)
        {
            uint64_t val;

            if (idx - colonIdx >2) {break;}     /* Only allow 2 digits after colon. */

            val = pCh[idx] - '0';
            if (parsedValue > (BIP_U64_MAX-val)/10) {       /* Check for integer overflow. */
                goto overflow;
            }
            parsedValue = (parsedValue*10) + val;
        }

        /* There should be a colon after the minutes. */
        if (pCh[idx] != ':') goto parseError;

        colonIdx = idx++;

        minutesValue = parsedValue;

        /* We have hours and minutes, now get the seconds. */
        parsedValue=0;
        for ( ; isdigit(pCh[idx]) ; idx++)
        {
            uint64_t val;

            if (idx - colonIdx >2) {break;}     /* Only allow 2 digits after colon. */

            val = pCh[idx] - '0';
            if (parsedValue > (BIP_U64_MAX-val)/10) {       /* Check for integer overflow. */
                goto overflow;
            }
            parsedValue = (parsedValue*10) + val;
        }

        secondsValue = parsedValue;

        /* There might be a decimal point after the seconds, if so, then get the fractional seconds. */
        if (pCh[idx] == '.')
        {
            uint32_t decimalFactor = 100;

            decimalPointIdx = idx++;

            decimalFactor = 100;
            parsedValue=0;
            for ( ; isdigit(pCh[idx]) ; idx++)
            {
                uint64_t val;

                if (decimalPointIdx - idx >3) {break;}     /* Only allow 3 digits after decimal point. */

                val = pCh[idx] - '0';
                if (parsedValue > (BIP_U64_MAX-val)/10) {       /* Check for integer overflow. */
                    goto overflow;
                }
                parsedValue = (parsedValue) + val*decimalFactor;
                decimalFactor /= 10;
            }

        msecondsValue = parsedValue;
        }
    }

    /* If we've come to some other non-digit, we must have this format: "<seconds>[.<fraction>]",
     * without the optional decimal point. */
    else
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "Found unrecoginzed terminator, npt format is \"<seconds>[.<fraction>]\"" BIP_MSG_PRE_ARG));

        secondsValue = parsedValue;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT "Summing up: hoursValue: %"PRIu64" minutesValue: %"PRIu64" secondsValue: %"PRIu64" msecondsValue: %"PRIu64""
               BIP_MSG_PRE_ARG, hoursValue, minutesValue, secondsValue, msecondsValue));

    /* Now combine the different units into a total number of milliseconds. */
    if (hoursValue > (BIP_U64_MAX-minutesValue)/60) {goto overflow;}
    minutesValue = (hoursValue * 60) + minutesValue;

    if (minutesValue > (BIP_U64_MAX-secondsValue)/60) {goto overflow;}
    secondsValue = (minutesValue * 60) + secondsValue;

    if (secondsValue > (BIP_U64_MAX-msecondsValue)/1000) {goto overflow;}
    msecondsValue = (secondsValue * 1000) + msecondsValue;

    BDBG_MSG(( BIP_MSG_PRE_FMT "Final answer. NPT time in milliseconds: %"PRIu64".%03u"
               BIP_MSG_PRE_ARG, msecondsValue/1000, msecondsValue%1000));

    if (msecondsValue == BIP_U64_MAX) {
        BDBG_ERR((BIP_MSG_PRE_FMT  "Result: %"PRIu64" exceeds allowed maximum of %"PRIu64""
                  BIP_MSG_PRE_ARG, msecondsValue, BIP_U64_MAX-1 ));
        goto parseError;
    }

    *pNptTimeInMs = msecondsValue;
    *pIdx = idx;
    return(rc);

overflow:
    BDBG_ERR((BIP_MSG_PRE_FMT  "Error: 64-bit overflow while parsing NPT-time" BIP_MSG_PRE_ARG ));

parseError:

    BDBG_ERR((BIP_MSG_PRE_FMT "Error parsing NPT time: %*s" BIP_MSG_PRE_ARG, lenCh, pCh));
    BDBG_ERR((BIP_MSG_PRE_FMT "Parse failed here:      %*s" BIP_MSG_PRE_ARG, 1 + idx, "^"));
    *pIdx = idx;
    return(BIP_ERR_HTTP_MESSAGE_INVALID);
}

static BIP_Status parseAvailableSeekRangeHeader(
    BIP_PlayerHandle            hPlayer,
    char                        *pRespHeaders,
    BIP_PlayerStatusFromServer  *pServerStatus
    )
{
    BIP_Status completionStatus = BIP_INF_NOT_AVAILABLE;

    hPlayer->dlnaFlags.availableSeekRangeSupported = false;

    /* Check if availableSeekRange header is present in the response & parse it. */
    /* Example header: availableSeekRange.dlna.org:1 npt=0:0:0.000-0:59:8.021 */
    if ( strcasestr(pRespHeaders, DLNA_AVAILABLE_SEEK_RANGE_HEADER_NAME) == NULL )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: AvailableSeekRange.dlna.org Header is not present in the response!" BIP_MSG_PRE_ARG, hPlayer ));
        completionStatus = BIP_INF_NOT_AVAILABLE;
    }
    else
    {
        char *pNpt;

        pServerStatus->availableSeekRange.firstPositionInMs = 0;
        pServerStatus->availableSeekRange.lastPositionInMs = hPlayer->streamInfo.durationInMs;
        if ( (pNpt = strcasestr(pRespHeaders, "npt=")))
        {
            size_t firstIdx=0, lastIdx=0;
            char *pLast = NULL;
            uint64_t positionInMs;

            firstIdx = strlen("npt=");
            completionStatus = BIP_HttpRequest_ParseNptTime(pNpt, strlen(pNpt), &firstIdx, &positionInMs );
            if (completionStatus == BIP_SUCCESS)
            {
                pServerStatus->availableSeekRange.firstPositionInMs = (NEXUS_PlaybackPosition)positionInMs;
                BDBG_MSG(( ">>>>>> firstIndex=%u firstPositionInMs=%u %s" , firstIdx, pServerStatus->availableSeekRange.firstPositionInMs, pNpt));
                if ( (pLast = strstr(pNpt, "-")))
                {
                    lastIdx = pLast - pNpt + 1;
                    completionStatus = BIP_HttpRequest_ParseNptTime(pNpt, strlen(pNpt), &lastIdx, &positionInMs );
                    if (completionStatus == BIP_SUCCESS)
                    {
                        pServerStatus->availableSeekRange.lastPositionInMs = (NEXUS_PlaybackPosition)positionInMs;
                        /* TODO: what happens in the rewind case? */
                        pServerStatus->availableSeekRange.durationInMs  = pServerStatus->availableSeekRange.lastPositionInMs > pServerStatus->availableSeekRange.firstPositionInMs ?
                            pServerStatus->availableSeekRange.lastPositionInMs - pServerStatus->availableSeekRange.firstPositionInMs:
                            pServerStatus->availableSeekRange.firstPositionInMs - pServerStatus->availableSeekRange.lastPositionInMs;
                        BDBG_MSG(( ">>>>>> lastIdx=%u lastPositionInMs=%u durationInMs=%u", lastIdx,
                                    pServerStatus->availableSeekRange.lastPositionInMs,
                                    pServerStatus->availableSeekRange.durationInMs ));
                        hPlayer->dlnaFlags.availableSeekRangeSupported = true;
                    }
                }
            }
        }
    }
    return (completionStatus);

} /* parseAvailableSeekRangeHeader */

static BIP_Status startApiTimer(
    BIP_PlayerHandle hPlayer,
    int timeoutInMs
    )
{
    BIP_Status bipStatus;

    if (timeoutInMs >= 0)
    {
        /* timer logic is enabled for non-blocking API invocation (== 0) or API w/ a timeout case (timeout > 0) */
        B_Time_Get(&hPlayer->apiStartTime);

        if (timeoutInMs > 0)
        {
            BIP_TimerCreateSettings timerCreateSettings;

            /* setup a timer callback only for +ve timeout value */
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Starting timer for %ld ms" BIP_MSG_PRE_ARG, hPlayer, timeoutInMs ));
            timerCreateSettings.input.callback    = processPlayerStateFromApiTimerCallback;
            timerCreateSettings.input.pContext    = hPlayer;
            timerCreateSettings.input.timeoutInMs = timeoutInMs;
            hPlayer->hApiTimer = BIP_Timer_Create(&timerCreateSettings);
            BIP_CHECK_GOTO(( hPlayer->hApiTimer ), ( "BIP_Timer_Create Failed"), error, BIP_ERR_INTERNAL, bipStatus );
        }
        bipStatus = BIP_SUCCESS;
    }
    else
    {
        /* No need to create a timer. */
        bipStatus = BIP_SUCCESS;
    }
error:
    return (bipStatus);
} /* startApiTimer */

static bool hasApiTimerExpired(
    BIP_PlayerHandle hPlayer,
    int timeoutInMs
    )
{
    B_Time timeNow;
    int elapsedTimeInMs;

    if ( timeoutInMs >= 0 )
    {
        B_Time_Get(&timeNow);
        elapsedTimeInMs = B_Time_Diff( &timeNow, &hPlayer->apiStartTime );
        if ( elapsedTimeInMs >= timeoutInMs )
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "API time expired: apiTimeoutInMs=%d elapsedTimeInMs=%d "
                       BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), timeoutInMs, elapsedTimeInMs ));
            return true;
        }
        else
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "API still in-progress, apiTimeoutInMs=%d elapsedTimeInMs=%d "
                       BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), timeoutInMs, elapsedTimeInMs ));
            return false;
        }
    }
    else
    {
        return false;
    }
} /* hasApiTimerExpired */

static void stopApiTimer(
    BIP_PlayerHandle hPlayer
    )
{
    if ( hPlayer->hApiTimer )
    {
        BIP_Timer_Destroy( hPlayer->hApiTimer );
        hPlayer->hApiTimer = NULL;
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Cancelled API Timer" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    }
}

static BIP_Status createHttpResponseObjectFromResponseHeaders(
    BIP_PlayerHandle        hPlayer,
    BIP_HttpResponseHandle  *phHttpResponse,
    const char              *pResponseHeader
    )
{
    BIP_HttpResponseHandle hHttpResponse = NULL;
    BIP_Status brc = BIP_SUCCESS;
    bool deserializeComplete;
    size_t  consumeSize=0;

    hHttpResponse = BIP_HttpResponse_Create(hPlayer, NULL);
    BIP_CHECK_GOTO(( hHttpResponse ), ( "BIP_HttpResponse_Create Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, brc );

    brc = BIP_HttpResponse_DeserializeFromBuffer(
            hHttpResponse,
            pResponseHeader,
            strlen(pResponseHeader),
            &deserializeComplete,
            &consumeSize
            );
    BIP_CHECK_GOTO(( brc == BIP_SUCCESS ), ( "BIP_HttpResponse_DeserializeFromBuffer Failed"), error, brc, brc );

    if((deserializeComplete == false) || (consumeSize == 0))
    {
        brc = BIP_ERR_INTERNAL;
        goto error;
    }
    *phHttpResponse = hHttpResponse;

    {
        bool printEnabled = false;
        /* The following two prints will set the printEnabled flag if the BDBG module is enabled. */
        BIP_MSG_SUM(( BIP_MSG_PRE_FMT "HTTP Response:" BIP_MSG_PRE_ARG));
        BIP_MSG_SUM(( BIP_MSG_PRE_FMT "Printing HTTP Response: %p %s"
                    BIP_MSG_PRE_ARG, hHttpResponse, (true?(printEnabled=true,""):"") ));
        if (printEnabled)
        {
            brc = BIP_HttpResponse_Print(hHttpResponse, NULL, NULL);
            BIP_CHECK_GOTO(( brc == BIP_SUCCESS ), ( "BIP_HttpResponse_Print Failed"), error, brc, brc );
        }
    }
    return brc;
error:
    if(hHttpResponse)
    {
        BIP_HttpResponse_Destroy(hHttpResponse, hPlayer);
    }
    return brc;
}

static BIP_Status processConnectingState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    BIP_PlayerConnectSettings *pSettings;

    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    if ( hPlayer->subState == BIP_PlayerSubState_eConnectingNew )
    {
        /* We are here because we have a new _Connect() API called from App. Initiate Connect related processing. */

        /* Note: BIP_Player_Connect() has validated the required API arguments. */
        pSettings = hPlayer->connectApi.pSettings;
        hPlayer->connectSettings = *pSettings;
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Accepted _Connect Arb: Start connect processing: url=%s"
                   BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->connectApi.pUrl ));
        BDBG_MSG(( BIP_MSG_PRE_FMT "ConnectSettings: pUserAgent=%s addHdr=%s netJitterInMs=%u hHeap=%p hDma=%p dtcpIp=%s netIf=%s"
                   BIP_MSG_PRE_ARG, pSettings->pUserAgent, pSettings->pAdditionalHeaders, pSettings->maxIpNetworkJitterInMs, pSettings->hHeap, pSettings->hDma, pSettings->enableDtcpIp?"Y":"N", pSettings->pNetworkInterfaceName ));

        /* Create a URL object */
        {
            hPlayer->hUrl = BIP_Url_Create( hPlayer->connectApi.pUrl );
            BIP_CHECK_GOTO(( hPlayer->hUrl), ( "BIP_Url_Create Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, completionStatus );
            BDBG_MSG((BIP_URL_PRINTF_FMT, BIP_URL_PRINTF_ARG(hPlayer->hUrl) ));
            BIP_CHECK_GOTO(( hPlayer->hUrl->host), ( "Invalid URL: host is not specified: url=%s", hPlayer->connectApi.pUrl), error, BIP_ERR_INVALID_PARAMETER, completionStatus );
        }
        /* Start API timer if enabled by the caller. */
        completionStatus = startApiTimer(hPlayer, hPlayer->connectSettings.timeoutInMs);
        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "startApiTimer Failed"), error, completionStatus, completionStatus );

        /* Use PBIP SessionOpen API to TCP Connect, Send Request, Recv Response from Server. */
        {
            hPlayer->pbipState.hPlaybackIp = B_PlaybackIp_Open(NULL);
            BIP_CHECK_GOTO(( hPlayer->pbipState.hPlaybackIp ), ( "B_PlaybackIp_Open Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, completionStatus );

            B_PlaybackIp_GetDefaultSessionOpenSettings(&hPlayer->pbipState.ipSessionOpenSettings);
            memset( &hPlayer->pbipState.ipSessionOpenStatus, 0, sizeof(hPlayer->pbipState.ipSessionOpenStatus) );
            /* Set Server. */
            if (!hPlayer->hUrl->userinfo)
            {
                strncpy(hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.ipAddr, hPlayer->hUrl->host, sizeof(hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.ipAddr)-1);
            }
            else
            {
                BIP_StringHandle hUserInfoAndServer;

                hUserInfoAndServer = BIP_String_CreateFromPrintf("%s@%s", hPlayer->hUrl->userinfo, hPlayer->hUrl->host);
                BIP_CHECK_GOTO(( hUserInfoAndServer ), ( "BIP_String_CreateFromPrintf() failed to build the UserInfo & Host Name string for URL=%s", hPlayer->connectApi.pUrl), error, BIP_ERR_INVALID_PARAMETER, completionStatus );
                strncpy(hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.ipAddr, BIP_String_GetString(hUserInfoAndServer), sizeof(hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.ipAddr)-1);
                BIP_String_Destroy(hUserInfoAndServer);
            }

            hPlayer->playerProtocol = mapSchemeToBipProtocol(hPlayer->hUrl->scheme);
            BIP_CHECK_GOTO(( hPlayer->playerProtocol < BIP_PlayerProtocol_eMax), ( "Unsupported URL=%s", hPlayer->connectApi.pUrl), error, BIP_ERR_INVALID_PARAMETER, completionStatus );
            hPlayer->pbipState.protocol = mapSchemeToPBipProtocol(hPlayer->hUrl->scheme);
            hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.protocol = hPlayer->pbipState.protocol;
            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp || hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
            {
                /* Only require path to be valid for HTTP based URLs, we can be forgiving for non-HTTP protocols like UDP/RTP. */
                BIP_CHECK_GOTO(( hPlayer->hUrl->path), ( "Invalid URL: path is not specified: url=%s", hPlayer->connectApi.pUrl), error, BIP_ERR_INVALID_PARAMETER, completionStatus );
            }

            if (hPlayer->hUrl->port)
            {
                hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.port = strtoul(hPlayer->hUrl->port, NULL, 0);
            }
            else
            {
                if (hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.protocol == B_PlaybackIpProtocol_eHttp)
                {
                    hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.port = 80;
                }
                else if (hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.protocol == B_PlaybackIpProtocol_eHttps)
                {
                    hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.port = 443;
                }
                else
                {
                    completionStatus = BIP_ERR_INVALID_PARAMETER;
                    BDBG_ERR(( BIP_MSG_PRE_FMT "hPlayer=%p Can't determine default port for this URL scheme=%s" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hUrl->scheme));
                    goto error;
                }
            }

            {
                BIP_SslClientFactoryStatus status;

                /* Check if SSLClent Factory has been initialized. */
                completionStatus = BIP_SslClientFactory_GetStatus(&status);
                if (strcasecmp(hPlayer->hUrl->scheme, "https") == 0 || hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.port == 443)
                {
                    BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "BIP_SslClientFactory_GetStatus() Failed for HTTPs type URL"), error, completionStatus, completionStatus );
                    hPlayer->pbipState.ipSessionOpenSettings.security.securityProtocol = B_PlaybackIpSecurityProtocol_Ssl;
                    hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.protocol = B_PlaybackIpProtocol_eHttp;
                    hPlayer->sslEnabled = true;
                    hPlayer->pbipState.ipSessionOpenSettings.security.initialSecurityContext = status.pPbipSslCtx;
                    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: SSL enabled!" BIP_MSG_PRE_ARG, hPlayer ));
                }
                else
                {
                    /* Irrespective of the BIP_SslClientFactory_GetStatus status, we just store the returned context. */
                    hPlayer->pbipState.ipSessionOpenSettings.security.initialSecurityContext = status.pPbipSslCtx;
                }
            }

            /* Create URL String as PBIP needs it w/o the Host name. */
            {
                if (hPlayer->hUrl->query)
                {
                    hPlayer->pbipState.hUrl = BIP_String_CreateFromPrintf("%s?%s%s",
                            hPlayer->hUrl->path,
                            hPlayer->hUrl->query ? hPlayer->hUrl->query:"",
                            hPlayer->hUrl->fragment ? hPlayer->hUrl->fragment:""
                            );
                }
                else
                {
                    hPlayer->pbipState.hUrl = BIP_String_CreateFromPrintf("%s", hPlayer->hUrl->path);
                }
                BIP_CHECK_GOTO(( hPlayer->pbipState.hUrl ), ( "BIP_String_CreateFromPrintf Failed for building the URL" ), error, completionStatus, completionStatus );
                BDBG_MSG(("url path %s", BIP_String_GetString(hPlayer->pbipState.hUrl)));
                hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.url = (char *)BIP_String_GetString(hPlayer->pbipState.hUrl);
            }

            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp)
            {
                hPlayer->pbipState.ipSessionOpenSettings.u.http.copyResponseHeaders = true;
                hPlayer->pbipState.ipSessionOpenSettings.u.http.userAgent = (char *)hPlayer->connectSettings.pUserAgent;
                hPlayer->pbipState.ipSessionOpenSettings.u.http.networkBufferSize = (4*1024*1024); /* Setting it to 4MB. */
                if (hPlayer->connectSettings.dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess)
                {
                    /* In addition to getContentFeatures, we also get the availableSeekRange header as part of the additional headers. */
                    hPlayer->hAdditionalHeaders = BIP_String_CreateFromPrintf("getAvailableSeekRange.dlna.org: 1\r\ngetContentFeatures.dlna.org: 1\r\n%s",
                            hPlayer->connectSettings.pAdditionalHeaders ? hPlayer->connectSettings.pAdditionalHeaders : "");
                }
                else
                {
                    /* Add getContentFeatures header as part of the additional headers. */
                    hPlayer->hAdditionalHeaders = BIP_String_CreateFromPrintf("getContentFeatures.dlna.org: 1\r\n%s",
                            hPlayer->connectSettings.pAdditionalHeaders ? hPlayer->connectSettings.pAdditionalHeaders : "");
                }
                if (hPlayer->hAdditionalHeaders)
                {
                    hPlayer->pbipState.ipSessionOpenSettings.u.http.additionalHeaders = (char *)BIP_String_GetString(hPlayer->hAdditionalHeaders);
                    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: additionalHeaders=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_String_GetString(hPlayer->hAdditionalHeaders) ));
                }
                else
                {
                    BIP_CHECK_GOTO(( hPlayer->hAdditionalHeaders ), ( "BIP_String_CreateFromPrintf Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, completionStatus );
                }
            }

            if (pSettings->pNetworkInterfaceName)
            {
                hPlayer->hNetworkInterfaceName = BIP_String_CreateFromPrintf("%s", pSettings->pNetworkInterfaceName);
                BIP_CHECK_GOTO(( hPlayer->hNetworkInterfaceName ), ( "BIP_String_CreateFromPrintf Failed for pNetworkInterfaceName=%s", pSettings->pNetworkInterfaceName), error, completionStatus, completionStatus );
                hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.interfaceName = (char *)BIP_String_GetString(hPlayer->hNetworkInterfaceName);
            }

            {
                hPlayer->pbipState.ipSessionOpenSettings.nonBlockingMode = true;
                hPlayer->pbipState.ipSessionOpenSettings.eventCallback = pbipCallback;
                hPlayer->pbipState.ipSessionOpenSettings.appCtx = hPlayer;
            }
#define NETWORK_TIMEOUT_IN_SEC 5
            /* TODO: need to align PBIP timer w/ the timeout value from the connect settings */
            hPlayer->pbipState.ipSessionOpenSettings.networkTimeout = NETWORK_TIMEOUT_IN_SEC;  /* timeout during network outage events */

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p server=%s port=%u proto=%d secProto=%d url=%s" BIP_MSG_PRE_ARG, hPlayer,
                        hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.ipAddr,
                        hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.port,
                        hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.protocol,
                        hPlayer->pbipState.ipSessionOpenSettings.security.securityProtocol,
                        hPlayer->pbipState.ipSessionOpenSettings.socketOpenSettings.url
                     ));

            hPlayer->pbipState.ipSessionOpenSettings.security.dmaHandle = hPlayer->hDma;

            hPlayer->pbipState.status = B_PlaybackIp_SessionOpen(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipSessionOpenSettings, &hPlayer->pbipState.ipSessionOpenStatus);
            BIP_CHECK_GOTO(( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS || hPlayer->pbipState.status == B_ERROR_SUCCESS), ( "B_PlaybackIp_SessionOpen Failed"), error, BIP_ERR_PLAYER_CONNECT, completionStatus );

            /* Change subState to indicate that we are now waiting for Server Response (which we will check below). */
            hPlayer->subState = BIP_PlayerSubState_eConnectingWaitForResponse;
        } /* PBIP SessionOpen call. */
    } /* subState == eNewConnect */

    /*
     * We get here for following reasons:
     *
     *  App had just called _Connect(), we started eNewConnect state processing above (invoked a PBIP above in async mode)
     *  and now will check for its completion. Or,
     *
     *  PBIP had finished processing our previous call, invoked our callback which is now running this function. Or,
     *
     *  We had a timeout event before PBIP could finish the call. Or,
     *
     *  We had initiaited DtcpIp AKE operation and its completion callback is running the state machine.
     *
     *
     *  In all of these cases, based on our current sub-state, we will check for completion of previous operation.
     *  In all of these cases, we will check the async status from PBIP.  It can either still be IN_PROGRESS or done w/ SUCCESS or ERROR.
     *  The later two cases will allow us to complete this API.
     */
    if ( hPlayer->subState == BIP_PlayerSubState_eConnectingWaitForResponse )
    {
        if ( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS )
        {
            /* Poll again to get the latest PBIP API status. */
            hPlayer->pbipState.status = B_PlaybackIp_SessionOpen(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipSessionOpenSettings, &hPlayer->pbipState.ipSessionOpenStatus);
        }

        /* Check if we successfuly completed previous PBIP operation & process its results. */
        if ( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS )
        {
            /* PBIP Call is still in-progress, check if API has timed out. */
            completionStatus = BIP_INF_IN_PROGRESS;
            if ( hasApiTimerExpired(hPlayer, hPlayer->connectSettings.timeoutInMs) == true )
            {
                /* API timedout, set status & move to Done state. We will complete the API Arb below. */
                completionStatus = BIP_INF_TIMEOUT;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Timedout while waiting for PBIP SessionOpen() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else if (hPlayer->disconnecting)
            {
                /* App has called _Disconnect() or _Destroy(), so complete this state. */
                completionStatus = BIP_ERR_PLAYER_BEING_DISCONNECTED;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Player is being Disconnected while waiting for PBIP SessionOpen() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Still waiting for PBIP SessionOpen() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                completionStatus = BIP_INF_IN_PROGRESS;
            }
        }
        else if ( hPlayer->pbipState.status == B_ERROR_SUCCESS )
        {
            /* PBIP Call has successfuly completed, so lets do the remaining success related processing. */
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionOpen() successful" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            if ( hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp )
            {
                completionStatus = createHttpResponseObjectFromResponseHeaders(
                                            hPlayer,
                                            &hPlayer->hHttpResponse,
                                            hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders
                                            );
                BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "createHttpResponseObjectFromResponseHeaders Failed"), error, completionStatus, completionStatus );

                if ( hPlayer->connectSettings.phHttpResponse )
                {
                    *hPlayer->connectSettings.phHttpResponse = hPlayer->hHttpResponse;
                }

                /* Parse DLNA Media Transport related Headers. */
                {
                    /* Parse the DLNA ContentFeatures Header. */
                    completionStatus = parseContentFeatureHeader(hPlayer, hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders);
                    BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "parseContentFeatureHeader Failed"), error, completionStatus, completionStatus );

                    /* Parse the DLNA AvailableSeekRange Header if it is available in the response (because either we had specified it or app it via additional headers). */
                    {
                        completionStatus = parseAvailableSeekRangeHeader(hPlayer, hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders, &hPlayer->getStatusFromServerState.serverStatus);
                        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS || completionStatus == BIP_INF_NOT_AVAILABLE), ( "parseAvailableSeekRangeHeader Failed!"), error, completionStatus, completionStatus );
                        if (completionStatus == BIP_SUCCESS)
                        {
                            B_Time_Get(&hPlayer->getStatusFromServerState.lastUpdateTime);
                            hPlayer->getStatusFromServerState.lastUpdateTimeSet = true;
                            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Successfully parsed availableSeekRange & set the update time!" BIP_MSG_PRE_ARG, hPlayer ));
                        }
                        else if (completionStatus == BIP_INF_NOT_AVAILABLE &&
                                 hPlayer->connectSettings.dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess )
                        {
                            BIP_CHECK_GOTO(( 0 ), ( "hPlayer=%p: Server didn't respond with availableSeekRange header even when App has explicitly asked for it!", hPlayer), error, completionStatus, completionStatus );
                        }
                    }
                }
            }

            /*
             * If Content is going to be DTCP/IP protected, then use the BIP_DtcpIpClient to do Ake in async mode.
             * We will then change subState to reflect this processing.
             */
            if (hPlayer->dtcpIpEnabled)
            {
                BIP_CallbackDesc asyncCallbackDesc;

                completionStatus = parseDtcpIpContentTypeHeader(hPlayer, hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders);
                if (completionStatus == BIP_SUCCESS)
                {
                    asyncCallbackDesc.context = hPlayer;
                    asyncCallbackDesc.callback = asyncApiCallbackFromDtcpIpClient;
                    completionStatus = BIP_DtcpIpClientFactory_DoAkeAsync( BIP_String_GetString(hPlayer->dlnaFlags.hDtcpIpHostIp), BIP_String_GetString(hPlayer->dlnaFlags.hDtcpIpPort), NULL, &hPlayer->hAkeEntry, &asyncCallbackDesc, &hPlayer->dtcpIpAkeCompletionStatus );
                    if (completionStatus != BIP_SUCCESS && completionStatus != BIP_INF_IN_PROGRESS)
                    {
                        BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_DtcpIpClientFactory_DoAkeAsync() Failed with completionStatus=%s"
                                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                        hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                    }
                    else
                    {
                        hPlayer->subState = BIP_PlayerSubState_eConnectingWaitForDtcpIpAke;
                        /* We will check dtcpIpAkeCompletionStatus below in the eConnectingWaitForDtcpIpAke sub-state processing. */
                        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "DTCP/IP is enabled, doing AKE!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                    }
                }
                else
                {
                    BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "parseDtcpIpContentTypeHeader() Failed with completionStatus=%s"
                                BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                }
            }
            else
            {
                /* DTCP/IP is not enabled, so we are done! */
                completionStatus = BIP_SUCCESS;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
            }
        }
        else
        {
            /* PBIP Call returned some error, so reflect it in the completionStatus. */
            completionStatus = BIP_ERR_PLAYER_CONNECT;
            hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
            BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionOpen() Failed with PBIP errorStatus=%d"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->pbipState.status ));
        }
    } /* subState == eConnectingWaitForResponse */

    if ( hPlayer->subState == BIP_PlayerSubState_eConnectingWaitForDtcpIpAke )
    {
        if (hPlayer->dtcpIpAkeCompletionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Check if API has timed out. */
            if ( hasApiTimerExpired(hPlayer, hPlayer->connectSettings.timeoutInMs) == true )
            {
                /* API timedout, set status. We will complete the API Arb below. */
                completionStatus = BIP_INF_TIMEOUT;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Timedout while waiting for DtcpIp AKE completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else if (hPlayer->disconnecting)
            {
                /* App has called _Disconnect() or _Destroy(), so complete this state. */
                completionStatus = BIP_ERR_PLAYER_BEING_DISCONNECTED;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Player is being Disconnected while waiting for DtpcIp AKE completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Still waiting for DtcpIp Ake completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                completionStatus = BIP_INF_IN_PROGRESS;
            }
        }
        else
        {
            /* AKE is finished, completionStatus will indicate sucess/failure. */
            BIP_DtcpIpClientFactoryAkeEntryStatus akeEntryStatus;

            completionStatus = hPlayer->dtcpIpAkeCompletionStatus;
            if (completionStatus == BIP_SUCCESS)
            {
                completionStatus = BIP_DtcpIpClientFactory_GetAkeEntryStatus(hPlayer->hAkeEntry, &akeEntryStatus);
                hPlayer->pDtcpIpLibAkeCtx = akeEntryStatus.pDtcpIpLibAkeCtx;
            }

            hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "DtcpIp AKE completed with status=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
        }
    } /* subState == _eConnectingWaitForDtcpIpAke */

error:
    /* Double check if completionStatus is NOT in progress. This can happen if we get here via a jump to error label. */
    if ( completionStatus != BIP_INF_IN_PROGRESS )
    {
        /* API is done either due to Success or Error status. Move to Done state. completionStatus must be set before this. */
        hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
    }

    /* If subState is Done, then update the states based on the completionStatus. */
    if ( hPlayer->subState == BIP_PlayerSubState_eConnectingDone )
    {
        if ( completionStatus == BIP_SUCCESS )
        {
            /* We are successfuly done w/ _Connect, so lets move to the next state. */
            hPlayer->state = BIP_PlayerState_eConnected;
            hPlayer->subState = BIP_PlayerSubState_eIdle;
            stopApiTimer(hPlayer);
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Connect() successful for url=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->hUrl->urlRaw ));
        }
        else
        {
            /* Error case, disconnect the Player and go back to the state before Connecting. */
            disconnectPlayer( hPlayer );
            hPlayer->state = BIP_PlayerState_eDisconnected;
            hPlayer->subState = BIP_PlayerSubState_eIdle;
            BDBG_ERR(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Connect() Failed, status=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
        }

        /* Check if disconnecting flag is set, meaning _Disconnect()/_Destroy() API must have been called which completed this state. */
        if (hPlayer->disconnecting)
        {
            /* Set a flag to re-run the top processPlayerState() so that it can process the actual _Disconnect()/_Destroy() API. */
            hPlayer->reRunState = true;
        }

        /* And finally complete the Connect Arb, so that caller can be notified of its completion. */
        BDBG_ASSERT(BIP_Arb_IsBusy(hPlayer->connectApi.hArb));
        BIP_Arb_CompleteRequest( hPlayer->connectApi.hArb, completionStatus );
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    return (completionStatus);
} /* processConnectingState_locked */

static BIP_Status processConnectedState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    BDBG_ASSERT(hPlayer->state == BIP_PlayerState_eConnected);
    /* Check if disconnecting flag is set, meaning _Disconnect()/_Destroy() API must have been called which completed this state. */
    if (hPlayer->disconnecting)
    {
        /* Cleanup this state. */
        disconnectPlayer(hPlayer);
        hPlayer->state = BIP_PlayerState_eDisconnected;
        hPlayer->subState = BIP_PlayerSubState_eIdle;
        hPlayer->reRunState = true;  /* Set the re-run variable so that top processPlayerState() can run the processDisconnectedState_locked(). */
        completionStatus = BIP_SUCCESS;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));

    return (completionStatus);
} /* processConnectedState_locked */

static BIP_Status processPreparedState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    BDBG_ASSERT(hPlayer->state == BIP_PlayerState_ePrepared);
    /* Check if disconnecting flag is set, meaning _Disconnect()/_Destroy() API must have been called which completed this state. */
    if (hPlayer->disconnecting)
    {
        /* Cleanup this state. */
        unPrepare(hPlayer);
        hPlayer->state = BIP_PlayerState_eConnected;
        hPlayer->subState = BIP_PlayerSubState_eIdle;
        hPlayer->reRunState = true;  /* Set the re-run variable so that top processPlayerState() can run the processConnectedState_locked(). */
        completionStatus = BIP_SUCCESS;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));

    return (completionStatus);
} /* processPreparedState_locked */

static bool useNexusPlaypumpForIpPlayback(
    bool                    serverSideTrickmodesSupported,
    bool                    timeSeekRangeHeaderEnabled,
    bool                    liveChannel,
    int64_t                 contentLength,
    NEXUS_TransportType     transportType,
    BIP_PlayerContainerType containerType,
    BIP_PlayerProtocol      playerProtocol
    )
{
    /* For RTP, UDP, RTSP based protocols, use Nexus Playpump as data is being pushed by the server and thus can't use the Nexus playback as it pulls data from the source/server. */
    if (playerProtocol != BIP_PlayerProtocol_eSimpleHttp) return true;

    /* Also, for adaptive streaming protocols like HLS & MPEG DASH, we use Playpump as data stream may need to be altered (converted to PES, or packaged in proper fragmented MP4 headers for MPEG DASH). */
    if (containerType == BIP_PlayerContainerType_eHls || containerType == BIP_PlayerContainerType_eMpegDash) return true;

    /* For SimpleHttp protocol (which includes HTTP & HTTPS), we can use either Nexus Playback or Playpump as these are pull based protocols. */
    if ((serverSideTrickmodesSupported) ||    /* Server support Server side trickmodes, we have to use Playpump as Playback always does client side trickmodes. */
        (timeSeekRangeHeaderEnabled && (transportType == NEXUS_TransportType_eTs || transportType == NEXUS_TransportType_eMpeg2Pes) ) || /* If timeSeekRange header is enabled for MPEG2 TS or PES streams, then use Playpump. */
        (!contentLength && (transportType == NEXUS_TransportType_eTs || transportType == NEXUS_TransportType_eMpeg2Pes) ) || /* If content-length is not available but its MPEG2 TS or PES streams, then use Playpump. */
        (liveChannel)
       )
    {
        return true;
    }

    /* By default use the Nexus Playback for all protocols */
    return false;
}

/* Determine Clock Recovery Mode: whether to configure STC Channel in Auto (Pull) or Live (PCR) tracking modes! */
static bool useLiveModeForIpPlayback(
    NEXUS_TransportType             transportType,
    bool                            transportTimestampEnabled,
    bool                            liveChannel,
    BIP_PlayerDataAvailabilityModel dataAvailabilityModel,
    BIP_PlayerProtocol              playerProtocol,
    BIP_PlayerClockRecoveryMode     *clockRecoveryModeForPlaybackIp
    )
{
    bool useLiveIpMode;

    /* Lets default to Pull mode. */
    *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;

    if (transportType != NEXUS_TransportType_eTs)
    {
        /* For non-Mpeg2 Transport streams (ES, ASF, MP4, etc. formats), we choose the non-live mode */
        /* as there is *no* good mechanism for locking to the sender's clock (TTS or PCRs are missing) */
        /* this is true irrespective of the network protocol (HTTP vs. RTP/UDP) */
        *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;
    }
    /* MPEG2 TS content over HTTP or UDP based protocols! */
    else if (playerProtocol == BIP_PlayerProtocol_eSimpleHttp)
    {
        /* For HTTP protocol carrying MPEG2 TS content, we decide pull vs live mode based on dataAvailability & live channel flag. */
        if (dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eFullRandomAccess)
        {
            /* We have full access to the stream (meaning can seek anywhere in it), so we choose pull mode. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;
        }
        else if (dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess)
        {
            /* We have limited access to the stream (true for time-shifted content whether it is paced or not), we choose pull mode. */
            /* Note & TODO: we can look into liveChannel (senderPacing dlna flag) to select the pure live mode, but this needs further work when switching modes during trickmodes. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;
        }
        else if (dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eNoRandomAccess || liveChannel)
        {
            /* We dont have any random access in the stream or live channel flag is set, we select the live mode. */
            if (transportTimestampEnabled)
            {
                /* Stream has 4 byte Timestamps, use TTS based pacing mode. */
                *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithTtsNoSyncSlip;
            }
            else
            {
                /* Live channel & stream doesn't has 4 byte Timestamps, so use PCRs as timestamps for pacing. */
                *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithPcrSyncSlip;
#if 0
                /* PCP Pacing method needs some more work until then, lets use SyncSlip mode. */
                *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip;
#endif
            }
        }
        else
        {
            /* We default to pull mode. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;
        }
    }
    else if (playerProtocol == BIP_PlayerProtocol_eSimpleUdp || playerProtocol == BIP_PlayerProtocol_eRtp || playerProtocol == BIP_PlayerProtocol_eRtsp || liveChannel)
    {
        /* For UDP, RTP, RTSP protocols carrying MPEG2 TS content, we always use live mode. */
        if (transportTimestampEnabled)
        {
            /* Stream has 4 byte Timestamps, use TTS based pacing mode. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithTtsNoSyncSlip;
        }
        else
        {
            /* Live channel & stream doesn't has 4 byte Timestamps, so use PCRs as timestamps for pacing. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithPcrSyncSlip;
#if 0
            /* PCP Pacing method needs some more work until then, lets use SyncSlip mode. */
            *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip;
#endif
        }
    }
    /* MPEG2 TS content in HTTP/HTTPS protocols in non-Live mode */
    else
    {
        /* We default to pull mode. */
        *clockRecoveryModeForPlaybackIp = BIP_PlayerClockRecoveryMode_ePull;
    }

    if (*clockRecoveryModeForPlaybackIp == BIP_PlayerClockRecoveryMode_ePull)
    {
        useLiveIpMode = false;
    }
    else
    {
        useLiveIpMode = true;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT "transportType=%s transportTimestampEnabled=%s liveChannel=%s dataAvailabilityModel=%s playerProtocol=%d useLiveIpMode=%s clockRecoveryMode=%s" BIP_MSG_PRE_ARG,
                BIP_ToStr_NEXUS_TransportType(transportType),
                transportTimestampEnabled?"Y":"N",
                liveChannel?"Y":"N",
                BIP_ToStr_BIP_PlayerDataAvailabilityModel(dataAvailabilityModel),
                playerProtocol,
                useLiveIpMode?"True":"False",
                BIP_ToStr_BIP_PlayerClockRecoveryMode(*clockRecoveryModeForPlaybackIp)
                ));

    return useLiveIpMode;
}

static bool getTrackOfTrackId(
    BIP_MediaInfoHandle     hMediaInfo,
    unsigned                trackId,
    BIP_MediaInfoTrack      *pMediaInfoTrackOut
    )
{
    bool                    trackFound = false;
    BIP_MediaInfoStream     *pMediaInfoStream;
    BIP_MediaInfoTrackGroup *pMediaInfoTrackGroup = NULL;
    bool                    trackGroupPresent = false;
    BIP_MediaInfoTrack      *pMediaInfoTrack;

    pMediaInfoStream = BIP_MediaInfo_GetStream(hMediaInfo);
    BDBG_ASSERT(pMediaInfoStream);
    if (!pMediaInfoStream) return false;

    if (pMediaInfoStream->numberOfTrackGroups != 0)
    {
        pMediaInfoTrackGroup = pMediaInfoStream->pFirstTrackGroupInfo;
        pMediaInfoTrack = pMediaInfoTrackGroup->pFirstTrackForTrackGroup;
        trackGroupPresent = true;
    }
    else
    {
        /* None of the track belongs to any trackGroup, in this case stream out all tracks from mediaInfoStream.*/
        pMediaInfoTrack = pMediaInfoStream->pFirstTrackInfoForStream;
    }

    if (!pMediaInfoTrack) return false;

    while (pMediaInfoTrack)
    {
        if (pMediaInfoTrack->trackId == trackId)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "Found trackType=%s with trackId=0x%x" BIP_MSG_PRE_ARG, BIP_ToStr_BIP_MediaInfoTrackType(pMediaInfoTrack->trackType), pMediaInfoTrack->trackId));
            *pMediaInfoTrackOut = *pMediaInfoTrack;
            trackFound = true;
            break;
        }

        if (true == trackGroupPresent)
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForTrackGroup;
        }
        else
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForStream;
        }
    }
    return (trackFound);
} /* getTrackOfTrackId */

static bool getTrackOfLanguage(
    BIP_MediaInfoHandle hMediaInfo,
    const char          *pLanguage,
    BIP_MediaInfoTrack  *pMediaInfoTrackOut
    )
{
    bool                    trackFound = false;
    BIP_MediaInfoStream     *pMediaInfoStream;
    BIP_MediaInfoTrackGroup *pMediaInfoTrackGroup = NULL;
    bool                    trackGroupPresent = false;
    BIP_MediaInfoTrack      *pMediaInfoTrack;

    pMediaInfoStream = BIP_MediaInfo_GetStream(hMediaInfo);
    BDBG_ASSERT(pMediaInfoStream);
    if (!pMediaInfoStream) return false;

    if (pMediaInfoStream->numberOfTrackGroups != 0)
    {
        pMediaInfoTrackGroup = pMediaInfoStream->pFirstTrackGroupInfo;
        pMediaInfoTrack = pMediaInfoTrackGroup->pFirstTrackForTrackGroup;
        trackGroupPresent = true;
    }
    else
    {
        /* None of the track belongs to any trackGroup, in this case stream out all tracks from mediaInfoStream.*/
        pMediaInfoTrack = pMediaInfoStream->pFirstTrackInfoForStream;
    }

    if (!pMediaInfoTrack) return false;

    while (pMediaInfoTrack)
    {
        BDBG_MSG((BIP_MSG_PRE_FMT "trackId=0x%x lang=%s user lang=%s" BIP_MSG_PRE_ARG, pMediaInfoTrack->trackId, pMediaInfoTrack->info.audio.pLanguage, pLanguage));
        if (pMediaInfoTrack->trackType == BIP_MediaInfoTrackType_eAudio && pMediaInfoTrack->info.audio.pLanguage && strcmp(pMediaInfoTrack->info.audio.pLanguage, pLanguage) == 0)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "Found track of language =%s with type=%s trackId=0x%x" BIP_MSG_PRE_ARG,
                        pLanguage, BIP_ToStr_BIP_MediaInfoTrackType(pMediaInfoTrack->trackType), pMediaInfoTrack->trackId));
            *pMediaInfoTrackOut = *pMediaInfoTrack;
            trackFound = true;
            break;
        }

        if (true == trackGroupPresent)
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForTrackGroup;
        }
        else
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForStream;
        }
    }
    return (trackFound);
} /* getTrackOfLanguage */

static bool getTrackOfType(
    BIP_MediaInfoHandle hMediaInfo,
    BIP_MediaInfoTrackType trackType,
    BIP_MediaInfoTrack *pMediaInfoTrackOut
    )
{
    bool                    trackFound = false;
    BIP_MediaInfoStream     *pMediaInfoStream;
    BIP_MediaInfoTrackGroup *pMediaInfoTrackGroup = NULL;
    bool                    trackGroupPresent = false;
    BIP_MediaInfoTrack      *pMediaInfoTrack;

    pMediaInfoStream = BIP_MediaInfo_GetStream(hMediaInfo);
    BDBG_ASSERT(pMediaInfoStream);
    if (!pMediaInfoStream) return false;

    if (pMediaInfoStream->numberOfTrackGroups != 0)
    {
        pMediaInfoTrackGroup = pMediaInfoStream->pFirstTrackGroupInfo;
        pMediaInfoTrack = pMediaInfoTrackGroup->pFirstTrackForTrackGroup;
        trackGroupPresent = true;
    }
    else
    {
        /* None of the track belongs to any trackGroup, in this case stream out all tracks from mediaInfoStream.*/
        pMediaInfoTrack = pMediaInfoStream->pFirstTrackInfoForStream;
    }

    if (!pMediaInfoTrack) return false;

    while (pMediaInfoTrack)
    {
        if (pMediaInfoTrack->trackType == trackType)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "Found trackType=%s with trackId=0x%x" BIP_MSG_PRE_ARG, BIP_ToStr_BIP_MediaInfoTrackType(trackType), pMediaInfoTrack->trackId));
            *pMediaInfoTrackOut = *pMediaInfoTrack;
            trackFound = true;
            break;
        }

        if (true == trackGroupPresent)
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForTrackGroup;
        }
        else
        {
            pMediaInfoTrack = pMediaInfoTrack->pNextTrackForStream;
        }
    }
    return (trackFound);
} /* getTrackOfType */

static void getProbedStreamInfo(
    BIP_PlayerHandle        hPlayer,
    BIP_PlayerStreamInfo    *pStreamInfo /* [out] */
    )
{
    BIP_MediaInfoStream *pMediaInfoStream;
    BIP_MediaInfoTrack mediaInfoTrack;

    pMediaInfoStream = BIP_MediaInfo_GetStream(hPlayer->hMediaInfo);
    BKNI_Memset(pStreamInfo, 0, sizeof(*pStreamInfo));

    pStreamInfo->durationInMs = pMediaInfoStream->durationInMs;
    pStreamInfo->liveChannel = pMediaInfoStream->liveChannel;
    if (hPlayer->dlnaFlags.senderPacingSupported) BDBG_ASSERT(pStreamInfo->liveChannel);

    pStreamInfo->serverSideTrickmodesSupported = hPlayer->dlnaFlags.playSpeedSupported ? true: false;

    pStreamInfo->avgBitRate = pMediaInfoStream->avgBitRate;
    pStreamInfo->contentLength = pMediaInfoStream->contentLength;

    pStreamInfo->mpeg2Ts.transportTimeStampEnabled = pMediaInfoStream->transportTimeStampEnabled;

    if (getTrackOfType(hPlayer->hMediaInfo, BIP_MediaInfoTrackType_ePcr, &mediaInfoTrack) == true)
    {
        pStreamInfo->mpeg2Ts.pcrPid = mediaInfoTrack.trackId;
    }
    else
    {
        pStreamInfo->mpeg2Ts.pcrPid = UINT_MAX;
    }

    if (getTrackOfType(hPlayer->hMediaInfo, BIP_MediaInfoTrackType_ePmt, &mediaInfoTrack) == true)
    {
        pStreamInfo->mpeg2Ts.pmtPid = mediaInfoTrack.trackId;
    }
    else
    {
        pStreamInfo->mpeg2Ts.pmtPid = UINT_MAX;
    }

    pStreamInfo->timeSeekRangeHeaderEnabled = hPlayer->dlnaFlags.timeBasedSeekSupported;

    if (hPlayer->pbipState.psi.hlsSessionEnabled)
        pStreamInfo->containerType = BIP_PlayerContainerType_eHls;
    else if (hPlayer->pbipState.psi.mpegDashSessionEnabled)
        pStreamInfo->containerType = BIP_PlayerContainerType_eMpegDash;
    else
        pStreamInfo->containerType = BIP_PlayerContainerType_eNexusTransportType;

    pStreamInfo->transportType = pMediaInfoStream->transportType;
    pStreamInfo->usePlaypump = hPlayer->useNexusPlaypump;
    pStreamInfo->usePlayback = hPlayer->useNexusPlaypump?false:true;
    pStreamInfo->dataAvailabilityModel = hPlayer->dataAvailabilityModel;

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "pcrPid=0x%x, duration=%u dataAvailabilityModel=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                pStreamInfo->mpeg2Ts.pcrPid, hPlayer->streamInfo.durationInMs, BIP_ToStr_BIP_PlayerDataAvailabilityModel(pStreamInfo->dataAvailabilityModel) ));
} /* getProbedStreamInfo */

static BIP_Status determineDataAvailabilityModel(
    BIP_PlayerHandle hPlayer
    )
{
    BIP_Status bipStatus = BIP_SUCCESS;

    /* First we determine the data availability model. */
    {
        if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleUdp || hPlayer->playerProtocol == BIP_PlayerProtocol_eRtp)
        {
            hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
        }
        else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
        {
            hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eFullRandomAccess;
        }
        else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp && hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eNexusTransportType)
        {
            /* For non-adaptive containers, we decide based on the DLNA flags. */
            if (hPlayer->dlnaFlags.sNIncreasing)
            {
                /* Note: we ignore to include the s0Increasing flag in this check & treat this progress recording case as Limited Data Availability (sN inceasing) as well. */
                /* Only difference in this case would be that starting position will not change, but since ending will, we will need to fetch the range from the server. */
                /* Server supports limited seek range */
                hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess;
            }
            else if (hPlayer->dlnaFlags.senderPacingSupported && !hPlayer->dlnaFlags.timeBasedSeekSupported && !hPlayer->dlnaFlags.byteBasedSeekSupported)
            {
                /* Server doesn't support limited seek range, is pacing the content & doesn't support time or byte based seeks, so it is pure live w/o time shifting. */
                hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
            }
            else if (hPlayer->dlnaFlags.timeBasedSeekSupported || hPlayer->dlnaFlags.byteBasedSeekSupported)
            {
                /* Neither Server supports limited seek range nor is pacing the content, so it is full availability model if either of DLNA_OP flags are set. */
                hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eFullRandomAccess;
            }
            else
            {
                /* No explicit flags set, no point in checking for accept-ranges header as not all server's send it. So lets try the full access model. */
                hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eFullRandomAccess;
            }
        }
        else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp && hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls)
        {
            B_PlaybackIpError brc;
            B_PlaybackIpStatus pbipStatus;

            brc = B_PlaybackIp_GetStatus(hPlayer->pbipState.hPlaybackIp, &pbipStatus);
            if (brc == B_ERROR_SUCCESS)
            {
                hPlayer->dataAvailabilityModel = pbipStatus.hlsStats.bounded ? BIP_PlayerDataAvailabilityModel_eFullRandomAccess : BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess;
            }
            else
            {
                hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
                BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Failed to get B_PlaybackIp_GetStatus, using NoRandomAccess model: brc =%d " BIP_MSG_PRE_ARG, hPlayer, brc));
            }
        }
        else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp && hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eMpegDash)
        {
            hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Trickmodes are not yet supported for MpegDash containerType, using NoRandomAccess model" BIP_MSG_PRE_ARG, hPlayer));
        }
        else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
        {
            hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Trickmodes are not yet supported for RTSP protocol, using NoRandomAccess model" BIP_MSG_PRE_ARG, hPlayer));
        }
        else
        {
            hPlayer->dataAvailabilityModel = BIP_PlayerDataAvailabilityModel_eNoRandomAccess;
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Using NoRandomAccess model for playerPtocol=%d container=%s" BIP_MSG_PRE_ARG, hPlayer,
                        hPlayer->playerProtocol, BIP_ToStr_BIP_PlayerContainerType(hPlayer->streamInfo.containerType)));
        }

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: app provided dataModel=%s player selected dataModel=%s, playerProtocol=%d containerType=%s " BIP_MSG_PRE_ARG, hPlayer,
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->connectSettings.dataAvailabilityModel),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel),
                        hPlayer->playerProtocol,
                        BIP_ToStr_BIP_PlayerContainerType(hPlayer->streamInfo.containerType)
                        ));
        bipStatus = BIP_SUCCESS;
    }
    if (hPlayer->connectSettings.dataAvailabilityModel != BIP_PlayerDataAvailabilityModel_eAuto && hPlayer->dataAvailabilityModel != hPlayer->connectSettings.dataAvailabilityModel)
    {
        BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: app provided dataModel=%s doesn't match the player determined dataModel=%s" BIP_MSG_PRE_ARG, hPlayer,
                    BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->connectSettings.dataAvailabilityModel ),
                    BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel )
                    ));
        bipStatus = BIP_INF_NOT_AVAILABLE;
    }

    return (bipStatus);
} /* determineDataAvailabilityModel */

static BIP_Status processProbingState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;

    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BSTD_UNUSED( threadOrigin );

    if ( hPlayer->subState == BIP_PlayerSubState_eProbingNew )
    {
        /* We are here because we have a new _ProbeMediaInfo() API called from App. Initiate this processing. */

        /* Note: BIP_Player_ProbeMediaInfo() has validated the required API arguments. */
        hPlayer->probeSettings = *hPlayer->probeMediaInfoApi.pSettings;
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Accepted _ProbeMediaInfo Arb: Start processing" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

        /* Free-up previously created MediaInfo object as Probe may be called multiple times. */
        if (hPlayer->hMediaInfo)
        {
            BIP_MediaInfo_Destroy(hPlayer->hMediaInfo);
            hPlayer->hMediaInfo = NULL;
        }

        /* Start API timer if enabled by the caller. */
        completionStatus = startApiTimer(hPlayer, hPlayer->probeSettings.timeoutInMs);
        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "startApiTimer Failed"), error, completionStatus, completionStatus );

        hPlayer->mediaProbeComplete = false;
        /* Use PBIP SessionSetup API to Probe the URL Media. */
        {
            B_PlaybackIp_GetDefaultSessionSetupSettings(&hPlayer->pbipState.ipSessionSetupSettings);
            BKNI_Memset(&hPlayer->pbipState.ipSessionSetupStatus, 0, sizeof(hPlayer->pbipState.ipSessionSetupStatus));

            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp)
            {
                hPlayer->pbipState.ipSessionSetupSettings.u.http.enablePayloadScanning = hPlayer->probeMediaInfoApi.pSettings->enablePayloadScanning;
                if (
                        hPlayer->probeMediaInfoApi.pSettings->transportType == NEXUS_TransportType_eEs ||
                        !hPlayer->dlnaFlags.playSpeedSupported ||
                        !hPlayer->dlnaFlags.timeBasedSeekSupported )
                {
                    /* We force turn-on payload inspection for ES formats or when server side PlaySpeed or TimeSeek is not supported as we will have to do client side trickmodes. */
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.enablePayloadScanning = true;
                }
                hPlayer->pbipState.ipSessionSetupSettings.u.http.dontUseIndex = hPlayer->probeMediaInfoApi.pSettings->dontUseIndex;
                hPlayer->pbipState.ipSessionSetupSettings.u.http.contentLengthHint = hPlayer->probeMediaInfoApi.pSettings->contentLength;

                if (
                        (strstr(hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders, "Content-Length") == NULL && hPlayer->probeMediaInfoApi.pSettings->contentLength == 0)
                    ||
                        (hPlayer->dlnaFlags.senderPacingSupported) )
                {
                    /* Server didn't sent content length header & app hasn't provided it to us either or sender is pacing the content, we limit the probe time. */
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.psiParsingTimeLimit = 500; /* TODO: should this be tied to the Probe() timeout ? */
                    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "ContentLength is not known or sending pacing set, limiting the probe time to %dmsec"
                               BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->pbipState.ipSessionSetupSettings.u.http.psiParsingTimeLimit ));
                }
                if (hPlayer->dlnaFlags.senderPacingSupported)
                {
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.liveChannel = true;
                    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "SenderPacing bit is set, so it is a liveChannel!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
            }

            if (hPlayer->dtcpIpEnabled)
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Setting DTCP/IP Security Settings: akeHandle=%p !" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->pDtcpIpLibAkeCtx ));
                hPlayer->pbipState.ipSessionSetupSettings.security.enableDecryption = true;
                hPlayer->pbipState.ipSessionSetupSettings.security.initialSecurityContext = hPlayer->pDtcpIpLibAkeCtx;
                hPlayer->pbipState.ipSessionSetupSettings.security.securityProtocol = B_PlaybackIpSecurityProtocol_DtcpIp;
            }

            hPlayer->pbipState.status = B_PlaybackIp_SessionSetup(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipSessionSetupSettings, &hPlayer->pbipState.ipSessionSetupStatus);
            BIP_CHECK_GOTO(( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS || hPlayer->pbipState.status == B_ERROR_SUCCESS), ( "B_PlaybackIp_SessionSetup Failed"), error, BIP_ERR_PLAYER_PROBE, completionStatus );

            /* Change subState to indicate that we are now waiting for Server Response (which we will check below). */
            hPlayer->subState = BIP_PlayerSubState_eProbingWaitForMediaInfo;
        } /* PBIP SessionSetup call */
    } /* subState == eProbingNew */

    if ( hPlayer->subState == BIP_PlayerSubState_eProbingWaitForMediaInfo )
    {
        /* Check status of previous PBIP Call. */
        if ( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS )
        {
            /* Poll again to get the latest PBIP API status. */
            hPlayer->pbipState.status = B_PlaybackIp_SessionSetup(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipSessionSetupSettings, &hPlayer->pbipState.ipSessionSetupStatus);
        }

        if ( hPlayer->pbipState.status == B_ERROR_IN_PROGRESS )
        {
            /* PBIP Call is still in-progress, check if API has timed out. */
            completionStatus = BIP_INF_IN_PROGRESS;
            if ( hasApiTimerExpired(hPlayer, hPlayer->probeSettings.timeoutInMs) == true )
            {
                /* API timedout, set status & move to Done state. We will complete the API Arb below. */
                completionStatus = BIP_INF_TIMEOUT;
                hPlayer->subState = BIP_PlayerSubState_eProbingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Timedout while waiting for PBIP SessionSetup() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else if (hPlayer->disconnecting)
            {
                /* App has called _Disconnect() or _Destroy(), so complete this state. */
                completionStatus = BIP_ERR_PLAYER_BEING_DISCONNECTED;
                hPlayer->subState = BIP_PlayerSubState_eConnectingDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Player is being Disconnected while waiting for PBIP SessionSetup() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
        }
        else if ( hPlayer->pbipState.status == B_ERROR_SUCCESS )
        {
            BIP_MediaInfoStream *pMediaInfoStream;
            const bmedia_probe_stream *pBmediaStream = NULL;

            /* PBIP Call has successfuly completed, so lets do the remaining success related processing. */
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionSetup() successful" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
            {
                hPlayer->pbipState.psi = hPlayer->pbipState.ipSessionSetupStatus.u.rtsp.psi;
                pBmediaStream = hPlayer->pbipState.ipSessionSetupStatus.u.rtsp.stream;
            }
            else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp)
            {
                hPlayer->pbipState.psi = hPlayer->pbipState.ipSessionSetupStatus.u.http.psi; /* first field in each union structure is the psi field. */
                pBmediaStream = hPlayer->pbipState.ipSessionSetupStatus.u.http.stream;
            }
            else
            {
                hPlayer->pbipState.psi = hPlayer->pbipState.ipSessionSetupStatus.u.udp.psi; /* first field in each union structure is the psi field. */
                pBmediaStream = hPlayer->pbipState.ipSessionSetupStatus.u.udp.stream;
            }

            if (hPlayer->dlnaFlags.availableSeekRangeSupported && hPlayer->getStatusFromServerState.lastUpdateTimeSet)
            {
                /* We use the duration that Server had returned in the availableSeekRange header only if it was updated! */
                hPlayer->pbipState.psi.duration = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.durationInMs;
            }
            else
            {
                /* Server didn't provide the seekable range via the availableSeekRange header, so we use what ever duration we had found during media probe. */
                hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.durationInMs = hPlayer->pbipState.psi.duration;
            }

            if (pBmediaStream)
            {
                hPlayer->hMediaInfo = BIP_MediaInfo_CreateFromBMediaStream_priv( pBmediaStream, hPlayer->pbipState.psi.duration, hPlayer->pbipState.psi.contentLength, hPlayer->pbipState.psi.liveChannel , NULL);
                BIP_CHECK_GOTO(( hPlayer->hMediaInfo ), ( "BIP_MediaInfo_CreateFromBMediaStream_priv Failed"), error, BIP_ERR_PLAYER_PROBE, completionStatus );
            }
            else
            {
                hPlayer->hMediaInfo = BIP_MediaInfo_CreateFromPbipPsi_priv((const B_PlaybackIpPsiInfo *)&hPlayer->pbipState.ipSessionSetupStatus.u.http.psi, NULL);
                BIP_CHECK_GOTO(( hPlayer->hMediaInfo ), ( "BIP_MediaInfo_CreateFromBMediaStream_priv Failed"), error, BIP_ERR_PLAYER_PROBE, completionStatus );
            }
            if (hPlayer->probeMediaInfoApi.phMediaInfo) *hPlayer->probeMediaInfoApi.phMediaInfo = hPlayer->hMediaInfo;
            pMediaInfoStream = BIP_MediaInfo_GetStream(hPlayer->hMediaInfo);

            hPlayer->hFile = hPlayer->pbipState.ipSessionSetupStatus.u.http.file;
            hPlayer->usePlaypump2ForAudio = hPlayer->pbipState.psi.usePlaypump2ForAudio;
            hPlayer->transportTypeForPlaypump2 = hPlayer->pbipState.psi.mpegType;

            hPlayer->mediaProbeComplete = true;
            completionStatus = BIP_SUCCESS;
            hPlayer->subState = BIP_PlayerSubState_eProbingDone;

            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_ProbeMediaInfo successful: transportType=%s contentLength=%lld, streamDuration=%u liveChannel=%d usePP2=%s"
                       BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_ToStr_NEXUS_TransportType(pMediaInfoStream->transportType),
                       pMediaInfoStream->contentLength, pMediaInfoStream->durationInMs, pMediaInfoStream->liveChannel, hPlayer->usePlaypump2ForAudio?"Y":"N" ));
        }
        else
        {
            /* PBIP Call returned some error, so reflect it in the completionStatus. */
            completionStatus = BIP_ERR_PLAYER_PROBE;
            hPlayer->subState = BIP_PlayerSubState_eProbingDone;
            BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionSetup() Failed with PBIP errorStatus=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->pbipState.status ));
        }
    }

error:
    /* Double check if completionStatus is NOT in progress. This can happen if we get here via a jump to error label. */
    if ( completionStatus != BIP_INF_IN_PROGRESS )
    {
        /* API is done either due to Success or Error status. Move to Done state. completionStatus must be set before this. */
        hPlayer->subState = BIP_PlayerSubState_eProbingDone;
    }

    /* If subState is Done, then update the states based on the completionStatus. */
    if (hPlayer->subState == BIP_PlayerSubState_eProbingDone)
    {
        if ( completionStatus == BIP_SUCCESS )
        {
            /* Now that probe information is available, determine StreamInfo to use, Clock Recovery & Streaming Feeding modes. */
            getProbedStreamInfo(hPlayer, &hPlayer->streamInfo);

            /* Determine the dataAvailabilityModel */
            completionStatus = determineDataAvailabilityModel(hPlayer);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "determineDataAvailabilityModel Failed"), error, completionStatus, completionStatus );

            /* Select Clock Recovery Mode. */
            hPlayer->useLiveIpMode = useLiveModeForIpPlayback(
                    hPlayer->streamInfo.transportType,
                    hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled,
                    hPlayer->streamInfo.liveChannel,
                    hPlayer->dataAvailabilityModel,
                    hPlayer->playerProtocol,
                    &hPlayer->clockRecoveryMode);

            /* Determine if Nexus Playpump or Playback should be used for the Playback. */
            hPlayer->useNexusPlaypump = useNexusPlaypumpForIpPlayback(
                    hPlayer->streamInfo.serverSideTrickmodesSupported,
                    hPlayer->streamInfo.timeSeekRangeHeaderEnabled,
                    hPlayer->streamInfo.liveChannel,
                    hPlayer->streamInfo.contentLength,
                    hPlayer->streamInfo.transportType,
                    hPlayer->streamInfo.containerType,
                    hPlayer->playerProtocol);
            hPlayer->streamInfo.usePlaypump = hPlayer->useNexusPlaypump;
            hPlayer->streamInfo.usePlayback = hPlayer->useNexusPlaypump?false:true;

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p useLiveIpMode=%s clockRecoveryMode=%s usePlayback=%s usePlaypump=%s transportType=%s playerContainerType=%s" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->useLiveIpMode?"Y":"N", BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode),
                        hPlayer->streamInfo.usePlayback?"Y":"N", hPlayer->streamInfo.usePlaypump?"Y":"N",
                        BIP_ToStr_NEXUS_TransportType(hPlayer->streamInfo.transportType), BIP_ToStr_BIP_PlayerContainerType(hPlayer->streamInfo.containerType) ));

            /* We are successfuly done w/ _Probe, so lets move back to the Connected state. */
            if (hPlayer->state == BIP_PlayerState_eProbing)
            {
                /* We only move to eConnected state if our main state was ePreparing (meaning app had directly called _Probing()). */
                hPlayer->state = BIP_PlayerState_eConnected;
            }
            else
            {
                /* Main state is not Probing, so this state function is run via another player state (mainly Preparing state). */
                /* Just update the completion status. Caller will update the main state. */
            }
            hPlayer->subState = BIP_PlayerSubState_eIdle;
            hPlayer->mediaProbeComplete = true;
            stopApiTimer(hPlayer);
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Probe successful: url=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->hUrl->urlRaw ));
        }
        else
        {
            /* Error case, determine the state to go back to! */
            if (hPlayer->state == BIP_PlayerState_eProbing)
            {
                /* We only move to eConnected state if our main state was eProbing (meaning app had directly called _Probe()). */
                hPlayer->state = BIP_PlayerState_eConnected;
                hPlayer->subState = BIP_PlayerSubState_eIdle;
            }
            else
            {
                /* Main state is not Probing, so this state function is run via another player state (mainly Preparing state). */
                /* Just update the completion status. Caller will update the main state. */
                hPlayer->subState = BIP_PlayerSubState_eIdle;
            }

            /* Free-up any acquired resources. */
            if (hPlayer->hMediaInfo)
            {
                BIP_MediaInfo_Destroy(hPlayer->hMediaInfo);
                hPlayer->hMediaInfo = NULL;
            }
            BDBG_ERR(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "ProbeMediaInfo() Failed, status=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
        }

        /* Check if disconnecting flag is set, meaning _Disconnect()/_Destroy() API must have been called which completed this state. */
        if (hPlayer->disconnecting)
        {
            /* Set a flag to re-run the top processPlayerState() so that it can process the actual _Disconnect()/_Destroy() API. */
            hPlayer->reRunState = true;
        }


        if (BIP_Arb_IsBusy(hPlayer->probeMediaInfoApi.hArb))
        {
            BIP_Arb_CompleteRequest( hPlayer->probeMediaInfoApi.hArb, completionStatus );
        }
    }
    return (completionStatus);
} /* processProbingState_locked */

static NEXUS_PidChannelHandle openPidChannel(
    BIP_PlayerHandle                        hPlayer,
    unsigned                                pid,
    const NEXUS_PlaybackPidChannelSettings  *pSettings /* attr{null_allowed=n} */
    )
{
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    NEXUS_PidChannelHandle hPidChannel = NULL;
    NEXUS_Error nrc;

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p trackId=0x%x useNexusPlaypump=%s usePlaypump2ForAudio=%s" BIP_MSG_PRE_ARG, hPlayer, pid,
                hPlayer->useNexusPlaypump ? "Y": "N", hPlayer->usePlaypump2ForAudio ? "Y": "N" ));
    if (hPlayer->useNexusPlaypump)
    {
        NEXUS_PlaypumpHandle hPlaypump;

        if (hPlayer->usePlaypump2ForAudio && pSettings->pidSettings.pidType == NEXUS_PidType_eAudio)
        {
            hPlaypump = hPlayer->hPlaypump2;
        }
        else
        {
            hPlaypump = hPlayer->hPlaypump;
        }
        hPidChannel = NEXUS_Playpump_OpenPidChannel(hPlaypump, pid, &pSettings->pidSettings);
        BIP_CHECK_GOTO(( hPidChannel ), ( "NEXUS_Playpump_OpenPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p NEXUS_Playpump_OpenPidChannel opened hPidChannel=%p for trackId=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPidChannel, pid ));
    }
    else
    {
        if (!hPlayer->hPlayback)
        {
            NEXUS_PlaybackSettings settings;

            hPlayer->hPlayback = NEXUS_Playback_Create();
            BIP_CHECK_GOTO(( hPlayer->hPlayback ), ( "NEXUS_Playback_Create Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );
            NEXUS_Playback_GetSettings(hPlayer->hPlayback, &settings);
            /* Use the App provided playbackSettings (which is initialized via BIP_Player_GetDefaultPrepareSettings */
            settings = hPlayer->playerSettings.playbackSettings;
            settings.playpump = hPlayer->hPlaypump;
            settings.playpumpSettings.transportType = hPlayer->streamInfo.transportType;
            nrc = NEXUS_Playback_SetSettings(hPlayer->hPlayback, &settings);
            BIP_CHECK_GOTO(( nrc == NEXUS_SUCCESS ), ( "NEXUS_Playback_SetSettings Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );
        }

        hPidChannel = NEXUS_Playback_OpenPidChannel(hPlayer->hPlayback, pid, pSettings);
        BIP_CHECK_GOTO(( hPidChannel ), ( "NEXUS_Playback_OpenPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p NEXUS_Playback_OpenPidChannel opened hPidChannel=%p trackId=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPidChannel, pid ));
    }

error:
    return hPidChannel;
} /* openPidChannel */

static BIP_Status prepareForPullMode(
    BIP_PlayerHandle hPlayer
    )
{
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    NEXUS_Error nrc;

    /* Setup StcChannel in auto mode. */
    if (hPlayer->hSimpleStcChannel)
    {
        NEXUS_SimpleStcChannelSettings settings;

        NEXUS_SimpleStcChannel_GetDefaultSettings( &settings );
        settings.mode = NEXUS_StcChannelMode_eAuto;
        if (hPlayer->streamInfo.transportType == NEXUS_TransportType_eMp4Fragment)
        {
            settings.modeSettings.Auto.transportType = NEXUS_TransportType_eMpeg2Pes;
        }
        else
        {
            settings.modeSettings.Auto.transportType = hPlayer->streamInfo.transportType;
        }

        if (hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess && BIP_Arb_IsBusy(hPlayer->prepareApi.hArb) && hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eNexusTransportType)
        {
            /* By default, we use audio master auto mode when starting to play a stream w/ limited random access data model at 1x. */
            settings.modeSettings.Auto.behavior = NEXUS_StcChannelAutoModeBehavior_eAudioMaster;
        }
        else
        {
            settings.modeSettings.Auto.behavior = NEXUS_StcChannelAutoModeBehavior_eVideoMaster;
        }
        nrc = NEXUS_SimpleStcChannel_SetSettings( hPlayer->hSimpleStcChannel, &settings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleStcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p is setup for pull mode of streaming, stc behavior=%s" BIP_MSG_PRE_ARG, hPlayer,
                    hPlayer->hSimpleStcChannel, BIP_ToStr_NEXUS_StcChannelAutoModeBehavior(settings.modeSettings.Auto.behavior) ));
    }
    else /* non-simple stcChannel case. */
    {
        NEXUS_StcChannelSettings settings;

        BDBG_ASSERT(hPlayer->hStcChannel);

        hPlayer->lockedTimebase = NEXUS_Timebase_Open(NEXUS_ANY_ID);
        BIP_CHECK_GOTO(( hPlayer->lockedTimebase != NEXUS_Timebase_eInvalid), ( "NEXUS_Timebase_Open Failed"), error, BIP_ERR_NEXUS, bipStatus );

        NEXUS_StcChannel_GetDefaultSettings( NEXUS_ANY_ID, &settings );
        settings.mode = NEXUS_StcChannelMode_eAuto;
        settings.timebase = hPlayer->lockedTimebase;
        if (hPlayer->streamInfo.transportType == NEXUS_TransportType_eMp4Fragment)
        {
            settings.modeSettings.Auto.transportType = NEXUS_TransportType_eMpeg2Pes;
        }
        else
        {
            settings.modeSettings.Auto.transportType = hPlayer->streamInfo.transportType;
        }
        if (hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess && BIP_Arb_IsBusy(hPlayer->prepareApi.hArb) && hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eNexusTransportType)
        {
            /* Use audio master auto mode when starting to play a stream w/ limited random access data model at 1x */
            settings.modeSettings.Auto.behavior = NEXUS_StcChannelAutoModeBehavior_eAudioMaster;
        }
        else
        {
            settings.modeSettings.Auto.behavior = NEXUS_StcChannelAutoModeBehavior_eVideoMaster;
        }

        nrc = NEXUS_StcChannel_SetSettings( hPlayer->hStcChannel, &settings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_StcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p is setup for pull mode of streaming, stc behavior=%s" BIP_MSG_PRE_ARG,
                    hPlayer , hPlayer->hStcChannel, BIP_ToStr_NEXUS_StcChannelAutoModeBehavior(settings.modeSettings.Auto.behavior) ));
    }

    if (hPlayer->useNexusPlaypump)
    {
        NEXUS_PlaypumpSettings settings;
        NEXUS_Playpump_GetSettings(hPlayer->hPlaypump, &settings);
        settings.transportType = hPlayer->streamInfo.transportType;
        if (hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled) settings.timestamp.type = NEXUS_TransportTimestampType_eMod300;
        nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump, &settings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is setup for pull mode of streaming!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
    }
    else
    {
        NEXUS_PlaybackSettings settings;

        NEXUS_Playback_GetSettings(hPlayer->hPlayback, &settings);
        /* Use the App provided playbackSettings (which is initialized via BIP_Player_GetDefaultPrepareSettings */
        settings = hPlayer->playerSettings.playbackSettings;
        if (hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled) settings.playpumpSettings.timestamp.type = NEXUS_TransportTimestampType_eMod300;
        settings.playpumpSettings.transportType = hPlayer->streamInfo.transportType;
        settings.playpump = hPlayer->hPlaypump;
        settings.stcChannel = hPlayer->hStcChannel;
        settings.simpleStcChannel = hPlayer->hSimpleStcChannel;
        if (settings.endOfStreamAction != NEXUS_PlaybackLoopMode_eLoop)
        {
            settings.endOfStreamCallback.callback = endOfStreamCallbackFromPlayback;
            settings.endOfStreamCallback.context = hPlayer;
        }
        else
        {
            settings.endOfStreamCallback.callback = NULL;
            settings.endOfStreamCallback.context = NULL;
        }
        settings.errorCallback.callback = errorCallbackFromPlayback;
        settings.errorCallback.context = hPlayer;
        settings.enableStreamProcessing = hPlayer->playerSettings.lpcm.convertLpcmToWave? true: false;
        nrc = NEXUS_Playback_SetSettings(hPlayer->hPlayback, &settings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playback_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playback=%p is setup for pull mode of streaming: loop on EOS=%s startPaused=%s" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->hPlayback, settings.endOfStreamAction == NEXUS_PlaybackLoopMode_eLoop?"Y":"N",
                    settings.startPaused?"Y":"N"));
    }

    /* Let PBIP know about Playback vs Playpump & Clock Recovery Modes. */
    {
        B_Error rc;

        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.useNexusPlaypump = hPlayer->useNexusPlaypump;
        hPlayer->pbipState.settings.ipMode = B_PlaybackIpClockRecoveryMode_ePull;
        if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_ePlay || hPlayer->playerSettings.playbackSettings.beginningOfStreamAction == NEXUS_PlaybackLoopMode_ePlay )
        {
            hPlayer->pbipState.settings.waitOnEndOfStream = true;
        }
        else if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_eLoop)
        {
            hPlayer->pbipState.settings.enableEndOfStreamLooping = true;
        }
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        BIP_CHECK_GOTO(( rc == B_ERROR_SUCCESS), ( "B_PlaybackIp_SetSettings Failed"), error, BIP_ERR_INTERNAL, bipStatus );

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: PBIP=%p is setup w/ PullMode using Nexus %s: loop on EOS=%s!" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->pbipState.hPlaybackIp, hPlayer->useNexusPlaypump?"Playpump":"Playback",
                    hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_eLoop?"Y":"N" ));
    }
    bipStatus = BIP_SUCCESS;
error:
    return (bipStatus);
}

static BIP_Status prepareForPushWithPcrSyncSlipMode(
    BIP_PlayerHandle hPlayer
    )
{
    NEXUS_Error nrc;
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    NEXUS_PidChannelHandle pcrPidChannel = NULL;

    /* In this Live SyncSlip mode, decoders lock to the incoming PCRs. */
    {
        hPlayer->hPcrPidChannel = NULL;
        /* Either create a new pidChannel or use audio/video pidChannel for the PCR pid. */
        if ( hPlayer->streamInfo.mpeg2Ts.pcrPid != hPlayer->audioTrackId && hPlayer->streamInfo.mpeg2Ts.pcrPid != hPlayer->videoTrackId)
        {
            NEXUS_PlaypumpOpenPidChannelSettings settings;

            /* Since PCR Pid is different than both Audio & Video, we will create a separate PidChannel for it. */
            BIP_CHECK_GOTO(( hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX ),
                    ( "PCR Pid is neither found nor specified by the App, Can't configure live clock recovery w/o it: pcrPid=%d vPid=%u aPid=%u",
                      hPlayer->streamInfo.mpeg2Ts.pcrPid, hPlayer->videoTrackId, hPlayer->audioTrackId),
                    error, BIP_ERR_INVALID_PARAMETER, bipStatus );

            /* Note that we are creating a PidChannel from Playpump because Live Mode will only use Playpump. */
            NEXUS_Playpump_GetDefaultOpenPidChannelSettings(&settings);
            settings.pidType = NEXUS_PidType_eOther;
            pcrPidChannel = hPlayer->hPcrPidChannel = NEXUS_Playpump_OpenPidChannel(hPlayer->hPlaypump, hPlayer->streamInfo.mpeg2Ts.pcrPid, &settings);
            BIP_CHECK_GOTO(( hPlayer->hPcrPidChannel ), ( "NEXUS_Playpump_OpenPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p NEXUS_Playpump_OpenPidChannel opened PCR hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPcrPidChannel ));
        }
        else
        {
            /* PCR Pid matches either Audio or Video, so use the PidChannel of the matching Pid. */
            if (hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX && hPlayer->streamInfo.mpeg2Ts.pcrPid == hPlayer->audioTrackId)
            {
                pcrPidChannel = hPlayer->hPrimaryAudioPidChannel;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p PCR Pid == Audio Pid (%d), using AudioPidChannel for hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPlayer->audioTrackId, pcrPidChannel ));
            }
            else if (hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX && hPlayer->streamInfo.mpeg2Ts.pcrPid == hPlayer->videoTrackId)
            {
                pcrPidChannel = hPlayer->hVideoPidChannel;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p PCR Pid == Video Pid (%d), using VideoPidChannel for hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPlayer->videoTrackId, pcrPidChannel ));
            }
            else
            {
                BIP_CHECK_GOTO(( false ),
                        ( "PCR Pid is neither found nor specified by the App, Can't configure live clock recovery w/o it: pcrPid=%d vPid=%u aPid=%u",
                          hPlayer->streamInfo.mpeg2Ts.pcrPid, hPlayer->videoTrackId, hPlayer->audioTrackId),
                        error, BIP_ERR_INVALID_PARAMETER, bipStatus );
            }
        }
    }

    /* Setup STC & Timebase to account for Jitter in the IP Network. */
    if (hPlayer->hSimpleStcChannel)
    {
        /* App is using NEXUS Simple STC. */
        NEXUS_SimpleStcChannelSettings stcChannelSettings;

        NEXUS_SimpleStcChannel_GetDefaultSettings(&stcChannelSettings);
        stcChannelSettings.mode = NEXUS_StcChannelMode_ePcr;
        stcChannelSettings.modeSettings.pcr.pidChannel = pcrPidChannel;
        stcChannelSettings.modeSettings.pcr.offsetThreshold = hPlayer->connectSettings.maxIpNetworkJitterInMs * 183;
        stcChannelSettings.modeSettings.pcr.maxPcrError = hPlayer->connectSettings.maxIpNetworkJitterInMs * 183/2;    /* in milliseconds: based on 90Khz clock */
        stcChannelSettings.modeSettings.pcr.disableJitterAdjustment  = true;
        stcChannelSettings.modeSettings.pcr.disableTimestampCorrection  = true;
        stcChannelSettings.modeSettings.highJitter.mode = NEXUS_SimpleStcChannelHighJitterMode_eDirect;
        stcChannelSettings.modeSettings.highJitter.threshold = hPlayer->connectSettings.maxIpNetworkJitterInMs;
        nrc = NEXUS_SimpleStcChannel_SetSettings(hPlayer->hSimpleStcChannel, &stcChannelSettings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleStcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p settings: mode=PCR offsetThreshold=%u maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->hSimpleStcChannel,
                    stcChannelSettings.modeSettings.pcr.offsetThreshold,
                    stcChannelSettings.modeSettings.pcr.maxPcrError,
                    stcChannelSettings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                    stcChannelSettings.modeSettings.pcr.disableTimestampCorrection?"Y":"N"));

        /* Configure output rate managers: Both display & audio outputs are decoupled from input locked time base & run from a freeRun timebase. */
    }
    else if (hPlayer->hStcChannel) /* non-simple stcChannel case. */
    {
        /* program the lockedTimebase: increase its track range & max pcr errors */
        {
            NEXUS_TimebaseSettings lockedTimebaseSettings;

            hPlayer->lockedTimebase = NEXUS_Timebase_Open(NEXUS_ANY_ID);
            BIP_CHECK_GOTO(( hPlayer->lockedTimebase != NEXUS_Timebase_eInvalid), ( "NEXUS_Timebase_Open Failed"), error, BIP_ERR_NEXUS, bipStatus );

            NEXUS_Timebase_GetSettings(hPlayer->lockedTimebase, &lockedTimebaseSettings);
            lockedTimebaseSettings.sourceType = NEXUS_TimebaseSourceType_ePcr;
            lockedTimebaseSettings.freeze = false;
            lockedTimebaseSettings.sourceSettings.pcr.pidChannel = pcrPidChannel;
            lockedTimebaseSettings.sourceSettings.pcr.maxPcrError = hPlayer->connectSettings.maxIpNetworkJitterInMs * 183/2;    /* in milliseconds: based on 90Khz clock */
            lockedTimebaseSettings.sourceSettings.pcr.trackRange = NEXUS_TimebaseTrackRange_e244ppm;
            /* disable the jitter correction that XPT can perform */
            lockedTimebaseSettings.sourceSettings.pcr.jitterCorrection = NEXUS_TristateEnable_eDisable;
            nrc = NEXUS_Timebase_SetSettings(hPlayer->lockedTimebase, &lockedTimebaseSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Timebase_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p lockedTimebase settings: timebase=%d maxIpNetworkJitterInMs=%u sourceType=%d freeze=%d maxPcrError=%u trackRange=%u jitterCorrection=%u" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->lockedTimebase, hPlayer->connectSettings.maxIpNetworkJitterInMs,
                        lockedTimebaseSettings.sourceType,
                        lockedTimebaseSettings.freeze,
                        lockedTimebaseSettings.sourceSettings.pcr.maxPcrError,
                        lockedTimebaseSettings.sourceSettings.pcr.trackRange,
                        lockedTimebaseSettings.sourceSettings.pcr.jitterCorrection));
        }

        /* Setup 2nd timebase to freerun, this is used for display & audioOutputs as these modules can't tolerate jitter > few msec. */
        {
            NEXUS_TimebaseSettings freeRunTimebaseSettings;

            hPlayer->freeRunTimebase = NEXUS_Timebase_Open(NEXUS_ANY_ID);
            BIP_CHECK_GOTO(( hPlayer->freeRunTimebase != NEXUS_Timebase_eInvalid), ( "NEXUS_Timebase_Open Failed"), error, BIP_ERR_NEXUS, bipStatus );

            NEXUS_Timebase_GetSettings(hPlayer->freeRunTimebase, &freeRunTimebaseSettings);
            freeRunTimebaseSettings.freeze = true;
            freeRunTimebaseSettings.sourceSettings.pcr.trackRange = NEXUS_TimebaseTrackRange_e61ppm;
            freeRunTimebaseSettings.sourceType = NEXUS_TimebaseSourceType_eFreeRun;
            nrc = NEXUS_Timebase_SetSettings(hPlayer->freeRunTimebase, &freeRunTimebaseSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Timebase_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p freeRunTimebase settings: timebase=%d sourceType=%d freeze=%d trackRange=%u" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->freeRunTimebase,
                        freeRunTimebaseSettings.sourceType,
                        freeRunTimebaseSettings.freeze,
                        freeRunTimebaseSettings.sourceSettings.pcr.trackRange));
        }

        /* Update STC Channel Settings to accomodate Network Jitter */
        {
            NEXUS_StcChannelSettings stcChannelSettings;

            NEXUS_StcChannel_GetDefaultSettings( NEXUS_ANY_ID, &stcChannelSettings );
            /* configure the StcChannel to do lipsync with the PCR */
            stcChannelSettings.timebase = hPlayer->lockedTimebase;     /* timebase */
            stcChannelSettings.mode = NEXUS_StcChannelMode_ePcr; /* Live Mode */
            /* offset threshold: uses upper 32 bits (183ticks/msec) of PCR clock */
            stcChannelSettings.modeSettings.pcr.offsetThreshold = hPlayer->connectSettings.maxIpNetworkJitterInMs * 183;
            /* max pcr error: uses upper 32 bits (183ticks/msec) of PCR clock */
            stcChannelSettings.modeSettings.pcr.maxPcrError =  hPlayer->connectSettings.maxIpNetworkJitterInMs * 183;
            stcChannelSettings.modeSettings.pcr.pidChannel = pcrPidChannel;
            /*  PCR Offset "Jitter Adjustment" is not suitable for use with IP channels Channels, so disable it */
            stcChannelSettings.modeSettings.pcr.disableJitterAdjustment = true;
            /* Disable Auto Timestamp correction for PCR Jitter */
            stcChannelSettings.modeSettings.pcr.disableTimestampCorrection = true;
            /* We just configured the Timebase, so turn off auto timebase config */
            stcChannelSettings.autoConfigTimebase = false;
            nrc = NEXUS_StcChannel_SetSettings( hPlayer->hStcChannel, &stcChannelSettings );
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_StcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p settings: timebase=%d mode=PCR offsetThreshold=%u maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s autoConfigTimebase=%s" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->hStcChannel,
                        stcChannelSettings.modeSettings.pcr.offsetThreshold,
                        stcChannelSettings.modeSettings.pcr.maxPcrError,
                        stcChannelSettings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                        stcChannelSettings.modeSettings.pcr.disableTimestampCorrection?"Y":"N",
                        stcChannelSettings.autoConfigTimebase?"Y":"N"));
        }

        /* Configure output rate managers: Both display & audio outputs are decoupled from input locked time base & run from a freeRun timebase. */
        {
            NEXUS_AudioOutput output;
            NEXUS_AudioOutputSettings audioOutputSettings;
            NEXUS_PlatformConfiguration platformConfig;

            NEXUS_Platform_GetConfiguration(&platformConfig);

#if NEXUS_NUM_AUDIO_DACS
            output = NEXUS_AudioDac_GetConnector(platformConfig.outputs.audioDacs[0]);
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->freeRunTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
#if NEXUS_NUM_SPDIF_OUTPUTS
            output = NEXUS_SpdifOutput_GetConnector(platformConfig.outputs.spdif[0]);
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->freeRunTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
#if NEXUS_NUM_HDMI_OUTPUTS
            output = NEXUS_HdmiOutput_GetAudioConnector(platformConfig.outputs.hdmi[0] );
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->freeRunTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
            /* TODO: Modify timebase of other audio outputs as well (e.g. i2s, etc) */

            /* Decouple the Display from input timebase and run it on free running timebase */
            {
                NEXUS_DisplaySettings displaySettings;

                NEXUS_Display_GetSettings(hPlayer->hDisplay, &displaySettings);
                displaySettings.timebase = hPlayer->freeRunTimebase;
                nrc = NEXUS_Display_SetSettings(hPlayer->hDisplay, &displaySettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Display_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
            }
        }

        if (hPlayer->hVideoPidChannel && hPlayer->hVideoDecoder)
        {
            NEXUS_VideoDecoderSettings  videoDecoderSettings;

            NEXUS_VideoDecoder_GetSettings(hPlayer->hVideoDecoder, &videoDecoderSettings);
            videoDecoderSettings.ptsOffset = hPlayer->connectSettings.maxIpNetworkJitterInMs * 45;    /* In 45Khz clock */
            nrc = NEXUS_VideoDecoder_SetSettings(hPlayer->hVideoDecoder, &videoDecoderSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_VideoDecoder_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: VideoDecoder ptsOffset=%d updated by jitter of %u msec" BIP_MSG_PRE_ARG, hPlayer, videoDecoderSettings.ptsOffset, hPlayer->connectSettings.maxIpNetworkJitterInMs ));
        }

        if (hPlayer->hPrimaryAudioPidChannel && hPlayer->hPrimaryAudioDecoder)
        {
            NEXUS_AudioDecoderSettings  audioDecoderSettings;

            NEXUS_AudioDecoder_GetSettings(hPlayer->hPrimaryAudioDecoder, &audioDecoderSettings);
            audioDecoderSettings.ptsOffset = hPlayer->connectSettings.maxIpNetworkJitterInMs * 45;    /* In 45Khz clock */
            nrc = NEXUS_AudioDecoder_SetSettings(hPlayer->hPrimaryAudioDecoder, &audioDecoderSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioDecoder_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: AudioDecoder ptsOffset=%d updated by jitter of %u msec" BIP_MSG_PRE_ARG, hPlayer, audioDecoderSettings.ptsOffset, hPlayer->connectSettings.maxIpNetworkJitterInMs ));
        }
    } /* non-simple stcChannel else case. */

    BDBG_ASSERT(hPlayer->useNexusPlaypump);
    {
        NEXUS_PlaypumpSettings settings;
        NEXUS_Playpump_GetSettings(hPlayer->hPlaypump, &settings);
        settings.transportType = hPlayer->streamInfo.transportType;
        if (hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled) settings.timestamp.type = NEXUS_TransportTimestampType_eMod300;
        nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump, &settings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is setup for live mode of streaming!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
    }

    /* Let PBIP know about using Playpump & Clock Recovery Modes. */
    {
        B_Error rc;

        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.useNexusPlaypump = true;
        hPlayer->pbipState.settings.ipMode = B_PlaybackIpClockRecoveryMode_ePushWithPcrSyncSlip;
        if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_ePlay || hPlayer->playerSettings.playbackSettings.beginningOfStreamAction == NEXUS_PlaybackLoopMode_ePlay )
        {
            hPlayer->pbipState.settings.waitOnEndOfStream = true;
        }
        else if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_eLoop)
        {
            hPlayer->pbipState.settings.enableEndOfStreamLooping = true;
        }
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        BIP_CHECK_GOTO(( rc == B_ERROR_SUCCESS), ( "B_PlaybackIp_SetSettings Failed"), error, BIP_ERR_INTERNAL, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: PBIP=%p is setup w/ PushWithPcrSyncSlip Clock-recovery mode using Nexus Playpump!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->pbipState.hPlaybackIp ));
    }

    bipStatus = BIP_SUCCESS;
error:
    if (bipStatus != BIP_SUCCESS)
    {
        if (hPlayer->hPcrPidChannel) NEXUS_Playpump_ClosePidChannel(hPlayer->hPlaypump, hPlayer->hPcrPidChannel);
        hPlayer->hPcrPidChannel = NULL;
    }
    return (bipStatus);
}

static BIP_Status prepareForPushWithTtsOrPcrNoSyncSlipMode(
    BIP_PlayerHandle hPlayer
    )
{
    B_Error rc;
    NEXUS_Error nrc;
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    NEXUS_PidChannelHandle pcrPidChannel;

    /* In this mode, playpump buffer is used as the dejitter buffer. XPT Playback paces the AV data in using the 4-byte timestamps. */

    /* Select a PidChannel. */
    {
        hPlayer->hPcrPidChannel = NULL;
        /* Either create a new pidChannel or use audio/video pidChannel for the PCR pid. */
        if ( hPlayer->streamInfo.mpeg2Ts.pcrPid != hPlayer->audioTrackId && hPlayer->streamInfo.mpeg2Ts.pcrPid != hPlayer->videoTrackId)
        {
            NEXUS_PlaypumpOpenPidChannelSettings settings;

            /* Since PCR Pid is different than both Audio & Video, we will create a separate PidChannel for it. */
            BIP_CHECK_GOTO(( hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX ),
                    ( "PCR Pid is neither found nor specified by the App, Can't configure live clock recovery w/o it: pcrPid=%d vPid=%u aPid=%u",
                      hPlayer->streamInfo.mpeg2Ts.pcrPid, hPlayer->videoTrackId, hPlayer->audioTrackId),
                    error, BIP_ERR_INVALID_PARAMETER, bipStatus );

            /* Note that we are creating a PidChannel from Playpump because Live Mode will only use Playpump. */
            NEXUS_Playpump_GetDefaultOpenPidChannelSettings(&settings);
            settings.pidType = NEXUS_PidType_eOther;
            pcrPidChannel = hPlayer->hPcrPidChannel = NEXUS_Playpump_OpenPidChannel(hPlayer->hPlaypump, hPlayer->streamInfo.mpeg2Ts.pcrPid, &settings);
            BIP_CHECK_GOTO(( hPlayer->hPcrPidChannel ), ( "NEXUS_Playpump_OpenPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p NEXUS_Playpump_OpenPidChannel opened PCR hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPcrPidChannel ));
        }
        else
        {
            /* PCR Pid matches either Audio or Video, so use the PidChannel of the matching Pid. */
            if (hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX && hPlayer->streamInfo.mpeg2Ts.pcrPid == hPlayer->audioTrackId)
            {
                pcrPidChannel = hPlayer->hPrimaryAudioPidChannel;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p PCR Pid == Audio Pid (%d), using AudioPidChannel for hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, pcrPidChannel ));
            }
            else if (hPlayer->streamInfo.mpeg2Ts.pcrPid != UINT_MAX && hPlayer->streamInfo.mpeg2Ts.pcrPid == hPlayer->videoTrackId)
            {
                pcrPidChannel = hPlayer->hVideoPidChannel;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p PCR Pid == Video Pid (%d), using VideoPidChannel for hPidChannel=%p" BIP_MSG_PRE_ARG, hPlayer, pcrPidChannel ));
            }
            else
            {
                BIP_CHECK_GOTO(( false ),
                        ( "PCR Pid is neither found nor specified by the App, Can't configure live clock recovery w/o it: pcrPid=%d vPid=%u aPid=%u",
                          hPlayer->streamInfo.mpeg2Ts.pcrPid, hPlayer->videoTrackId, hPlayer->audioTrackId),
                        error, BIP_ERR_INVALID_PARAMETER, bipStatus );
            }
        }
    }

    /* Setup STC & Timebase to account for Jitter in the IP Network. */
    if (hPlayer->hSimpleStcChannel)
    {
        /* App is using NEXUS Simple STC. */
        NEXUS_SimpleStcChannelSettings stcChannelSettings;

        NEXUS_SimpleStcChannel_GetDefaultSettings(&stcChannelSettings);
        stcChannelSettings.mode = NEXUS_StcChannelMode_ePcr;
        stcChannelSettings.modeSettings.pcr.pidChannel = pcrPidChannel;
        stcChannelSettings.modeSettings.highJitter.mode = NEXUS_SimpleStcChannelHighJitterMode_eTtsPacing;
        nrc = NEXUS_SimpleStcChannel_SetSettings(hPlayer->hSimpleStcChannel, &stcChannelSettings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleStcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p settings: timebase=%d mode=PCR offsetThreshold=%u maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->hSimpleStcChannel,
                    stcChannelSettings.modeSettings.pcr.offsetThreshold,
                    stcChannelSettings.modeSettings.pcr.maxPcrError,
                    stcChannelSettings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                    stcChannelSettings.modeSettings.pcr.disableTimestampCorrection?"Y":"N"));

    }
    else if (hPlayer->hStcChannel) /* non-simple stcChannel case. */
    {
        /* program the lockedTimebase: increase its track range & max pcr errors */
        {
            NEXUS_TimebaseSettings lockedTimebaseSettings;

            hPlayer->lockedTimebase = NEXUS_Timebase_Open(NEXUS_ANY_ID);
            BIP_CHECK_GOTO(( hPlayer->lockedTimebase != NEXUS_Timebase_eInvalid), ( "NEXUS_Timebase_Open Failed"), error, BIP_ERR_NEXUS, bipStatus );

            NEXUS_Timebase_GetSettings(hPlayer->lockedTimebase, &lockedTimebaseSettings);
            lockedTimebaseSettings.sourceType = NEXUS_TimebaseSourceType_ePcr;
            lockedTimebaseSettings.freeze = false;
            lockedTimebaseSettings.sourceSettings.pcr.pidChannel = pcrPidChannel;
            lockedTimebaseSettings.sourceSettings.pcr.maxPcrError = 255;
            lockedTimebaseSettings.sourceSettings.pcr.trackRange = NEXUS_TimebaseTrackRange_e61ppm;
            nrc = NEXUS_Timebase_SetSettings(hPlayer->lockedTimebase, &lockedTimebaseSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Timebase_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p lockedTimebase settings: timebase=%d maxIpNetworkJitterInMs=%u sourceType=%d freeze=%d maxPcrError=%u trackRange=%u jitterCorrection=%u" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->lockedTimebase, hPlayer->connectSettings.maxIpNetworkJitterInMs,
                        lockedTimebaseSettings.sourceType,
                        lockedTimebaseSettings.freeze,
                        lockedTimebaseSettings.sourceSettings.pcr.maxPcrError,
                        lockedTimebaseSettings.sourceSettings.pcr.trackRange,
                        lockedTimebaseSettings.sourceSettings.pcr.jitterCorrection));
        }

        /* Update STC Channel Settings to accomodate Network Jitter */
        {
            NEXUS_StcChannelSettings stcChannelSettings;

            NEXUS_StcChannel_GetDefaultSettings( NEXUS_ANY_ID, &stcChannelSettings );
            /* configure the StcChannel to do lipsync with the PCR */
            stcChannelSettings.timebase = hPlayer->lockedTimebase;     /* timebase */
            stcChannelSettings.mode = NEXUS_StcChannelMode_ePcr; /* Live Mode */
            /* Need to document these hardcoded values. */
            stcChannelSettings.modeSettings.pcr.offsetThreshold = 8;
            stcChannelSettings.modeSettings.pcr.maxPcrError = 255;
            stcChannelSettings.modeSettings.pcr.pidChannel = pcrPidChannel;
            /*  PCR Offset "Jitter Adjustment" is not suitable for use with IP channels Channels, so disable it */
            stcChannelSettings.modeSettings.pcr.disableJitterAdjustment = true;
            /* Disable Auto Timestamp correction for PCR Jitter */
            stcChannelSettings.modeSettings.pcr.disableTimestampCorrection = true;
            /* We just configured the Timebase, so turn off auto timebase config */
            stcChannelSettings.autoConfigTimebase = false;
            nrc = NEXUS_StcChannel_SetSettings( hPlayer->hStcChannel, &stcChannelSettings );
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_StcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus stcChannel=%p settings: timebase=%p mode=PCR offsetThreshold=%u maxPcrError=%u disableJitterAdjustment=%s disableTimestampCorrection=%s autoConfigTimebase=%s" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->hStcChannel,
                        hPlayer->lockedTimebase,
                        stcChannelSettings.modeSettings.pcr.offsetThreshold,
                        stcChannelSettings.modeSettings.pcr.maxPcrError,
                        stcChannelSettings.modeSettings.pcr.disableJitterAdjustment?"Y":"N",
                        stcChannelSettings.modeSettings.pcr.disableTimestampCorrection?"Y":"N",
                        stcChannelSettings.autoConfigTimebase?"Y":"N"));
        }

        /* Configure output rate managers: Both display & audio outputs use the input locked time base. */
        {
            NEXUS_AudioOutput output;
            NEXUS_AudioOutputSettings audioOutputSettings;
            NEXUS_PlatformConfiguration platformConfig;

            NEXUS_Platform_GetConfiguration(&platformConfig);

#if NEXUS_NUM_AUDIO_DACS
            output = NEXUS_AudioDac_GetConnector(platformConfig.outputs.audioDacs[0]);
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->lockedTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
#if NEXUS_NUM_SPDIF_OUTPUTS
            output = NEXUS_SpdifOutput_GetConnector(platformConfig.outputs.spdif[0]);
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->lockedTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
#if NEXUS_NUM_HDMI_OUTPUTS
            output = NEXUS_HdmiOutput_GetAudioConnector(platformConfig.outputs.hdmi[0] );
            NEXUS_AudioOutput_GetSettings(output, &audioOutputSettings);
            audioOutputSettings.timebase = hPlayer->lockedTimebase;
            nrc = NEXUS_AudioOutput_SetSettings(output, &audioOutputSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioOutput_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
#endif
            /* Configure the Display so that it uses the lockTimebase. */
            {
                NEXUS_DisplaySettings displaySettings;

                NEXUS_Display_GetSettings(hPlayer->hDisplay, &displaySettings);
                displaySettings.timebase = hPlayer->lockedTimebase;
                nrc = NEXUS_Display_SetSettings(hPlayer->hDisplay, &displaySettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Display_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );
            }
        }
    }

    BDBG_ASSERT(hPlayer->useNexusPlaypump);
    {
        NEXUS_PlaypumpSettings settings;
        NEXUS_PlaypumpSettings settings1;

        /* Setup a freerun timebase for monitoring the dejitter buffer and controlling the Pacing. */
        {
            NEXUS_TimebaseSettings freeRunTimebaseSettings;

            hPlayer->freeRunTimebase = NEXUS_Timebase_Open(NEXUS_ANY_ID);
            BIP_CHECK_GOTO(( hPlayer->freeRunTimebase != NEXUS_Timebase_eInvalid), ( "NEXUS_Timebase_Open Failed"), error, BIP_ERR_NEXUS, bipStatus );

            NEXUS_Timebase_GetSettings(hPlayer->freeRunTimebase, &freeRunTimebaseSettings);
            freeRunTimebaseSettings.freeze = true;
            freeRunTimebaseSettings.sourceSettings.pcr.trackRange = NEXUS_TimebaseTrackRange_e122ppm;
            freeRunTimebaseSettings.sourceType = NEXUS_TimebaseSourceType_eFreeRun;
            nrc = NEXUS_Timebase_SetSettings(hPlayer->freeRunTimebase, &freeRunTimebaseSettings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Timebase_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p freeRunTimebase settings: timebase=%d sourceType=%d freeze=%d trackRange=%u" BIP_MSG_PRE_ARG,
                        hPlayer, hPlayer->freeRunTimebase,
                        freeRunTimebaseSettings.sourceType,
                        freeRunTimebaseSettings.freeze,
                        freeRunTimebaseSettings.sourceSettings.pcr.trackRange));
        }

        NEXUS_Playpump_GetSettings(hPlayer->hPlaypump, &settings);
        settings.transportType = hPlayer->streamInfo.transportType;
        if (hPlayer->clockRecoveryMode == BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip)
        {
            /* PCR Pacing mode doesn't use the 4 byte timestamps even if it is present in the stream. */
            settings.timestamp.type = NEXUS_TransportTimestampType_eNone;
        }
        else
        {
            settings.timestamp.type = NEXUS_TransportTimestampType_e32_Binary;
        }
        settings.timestamp.pacing = true;
        settings.timestamp.pacingMaxError = hPlayer->playerSettings.ttsParams.pacingMaxError;
        settings.timestamp.pacingOffsetAdjustDisable = true;
        settings.timestamp.resetPacing = true;
        settings.timestamp.timebase = hPlayer->freeRunTimebase;
        if (hPlayer->clockRecoveryMode == BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip)
        {
            settings.timestamp.pcrPacingPid = hPlayer->streamInfo.mpeg2Ts.pcrPid;
        }
        nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump, &settings);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, bipStatus );

        NEXUS_Playpump_GetSettings(hPlayer->hPlaypump, &settings1);
        BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: playpump=%p: timestamp attributes: type=%d pacing=%s pacingMaxError=%u pacingOffsetAdjustDisable=%s resetPacing=%s timebase=%u"
                    BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump,
                    settings1.timestamp.type,
                    settings1.timestamp.pacing?"Y":"N",
                    settings1.timestamp.pacingMaxError,
                    settings1.timestamp.pacingOffsetAdjustDisable?"Y":"N",
                    settings1.timestamp.resetPacing?"Y":"N",
                    settings1.timestamp.timebase
                    ));
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is setup!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
    }

    /* Let PBIP know about the TTS Pacing & its settings. */
    {
        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.useNexusPlaypump = true;
        hPlayer->pbipState.settings.ipMode = B_PlaybackIpClockRecoveryMode_ePushWithTtsNoSyncSlip;
        hPlayer->pbipState.settings.ttsParams.autoDetect = false;
        hPlayer->pbipState.settings.ttsParams.pacingMaxError = hPlayer->playerSettings.ttsParams.pacingMaxError;
        hPlayer->pbipState.settings.ttsParams.throttleParams.initBufDepth = hPlayer->playerSettings.ttsParams.initBufDepth;
        hPlayer->pbipState.settings.ttsParams.throttleParams.maxBufDepth = hPlayer->playerSettings.ttsParams.maxBufDepth;
        hPlayer->pbipState.settings.ttsParams.throttleParams.minBufDepth = hPlayer->playerSettings.ttsParams.minBufDepth;
        hPlayer->pbipState.settings.ttsParams.throttleParams.maxClockMismatch = hPlayer->playerSettings.ttsParams.maxClockMismatch;
        /* TODO: set the callback */
        if (hPlayer->clockRecoveryMode == BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip)
        {
            /* TODO: need to review these settings. */
            /* set to 200 ms per RDB */
            hPlayer->pbipState.settings.ttsParams.pacingMaxError = 27000;

            /* add 200 ms for PCR pacing */
            hPlayer->pbipState.settings.ttsParams.throttleParams.initBufDepth += 202500;
            hPlayer->pbipState.settings.ttsParams.throttleParams.maxBufDepth += 202500;
            hPlayer->pbipState.settings.ttsParams.throttleParams.minBufDepth += 202500;
            hPlayer->pbipState.settings.ttsParams.throttleParams.maxClockMismatch = 2*hPlayer->playerSettings.ttsParams.maxClockMismatch;
        }
        if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_ePlay || hPlayer->playerSettings.playbackSettings.beginningOfStreamAction == NEXUS_PlaybackLoopMode_ePlay )
        {
            hPlayer->pbipState.settings.waitOnEndOfStream = true;
        }
        else if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_eLoop)
        {
            hPlayer->pbipState.settings.enableEndOfStreamLooping = true;
        }
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        BIP_CHECK_GOTO(( rc == B_ERROR_SUCCESS), ( "B_PlaybackIp_SetSettings Failed"), error, BIP_ERR_INTERNAL, bipStatus );

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: PBIP=%p is setup w/ TTS Pacing Mode!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->pbipState.hPlaybackIp ));
    }

    bipStatus = BIP_SUCCESS;
error:
    if (bipStatus != BIP_SUCCESS)
    {
        if (hPlayer->hPcrPidChannel) NEXUS_Playpump_ClosePidChannel(hPlayer->hPlaypump, hPlayer->hPcrPidChannel);
        hPlayer->hPcrPidChannel = NULL;
    }
    return (bipStatus);
} /* prepareForPushWithTtsOrPcrNoSyncSlipMode */

static BIP_PlayerMode determinePlayerMode(
    BIP_PlayerHandle            hPlayer,
    const BIP_PlayerSettings    *pPlayerSettings
    )
{
    BIP_PlayerMode playerMode;

    if ( (pPlayerSettings->videoTrackSettings.pidTypeSettings.video.decoder || pPlayerSettings->videoTrackSettings.pidTypeSettings.video.simpleDecoder) &&
         (pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.primary || pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.simpleDecoder)
       )
    {
        playerMode = BIP_PlayerMode_eAudioVideoDecode;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: mode=AudioVideoDecode as both AV decoder handles are set!" BIP_MSG_PRE_ARG, hPlayer ));
    }
    else if ( pPlayerSettings->videoTrackSettings.pidTypeSettings.video.decoder || pPlayerSettings->videoTrackSettings.pidTypeSettings.video.simpleDecoder )
    {
        playerMode = BIP_PlayerMode_eVideoOnlyDecode;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: mode=VideoOnlyDecode as Video decoder handle is set!" BIP_MSG_PRE_ARG, hPlayer ));
    }
    else if ( pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.primary || pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.simpleDecoder )
    {
        playerMode = BIP_PlayerMode_eAudioOnlyDecode;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: mode=AudioOnlyDecode as Audio decoder handle is set!" BIP_MSG_PRE_ARG, hPlayer ));
    }
    else
    {
        playerMode = BIP_PlayerMode_eRecord;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: mode=Record as AV handles are NOT set!" BIP_MSG_PRE_ARG, hPlayer ));
    }
    return (playerMode);
} /* determinePlayerMode */

static BIP_Status selectAudioTrackAndCodec(
    const BIP_PlayerHandle      hPlayer,
    const BIP_PlayerSettings    *pPlayerSettings,
    BIP_PlayerMode              playerMode,
    unsigned                    *pAudioTrackId,
    NEXUS_AudioCodec            *pAudioCodec,
    bool                        *pUsePlaypump2ForAudio,
    NEXUS_TransportType         *pTransportTypeForPlaypump2,
    bool                        *pAudioTrackIdChanged
    )
{
    BIP_MediaInfoTrack  trackInfo;
    BIP_Status          bipStatus;
    bool                trackFound = false;

    BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p playerMode=%s trackId=0x%x, pPreferredAudioLanguage=%s"
                BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(playerMode), pPlayerSettings->audioTrackId,
                pPlayerSettings->pPreferredAudioLanguage ? pPlayerSettings->pPreferredAudioLanguage: "" ));
    /*
     * Select audio track in the following order:
     *      -Prefer explicit trackId if set, Or
     *      -Determine trackId based on trackGroupId (TODO), Or
     *      -Determine trackId based on audio language name, Or
     *      -Select the 1st audio track if player mode is decode.
     */

    *pUsePlaypump2ForAudio = false;
    *pTransportTypeForPlaypump2 = hPlayer->streamInfo.transportType;
    if (pAudioTrackIdChanged) *pAudioTrackIdChanged = false;

    /* Use explicit trackId if it is provided. */
    if (pPlayerSettings->audioTrackId != UINT_MAX)
    {
        /* If Adaptive streaming container is being used by the Player, then the Audio trackId is a virtual trackId. */
        /* The real one is within the container specific track section. This is done so as multiple audio tracks (different languages) */
        /* may share the same trackId (PID) as they may all be on the different streams. */
        if (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls)
        {
            trackFound = getTrackOfTrackId(hPlayer->hMediaInfo, pPlayerSettings->audioTrackId, &trackInfo);
            BIP_CHECK_GOTO(( trackFound == true ), ( "hPlayer=%p: Failed to lookup trackInfo using explicit trackId=0x%x for HLS Container type" , hPlayer, *pAudioTrackId )
                    , error, BIP_ERR_INVALID_PARAMETER, bipStatus );

            *pAudioTrackId = trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid ? trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid : pPlayerSettings->audioTrackId;
            *pUsePlaypump2ForAudio = trackInfo.info.audio.containerSpecificInfo.hls.requiresSecondPlaypumForAudio;
            *pAudioCodec = trackInfo.info.audio.codec;
            *pTransportTypeForPlaypump2 = trackInfo.info.audio.containerSpecificInfo.hls.hlsExtraAudioSpecificContainerType;
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: HLS Explicit trackId=0x%x, pid=0x%x, codec=%s usePlaypump2ForAudio=%s language=%s transportType=%s"
                        BIP_MSG_PRE_ARG, hPlayer, pPlayerSettings->audioTrackId, *pAudioTrackId, BIP_ToStr_NEXUS_AudioCodec(*pAudioCodec),
                        *pUsePlaypump2ForAudio? "Y":"N", trackInfo.info.audio.pLanguage, BIP_ToStr_NEXUS_TransportType(*pTransportTypeForPlaypump2) ));
        }
        else
        {
            /* Non-adaptive container, note the app provided explicit trackId. */
            *pAudioTrackId = pPlayerSettings->audioTrackId;

            /* Check if app has also provided explicit codec, otherwise find one using the selected trackId. */
            if ( pPlayerSettings->audioTrackSettings.pidSettings.pidTypeSettings.audio.codec != NEXUS_AudioCodec_eUnknown )
            {
                *pAudioCodec = pPlayerSettings->audioTrackSettings.pidSettings.pidTypeSettings.audio.codec;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App provided explicit audioCodec=%s trackId=0x%x" BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_NEXUS_AudioCodec(*pAudioCodec), *pAudioTrackId ));
            }
            else
            {
                /* App didn't provide the audio codec, so select the one for the selected trackId. */
                trackFound = getTrackOfTrackId(hPlayer->hMediaInfo, *pAudioTrackId, &trackInfo);
                BIP_CHECK_GOTO(( trackFound == true ), ( "hPlayer=%p: Failed to lookup trackInfo using explicit trackId=0x%x for HLS Container type" , hPlayer, *pAudioTrackId )
                        , error, BIP_ERR_INVALID_PARAMETER, bipStatus );
                *pAudioCodec = trackInfo.info.audio.codec;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit audioCodec, so select one for the trackId=0x%x audioCodec=%s" BIP_MSG_PRE_ARG, hPlayer, *pAudioTrackId, BIP_ToStr_NEXUS_AudioCodec(*pAudioCodec) ));
            }
        }
    }
    else if (pPlayerSettings->pPreferredAudioLanguage)
    {
        trackFound = getTrackOfLanguage(hPlayer->hMediaInfo, pPlayerSettings->pPreferredAudioLanguage, &trackInfo);
        BIP_CHECK_GOTO(( trackFound == true ), ( "hPlayer=%p: Failed to lookup trackInfo using pPreferredAudioLanguage=%s" , hPlayer, pPlayerSettings->pPreferredAudioLanguage )
                , error, BIP_ERR_INVALID_PARAMETER, bipStatus );
        if (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls)
        {
            *pAudioTrackId = trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid ? trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid : pPlayerSettings->audioTrackId;
            *pTransportTypeForPlaypump2 = trackInfo.info.audio.containerSpecificInfo.hls.hlsExtraAudioSpecificContainerType;
            *pUsePlaypump2ForAudio = trackInfo.info.audio.containerSpecificInfo.hls.requiresSecondPlaypumForAudio;
        }
        else
        {
            *pAudioTrackId = trackInfo.trackId;
        }
        *pAudioCodec = trackInfo.info.audio.codec;
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit audioTrackId but preferredLanguage=%s, matching trackId=0x%x audioCodec=%s transportType=%s usePlaypump2ForAudio=%s"
                    BIP_MSG_PRE_ARG, hPlayer, pPlayerSettings->pPreferredAudioLanguage, *pAudioTrackId, BIP_ToStr_NEXUS_AudioCodec(*pAudioCodec), BIP_ToStr_NEXUS_TransportType(*pTransportTypeForPlaypump2), *pUsePlaypump2ForAudio?"Y":"N" ));
    }
    else
    {
        /* App neither provided audio trackId nor the preferred language. */
        /* If player is in decode mode, select first audio track if it is available in the MediaInfo Stream. */
        if ( (playerMode == BIP_PlayerMode_eAudioOnlyDecode || playerMode == BIP_PlayerMode_eAudioVideoDecode) &&
                    (trackFound = getTrackOfType(hPlayer->hMediaInfo, BIP_MediaInfoTrackType_eAudio, &trackInfo)) == true )
        {
            if (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls)
            {
                *pAudioTrackId = trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid ? trackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid : pPlayerSettings->audioTrackId;
                *pTransportTypeForPlaypump2 = trackInfo.info.audio.containerSpecificInfo.hls.hlsExtraAudioSpecificContainerType;
            }
            else
            {
                *pAudioTrackId = trackInfo.trackId;
            }
            *pAudioCodec = trackInfo.info.audio.codec;
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit trackId or language preference, so auto selected 1st audio trackId=0x%x, codec=%s transportType=%s"
                        BIP_MSG_PRE_ARG, hPlayer, *pAudioTrackId, BIP_ToStr_NEXUS_AudioCodec(*pAudioCodec), BIP_ToStr_NEXUS_TransportType(*pTransportTypeForPlaypump2) ));
        }
        else
        {
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit trackId/language preference & audio decoder handle is not set or didn't find audio track, so not selecting audio trackId" BIP_MSG_PRE_ARG, hPlayer));
            *pAudioTrackId = UINT_MAX;
            *pAudioCodec = NEXUS_AudioCodec_eUnknown;
        }
    }
    if (pAudioTrackIdChanged && *pAudioTrackId != UINT_MAX)
    {
        /* Caller has given us the pointer to a variable to check if current & newly selected tracks are different. */
        /* Check if the selected trackId is different from the current one and set the output flag. */
        if (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls)
        {
            if ( hPlayer->audioTrackInfo.trackId != trackInfo.trackId )
            {
                *pAudioTrackIdChanged = true;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: audioTrackId changed: cur=0x%x, new=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPlayer->audioTrackInfo.trackId, trackInfo.trackId ));
            }
            else
            {
                *pAudioTrackIdChanged = false;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: audioTrackId didn't change: cur=0x%x, new=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPlayer->audioTrackInfo.trackId, trackInfo.trackId ));
            }
        }
        else
        {
            if ( hPlayer->audioTrackId != *pAudioTrackId)
            {
                *pAudioTrackIdChanged = true;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: audioTrackId changed: cur=0x%x, new=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPlayer->audioTrackId, *pAudioTrackId ));
            }
            else
            {
                *pAudioTrackIdChanged = false;
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: audioTrackId didn't change: cur=0x%x, new=0x%x" BIP_MSG_PRE_ARG, hPlayer, hPlayer->audioTrackId, *pAudioTrackId ));
            }
        }
    }

    if (trackFound) hPlayer->audioTrackInfo = trackInfo;
    bipStatus = BIP_SUCCESS;

error:
    return (bipStatus);
} /* selectAudioTrackAndCodec */

static void selectVideoTrackAndCodec(
    const BIP_PlayerHandle      hPlayer,
    const BIP_PlayerSettings    *pPlayerSettings,
    BIP_PlayerMode              playerMode,
    unsigned                    *pVideoTrackId,
    NEXUS_VideoCodec            *pVideoCodec,
    bool                        *pVideoTrackIdChanged
    )
{
    BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p playerMode=%s trackId=0x%x"
                BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(playerMode), pPlayerSettings->videoTrackId ));
    /*
     * Select video track in the following order:
     *      -Prefer explicit trackId if set, Or
     *      -Determine trackId based on trackGroupId (TODO), Or
     *      -Determine trackId based on video view name, Or
     *      -Select the 1st video track if player mode is decode.
     */
    if ( pPlayerSettings->videoTrackId != UINT_MAX )
    {
        *pVideoTrackId = pPlayerSettings->videoTrackId;
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App provided explicit trackId=0x%x" BIP_MSG_PRE_ARG, hPlayer, *pVideoTrackId));
    }
    else if (pPlayerSettings->pPreferredVideoName)
    {
        BDBG_ERR((BIP_MSG_PRE_FMT "hPlayer=%p: TODO: Need to add logic to select videoTrackId using pPreferredVideoName=%s" BIP_MSG_PRE_ARG, hPlayer, pPlayerSettings->pPreferredVideoName));
        BDBG_ASSERT(NULL);
    }
    else
    {
        BIP_MediaInfoTrack trackInfo;
        /* App didn't provide video trackId & player is in decode mode, select first video track if it is available in the MediaInfo Stream. */
        if ( (playerMode == BIP_PlayerMode_eVideoOnlyDecode || playerMode == BIP_PlayerMode_eAudioVideoDecode) &&
                    getTrackOfType(hPlayer->hMediaInfo, BIP_MediaInfoTrackType_eVideo, &trackInfo) == true )
        {
            *pVideoTrackId = trackInfo.trackId;
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit trackId, so auto selected 1st video trackId=0x%x" BIP_MSG_PRE_ARG, hPlayer, *pVideoTrackId));
        }
        else
        {
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit trackId & video decoder handle is not set or didn't find videoTrack, so not selecting video trackId" BIP_MSG_PRE_ARG, hPlayer));
            *pVideoTrackId = UINT_MAX;
        }
    }

    /* Select video codec only if we have selected a valid video trackId above. */
    if (*pVideoTrackId == UINT_MAX)
    {
        *pVideoCodec = NEXUS_VideoCodec_eNone;
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: We didn't choose a videoTrackId above, so not auto-selecting video codec" BIP_MSG_PRE_ARG, hPlayer));
    }
    else if ( pPlayerSettings->videoTrackSettings.pidTypeSettings.video.codec != NEXUS_VideoCodec_eUnknown )
    {
        *pVideoCodec = pPlayerSettings->videoTrackSettings.pidTypeSettings.video.codec;
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App provided explicit videoCodec=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_NEXUS_VideoCodec(*pVideoCodec) ));
    }
    else
    {
        BIP_MediaInfoTrack trackInfo;

        /* App didn't provide video track codec, select first video track if it is available in the MediaInfo Stream. */
        if ( (playerMode == BIP_PlayerMode_eVideoOnlyDecode || playerMode == BIP_PlayerMode_eAudioVideoDecode) &&
                    getTrackOfType(hPlayer->hMediaInfo, BIP_MediaInfoTrackType_eVideo, &trackInfo) == true )
        {
            *pVideoCodec = trackInfo.info.video.codec;
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit videoCodec, so auto-selecting 1st videoCodec=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_NEXUS_VideoCodec(*pVideoCodec) ));
        }
        else
        {
            *pVideoCodec = NEXUS_VideoCodec_eNone;
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: App didn't provide explicit video codec & video decoder handle is not set, so not auto-selecting video codec" BIP_MSG_PRE_ARG, hPlayer));
        }
    }
    if (pVideoTrackIdChanged && hPlayer->videoTrackId != *pVideoTrackId )
    {
        *pVideoTrackIdChanged = true;
    }
} /* selectVideoTrackAndCodec */

static BIP_Status setupClockRecoveryForPlayer(
    BIP_PlayerHandle hPlayer
    )
{
    BIP_Status completionStatus;
    switch (hPlayer->clockRecoveryMode)
    {
        case BIP_PlayerClockRecoveryMode_ePull:
            completionStatus = prepareForPullMode(hPlayer);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "prepareForPullMode Failed"), error, BIP_ERR_NEXUS, completionStatus );
            break;
        case BIP_PlayerClockRecoveryMode_ePushWithPcrSyncSlip:
            completionStatus = prepareForPushWithPcrSyncSlipMode(hPlayer);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "prepareForPushWithPcrSyncSlipMode Failed"), error, BIP_ERR_NEXUS, completionStatus );
            break;
        case BIP_PlayerClockRecoveryMode_ePushWithTtsNoSyncSlip:
        case BIP_PlayerClockRecoveryMode_ePushWithPcrNoSyncSlip:
            completionStatus = prepareForPushWithTtsOrPcrNoSyncSlipMode(hPlayer);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "prepareForPushWithTtsOrPcrNoSyncSlipMode Failed"), error, BIP_ERR_NEXUS, completionStatus );
            break;
        default:
            BDBG_ASSERT(NULL);
    }
error:
    return (completionStatus);
} /* setupClockRecoveryForPlayer */

static void stopVideoDecoder(
    BIP_PlayerHandle hPlayer
    )
{
    if (hPlayer->hVideoDecoder)
    {
        NEXUS_VideoDecoder_Stop(hPlayer->hVideoDecoder);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: non-simple VideoDecoder=%p is stopped" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hVideoDecoder ));
    }
    else if (hPlayer->hSimpleVideoDecoder)
    {
        NEXUS_SimpleVideoDecoder_Stop(hPlayer->hSimpleVideoDecoder);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: simple VideoDecoder=%p is stopped" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hSimpleVideoDecoder ));
    }
} /* stopVideoDecoder */

static BIP_Status startVideoDecoder(
    BIP_PlayerHandle        hPlayer,
    NEXUS_PidChannelHandle  hVideoPidChannel,
    NEXUS_VideoCodec        newVideoCodec
    )
{
    BIP_Status completionStatus;
    NEXUS_Error nrc;

    if (hPlayer->hVideoDecoder)
    {
        hPlayer->startSettings.videoStartSettings.pidChannel = hVideoPidChannel;
        hPlayer->startSettings.videoStartSettings.codec = newVideoCodec;
        nrc = NEXUS_VideoDecoder_Start( hPlayer->hVideoDecoder, &hPlayer->startSettings.videoStartSettings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_VideoDecoder_Start() Failed during Video Track Change"), error, BIP_ERR_NEXUS, completionStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: non-simple VideoDecoder=%p is re-started" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hVideoDecoder ));
    }
    else if (hPlayer->hSimpleVideoDecoder)
    {
        hPlayer->startSettings.simpleVideoStartSettings.settings.pidChannel = hVideoPidChannel;
        hPlayer->startSettings.simpleVideoStartSettings.settings.codec = newVideoCodec;
        nrc = NEXUS_SimpleVideoDecoder_Start( hPlayer->hSimpleVideoDecoder, &hPlayer->startSettings.simpleVideoStartSettings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleVideoDecoder_Start() Failed during Video Track Change"), error, BIP_ERR_NEXUS, completionStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: simple VideoDecoder=%p is re-started" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hSimpleVideoDecoder ));
    }
    completionStatus = BIP_SUCCESS;

error:
    return (completionStatus);
} /* startVideoDecoder */

static void stopAudioDecoder(
    BIP_PlayerHandle hPlayer
    )
{
    if (hPlayer->hPrimaryAudioDecoder)
    {
        NEXUS_AudioDecoder_Stop(hPlayer->hPrimaryAudioDecoder);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: non-simple AudioDecoder=%p is stopped" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPrimaryAudioDecoder ));
    }
    else if (hPlayer->hSimplePrimaryAudioDecoder)
    {
        NEXUS_SimpleAudioDecoder_Stop(hPlayer->hSimplePrimaryAudioDecoder);
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: simple AudioDecoder=%p is stopped" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hSimplePrimaryAudioDecoder ));
    }
} /* stopAudioDecoder */

static BIP_Status startAudioDecoder(
    BIP_PlayerHandle        hPlayer,
    NEXUS_PidChannelHandle  hAudioPidChannel,
    NEXUS_AudioCodec        newAudioCodec
    )
{
    BIP_Status completionStatus;
    NEXUS_Error nrc;

    if (hPlayer->hPrimaryAudioDecoder)
    {
        hPlayer->startSettings.audioStartSettings.pidChannel = hAudioPidChannel;
        hPlayer->startSettings.audioStartSettings.codec = newAudioCodec;
        nrc = NEXUS_AudioDecoder_Start( hPlayer->hPrimaryAudioDecoder, &hPlayer->startSettings.audioStartSettings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_AudioDecoder_Start() Failed during Audio Track Change"), error, BIP_ERR_NEXUS, completionStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: non-simple AudioDecoder=%p is re-started" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPrimaryAudioDecoder ));
    }
    else if (hPlayer->hSimplePrimaryAudioDecoder)
    {
        hPlayer->startSettings.simpleAudioStartSettings.primary.pidChannel = hAudioPidChannel;
        hPlayer->startSettings.simpleAudioStartSettings.primary.codec = newAudioCodec;
        nrc = NEXUS_SimpleAudioDecoder_Start( hPlayer->hSimplePrimaryAudioDecoder, &hPlayer->startSettings.simpleAudioStartSettings );
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleAudioDecoder_Start() Failed during Audio Track Change"), error, BIP_ERR_NEXUS, completionStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: simple AudioDecoder=%p is re-started" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hSimplePrimaryAudioDecoder ));
    }
    completionStatus = BIP_SUCCESS;

error:
    return (completionStatus);
} /* startAudioDecoder */

static BIP_Status determineAndReConfigureClockRecovery(
    BIP_PlayerHandle        hPlayer,
    int                     playRate,
    NEXUS_PlaybackPosition  currentPlayPositionInMs,
    NEXUS_PlaybackPosition  currentTimeshiftBufferLastPositionInMs
    )
{
    BIP_Status                      completionStatus = BIP_SUCCESS;
    BIP_PlayerClockRecoveryMode     newClockRecoveryMode;

    /* Clock Recovery Settings can only change during run-time for LimitedRandomAccess Data availability model. */
    /* For all other cases (live w/o Time-shifting or DVR playback), it stays fixed after Start. */
    BDBG_ASSERT( hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess );

    /*
     * Determine the new clock recovery mode for Limited Random Access model (time-shifted content):
     *  -Playing non-live time shifted (dlnaFlags.enderPacingEnabled is not set), user rewinds back to the start of time-shift buffer, and plays.
     *  -Playing Pure Live (dlnaFlags.enderPacingEnabled is set), user rewinds back to the start of time-shift buffer, and plays.
     */
    {
        hPlayer->useLiveIpMode = useLiveModeForIpPlayback(
                hPlayer->streamInfo.transportType,
                hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled,
                hPlayer->dlnaFlags.senderPacingSupported,
                hPlayer->dataAvailabilityModel,
                hPlayer->playerProtocol,
                &newClockRecoveryMode);
    }

    /* If new clock recovery mode is different than the current one, setup the clock recovery again. */
    if (newClockRecoveryMode != hPlayer->clockRecoveryMode)
    {
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: clock recovery mode changing from=%s to=%s, setup done!"
                  BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode), BIP_ToStr_BIP_PlayerClockRecoveryMode(newClockRecoveryMode) ));
        hPlayer->clockRecoveryMode = newClockRecoveryMode;
        if (newClockRecoveryMode == BIP_PlayerClockRecoveryMode_ePull)
        {
            /* Stop & start AV decoders as we are switching the clock recovery mods. */
            stopAudioDecoder(hPlayer);
            stopVideoDecoder(hPlayer);
        }

        completionStatus = setupClockRecoveryForPlayer(hPlayer);
        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "setupClockRecoveryForPlayer Failed"), error, BIP_ERR_NEXUS, completionStatus );

        if (newClockRecoveryMode == BIP_PlayerClockRecoveryMode_ePull)
        {
            completionStatus = startVideoDecoder( hPlayer, hPlayer->hVideoPidChannel, hPlayer->startSettings.videoStartSettings.codec );
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "startVideoDecoder Failed"), error, completionStatus, completionStatus );

            completionStatus = startAudioDecoder( hPlayer, hPlayer->hPrimaryAudioPidChannel, hPlayer->startSettings.audioStartSettings.codec );
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "startAudioDecoder Failed"), error, completionStatus, completionStatus );

            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: Stopped & Re-started AV Decoders" BIP_MSG_PRE_ARG, hPlayer));
        }
    }
    else
    {
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: clock recovery mode didn't change from=%s"
                  BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode) ));
    }

    /* Now fine tune the STC Channel behavior (Audio vs Video Master Mode) based on the timeshift buffer depth. */
    if ( newClockRecoveryMode == BIP_PlayerClockRecoveryMode_ePull )
    {
        NEXUS_PlaybackPosition              timeShiftBufferDepthFromCurrentPositionInMs;
        NEXUS_StcChannelAutoModeBehavior    stcChannelAutoBehavior;
        NEXUS_Error                         nrc;

#define MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS 3000
        timeShiftBufferDepthFromCurrentPositionInMs = currentTimeshiftBufferLastPositionInMs - currentPlayPositionInMs;
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: clockRecoveryMode=Pull lastPositionInMs=%u currentPlayPositionInMs=%u timeShiftBufferDepthFromCurrentPositionInMs=%u"
                  BIP_MSG_PRE_ARG, hPlayer, currentTimeshiftBufferLastPositionInMs, currentPlayPositionInMs, timeShiftBufferDepthFromCurrentPositionInMs ));
        if ( playRate != NEXUS_NORMAL_PLAY_SPEED )
        {
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: we are either doing a trickplay or are paused, so choose video master mode"
                      BIP_MSG_PRE_ARG, hPlayer));
            stcChannelAutoBehavior = NEXUS_StcChannelAutoModeBehavior_eVideoMaster;
        }
        else if ( timeShiftBufferDepthFromCurrentPositionInMs <= MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS &&
                  hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eNexusTransportType)
        {
            /*
             * We use audio master mode (for basic Nexus container formats) when current position is really close to the live point of the timeshift buffer.
             * Otherwise in video master mode, we may not get the audio sync as it may always be late.
             */
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: current play position depth is <= MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS=%u, using STC Channel in audioMasterAutoMode"
                      BIP_MSG_PRE_ARG, hPlayer, MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS ));
            stcChannelAutoBehavior = NEXUS_StcChannelAutoModeBehavior_eAudioMaster;
        }
        else
        {
            BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: current play position depth is > MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS=%u, using STC Channel in videoMasterAutoMode"
                      BIP_MSG_PRE_ARG, hPlayer, MIN_BUFFERING_DURATION_FOR_VIDEO_MASTER_STC_BEHAVIOR_IN_MS ));
            stcChannelAutoBehavior = NEXUS_StcChannelAutoModeBehavior_eVideoMaster;
        }

        /* Now Configure this mode behavior. */
        if (hPlayer->hSimpleStcChannel)
        {
            NEXUS_SimpleStcChannelSettings settings;

            NEXUS_SimpleStcChannel_GetSettings( hPlayer->hSimpleStcChannel, &settings );
            settings.modeSettings.Auto.behavior = stcChannelAutoBehavior;
            nrc = NEXUS_SimpleStcChannel_SetSettings( hPlayer->hSimpleStcChannel, &settings );
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleStcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );
        }
        else /* non-simple stcChannel case. */
        {
            NEXUS_StcChannelSettings settings;

            NEXUS_StcChannel_GetSettings( hPlayer->hStcChannel, &settings );
            settings.modeSettings.Auto.behavior = stcChannelAutoBehavior;
            nrc = NEXUS_StcChannel_SetSettings( hPlayer->hStcChannel, &settings );
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_StcChannel_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );
        }
        BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: StcChannel is re-configured in mode=%s"
                  BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_NEXUS_StcChannelAutoModeBehavior(stcChannelAutoBehavior) ));
    }
error:
    return (completionStatus);
} /* determineAndReConfigureClockRecovery */

static BIP_Status playerReconfigureAudioTrack(
    BIP_PlayerHandle    hPlayer,
    unsigned            newAudioTrackId,
    NEXUS_AudioCodec    newAudioCodec,
    bool                usePlaypump2ForNewAudio,
    NEXUS_TransportType transportTypeForPlaypump2,
    const NEXUS_PlaybackPidChannelSettings *pNewPidChannelSettings
    )
{
    B_PlaybackIpError   rc;
    BIP_Status          completionStatus = BIP_SUCCESS;
    bool                closePlaypump2 = false;
    NEXUS_Error         nrc;

    /* If we are playing alternate audio using PBIP, tell it to stop doing so. */
    if ( hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls && hPlayer->usePlaypump2ForAudio )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Notify PBIP to stop Playing Current Alternate Audio track" BIP_MSG_PRE_ARG, hPlayer));
        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.stopAlternateAudio = true;
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        BIP_CHECK_GOTO(( rc==B_ERROR_SUCCESS ), ( "B_PlaybackIp_SetSettings Failed"), error, BIP_ERR_PLAYER_PBIP, completionStatus );

        /* Now Stop the Playpump as PBIP has stop feeding the alternate audio to it. */
        BDBG_ASSERT(hPlayer->hPlaypump2);
        NEXUS_Playpump_Stop(hPlayer->hPlaypump2);

        /* Set a flag to indicate that we need to close PP2 if it is no longer needed. */
        closePlaypump2 = true;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p PBIP notified & hPlaypump2=%p Stopped" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
    }

    /* If Player is currently in one of the Audio Decode modes, Stop Audio Decoder & Close current pid channel */
    if ( (hPlayer->mode == BIP_PlayerMode_eAudioVideoDecode || hPlayer->mode == BIP_PlayerMode_eAudioOnlyDecode) && hPlayer->hPrimaryAudioPidChannel)
    {
#if 0
        /* TODO: */
        /* May need to pause the playpump if we have to stop the audio decoder. */
        NEXUS_Playpump_SetPause(hPlayer->hPlaypump, true);
#endif
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Stop Audio Decoder!" BIP_MSG_PRE_ARG, hPlayer ));
        stopAudioDecoder(hPlayer);
        closePidChannel(hPlayer, hPlayer->hPrimaryAudioPidChannel);
        hPlayer->hPrimaryAudioPidChannel = NULL;
    }

    /* Stop & Close Playpump2 if it was being used & is no longer needed for the new audio track. */
    if ( closePlaypump2 && !usePlaypump2ForNewAudio )
    {
        NEXUS_Playpump_Close(hPlayer->hPlaypump2);
        hPlayer->hPlaypump2 = NULL;
        hPlayer->usePlaypump2ForAudio = false;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p hPlaypump2=%p Closed as it is no longer needed for the next audio track" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
    }

    /* Open Playpump2 if it is not already opened & is needed. */
    if (usePlaypump2ForNewAudio)
    {
        if (!hPlayer->hPlaypump2)
        {
            NEXUS_PlaypumpOpenSettings playpumpOpenSettings;

            NEXUS_Playpump_GetDefaultOpenSettings(&playpumpOpenSettings);
            playpumpOpenSettings.fifoSize *= 2;
            playpumpOpenSettings.numDescriptors = 200;
            hPlayer->hPlaypump2 = NEXUS_Playpump_Open(NEXUS_ANY_ID, &playpumpOpenSettings);
            BIP_CHECK_GOTO(( hPlayer->hPlaypump ), ( "NEXUS_Playpump_Open Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump2=%p is opened!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
        }

        /* Now configure the PP2 */
        {
            NEXUS_PlaypumpSettings settings;

            NEXUS_Playpump_GetSettings(hPlayer->hPlaypump2, &settings);
            settings = hPlayer->playerSettings.playbackSettings.playpumpSettings;
            settings.transportType = transportTypeForPlaypump2;
            nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump2, &settings);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump2=%p is setup: transportTypeForPlaypump2=%s" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump, BIP_ToStr_NEXUS_TransportType(transportTypeForPlaypump2) ));
            hPlayer->usePlaypump2ForAudio = true;
            hPlayer->transportTypeForPlaypump2 = transportTypeForPlaypump2;
        }
    }

    /* Open audioPidChannel corresponding to the new audio track. */
    {
        NEXUS_PlaybackPidChannelSettings pidChannelSettings;

        NEXUS_Playback_GetDefaultPidChannelSettings( &pidChannelSettings );
        pidChannelSettings = *pNewPidChannelSettings;
        pidChannelSettings.pidSettings.pidTypeSettings.audio.codec = newAudioCodec;
        pidChannelSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
        hPlayer->hPrimaryAudioPidChannel = openPidChannel( hPlayer, newAudioTrackId, &pidChannelSettings );
        BIP_CHECK_GOTO(( hPlayer->hPrimaryAudioPidChannel ), ( "openPidChannel Failed for the new audio track!"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );
    }

    /* Associate this pid channel to Audio decoder and Start the Audio Decoder again. */
    completionStatus = startAudioDecoder( hPlayer, hPlayer->hPrimaryAudioPidChannel, newAudioCodec );
    BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "startAudioDecoder Failed"), error, completionStatus, completionStatus );

#if 0
    NEXUS_Playpump_SetPause(hPlayer->hPlaypump, false);
#endif

    /* If we are playing alternate audio using PBIP, tell it to start doing so. */
    if ( hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls && hPlayer->usePlaypump2ForAudio )
    {
        /* Start the Playpump first. */
        nrc = NEXUS_Playpump_Start(hPlayer->hPlaypump2);
        BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_Start Failed"), error, BIP_ERR_NEXUS, completionStatus );
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: hPlaypump2=%p is started!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump2 ));

        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.startAlternateAudio = true;
        hPlayer->pbipState.settings.nexusHandles.playpump2 = hPlayer->hPlaypump2;
        hPlayer->pbipState.settings.nexusHandlesValid = true;
        hPlayer->pbipState.settings.alternateAudio.containerType = transportTypeForPlaypump2;
        hPlayer->pbipState.settings.alternateAudio.language = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.pHlsLanguageCode;
        hPlayer->pbipState.settings.alternateAudio.groupId = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.pGroupId;
        hPlayer->pbipState.settings.alternateAudio.pid = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid;
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        BIP_CHECK_GOTO(( rc==B_ERROR_SUCCESS ), ( "B_PlaybackIp_SetSettings Failed"), error, BIP_ERR_PLAYER_PBIP, completionStatus );

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Alternate Rendition Playback is started in HLS Player!" BIP_MSG_PRE_ARG, hPlayer ));
    }
    completionStatus = BIP_SUCCESS;

    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Complete!" BIP_MSG_PRE_ARG, hPlayer ));
error:
    return (completionStatus);
} /* playerReconfigureAudioTrack */

static BIP_Status setSettings(
    BIP_PlayerHandle hPlayer,
    const BIP_PlayerSettings *pNewPlayerSettings
    )
{
    BIP_Status              completionStatus = BIP_SUCCESS;
    BIP_PlayerMode          newPlayerMode;
    unsigned                newVideoTrackId, newAudioTrackId;
    NEXUS_VideoCodec        newVideoCodec;
    NEXUS_AudioCodec        newAudioCodec;
    bool                    playerModeChanged = false;
    bool                    videoTrackIdChanged = false;
    bool                    audioTrackIdChanged = false;
    bool                    usePlaypump2ForAudio = false;
    NEXUS_TransportType     transportTypeForPlaypump2;

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT
                "Settings: videoTrackId=0x%x, videoCodec=%s, audioTrackId=0x%x, audioCodec=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                pNewPlayerSettings->videoTrackId, BIP_ToStr_NEXUS_VideoCodec(pNewPlayerSettings->videoTrackSettings.pidTypeSettings.video.codec),
                pNewPlayerSettings->audioTrackId, BIP_ToStr_NEXUS_AudioCodec(pNewPlayerSettings->audioTrackSettings.pidSettings.pidTypeSettings.audio.codec) ));
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: current playerMode=%s language=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(hPlayer->mode), pNewPlayerSettings->pPreferredAudioLanguage? pNewPlayerSettings->pPreferredAudioLanguage:""));

    /* If Player hasn't been prepared yet, then we will just cache the app provided settings and use them during Prepare. */
    if (hPlayer->state < BIP_PlayerState_ePreparing)
    {
        hPlayer->playerSettings = *pNewPlayerSettings;
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Cached current settings as player is still not in the Prepared state!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        return (BIP_SUCCESS);
    }
    else if (hPlayer->state == BIP_PlayerState_ePreparing)
    {
        BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: Can't change settings while BIP_Player_Prepare() is in progress!" BIP_MSG_PRE_ARG, hPlayer ));
        return (BIP_ERR_INVALID_API_SEQUENCE);
    }

    /* Determine playerMode from the new player settings. */
    newPlayerMode = determinePlayerMode(hPlayer, pNewPlayerSettings);
    if (newPlayerMode != hPlayer->mode)
    {
        playerModeChanged = true;
    }

    /* Using the new PlayerSettings, determine app has changed video track. */
    selectVideoTrackAndCodec(hPlayer, pNewPlayerSettings, newPlayerMode, &newVideoTrackId, &newVideoCodec, &videoTrackIdChanged);

    /* Using the new PlayerSettings, determine app has changed audio track. */
    completionStatus = selectAudioTrackAndCodec( hPlayer, pNewPlayerSettings, newPlayerMode, &newAudioTrackId, &newAudioCodec, &usePlaypump2ForAudio, &transportTypeForPlaypump2, &audioTrackIdChanged);
    BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "selectAudioTrackAndCodec Failed"), error, completionStatus, completionStatus );

    /* Validate settings that can be changed during runtime. */
    if ( playerModeChanged && newPlayerMode == BIP_PlayerMode_eRecord && (videoTrackIdChanged || audioTrackIdChanged) )
    {
        BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: Can't change trackId when Player is being switched to recording mode: modes cur=%s new=%s " BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(hPlayer->mode), BIP_ToStr_BIP_PlayerMode(newPlayerMode) ));
        return (BIP_ERR_INVALID_PARAMETER);
    }
    /* TODO: add more PlayerSettings validation here. */

    /* Now we have valid settings, so apply them. */

    /* We start w/ track related changes. */
    if ( videoTrackIdChanged  && hPlayer->videoTrackId != newVideoTrackId )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: video trackId has changed: cur=0x%x new0x%x modes cur=%s new=%s " BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->videoTrackId, newVideoTrackId, BIP_ToStr_BIP_PlayerMode(hPlayer->mode), BIP_ToStr_BIP_PlayerMode(newPlayerMode) ));
        /* TODO: add this logic later as it is not a primary usage case right now. */
        BDBG_ASSERT(NULL);
    }

    if ( audioTrackIdChanged )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: audio trackId has changed: ReConfigure to play different audio track" BIP_MSG_PRE_ARG, hPlayer));
        hPlayer->hPrimaryAudioDecoder = pNewPlayerSettings->audioTrackSettings.pidTypeSettings.audio.primary;
        hPlayer->hSimplePrimaryAudioDecoder = pNewPlayerSettings->audioTrackSettings.pidTypeSettings.audio.simpleDecoder;
        completionStatus = playerReconfigureAudioTrack(hPlayer, newAudioTrackId, newAudioCodec, usePlaypump2ForAudio, transportTypeForPlaypump2, &pNewPlayerSettings->audioTrackSettings );
        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "playerReconfigureAudioTrack Failed"), error, completionStatus, completionStatus );

        /* All is well, so cache the updated trackId. */
        hPlayer->audioTrackId = newAudioTrackId;
    }

    if ( playerModeChanged && newPlayerMode == BIP_PlayerMode_eRecord )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: going to recording mode: modes cur=%s new=%s " BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(hPlayer->mode), BIP_ToStr_BIP_PlayerMode(newPlayerMode) ));
        /* tell PBIP to not use handles & reconfigure clock. */
#if 0
        /* TODO: defer this setting as PBIP has to be changed to remove any callbacks from the Decoders & it's not really needed for now. */
        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
#endif
        hPlayer->mode = newPlayerMode;
    }
    else if ( playerModeChanged && newPlayerMode != BIP_PlayerMode_eRecord )
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: going to decoding mode: modes cur=%s new=%s " BIP_MSG_PRE_ARG, hPlayer, BIP_ToStr_BIP_PlayerMode(hPlayer->mode), BIP_ToStr_BIP_PlayerMode(newPlayerMode) ));

        completionStatus = setupClockRecoveryForPlayer(hPlayer);
        BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "setupClockRecoveryForPlayer Failed"), error, BIP_ERR_NEXUS, completionStatus );
        hPlayer->mode = newPlayerMode;
    }
    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Complete!" BIP_MSG_PRE_ARG, hPlayer ));
error:
    return (completionStatus);
} /* setSettings */

static BIP_Status processPreparingState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;

    NEXUS_Error nrc;
    const BIP_PlayerPrepareSettings         *pPrepareSettings = hPlayer->prepareApi.pPrepareSettings;
    const BIP_PlayerSettings                *pPlayerSettings = hPlayer->prepareApi.pPlayerSettings;
    const BIP_PlayerProbeMediaInfoSettings  *pProbeSettings = hPlayer->prepareApi.pProbeSettings;
    const BIP_PlayerStreamInfo              *pStreamInfo = hPlayer->prepareApi.pStreamInfo;
    BIP_PlayerPrepareStatus                 *pPrepareStatus = hPlayer->prepareApi.pPrepareStatus;
    BIP_PlayerSettings                      playerSettings;

    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    if ( hPlayer->subState == BIP_PlayerSubState_ePreparingNew )
    {
        /* We are here because we have a new _Prepare() API called from App. Initiate this processing. */

        /* Note: BIP_Player_Prepare() has validated the required API arguments. */
        bool needMediaProbeInfo = false;

        /* App provides info on desired callbacks via playerSettings.playbackSettings structure fields. Save those app callbaks. */
        /* We will fill-in BIP Player's callback w/ Nexus Playback (if it is used for playing media) and then invoke app's callback via Arb. */
        hPlayer->endOfStreamCallback = hPlayer->prepareApi.pPlayerSettings->playbackSettings.endOfStreamCallback;
        hPlayer->beginningOfStreamCallback = hPlayer->prepareApi.pPlayerSettings->playbackSettings.beginningOfStreamCallback;
        hPlayer->errorCallback = hPlayer->prepareApi.pPlayerSettings->playbackSettings.errorCallback;

        /* Now that we have saved the Apps's callback descriptors, clear out the Nexus Playback's playbackSettings as we don't app it to directly call Apps. */
        /* Instead, we will fill-in our callback and then in-turn call App after running thru our state machine. */
        hPlayer->playerSettings.playbackSettings.endOfStreamCallback.callback = NULL;
        hPlayer->playerSettings.playbackSettings.endOfStreamCallback.context = NULL;
        hPlayer->playerSettings.playbackSettings.errorCallback.callback = NULL;
        hPlayer->playerSettings.playbackSettings.errorCallback.context = NULL;
        hPlayer->playerSettings.playbackSettings.beginningOfStreamCallback.callback = NULL;
        hPlayer->playerSettings.playbackSettings.beginningOfStreamCallback.context = NULL;
        hPlayer->prepareSettings = *pPrepareSettings;

        if (pPrepareStatus) BKNI_Memset(pPrepareStatus, 0, sizeof(BIP_PlayerPrepareStatus));

        /* Determine the Player Mode. */
        hPlayer->mode = determinePlayerMode(hPlayer, pPlayerSettings);

        /*
         * App is not required to explicitly call _ProbeMediaInfo. We determine whether probe should be done or not.
         * Then, we will call _ProbeForMediaInfo() here if app hasn't apriori done it or if PlayerSettings dont contain required AV info.
         */
        needMediaProbeInfo = false;
        if (!pStreamInfo )
        {
            needMediaProbeInfo = true;
        }
        else if ( pPlayerSettings->lpcm.convertLpcmToWave )
        {
            /* App has provided us LPCM stream info, we dont probe & instead use media info provided in the pPlayerSettings.lpcm structure. */
            needMediaProbeInfo = false;
        }
        else if ( hPlayer->mode == BIP_PlayerMode_eVideoOnlyDecode &&
                (pPlayerSettings->videoTrackId == UINT_MAX || pPlayerSettings->videoTrackSettings.pidTypeSettings.video.codec == NEXUS_VideoCodec_eUnknown) )
        {
            needMediaProbeInfo = true;
        }
        else if ( hPlayer->mode == BIP_PlayerMode_eAudioOnlyDecode &&
                (pPlayerSettings->audioTrackId == UINT_MAX || pPlayerSettings->audioTrackSettings.pidSettings.pidTypeSettings.audio.codec == NEXUS_AudioCodec_eUnknown ))
        {
            needMediaProbeInfo = true;
        }
        else if ( hPlayer->mode == BIP_PlayerMode_eAudioVideoDecode &&
                (pPlayerSettings->videoTrackId == UINT_MAX || pPlayerSettings->videoTrackSettings.pidTypeSettings.video.codec == NEXUS_VideoCodec_eUnknown ||
                 pPlayerSettings->audioTrackId == UINT_MAX || pPlayerSettings->audioTrackSettings.pidSettings.pidTypeSettings.audio.codec == NEXUS_AudioCodec_eUnknown ))
        {
            needMediaProbeInfo = true;
        }
        else if ( pStreamInfo->liveChannel && pStreamInfo->mpeg2Ts.pcrPid == UINT_MAX )
        {
            /* App wants to play a live channel but didn't provide PCR Pid, lets do the probe. */
            needMediaProbeInfo = true;
        }

        if ( needMediaProbeInfo && !hPlayer->mediaProbeComplete )
        {
            /* We need to Probe the media as we haven't yet done so! */
            /* Change player's subState to ProbingNew so that probing state function will act on this request. */
            hPlayer->subState = BIP_PlayerSubState_eProbingNew;
            hPlayer->probeMediaInfoApi.pSettings = pProbeSettings;
            hPlayer->probeMediaInfoApi.phMediaInfo = &pPrepareStatus->hMediaInfo;
            completionStatus = processProbingState_locked( hPlayer, threadOrigin );
            /* We check this completionStatus below in the WaitForMediaInfo state. */

            /* After starting the Probe, we save the current subState as we want to re-run the state machine via this Preparing sub-state function. */
            hPlayer->cachedProbingSubState = hPlayer->subState;
            hPlayer->subState = BIP_PlayerSubState_ePreparingWaitForMediaInfo;
        }
        else if ( !hPlayer->mediaProbeComplete )
        {
            /* needMediaProbeInfo is not set (meaning app has provided us necessary track & codec information needed to play the stream. So we tell PBIP to skip the probe. */
            /* Note: when we skip the media probe, B_PlaybackIp_SessionSetup() is a quick call, so we just directly invoke the call here. */
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Skipping Media Probe as App provided the info, tell PBIP to skip probe!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            B_PlaybackIp_GetDefaultSessionSetupSettings(&hPlayer->pbipState.ipSessionSetupSettings);
            BKNI_Memset(&hPlayer->pbipState.ipSessionSetupStatus, 0, sizeof(hPlayer->pbipState.ipSessionSetupStatus));

            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleHttp)
            {
                hPlayer->pbipState.ipSessionSetupSettings.u.http.skipPsiParsing = true;
                if (pPlayerSettings->lpcm.convertLpcmToWave)
                {
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.convertLpcmToWave = true;
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.bitsPerSample = pPlayerSettings->lpcm.bitsPerSample;
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.sampleRate = pPlayerSettings->lpcm.sampleRate;
                    hPlayer->pbipState.ipSessionSetupSettings.u.http.numChannels = pPlayerSettings->lpcm.numChannels;
                }
            }
            else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eSimpleUdp || hPlayer->playerProtocol == BIP_PlayerProtocol_eRtp)
            {
                hPlayer->pbipState.ipSessionSetupSettings.u.udp.skipPsiParsing = true;
            }
            else if (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
            {
                hPlayer->pbipState.ipSessionSetupSettings.u.rtsp.skipPsiParsing = true;
            }

            hPlayer->pbipState.status = B_PlaybackIp_SessionSetup(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipSessionSetupSettings, &hPlayer->pbipState.ipSessionSetupStatus);
            BIP_CHECK_GOTO( (hPlayer->pbipState.status == B_ERROR_SUCCESS), ( "B_PlaybackIp_SessionSetup Failed"), error, BIP_ERR_PLAYER_PROBE, completionStatus );

            /* Since we have the Probe Info, so change state to reflect that. */
            hPlayer->subState = BIP_PlayerSubState_ePreparingMediaInfoAvailable;
            hPlayer->hFile = hPlayer->pbipState.ipSessionSetupStatus.u.http.file;

            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionSetup is success, changed state!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        } /* PBIP SessionSetup call */
        else
        {
            /* Probe Info is available. */
            hPlayer->subState = BIP_PlayerSubState_ePreparingMediaInfoAvailable;
        }
    } /* subState = ePreparingNew */

    if ( hPlayer->subState == BIP_PlayerSubState_ePreparingWaitForMediaInfo )
    {
        if (completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Run the Probing state machine again to get the latest probe status. */
            hPlayer->subState = hPlayer->cachedProbingSubState;
            completionStatus = processProbingState_locked( hPlayer, threadOrigin );
        }
        if (completionStatus == BIP_INF_IN_PROGRESS )
        {
            /* Probe is not yet complete, note current subState & override it w/ our current state. */
            hPlayer->cachedProbingSubState = hPlayer->subState;
            hPlayer->subState = BIP_PlayerSubState_ePreparingWaitForMediaInfo;
            /* Note: We dont run a explict timer in the Prepare sub-state as only slow API call this state invokes is in the Probing state machine (which runs its own timer). */
        }
        else if (completionStatus == BIP_SUCCESS )
        {
            hPlayer->subState = BIP_PlayerSubState_ePreparingMediaInfoAvailable;
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Probe() is successful, now continue w/ Prepare(): url=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->hUrl->urlRaw ));
        }
        else
        {
            hPlayer->subState = BIP_PlayerSubState_ePreparingDone;
        }
    } /* subState = ePreparingWaitForMediaInfo */

    if ( hPlayer->subState == BIP_PlayerSubState_ePreparingMediaInfoAvailable )
    {
        bool appChangedStreamInfo = false;

        if (pStreamInfo && BKNI_Memcmp(&hPlayer->streamInfo, pStreamInfo, sizeof(hPlayer->streamInfo)) != 0)
        {
            /*
             * App provided streamInfo is different than Player version, so use App provided one. This can happen for several reasons:
             *  App didn't Probe Media as it already knows the Media Stream Info (via some out-of-band channel). Or
             *  App overrode some stream information that we had provided it via _ProbeMediaInfo().
             */
            hPlayer->streamInfo = *pStreamInfo;
            appChangedStreamInfo = true;
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "App has provided a different streamInfo than what we had found during Probe(), so use it." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }

        /* If App hasn't provided an explicit clock-recovery mode & app changed the streamInfo, Re-Select Clock Recovery Mode. */
        if (pPlayerSettings->clockRecoveryMode == BIP_PlayerClockRecoveryMode_eInvalid && appChangedStreamInfo)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "App didn't explicitly set the Clock Recovery Mode, so run logic to determine that!" BIP_MSG_PRE_ARG ));
            hPlayer->useLiveIpMode = useLiveModeForIpPlayback(
                    hPlayer->streamInfo.transportType,
                    hPlayer->streamInfo.mpeg2Ts.transportTimeStampEnabled,
                    hPlayer->streamInfo.liveChannel,
                    hPlayer->dataAvailabilityModel,
                    hPlayer->playerProtocol,
                    &hPlayer->clockRecoveryMode);
        }
        else if (pPlayerSettings->clockRecoveryMode != BIP_PlayerClockRecoveryMode_eInvalid)
        {
            /* App didn't change streamInfo (appChangedStreamInfo is false) but it did explicitly set the clockRecoveryMode, so use that. */
            hPlayer->clockRecoveryMode = pPlayerSettings->clockRecoveryMode;
            hPlayer->useLiveIpMode = (hPlayer->clockRecoveryMode == BIP_PlayerClockRecoveryMode_ePull) ? false:true;
            BDBG_MSG(( BIP_MSG_PRE_FMT "App did explicitly set the Clock Recovery Mode=%s useLiveIpMode=%s" BIP_MSG_PRE_ARG, BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode), hPlayer->useLiveIpMode?"Y":"N" ));
        }
        /* else neither app changed the streamInfo nor it changed the clock recovery mode, we will keep the one that we had determined during Probe. */

        /* If App hasn't set the flag whether to use playpump or playback & app changed the streamInfo, then Re-Determine it. */
        if (pStreamInfo && !pStreamInfo->usePlaypump && !pStreamInfo->usePlayback && appChangedStreamInfo)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "App didn't explicitly set whether to use Playpump or Playback, so run logic to determine that!" BIP_MSG_PRE_ARG ));
            hPlayer->useNexusPlaypump = useNexusPlaypumpForIpPlayback(
                    hPlayer->streamInfo.serverSideTrickmodesSupported,
                    hPlayer->streamInfo.timeSeekRangeHeaderEnabled,
                    hPlayer->streamInfo.liveChannel,
                    hPlayer->streamInfo.contentLength,
                    hPlayer->streamInfo.transportType,
                    hPlayer->streamInfo.containerType,
                    hPlayer->playerProtocol);
        }
        else if (pStreamInfo && (pStreamInfo->usePlaypump || pStreamInfo->usePlayback))
        {
            /* App didn't change streamInfo (appChangedStreamInfo is false) but it did explicitly set to use either playpump or playback, so use them. */
            hPlayer->useNexusPlaypump = pStreamInfo->usePlaypump ? true:false;
            if (hPlayer->useNexusPlaypump == false && hPlayer->useLiveIpMode == true )
            {
                hPlayer->useNexusPlaypump = true;
                BDBG_MSG(( BIP_MSG_PRE_FMT "Can't use Nexus Playback for Live Mode, over-riding App setting to use Nexus Playpump instead!" BIP_MSG_PRE_ARG ));
            }
            else
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT "App explicitly set the flag to use %s for feeding-in the stream!" BIP_MSG_PRE_ARG, hPlayer->useNexusPlaypump ? "Nexus Playpump":"Nexus Playback" ));
            }
        }

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p playerMode=%s useLiveIpMode=%s clockRecoveryMode=%s usePlayback=%s usePlaypump=%s transportType=%s playerContainerType=%s" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->mode == BIP_PlayerMode_eRecord? "Record":"Decode", hPlayer->useLiveIpMode?"Y":"N", BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode),
                    hPlayer->streamInfo.usePlayback?"Y":"N", hPlayer->streamInfo.usePlaypump?"Y":"N",
                    BIP_ToStr_NEXUS_TransportType(hPlayer->streamInfo.transportType), BIP_ToStr_BIP_PlayerContainerType(hPlayer->streamInfo.containerType) ));

        /* Now we know the stream info & how to feed the stream (Playpump/Playback).*/
        /* We Next select the Audio and/or Video tracks based on the app provided PlayerSettings. */
        {
            playerSettings = *pPlayerSettings;
            /* Select Video Track first. */
            selectVideoTrackAndCodec(hPlayer, pPlayerSettings, hPlayer->mode, &playerSettings.videoTrackId, &playerSettings.videoTrackSettings.pidTypeSettings.video.codec, NULL /*newAudioTrackId*/ );

            /* Select Audio Track next. */
            completionStatus = selectAudioTrackAndCodec(
                    hPlayer, pPlayerSettings, hPlayer->mode, &playerSettings.audioTrackId,
                    &playerSettings.audioTrackSettings.pidSettings.pidTypeSettings.audio.codec,
                    &hPlayer->usePlaypump2ForAudio, &hPlayer->transportTypeForPlaypump2, NULL /* audioTrackIdChanged */);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "selectAudioTrackAndCodec Failed"), error, completionStatus, completionStatus );
        }

        /* If we have selected either audio or video tracks or are Recording, open & configure Playpump[s] (used for both Playback & Playpump modes). */
        if ( playerSettings.videoTrackId != UINT_MAX || playerSettings.audioTrackId != UINT_MAX || hPlayer->mode == BIP_PlayerMode_eRecord )
        {
            if (!pPrepareSettings->hPlaypump)
            {
                NEXUS_PlaypumpOpenSettings playpumpOpenSettings;

                NEXUS_Playpump_GetDefaultOpenSettings(&playpumpOpenSettings);
                playpumpOpenSettings.fifoSize *= 2;
                playpumpOpenSettings.numDescriptors = 200;
                hPlayer->hPlaypump = NEXUS_Playpump_Open(NEXUS_ANY_ID, &playpumpOpenSettings);
                BIP_CHECK_GOTO(( hPlayer->hPlaypump ), ( "NEXUS_Playpump_Open Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is opened!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
            }
            else
            {
                hPlayer->hPlaypump = pPrepareSettings->hPlaypump;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Using app provided playpump=%p!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
            }

            {
                NEXUS_PlaypumpSettings settings;

                NEXUS_Playpump_GetSettings(hPlayer->hPlaypump, &settings);
                /* Use the App provided playpumpSettings (which is initialized via BIP_Player_GetDefaultPrepareSettings */
                settings = pPlayerSettings->playbackSettings.playpumpSettings;
                settings.transportType = hPlayer->streamInfo.transportType;
                nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump, &settings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );

                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is setup!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
            }

            if (!pPrepareSettings->hPlaypump2 && hPlayer->usePlaypump2ForAudio)
            {
                NEXUS_PlaypumpOpenSettings playpumpOpenSettings;

                NEXUS_Playpump_GetDefaultOpenSettings(&playpumpOpenSettings);
                /* TODO: need to consider the MPEG DASH buffering requirement for PP2. */
                playpumpOpenSettings.fifoSize *= 2;
                playpumpOpenSettings.numDescriptors = 200;
                hPlayer->hPlaypump2 = NEXUS_Playpump_Open(NEXUS_ANY_ID, &playpumpOpenSettings);
                BIP_CHECK_GOTO(( hPlayer->hPlaypump ), ( "NEXUS_Playpump_Open Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump2=%p is opened!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump2 ));
            }
            else
            {
                hPlayer->hPlaypump2 = pPrepareSettings->hPlaypump2;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: App provided playpump2=%p!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
            }

            if (hPlayer->hPlaypump2)
            {
                NEXUS_PlaypumpSettings settings;

                NEXUS_Playpump_GetSettings(hPlayer->hPlaypump2, &settings);
                /* Use the App provided playpumpSettings (which is initialized via BIP_Player_GetDefaultPrepareSettings */
                settings = pPlayerSettings->playbackSettings.playpumpSettings;
                settings.transportType = hPlayer->transportTypeForPlaypump2;
                nrc = NEXUS_Playpump_SetSettings(hPlayer->hPlaypump2, &settings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );

                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump2=%p is setup!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump2 ));
            }
        }

        /* AV Tracks have been selected, Plapumps have been opened & setup, so now create the PidChannels associated w/ the AV Tracks. */
        {
            /* If we selected a valid video track & codec, then open pidChannel associated with it. */
            {
                if ( playerSettings.videoTrackId != UINT_MAX && playerSettings.videoTrackSettings.pidTypeSettings.video.codec != NEXUS_VideoCodec_eUnknown )
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Open VideoPidChannel for videoTrackId=0x%x videoCodec=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                                playerSettings.videoTrackId, BIP_ToStr_NEXUS_VideoCodec( playerSettings.videoTrackSettings.pidTypeSettings.video.codec) ));

                    hPlayer->hVideoPidChannel = openPidChannel(hPlayer, playerSettings.videoTrackId, &pPlayerSettings->videoTrackSettings);
                    BIP_CHECK_GOTO(( hPlayer->hVideoPidChannel ), ( "openPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );

                    hPlayer->videoTrackId = playerSettings.videoTrackId;
                    hPlayer->hVideoDecoder = pPlayerSettings->videoTrackSettings.pidTypeSettings.video.decoder;
                    hPlayer->hSimpleVideoDecoder = pPlayerSettings->videoTrackSettings.pidTypeSettings.video.simpleDecoder;
                }
                else
                {
                    hPlayer->videoTrackId = UINT_MAX;
                    hPlayer->hVideoDecoder = NULL;
                    hPlayer->hSimpleVideoDecoder = NULL;
                    hPlayer->hVideoPidChannel = NULL;
                }
            }

            /* If we selected a valid audio track & codec, then open pidChannel associated with it. */
            {
                if (playerSettings.audioTrackId != UINT_MAX && playerSettings.audioTrackSettings.pidSettings.pidTypeSettings.audio.codec != NEXUS_AudioCodec_eUnknown)
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Open AudioPidChannel for audioTrackId=0x%x audioCodec=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                                playerSettings.audioTrackId, BIP_ToStr_NEXUS_AudioCodec(playerSettings.audioTrackSettings.pidSettings.pidTypeSettings.audio.codec) ));

                    hPlayer->hPrimaryAudioPidChannel = openPidChannel(hPlayer, playerSettings.audioTrackId, &pPlayerSettings->audioTrackSettings);
                    BIP_CHECK_GOTO(( hPlayer->hPrimaryAudioPidChannel ), ( "openPidChannel Failed"), error, BIP_INF_NEXUS_RESOURCE_NOT_AVAILABLE, completionStatus );

                    hPlayer->audioTrackId = playerSettings.audioTrackId;
                    hPlayer->hPrimaryAudioDecoder = pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.primary;
                    hPlayer->hSimplePrimaryAudioDecoder = pPlayerSettings->audioTrackSettings.pidTypeSettings.audio.simpleDecoder;
                }
                else
                {
                    hPlayer->audioTrackId = UINT_MAX;
                    hPlayer->hPrimaryAudioDecoder = NULL;
                    hPlayer->hSimplePrimaryAudioDecoder = NULL;
                    hPlayer->hPrimaryAudioPidChannel = NULL;
                }
            }

            /* If either Video and/or Audio Pid Channels are opened, fill-in the Prepare Status. */
            if ( hPlayer->hVideoPidChannel || hPlayer->hPrimaryAudioPidChannel )
            {
                /* Save the remaining Nexus handles. */
                hPlayer->hDisplay = pPlayerSettings->hDisplay;
                hPlayer->hWindow = pPlayerSettings->hWindow;
                hPlayer->hStcChannel = pPlayerSettings->playbackSettings.stcChannel;
                hPlayer->hSimpleStcChannel  = pPlayerSettings->playbackSettings.simpleStcChannel;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p prepareSettings: hStc=%p hSimpleStc=%p hDisplay=%p hPlaypum=%p " BIP_MSG_PRE_ARG, hPlayer,
                            pPlayerSettings->playbackSettings.stcChannel, pPlayerSettings->playbackSettings.simpleStcChannel, pPlayerSettings->hDisplay, pPrepareSettings->hPlaypump));

                BDBG_ASSERT(pPrepareStatus);

                pPrepareStatus->audioCodec = playerSettings.audioTrackSettings.pidSettings.pidTypeSettings.audio.codec;
                pPrepareStatus->videoCodec = playerSettings.videoTrackSettings.pidTypeSettings.video.codec;
                pPrepareStatus->hAudioPidChannel = hPlayer->hPrimaryAudioPidChannel;
                pPrepareStatus->hVideoPidChannel = hPlayer->hVideoPidChannel;

                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: audioCodec=%s audioPidCh=%p videoCodec=%s videoPidCh=%p" BIP_MSG_PRE_ARG, hPlayer,
                            BIP_ToStr_NEXUS_AudioCodec(pPrepareStatus->audioCodec), pPrepareStatus->hAudioPidChannel,
                            BIP_ToStr_NEXUS_VideoCodec(pPrepareStatus->videoCodec), pPrepareStatus->hVideoPidChannel));
                BIP_MSG_SUM(( BIP_MSG_PRE_FMT "hPlayer=%p: Current Settings: videoTrackId=0x%x, videoCodec=%s, audioTrackId=0x%x, audioCodec=%s, pcrPid=0x%x, transportType=%s" BIP_MSG_PRE_ARG,
                            hPlayer, playerSettings.videoTrackId, BIP_ToStr_NEXUS_VideoCodec(playerSettings.videoTrackSettings.pidTypeSettings.video.codec),
                            playerSettings.audioTrackId, BIP_ToStr_NEXUS_AudioCodec(playerSettings.audioTrackSettings.pidSettings.pidTypeSettings.audio.codec), hPlayer->streamInfo.mpeg2Ts.pcrPid,
                            BIP_ToStr_NEXUS_TransportType(hPlayer->streamInfo.transportType) ));
                /* TODO: extra videoPidChannel ? */
            }
            else
            {
                completionStatus = BIP_SUCCESS;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Neither Video nor Audio codecs & trackIds are provided, so didn't need to open pid channels." BIP_MSG_PRE_ARG, hPlayer ));
            }

            /* All well regarding the PlayerSettings, so cache them. */
            hPlayer->playerSettings = playerSettings;
        }

        /* So far, so good. We have the pid channels, so do the clock recovery setup if Player is in Decode mode. */
        if (hPlayer->mode != BIP_PlayerMode_eRecord && ( hPlayer->hVideoPidChannel || hPlayer->hPrimaryAudioPidChannel ))
        {
            completionStatus = setupClockRecoveryForPlayer(hPlayer);
            BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "setupClockRecoveryForPlayer Failed"), error, BIP_ERR_NEXUS, completionStatus );
        }

        /* For adaptive protocols, enable smooth transition during media segment stream paramter changes. */
        if ( hPlayer->mode != BIP_PlayerMode_eRecord && hPlayer->hWindow && ( hPlayer->hVideoPidChannel || hPlayer->hPrimaryAudioPidChannel ) &&
                (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls || hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eMpegDash) )
        {
            {
                NEXUS_VideoWindowSettings windowSettings;

                NEXUS_VideoWindow_GetSettings(hPlayer->hWindow, &windowSettings);
                /* (1) Won't re-alloc buffers to meet source size changges and also put MADR/MCVP HW into bypass when not needed. */
                windowSettings.minimumSourceFormat = NEXUS_VideoFormat_e1080p;

                /* (2) Use fullscreen size buffer instead of PIP/PIG size to determine allocation. */
                windowSettings.allocateFullScreen = true;

                /* (3) Not auto snapping to integer scaling factor. */
                windowSettings.scaleFactorRounding.enabled = true;
                windowSettings.scaleFactorRounding.verticalTolerance = 0;
                windowSettings.scaleFactorRounding.horizontalTolerance = 0;

                nrc = NEXUS_VideoWindow_SetSettings(hPlayer->hWindow, &windowSettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_VideoWindow_SetSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );
            }

            {
                NEXUS_VideoWindowScalerSettings scalerSettings;

                /* (4) Fixed scaler -> capture vs capture -> scaler orienation to avoid reconfiguration of VNET (Video Network). */
                /* This require RTS and Usage analysis to make sure it supportted. */
                NEXUS_VideoWindow_GetScalerSettings(hPlayer->hWindow, &scalerSettings);
                scalerSettings.bandwidthEquationParams.bias = NEXUS_ScalerCaptureBias_eScalerBeforeCapture;
                scalerSettings.bandwidthEquationParams.delta = 1*1000*1000;
                nrc = NEXUS_VideoWindow_SetScalerSettings(hPlayer->hWindow, &scalerSettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_VideoWindow_SetScalerSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );
            }

            {
                NEXUS_VideoWindowMadSettings madSettings;

                /* (5) For chips (7400/7405/7420/7408) that have older deinterlacer that does not have HW bypass capability setting this flag will */
                /* prevent deinteralcer forced off when in 480i->480i, * 576i->576i, and 1080i->1080i non-scaled mode.  See also jira  SW7420-2423.*/
                NEXUS_VideoWindow_GetMadSettings(hPlayer->hWindow, &madSettings);
                madSettings.pqEnhancement = NEXUS_MadPqEnhancement_eOff;
                nrc = NEXUS_VideoWindow_SetMadSettings(hPlayer->hWindow, &madSettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_VideoWindow_SetMadSettings Failed"), error, BIP_ERR_NEXUS, completionStatus );
            }
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Updated VideoWindow settings for smooth transitions during stream parameter changes in HLS/MPEG DASH Adaptive Playback " BIP_MSG_PRE_ARG, hPlayer));
        }

        /* Prepare is successful, move to Done state. */
        completionStatus = BIP_SUCCESS;
        hPlayer->subState = BIP_PlayerSubState_ePreparingDone;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Successful!" BIP_MSG_PRE_ARG, hPlayer));
    } /* subState = ePreparingMediaInfoAvailable */

error:
    /* Double check if completionStatus is NOT in progress. This can happen if we get here via a jump to error label. */
    if ( completionStatus != BIP_INF_IN_PROGRESS )
    {
        /* API is done either due to Success or Error status. Move to Done state. completionStatus must be set before this. */
        hPlayer->subState = BIP_PlayerSubState_ePreparingDone;
    }

    /* If subState is Done, then update the states based on the completionStatus. */
    if ( hPlayer->subState == BIP_PlayerSubState_ePreparingDone )
    {
        if ( completionStatus == BIP_SUCCESS )
        {
            /* We are successfuly done w/ _Connect, so lets move to the next state. */
            hPlayer->state = BIP_PlayerState_ePrepared;
            hPlayer->subState = BIP_PlayerSubState_eIdle;
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Prepare() successful for url=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->hUrl->urlRaw ));
        }
        else
        {
            /* Error case */
            hPlayer->state = BIP_PlayerState_eConnected;
            hPlayer->subState = BIP_PlayerSubState_eIdle;

            /* Cleanup any resources acquired above in this Prepare processing. */
            unPrepare(hPlayer);
            BDBG_ERR(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Prepare() Failed, status=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
        }

        /* Check if disconnecting flag is set, meaning _Disconnect()/_Destroy() API must have been called which completed this state. */
        if (hPlayer->disconnecting)
        {
            /* Set a flag to re-run the top processPlayerState() so that it can process the actual _Disconnect()/_Destroy() API. */
            hPlayer->reRunState = true;
        }

        /* And finally complete the Connect Arb, so that caller can be notified of its completion. */
        BDBG_ASSERT(BIP_Arb_IsBusy(hPlayer->prepareApi.hArb));
        BIP_Arb_CompleteRequest( hPlayer->prepareApi.hArb, completionStatus );
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    return (completionStatus);
} /* processPreparingState_locked */

static BIP_Status processStartingState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    B_Error rc;
    NEXUS_Error nrc;
    BIP_Status completionStatus = BIP_ERR_INTERNAL;

    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    if (hPlayer->subState == BIP_PlayerSubState_eStartingNew)
    {
        /* Default Player playRate is 1x (NEXUS_NORMAL_PLAY_SPEED). */
        hPlayer->playRate = NEXUS_NORMAL_PLAY_SPEED;

        if (hPlayer->startApi.pSettings->timePositionInMs)
        {
            /* If app wants to start from a non-zero time position, then we will startPaused first, seek to the new position, and then Play. */
            hPlayer->playerSettings.playbackSettings.startPaused = true;
        }

        /* Reset flags indicating EOS or Error Callbacks. */
        hPlayer->receivedNetworkError = false;
        hPlayer->reachedEndOfStream = false;
        hPlayer->reachedBeginningOfStream = false;

        /*
         * Player needs to start either Nexus Playback or Playpump & PBIP.
         * These modules are started in this order:
         * - When Playpump is being used, we start Playpump & then PBIP as PBIP is driving the feeding of AV stream into the Playpump.
         * - When Playback is being used, we start PBIP & then Playback as Playback is driving the consuming of AV stream by reading it from PBIP bfile i/f.
         */
        if (hPlayer->useNexusPlaypump && hPlayer->hPlaypump)
        {
            nrc = NEXUS_Playpump_Start(hPlayer->hPlaypump);
            BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_Start Failed"), error, BIP_ERR_NEXUS, completionStatus );
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump=%p is started!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));

            if (hPlayer->usePlaypump2ForAudio)
            {
                nrc = NEXUS_Playpump_Start(hPlayer->hPlaypump2);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playpump_Start Failed"), error, BIP_ERR_NEXUS, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playpump2=%p is started!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump2 ));
            }

            if (hPlayer->playerSettings.playbackSettings.startPaused)
            {
                if (hPlayer->hSimpleStcChannel)
                {
                    nrc = NEXUS_SimpleStcChannel_SetRate(hPlayer->hSimpleStcChannel, 0, 0 );
                    BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_SimpleStcChannel_SetRate Failed"), error, BIP_ERR_NEXUS, completionStatus );
                }
                else if (hPlayer->hStcChannel)
                {
                    nrc = NEXUS_StcChannel_SetRate(hPlayer->hStcChannel, 0, 0 );
                    BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_StcChannel_SetRate Failed"), error, BIP_ERR_NEXUS, completionStatus );
                }
            }
        }

        /* Start PBIP so the playback can start. */
        {
            memset(&hPlayer->pbipState.ipStartSettings, 0, sizeof(hPlayer->pbipState.ipStartSettings));
            hPlayer->pbipState.ipStartSettings.mpegType = hPlayer->streamInfo.transportType;
            hPlayer->pbipState.ipStartSettings.nexusHandlesValid = true;
            if (hPlayer->playerSettings.videoTrackId != UINT_MAX)
            {
                hPlayer->pbipState.ipStartSettings.nexusHandles.videoDecoder = hPlayer->hVideoDecoder;
                hPlayer->pbipState.ipStartSettings.nexusHandles.simpleVideoDecoder = hPlayer->hSimpleVideoDecoder;
                hPlayer->pbipState.ipStartSettings.nexusHandles.videoPidChannel = hPlayer->hVideoPidChannel;
                hPlayer->pbipState.ipStartSettings.nexusHandles.videoStartSettings = hPlayer->startApi.pSettings->videoStartSettings;
                hPlayer->pbipState.ipStartSettings.nexusHandles.simpleVideoStartSettings = hPlayer->startApi.pSettings->simpleVideoStartSettings;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Providing video decoder handles to PBIP" BIP_MSG_PRE_ARG, hPlayer ));
            }
            if (hPlayer->playerSettings.audioTrackId != UINT_MAX)
            {
                hPlayer->pbipState.ipStartSettings.nexusHandles.primaryAudioDecoder = hPlayer->hPrimaryAudioDecoder;
                hPlayer->pbipState.ipStartSettings.nexusHandles.simpleAudioDecoder = hPlayer->hSimplePrimaryAudioDecoder;
                hPlayer->pbipState.ipStartSettings.nexusHandles.audioPidChannel = hPlayer->hPrimaryAudioPidChannel;
                hPlayer->pbipState.ipStartSettings.nexusHandles.audioStartSettings = hPlayer->startApi.pSettings->audioStartSettings;
                hPlayer->pbipState.ipStartSettings.nexusHandles.simpleAudioStartSettings = hPlayer->startApi.pSettings->simpleAudioStartSettings;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Providing audio decoder handles to PBIP" BIP_MSG_PRE_ARG, hPlayer ));
            }
            hPlayer->pbipState.ipStartSettings.nexusHandles.stcChannel  = hPlayer->hStcChannel;
            hPlayer->pbipState.ipStartSettings.nexusHandles.simpleStcChannel  = hPlayer->hSimpleStcChannel;
            hPlayer->pbipState.ipStartSettings.nexusHandles.playpump = hPlayer->hPlaypump;
            hPlayer->pbipState.ipStartSettings.nexusHandles.playpump2 = hPlayer->hPlaypump2;
            hPlayer->pbipState.ipStartSettings.nexusHandles.playback = hPlayer->hPlayback;
            hPlayer->pbipState.ipStartSettings.nexusHandles.pcrPidChannel = hPlayer->hPcrPidChannel;
            hPlayer->pbipState.ipStartSettings.startPaused = hPlayer->playerSettings.playbackSettings.startPaused;
            if (hPlayer->dlnaFlags.s0Increasing && hPlayer->dlnaFlags.sNIncreasing && hPlayer->startApi.pSettings->timePositionInMs == 0 )
            {
                hPlayer->pbipState.ipStartSettings.u.http.initialPlayPositionOffsetInMs = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs;
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Providing the initialPlayPositionOffsetInMs=%u to PBIP" BIP_MSG_PRE_ARG, hPlayer, hPlayer->pbipState.ipStartSettings.u.http.initialPlayPositionOffsetInMs ));
            }
            else
            {
                /* TODO: Add starting position for container types like HLS & DASH. */
            }

            if (hPlayer->playerSettings.disableMediaPositionUsingWallClockTime == false)
            {
                hPlayer->pbipState.ipStartSettings.mediaPositionUsingWallClockTime = true;
            }
            if (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
            {
                hPlayer->pbipState.ipStartSettings.u.rtsp.mediaTransportProtocol = B_PlaybackIpProtocol_eRtp;
                hPlayer->pbipState.ipStartSettings.u.rtsp.keepAliveInterval = 10; /* rtsp heart beats. */
            }

            if ( hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls && hPlayer->usePlaypump2ForAudio )
            {
                hPlayer->pbipState.ipStartSettings.startAlternateAudio = true;
                hPlayer->pbipState.ipStartSettings.alternateAudio.groupId = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.pGroupId;
                hPlayer->pbipState.ipStartSettings.alternateAudio.pid = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.hlsAudioPid;
                hPlayer->pbipState.ipStartSettings.alternateAudio.containerType = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.hlsExtraAudioSpecificContainerType;
                hPlayer->pbipState.ipStartSettings.alternateAudio.language = hPlayer->audioTrackInfo.info.audio.containerSpecificInfo.hls.pHlsLanguageCode;
            }
            hPlayer->pbipState.status = B_PlaybackIp_SessionStart(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipStartSettings, &hPlayer->pbipState.ipStartStatus);
            hPlayer->subState = BIP_PlayerSubState_eStartingWaitForPbipStart;
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    } /* if (hPlayer->subState == BIP_PlayerSubState_eStartingNew) */

    if ( hPlayer->subState == BIP_PlayerSubState_eStartingWaitForPbipStart )
    {
        if (hPlayer->pbipState.status == B_ERROR_IN_PROGRESS)
        {
            hPlayer->pbipState.status = B_PlaybackIp_SessionStart(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.ipStartSettings, &hPlayer->pbipState.ipStartStatus);
        }
        if (hPlayer->pbipState.status == B_ERROR_IN_PROGRESS)
        {
            completionStatus = BIP_INF_IN_PROGRESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Waiting for B_PlaybacIp_SessionStart to complete!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->pbipState.status == B_ERROR_SUCCESS)
        {
            /* PBIP Start is successful. Now Start Nexus Playback if Player is using it instead of Nexux Playpump. */
            if (!hPlayer->useNexusPlaypump && hPlayer->hPlayback)
            {
                /* Using Nexus Playback. */
                NEXUS_PlaybackStartSettings startSettings;
                NEXUS_PlaybackSettings settings;

                if (hPlayer->playerSettings.playbackSettings.startPaused)
                {
                    NEXUS_Playback_GetSettings(hPlayer->hPlayback, &settings);
                    settings.startPaused = true;
                    nrc = NEXUS_Playback_SetSettings(hPlayer->hPlayback, &settings);
                    BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playback_SetSettings Failed"), error, BIP_ERR_PLAYER_PBIP, completionStatus );
                }

                NEXUS_Playback_GetDefaultStartSettings(&startSettings);
                if (hPlayer->streamInfo.transportType == NEXUS_TransportType_eTs) { startSettings.mpeg2TsIndexType = NEXUS_PlaybackMpeg2TsIndexType_eSelf; }
                nrc = NEXUS_Playback_Start(hPlayer->hPlayback, hPlayer->hFile, &startSettings);
                BIP_CHECK_GOTO(( nrc==NEXUS_SUCCESS ), ( "NEXUS_Playback_Start Failed"), error, BIP_ERR_NEXUS, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Nexus playback=%p is started!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlayback ));
            }

            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Playpump/Playback & PBIP Start is successful: startPositionInMs=%d" BIP_MSG_PRE_ARG, hPlayer, hPlayer->startApi.pSettings->timePositionInMs ));
            completionStatus = BIP_SUCCESS;

            /* Playpump/Playback & PBIP starts are successful. Check if app asked us to start from a non-zero time position. */
            if (hPlayer->startApi.pSettings->timePositionInMs)
            {
                B_PlaybackIpTrickModesSettings ipTrickModeSettings;

                /* We have already set the startPaused flag above. Now, lets seek to this offset & then call Play. */
                B_PlaybackIp_GetTrickModeSettings(hPlayer->pbipState.hPlaybackIp, &ipTrickModeSettings);
                ipTrickModeSettings.seekPosition = hPlayer->startApi.pSettings->timePositionInMs;
                ipTrickModeSettings.nonBlockingMode = false; /* TODO: make this non-blocking & add additional state here. */
                rc = B_PlaybackIp_Seek(hPlayer->pbipState.hPlaybackIp, &ipTrickModeSettings);
                BIP_CHECK_GOTO(( rc==B_ERROR_SUCCESS ), ( "B_PlaybackIp_Seek Failed"), error, BIP_ERR_PLAYER_PBIP, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: B_PlaybacIp_Seek to startPosition=%d ms is Successful!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->startApi.pSettings->timePositionInMs ));

                /* Check if we need to reconfigure the clock recovery mode based on the data model & its seek position wrt to end of timeshift buffer. */
                if ( hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess )
                {
                    BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: app's startingPosition=%u is different than default of live point for LimitedRandonAccess data model (timeshifted content)"
                              BIP_MSG_PRE_ARG, hPlayer, hPlayer->startApi.pSettings->timePositionInMs));
                    completionStatus = determineAndReConfigureClockRecovery(hPlayer, NEXUS_NORMAL_PLAY_SPEED, hPlayer->startApi.pSettings->timePositionInMs, hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs);
                    BIP_CHECK_GOTO(( completionStatus == BIP_SUCCESS ), ( "determineAndReConfigureClockRecovery Failed"), error, completionStatus, completionStatus );
                }

                /* We have seeked to the initial position, so start Playing. */
                rc = B_PlaybackIp_Play(hPlayer->pbipState.hPlaybackIp);
                BIP_CHECK_GOTO(( rc==B_ERROR_SUCCESS ), ( "B_PlaybackIp_Play Failed"), error, BIP_ERR_PLAYER_PBIP, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: B_PlaybacIp_Play is Successful to start from an initial start offset!" BIP_MSG_PRE_ARG, hPlayer));

                /* Clear this flag so that we can switch to the playing state. */
                hPlayer->playerSettings.playbackSettings.startPaused = false;
                completionStatus = BIP_SUCCESS;
            }
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Player Start is completed, do the state transition!" BIP_MSG_PRE_ARG, hPlayer ));
        }
        else
        {
            completionStatus = BIP_ERR_PLAYER_PBIP;
            BDBG_ERR(( BIP_MSG_PRE_FMT "hPlayer=%p: B_PlaybacIp_SessionStart failed w/ pbipStatus = %d!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->pbipState.status ));
        }
    } /* subState == BIP_PlayerSubState_eStartingWaitForPbipStart */

error:
    if (completionStatus != BIP_INF_IN_PROGRESS)
    {
        /* We are done with the Starting state. */
        hPlayer->subState = BIP_PlayerSubState_eStartingDone;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: Player Start is done: completionStatus=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_StatusGetText(completionStatus) ));
    }

    if (hPlayer->subState == BIP_PlayerSubState_eStartingDone)
    {
        if ( completionStatus == BIP_SUCCESS)
        {
            /* All is well, so change state! */

            /* We switch to Paused state if app had requested us to start in the Paused state. */
            if (hPlayer->playerSettings.playbackSettings.startPaused)
            {
                hPlayer->subState = BIP_PlayerSubState_ePaused;
                hPlayer->state = BIP_PlayerState_ePaused;
            }
            else
            {
                hPlayer->subState = BIP_PlayerSubState_eStartedPlayingNormal;
                hPlayer->state = BIP_PlayerState_eStarted;
            }
            hPlayer->startSettings = *hPlayer->startApi.pSettings;

            /* Reset any state. */
            hPlayer->reachedEndOfStream = false;
            hPlayer->reachedBeginningOfStream = false;
            hPlayer->receivedNetworkError = false;
            hPlayer->seekCompleted = false;

            BIP_MSG_SUM(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Success: live=%s stcMode=%s playback=%s playpump=%s transport=%s security=%s startPaused=%s trick=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        hPlayer->useLiveIpMode?"Y":"N", BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode),
                        hPlayer->streamInfo.usePlayback?"Y":"N", hPlayer->streamInfo.usePlaypump?"Y":"N",
                        BIP_ToStr_NEXUS_TransportType(hPlayer->streamInfo.transportType),
                        hPlayer->dtcpIpEnabled?"dtcpIp": (hPlayer->sslEnabled?"ssl":"none"),
                        hPlayer->playerSettings.playbackSettings.startPaused?"Y":"N",
                        hPlayer->dlnaFlags.playSpeedSupported? "serverSide":"clientSide"
                        ));
            BIP_MSG_SUM(( BIP_MSG_PRE_FMT "url=%s" BIP_MSG_PRE_ARG, hPlayer->hUrl->urlRaw));
        }
        else
        {
            hPlayer->subState = BIP_PlayerSubState_eIdle;
            hPlayer->state = BIP_PlayerState_ePrepared;
            BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Start() Failed: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
        }

        /* And finally complete the Arb, so that caller can be notified of its completion. */
        if (BIP_Arb_IsBusy(hPlayer->startApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->startApi.hArb, completionStatus );
    } /* subState == BIP_PlayerSubState_eStartingDone */

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    return (completionStatus);
} /* processStartingState_locked */

static BIP_Status enqueApiForLaterProcessing(
    BIP_PlayerHandle hPlayer
    )
{
    BIP_ArbHandle   hArb;
    BIP_Status      completionStatus = BIP_ERR_INVALID_API_SEQUENCE;

    if (
            BIP_Arb_IsNew(hArb = hPlayer->seekApi.hArb) ||
            BIP_Arb_IsNew(hArb = hPlayer->pauseApi.hArb) ||
            BIP_Arb_IsNew(hArb = hPlayer->playApi.hArb) ||
            BIP_Arb_IsNew(hArb = hPlayer->playAtRateApi.hArb) ||
            BIP_Arb_IsNew(hArb = hPlayer->playByFrameApi.hArb)
            )
    {
        BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP Player currently doesn't support simultaneous invocation of its APIs while one is currently in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        BIP_Arb_RejectRequest(hArb, completionStatus);
        /* TODO: Instead of Failing it, Add this API Arb to a list of Arbs that will get processed once currently pending API is completed. */
    }
    else
    {
        /* No New API is invoked at this time, so we are good. */
        completionStatus = BIP_SUCCESS;
    }
    return (completionStatus);
} /* enqueApiForLaterProcessing */

static void handleApiErrorsAndUpdatePlayerState(
    BIP_PlayerHandle                    hPlayer,
    const char                          *pApiName,
    NEXUS_PlaybackErrorHandlingMode     errorHandlingMode,
    BIP_PlayerState                     previousState,
    BIP_PlayerSubState                  prevSubState,
    BIP_PlayerState                     *newState,
    BIP_PlayerSubState                  *newSubState
    )
{
    if (errorHandlingMode == NEXUS_PlaybackErrorHandlingMode_eAbort)
    {
        /* Since App wants Player to abort playback on error condition, we will change states to indicate that. */
        *newState = BIP_PlayerState_eAborted;
        *newSubState = BIP_PlayerSubState_eAborted;
        if (hPlayer->errorCallback.callback)
        {
            BIP_Arb_AddDeferredCallback( hPlayer->startApi.hArb, (BIP_CallbackDesc *) &hPlayer->errorCallback); /* Note: BIP_Callback & NEXUS_Callback structures have same definition! */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Error handling mode is _eAbort: error callback is queued!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "%s() Failed, but errorHandling is set to Abort, so switching to that state!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), pApiName ));
    }
    else if (errorHandlingMode == NEXUS_PlaybackErrorHandlingMode_eIgnore)
    {
        /* Since App wants Player to Ignore the error, we will keep both main & sub states of player to the previous one when Api was started. */
        *newState = previousState;
        *newSubState = prevSubState;
        BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "%s() Failed, but errorHandling is set to Ignore mode, so remaining in the previous state!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), pApiName ));
    }
    else if (errorHandlingMode == NEXUS_PlaybackErrorHandlingMode_eEndOfStream)
    {
        /* Since App wants Player to treat the error condition same as EndOfStream condition, we will keep the previous state and issue EOS callback if defined. */
        *newState = previousState;
        *newSubState = prevSubState;
        BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "%s() Failed, but errorHandling is set to EndOfStream mode, so remaining in the previous state & Queue up the EOS callback" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), pApiName ));
        /* Notify App if they have set the endOfStream callback. */
        if (hPlayer->endOfStreamCallback.callback)
        {
            BIP_Arb_AddDeferredCallback( hPlayer->startApi.hArb, (BIP_CallbackDesc *) &hPlayer->endOfStreamCallback); /* Note: BIP_Callback & NEXUS_Callback structures have same definition! */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Error handling mode is _eEndOfStream: endOfStream callback is queued!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
    }
}

static BIP_Status processGetServerStatusApiState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BSTD_UNUSED( threadOrigin );

    if (hPlayer->state < BIP_PlayerState_eConnected)
    {
        BIP_MSG_TRC(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Can't Get ServerStatus in this state.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        return (BIP_SUCCESS);
    }

    if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eNew)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

        /* Check what type of ServerStatus app is interested in. */
        if ( hPlayer->getStatusFromServerSettings.getAvailableSeekRange )
        {
            /* We only need to directly get the Server Status for LimitedRandomAccess model w/ DLNA sN increasing flags */
            /* as here the availableSeekRange changes over time & PBIP doesn't maintain it. */
            /* For rest of the cases, PBIP maintains the correct durations & positions. */
            if (hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess && hPlayer->dlnaFlags.sNIncreasing )
            {
                /* Server supports availableSeekRange as it has sN bit set in the contentFeatures ORG_FLAGS field (meaning content seekable range is increasing), */
                /* we send HTTP HEAD Request to Server with getAvailableSeekRangeHeader and parse response to get the latest available seekRange. */

                /* TODO: compare current time w/ last time we got status from server. If it is with-in an interval, locally update the positions. This avoids excessive Requests to the Server. */

                /* Start API timer if enabled by the caller. */
                hPlayer->getStatusFromServerState.completionStatus = startApiTimer(hPlayer, hPlayer->getStatusFromServerSettings.timeoutInMs);
                BIP_CHECK_GOTO(( hPlayer->getStatusFromServerState.completionStatus == BIP_SUCCESS ), ( "startApiTimer Failed"), error, hPlayer->getStatusFromServerState.completionStatus, hPlayer->getStatusFromServerState.completionStatus );

                /* Use PBIP SessionOpen API to TCP Connect, Send Request, Recv Response from Server. */
                hPlayer->getStatusFromServerState.hPlaybackIp = B_PlaybackIp_Open(NULL);
                BIP_CHECK_GOTO(( hPlayer->getStatusFromServerState.hPlaybackIp ), ( "B_PlaybackIp_Open Failed"), error, BIP_ERR_PLAYER_PBIP, hPlayer->getStatusFromServerState.completionStatus );

                /* Copy all the base settings from main pbip state as we have already successfully done the session open. */
                hPlayer->getStatusFromServerState.ipSessionOpenSettings = hPlayer->pbipState.ipSessionOpenSettings;
                hPlayer->getStatusFromServerState.ipSessionOpenSettings.u.http.useHeadRequestMethod = true;

                /* Build getAvailableSeekRange header if it is not already done. */
                if ( !hPlayer->getStatusFromServerState.hAdditionalHeaders )
                {
                    hPlayer->getStatusFromServerState.hAdditionalHeaders = BIP_String_CreateFromPrintf("getAvailableSeekRange.dlna.org: 1\r\ngetContentFeatures.dlna.org: 1\r\n%s",
                            hPlayer->connectSettings.pAdditionalHeaders ? hPlayer->connectSettings.pAdditionalHeaders : "");
                    BIP_CHECK_GOTO(( hPlayer->getStatusFromServerState.hAdditionalHeaders ), ( "BIP_String_CreateFromPrintf Failed to create Additional Header String"), error, BIP_ERR_PLAYER_CONNECT, hPlayer->getStatusFromServerState.completionStatus );
                }
                hPlayer->getStatusFromServerState.ipSessionOpenSettings.u.http.additionalHeaders = (char *)BIP_String_GetString(hPlayer->getStatusFromServerState.hAdditionalHeaders);
                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: additionalHeaders=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_String_GetString(hPlayer->getStatusFromServerState.hAdditionalHeaders) ));

                memset( &hPlayer->getStatusFromServerState.ipSessionOpenStatus, 0, sizeof(hPlayer->getStatusFromServerState.ipSessionOpenStatus) );

                BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p server=%s port=%u proto=%d secProto=%d url=%s" BIP_MSG_PRE_ARG, hPlayer,
                            hPlayer->getStatusFromServerState.ipSessionOpenSettings.socketOpenSettings.ipAddr,
                            hPlayer->getStatusFromServerState.ipSessionOpenSettings.socketOpenSettings.port,
                            hPlayer->getStatusFromServerState.ipSessionOpenSettings.socketOpenSettings.protocol,
                            hPlayer->getStatusFromServerState.ipSessionOpenSettings.security.securityProtocol,
                            hPlayer->getStatusFromServerState.ipSessionOpenSettings.socketOpenSettings.url
                         ));

                hPlayer->getStatusFromServerState.status = B_PlaybackIp_SessionOpen(hPlayer->getStatusFromServerState.hPlaybackIp, &hPlayer->getStatusFromServerState.ipSessionOpenSettings, &hPlayer->getStatusFromServerState.ipSessionOpenStatus);
                BIP_CHECK_GOTO(( hPlayer->getStatusFromServerState.status == B_ERROR_IN_PROGRESS || hPlayer->getStatusFromServerState.status == B_ERROR_SUCCESS), ( "B_PlaybackIp_SessionOpen Failed"), error, BIP_ERR_PLAYER_CONNECT, hPlayer->getStatusFromServerState.completionStatus );

                /* Change subState to indicate that we are now waiting for Server Response (which we will check below). */
                hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eInProgress;
            }
            else
            {
                /* For all other cases (Non LimitedRandomAccess w/ DLNA sNIncreaseing flag) of DVR, HLS, etc., we get the Server Status from PBIP. */
                B_PlaybackIpStatus pbipStatus;

                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "DVR/VOD/HLS type Stream, Get ServerStatus Range from PBIP" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

                hPlayer->getStatusFromServerState.status = B_PlaybackIp_GetStatus(hPlayer->pbipState.hPlaybackIp, &pbipStatus);
                BIP_CHECK_GOTO(( hPlayer->getStatusFromServerState.status == B_ERROR_SUCCESS ), ( "B_PlaybackIp_GetStatus Failed"), error, BIP_ERR_PLAYER_PBIP, hPlayer->getStatusFromServerState.status );
                hPlayer->getStatusFromServerState.completionStatus = BIP_SUCCESS;

                hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs = pbipStatus.first;
                if (pbipStatus.last)
                {
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs = pbipStatus.last;
                }
                hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.durationInMs = pbipStatus.last - pbipStatus.first;
                if (BIP_Arb_IsBusy(hPlayer->getStatusFromServerApi.hArb))
                {
                    *hPlayer->getStatusFromServerApi.pServerStatus = hPlayer->getStatusFromServerState.serverStatus;
                }
                hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
            }
        }
        else
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "TODO: add logic to retrieve other server status for this url=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->hUrl->urlRaw ));
            hPlayer->getStatusFromServerState.completionStatus = BIP_ERR_INTERNAL;
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eInProgress)
    {
        if ( hPlayer->getStatusFromServerState.status == B_ERROR_IN_PROGRESS )
        {
            /* Poll again to get the latest PBIP API status. */
            hPlayer->getStatusFromServerState.status = B_PlaybackIp_SessionOpen(hPlayer->getStatusFromServerState.hPlaybackIp, &hPlayer->getStatusFromServerState.ipSessionOpenSettings, &hPlayer->getStatusFromServerState.ipSessionOpenStatus);
        }

        /* Check if we successfuly completed previous PBIP operation & process its results. */
        if ( hPlayer->getStatusFromServerState.status == B_ERROR_IN_PROGRESS )
        {
            /* PBIP Call is still in-progress, check if API has timed out. */
            hPlayer->getStatusFromServerState.completionStatus = BIP_INF_IN_PROGRESS;
            if ( hasApiTimerExpired(hPlayer, hPlayer->getStatusFromServerSettings.timeoutInMs) == true )
            {
                /* API timedout, set status & move to Done state. We will complete the API Arb below. */
                hPlayer->getStatusFromServerState.completionStatus = BIP_INF_TIMEOUT;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Timedout while waiting for PBIP SessionOpen() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else if (hPlayer->disconnecting)
            {
                /* App has called _Disconnect() or _Destroy(), so complete this state. */
                hPlayer->getStatusFromServerState.completionStatus = BIP_ERR_PLAYER_BEING_DISCONNECTED;
                hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
                BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Player is being Disconnected while waiting for PBIP SessionOpen() completion during GetStatusFromServer()" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Still waiting for PBIP SessionOpen() completion!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                hPlayer->getStatusFromServerState.completionStatus = BIP_INF_IN_PROGRESS;
            }
        }
        else if ( hPlayer->getStatusFromServerState.status == B_ERROR_SUCCESS )
        {
            /* PBIP Call has successfuly completed, so lets find out the available seek range. */
            BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionOpen() successful" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            /* Parse the DLNA availableSeekRange Header. */
            hPlayer->getStatusFromServerState.completionStatus = parseAvailableSeekRangeHeader(hPlayer, hPlayer->getStatusFromServerState.ipSessionOpenStatus.u.http.responseHeaders, &hPlayer->getStatusFromServerState.serverStatus);
            if (hPlayer->getStatusFromServerState.completionStatus == BIP_SUCCESS)
            {
                /* Parse the DLNA ContentFeatures Header. */
                hPlayer->getStatusFromServerState.completionStatus = parseContentFeatureHeader(hPlayer, hPlayer->pbipState.ipSessionOpenStatus.u.http.responseHeaders);
                if (hPlayer->getStatusFromServerState.completionStatus == BIP_SUCCESS)
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: parseAvailableSeekRangeHeader & parseContentFeatureHeader successfull!" BIP_MSG_PRE_ARG, hPlayer ));
                    if (BIP_Arb_IsBusy(hPlayer->getStatusFromServerApi.hArb))
                    {
                        *hPlayer->getStatusFromServerApi.pServerStatus = hPlayer->getStatusFromServerState.serverStatus;
                    }
                }
                else
                {
                    BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: parseContentFeatureHeader Failed" BIP_MSG_PRE_ARG, hPlayer ));
                }
            }
            else
            {
                BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: parseAvailableSeekRangeHeader Failed" BIP_MSG_PRE_ARG, hPlayer ));
            }
            free(hPlayer->getStatusFromServerState.ipSessionOpenStatus.u.http.responseHeaders);
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
        }
        else
        {
            /* PBIP Call returned some error, so reflect it in the completionStatus. */
            hPlayer->getStatusFromServerState.completionStatus = BIP_ERR_PLAYER_PBIP;
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
            BDBG_ERR(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP SessionOpen() Failed with PBIP errorStatus=%d"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->getStatusFromServerState.status ));
        }
    }

error:
    if (hPlayer->getStatusFromServerState.completionStatus != BIP_INF_IN_PROGRESS && hPlayer->getStatusFromServerState.apiState != BIP_PlayerApiState_eIdle)
    {
        hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eDone;
    }

    if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Now get the current position. */
        B_PlaybackIpError brc;
        B_PlaybackIpStatus pbipStatus;

        brc = B_PlaybackIp_GetStatus(hPlayer->pbipState.hPlaybackIp, &pbipStatus);
        if (brc == B_ERROR_SUCCESS)
        {
            hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs = pbipStatus.position;
        }
        else
        {
            hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs = 0;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP GetStatus() Failed with PBIP errorStatus=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), brc ));
        }

        /* Since we are done w/ this API (success or failure case), we will complete the API here. */
        if (BIP_Arb_IsBusy(hPlayer->getStatusFromServerApi.hArb))
        {

            hPlayer->getStatusFromServerApi.pServerStatus->currentPositionInMs = hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs;
            BIP_Arb_CompleteRequest(hPlayer->getStatusFromServerApi.hArb, hPlayer->getStatusFromServerState.completionStatus);
        }
        if (hPlayer->getStatusFromServerState.hPlaybackIp)
        {
            B_Error brc;
            brc = B_PlaybackIp_Close(hPlayer->getStatusFromServerState.hPlaybackIp);
            BDBG_ASSERT(brc == B_ERROR_SUCCESS);
            hPlayer->getStatusFromServerState.hPlaybackIp = NULL;
        }

        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: first=%u last=%u duration=%u position=%u completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs,
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs,
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.durationInMs,
                    hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs,
                    BIP_StatusGetText(hPlayer->getStatusFromServerState.completionStatus) ));

        stopApiTimer(hPlayer);
        hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eIdle;
    }
    return (hPlayer->getStatusFromServerState.completionStatus);
} /* processGetServerStatusApiState_locked */

static void truncateAndUpdatePosition(
    BIP_PlayerHandle        hPlayer,
    NEXUS_PlaybackPosition  *pCurrentPosition
    )
{
    B_PlaybackIpStatus      status;
    bool                    currentPositionUpdated = false;
    B_PlaybackIpError       rc;
    NEXUS_PlaybackPosition  currentPosition, origCurrentPosition;

    /* Assumption: Caller has already updated the seekable ranges using processGetServerStatusApiState_locked(). */
    /* This function will adjust the current position as per these seek begin & end bounds. */

    if (!pCurrentPosition)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Get current position from PBIP" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        rc = B_PlaybackIp_GetStatus(hPlayer->pbipState.hPlaybackIp, &status);
        if (rc != B_ERROR_SUCCESS)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP GetStatus() Failed with PBIP errorStatus=%d, resetting to the starting position" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), rc));
            currentPosition = origCurrentPosition = 0;
        }
        else
        {
            currentPosition = origCurrentPosition = status.position;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "currentPosition=%u from PBIP" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), currentPosition ));
            if (currentPosition == 0)
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "updated currentPosition=%u to 2msec" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), currentPosition ));
                currentPosition = 2; /* we go back few msec as PBIP wont include timeSeekRange if it is 0. */
                currentPositionUpdated = true;
            }
        }
    }
    else
    {
        currentPosition = origCurrentPosition = *pCurrentPosition;
    }

    if (currentPosition < hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "dataModel=%s: curPos=%u is < firstPos=%u in ms, setting it to first (seek.begin) "
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel),
                    currentPosition, hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs));
        currentPosition = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs;
        currentPositionUpdated = true;
    }
    else if (currentPosition > hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "dataModel=%s cur=%u is outside last=%u, setting it to last (seek.end) "
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel),
                    currentPosition, hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs));
        currentPosition = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs;
        currentPositionUpdated = true;
    }
    else
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "dataAvailabilityModel=%s: cur=%u is within first=%u & last=%u in ms"
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel), currentPosition,
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs,
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs
                 ));
    }
    if (currentPosition == origCurrentPosition)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "cur=%u & originalCurrentPosition=%u are the same, so not updating the position"
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), currentPosition, origCurrentPosition ));
        currentPositionUpdated = false;
    }

    /* If we had updated the currentPosition, then let PBIP know about this updated currentPosition. */
    if ( currentPositionUpdated )
    {
        B_PlaybackIp_GetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        hPlayer->pbipState.settings.playPositionOffsetValid = true;
        hPlayer->pbipState.settings.playPositionOffsetInMs = currentPosition;
        rc = B_PlaybackIp_SetSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->pbipState.settings);
        if (rc != B_ERROR_SUCCESS)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP B_PlaybackIp_SetSettings() Failed with PBIP errorStatus=%d, continuing Play w/ currentPositionInMs=%u"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), rc, currentPosition ));
        }
        else
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP B_PlaybackIp_SetSettings() succeeded to update to currentPositionInMs=%u"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), currentPosition ));
            hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs = currentPosition;
        }
        if (pCurrentPosition) *pCurrentPosition = currentPosition;
    }
} /* truncateAndUpdatePosition */

static BIP_Status determinePauseTimeoutAndStartTimer(
    BIP_PlayerHandle hPlayer,
    bool             *dontPause
    )
{
    BIP_Status bipStatus;

    /* By default, set dontPause to false. We will only set it to true if we can't pause when current position is at the seek.begin & timeshift buffer is full. */
    *dontPause = false;
    if (hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess && hPlayer->prepareSettings.pauseTimeoutAction == NEXUS_PlaybackLoopMode_ePlay)
    {
        hPlayer->pauseState.pauseTimeoutInMs =
            hPlayer->prepareSettings.timeshiftBufferMaxDurationInMs +
            hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs -
            hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs;

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Pause timeout=%d ms, tsBufMaxDurationInMs=%u curPosInMs=%u lastPosInMs=%u" BIP_MSG_PRE_ARG,
                    hPlayer, hPlayer->pauseState.pauseTimeoutInMs, hPlayer->prepareSettings.timeshiftBufferMaxDurationInMs,
                    hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs,
                    hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs));

        if (hPlayer->pauseState.pauseTimeoutInMs > 0)
        {
            /* We setup the pause timeout timer only for +ve timeout value. */
            BIP_TimerCreateSettings timerCreateSettings;

            B_Time_Get(&hPlayer->pauseState.pauseStartTime);
            timerCreateSettings.input.callback    = processPlayerStateFromPauseTimerCallback;
            timerCreateSettings.input.pContext    = hPlayer;
            timerCreateSettings.input.timeoutInMs = hPlayer->pauseState.pauseTimeoutInMs;
            hPlayer->pauseState.hPauseTimer = BIP_Timer_Create(&timerCreateSettings);
            BIP_CHECK_GOTO(( hPlayer->pauseState.hPauseTimer ), ( "BIP_Timer_Create Failed"), error, BIP_ERR_INTERNAL, bipStatus );

            hPlayer->pauseState.pauseTimerActive = true;
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Pause Timeout timer=%u started." BIP_MSG_PRE_ARG, hPlayer, hPlayer->pauseState.pauseTimeoutInMs));
        }
        else
        {
            /* PauseTimeout happened to be <= 0, meaning the current position is at the oldest position (seek.begin) in the timeshift buffer */
            /* which will anyway expire, so there is no point in pausing. */
            *dontPause = true;
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: setting dontPause=true as Pause Timeout timer=%d will expire now!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->pauseState.pauseTimeoutInMs));
        }
        bipStatus = BIP_SUCCESS;
    }
    else
    {
        /* No need to create a timer. */
        bipStatus = BIP_SUCCESS;
    }

error:
    return (bipStatus);
} /* determinePauseTimeoutAndStartTimer */

static bool hasPauseTimerExpired(
    BIP_PlayerHandle hPlayer,
    int timeoutInMs
    )
{
    B_Time timeNow;
    int elapsedTimeInMs;

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "pauseTimeoutInMs=%d pauseTimerActive=%s"
                BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), timeoutInMs, hPlayer->pauseState.pauseTimerActive?"Y":"N"));
    if (!hPlayer->pauseState.pauseTimerActive) { return false; }

    B_Time_Get(&timeNow);
    elapsedTimeInMs = B_Time_Diff( &timeNow, &hPlayer->pauseState.pauseStartTime );
    if ( elapsedTimeInMs >= timeoutInMs )
    {
        BDBG_WRN(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PauseTimeout Timer expired: pauseTimeoutInMs=%d elapsedTimeInMs=%d "
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), timeoutInMs, elapsedTimeInMs ));
        return true;
    }
    else
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "PauseTimeout still in-progress, pauseTimeoutInMs=%d elapsedTimeInMs=%d "
                    BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), timeoutInMs, elapsedTimeInMs ));
        return false;
    }
} /* hasPauseTimerExpired */

static void stopPauseTimer(
    BIP_PlayerHandle hPlayer
    )
{
    if (hPlayer->pauseState.pauseTimerActive)
    {
        if (hPlayer->pauseState.hPauseTimer) BIP_Timer_Destroy(hPlayer->pauseState.hPauseTimer);
        hPlayer->pauseState.hPauseTimer = NULL;
        hPlayer->pauseState.pauseTimerActive = false;
        BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Stopped PauseTimeout Timer" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    }
} /* stopPauseTimer */

static BIP_Status processPauseApiState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    B_Error rc;
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    if (hPlayer->pauseState.apiState == BIP_PlayerApiState_eNew)
    {
        /* Pause is only supported for LimitedRandom & FullRandom data models (this was verified in the main processPlayerState function). */

        /* Determine pause timeout if it applies to this player instance. This requires us to get the latest available seek range (timeshift window). */
        /* So start w/ getting latest server status. Note: processGetServerStatusApiState() only fetches the status from server for the Limited data model in which timeshift is enabled. */
        if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
        {
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->getStatusFromServerSettings.timeoutInMs = -1;
            hPlayer->getStatusFromServerSettings.getAvailableSeekRange = true;
        }
        hPlayer->pauseState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        hPlayer->pauseState.apiState = BIP_PlayerApiState_eInProgress;
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->pauseState.apiState == BIP_PlayerApiState_eInProgress)
    {
        if (hPlayer->pauseState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Poll processGetServerStatusApiState to see if it has completed. */
            hPlayer->pauseState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        }
        if (hPlayer->pauseState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->pauseState.completionStatus == BIP_SUCCESS)
        {
            B_PlaybackIpTrickModesSettings ipTrickModeSettings;
            bool dontPause = false;
            /* processGetServerStatusApiState successfully completed. */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() successfully completed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            /* adjust the current position using this updated seek ranges. */
            truncateAndUpdatePosition(hPlayer, NULL);

            /* Now determine the pause timeout interval & start the time if it applies! */
            hPlayer->pauseState.completionStatus = determinePauseTimeoutAndStartTimer(hPlayer, &dontPause);
            if (hPlayer->pauseState.completionStatus == BIP_SUCCESS)
            {
                if (dontPause == false)
                {
                    /* We use PBIP Pause() API to pause. Since Pause is a "fast" call (meaning it can't block on any operation), we do it in the synchronous fashion. */
                    /* Pause is only supported for LimitedRandom & FullRandom data models (this was verified in the main processPlayerState function). */

                    B_PlaybackIp_GetTrickModeSettings(hPlayer->pbipState.hPlaybackIp, &ipTrickModeSettings);
                    ipTrickModeSettings.pauseMethod = hPlayer->pauseApi.pSettings->pauseMethod == BIP_PlayerPauseMethod_eUseConnectionStalling ?  B_PlaybackIpPauseMethod_UseConnectionStalling: B_PlaybackIpPauseMethod_UseDisconnectAndSeek;
                    rc = B_PlaybackIp_Pause(hPlayer->pbipState.hPlaybackIp, &ipTrickModeSettings);
                    if (rc == B_ERROR_SUCCESS)
                    {
                        hPlayer->pauseState.completionStatus = BIP_SUCCESS;
                        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Pause is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

                        /* Update playRate to reflect that we are paused. */
                        hPlayer->playRate = 0;
                    }
                    else
                    {
                        /* Pause Failed. */
                        BDBG_ASSERT(rc != B_ERROR_IN_PROGRESS); /* This is the case as PBIP Pause is currently a blocking call. */
                        hPlayer->pauseState.completionStatus = BIP_ERR_PLAYER_PBIP;
                        BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Pause failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                    }
                }
                else
                {
                    hPlayer->pauseState.completionStatus = BIP_INF_PLAYER_CANT_PAUSE;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Can't pause for dataAvailabilityModel=%s at cur=%u first=%u & last=%u in ms"
                                BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel),
                                hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs,
                                hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs,
                                hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs
                    ));
                }
            }
            else
            {
                BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "determinePauseTimeoutAndStartTimer() failed to start the pauseTimeout timer!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            hPlayer->pauseState.apiState = BIP_PlayerApiState_eDone;
        }
        else
        {
            /* processGetServerStatusApiState failed */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            hPlayer->pauseState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eInProgress */

    if (hPlayer->pauseState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Cleanup up: cancel timers, etc.............. */
        hPlayer->pauseState.apiState = BIP_PlayerApiState_eIdle;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: hPlayer->pauseState.completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(hPlayer->pauseState.completionStatus) ));
    return (hPlayer->pauseState.completionStatus);
} /* processPauseApiState_locked */

static BIP_Status processPlayApiState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    if (hPlayer->playState.apiState == BIP_PlayerApiState_eNew)
    {
        /* Trickmodes are only supported for LimitedRandom & FullRandom data models. */
        /* First get the latest status to make sure our current play position is valid in the available seekRange */
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Update latest seek range using processGetServerStatusApiState()" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
        {
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->getStatusFromServerSettings.timeoutInMs = -1;
            hPlayer->getStatusFromServerSettings.getAvailableSeekRange = true;
        }
        hPlayer->playState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        hPlayer->playState.apiState = BIP_PlayerApiState_eInProgress;
        hPlayer->playState.waitingForServerStatus = true;
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->playState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playState.waitingForServerStatus)
    {
        if (hPlayer->playState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Poll processGetServerStatusApiState to see if it has completed. */
            hPlayer->playState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        }
        if (hPlayer->playState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playState.completionStatus == BIP_SUCCESS)
        {
            /* processGetServerStatusApiState successfully completed. */
            hPlayer->playState.waitingForServerStatus = false;

            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() successfully completed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            /* adjust the current position using this updated seek ranges. */
            truncateAndUpdatePosition(hPlayer, NULL);

            /* Check if we need to reconfigure the clock recovery mode based on the data model & its current position wrt to end of timeshift buffer. */
            if ( hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess )
            {
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: reconfigure clock recovery if needed!" BIP_MSG_PRE_ARG, hPlayer ));
                hPlayer->playState.completionStatus = determineAndReConfigureClockRecovery(hPlayer, NEXUS_NORMAL_PLAY_SPEED,
                        hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs,
                        hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs);
            }

            if ( hPlayer->playState.completionStatus == BIP_SUCCESS )
            {
                /* Now call PBIP Play() API to Resume Playing. */
                /* Currently, PBIP Play() is a blocking call, we may need to switch it to non-blocking if it becomes an issue or invoke it from outside the state machine! */
                hPlayer->playState.pbipStatus = B_PlaybackIp_PlayAsync(hPlayer->pbipState.hPlaybackIp);
                /* Note: we will check the playState.pbipStatus below. */
            }
            else
            {
                BDBG_WRN((BIP_MSG_PRE_FMT "hPlayer=%p: determineAndReConfigureClockRecovery Failed" BIP_MSG_PRE_ARG, hPlayer ));
            }

            /* update the local state. */
            hPlayer->playState.waitingForPlayCompletion = true;
        }
        else
        {
            /* processGetServerStatusApiState failed */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            hPlayer->playState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eInProgress && waitingForServerStatus */

    if (hPlayer->playState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playState.waitingForPlayCompletion)
    {
        if (hPlayer->playState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Since Play is still in progress, lets check its status. */
            hPlayer->playState.pbipStatus = B_PlaybackIp_PlayAsync(hPlayer->pbipState.hPlaybackIp);
        }
        if (hPlayer->playState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Play is still in progress, we are done for now. PBIP will give us a callback when Play completes. */

            /* TODO: Check for Play API timeout. */
            hPlayer->playState.completionStatus = BIP_INF_IN_PROGRESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Initiated PBIP Play!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playState.pbipStatus == B_ERROR_SUCCESS)
        {
            hPlayer->playState.completionStatus = BIP_SUCCESS;
            hPlayer->playState.apiState = BIP_PlayerApiState_eDone;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Play is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

            /* Reset Player playRate to 1x (NEXUS_NORMAL_PLAY_SPEED). */
            hPlayer->playRate = NEXUS_NORMAL_PLAY_SPEED;
        }
        else
        {
            /* Play Failed. */
            hPlayer->playState.completionStatus = BIP_ERR_PLAYER_PLAY;
            hPlayer->playState.apiState = BIP_PlayerApiState_eDone;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Play failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
    }

    if (hPlayer->playState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Cleanup up: cancel timers, etc.............. */
        hPlayer->playState.apiState = BIP_PlayerApiState_eIdle;
        hPlayer->playState.waitingForServerStatus = false;
        hPlayer->playState.waitingForPlayCompletion = false;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: hPlayer->playState.completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(hPlayer->playState.completionStatus) ));
    return (hPlayer->playState.completionStatus);
} /* processPlayApiState_locked */

static BIP_Status processSeekApiSubState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    /* First get the latest server status to make sure seek position is valid in the available seekRange */
    if (hPlayer->seekState.apiState == BIP_PlayerApiState_eNew)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Update latest seek range using processGetServerStatusApiState()" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
        {
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->getStatusFromServerSettings.timeoutInMs = -1;
            hPlayer->getStatusFromServerSettings.getAvailableSeekRange = true;
        }
        hPlayer->seekState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);

        hPlayer->seekState.apiState = BIP_PlayerApiState_eInProgress;
        hPlayer->seekState.waitingForServerStatus = true;
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->seekState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->seekState.waitingForServerStatus )
    {
        if (hPlayer->seekState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Poll processGetServerStatusApiState to see if it has completed. */
            hPlayer->seekState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        }
        if (hPlayer->seekState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->seekState.completionStatus == BIP_SUCCESS)
        {
            /* processGetServerStatusApiState successfully completed. */
            hPlayer->seekState.waitingForServerStatus = false;

            /* adjust the current position using this updated seek ranges. */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "truncate seek position (if needed!)" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            truncateAndUpdatePosition(hPlayer, &hPlayer->seekApi.seekPositionInMs);

            /* Check if we need to reconfigure the clock recovery mode based on the data model & its current position wrt to end of timeshift buffer. */
            if ( hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess )
            {
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: reconfigure clock recovery if needed!" BIP_MSG_PRE_ARG, hPlayer ));
                hPlayer->seekState.completionStatus = determineAndReConfigureClockRecovery(hPlayer, hPlayer->state == BIP_PlayerState_ePaused ? 0:NEXUS_NORMAL_PLAY_SPEED,
                        hPlayer->seekApi.seekPositionInMs,
                        hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs);
            }

            /* Now call PBIP Seek() API: Note: we invoke PBIP Seek in the non-blocking way. */
            /* It will give us a callback when it is done which runs this state machine. */
            /* We will then check the status of Seek and accordingly complete the Seek API Arb. */
            if (hPlayer->seekState.completionStatus == BIP_SUCCESS)
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Initiate PBIP Seek())" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                B_PlaybackIp_GetTrickModeSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->seekState.pbipSettings);
                hPlayer->seekState.pbipSettings.seekPositionIsRelative = false;
                hPlayer->seekState.pbipSettings.seekBackward = false;
                hPlayer->seekState.pbipSettings.seekPosition = hPlayer->seekApi.seekPositionInMs;
                hPlayer->seekState.pbipSettings.nonBlockingMode = true;
                hPlayer->seekState.pbipSettings.method = hPlayer->dlnaFlags.timeBasedSeekSupported ? B_PlaybackIpTrickModesMethod_UseTimeSeekRange : B_PlaybackIpTrickModesMethod_UseByteRange;
                hPlayer->seekState.pbipStatus = B_PlaybackIp_Seek(hPlayer->pbipState.hPlaybackIp, &hPlayer->seekState.pbipSettings);
                /* Note: we will check the seekState.pbipStatus below. */
            }
            else
            {
                BDBG_WRN((BIP_MSG_PRE_FMT "hPlayer=%p: determineAndReConfigureClockRecovery Failed" BIP_MSG_PRE_ARG, hPlayer ));
            }

            /* update the local state. */
            hPlayer->seekState.waitingForSeekCompletion = true;
        }
        else
        {
            /* processGetServerStatusApiState failed */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            hPlayer->seekState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eInProgress  && waitingForServerStatus */

    if (hPlayer->seekState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->seekState.waitingForSeekCompletion)
    {
        if (hPlayer->seekState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Since Seek is still in progress, lets check its status. */
            hPlayer->seekState.pbipStatus = B_PlaybackIp_Seek(hPlayer->pbipState.hPlaybackIp, &hPlayer->seekState.pbipSettings);
        }
        if (hPlayer->seekState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Seek is still in progress, we are done for now. PBIP will give us a callback when Seek completes. */

            /* TODO: Check for Seek API timeout. */

            hPlayer->seekState.completionStatus = BIP_INF_IN_PROGRESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Initiated PBIP Seek to %d ms position!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->seekApi.seekPositionInMs ));
        }
        else if (hPlayer->seekState.pbipStatus == B_ERROR_SUCCESS)
        {
            /* Seek is successful. */
            hPlayer->seekState.apiState = BIP_PlayerApiState_eDone;
            hPlayer->seekState.completionStatus = BIP_SUCCESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Seek to %d ms position is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->seekApi.seekPositionInMs ));

            if (hPlayer->state == BIP_PlayerState_ePaused)
            {
                /* Keep playRate to 0 to reflect that we are paused. */
                hPlayer->playRate = 0;
            }
            else
            {
                /* Else, reset Player playRate to 1x (NEXUS_NORMAL_PLAY_SPEED). */
                hPlayer->playRate = NEXUS_NORMAL_PLAY_SPEED;
            }
        }
        else
        {
            /* Seek Failed. */
            hPlayer->seekState.completionStatus = BIP_ERR_PLAYER_SEEK;
            hPlayer->seekState.apiState = BIP_PlayerApiState_eDone;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP Seek to %d ms position failed w/ pbipStatus=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->seekApi.seekPositionInMs, hPlayer->seekState.pbipStatus ));
        }
    }

    if (hPlayer->seekState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Cleanup up: cancel timers, etc.............. */
        hPlayer->seekState.apiState = BIP_PlayerApiState_eIdle;
        hPlayer->seekState.waitingForServerStatus = false;
        hPlayer->seekState.waitingForSeekCompletion = false;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(hPlayer->seekState.completionStatus) ));
    return (hPlayer->seekState.completionStatus);
} /* processSeekApiSubState_locked */

extern int B_PlaybackIp_UtilsParsePlaySpeedString(const char *playSpeedStringOrig, int *speedNumerator, int *speedDenominator, int *direction);
static BIP_Status processPlayAtRateApiSubState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    /* First get the latest server status to make sure seek position is valid in the available seekRange */
    if (hPlayer->playAtRateState.apiState == BIP_PlayerApiState_eNew)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Update latest seek range using processGetServerStatusApiState()" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
        {
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->getStatusFromServerSettings.timeoutInMs = -1;
            hPlayer->getStatusFromServerSettings.getAvailableSeekRange = true;
        }
        hPlayer->playAtRateState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);

        hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eInProgress;
        hPlayer->playAtRateState.waitingForServerStatus = true;
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->playAtRateState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playAtRateState.waitingForServerStatus )
    {
        if (hPlayer->playAtRateState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Poll processGetServerStatusApiState to see if it has completed. */
            hPlayer->playAtRateState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        }
        if (hPlayer->playAtRateState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playAtRateState.completionStatus == BIP_SUCCESS)
        {
            /* processGetServerStatusApiState successfully completed. */
            int speedNumerator=1, speedDenominator=1;

            /* Determine playRate in NEXUS_NORMAL_PLAY_SPEED units. */
            if (hPlayer->playAtRateApi.pRate)
            {
                if (B_PlaybackIp_UtilsParsePlaySpeedString(hPlayer->playAtRateApi.pRate, &speedNumerator, &speedDenominator, NULL /*&direction*/) < 0)
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Couldn't parse PlayAtRate=%s, assuming 1x Playback" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->playAtRateApi.pRate ));
                    speedNumerator = 1;
                    speedDenominator = 1;
                }
                hPlayer->newPlayRate = (speedNumerator * NEXUS_NORMAL_PLAY_SPEED) / speedDenominator;
                BDBG_MSG((BIP_MSG_PRE_FMT "speed: speedNumerator=%d speedDenominator=%d playRate=%d"
                          BIP_MSG_PRE_ARG, speedNumerator, speedDenominator, hPlayer->newPlayRate));
            }
            else
            {
                hPlayer->newPlayRate = hPlayer->playAtRateApi.rate;
            }
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "playRate=%d!)" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->newPlayRate ));
            hPlayer->playAtRateState.waitingForServerStatus = false;

            /* adjust the current position using this updated seek ranges. */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "truncate seek position (if needed!)" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            truncateAndUpdatePosition(hPlayer, NULL);

            if ( hPlayer->dataAvailabilityModel == BIP_PlayerDataAvailabilityModel_eLimitedRandomAccess )
            {
                BDBG_MSG((BIP_MSG_PRE_FMT "hPlayer=%p: reconfigure clock recovery if needed!" BIP_MSG_PRE_ARG, hPlayer ));
                hPlayer->playState.completionStatus = determineAndReConfigureClockRecovery(hPlayer, hPlayer->newPlayRate,
                        hPlayer->getStatusFromServerState.serverStatus.currentPositionInMs,
                        hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs);
            }

            /* Now start the trickmode related work. */
            if (hPlayer->playAtRateState.completionStatus == BIP_SUCCESS)
            {
                /* Note: we invoke PBIP PlayAtRate in the non-blocking way. It will give us a callback when it is done which runs this state machine. */
                /* We will then check the status of PlayAtRate and accordingly complete the PlayAtRate API Arb. */
                B_PlaybackIp_GetTrickModeSettings(hPlayer->pbipState.hPlaybackIp, &hPlayer->playAtRateState.pbipSettings);
                hPlayer->playAtRateState.pbipSettings.nonBlockingMode = true;
                if (
                        (hPlayer->playAtRateApi.pSettings->playRateMethod == BIP_PlayerPlayRateMethod_eAuto && hPlayer->dlnaFlags.playSpeedSupported) ||
                        (hPlayer->playAtRateApi.pSettings->playRateMethod == BIP_PlayerPlayRateMethod_eUsePlaySpeed) ||
                        (hPlayer->streamInfo.containerType == BIP_PlayerContainerType_eHls) ||
                        (hPlayer->playerProtocol == BIP_PlayerProtocol_eRtsp)
                        )
                {
                    if (hPlayer->playAtRateApi.pRate)
                    {
                        hPlayer->playAtRateState.pbipSettings.playSpeedStringDefined = true;
                        hPlayer->playAtRateState.pbipSettings.absoluteRateDefined = false;
                        hPlayer->playAtRateState.pbipSettings.playSpeedString = (char *)hPlayer->playAtRateApi.pRate;
                        BDBG_MSG(( BIP_MSG_PRE_FMT  "Using PlaySpeed Method for PlayAtRate string=%s" BIP_MSG_PRE_ARG, hPlayer->playAtRateApi.pRate ));
                    }
                    else
                    {
                        hPlayer->playAtRateState.pbipSettings.playSpeedStringDefined = false;
                        hPlayer->playAtRateState.pbipSettings.absoluteRateDefined = true;
                        hPlayer->playAtRateState.pbipSettings.absoluteRate = hPlayer->playAtRateApi.rate;
                        BDBG_MSG(( BIP_MSG_PRE_FMT  "Using PlaySpeed Method for PlayAtRate rate=%d" BIP_MSG_PRE_ARG, hPlayer->playAtRateApi.rate ));
                    }
                    hPlayer->playAtRateState.pbipSettings.method = B_PlaybackIpTrickModesMethod_UsePlaySpeed;
                }
                else
                {
                    /* Neither server hasn't indicated support for Server side trickmodes nor has app asked us to do Server side trickmodes. */
                    /* So we will do client side trickmodes. Check if app provided the rate string or absolute value. */
                    if (hPlayer->playAtRateApi.pRate)
                    {
                        if (speedDenominator > 1)
                        {
                            hPlayer->playAtRateState.pbipSettings.absoluteRate = (NEXUS_NORMAL_PLAY_SPEED*speedNumerator) / speedDenominator;
                        }
                        else
                        {
                            hPlayer->playAtRateState.pbipSettings.absoluteRate = NEXUS_NORMAL_PLAY_SPEED * speedNumerator;
                        }
                        BDBG_MSG(( BIP_MSG_PRE_FMT  "Using ByteRange Method for PlayAtRate string=%s, rate=%d" BIP_MSG_PRE_ARG, hPlayer->playAtRateApi.pRate, hPlayer->playAtRateState.pbipSettings.absoluteRate ));
                    }
                    else
                    {
                        hPlayer->playAtRateState.pbipSettings.absoluteRate = hPlayer->playAtRateApi.rate;
                        BDBG_MSG(( BIP_MSG_PRE_FMT  "Using ByteRange Method for PlayAtRate rate=%d" BIP_MSG_PRE_ARG, hPlayer->playAtRateState.pbipSettings.absoluteRate ));
                    }
                    hPlayer->playAtRateState.pbipSettings.method = B_PlaybackIpTrickModesMethod_UseByteRange;
                    hPlayer->playAtRateState.pbipSettings.absoluteRateDefined = true;
                }
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Initiate PBIP TrickMode())" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                hPlayer->playAtRateState.pbipStatus = B_PlaybackIp_TrickMode(hPlayer->pbipState.hPlaybackIp, &hPlayer->playAtRateState.pbipSettings);
                /* Note: we will check the playAtRateState.pbipStatus below. */
                /* update the local state. */
                hPlayer->playAtRateState.waitingForTrickmodeCompletion = true;
            }
            else
            {
                /* processGetServerStatusApiState failed */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "determineAndReConfigureClockRecovery() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eDone;
            }
        }
        else
        {
            /* processGetServerStatusApiState failed */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eInProgress  && waitingForServerStatus */

    if (hPlayer->playAtRateState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playAtRateState.waitingForTrickmodeCompletion)
    {
        if (hPlayer->playAtRateState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Since TrickMode() is still in progress, lets check its status. */
            hPlayer->playAtRateState.pbipStatus = B_PlaybackIp_TrickMode(hPlayer->pbipState.hPlaybackIp, &hPlayer->playAtRateState.pbipSettings);
        }
        if (hPlayer->playAtRateState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* TrickMode() is still in progress, we are done for now. PBIP will give us a callback when Seek completes. */

            hPlayer->playAtRateState.completionStatus = BIP_INF_IN_PROGRESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP TrickMode operation in progress!!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playAtRateState.pbipStatus == B_ERROR_SUCCESS)
        {
            /* PBIP TrickMode API is successful. */
            hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eDone;
            hPlayer->playAtRateState.completionStatus = BIP_SUCCESS;

            /* Update playRate to reflect the new rate */
            hPlayer->playRate = hPlayer->newPlayRate;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP TrickMode is successful, playRate=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->playRate));
        }
        else
        {
            /* Seek Failed. */
            hPlayer->playAtRateState.completionStatus = BIP_ERR_PLAYER_SEEK;
            hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eDone;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP TrickMode operation failed w/ pbipStatus=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->playAtRateState.pbipStatus ));
        }
    }

    if (hPlayer->playAtRateState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Cleanup up: cancel timers, etc.............. */
        hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eIdle;
        hPlayer->playAtRateState.waitingForServerStatus = false;
        hPlayer->playAtRateState.waitingForTrickmodeCompletion = false;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(hPlayer->playAtRateState.completionStatus) ));
    return (hPlayer->playAtRateState.completionStatus);
} /* processPlayAtRateApiSubState_locked */

static BIP_Status processPlayByFrameApiSubState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    /* First get the latest server status to make sure seek position is valid in the available seekRange */
    if (hPlayer->playByFrameState.apiState == BIP_PlayerApiState_eNew)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Update latest seek range using processGetServerStatusApiState()" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
        {
            hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->getStatusFromServerSettings.timeoutInMs = -1;
            hPlayer->getStatusFromServerSettings.getAvailableSeekRange = true;
        }
        hPlayer->playByFrameState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);

        hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eInProgress;
        hPlayer->playByFrameState.waitingForServerStatus = true;
    } /* BIP_PlayerApiState_eNew */

    if (hPlayer->playByFrameState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playByFrameState.waitingForServerStatus )
    {
        if (hPlayer->playByFrameState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            /* Poll processGetServerStatusApiState to see if it has completed. */
            hPlayer->playByFrameState.completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
        }
        if (hPlayer->playByFrameState.completionStatus == BIP_INF_IN_PROGRESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playByFrameState.completionStatus == BIP_SUCCESS)
        {
            /* processGetServerStatusApiState successfully completed. */
            hPlayer->playByFrameState.waitingForServerStatus = false;

            /* adjust the current position using this updated seek ranges. */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "truncate seek position (if needed!)" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            truncateAndUpdatePosition(hPlayer, NULL);

            /* Now start the trickmode related work. */
            {
                /* Note: we invoke PBIP PlayByFrame in the non-blocking way. It will give us a callback when it is done which runs this state machine. */
                /* We will then check the status of PlayByFrame and accordingly complete the PlayByFrame API Arb. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Initiate B_PlaybackIp_FrameAdvance: forward=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->playByFrameApi.forward ));
                hPlayer->playByFrameState.pbipStatus = B_PlaybackIp_FrameAdvance(hPlayer->pbipState.hPlaybackIp, hPlayer->playByFrameApi.forward, true /* nonBlockingMode*/);
                /* Note: we will check the playByFrameState.pbipStatus below. */
            }
            /* update the local state. */
            hPlayer->playByFrameState.waitingForTrickmodeCompletion = true;
        }
        else
        {
            /* processGetServerStatusApiState failed */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState() failed!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eDone;
        }
    } /* BIP_PlayerApiState_eInProgress  && waitingForServerStatus */

    if (hPlayer->playByFrameState.apiState == BIP_PlayerApiState_eInProgress && hPlayer->playByFrameState.waitingForTrickmodeCompletion)
    {
        if (hPlayer->playByFrameState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* Since TrickMode() is still in progress, lets check its status. */
            hPlayer->playByFrameState.pbipStatus = B_PlaybackIp_FrameAdvance(hPlayer->pbipState.hPlaybackIp, hPlayer->playByFrameApi.forward, true /* nonBlockingMode*/);
        }
        if (hPlayer->playByFrameState.pbipStatus == B_ERROR_IN_PROGRESS)
        {
            /* TrickMode() is still in progress, we are done for now. PBIP will give us a callback when Seek completes. */

            hPlayer->playByFrameState.completionStatus = BIP_INF_IN_PROGRESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP FrameAdvance operation in progress!!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else if (hPlayer->playByFrameState.pbipStatus == B_ERROR_SUCCESS)
        {
            /* Seek is successful. */
            hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eDone;
            hPlayer->playByFrameState.completionStatus = BIP_SUCCESS;
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP FrameAdvance is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
        }
        else
        {
            /* Seek Failed. */
            hPlayer->playByFrameState.completionStatus = BIP_ERR_PLAYER_SEEK;
            hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eDone;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PBIP FrameAdvance operation failed w/ pbipStatus=%d" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), hPlayer->playByFrameState.pbipStatus ));
        }
    }

    if (hPlayer->playByFrameState.apiState == BIP_PlayerApiState_eDone)
    {
        /* Cleanup up: cancel timers, etc.............. */
        hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eIdle;
        hPlayer->playByFrameState.waitingForServerStatus = false;
        hPlayer->playByFrameState.waitingForTrickmodeCompletion = false;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(hPlayer->playByFrameState.completionStatus) ));
    return (hPlayer->playByFrameState.completionStatus);
} /* processPlayByFrameApiSubState_locked */

static BIP_Status processStopApiState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    B_Error    rc;
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    if (hPlayer->stopState.apiState == BIP_PlayerApiState_eNew)
    {
        /* All Stop tasks are fast, so they are done inline in the state machine in the done state below. */
        hPlayer->stopState.apiState = BIP_PlayerApiState_eDone;
    }

    if (hPlayer->stopState.apiState == BIP_PlayerApiState_eDone)
    {
        /*
         * Player needs to Stop both PlaybackIp & Nexus Playback or Playpump. The order should be the opposite of how they are started.
         * - When Playpump is being used, we stop PBIP first & then the Playpump as PBIP is driving the feeding of AV stream into the Playpump.
         * - When Playback is being used, we stop Playback first & then PBIP as Playback is driving the consuming of AV stream by reading it from PBIP bfile i/f.
         */
        if (!hPlayer->useNexusPlaypump && hPlayer->hPlayback)
        {
            /* Using Playback, so stop it first. */
            NEXUS_StopCallbacks(hPlayer->hPlayback);
            NEXUS_Playback_Stop(hPlayer->hPlayback);
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p Nexus Playback hPlayback=%p Stopped!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlayback ));
        }

        /* Now Stop PBIP */
        rc = B_PlaybackIp_SessionStop(hPlayer->pbipState.hPlaybackIp);
        if (rc == B_ERROR_SUCCESS)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p: B_PlaybackIp_SessionStop() Success" BIP_MSG_PRE_ARG, hPlayer ));
        }
        else
        {
            /* SessionStop failed.. */
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer=%p: B_PlaybackIp_SessionStop() Failed: rc=%d" BIP_MSG_PRE_ARG, hPlayer, rc ));
        }
        /* Note: we are ignore the completionStatus from PBIP SessionStop API above as BIP Player's Stop doesn't really care about it. */

        if (hPlayer->useNexusPlaypump && hPlayer->hPlaypump)
        {
            /* Using Nexus Playpump, so Stop it. */
            NEXUS_Playpump_Stop(hPlayer->hPlaypump);
            if (hPlayer->usePlaypump2ForAudio && hPlayer->hPlaypump2) NEXUS_Playpump_Stop(hPlayer->hPlaypump2);
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer=%p hPlaypump=%p Stopped!" BIP_MSG_PRE_ARG, hPlayer, hPlayer->hPlaypump ));
        }

        hPlayer->stopState.apiState = BIP_PlayerApiState_eIdle;
        completionStatus = BIP_SUCCESS;
    }

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    return (completionStatus);
} /* processStopApiState_locked */

static BIP_Status processStartedPausedOrAbortedState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status      completionStatus = BIP_INF_IN_PROGRESS;
    BIP_ArbHandle   hArb = NULL;
    bool            reRunState = true;

    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player );
    BSTD_UNUSED( threadOrigin );

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Entering.." BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    while (reRunState)
    {
        reRunState = false;
        hArb = NULL;

        /* Very first thing we check is to see if App wants to Stop the Player. */
        if (
                ( BIP_Arb_IsNew(hPlayer->stopApi.hArb) ) ||                                             /* App has issued a Stop() API. Or, */
                ( hPlayer->disconnecting && hPlayer->stopState.apiState == BIP_PlayerApiState_eIdle) )  /* App has issued Disconnect() or Destroy() APIs & we haven't yet started StopApi processing! */
        {
            /*
             * For control to come here, processPlayerState() has already verified that Stop() (if invoked) is being called in the valid main states (Started, Paused, Aborted).
             * Disconnect/Destroy() can be called in any Player State.
             * However, we may be in the middle of processing another API (such as SeekAsync, etc. ) when app called Stop()/Disconnect()/Destroy().
             * We will process Stop() irrespective of player's subState and state machine should handle a completion callback for currently pending API that may come after Stop completion.
             * This is to allow a better channel change time.
            */
            if ( BIP_Arb_IsNew(hArb = hPlayer->stopApi.hArb) )
            {
                BIP_Arb_AcceptRequest(hArb);
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_Stop() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
            else
            {
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Starting Stop processing during Disconnect()/Destroy() API calls!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }

            hPlayer->stopState.apiState = BIP_PlayerApiState_eNew;
            hPlayer->subState = (hPlayer->subState == BIP_PlayerSubState_ePaused) ? BIP_PlayerSubState_ePausedWaitForStopCompletion : BIP_PlayerSubState_eStartedWaitForStopCompletion;
        } /* StopApi */

        /* Then, process API Arbs in the steady/completed states (such as PlayingNormal, PlayingTrickmode, Paused, etc.) */
        if (    hPlayer->subState == BIP_PlayerSubState_eStartedPlayingNormal ||
                hPlayer->subState == BIP_PlayerSubState_eStartedPlayingTrickmode ||
                hPlayer->subState == BIP_PlayerSubState_ePaused ||
                hPlayer->subState == BIP_PlayerSubState_eAborted
           )
        {
            /* Note: we check for endOfStream or networkError events from PBIP only in these completed states & not while another API is currently in progress. */
            /* The subState logic below re-runs this state machine to catch these events if they had come while state machine was in the middle of procesing an API. */
            if (hPlayer->reachedEndOfStream == true || hPlayer->receivedNetworkError == true)
            {
                /* We have received either EndOfStream or NetworkError Events from PBIP. We only inform app about it if it hasn't yet called Stop. */
                if (hPlayer->reachedEndOfStream && hPlayer->endOfStreamCallback.callback)
                {
                    BIP_Arb_AddDeferredCallback( hPlayer->playApi.hArb, (BIP_CallbackDesc *) &hPlayer->endOfStreamCallback); /* Note: BIP_Callback & NEXUS_Callback structures have same definition! */
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Processed EndOfStream condition: app callback is queued!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                if (hPlayer->receivedNetworkError && hPlayer->errorCallback.callback)
                {
                    BIP_Arb_AddDeferredCallback( hPlayer->playApi.hArb, (BIP_CallbackDesc *) &hPlayer->errorCallback); /* Note: BIP_Callback & NEXUS_Callback structures have same definition! */
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Processed Network Error condition: app callback is queued!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                hPlayer->subState = BIP_PlayerSubState_ePaused;
                hPlayer->state = BIP_PlayerState_ePaused;
                hPlayer->reachedEndOfStream = false;

                /* If App wants us to start Playing from the live point, then we trigger the play related work. */
                if (hPlayer->playerSettings.playbackSettings.endOfStreamAction == NEXUS_PlaybackLoopMode_ePlay)
                {
                    hPlayer->playState.apiState = BIP_PlayerApiState_eNew;
                    /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                    hPlayer->playState.prevPlayerSubState = hPlayer->subState;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "EndOfStream acton is to Play from the live point, starting that work! " BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                    hPlayer->subState = BIP_PlayerSubState_ePausedWaitForPlayCompletion;
                }
                /* Note: we dont break from this state machine here. Instead, we will process any New API Arbs from the App. */
                /* App may have issued a Rewind/Seek operation right at the same time player had gotten endOfStream event from PBIP. */
                /* Alternatively, we can fail all the APIs after endOfStream processing here is done, but this doesn't seem like a good choice!. */
            } /* reachedEndOfStream || receivedNetworkError */

            if (hPlayer->reachedBeginningOfStream == true)
            {
                /* We have received beginning of the stream, check what action app wants us to take. */
                hPlayer->reachedBeginningOfStream = false;

                /* First we move to the paused state as PBIP has played out all content till the start & is waiting for Player to action an action. */
                hPlayer->subState = BIP_PlayerSubState_ePaused;
                hPlayer->state = BIP_PlayerState_ePaused;

                /* If App wants us to start Playing from the start, then we trigger the play related work. */
                if (hPlayer->playerSettings.playbackSettings.beginningOfStreamAction == NEXUS_PlaybackLoopMode_ePlay)
                {
                    hPlayer->playState.apiState = BIP_PlayerApiState_eNew;
                    /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                    hPlayer->playState.prevPlayerSubState = hPlayer->subState;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BeginningOfStream acton is to Play the stream, starting that work!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                    hPlayer->subState = BIP_PlayerSubState_ePausedWaitForPlayCompletion;
                }
                if (hPlayer->beginningOfStreamCallback.callback)
                {
                    BIP_Arb_AddDeferredCallback( hPlayer->playApi.hArb, (BIP_CallbackDesc *) &hPlayer->beginningOfStreamCallback); /* Note: BIP_Callback & NEXUS_Callback structures have same definition! */
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Processed BeginningOfStream condition: app callback is queued!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
            } /* reachedBeginningOfStream */

            if (hPlayer->subState == BIP_PlayerSubState_ePaused && hasPauseTimerExpired(hPlayer, hPlayer->pauseState.pauseTimeoutInMs) == true)
            {
                /* Pause timeout timer has expired, resume Playing as that is the only reason we start the pause timer for. */
                hPlayer->playState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->playState.prevPlayerSubState = hPlayer->subState;
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Pause timeout expired so Play the stream, starting that work!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                hPlayer->subState = BIP_PlayerSubState_ePausedWaitForPlayCompletion;
            }

            /* TODO: Get the next Arb from the Arb list, this allows app to call another Player API while previous one is in progress. */
            /* E.g. App may call _SeekAsync(), and then _Stop() before seek completes (due to channel change) or call _PlayAtRate().  */
            /* Note: 2nd invocation of same API (e.g. calling SeekAsync() while previous SyncAsync() is in progress will return an error on 2nd one. */
            /* We dont current see a need for this, but this can also be supported by using Dynamic ARBs. */

            if (BIP_Arb_IsNew(hArb = hPlayer->pauseApi.hArb))
            {
                /* Initiate the New Pause operation. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_Pause() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                BIP_Arb_AcceptRequest(hArb);
                hPlayer->pauseState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->pauseState.prevPlayerSubState = hPlayer->subState;
                hPlayer->subState = BIP_PlayerSubState_eStartedWaitForPauseCompletion;
            } /* pauseApi */

            if (BIP_Arb_IsNew(hArb = hPlayer->playApi.hArb))
            {
                /* Initiate the New Play operation. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_Play() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                BIP_Arb_AcceptRequest(hArb);

                hPlayer->playState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->playState.prevPlayerSubState = hPlayer->subState;
                hPlayer->subState = hPlayer->subState == BIP_PlayerSubState_ePaused ? BIP_PlayerSubState_ePausedWaitForPlayCompletion : BIP_PlayerSubState_eStartedWaitForPlayCompletion;
            } /* playApi */

            if (BIP_Arb_IsNew(hArb = hPlayer->seekApi.hArb))
            {
                /* Initiate the New Seek operation. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_Seek() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                BIP_Arb_AcceptRequest(hArb);

                hPlayer->seekState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->seekState.prevPlayerSubState = hPlayer->subState;
                hPlayer->subState = hPlayer->subState == BIP_PlayerSubState_ePaused ? BIP_PlayerSubState_ePausedWaitForSeekCompletion : BIP_PlayerSubState_eStartedWaitForSeekCompletion;
            } /* seekApi */

            if (BIP_Arb_IsNew(hArb = hPlayer->playAtRateApi.hArb))
            {
                /* Initiate the New PlayAtRate operation. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_PlayAtRate() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                BIP_Arb_AcceptRequest(hArb);

                hPlayer->playAtRateState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->playAtRateState.prevPlayerSubState = hPlayer->subState;
                hPlayer->subState = hPlayer->subState == BIP_PlayerSubState_ePaused ? BIP_PlayerSubState_ePausedWaitForPlayAtRateCompletion : BIP_PlayerSubState_eStartedWaitForPlayAtRateCompletion;
            } /* playAtRate */

            if (BIP_Arb_IsNew(hArb = hPlayer->playByFrameApi.hArb))
            {
                /* Initiate the New PlayByFrame operation. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted BIP_Player_PlayByFrame() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                BIP_Arb_AcceptRequest(hArb);

                hPlayer->playByFrameState.apiState = BIP_PlayerApiState_eNew;
                /* Save current subState as we may need to revert to it if we get an error while processing this API. */
                hPlayer->playByFrameState.prevPlayerSubState = hPlayer->subState;
                hPlayer->subState = BIP_PlayerSubState_ePausedWaitForPlayByFrameCompletion;
            } /* playByFrame */

        } /* subState == PlayingNormal || PlayingTrickmode */

        /* Stop PauseTimeout timer if we are no longer in the Paused state. */
        if (hPlayer->subState != BIP_PlayerSubState_ePaused && hPlayer->pauseState.pauseTimerActive)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Stopping Pause Timer due to no longer being in Paused state!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            stopPauseTimer(hPlayer);
        }

        if (hPlayer->subState == BIP_PlayerSubState_ePausedWaitForStopCompletion || hPlayer->subState == BIP_PlayerSubState_eStartedWaitForStopCompletion)
        {
            /* Call StopApi state function & process based on its returned status. */
            completionStatus = processStopApiState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                if (completionStatus == BIP_SUCCESS)
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player is Stopped!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* Stop Failed. */
                    BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Failed to Stop the Player, continuing!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                /* Irrespective of Stop results, Player transitions to the Prepared State. */
                hPlayer->state      = BIP_PlayerState_ePrepared;
                hPlayer->subState   = BIP_PlayerSubState_eIdle;

                /* And finally complete the Stop Arb, so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->stopApi.hArb))
                {
                    BIP_Arb_CompleteRequest( hPlayer->stopApi.hArb, completionStatus );
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Stop() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                }
                else
                {
                    BDBG_ASSERT(hPlayer->disconnecting);    /* App must have called Disconnect/Destroy() in the Started/Paused/Aborted states. */
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Internal Stop processing is completed (for Disconnect/Destroy() cases) with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                    /* We will set a flag to run the main processPlayerState() again, so that player can continue to un-wind during this disconnecting state. */
                    hPlayer->reRunState = true;
                }
                continue; /* Done w/ Stop() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* Stop is still in progress, nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player is still not Stopped!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
        }

        if (hPlayer->subState == BIP_PlayerSubState_eStartedWaitForPauseCompletion)
        {
            completionStatus = processPauseApiState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                if (completionStatus == BIP_SUCCESS)
                {
                    /* Player is now successfuly Paused: we switch from Started to Paused State. */
                    hPlayer->state = BIP_PlayerState_ePaused;

                    /* SubState is Paused as Player is not doing anything but waiting in Paused mode. */
                    hPlayer->subState = BIP_PlayerSubState_ePaused;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player is Paused!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else if (completionStatus == BIP_INF_PLAYER_CANT_PAUSE)
                {
                    /* Player can't currently pause (most likely due to pause position == seek.begin). */
                    /* so we remain in the same main state. */

                    /* And we restore the previus sub-state. */
                    hPlayer->subState = hPlayer->pauseState.prevPlayerSubState;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Can't Pause at this time, ignore it & stay in same states!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* Pause Failed. */
                    handleApiErrorsAndUpdatePlayerState(
                            hPlayer, "BIP_Player_Pause",
                            hPlayer->playerSettings.playbackSettings.playErrorHandling,
                            hPlayer->state, hPlayer->pauseState.prevPlayerSubState,
                            &hPlayer->state, &hPlayer->subState);
                }

                /* And finally complete the Pause Arb (if its busy), so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->pauseApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->pauseApi.hArb, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Pause() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                continue; /* Done w/ Pause() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* Pause is still in progress, nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player is still not Paused!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
        }

        if (hPlayer->subState == BIP_PlayerSubState_eStartedWaitForPlayCompletion || hPlayer->subState == BIP_PlayerSubState_ePausedWaitForPlayCompletion )
        {
            completionStatus = processPlayApiState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                if (completionStatus == BIP_SUCCESS)
                {
                    /* Play() is successful: update the main & subStates. */
                    hPlayer->state = BIP_PlayerState_eStarted;
                    hPlayer->subState = BIP_PlayerSubState_eStartedPlayingNormal;
                    completionStatus = BIP_SUCCESS;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player is Playing!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* Play Failed. */
                    handleApiErrorsAndUpdatePlayerState(
                            hPlayer, "BIP_Player_Play",
                            hPlayer->playerSettings.playbackSettings.playErrorHandling,
                            hPlayer->state, hPlayer->playState.prevPlayerSubState,
                            &hPlayer->state, &hPlayer->subState);
                }

                /* And finally complete the Play Arb (if its busy), so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->playApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->playApi.hArb, completionStatus );
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Play() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                continue; /* Done w/ Play() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* Play is still in progress, nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Play API is still in progress!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            }
        }

        if (hPlayer->subState == BIP_PlayerSubState_eStartedWaitForSeekCompletion || hPlayer->subState == BIP_PlayerSubState_ePausedWaitForSeekCompletion )
        {
            completionStatus = processSeekApiSubState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                /* Seek is completed, check its completion status. */
                if (completionStatus == BIP_SUCCESS)
                {
                    /* Seek is successful, do the state transitions. Main player state stays the same (either Started or Paused). */

                    /* Seek doesn't affect the subState, so we change it back to the one when Seek() was invoked. */
                    hPlayer->subState = hPlayer->seekState.prevPlayerSubState;
                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Seek is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* Seek Failed, call this function to handle the error & do the right thing! */
                    handleApiErrorsAndUpdatePlayerState(
                            hPlayer, "BIP_Player_Seek",
                            hPlayer->playerSettings.playbackSettings.seekErrorHandling,
                            hPlayer->state, hPlayer->seekState.prevPlayerSubState,
                            &hPlayer->state, &hPlayer->subState);
                }
                /* And finally complete the Pause Arb (if its busy), so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->seekApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->seekApi.hArb, completionStatus );

                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_Seek() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                continue; /* Done w/ Seek() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* Seek is still in progress, so there is nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player still has not Seeked to new position!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                /* TODO: To support noWait version of APIs, we will complete the ARB here w/ Success. But seekState will remain in the Waiting subState until it is internally finished. */
            }
        }

        if (hPlayer->subState == BIP_PlayerSubState_eStartedWaitForPlayAtRateCompletion || hPlayer->subState == BIP_PlayerSubState_ePausedWaitForPlayAtRateCompletion )
        {
            completionStatus = processPlayAtRateApiSubState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                /* PlayAtRate is completed, check its completion status. */
                if (completionStatus == BIP_SUCCESS)
                {
                    /* PlayAtRate is successful, do the state transitions. */
                    /* Main player state changes to Started. */
                    hPlayer->state = BIP_PlayerState_eStarted;

                    /* subState reflects that we are doing trickPlay. */
                    hPlayer->subState = BIP_PlayerSubState_eStartedPlayingTrickmode;

                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PlayAtRate is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* PlayAtRate Failed, call this function to handle the error & do the right thing! */
                    handleApiErrorsAndUpdatePlayerState(
                            hPlayer, "BIP_Player_PlayAtRate",
                            hPlayer->playerSettings.playbackSettings.trickErrorHandling,
                            hPlayer->state, hPlayer->playAtRateState.prevPlayerSubState,
                            &hPlayer->state, &hPlayer->subState);
                }
                /* And finally complete the Pause Arb (if its busy), so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->playAtRateApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->playAtRateApi.hArb, completionStatus );

                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_PlayAtRate() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                continue; /* Done w/ PlayAtRate() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* PlayAtRate is still in progress, so there is nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player still has not PlayAtRateed to new position!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                /* TODO: To support noWait version of APIs, we will complete the ARB here w/ Success. But seekState will remain in the Waiting subState until it is internally finished. */
            }
        }

        if (hPlayer->subState == BIP_PlayerSubState_ePausedWaitForPlayByFrameCompletion )
        {
            completionStatus = processPlayByFrameApiSubState_locked(hPlayer, threadOrigin);
            if (completionStatus != BIP_INF_IN_PROGRESS)
            {
                /* PlayByFrame is completed, check its completion status. */
                if (completionStatus == BIP_SUCCESS)
                {
                    /* PlayByFrame is successful, do the state transitions. */
                    BDBG_ASSERT(hPlayer->state == BIP_PlayerState_ePaused);
                    hPlayer->subState = BIP_PlayerSubState_ePaused; /* back to paused subState. */

                    BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PlayByFrame is successful!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                }
                else
                {
                    /* PlayByFrame Failed, call this function to handle the error & do the right thing! */
                    handleApiErrorsAndUpdatePlayerState(
                            hPlayer, "BIP_Player_PlayByFrame",
                            hPlayer->playerSettings.playbackSettings.trickErrorHandling,
                            hPlayer->state, hPlayer->playByFrameState.prevPlayerSubState,
                            &hPlayer->state, &hPlayer->subState);
                }
                /* And finally complete the Pause Arb (if its busy), so that caller can be notified of its completion. */
                if (BIP_Arb_IsBusy(hPlayer->playByFrameApi.hArb)) BIP_Arb_CompleteRequest( hPlayer->playByFrameApi.hArb, completionStatus );

                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_PlayByFrame() completed with completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
                continue; /* Done w/ PlayByFrame() processing but re-run this state machine to process either Arbs or events (EOS/NetworkError) that may have come while we were still processing this subState. */
            }
            else /* completionStatus == BIP_INF_IN_PROGRESS */
            {
                /* PlayByFrame is still in progress, so there is nothing to do & we will exit the state machine below. */
                BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Player still has not PlayByFrameed to new position!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
                /* TODO: To support noWait version of APIs, we will complete the ARB here w/ Success. But seekState will remain in the Waiting subState until it is internally finished. */
            }
        }

        /* Check & queue any APIs that may have been called while another API is in progress. */
        completionStatus = enqueApiForLaterProcessing(hPlayer);
        break;  /* As we have gone thru the whole state machine once to give various subStates a chance to process & handle any events. */
    } /* while reRunState */

    BDBG_MSG(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "Returning: completionStatus=%s" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    return (completionStatus);
} /* processStartedPausedOrAbortedState_locked */

static BIP_Status processPlayerState_locked(
    BIP_PlayerHandle        hPlayer,
    BIP_Arb_ThreadOrigin    threadOrigin
    )
{
    BIP_Status completionStatus = BIP_INF_IN_PROGRESS;
    BSTD_UNUSED( threadOrigin );

    /* We call GetServerStatus irrespective */
    completionStatus = processGetServerStatusApiState_locked(hPlayer, threadOrigin);
    if (completionStatus != BIP_SUCCESS && completionStatus != BIP_INF_IN_PROGRESS)
    {
        BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "processGetServerStatusApiState_locked(0 Failed w/ status=%s, but continuing!" BIP_MSG_PRE_ARG,
                    BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    }

    do
    {
        hPlayer->reRunState = false;

        switch ( hPlayer->state )
        {
            case BIP_PlayerState_eDisconnected:
                completionStatus = processDisconnectedState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_eConnecting:
                completionStatus = processConnectingState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_eConnected:
                completionStatus = processConnectedState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_eProbing:
                completionStatus = processProbingState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_ePreparing:
                completionStatus = processPreparingState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_ePrepared:
                completionStatus = processPreparedState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_eStarting:
                completionStatus = processStartingState_locked( hPlayer, threadOrigin );
                break;
            case BIP_PlayerState_eStarted:
            case BIP_PlayerState_ePaused:
            case BIP_PlayerState_eAborted:
                completionStatus = processStartedPausedOrAbortedState_locked( hPlayer, threadOrigin );
                break;
            default:
                break;
        }
    } while (hPlayer->reRunState == true);

    return (completionStatus);
} /* processPlayerState_locked */

void processPlayerState(
        void *hObject,
        int value,
        BIP_Arb_ThreadOrigin threadOrigin
        )
{
    BIP_PlayerHandle        hPlayer = hObject;    /* Player object handle */
    BIP_ArbHandle           hArb;
    BIP_Status              completionStatus = BIP_INF_IN_PROGRESS;
    bool                    muteDebugPrint = false;

    BSTD_UNUSED(value);
    BDBG_ASSERT(hPlayer);
    BDBG_OBJECT_ASSERT( hPlayer, BIP_Player);

    B_Mutex_Lock( hPlayer->hStateMutex );
    if (BIP_Arb_IsNew(hPlayer->getStatusApi.hArb) == false && BIP_Arb_IsNew(hPlayer->getStatusFromServerApi.hArb) == false)
    {
        /* Since GetStatus() can be called quite often to get the player timeline, ignore its print. */
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "ENTER --------------------> " BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    }
    else
    {
        /* GetStatus() API, so mute its prints. */
        muteDebugPrint = true;
    }
    BIP_MSG_TRC(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "ENTER --------------------> " BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

    /* Check if we have a new _Connect() API called from App. */
    if (BIP_Arb_IsNew(hArb = hPlayer->connectApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eDisconnected)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Connect() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _Connect() in eDisconnected state, start its processing. */
            hPlayer->state = BIP_PlayerState_eConnecting;
            hPlayer->subState = BIP_PlayerSubState_eConnectingNew;
            BIP_Arb_AcceptRequest(hArb);
            /* Rest of processing happens below via the processConnectingState_locked(). */
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->probeMediaInfoApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eConnected)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_ProbeMediaInfo() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _ProbeMediaInfo() in eConnected state, start its processing. */
            hPlayer->state = BIP_PlayerState_eProbing;
            hPlayer->subState = BIP_PlayerSubState_eProbingNew;
            BIP_Arb_AcceptRequest(hArb);
            /* Rest of processing happens below via the processProbingState_locked(). */
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->getProbedStreamInfo.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eConnected)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_GetProbedStreamInfo() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mediaProbeComplete == false)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: StreamInfo is not yet available: either call BIP_Player_ProbeStreamInfo() or BIP_Player_Prepare() before calling this API, currentState=%s"
                       BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* Return the streamInfo. */
            BIP_Arb_AcceptRequest(hArb);
            getProbedStreamInfo(hPlayer, hPlayer->getProbedStreamInfo.pStreamInfo);
            BIP_Arb_CompleteRequest(hArb, BIP_SUCCESS);
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->prepareApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eConnected)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Prepare() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _prepare() in eConnected state, start its processing. */
            hPlayer->state = BIP_PlayerState_ePreparing;
            hPlayer->subState = BIP_PlayerSubState_ePreparingNew;
            BIP_Arb_AcceptRequest(hArb);
            /* Rest of processing happens below via the processPreparingState_locked(). */
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->openPidChannelApi.hArb))
    {
        if (hPlayer->state < BIP_PlayerState_ePrepared)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_OpenPidChannel() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _OpenPidChannel() after ePrepared state, open a pid channel & return it. */
            BIP_Arb_AcceptRequest(hArb);
            completionStatus = openAndAddPidChannelToTrackList(hPlayer, hPlayer->openPidChannelApi.trackId, hPlayer->openPidChannelApi.pSettings, hPlayer->openPidChannelApi.phPidChannel);
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->closePidChannelApi.hArb))
    {
        BIP_Arb_AcceptRequest(hArb);
        completionStatus = closeAndRemovePidChannelFromTrackList(hPlayer, hPlayer->closePidChannelApi.hPidChannel);
        BIP_Arb_CompleteRequest(hArb, completionStatus);
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->closeAllPidChannelsApi.hArb))
    {
        BIP_Arb_AcceptRequest(hArb);
        closeAndRemoveAllPidChannelsFromTrackList(hPlayer);
        BIP_Arb_CompleteRequest(hArb, BIP_SUCCESS);
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->startApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_ePrepared)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Start() is not allowed in this state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _start() in ePrepared state, start its processing. */
            hPlayer->state = BIP_PlayerState_eStarting;
            hPlayer->subState = BIP_PlayerSubState_eStartingNew;
            BIP_Arb_AcceptRequest(hArb);
            /* Rest of processing happens below via the processStartingState_locked(). */
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->pauseApi.hArb))
    {
        if (hPlayer->state == BIP_PlayerState_ePaused)
        {
            BIP_Arb_AcceptRequest(hArb);
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Ignoring BIP_Player_Pause(): current player state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_SUCCESS;
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
        else if (hPlayer->state != BIP_PlayerState_eStarted)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Pause() is only allowed after BIP_Player_Start(), current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (!BIP_Player_TrickmodeSupported(hPlayer))
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Pause not supported on this data availability model=%s!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mode == BIP_PlayerMode_eRecord)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Pause() is not allowed in the Recording Mode: current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _Pause() in eStarted state, its processing is called out via the processStartedPausedOrAbortedState_locked() call below. */
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->playApi.hArb))
    {
        if (hPlayer->state == BIP_PlayerState_eStarted && hPlayer->subState == BIP_PlayerSubState_eStartedPlayingNormal)
        {
            BIP_Arb_AcceptRequest(hArb);
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: Ignoring BIP_Player_Play() in Normal Playing state!" BIP_MSG_PRE_ARG, hPlayer ));
            completionStatus = BIP_SUCCESS;
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
        else if (hPlayer->state != BIP_PlayerState_eStarted && hPlayer->state != BIP_PlayerState_ePaused)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Play() is not allowed in the current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (!BIP_Player_TrickmodeSupported(hPlayer))
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Play not supported on this data availability model=%s!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mode == BIP_PlayerMode_eRecord)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Play() is not allowed in the Recording Mode: current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _Play() in either eStarted or ePaused state, its processing is happens below in the switch where we call the process function of the current state. */
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->playAtRateApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eStarted && hPlayer->state != BIP_PlayerState_ePaused)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_PlayAtRate() is not allowed in the current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (!BIP_Player_TrickmodeSupported(hPlayer))
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PlayAtRate not supported on this data availability model=%s!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->playAtRateApi.pSettings->playRateMethod == BIP_PlayerPlayRateMethod_eUseByteRange && hPlayer->useNexusPlaypump)
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PlayAtRate BIP_PlayerPlayRateMethod_eUseByteRange not supported when Nexus Playpump is being used for Playing AV stream!"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->playAtRateApi.pSettings->playRateMethod == BIP_PlayerPlayRateMethod_eUsePlaySpeed && !hPlayer->useNexusPlaypump)
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "PlayAtRate BIP_PlayerPlayRateMethod_eUsePlaySpeed not supported when Nexus Playpump is not being used for Playing AV stream!"
                        BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mode == BIP_PlayerMode_eRecord)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_PlayAtRate() is not allowed in the Recording Mode: current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _PlayAtRate() in either eStarted or ePaused state, its processing is happens below in the switch where we call the process function of the current state. */
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->playByFrameApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_ePaused)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_PlayAtRate() is not allowed in the current state=%s, App must call BIP_Player_Pause() to Pause Player before calling this API!"
                       BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (!BIP_Player_TrickmodeSupported(hPlayer))
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "BIP_Player_PlayByFrame not supported on this data availability model=%s!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mode == BIP_PlayerMode_eRecord)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_PlayByFrame() is not allowed in the Recording Mode: current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _PlayByFrame() in ePaused state, its processing is happens below in the switch where we call the process function of the current state. */
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->seekApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eStarted && hPlayer->state != BIP_PlayerState_ePaused)
        {
            BDBG_ERR(( BIP_MSG_PRE_FMT "hPlayer %p: Can't call BIP_Player_Seek() unless Player is in Started or Paused states, current player state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (!BIP_Player_TrickmodeSupported(hPlayer))
        {
            completionStatus = BIP_INF_NOT_AVAILABLE;
            BDBG_WRN(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Seek not supported on this data availability model=%s!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer),
                        BIP_ToStr_BIP_PlayerDataAvailabilityModel(hPlayer->dataAvailabilityModel) ));
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else if (hPlayer->mode == BIP_PlayerMode_eRecord)
        {
            BDBG_WRN(( BIP_MSG_PRE_FMT "hPlayer %p: BIP_Player_Seek() is not allowed in the Recording Mode: current state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_RejectRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _Seek() in eStarted or ePaused state, these states below will handle it. */
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->stopApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eStarted && hPlayer->state != BIP_PlayerState_ePaused && hPlayer->state != BIP_PlayerState_eAborted)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Player is neither started, paused, not aborted, ignoring BIP_Player_Stop(), Player state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            BIP_Arb_AcceptRequest(hArb);
            completionStatus = BIP_SUCCESS;
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
        else
        {
            /* App called _stop() in eStarted or ePaused state, these states below will handle it. */
            BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted _Stop() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
            /* Rest of processing happens below via the processStartedPausedOrAbortedState_locked() */
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->disconnectApi.hArb))
    {
        /* Disconnect() is accepted in all states! */
        BIP_Arb_AcceptRequest(hArb);
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "Accepted Disconnect() Arb!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));

        if (hPlayer->state == BIP_PlayerState_eDisconnected)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Ignoring BIP_Player_Disconnect() in state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            completionStatus = BIP_SUCCESS;
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
        else
        {
            /* For all other states, we will first let the player run thru its current state, complete it (including pending Arb), and then do the remaining Disconnect() related processing. */
            hPlayer->disconnecting = true;
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->destroyApi.hArb))
    {
        BIP_Arb_AcceptRequest(hArb);
        completionStatus = BIP_INF_IN_PROGRESS;

        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Accepted _Destroy Arb: state %s!" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));

        /* Stop & Disconnect if needed. */
        if (hPlayer->state == BIP_PlayerState_eDisconnected)
        {
            /* Player is already in eDisconnected state, so there is nothing to do! Just complete the Arb. */
            completionStatus = BIP_SUCCESS;
            BIP_Arb_CompleteRequest( hArb, completionStatus );
        }
        else
        {
            /* For all other states, we will first let the player run thru its current state, complete it (including pending Arb), and then do the remaining Destroy() related processing. */
            hPlayer->disconnecting = true;
            completionStatus = BIP_INF_IN_PROGRESS;
        }
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->setSettingsApi.hArb))
    {
        BIP_Arb_AcceptRequest(hArb);
        completionStatus = setSettings(hPlayer, hPlayer->setSettingsApi.pSettings);
        BIP_Arb_CompleteRequest( hArb, completionStatus);
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "SetSettings() done!" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer) ));
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->getSettingsApi.hArb))
    {
        /* App is requesting current Player settings. */
        BIP_Arb_AcceptRequest(hArb);

        /* Return the current cached settings. */
        *hPlayer->getSettingsApi.pSettings  = hPlayer->playerSettings;

        /* We are done this API Arb, so set its completion status. */
        completionStatus = BIP_SUCCESS;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: GetSettings Arb request is complete: state %s!" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
        BIP_Arb_CompleteRequest( hArb, completionStatus);
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->getStatusApi.hArb))
    {
        /* App is requesting current Player settings. */
        BIP_Arb_AcceptRequest(hArb);

        /* Return the current status. */
        BKNI_Memset(hPlayer->getStatusApi.pStatus, 0, sizeof(*hPlayer->getStatusApi.pStatus));
        hPlayer->getStatusApi.pStatus->stats = hPlayer->stats;
        hPlayer->getStatusApi.pStatus->state = hPlayer->state;
        hPlayer->getStatusApi.pStatus->subState = hPlayer->subState;
        hPlayer->getStatusApi.pStatus->clockRecoveryMode = hPlayer->clockRecoveryMode;
        hPlayer->getStatusApi.pStatus->dataAvailabilityModel = hPlayer->dataAvailabilityModel;
        hPlayer->getStatusApi.pStatus->prepareStatus.hAudioPidChannel = hPlayer->hPrimaryAudioPidChannel;
        hPlayer->getStatusApi.pStatus->prepareStatus.hVideoPidChannel = hPlayer->hVideoPidChannel;
        hPlayer->getStatusApi.pStatus->prepareStatus.hMediaInfo = hPlayer->hMediaInfo;
        hPlayer->getStatusApi.pStatus->prepareStatus.hExtraVideoPidChannel = NULL;
        hPlayer->getStatusApi.pStatus->mode = hPlayer->mode;
        if (hPlayer->mode != BIP_PlayerMode_eRecord)
        {
            B_PlaybackIpError brc;
            B_PlaybackIpStatus pbipStatus;

            brc = B_PlaybackIp_GetStatus(hPlayer->pbipState.hPlaybackIp, &pbipStatus);
            if (brc == B_ERROR_SUCCESS)
            {
                hPlayer->getStatusApi.pStatus->currentPositionInMs = pbipStatus.position;
                if ( hPlayer->dlnaFlags.availableSeekRangeSupported )
                {
                    /* Timeshifting case, first & last are set to the seekable window. */
                    hPlayer->getStatusApi.pStatus->lastPositionInMs  = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.lastPositionInMs;
                    hPlayer->getStatusApi.pStatus->firstPositionInMs = hPlayer->getStatusFromServerState.serverStatus.availableSeekRange.firstPositionInMs;
                }
                else
                {
                    hPlayer->getStatusApi.pStatus->firstPositionInMs = 0;
                    hPlayer->getStatusApi.pStatus->lastPositionInMs = pbipStatus.last;
                }
                completionStatus = BIP_SUCCESS;
            }
            else
            {
                completionStatus = BIP_ERR_PLAYER_PBIP;
            }
        }
        else
        {
            hPlayer->getStatusApi.pStatus->currentPositionInMs = 0;
            hPlayer->getStatusApi.pStatus->firstPositionInMs = 0;
            hPlayer->getStatusApi.pStatus->lastPositionInMs = 0;
            completionStatus = BIP_SUCCESS;
        }
        BIP_MSG_TRC(( BIP_MSG_PRE_FMT "hPlayer %p: GetStatus Arb request is complete: state=%s clockRecoveryMode=%s pos=%d status=%s"
                    BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state), BIP_ToStr_BIP_PlayerClockRecoveryMode(hPlayer->clockRecoveryMode),
                    hPlayer->getStatusApi.pStatus->currentPositionInMs, BIP_StatusGetText(completionStatus) ));
        BIP_Arb_CompleteRequest( hArb, completionStatus);
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->printStatusApi.hArb))
    {
        /* App is requesting to print Player status. */
        BIP_Arb_AcceptRequest(hArb);
        playerPrintStatus(hPlayer);
        completionStatus = BIP_SUCCESS;
        BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: PrintStatus Arb request is complete: state %s!" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
        BIP_Arb_CompleteRequest( hArb, completionStatus);
    }
    else if (BIP_Arb_IsNew(hArb = hPlayer->getStatusFromServerApi.hArb))
    {
        if (hPlayer->state != BIP_PlayerState_eStarted && hPlayer->state != BIP_PlayerState_ePaused && hPlayer->state != BIP_PlayerState_eAborted)
        {
            BDBG_MSG(( BIP_MSG_PRE_FMT "hPlayer %p: Player is neither started, paused, not aborted, can't call BIP_Player_GetStatusFromServer(), Player state=%s" BIP_MSG_PRE_ARG, hPlayer, BIP_PLAYER_STATE(hPlayer->state) ));
            BIP_Arb_AcceptRequest(hArb);
            completionStatus = BIP_ERR_INVALID_API_SEQUENCE;
            BIP_Arb_CompleteRequest(hArb, completionStatus);
        }
        else
        {
            completionStatus = BIP_INF_IN_PROGRESS;
            if (hPlayer->getStatusFromServerState.apiState == BIP_PlayerApiState_eIdle)
            {
                hPlayer->getStatusFromServerState.apiState = BIP_PlayerApiState_eNew;
            }
            hPlayer->getStatusFromServerSettings = *hPlayer->getStatusFromServerApi.pSettings;
            BIP_Arb_AcceptRequest(hArb);
            /* It is processed below via processPlayerState_locked() */
        }
    }

    /* If completion status is still in progress, then process various Player States. */
    if (completionStatus == BIP_INF_IN_PROGRESS)
    {
        completionStatus = processPlayerState_locked(hPlayer, threadOrigin);
    }

    /*
     * Done with state processing. We have to unlock state machine before asking Arb to do any deferred callbacks!
     */
    B_Mutex_Unlock( hPlayer->hStateMutex );
    if (!muteDebugPrint)
    {
        BDBG_MSG(( BIP_MSG_PRE_FMT  BIP_PLAYER_STATE_PRINTF_FMT "EXIT: completionStatus=%s <-------------------" BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));
    }
    BIP_MSG_TRC(( BIP_MSG_PRE_FMT BIP_PLAYER_STATE_PRINTF_FMT "EXIT: completionStatus=%s <------------------- " BIP_MSG_PRE_ARG, BIP_PLAYER_STATE_PRINTF_ARG(hPlayer), BIP_StatusGetText(completionStatus) ));

    /* Tell ARB to do any deferred work. */
    completionStatus = BIP_Arb_DoDeferred( NULL, threadOrigin );
    BDBG_ASSERT( completionStatus == BIP_SUCCESS );
    return;
} /* processPlayerState */
