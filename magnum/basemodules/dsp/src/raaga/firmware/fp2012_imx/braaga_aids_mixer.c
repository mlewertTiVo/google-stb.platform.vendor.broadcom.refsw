/*******************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 ******************************************************************************/

#include "bchp.h"
const uint32_t BDSP_IMG_mixer_ids_array1[] = {
	0x53fcd1cf,
	0x60fffffe,
	0x9a1d11cf,
	0x3c1ca707,
	0xfc1cab17,
	0xe7a00307,
	0x3c1caf27,
	0xfc1cb337,
	0xd3817e07,
	0x3c1cb747,
	0xfc1cbb57,
	0x3c1cbf67,
	0xfc1cc377,
	0x53fb71d7,
	0x50fbf264,
	0xf55f9187,
	0x232d41f8,
	0x58801117,
	0x2080fe1f,
	0x91be4007,
	0x21aa420f,
	0x88ac7e17,
	0x0821c907,
	0x38018107,
	0x081fb907,
	0x381fb107,
	0x081f4107,
	0x383f5107,
	0x083fc107,
	0x383fb907,
	0x081fc907,
	0x381fd107,
	0x081fd907,
	0x381fe107,
	0x081fe907,
	0x381ff107,
	0x201ff907,
	0x31e0433f,
	0x53ce31d7,
	0x50fbf27e,
	0x21b67337,
	0x88807e17,
	0x27b34e07,
	0xc8967e1f,
	0x6071fff4,
	0xc1a6734f,
	0x8650302f,
	0xc8007e07,
	0x53f231d7,
	0x50fbf260,
	0x2080520f,
	0x98807e17,
	0x30907e1f,
	0x97bffe27,
	0xc8807e37,
	0x70122d10,
	0x86507597,
	0xc8007e07,
	0x53efb1d7,
	0x50fbf260,
	0x70c26909,
	0x73327009,
	0xc89e7e17,
	0xc1984807,
	0x53c7b1d7,
	0x50fbf27e,
	0x082059cf,
	0x281f1907,
	0xc1824927,
	0xf55f0187,
	0xab2801f8,
	0x98002107,
	0x21de4367,
	0x88804347,
	0x2080431f,
	0x98807f2f,
	0x20807f5f,
	0x988e7f77,
	0x21907357,
	0x818c736f,
	0xa7000967,
	0xdc008187,
	0xab1d01f8,
	0x2180cb2f,
	0x81d45147,
	0x24024b87,
	0xb18e471f,
	0xabfe71f8,
	0xc8804407,
	0x53e3b1d7,
	0x50fbf265,
	0x08001907,
	0x2000a107,
	0x9041590f,
	0xab3e71d7,
	0x73626089,
	0x0fb4ce07,
	0xc8042907,
	0xc8804c17,
	0xab3fb1d7,
	0x71627009,
	0x9000b007,
	0x54400387,
	0xb00000bb,
	0xab1d41f8,
	0x904021cf,
	0xdc008187,
	0xab0321f8,
	0x084099cf,
	0x204029cf,
	0x719ee072,
	0x085fa1cf,
	0x383f59cf,
	0x08012107,
	0x38011907,
	0xdc008287,
	0x985f99c8,
	0xdc008087,
	0x985fa9c8,
	0x08041907,
	0x2003a107,
	0x71527049,
	0xc880fe0f,
	0x9800f1cf,
	0x082091cf,
	0x3803e9cf,
	0x0c020187,
	0xb82409cf,
	0xab1681f8,
	0x7b018110,
	0xce010e01,
	0x980079c9,
	0x9803f9c8,
	0x902059cf,
	0xf5400e87,
	0xab04a1f8,
	0x71226049,
	0x3001810f,
	0x27bffe37,
	0x70826009,
	0x2084fe17,
	0x919a7227,
	0x088c7e2f,
	0x981fa1cf,
	0x864dbd4f,
	0xc8127e07,
	0x53da31d7,
	0x50fbf25e,
	0xc8297e07,
	0x53d971d7,
	0x50fbf25e,
	0x08041907,
	0x2003a107,
	0x9000690f,
	0xf55f8187,
	0xab0121f8,
	0x71726049,
	0xdc008387,
	0xab1bc1f8,
	0x3004810f,
	0x27b34e07,
	0x20807e17,
	0x98967e1f,
	0x6071fff4,
	0x085f3907,
	0x3823d9cf,
	0x0844d107,
	0x382461cf,
	0x864ffd6f,
	0xc8007e07,
	0x53d3b1d7,
	0x50fbf260,
	0x08003127,
	0x2000e90f,
	0x7b008110,
	0x0816fe01,
	0x9800d127,
	0x0815fe00,
	0x983fc901,
	0x981f8101,
	0x53d1b1d7,
	0x50fbf25e,
	0x08003927,
	0x221d11cf,
	0x60982f49,
	0x65a06649,
	0xf55f8087,
	0x65a8af49,
	0x38d1ffff,
	0xf55f8197,
	0x0880fe00,
	0x983fe902,
	0x08206900,
	0x3c1743cf,
	0x9c1027cf,
	0x71104619,
	0xf83d31ff,
	0xab0031ff,
	0x0e174a17,
	0xc0602947,
	0xc880540f,
	0x71d0de2f,
	0x2eb7c37f,
	0xdc010087,
	0x23e1e1f8,
	0x58805e07,
	0x70a0d0cb,
	0x86503387,
	0x70a0d0cb,
	0xe7b6d407,
	0x53c631d7,
	0x50fbf261,
	0x900031cf,
	0xf55f8087,
	0xab0541f8,
	0x902061cf,
	0x0bdef1ff,
	0x58001907,
	0xce020e07,
	0x0beab1ff,
	0x580079cf,
	0x9000b807,
	0x54400387,
	0xb00000bb,
	0xabe261f8,
	0x90002807,
	0x54400087,
	0xb00000bb,
	0xabe141f8,
	0xabe4b1ff,
	0xab0bb1d7,
	0x73327009,
	0xe300100f,
	0x0bd7f1ff,
	0x58002107,
	0x7170d04b,
	0x2fbfc417,
	0xc3817e27,
	0x30007e2f,
	0x97bffe37,
	0xc8805e07,
	0x86503317,
	0xc8007e07,
	0x53c171d7,
	0x50fbf260,
	0xf55fd787,
	0xab0221f8,
	0x90012117,
	0xc8800407,
	0x98011907,
	0xab06b1d7,
	0x73327009,
	0xe300100f,
	0x98002107,
	0x90000117,
	0x541b0187,
	0xb0266646,
	0xabd4a1f8,
	0x90404117,
	0xf55f8187,
	0xabd3e1f8,
	0x20002117,
	0x2880ff5f,
	0x98001907,
	0xab0331d7,
	0x73327009,
	0xe300100f,
	0x0bd231ff,
	0x58002107,
	0x9020690f,
	0xdc018087,
	0x98205900,
	0x9800b120,
	0x08204900,
	0x383f3900,
	0xabe331ff,
	0x54220087,
	0xb00000ac,
	0xc883fe47,
	0xf83d21f8,
	0x56a200c7,
	0x600000ac,
	0xab0541f8,
	0x54000087,
	0xb0000177,
	0xc885fe47,
	0xf83d21f8,
	0x568000c7,
	0x60000177,
	0xab0841f8,
	0x54080087,
	0xb00002b1,
	0x203d21f8,
	0xf886fe47,
	0x54000087,
	0xb00002ee,
	0xc8877e47,
	0xf83d21f8,
	0x54000087,
	0xb00001f4,
	0xab0781f8,
	0xc8867e47,
	0xf83d31ff,
	0x70dfa000,
	0xc881fe47,
	0xf83d21f8,
	0x56c000c7,
	0x6000003e,
	0xab0601f8,
	0x54600087,
	0xb000005d,
	0xc882fe47,
	0xf83d21f8,
	0x54000087,
	0xb000007d,
	0xc8837e47,
	0xf83d21f8,
	0x54110087,
	0xb0000056,
	0xab02c1f8,
	0xc8827e47,
	0xf83d31ff,
	0x54000087,
	0xb00000fa,
	0xc884fe47,
	0xf83d21f8,
	0x54440087,
	0xb0000158,
	0xc8857e47,
	0xf83d21f8,
	0xc8847e47,
	0xf83d31ff,
	0x54088087,
	0xb000002b,
	0xc880fe47,
	0xf83d21f8,
	0x70dbb800,
	0xc8817e47,
	0xf83d21f8,
	0x70d7d000,
	0xabfd81f8,
	0xc8807e47,
	0xf83d31ff,
	0x2c008487,
	0xbc01858f,
	0x24020497,
	0xb8800047,
	0xf83d21f8,
	0xce010247,
	0xf83d01f9,
	0xc8800247,
	0xce020242,
	0xf83d31ff,
	0x53fc81cf,
	0x60fffffe,
	0x9a1d11cf,
	0x0840b1cf,
	0x3b194fcf,
	0x3c1cb327,
	0xfc1cb737,
	0xc8800797,
	0x3c1cbf57,
	0xfc1cc367,
	0x3c1ccb87,
	0xfc1cbb47,
	0xc18a7347,
	0x3c1cab07,
	0xfc1caf17,
	0x27b40507,
	0xc8800117,
	0xfc1cc777,
	0xc1927377,
	0xd3817e07,
	0x53e171d7,
	0x50fbf263,
	0xb104090f,
	0x3d574387,
	0xe55f918f,
	0xab1d01f9,
	0xabfe71f8,
	0x08149107,
	0x20131907,
	0x0811a107,
	0x20122907,
	0x0813b107,
	0x20153907,
	0x60100fc3,
	0x21947387,
	0x819e737f,
	0xc8827e0f,
	0xf4a003cd,
	0x30d1ffff,
	0xf3010117,
	0xea3f8187,
	0x28d1ffff,
	0xfc008487,
	0x2221017f,
	0xd1808618,
	0xc1808007,
	0x0c008787,
	0xb83f99cf,
	0x08807e00,
	0xf81ff1cf,
	0x244039cf,
	0x2880455f,
	0x20804567,
	0x98807f0f,
	0xc188732f,
	0xe316c37f,
	0xdc00db87,
	0xab1541f8,
	0xf55fdb87,
	0xab01e1f8,
	0xf550d287,
	0xab37e1f8,
	0xf550cc87,
	0xab4021f8,
	0xf550c687,
	0xb8024718,
	0x2180c30f,
	0x818e575f,
	0x24024387,
	0xb1d45967,
	0xabfc71f8,
	0x0e01530f,
	0x9811a107,
	0x0eb0ca07,
	0xd8149107,
	0x0800d807,
	0x28131907,
	0xdc008387,
	0xab3401f8,
	0x20817e07,
	0x98807f5f,
	0x98003107,
	0xce014d7f,
	0xeeb7ca07,
	0x9000d807,
	0xdc008387,
	0xab2c81f8,
	0x20817e07,
	0x98807f0f,
	0x98003907,
	0x25a0472f,
	0xb8807f6f,
	0x90165807,
	0xdc00d987,
	0xab38c1f8,
	0xf55fd687,
	0xab1a61f8,
	0xeeb7cb2f,
	0x9000592f,
	0xdc008187,
	0xab2841f8,
	0xc880570f,
	0x2400d987,
	0xb8807f5f,
	0x232241f8,
	0x58807f67,
	0x56ffc347,
	0x6000001f,
	0xc510420f,
	0x52004308,
	0x70000020,
	0x20806407,
	0x9880fe17,
	0xdc00d687,
	0xb2010807,
	0x08130807,
	0x38148997,
	0x08119007,
	0x38139807,
	0x08152807,
	0x38122007,
	0x0815b007,
	0x3810b807,
	0x08164007,
	0x3800c807,
	0x9815d100,
	0xa7000947,
	0x53c1f1d7,
	0x50fbf264,
	0xf5575187,
	0xabff31f8,
	0x0a1d11cf,
	0x23194fcf,
	0x60940f49,
	0x61ac4649,
	0x65a48f49,
	0x0c1643cf,
	0x3c1747cf,
	0x9c184bcf,
	0x71105019,
	0xf83d31ff,
	0xab0031ff,
	0x90002967,
	0xdc010187,
	0xab0221f8,
	0x884039cf,
	0xf88401c7,
	0xab1a41f8,
	0xdc024787,
	0xab3f41f8,
	0x23eb31ff,
	0x5880431f,
	0x208e7e17,
	0x91b2720f,
	0x26014237,
	0xce01c38f,
	0x50c1695f,
	0x20000002,
	0x7020ae65,
	0xeea2c41f,
	0xc8800607,
	0x9a41c1cf,
	0x864ff9e7,
	0x50c1695f,
	0x20000002,
	0x21bc7227,
	0x81b27207,
	0xeeb8880f,
	0x53f871d7,
	0x50fbf25f,
	0x09bc720f,
	0x8241c1cf,
	0xe2e0038f,
	0x56ff81c7,
	0x60000001,
	0xabe4e1f8,
	0x218c7217,
	0x88800607,
	0x2b18c217,
	0xc3817e27,
	0x50c0e95f,
	0x20000002,
	0x30007e2f,
	0x97bffe37,
	0xe7bfe217,
	0x864ff9cf,
	0xc8007e07,
	0x53f731d7,
	0x50fbf25e,
	0x9000018f,
	0x541b0187,
	0xb0266646,
	0xabe0a1f8,
	0x9040418f,
	0xea36c387,
	0xf55f8087,
	0xab4aa1f8,
	0xdc008187,
	0xe830cd30,
	0x33def1ff,
	0x5831c318,
	0xf55fc287,
	0xab0241f8,
	0x2400d987,
	0xb880ff5f,
	0xab2101f8,
	0x23887f67,
	0xa8807f0f,
	0x51007e0f,
	0x90ffffe0,
	0xabe771ff,
	0x2400d987,
	0xb8817f5f,
	0xabfe71f8,
	0x20d47e0f,
	0x91b27387,
	0x2600c617,
	0xc8827f5f,
	0x23887f0f,
	0xa8807f67,
	0x26a1457f,
	0xd880600f,
	0x20d1ffff,
	0xf18c041f,
	0x3001297f,
	0x2ea1c407,
	0x864ffaef,
	0x3001297f,
	0x27b2e007,
	0x53e931d7,
	0x50fbf25f,
	0x900021cf,
	0x56ff80c7,
	0x6000000f,
	0xab01e1f8,
	0xc810fe07,
	0x53eb71d7,
	0x50fbf25c,
	0xc83afe07,
	0x53eab1d7,
	0x50fbf25c,
	0xc8d47e17,
	0xe601460f,
	0xeea0c517,
	0x90002917,
	0xdc010087,
	0xabdd21f8,
	0xf55fda87,
	0xabdca1f8,
	0xdf76c347,
	0x33dc31ff,
	0x5836c308,
	0x90004107,
	0xdc008087,
	0xab0121f8,
	0x90005107,
	0xdc008187,
	0xabe4e1f8,
	0xc880fe07,
	0x0be471ff,
	0x58004907,
	0xdf75c347,
	0x33d831ff,
	0x5835c308,
	0x9000b007,
	0xdc008387,
	0xab1801f8,
	0x20d47e0f,
	0x91b27367,
	0x0e00cc17,
	0xc0153907,
	0x26a1450f,
	0xd880580f,
	0x20d1ffff,
	0xf18c0407,
	0x3001290f,
	0x2ea04407,
	0x864ff64f,
	0x3001290f,
	0x27b2d807,
	0x53dc71d7,
	0x50fbf25f,
	0x0bd031ff,
	0x5010a1cf,
	0x23c9b1ff,
	0x58827f4f,
	0x9000b007,
	0xdc008387,
	0xab2081f8,
	0x0813b107,
	0x20122907,
	0x20d47e27,
	0x91b27367,
	0x26025217,
	0xc880fe0f,
	0x0eb0cb0f,
	0xd800c107,
	0xc880580f,
	0xeea1455f,
	0x20d1ffff,
	0xf18c041f,
	0x3001295f,
	0x2ea1c407,
	0x864ff57f,
	0x3001295f,
	0x27b2d807,
	0x53d5f1d7,
	0x50fbf25f,
	0x0800310f,
	0x2015a1cf,
	0xdc008187,
	0xabc6e1f8,
	0x9000110f,
	0x90405007,
	0xf55f0087,
	0xabc5e1f8,
	0xce010007,
	0x33c571ff,
	0x5f00575f,
	0x23c0f1ff,
	0x58827f37,
	0x20d47e0f,
	0x91b27387,
	0xe600c617,
	0x26a1456f,
	0xd880600f,
	0x20d1ffff,
	0xf18c0407,
	0x3001296f,
	0x2ea04407,
	0x864ff7af,
	0x3001296f,
	0x27b2e007,
	0x53cf31d7,
	0x50fbf25f,
	0x0bc431ff,
	0x5016a1cf,
	0x20d47e0f,
	0x91b2735f,
	0xe600c617,
	0x26a1450f,
	0xd880560f,
	0x20d1ffff,
	0xf18c041f,
	0x3001290f,
	0x2ea1c407,
	0x864ff72f,
	0x3001290f,
	0x27b2d607,
	0x53cb31d7,
	0x50fbf25f,
	0x9000290f,
	0xdc010187,
	0x0b0901f8,
	0x500021cf,
	0x56ff8147,
	0x6000000f,
	0x52000108,
	0x70000010,
	0x2081ff5f,
	0x98807f67,
	0xabe231ff,
	0x20109007,
	0x28d47e0f,
	0x31b2736f,
	0x8600cc17,
	0x9035490f,
	0x30805a0f,
	0x9ea14567,
	0x20d1ffff,
	0xf18c0407,
	0x30012967,
	0x2ea04407,
	0xc310541f,
	0xdc11d487,
	0xda8087d0,
	0xb8015550,
	0x864ff32f,
	0x30012967,
	0x27b2da07,
	0x53c371d7,
	0x50fbf25f,
	0x9040510f,
	0x9010a1cf,
	0xf55f0087,
	0xabb661f8,
	0xce010007,
	0x33b5f1ff,
	0x5f00430f,
	0xf55f8087,
	0xab0221f8,
	0x904231cf,
	0x9000a027,
	0xdc008387,
	0xabf661f8,
	0x56ff81c7,
	0x6000000f,
	0xabf581f8,
	0x2080ff5f,
	0x98807f0f,
	0x2bd871ff,
	0x53887f67,
	0xc8d47e0f,
	0xe600c21f,
	0x218c0617,
	0x81a8720f,
	0xeea14407,
	0x864db7df,
	0x20012967,
	0x21a87207,
	0xc8804a0f,
	0x53fbb1d7,
	0x50fbf25e,
	0x900021cf,
	0x56ff80c7,
	0x60000017,
	0x33a8b1ff,
	0x5831c318,
	0x90159007,
	0x9033c95f,
	0x08d1ffff,
	0xf020395f,
	0xc3104e0f,
	0x1c11ce87,
	0xb68300cf,
	0xda8083d0,
	0xb8014f38,
	0xab0621f9,
	0x268200c7,
	0x68827f27,
	0xab0261f8,
	0x24008087,
	0xb880ff27,
	0xab01a1f8,
	0xf55f8087,
	0xab00e1f8,
	0xb68101c7,
	0xab0601f8,
	0xc8817f27,
	0x9020415f,
	0xdc008087,
	0x5080095f,
	0x20000001,
	0xc180c920,
	0xdc010187,
	0xab0901f8,
	0xdc008187,
	0xab05c1f8,
	0x0813b107,
	0x38122907,
	0xabd871ff,
	0xdc048087,
	0xab0421f8,
	0xb68480c7,
	0xab0261f8,
	0xdc038087,
	0xab0321f8,
	0xdc040087,
	0xabfb01f8,
	0x23fb31ff,
	0x58837f27,
	0x23fab1ff,
	0x5881ff27,
	0xdc110087,
	0xab0121f8,
	0xdc128087,
	0xabf901f8,
	0xabfe31ff,
	0x23f8f1ff,
	0x5882ff27,
	0xc302c807,
	0xb68081c7,
	0xabfa21f8,
	0x2010fe07,
	0x98817f27,
	0x53efb1d7,
	0x50fbf25b,
	0xc8187e07,
	0x53eef1d7,
	0x50fbf25b,
	0xabf831ff,
	0xc302c807,
	0xb68081c7,
	0xabf761f8,
	0x2010fe07,
	0x98837f27,
	0x53ecf1d7,
	0x50fbf25b,
	0xc818fe07,
	0x53ec31d7,
	0x50fbf25b,
	0x5080095f,
	0x20000001,
	0xabf471ff,
	0x239531ff,
	0x5880434f
};
const uint32_t BDSP_IMG_mixer_ids_header [2] = {sizeof(BDSP_IMG_mixer_ids_array1), 1};
const void * const BDSP_IMG_mixer_ids [2] = {BDSP_IMG_mixer_ids_header, BDSP_IMG_mixer_ids_array1};
const uint32_t BDSP_IMG_mixer_ids_inter_frame_array1[] = {
	0xffffffff,
	0x00000002,
	0x00000004,
	0x00000003,
	0x00000002,
	0x00000003,
	0x00000000,
	0x00000003
};
const uint32_t BDSP_IMG_mixer_ids_inter_frame_header [2] = {sizeof(BDSP_IMG_mixer_ids_inter_frame_array1), 1};
const void * const BDSP_IMG_mixer_ids_inter_frame [2] = {BDSP_IMG_mixer_ids_inter_frame_header, BDSP_IMG_mixer_ids_inter_frame_array1};
/* End of File */
