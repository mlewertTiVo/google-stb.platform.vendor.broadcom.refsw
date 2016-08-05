/***************************************************************************
*  Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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
* 
* API Description:
*   API name: AudioInput
*   Generic API for inputs to an audio mixer.
*
***************************************************************************/
#ifndef NEXUS_AUDIO_INPUT_H__
#define NEXUS_AUDIO_INPUT_H__

#include "nexus_types.h"
#include "nexus_audio_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
Summary:
Input Status Information
 ***************************************************************************/
typedef struct NEXUS_AudioInputStatus
{
    bool valid;                 /* If true, a signal is present, and status is valid */
    NEXUS_AudioCodec codec;     /* Stream Codec */
    unsigned numPcmChannels;    /* Number of PCM channels if codec == NEXUS_AudioCodec_ePcm */
    unsigned sampleRate;        /* Sample Rate in Hz */
} NEXUS_AudioInputStatus;

/***************************************************************************
Summary:
Shutdown the connector for an audio input/processing object.

Description:
This must be called before the corresponding object is closed.
 ***************************************************************************/
void NEXUS_AudioInput_Shutdown(
    NEXUS_AudioInputHandle input
    );

/***************************************************************************
Summary:
Determine if two audio nodes are connected to one another.

Description:
This routine will return true if the two nodes are linked in a filter 
graph.  Order is not important, both directions will be scanned.  It will 
also return true even if there are nodes between the two provided in this 
call. 
 ***************************************************************************/
void NEXUS_AudioInput_IsConnectedToInput(
    NEXUS_AudioInputHandle input1,
    NEXUS_AudioInputHandle input2,
    bool *pConnected            /* [out] True if the nodes are connected in a filter graph */
    );

/***************************************************************************
Summary:
Determine if an output is connected to a node.

Description:
This routine will return true if the input and output provided are linked 
in a filter graph.  It will also return true even if there are nodes between 
input and output.
 ***************************************************************************/
void NEXUS_AudioInput_IsConnectedToOutput(
    NEXUS_AudioInputHandle input,
    NEXUS_AudioOutputHandle output,
    bool *pConnected            /* [out] True if the nodes are connected in a filter graph */
    );

/***************************************************************************
Summary:
Determine if any outputs are connected to a node.

Description:
This routine will return true if the input has one or more outputs connected  
in a filter graph.  It will also return true even if there are nodes between 
input and output.
 ***************************************************************************/
void NEXUS_AudioInput_HasConnectedOutputs(
    NEXUS_AudioInputHandle input,
    bool *pConnected            /* [out] True if one or more output is connected to this node */
    );

/***************************************************************************
Summary:
  Get the status of an Audio Input.

 ***************************************************************************/
NEXUS_Error NEXUS_AudioInput_GetInputStatus(
    NEXUS_AudioInputHandle input,
    NEXUS_AudioInputStatus *pStatus     /* [out] */
    );

#ifdef __cplusplus
}
#endif

#endif /* #ifndef NEXUS_AUDIO_INPUT_H__ */
