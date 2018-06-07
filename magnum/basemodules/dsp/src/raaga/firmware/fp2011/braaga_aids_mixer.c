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

#include "bchp.h"
const uint32_t BDSP_IMG_mixer_ids_array1[] = {
	0xd91ff741,
	0x960475cb,
	0xba1cc09f,
	0xba1cc4af,
	0x46000307,
	0x30017e07,
	0xba1cc8bf,
	0xba1ccccf,
	0xba1cd0df,
	0xba1cd4ef,
	0xba1cd8ff,
	0xba1cdd0f,
	0xa75e8dd7,
	0x001df934,
	0x1b5f9183,
	0xb42d85fc,
	0x07a47f17,
	0x2b00fe1f,
	0x3dbe4007,
	0x3daa420f,
	0x2b2c7e17,
	0x94320707,
	0x94000707,
	0x940e7f07,
	0x940c7f07,
	0x94107d07,
	0x94347d07,
	0x94307f07,
	0x942e7f07,
	0x94127f07,
	0x94147f07,
	0x94167f07,
	0x94187f07,
	0x941a7f07,
	0x941c7f07,
	0x941e7f07,
	0x3de0433f,
	0xa7420dd7,
	0x001df940,
	0x3db67337,
	0x07bffe17,
	0x46134e07,
	0x2b167e1f,
	0x461ffe27,
	0x07bffe37,
	0x3da6734b,
	0xc694063b,
	0x07bffe03,
	0xa72b8dd7,
	0x001df932,
	0xfe074e8b,
	0x2b107e1f,
	0x461ffe27,
	0x07bffe33,
	0xd88845a0,
	0xc6941db3,
	0x07bffe03,
	0xa72989d7,
	0x001df932,
	0x7454490f,
	0x00000009,
	0xd8204f01,
	0x2b1e7e13,
	0x3d984803,
	0xa73b89d7,
	0x001df940,
	0x743601cf,
	0x94067d07,
	0x3d824923,
	0x1b5f0183,
	0xb42805f8,
	0x94080103,
	0x3dde4367,
	0x07b0ff47,
	0xfe3f0f5b,
	0x07bfff5f,
	0x2b0e7f77,
	0x3d907357,
	0x3d8c736f,
	0x64020163,
	0x15008183,
	0xb41d85f8,
	0x3d80cb2f,
	0x3dd45147,
	0x15024b87,
	0x3d8e471f,
	0xb6fe09f8,
	0x07b17e03,
	0xa7078dd7,
	0x001df935,
	0x74060107,
	0x74080307,
	0x7456050b,
	0xa73c89d3,
	0xd8684d41,
	0x4614ce07,
	0x940a1107,
	0x07b37e13,
	0xa73d8dd3,
	0xd8084f01,
	0x740c0203,
	0x15400387,
	0x000000bb,
	0xb41e01f8,
	0x744801cb,
	0x15008183,
	0xb60301f8,
	0x744a01cf,
	0x744603cf,
	0xd805dc02,
	0x94487fcf,
	0x94367dcf,
	0x94080507,
	0x94060507,
	0x013fffff,
	0x15008247,
	0x15008047,
	0x94467fc8,
	0x944a7fc8,
	0x74061107,
	0x74080f07,
	0xd8084e61,
	0x2b00fe0b,
	0x941c03cb,
	0x942403cf,
	0x941a0fcf,
	0x15020187,
	0x942211cf,
	0xb41701f8,
	0xd0018190,
	0x21010e01,
	0x941e01c9,
	0x941e0fc8,
	0x743601cb,
	0x1b400e43,
	0xb60405f8,
	0xd8204de1,
	0x7400070f,
	0x461ffe37,
	0xd8084dc1,
	0x2b04fe17,
	0x3d9a7227,
	0x2b0c7e2f,
	0x94087fcf,
	0xc6937b63,
	0x2a927e03,
	0xa74189d7,
	0x001df931,
	0x2aa97e03,
	0xa7408dd7,
	0x001df931,
	0x74061107,
	0x74080f07,
	0x741a010b,
	0x1b5f8183,
	0xb60085f8,
	0xd8204da1,
	0x15008383,
	0xb41b85f8,
	0x7400130f,
	0x46134e07,
	0xf611fed3,
	0x461ffe27,
	0x07bffe37,
	0x944e7d07,
	0x94360fcf,
	0x94541307,
	0x943811cf,
	0xc694037b,
	0x07bffe03,
	0xa70d0dd7,
	0x001df932,
	0x740c0127,
	0x741a030f,
	0xd0008190,
	0x2a96fe05,
	0x94140327,
	0x2a95fe04,
	0x94327f01,
	0x94007f01,
	0xa73909d7,
	0x001df931,
	0x760475cf,
	0x740e0127,
	0x540ac5cf,
	0x540bc9cf,
	0x540ccdcf,
	0x540dd1cf,
	0x013fffff,
	0x1b5f81b7,
	0x1b5f8047,
	0x943a7f06,
	0x540ed5cf,
	0x540fd9cf,
	0x2b00fe00,
	0x943a0104,
	0x5410ddcf,
	0x5409c1cb,
	0xd91008c1,
	0xa9fd0ffb,
	0xb6ff8dfb,
	0x40574a17,
	0x746a0147,
	0x07b57e0b,
	0x3dbc057f,
	0x00000003,
	0x0857c37f,
	0x15010047,
	0xb6e001fc,
	0x07b7fe07,
	0xd8401ba3,
	0xc69409fb,
	0xd8401ba3,
	0x4616d403,
	0xa7570dd7,
	0x001df932,
	0xd707f008,
	0xb40485f8,
	0x743801cb,
	0xb6dd0dff,
	0x94060107,
	0x21020e03,
	0xb6e90dff,
	0x941e01cf,
	0x740e0203,
	0x15400387,
	0x000000bb,
	0xb6e101f8,
	0x740a0003,
	0xd8a5dc00,
	0xb4e005f8,
	0xb6e38dfb,
	0xa70b09d3,
	0xd8204f01,
	0x8444000b,
	0xb6d68dff,
	0x94080107,
	0xd8201ba3,
	0x461fc417,
	0x30017e27,
	0xfe47ffab,
	0x07b7fe03,
	0xc694099b,
	0x07bffe03,
	0xa77a8dd7,
	0x001df931,
	0x1b5fd783,
	0xb60185f8,
	0x74080513,
	0x07a17e03,
	0x94060503,
	0xa7060dd3,
	0xd8204f01,
	0x8444000b,
	0x94080103,
	0x74000113,
	0x151b0187,
	0x00266646,
	0xb6d385f8,
	0x74500113,
	0x1b5f8183,
	0xb6d301f8,
	0x74080117,
	0x2b00ff5f,
	0x94060103,
	0xa7028dd3,
	0xd8204f01,
	0x8444000b,
	0xb6d109ff,
	0x94080107,
	0x743a010b,
	0x15018043,
	0x94360100,
	0x940c0324,
	0x94320100,
	0xb6e289ff,
	0x942e7d00,
	0xd8a56220,
	0xa9fd07fc,
	0x2b03fe47,
	0xdab56220,
	0xb40501f8,
	0x15000047,
	0x00000177,
	0xa9fd07fc,
	0x2b05fe47,
	0x11000087,
	0x00000177,
	0xb40701f8,
	0x15080047,
	0x000002b1,
	0xa9fd07fc,
	0x2b06fe47,
	0x15000047,
	0x000002ee,
	0xa9fd07fc,
	0x2b077e47,
	0x15000047,
	0x000001f4,
	0xb40601f8,
	0xa9fd0fff,
	0x2b067e47,
	0xd8a1f400,
	0xa9fd07fc,
	0x2b01fe47,
	0xdab1f400,
	0xb40485f8,
	0xd8a2ee00,
	0xa9fd07fc,
	0x2b02fe47,
	0xd8a3e800,
	0xa9fd07fc,
	0x2b037e47,
	0xd8a2b110,
	0xb40205f8,
	0xa9fd0fff,
	0x2b027e47,
	0xd8a7d000,
	0xa9fd07fc,
	0x2b04fe47,
	0x15440047,
	0x00000158,
	0xa9fd07fc,
	0x2b057e47,
	0xa9fd0fff,
	0x2b047e47,
	0xd8a15888,
	0xa9fd07fc,
	0x2b00fe47,
	0xd8a17700,
	0xa9fd07fc,
	0x2b017e47,
	0xd8a0fa00,
	0xb4fd81f8,
	0xa9fd0fff,
	0x07bffe47,
	0x15008447,
	0x1501858f,
	0x15020477,
	0x07a07e47,
	0xa9fd07f8,
	0xa9dd07fd,
	0x21010247,
	0x07a0fe43,
	0xa9fd0fff,
	0x21020246,
	0xd91ff601,
	0x960475cb,
	0x944c03cf,
	0x9713e5cf,
	0xba1cc8cf,
	0xba1cccdf,
	0x07a1ff93,
	0xba1cd4ff,
	0xba1cd90f,
	0xba1ce12f,
	0xba1cd0ef,
	0x3d8a7343,
	0xba1cc0af,
	0xba1cc4bf,
	0x46140507,
	0x07a07f17,
	0xba1cdd1f,
	0x3d927377,
	0x30017e03,
	0xa70689d7,
	0x001df934,
	0x8c02110b,
	0x1b574387,
	0x1b5f918f,
	0xb41d05f9,
	0xb6fe09f8,
	0x74045307,
	0x74064d07,
	0x74084707,
	0x740a4907,
	0x740c4f07,
	0x740e5507,
	0xfe07fe1b,
	0x3d947387,
	0x3d9e737f,
	0x2b027e0b,
	0xa801f40b,
	0x013fffff,
	0x84400517,
	0x9e407f83,
	0x013fffff,
	0x15008447,
	0x9e40057f,
	0x3d808618,
	0x3d808003,
	0x15008787,
	0x94267fcf,
	0xa09f8e04,
	0x941c7fcf,
	0x904e01cf,
	0x07b17f5f,
	0xffbf1663,
	0x3d88732b,
	0x8450db7b,
	0x1500db83,
	0xb41585f8,
	0x1b5fdb83,
	0xb60181f8,
	0x1b50d243,
	0xb63605f8,
	0x1b50cc43,
	0xb63e01f8,
	0x1b50c643,
	0x5f024718,
	0x3d80c30f,
	0x3d8e575f,
	0x15024387,
	0x3dd45967,
	0xb6fc09f8,
	0x2101530f,
	0x94084707,
	0x0850ca07,
	0x94045307,
	0x74160207,
	0x94064d07,
	0x15008383,
	0xb43281f8,
	0x2b017e07,
	0x07bfff5f,
	0x940c0103,
	0x21014d7b,
	0x0857ca03,
	0x74160203,
	0x15008383,
	0xb42b81f8,
	0x2b017e07,
	0x07bfff0f,
	0x940e0103,
	0x2101460f,
	0x07bfff6f,
	0x0840ca03,
	0x74165803,
	0x1500d983,
	0xb43681f8,
	0x1b5fd643,
	0xb61a01f8,
	0x0857cb2b,
	0x7416012b,
	0x15008183,
	0xb42701f8,
	0x07b5ff0b,
	0x1500d987,
	0x07bfff5f,
	0xb42101fc,
	0x07bfff67,
	0x117fc387,
	0x0000001f,
	0x3990420b,
	0x5d00430c,
	0x00000020,
	0xe0139683,
	0x1500d643,
	0x8d020407,
	0x94025397,
	0x94024c03,
	0x94044607,
	0x94064e07,
	0x940a5407,
	0x94084807,
	0x940c5607,
	0x940e4207,
	0x94105807,
	0x94120207,
	0x94145700,
	0x64020143,
	0xa72809d7,
	0x001df934,
	0x1b575183,
	0xb6fe8df8,
	0x760475cf,
	0x7713e5cf,
	0x540ac1cf,
	0x540bc5cf,
	0x540cc9cf,
	0x540dcdcf,
	0x540ed1cf,
	0x540fd5cf,
	0x5410d9cf,
	0x5411ddcf,
	0x5412e1cb,
	0xd9100a01,
	0xa9fd0ffb,
	0xb6ff8dfb,
	0x740a0163,
	0x15010183,
	0xb60185f8,
	0x704e01cb,
	0xa1000fc3,
	0xb41881f8,
	0x15024783,
	0xb43b85f8,
	0xb6e98dff,
	0x07b0ff1f,
	0x2b0e7e17,
	0x3db2720f,
	0x40414237,
	0x2101c38f,
	0x745a055f,
	0x00000002,
	0xdd9015c1,
	0x0842c41b,
	0x07a1fe03,
	0x965007cb,
	0xc69400ab,
	0x745a055f,
	0x00000002,
	0x3dbc7227,
	0x3db27207,
	0x0858880b,
	0xa70c09d7,
	0x001df932,
	0x3dbc720f,
	0x765007cf,
	0x8400818b,
	0x117f8107,
	0x00000001,
	0xb6e385f8,
	0xe126c611,
	0x8450e217,
	0x30017e27,
	0x745a035f,
	0x00000002,
	0xfe47ffab,
	0x461fe213,
	0xc694009b,
	0x07bffe03,
	0xa7328dd7,
	0x001df931,
	0x7400018b,
	0x151b0187,
	0x00266646,
	0xb6e001f8,
	0x7450018b,
	0x9e50db83,
	0x1b5f8043,
	0xb64705f8,
	0x15008183,
	0x03f0cd30,
	0xb6de09ff,
	0x03f1c318,
	0x1b5fc243,
	0xb40185f8,
	0x1500d987,
	0x2b00ff5f,
	0xb41e81f8,
	0x30087f67,
	0x07bfff0f,
	0xdbdf0001,
	0xb6e70dfb,
	0x1500d987,
	0x2b017f5f,
	0xb6fe0df8,
	0x2b547e0f,
	0x3db27387,
	0x4040c617,
	0x2b027f5f,
	0x30087f0f,
	0x07bfff67,
	0x0841457f,
	0x07b87e0f,
	0x740a057f,
	0x3d8c041f,
	0xe15ffc3b,
	0xc694025b,
	0x740a057f,
	0x4612e007,
	0xa77d8dd7,
	0x001df931,
	0x740801cb,
	0xdab07ff8,
	0xb60181f8,
	0x2a90fe03,
	0xa75589d7,
	0x001df930,
	0x2abafe03,
	0xa7548dd7,
	0x001df930,
	0x2b547e13,
	0x4041460b,
	0x0840c513,
	0xd7f0a810,
	0xb6dd85f8,
	0x1b5fda43,
	0xb6dd05f8,
	0x1a56c383,
	0xb6dc89ff,
	0x03f6c308,
	0x74100103,
	0x15008043,
	0xb60085f8,
	0x74140103,
	0x15008183,
	0xb6e601f8,
	0x2b00fe03,
	0xb6e50dff,
	0x94120107,
	0x1a55c383,
	0xb6d889ff,
	0x03f5c308,
	0xd6e87081,
	0xb41681f8,
	0x2b547e0f,
	0x3db27367,
	0x4040cc17,
	0x740e5507,
	0x0841450f,
	0x07b67e0f,
	0x740a050f,
	0x3d8c0407,
	0xe159fc3b,
	0xc693fd53,
	0x740a050f,
	0x4612d807,
	0xa77209d7,
	0x001df931,
	0xb6d08dff,
	0x740843cf,
	0xb6ca0dff,
	0x2b027f4f,
	0xd6e87081,
	0xb41f01f8,
	0x740c4f07,
	0x740a4907,
	0x2b547e27,
	0x3db27367,
	0x40425217,
	0x2b00fe0f,
	0x07b67e0f,
	0x94100307,
	0x0850cb0b,
	0x0841455b,
	0x740a055f,
	0x3d8c041f,
	0xe15ffc3b,
	0xc693fc93,
	0x740a055f,
	0x4612d807,
	0xa76c09d7,
	0x001df931,
	0x740c010f,
	0x740857cf,
	0x15008183,
	0xb6c805f8,
	0x7404010b,
	0x74540003,
	0x1b5f0043,
	0xb6c705f8,
	0x21010003,
	0xb6c689ff,
	0x0a40575f,
	0xb6c209ff,
	0x2b027f37,
	0x2b547e0f,
	0x3db27387,
	0x4040c613,
	0x0841456f,
	0x07b87e0f,
	0x740a056f,
	0x3d8c0407,
	0xe159fc3b,
	0xc693ff53,
	0x740a056f,
	0x4612e007,
	0xa76589d7,
	0x001df931,
	0xb6c58dff,
	0x74085bcf,
	0x2b547e0f,
	0x3db2735f,
	0x4040c613,
	0x0841450f,
	0x07b5fe0f,
	0x740a050f,
	0x3d8c041f,
	0xe15ffc3b,
	0xc693fedb,
	0x740a050f,
	0x4612d607,
	0xa7618dd7,
	0x001df931,
	0x740a010b,
	0x15010187,
	0x740801cf,
	0xb40805f8,
	0x117f8187,
	0x0000000f,
	0x5d00010c,
	0x00000010,
	0x2b01ff5f,
	0x07bfff67,
	0xb6e389fb,
	0x74044207,
	0x2b547e0f,
	0x3db2736f,
	0x4040cc17,
	0x7432550b,
	0x07b6fe0f,
	0x08414567,
	0x3d8c0407,
	0x740a0567,
	0x08404403,
	0x3f90541b,
	0x1511d443,
	0x13008700,
	0x5f015550,
	0xc693fa5b,
	0x740a0567,
	0x4612da07,
	0xa75a0dd7,
	0x001df931,
	0x7454010f,
	0x740843cf,
	0x1b5f0043,
	0xb6b881f8,
	0x21010003,
	0xb6b78dff,
	0x0a40430f,
	0x1b5f8043,
	0xb60185f8,
	0x744c09cb,
	0x74080223,
	0x15008383,
	0xb6f605f8,
	0x117f8107,
	0x0000000f,
	0xb4f581f8,
	0x2b00ff5f,
	0x07bfff0f,
	0xb6d98dff,
	0x30087f67,
	0x2b547e0b,
	0x4040c21b,
	0x3d8c0617,
	0x3da8720f,
	0x08414403,
	0xc69377eb,
	0x740a0567,
	0x3da87207,
	0x07b2fe0b,
	0xa75289d7,
	0x001df931,
	0x740801cb,
	0xdab0bff8,
	0xb6aa8dff,
	0x03f1c318,
	0x74045603,
	0x74324f5b,
	0x013fffff,
	0x742e015f,
	0x3f904e0b,
	0x1511ce47,
	0x1103008f,
	0x13008300,
	0x5f014f38,
	0xb60585f9,
	0x11020087,
	0x2b027f27,
	0xb60201f8,
	0x15008047,
	0x2b00ff27,
	0xb60105f8,
	0x1b5f8043,
	0xb60081f8,
	0x11010103,
	0xb40585f8,
	0x2b017f23,
	0x7430015b,
	0x15008043,
	0x7402015f,
	0x00000001,
	0x3d80c920,
	0x15010183,
	0xb40885f8,
	0x15008183,
	0xb40581f8,
	0x940c4f07,
	0x940a4907,
	0xb6d90dfb,
	0x15048043,
	0xb60385f8,
	0x11048083,
	0xb60201f8,
	0x15038043,
	0xb60285f8,
	0x15040043,
	0xb4fa85f8,
	0xb6fa89ff,
	0x2b037f27,
	0xb6fa09ff,
	0x2b01ff27,
	0x15110043,
	0xb60085f8,
	0x15128043,
	0xb4f885f8,
	0xb6fd8dfb,
	0xb6f80dff,
	0x2b02ff27,
	0x3f82c803,
	0x11008103,
	0xb6f985f8,
	0x2a90fe07,
	0x2b017f27,
	0xa71c09d7,
	0x001df930,
	0x2a987e03,
	0xa71b0dd7,
	0x001df930,
	0xb6f78dfb,
	0x3f82c803,
	0x11008103,
	0xb6f701f8,
	0x2a90fe07,
	0x2b037f27,
	0xa7190dd7,
	0x001df930,
	0x2a98fe03,
	0xa71889d7,
	0x001df930,
	0x7402015f,
	0x00000001,
	0xb6f409fb,
	0xb6970dff,
	0x07b0ff4f
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
