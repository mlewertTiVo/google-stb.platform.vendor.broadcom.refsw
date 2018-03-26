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
 *****************************************************************************/

#include "bmuxlib_asp_source_helper_priv.h"
#include "bmuxlib_asp_source_entry_context.h"

BDBG_MODULE(BMUXLIB_ASP_SOURCE_HELPER);

void
BMUXlib_ASP_P_Source_Helper_Output_Descriptor_Initialize(
      BMUXlib_Source_Entry_Handle hSourceEntryHandle,
      uint32_t uiEndESCR,
      uint8_t uiPacketSize,
      BMUXlib_ASP_Output_Handle hOutputHandle,
      uint64_t uiPayloadOffset,
      size_t uiPayloadSize,
      uint16_t uiPIDChannelIndex,
      BMUXlib_ASP_Output_Descriptor_t *pstOutputDescriptor,
      BMUXlib_ASP_Output_Descriptor_Metadata_t *pstOutputDescriptorMetadata
      )
{
   /* Calculate how much payload until uiEndESCR */
   {
      size_t uiMaxPayloadSize = ( ( (uiEndESCR - hSourceEntryHandle->stTiming.uiCurrentESCR) + ( hSourceEntryHandle->stTiming.uiCurrentPacket2PacketTimestampDelta - 1 ) ) / hSourceEntryHandle->stTiming.uiCurrentPacket2PacketTimestampDelta ) * uiPacketSize;
      if ( 0 == uiMaxPayloadSize )
      {
         /* Always send at least one packet to avoid deadlock when two inputs have the same currentESCR */
         uiMaxPayloadSize = uiPacketSize;
      }
      if ( uiPayloadSize > uiMaxPayloadSize )
      {
         uiPayloadSize = uiMaxPayloadSize;
      }
   }

   BMUXlib_ASP_P_Output_Descriptor_Initialize( hOutputHandle, uiPayloadOffset, uiPayloadSize, uiPIDChannelIndex, pstOutputDescriptor, pstOutputDescriptorMetadata );

   /* Set Next Packet Pacing Timestamp */
   pstOutputDescriptor->uiNextPacketPacingTimestamp = hSourceEntryHandle->stTiming.uiCurrentESCR;
   pstOutputDescriptor->bNextPacketPacingTimestampValid = TRUE;

   /* Set Packet2Packet Timestamp Delta */
   pstOutputDescriptor->uiPacket2PacketTimestampDelta = hSourceEntryHandle->stTiming.uiCurrentPacket2PacketTimestampDelta;
   pstOutputDescriptor->bPacket2PacketTimestampDeltaValid = TRUE;
}
