/*******************************************************************************
 * Copyright (C) 2018 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to
 * the terms and conditions of a separate, written license agreement executed
 * between you and Broadcom (an "Authorized License").  Except as set forth in
 * an Authorized License, Broadcom grants no license (express or implied),
 * right to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein. IF YOU HAVE NO AUTHORIZED LICENSE,
 * THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 * IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
 * THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 * OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 ******************************************************************************/
#include "bdsp_types.h"
#include "bdsp_raaga_fw.h"

const BDSP_VOM_Algo_Start_Addr BDSP_sAlgoStartAddr =
{
	{
		/* BDSP_AF_P_AlgoId_eMpegDecode */
		0x30011800,

		/* BDSP_AF_P_AlgoId_eAc3Decode */
		0x300480d4,

		/* BDSP_AF_P_AlgoId_eAacDecode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAacHeLpSbrDecode */
		0x100c2000,

		/* BDSP_AF_P_AlgoId_eDdpDecode */
		0x300880ec,

		/* BDSP_AF_P_AlgoId_eDdLosslessDecode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eLpcmCustomDecode */
		0x3028d000,

		/* BDSP_AF_P_AlgoId_eBdLpcmDecode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDvdLpcmDecode */
		0x3028d000,

		/* BDSP_AF_P_AlgoId_eHdDvdLpcmDecode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegMcDecode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eWmaStdDecode */
		0x1010b860,

		/* BDSP_AF_P_AlgoId_eWmaProStdDecode */
		0x3017516c,

		/* BDSP_AF_P_AlgoId_eMlpDecode */
		0x1045a000,

		/* BDSP_AF_P_AlgoId_eDdp71Decode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDtsDecode */
		0x30144000,

		/* BDSP_AF_P_AlgoId_eDtsLbrDecode */
		0x3011e000,

		/* BDSP_AF_P_AlgoId_eDtsHdDecode */
		0x30144000,

		/* BDSP_AF_P_AlgoId_ePcmWavDecode */
		0x1016f800,

		/* BDSP_AF_P_AlgoId_eAmrDecode */
		0x10326000,

		/* BDSP_AF_P_AlgoId_eDraDecode */
		0x101d0000,

		/* BDSP_AF_P_AlgoId_eRealAudioLbrDecode */
		0x30293000,

		/* BDSP_AF_P_AlgoId_eDolbyPulseDecode */
		0x301dbd60,

		/* BDSP_AF_P_AlgoId_eMs10DdpDecode */
		0x300a0000,

		/* BDSP_AF_P_AlgoId_eAdpcmDecode */
		0x302af000,

		/* BDSP_AF_P_AlgoId_eG711G726Decode */
		0x3037e800,

		/* BDSP_AF_P_AlgoId_eG729Decode */
		0x30383800,

		/* BDSP_AF_P_AlgoId_eVorbisDecode */
		0x303bb000,

		/* BDSP_AF_P_AlgoId_eG723_1Decode */
		0x103cb000,

		/* BDSP_AF_P_AlgoId_eFlacDecode */
		0x1044dda8,

		/* BDSP_AF_P_AlgoId_eMacDecode */
		0x30453800,

		/* BDSP_AF_P_AlgoId_eAmrWbDecode */
		0x10474000,

		/* BDSP_AF_P_AlgoId_eiLBCDecode */
		0x104d9f38,

		/* BDSP_AF_P_AlgoId_eiSACDecode */
		0x304f4844,

		/* BDSP_AF_P_AlgoId_eUdcDecode */
		0x30529800,

		/* BDSP_AF_P_AlgoId_eDolbyAacheDecode */
		0x305f9d60,

		/* BDSP_AF_P_AlgoId_eOpusDecode */
		0x306b024c,

		/* BDSP_AF_P_AlgoId_eALSDecode */
		0x306e85b4,

		/* BDSP_AF_P_AlgoId_eAC4Decode */
		0x306efe48,

		/* BDSP_AF_P_AlgoId_eEndOfAudioDecodeAlgos */
		0x00000000,

		/* BDSP_VF_P_AlgoId_eRealVideo9Decode */
		0x00000000,

		/* BDSP_VF_P_AlgoId_eVP6Decode */
		0x102c1000,

		/* BDSP_AF_P_AlgoId_eEndOfDecodeAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eMpegMcFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAdtsFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eLoasFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eWmaStdFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eWmaProFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eAc3FrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDdpFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDdp71FrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDtsFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDtsLbrFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDtsHdFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDtsHdFrameSync_1 */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDtsHdHdDvdFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDdLosslessFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMlpFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eMlpHdDvdFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_ePesFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eBdLpcmFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eHdDvdLpcmFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDvdLpcmFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDvdLpcmFrameSync_1 */
		0x00000000,

		/* BDSP_AF_P_AlgoId_ePcmWavFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eDraFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eRealAudioLbrFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eMs10DdpFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eVorbisFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eFlacFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eMacFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eUdcFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eAC4FrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eALSFrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eEndOfAudioDecFsAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eRealVideo9FrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eVP6FrameSync */
		0x30000004,

		/* BDSP_AF_P_AlgoId_eEndOfDecFsAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAc3Encode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegL2Encode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegL3Encode */
		0x10370000,

		/* BDSP_AF_P_AlgoId_eAacLcEncode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAacHeEncode */
		0x102df908,

		/* BDSP_AF_P_AlgoId_eDtsEncode */
		0x3027b0c8,

		/* BDSP_AF_P_AlgoId_eDtsBroadcastEncode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eSbcEncode */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMs10DDTranscode */
		0x1025c800,

		/* BDSP_AF_P_AlgoId_eG711G726Encode */
		0x3038f000,

		/* BDSP_AF_P_AlgoId_eG729Encode */
		0x10394800,

		/* BDSP_AF_P_AlgoId_eG723_1Encode */
		0x103d6800,

		/* BDSP_AF_P_AlgoId_eG722Encode */
		0x1046f800,

		/* BDSP_AF_P_AlgoId_eAmrEncode */
		0x10486000,

		/* BDSP_AF_P_AlgoId_eAmrwbEncode */
		0x304a7800,

		/* BDSP_AF_P_AlgoId_eiLBCEncode */
		0x304c4800,

		/* BDSP_AF_P_AlgoId_eiSACEncode */
		0x304e5800,

		/* BDSP_AF_P_AlgoId_eLpcmEncode */
		0x30525800,

		/* BDSP_AF_P_AlgoId_eOpusEncode */
		0x30569800,

		/* BDSP_AF_P_AlgoId_eDDPEncode */
		0x305a8000,

		/* BDSP_AF_P_AlgoId_eEndOfAudioEncodeAlgos */
		0x00000000,

		/* BDSP_VF_P_AlgoId_eH264Encode */
		0x103fb000,

		/* BDSP_VF_P_AlgoId_eX264Encode */
		0x10424000,

		/* BDSP_VF_P_AlgoId_eXVP8Encode */
		0x10433000,

		/* BDSP_AF_P_AlgoId_eEndOfEncodeAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAc3EncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegL3EncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMpegL2EncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAacLcEncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAacHeEncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDtsEncFrameSync */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eEndOfEncFsAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_ePassThru */
		0x300ffc80,

		/* BDSP_AF_P_AlgoId_eMlpPassThru */
		0x30472000,

		/* BDSP_AF_P_AlgoId_eEndOfAuxAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eSrsTruSurroundPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eSrcPostProc */
		0x10255800,

		/* BDSP_AF_P_AlgoId_eDdbmPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDownmixPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eCustomSurroundPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eCustomBassPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eKaraokeCapablePostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eCustomVoicePostProc */
		0x101c7000,

		/* BDSP_AF_P_AlgoId_ePeqPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAvlPostProc */
		0x302b3000,

		/* BDSP_AF_P_AlgoId_ePl2PostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eXenPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eBbePostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDsolaPostProc */
		0x3028a800,

		/* BDSP_AF_P_AlgoId_eDtsNeoPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDDConvert */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAudioDescriptorFadePostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAudioDescriptorPanPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_ePCMRouterPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eWMAPassThrough */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eSrsTruSurroundHDPostProc */
		0x102a6000,

		/* BDSP_AF_P_AlgoId_eSrsTruVolumePostProc */
		0x1029c800,

		/* BDSP_AF_P_AlgoId_eDolbyVolumePostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eBrcm3DSurroundPostProc */
		0x302b8000,

		/* BDSP_AF_P_AlgoId_eFWMixerPostProc */
		0x3026c800,

		/* BDSP_AF_P_AlgoId_eMonoDownMixPostProc */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMs10DDConvert */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eDdrePostProc */
		0x10335800,

		/* BDSP_AF_P_AlgoId_eDv258PostProc */
		0x10343800,

		/* BDSP_AF_P_AlgoId_eDpcmrPostProc */
		0x10356768,

		/* BDSP_AF_P_AlgoId_eGenCdbItbPostProc */
		0x102bb800,

		/* BDSP_AF_P_AlgoId_eBtscEncoderPostProc */
		0x103b3860,

		/* BDSP_AF_P_AlgoId_eSpeexAECPostProc */
		0x103e5ca8,

		/* BDSP_AF_P_AlgoId_eKaraokePostProc */
		0x10527000,

		/* BDSP_AF_P_AlgoId_eMixerDapv2PostProc */
		0x3066f800,

		/* BDSP_AF_P_AlgoId_eOutputFormatterPostProc */
		0x106ca000,

		/* BDSP_AF_P_AlgoId_eVocalPostProc */
		0x306cc000,

		/* BDSP_AF_P_AlgoId_eFadeCtrlPostProc */
		0x306cd800,

		/* BDSP_AF_P_AlgoId_eAmbisonicsPostProc */
		0x306d2cc4,

		/* BDSP_AF_P_AlgoId_eTsmCorrectionPostProc */
		0x30770000,

		/* BDSP_AF_P_AlgoId_eEndOfPpAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eMixerFrameSync */
		0x3036e800,

		/* BDSP_AF_P_AlgoId_eMixerDapv2FrameSync */
		0x30668800,

		/* BDSP_AF_P_AlgoId_eEndOfPpFsAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eSysLib */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eAlgoLib */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eIdsCommon */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eVidIdsCommon */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eEndOfLibAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eScm1 */
		0x304fe800,

		/* BDSP_AF_P_AlgoId_eScm2 */
		0x304ff000,

		/* BDSP_AF_P_AlgoId_eScm3 */
		0x2000293c,

		/* BDSP_AF_P_AlgoId_eEndOfScmAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eScmTask */
		0x3051f800,

		/* BDSP_AF_P_AlgoId_eVideoDecodeTask */
		0x30521800,

		/* BDSP_AF_P_AlgoId_eVideoEncodeTask */
		0x30523800,

		/* BDSP_AF_P_AlgoId_eEndOfTaskAlgos */
		0x00000000,

		/* BDSP_AF_P_AlgoId_eEndOfAlgos */
		0x00000000,

	},
};
