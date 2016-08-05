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

/* Test with video only, since the audio security setups is very similar to video's. */
#if NEXUS_HAS_SECURITY && NEXUS_HAS_PLAYBACK && NEXUS_HAS_RECORD && (NEXUS_SECURITY_ZEUS_VERSION_MAJOR >= 4) && NEXUS_HAS_VIDEO_DECODER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nexus_security_examples_setups.h"

/* A clear stream, or stream with a clear program to be played and recorded to an encrpted stream. */
#define CLEAR_STREAM_FILE     "videos/576i50_2min.ts"

/* Any location that can be accessed by the STB
 * to record the encrypted stream with a clear program in the above stream. */
#define RECORDED_FILE "videos/recorded_stream_3des_askm.mpg"

static int      SecurityExampleSetupAskmKeyladder ( NEXUS_ExampleSecuritySettings * settings,
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

    settings.recfname = RECORDED_FILE;
    SecurityExampleSetupRecord4Encrpytion ( &settings );

    /* Setup the security to encrypt the recorded stream. */
    SecurityExampleSetupAskmKeyladder ( &settings, NEXUS_SecurityOperation_eEncrypt );

    SecurityExampleStartRecord ( &settings );
    SecurityExampleStartPlayback ( &settings );

    /* Wait for the two seconds */
    sleep ( 2 );
    printf ( "Record has been created with file %s. Use playback to inspect on HDMI.\n", RECORDED_FILE );

    /* Shutdown the modules and nexus platform. */
    SecurityExampleShutdown ( &settings );

    return 0;
}

static int SecurityExampleAllocateVkl ( NEXUS_SecurityCustomerSubMode custSubMode,
                                        NEXUS_SecurityVirtualKeyladderID * vkl,
                                        NEXUS_VirtualKeyLadderHandle * vklHandle )
{
    NEXUS_SecurityVKLSettings vklSettings;
    NEXUS_VirtualKeyLadderInfo vklInfo;

    BDBG_ASSERT ( vkl );
    BDBG_ASSERT ( vklHandle );

    NEXUS_Security_GetDefaultVKLSettings ( &vklSettings );

    /* For pre Zeus 3.0, please set vklSettings.custSubMode */
    vklSettings.custSubMode = custSubMode;

    *vklHandle = NEXUS_Security_AllocateVKL ( &vklSettings );

    if ( !( *vklHandle ) )
    {
        printf ( "\nAllocate VKL failed \n" );
        return 1;
    }

    NEXUS_Security_GetVKLInfo ( *vklHandle, &vklInfo );
    printf ( "\nVKL Handle %p is allocated for VKL#%d\n", ( void * ) *vklHandle, vklInfo.vkl & 0x7F );

    *vkl = vklInfo.vkl;

    return 0;
}

static int SecurityExampleSetupAskmKeyladder ( NEXUS_ExampleSecuritySettings * settings,
                                               NEXUS_SecurityOperation operation )
{
    NEXUS_SecurityEncryptedSessionKey encryptedSessionkey;
    NEXUS_SecurityEncryptedControlWord encrytedCW;
    NEXUS_SecurityAlgorithmSettings NexusConfig;
    NEXUS_SecurityKeySlotSettings keySlotSettings;
    NEXUS_SecurityVirtualKeyladderID vklId;

    /* The encryption keys are the same for Audio/Video EVEN/ODD keys in this example.  Those
     * Keys can be different in a real application */

    unsigned char   ucProcInForKey3[16] = {
        0xEB, 0x05, 0x5B, 0x0E, /*high key */
        0xAC, 0xc0, 0x94, 0x31,
        0xEB, 0x05, 0x5B, 0x0E, /*low key  use twice */
        0xAC, 0xc0, 0x94, 0x30
    };

    unsigned char   ucProcInKey4[16] = {
        0xc1, 0x2c, 0x19, 0x24,
        0xd4, 0x84, 0x66, 0xa1,
        0xb4, 0x05, 0x3c, 0xac,
        0xe8, 0x0c, 0x95, 0x0e,
    };

    /* Request for an VKL to use */
    if ( SecurityExampleAllocateVkl ( NEXUS_SecurityCustomerSubMode_eGeneric_CP_128_4, &vklId, &settings->vklHandle ) )
    {
        printf ( "\nAllocate VKL failed.\n" );
        return 1;
    }

    NEXUS_Security_GetDefaultKeySlotSettings ( &keySlotSettings );
    /* keySlotSettings.keySlotEngine = NEXUS_SecurityEngine_eCaCp; */
    keySlotSettings.keySlotEngine = NEXUS_SecurityEngine_eM2m;
    /* Allocate AV keyslots */
    settings->videoKeyHandle = NEXUS_Security_AllocateKeySlot ( &keySlotSettings );
    if ( !settings->videoKeyHandle )
    {
        printf ( "\nAllocate CA video keyslot failed \n" );
        return 1;
    }

    /* Set up key for clear key */
    NEXUS_Security_GetDefaultAlgorithmSettings ( &NexusConfig );
    NexusConfig.algorithm = NEXUS_SecurityAlgorithm_e3DesAba;
    NexusConfig.algorithmVar = NEXUS_SecurityAlgorithmVariant_eEcb;

    NexusConfig.terminationMode = NEXUS_SecurityTerminationMode_eClear;
    NexusConfig.ivMode = NEXUS_SecurityIVMode_eRegular;
    NexusConfig.solitarySelect = NEXUS_SecuritySolitarySelect_eClear;
    NexusConfig.caVendorID = 0x1234;
    NexusConfig.askmModuleID = NEXUS_SecurityAskmModuleID_eModuleID_8;
    NexusConfig.otpId = NEXUS_SecurityOtpId_eOneVal;
    NexusConfig.key2Select = NEXUS_SecurityKey2Select_eFixedKey;

    NexusConfig.keyDestEntryType = NEXUS_SecurityKeyType_eClear;

    /* Just test encrypt a stream with ASKM and record it to another stream
     * and descrpyt the recorded stream. */
    NexusConfig.operation = operation;
    NexusConfig.dest =
        ( operation ==
          NEXUS_SecurityOperation_eEncrypt ) ? NEXUS_SecurityAlgorithmConfigDestination_eCps :
        NEXUS_SecurityAlgorithmConfigDestination_eCpd;

    /* configure the key slot */
    NEXUS_Security_ConfigAlgorithm ( settings->videoKeyHandle, &NexusConfig );

    memset ( &encryptedSessionkey, 0, sizeof ( NEXUS_SecurityEncryptedSessionKey ) );
    memset ( &encrytedCW, 0, sizeof ( NEXUS_SecurityEncryptedControlWord ) );
    /* Load session key - key3 */
    NEXUS_Security_GetDefaultSessionKeySettings ( &encryptedSessionkey );
    encryptedSessionkey.keyladderType = NEXUS_SecurityKeyladderType_e3Des;
    encryptedSessionkey.swizzleType = NEXUS_SecuritySwizzleType_eNone;
    encryptedSessionkey.cusKeyL = 0x00;
    encryptedSessionkey.cusKeyH = 0x00;
    encryptedSessionkey.cusKeyVarL = 0x00;
    encryptedSessionkey.cusKeyVarH = 0xFF;
    /* -------- */
    encryptedSessionkey.keyGenCmdID = NEXUS_SecurityKeyGenCmdID_eKeyGen;
    encryptedSessionkey.sessionKeyOp = NEXUS_SecuritySessionKeyOp_eNoProcess;
    encryptedSessionkey.bASKMMode = true;
    encryptedSessionkey.rootKeySrc = NEXUS_SecurityRootKeySrc_eOtpKeyA;
    encryptedSessionkey.bSwapAESKey = false;
    encryptedSessionkey.keyDestIVType = NEXUS_SecurityKeyIVType_eNoIV;

    /* -------- */
    encryptedSessionkey.bRouteKey = false;
    encryptedSessionkey.operation = NEXUS_SecurityOperation_eDecrypt;
    encryptedSessionkey.operationKey2 = NEXUS_SecurityOperation_eEncrypt;
    encryptedSessionkey.keyEntryType = NEXUS_SecurityKeyType_eClear;

    /* ++++++++ */
    encryptedSessionkey.custSubMode = NEXUS_SecurityCustomerSubMode_eGeneric_CP_128_4;
    encryptedSessionkey.virtualKeyLadderID = vklId;
    encryptedSessionkey.keyMode = NEXUS_SecurityKeyMode_eRegular;
    /* ++++++++ */

    BKNI_Memcpy ( encryptedSessionkey.keyData, ucProcInForKey3, sizeof ( ucProcInForKey3 ) );

    if ( NEXUS_Security_GenerateSessionKey ( settings->videoKeyHandle, &encryptedSessionkey ) != 0 )
    {
        printf ( "\nLoading session key failed \n" );
        return 1;
    }

    printf ( "\n\n\n\nGenerate Session Key Success\n" );

    /* Load CW - key4 --- routed out */
    NEXUS_Security_GetDefaultControlWordSettings ( &encrytedCW );
    encrytedCW.keyladderType = NEXUS_SecurityKeyladderType_e3Des;
    encrytedCW.keySize = sizeof ( ucProcInKey4 );
    encrytedCW.keyEntryType = NEXUS_SecurityKeyType_eClear;

    /* ++++++++ */
    encrytedCW.custSubMode = NEXUS_SecurityCustomerSubMode_eGeneric_CP_128_4;
    encrytedCW.virtualKeyLadderID = vklId;
    encrytedCW.keyMode = NEXUS_SecurityKeyMode_eRegular;
    /* ++++++++ */

    /*encrytedCW.pKeyData = ucProcInKey4; */
    BKNI_Memcpy ( encrytedCW.keyData, ucProcInKey4, encrytedCW.keySize );
    encrytedCW.operation = NEXUS_SecurityOperation_eDecrypt;
    encrytedCW.bRouteKey = true;
    encrytedCW.keyDestIVType = NEXUS_SecurityKeyIVType_eNoIV;
    encrytedCW.keyGenCmdID = NEXUS_SecurityKeyGenCmdID_eKeyGen;
    encrytedCW.bSwapAESKey = false;

    printf ( "\n\n\n\nGenerate control word\n" );

    if ( NEXUS_Security_GenerateControlWord ( settings->videoKeyHandle, &encrytedCW ) )
    {
        printf ( "\nGenerate Control Word failed\n" );
        return 1;
    }

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
