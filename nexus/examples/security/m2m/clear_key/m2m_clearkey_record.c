/******************************************************************************
* Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
*
* This program is the proprietary software of Broadcom and/or its
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
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. , WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
******************************************************************************/
 /* Example m2m AES-ECB encryption record with clearkey
  * This example makes a pair with playback using "m2m_clearkey_playback.c"
  *
  * The app play a very short clear stream, and then record it to another encrypted stream.
  * This remove the need to setup the frontend and channel information etc.
  */

#if NEXUS_HAS_SECURITY && NEXUS_HAS_PLAYBACK && NEXUS_HAS_RECORD && (NEXUS_SECURITY_ZEUS_VERSION_MAJOR >= 4) && NEXUS_HAS_VIDEO_DECODER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nexus_security_examples_setups.h"

/* Input stream file for playback -> record with encryption. */
#define CLEAR_STREAM_FILE               "videos/576i50_2min.ts";

/* Output stream from record. */
#define ENCRYPTED_STREAM_FILE           "videos/recorded_m2m_aesecb_clearkey.mpg"

static int      SecurityExampleSetupM2mClearkeyslot ( NEXUS_ExampleSecuritySettings * settings,
                                                      NEXUS_SecurityOperation operation );

int main ( void )
{
    NEXUS_ExampleSecuritySettings settings;

    /* Nexus platform initilisation. */
    SecurityExampleInitPlatform ( &settings );

    settings.playfname = CLEAR_STREAM_FILE;
    SecurityExampleSetupPlayback ( &settings );

    SecurityExampleSetupDecoders ( &settings );

    SecurityExampleSetupPlaybackPidChannels ( &settings );

    settings.recfname = ENCRYPTED_STREAM_FILE;
    SecurityExampleSetupRecord4Encrpytion ( &settings );

    /* Config M2M encryption. */
    SecurityExampleSetupM2mClearkeyslot ( &settings, NEXUS_SecurityOperation_eEncrypt );

    SecurityExampleStartRecord ( &settings );
    SecurityExampleStartPlayback ( &settings );

    /* Wait for the two seconds */
    sleep ( 2 );
    printf ( "Record has been created with file %s. Use playback to inspect on HDMI.\n", ENCRYPTED_STREAM_FILE );

    /* Shutdown the modules and nexus platform. */
    SecurityExampleShutdown ( &settings );

    return 0;
}

static int SecurityExampleSetupM2mClearkeyslot ( NEXUS_ExampleSecuritySettings * settings,
                                                 NEXUS_SecurityOperation operation )
{
    NEXUS_SecurityAlgorithmSettings algConfig;
    NEXUS_SecurityClearKey key;
    NEXUS_SecurityKeySlotSettings keySettings;
    NEXUS_SecurityInvalidateKeySettings invSettings;
    NEXUS_SecurityKeySlotInfo keyslotInfo;

    const uint8_t   keys[16] = VIDEO_ENCRYPTION_KEY;

    /* Allocate AV keyslots */
    NEXUS_Security_GetDefaultKeySlotSettings ( &keySettings );
    keySettings.keySlotEngine = NEXUS_SecurityEngine_eM2m;
    /* For transport stream, use type0 or 2.
     * By default, type 3 is used, which is also fine for M2M engine. */
    settings->videoKeyHandle = NEXUS_Security_AllocateKeySlot ( &keySettings );
    if ( !settings->videoKeyHandle )
    {
        printf ( "\nAllocate enc keyslot failed \n" );
        return 1;
    }

    /* Sanity check. */
    NEXUS_KeySlot_GetInfo ( settings->videoKeyHandle, &keyslotInfo );
    if ( keySettings.keySlotType != keyslotInfo.keySlotType
         && keySettings.keySlotType != NEXUS_SecurityKeySlotType_eAuto )
    {
        printf ( "Warning: nexus keyslot type mismatches as expected type %d got key type %d\n",
                 keySettings.keySlotType, keyslotInfo.keySlotType );
    }
    else
    {
        printf ( "Got expected nexus keyslot type %d from key slot handler key type %d\n", keySettings.keySlotType,
                 keyslotInfo.keySlotType );
    }

    /* Invalidate all the keys. */
    NEXUS_Security_GetDefaultInvalidateKeySettings ( &invSettings );
    invSettings.invalidateAllEntries = true;
    invSettings.invalidateKeyType = NEXUS_SecurityInvalidateKeyFlag_eDestKeyOnly;
    NEXUS_Security_InvalidateKey ( settings->videoKeyHandle, &invSettings );

    NEXUS_Security_GetDefaultAlgorithmSettings ( &algConfig );
    algConfig.algorithm = NEXUS_SecurityAlgorithm_eAes128;
    algConfig.algorithmVar = NEXUS_SecurityAlgorithmVariant_eEcb;
    algConfig.terminationMode = NEXUS_SecurityTerminationMode_eCipherStealing;
    algConfig.operation = operation;
    algConfig.dest = NEXUS_SecurityAlgorithmConfigDestination_eMem2Mem;

    algConfig.keyDestEntryType = NEXUS_SecurityKeyType_eClear;
    NEXUS_Security_ConfigAlgorithm ( settings->videoKeyHandle, &algConfig );
    algConfig.keyDestEntryType = NEXUS_SecurityKeyType_eEven;
    NEXUS_Security_ConfigAlgorithm ( settings->videoKeyHandle, &algConfig );
    algConfig.keyDestEntryType = NEXUS_SecurityKeyType_eOdd;
    NEXUS_Security_ConfigAlgorithm ( settings->videoKeyHandle, &algConfig );

    /* Load AV keys */
    NEXUS_Security_GetDefaultClearKey ( &key );
    key.dest = NEXUS_SecurityAlgorithmConfigDestination_eMem2Mem;
    key.keyIVType = NEXUS_SecurityKeyIVType_eNoIV;
    key.keySize = sizeof ( keys );
    BKNI_Memcpy ( key.keyData, keys, sizeof ( keys ) );

    key.keyEntryType = NEXUS_SecurityKeyType_eClear;
    NEXUS_Security_LoadClearKey ( settings->videoKeyHandle, &key );

    key.keyEntryType = NEXUS_SecurityKeyType_eOdd;
    NEXUS_Security_LoadClearKey ( settings->videoKeyHandle, &key );

    key.keyEntryType = NEXUS_SecurityKeyType_eEven;
    NEXUS_Security_LoadClearKey ( settings->videoKeyHandle, &key );

    /* Add video PID channel to keyslot */
    BDBG_ASSERT ( settings->videoPidChannel );
    NEXUS_KeySlot_AddPidChannel ( settings->videoKeyHandle, settings->videoPidChannel );

    return 0;
}

#else /* NEXUS_HAS_SECURITY */
#include <stdio.h>
int main ( void )
{
    printf ( "This application is not supported on this platform!\n" );
    return -1;
}
#endif
