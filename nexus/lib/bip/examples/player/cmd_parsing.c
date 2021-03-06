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

#include "bip.h"
#include "cmd_parsing.h"

BDBG_MODULE(player);

static void printUsage(
    char   *pCmdName
    )
{
    printf( "Usage: %s [Options] <URL> \n", pCmdName);
    printf(
            "  --help or -h for help\n"
            "  -interface       #   Optional Interface name to bind the player to\n"
            "  -pos <msec>      #   Initial Position to start Playback from\n"
            "  -jump <msec>     #   Time Position to use with runtime j (jump/seek) option: default=30000ms \n"
            "  -loop            #   Continous looping after reach End of stream\n"
            "  -playtime <msec> #   Duration for which you want the stream playback: default is play till the end.\n"
            "  -stats           #   Print Periodic Stats. \n"
          );
    printf(
            "  -sStats          #   Print Periodic Server Stats. \n"
            "  -sStats          #   Print Periodic Server Stats. \n"
            "  -disableVideo    #   Disable Video. \n"
            "  -disableAudio    #   Disable Audio. \n"
            "  -startPaused     #   Start Paused, Press play during runtime to resume playing. \n"
            "  -dtcpIpKeyFormat <keyFormat> # keyFormat values are: [commonDrm | test]. Default is commonDrm \n"
          );
    printf(
            "  -enableTimeshifting #   Set to indicate URL Playback is from a source that is timeshifting before streaming out the content. \n"
            "  -timeshiftBufferMaxDurationInMs #   Set to max duration of timeshift buffer (required if enabledTimeshift is set: defaults to 1hour). \n"
          );

    printf("  -displayformat   #   {ntsc,pal,pal_m,pal_n,pal_nc,576i,1080i,1080i50,720p,720p24,720p25,720p30,720p50,480p,576p,1080p,1080p24,1080p25,1080p30,1080p50,1080p60,720p3D,1080p3D,3840x2160p24,3840x2160p25,3840x2160p30,3840x2160p50,3840x2160p60,4096x2160p24,4096x2160p25,4096x2160p30,4096x2160p50,4096x2160p60}\n");
    printf("                   #    Note: displayformat is not used in nxclient mode.\n");
    printf("  -language        #   This is to select a language specific audio track. Specifies 3-byte language code defining language of the audio service in ISO 639-2code. \n"
           "                       Eg: eng for english, por for portugal \n"
           "  -ac3ServiceType  #   This is to select a ac3(only) serviceType specific audio track.For more details please see BIP_MediaInfoAudioAc3Bsmod enum definition in bip_media_info.h file.\n");
    printf(
            "  -usePlaypump    #   Allows user to force usage of Nexus Playpump\n"
            "  -usePlayback    #   Allows user to force usage of Nexus Playback\n"
            "  -enablePayloadScanning    #   Enables ES level Media Probe\n"
            "  -enableAutoPlayAfterStartingPaused #   If startPaused option is set & then immediately start playing after starting the Player\n"
            );
    exit(0);
} /* printUsage */

BIP_Status parseOptions(
    int    argc,
    char   *argv[],
    AppCtx *pAppCtx
    )
{
    int i;
    BIP_Status bipStatus = BIP_ERR_INTERNAL;
    bool urlIsSet = false;

    for (i=1; i<argc; i++)
    {
        if ( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") )
        {
            printUsage(argv[0]);
        }
        else if ( !strcmp(argv[i], "-interface") && i+1<argc )
        {
            BIP_String_StrcpyChar( pAppCtx->hInterfaceName, argv[++i] );
        }
        else if ( !strcmp(argv[i], "-dtcpIpKeyFormat") && i+1<argc )
        {
            BIP_String_StrcpyChar( pAppCtx->hDtcpIpKeyFormat, argv[++i] );
        }
        else if ( !strcmp(argv[i], "-loop") )
        {
            pAppCtx->enableContinousPlay = true;
        }
        else if ( !strcmp(argv[i], "-stats") )
        {
            pAppCtx->printStatus = true;
        }
        else if ( !strcmp(argv[i], "-sStats") )
        {
            pAppCtx->printServerStatus = true;
        }
        else if ( !strcmp(argv[i], "-pos") )
        {
            pAppCtx->initialPlaybackPositionInMs = strtoul(argv[++i], NULL, 0);
        }
        else if ( !strcmp(argv[i], "-jump") )
        {
            pAppCtx->jumpOffsetInMs = strtoul(argv[++i], NULL, 0);
        }
        else if ( !strcmp(argv[i], "-playtime") )
        {
            pAppCtx->playTimeInMs = strtoul(argv[++i], NULL, 0);
        }
        else if(!strcmp(argv[i], "-displayformat"))
        {
            pAppCtx->displayFormat = BIP_StrTo_NEXUS_VideoFormat(argv[++i]);
        }
        else if ( i+1 == argc )
        {
            BIP_String_StrcpyChar( pAppCtx->hUrl, argv[i] );
            urlIsSet = true;
        }
        else if ( !strcmp(argv[i], "-disableVideo") )
        {
            pAppCtx->disableVideo = true;
        }
        else if ( !strcmp(argv[i], "-startPaused") )
        {
            pAppCtx->startPaused = true;
        }
        else if ( !strcmp(argv[i], "-disableAudio") )
        {
            pAppCtx->disableAudio = true;
        }
        else if ( !strcmp(argv[i], "-enableTimeshifting") )
        {
            pAppCtx->enableTimeshifting = true;
        }
        else if ( !strcmp(argv[i], "-timeshiftBufferMaxDurationInMs") )
        {
            pAppCtx->timeshiftBufferMaxDurationInMs = strtoul(argv[++i], NULL, 0);
        }
        else if ( !strcmp(argv[i], "-language") )
        {
            BIP_String_StrcpyChar( pAppCtx->hLanguage, argv[++i] );
        }
        else if ( !strcmp(argv[i], "-ac3ServiceType") )
        {
            pAppCtx->ac3ServiceType = strtoul(argv[++i], NULL, 0);
        }
        else if ( !strcmp(argv[i], "-usePlaypump") )
        {
            pAppCtx->usePlaypump = true;
        }
        else if ( !strcmp(argv[i], "-usePlayback") )
        {
            pAppCtx->usePlayback = true;
        }
        else if ( !strcmp(argv[i], "-enablePayloadScanning") )
        {
            pAppCtx->enablePayloadScanning = true;
        }
        else if ( !strcmp(argv[i], "-enableAutoPlayAfterStartingPaused") )
        {
            pAppCtx->enableAutoPlayAfterStartingPaused = true;
        }
        else
        {
            printf("Error: incorrect or unsupported option: %s\n", argv[i]);
            printUsage(argv[0]);
        }
    }
    if (!urlIsSet) { BDBG_ERR((BIP_MSG_PRE_FMT "%s requires URL to be specified" BIP_MSG_PRE_ARG, argv[0])); printUsage(argv[0]);}
    if (pAppCtx->jumpOffsetInMs == 0) pAppCtx->jumpOffsetInMs = JUMP_OFFSET_IN_MS;
    if (pAppCtx->enableTimeshifting && !pAppCtx->timeshiftBufferMaxDurationInMs) { BDBG_ERR((BIP_MSG_PRE_FMT "%s enableTimeshifting option requires app to also specify timeshiftBufferMaxDurationInMs" BIP_MSG_PRE_ARG, argv[0])); printUsage(argv[0]);}
    if (!pAppCtx->timeshiftBufferMaxDurationInMs) { pAppCtx->timeshiftBufferMaxDurationInMs = 3600000; }
    bipStatus = BIP_SUCCESS;
    BDBG_LOG(( BIP_MSG_PRE_FMT "interface=%s, jumpOffsetInMs=%d url=%s dtcpIpKeyFormat=%s" BIP_MSG_PRE_ARG,
                BIP_String_GetString( pAppCtx->hInterfaceName ),
                pAppCtx->jumpOffsetInMs,
                BIP_String_GetString( pAppCtx->hUrl),
                BIP_String_GetString( pAppCtx->hDtcpIpKeyFormat)
             ));
    return ( bipStatus );
} /* parseOptions */

void unInitAppCtx(
    AppCtx *pAppCtx
    )
{
    if (!pAppCtx) return;
    if (pAppCtx->hInterfaceName) BIP_String_Destroy( pAppCtx->hInterfaceName);
    if (pAppCtx->hDtcpIpKeyFormat) BIP_String_Destroy( pAppCtx->hDtcpIpKeyFormat);
    if (pAppCtx->hUrl) BIP_String_Destroy( pAppCtx->hUrl);
    if (pAppCtx->hLanguage) BIP_String_Destroy( pAppCtx->hLanguage);
    if (pAppCtx) B_Os_Free( pAppCtx );
} /* unInitAppCtx */

AppCtx *initAppCtx( void )
{
    AppCtx *pAppCtx = NULL;
    BIP_Status bipStatus = BIP_ERR_INTERNAL;

    pAppCtx = B_Os_Calloc( 1, sizeof( AppCtx ));
    BIP_CHECK_GOTO(( pAppCtx ), ( "Memory Allocation Failed" ), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus );

    /* Setup default values for global App settings */
    pAppCtx->hInterfaceName = BIP_String_Create();
    BIP_CHECK_GOTO( (pAppCtx->hInterfaceName), ("BIP_String_Create() Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus);

    pAppCtx->hDtcpIpKeyFormat = BIP_String_CreateFromChar(DTCP_IP_KEY_FORMAT_COMMON_DRM);
    BIP_CHECK_GOTO( (pAppCtx->hDtcpIpKeyFormat), ("BIP_String_Create() Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus);

    pAppCtx->hUrl = BIP_String_Create();
    BIP_CHECK_GOTO( (pAppCtx->hUrl), ("BIP_String_Create() Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus);

    pAppCtx->hLanguage = BIP_String_Create();
    BIP_CHECK_GOTO( (pAppCtx->hLanguage), ("BIP_String_Create() Failed"), error, BIP_ERR_OUT_OF_SYSTEM_MEMORY, bipStatus);

    pAppCtx->ac3ServiceType = UINT_MAX;

    pAppCtx->displayFormat = NEXUS_VideoFormat_eNtsc;
error:
    return pAppCtx;
} /* initAppCtx */

#define USER_INPUT_BUF_SIZE 256
BIP_Status runTimeCmdParsing(AppCtx *pAppCtx, CmdOptions *pCmdOptions)
{
    BIP_Status bipStatus = BIP_SUCCESS;
    char    buffer[USER_INPUT_BUF_SIZE];
    size_t  bytesRead;

    memset(buffer, 0, sizeof(buffer));
    BIP_Fd_ReadWithTimeout(STDIN_FILENO, sizeof(buffer)-1, 1000 /*timeout*/, buffer, &bytesRead );
    buffer[bytesRead-1] = '\0';   /* Replace \n w/ Null Char. */

    pCmdOptions->cmd = PlayerCmd_eMax;
    if (!strcmp(buffer, "?") || !strcmp(buffer, "help"))
    {
        printf(
                "Commands:\n"
                "  play - resume normal playback\n"
                "  pause - pause playback\n"
                "  p - pause/play toggle\n"
                "  fa - frame advance\n"
                "  fr - frame reverse\n"
                "  rate(rate) - set trick rate (e.g. 6 for 6x, 1/4 for 1/4x, -6 for -6x, etc.\n"
              );
        printf(
                "  seek(pos) - seek to absolute position (in ms. e.g. 2:10.30 for 2min, 10sec, & 30ms, 2:10 for 2min & 10sec, 120 for 120ms)\n"
                "  jf -  jump forward by jump offset (default is 30000ms, changed via command line option -jump)\n"
                "  jb -  jump backward by jump offset (default is 30000ms, changed via command line option -jump)\n"
                "  st -  print status\n"
                "  sst-  print server status\n"
              );
        printf(
                "  lng(new) - This is to play audio for another language provided audio track for that language exists in the stream.\n"
                "             This is 3-byte language code defining language of the audio service in ISO 639-2code.\n"
                "             Eg: lng(eng) for english, lng(por) for portugal \n"
              );
        printf( "  bsmod(new) - This is to select a ac3(only) serviceType specific audio track provided a track of that type exist.\n"
                "               For more details please see BIP_MediaInfoAudioAc3Bsmod enum definition in bip_media_info.h file.\n"
                "               Eg: bsmod(2) for VisuallyImpaired\n"
                "  chng(url) - play for the new url.\n"
               );
    }
    else if (!strcmp(buffer, "q") || !strcmp(buffer, "quit"))
    {
        pCmdOptions->cmd = PlayerCmd_eQuit;
        BDBG_WRN(("User Quit, we are done.."));
        return (0);
    }
    else if (!strcmp(buffer, "st"))
    {
        pCmdOptions->cmd = PlayerCmd_ePrintStatus;
    }
    else if (!strcmp(buffer, "sst"))
    {
        pCmdOptions->cmd = PlayerCmd_ePrintServerStatus;
    }
    else if (!strcmp(buffer, "play") || !strcmp(buffer, "pl"))
    {
        pCmdOptions->cmd = PlayerCmd_ePlay;
        pCmdOptions->prevCmd = pCmdOptions->cmd;
    }
    else if (!strcmp(buffer, "pause") || !strcmp(buffer, "pu"))
    {
        pCmdOptions->cmd = PlayerCmd_ePause;
        pCmdOptions->prevCmd = pCmdOptions->cmd;
    }
    else if (!strcmp(buffer, "p"))
    {
        if (pCmdOptions->prevCmd == PlayerCmd_ePause)
            pCmdOptions->cmd = PlayerCmd_ePlay;
        else
            pCmdOptions->cmd = PlayerCmd_ePause;
        pCmdOptions->prevCmd = pCmdOptions->cmd;
    }
    else if (!strcmp(buffer, "jf"))
    {
        pCmdOptions->cmd = PlayerCmd_eRelativeSeekFwd;
    }
    else if (!strcmp(buffer, "jb"))
    {
        pCmdOptions->cmd = PlayerCmd_eRelativeSeekRev;
    }
    else if (!strcmp(buffer, "fa"))
    {
        pCmdOptions->cmd = PlayerCmd_eFrameAdvance;
    }
    else if (!strcmp(buffer, "fr"))
    {
        pCmdOptions->cmd = PlayerCmd_eFrameReverse;
    }
    else if (strstr(buffer, "seek(") == buffer)
    {
        unsigned min, sec, msec;

        if (sscanf(buffer+5,"%u:%u.%u", &min, &sec, &msec)==3)
        {
            pCmdOptions->seekPositionInMs = (min*60+sec)*1000+msec;
        }
        else if (sscanf(buffer+5,"%u:%u", &min, &sec)==2)
        {
            pCmdOptions->seekPositionInMs = (min*60+sec)*1000;
        }
        else
        {
            int count;
            count = sscanf(buffer+5, "%u", &pCmdOptions->seekPositionInMs);
            BDBG_ASSERT(count == 1);
        }
        pCmdOptions->cmd = PlayerCmd_eSeek;
        BDBG_WRN(("seekPositionInMs=%d", pCmdOptions->seekPositionInMs));
    }
    else if (strstr(buffer, "rate(") == buffer)
    {
        char *pTmp;
        memset(pCmdOptions->playSpeed, 0, sizeof(pCmdOptions->playSpeed));
        if ((pTmp = strstr(buffer+5, ")")) )
        {
            *pTmp = '\0';
        }
        strncpy(pCmdOptions->playSpeed, buffer+5, sizeof(pCmdOptions->playSpeed)-1);
        BDBG_WRN(("rate=%s", pCmdOptions->playSpeed));
        pCmdOptions->cmd = PlayerCmd_ePlayAtRate;
    }
    else if (strstr(buffer, "lng(") == buffer)
    {
        char *pStart = (buffer+4);
        char *pEnd = NULL;
        pEnd = strstr(buffer, ")");

        BIP_String_StrcpyCharN( pAppCtx->hLanguage, (buffer+4), (pEnd-pStart) );
        pCmdOptions->cmd = PlayerCmd_ePlayLanguageSpecificTrack;

        BDBG_MSG((" New language is  ---------------->|%s| ", BIP_String_GetString(pAppCtx->hUrl)));
    }
    else if (strstr(buffer, "bsmod(") == buffer)
    {
        int count;
        /*  BIP_String_StrcpyCharN( pAppCtx->hLanguage, (buffer+6), (pEnd-pStart) );  */

        count = sscanf(buffer+6, "%u", &pAppCtx->ac3ServiceType);
        BDBG_ASSERT(count == 1);
        pCmdOptions->cmd = PlayerCmd_ePlayBsmodSpecificTrack;

        BDBG_MSG((" New ac3 specific bsmod is  ---------------->|%u| ", pAppCtx->ac3ServiceType));
    }
    else if (strstr(buffer, "chng(") == buffer)
    {
        char *pStart = (buffer+5);
        char *pEnd = NULL;
        pEnd = strstr(buffer, ")");

        BIP_String_StrcpyCharN( pAppCtx->hUrl, (buffer+5), (pEnd-pStart) );
        pCmdOptions->cmd = PlayerCmd_ePlayNewUrl;

        BDBG_MSG((" New url is ---------------->|%s| ", BIP_String_GetString(pAppCtx->hUrl)));
    }
    else
    {
        if (bytesRead > 1) BDBG_WRN(("This cmd=%s is not yet supported", buffer));
    }

    if (pAppCtx->playTimeInMs)
    {
        B_Time curTime;
        if (!pAppCtx->playStartTimeSet)
        {
            B_Time_Get(&pAppCtx->playStartTime);
            pAppCtx->playStartTimeSet = true;
            if (pAppCtx->streamDurationInMs && pAppCtx->streamDurationInMs < pAppCtx->playTimeInMs) pAppCtx->playTimeInMs = pAppCtx->streamDurationInMs;
        }
        B_Time_Get(&curTime);
        if( B_Time_Diff(&curTime, &pAppCtx->playStartTime) > (int)pAppCtx->playTimeInMs )
        {
            pCmdOptions->cmd = PlayerCmd_eQuit;
            BDBG_WRN(("Stream Playtime=%u exceeded", pAppCtx->playTimeInMs));
        }
    }

    if (!pAppCtx->enableContinousPlay
        && pAppCtx->streamDurationInMs
        && (pAppCtx->playbackDone || (pAppCtx->playTimeInMs && pCmdOptions->cmd == PlayerCmd_eQuit))
        &&  ( pCmdOptions->cmd != PlayerCmd_ePlayNewUrl)
        )
    {
        /* We know stream duration && we either reached EOF or playTime was restricted, check if we played the expected duration. */
        unsigned expectedPosition, currentPositionInMs;
        BIP_PlayerStatus playerStatus;

        bipStatus = BIP_Player_GetStatus(pAppCtx->hPlayer, &playerStatus);
        if (bipStatus == BIP_SUCCESS )
        {
            expectedPosition = pAppCtx->playTimeInMs ? pAppCtx->playTimeInMs : pAppCtx->streamDurationInMs;
            currentPositionInMs = playerStatus.currentPositionInMs;
            if ( abs(currentPositionInMs - expectedPosition) > 2000 )
            {
                bipStatus = BIP_ERR_PLAYER_PLAY;
                BDBG_ERR(("Player didn't play the correct duration: currentPositionInMs=%u, expectedPosition=%u", currentPositionInMs, expectedPosition));
            }
            else
            {
                BDBG_WRN(("Player successfully played the correct duration: currentPositionInMs=%u, expectedPosition=%u", currentPositionInMs, expectedPosition));
            }
        }
    }
    return (bipStatus);
}

bool playerGetTrackOfType(
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
            BDBG_MSG(( BIP_MSG_PRE_FMT "Found trackType=%s with trackId=%d" BIP_MSG_PRE_ARG, BIP_ToStr_BIP_MediaInfoTrackType(trackType), pMediaInfoTrack->trackId));
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

bool playerGetSpecialAudioTrackType(
        BIP_MediaInfoHandle             hMediaInfo,
        BIP_MediaInfoTrack              *pMediaInfoTrackOut,
        const char                      *language,
        unsigned                        ac3ServiceType
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
        if (pMediaInfoTrack->trackType == BIP_MediaInfoTrackType_eAudio)
        {
            if(language && ac3ServiceType != UINT_MAX) /* If both are set */
            {
                int ret = 0;

                if(pMediaInfoTrack->info.audio.pLanguage) /* check whether track specific language info is at all present.*/
                {
                    ret = strncmp(language, pMediaInfoTrack->info.audio.pLanguage, BIP_MEDIA_INFO_LANGUAGE_FIELD_SIZE);
                }

                if((!ret) && (pMediaInfoTrack->info.audio.descriptor.ac3.bsmod == ac3ServiceType))
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT "Found Special Audio track for language = %s and pMediaInfoTrack->info.audio.descriptor.ac3.bsmod = %d" BIP_MSG_PRE_ARG,pMediaInfoTrack->info.audio.pLanguage, pMediaInfoTrack->info.audio.descriptor.ac3.bsmod));
                    *pMediaInfoTrackOut = *pMediaInfoTrack;
                    trackFound = true;
                    break;
                }
            }
            else if( language && pMediaInfoTrack->info.audio.pLanguage )
            {
                if(!strncmp(language, pMediaInfoTrack->info.audio.pLanguage, BIP_MEDIA_INFO_LANGUAGE_FIELD_SIZE))
                {
                    BDBG_MSG(( BIP_MSG_PRE_FMT "Found Audio track for language = %s and trackId =%d" BIP_MSG_PRE_ARG,pMediaInfoTrack->info.audio.pLanguage, pMediaInfoTrack->trackId));
                    *pMediaInfoTrackOut = *pMediaInfoTrack;
                    trackFound = true;
                    break;
                }
            }
            else if(ac3ServiceType != UINT_MAX
                    && (pMediaInfoTrack->info.audio.descriptor.ac3.bsmodValid)
                    && (pMediaInfoTrack->info.audio.descriptor.ac3.bsmod == ac3ServiceType))
            {
                    BDBG_MSG(( BIP_MSG_PRE_FMT "Found Special Audio track for ac3ServiceType = %d and trackId =%d" BIP_MSG_PRE_ARG, ac3ServiceType, pMediaInfoTrack->trackId));
                    *pMediaInfoTrackOut = *pMediaInfoTrack;
                    trackFound = true;
                    break;
            }
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
} /* playerGetSpecialAudioTrackType */
