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

const uint32_t BDSP_ARM_IMG_idle_process_array1[] = {
0x21415a54,0x40107,0x49000101,0x1,0x2c000001,0x14001402,0x501e0000,0x501e0000,0x0,0x0,0x0,0x142f0000,0x10001,0x100,0x0,0x0,
0x20004,0x203,0xa0557200,0x505f425a,0x525f4552,0x41454c45,0x4553,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x69ef0de,0x2c9dea17,0xcd92c815,0x93b76bae,0x66f49aa5,0xf90b18f0,0x2cff6a70,0xbd682171,
0x3cab5196,0xd075ca0d,0xfe63d29,0x442b4079,0x0,0x0,0x0,0x0,0x1,0x45444342,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x926be5ef,0x4cd39708,0x7a0c0b15,0xc1263e4d,0x680c7859,0x62afe204,0xf13bd97d,0x8f0da3ab,0xc2c5ecc2,0xd12f0c5e,0xbdbaad62,0x16cd491e,0x408c4594,0x1177dfb8,0xb45a9b8,0xf690fceb,
0xf19fa5d,0x3787410e,0x1da8d,0x77da04c5,0xe080dbae,0xdb57325b,0x8af4480b,0xed6da27e,0x2976c2d8,0x88370394,0xd8e58b2e,0x590bd4f4,0xed54e3,0xcf7b6b2e,0xe817086d,0x109e931c,
0x561abb84,0x8d7a9075,0x2428fc5f,0x5f026685,0x5079e723,0x370fa865,0xf603303c,0xd9fd2334,0x59817ee3,0xe9ec9f96,0xf1203f47,0xe2db12d6,0x2f2720e0,0x90649de4,0xe89b9a99,0xbfa272f0,
0xeff4dfe7,0x2eb2ca8e,0xfd963781,0x18587945,0x3a70fb63,0xb16b1bf4,0x26dfa0b3,0x438a14a2,0xe690c945,0xf7c8007c,0x27d640b9,0x96f54f94,0xcfceba23,0xa54fdda0,0x8029354b,0xa63f673a,
0x10000100,0x10000,0xfc0000,0x1000000,0x1010000,0x184a9ede,0x57c1d713,0x63f44c50,0xb98e6b01,0xe68b7035,0xd61e7972,0xa821c34d,0xa9252323,0x5cd82c3a,0x9849111,0x316b0f34,
0x937df1f0,0x2b88e917,0xd879f4f0,0x628322c1,0xdd16f222,0x2492e29a,0xde90195f,0xc2e103a7,0x8f5035c7,0x33404b7,0x46437b75,0xb56549b1,0x2e7ea7a3,0x192c085d,0xb8a09c07,0xec04d97a,
0xe7e060a,0x49218690,0x921f27d9,0x83d54cda,0xcb8288cb,0x85b70897,0x7a49dfc3,0x8240e0b4,0x1213e0eb,0xb585c6dc,0xa6ad9201,0xe55d6774,0xc9990c21,0x31663eaa,0x9bd3e68c,0x29a9693b,
0x6baa2225,0x7908fe2b,0x9e481230,0x13cca261,0x1cfe16b8,0xe8be9e09,0x387898f8,0xda5ed645,0x209ee01b,0xc4ce0192,0x3f6ae5f2,0x863ab88d,0x31ccc570,0x97cd3cf8,0xfb1dc276,0xc23e6641,
0x83d1fbac,0xe63ec22c,0x1377700,0x73a31b6c,0x14daa22,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8a4b6571,
0x2b1ea687,0x1962c382,0x39b32ad9,0x463227b4,0xa13528f9,0x7dd39012,0x5bf970a,0x246eb794,0x668d185a,0x9f933a13,0x3ea46198,0xddfa5246,0xc6d0d924,0x9768b693,0xe4a75173,0x3b24faf1,
0xdd3e1fb,0x6222495b,0xf396e62a,0x98ba3313,0x55ce6792,0x8bb99807,0x906149dd,0x8da6bd0d,0x7f3c8c15,0x6a57d5cf,0xfd978711,0xf4f7e372,0x289531b9,0x89a9471d,0x43292848,0xdf006379,
0xf724a14d,0xce83b6d,0x9f321265,0x7d20e6f0,0xdde62fe4,0x9d7713bd,0x62056be0,0xcf25894,0x7631d6ec,0x7af3f48d,0x44d75869,0x4c7d244f,0xc2fad979,0x981276c,0x7bd71f44,0xdf89f430,
0xe1fc830e,0x55145413,0x5e23417c,0x897243c2,0x70bb06b3,0x4cacf662,0x5de36e01,0x195acd34,0x4b593469,0xd334e863,0x1043ccf,0x52d89fc1,0xca9571c4,0x3e91a82e,0xe10d49fd,0x38ab95bc,
0xe3f74a3,0x412adbbe,0x49aec942,0xd568a3b8,0x713f5cff,0x542cdf6c,0x704dc0c8,0x5e25920d,0x556670a8,0xc2c1499f,0x963c2994,0x42826c23,0x2754fa21,0x25443208,0x430fe9aa,0x834c6a94,
0x20aa43fb,0x452ca8aa,0xda04e2f4,0xc542e88c,0x9713fca9,0x2ce52b23,0x6d2e8f6d,0x6926258d,0x515a2684,0x158a5c85,0x2b003800,0x6f09b104,0x8d47c2d3,0x57c21724,0x4085f119,0x933491d,
0x6f3c5fb8,0xdc6a4a2f,0x73c26765,0xf4acb83e,0x823a5e77,0x114c7f96,0x6e9e7bcf,0x914cdbb4,0x85949d27,0xaa1864a6,0xfd973c4,0x8f2e0c70,0x44e36b25,0xfc3fc5a3,0x53d162f1,0xd5530aca,
0x395e1977,0xfd8c8067,0xc138cb6,0xcac0067a,0x18e29337,0x616f9082,0x84b55361,0xfec2053a,0xbbdcc8b3,0xbf8a2aa8,0xa9aaf311,0x5efbb1b6,0x672a0be,0xfd9249b8,0x1b17a6be,0xb65f9255,
0x75bf4f2c,0xcddf0b02,0xfd5f7615,0x745f74d4,0x594173af,0xf6b70fb4,0x6dc4f8e8,0x2bcb3f7b,0x1a6209ad,0x24ea2ec,0xe29ffce2,0x4057c7b4,0x4934679a,0x366586e3,0x82b592ef,0x25cc7ec8,
0x63ecdca8,0x3c8ccdcf,0xee47c9ad,0x48691284,0x32508832,0x7f115e81,0x5ec4d1d6,0xa83bfe64,0x577b41a0,0x941b88af,0xbdac7142,0xa7fb928e,0x67f70ac0,0x4881b3e5,0x6fd9db5b,0x58ffa8b,
0x55959aef,0x3be27dad,0x91f18299,0xd23d371e,0xb499fc11,0xfaafb702,0xb2620a7f,0x236bdadb,0xf05e39f0,0xef94f160,0xefb52b2c,0xf29b29,0xa2476c10,0xd729fa19,0xe2f3be0a,0x6b478a1,
0x7deb2207,0xad6bbd15,0x40b79583,0x31249e6b,0xf0778ca3,0xa7342ed3,0xa4acdaaa,0x16c9de32,0xce0d1368,0xce227272,0x9f5166d7,0xdcfba1f,0xbcb7488,0xbc459f4b,0x4d74f3fc,0xa2f8b880,
0x11f201d2,0x132b84c,0xb5ed9fda,0xa02101bc,0x55341f43,0xc6a1f4cf,0xa8b9ec16,0xf4c8a52,0x5c626825,0xbc1db9c4,0xce8009e9,0x4c80b9c0,0x4cb37240,0xa34666c4,0x4fa60ff6,0x56558a1a,
0x5a4feaa0,0x4ab06c23,0x88153c6e,0xcfefd7a7,0x85d195e5,0xa4e9f5f0,0x8942e46c,0x36900d9e,0x841d0124,0xfd298a1b,0xd024ecf1,0xa1eb5365,0x7f76016a,0x748d20cf,0x33c50c2c,0x1e8f4b99,
0xfa66ceb,0x6fea54fb,0x567c3f70,0x4c883c7,0x3b61ee96,0x906d6e7d,0xaec36af4,0x32bf28c,0xc813b6cf,0xa94d4b60,0xeab86e2a,0x6e38514d,0x3d025656,0xc77c65ff,0x4f8a57b6,0x2d282909,
0xc5277e51,0x58814c40,0xc4b8ef8c,0x2eff37c7,0xddfd7f90,0x632fb21a,0xa6d09cb1,0xbc7696e1,0x5597243e,0x1271198f,0x4f8efd0c,0xf06107d,0xdd62df8b,0xc8427ed,0xc5b8abcf,0x19b0ef6e,
0x264452ab,0x777cf8b6,0x2f9a9ba0,0x1c89f790,0xb9c2f385,0xf7dfc60e,0x8deb0410,0x6267a31,0x7dedfdf7,0xc6fa58ac,0x9bc4c9b2,0x53692f42,0x78be8c61,0x2e8e9010,0x2ac0e041,0x63439151,
0xb6e4d87a,0x6e6fdb5b,0xb7fc7f23,0xab1ec6e3,0x84ff7942,0x71fc2301,0xec202090,0x47284a3e,0x8a4545fd,0xf4d1310a,0x4118be4c,0x74023717,0xc4f1ec47,0xf4f5c190,0xf4be4155,0x77f87919,
0x72ff19b4,0x1bd905e3,0x10da16d3,0xb417c844,0xdf1b1a68,0xeb6df1f,0xc7d4ba83,0x3b729ebc,0x5bbe6174,0x76df80ad,0x1bec84b,0xd1c29b77,0xc2587f3e,0x26667b95,0xb9c411a3,0x59240646,
0x32c7456e,0x1ff7bdb9,0x2a8de1f1,0xa6370ece,0xade7430c,0x496be4e7,0x21af452,0x7dcf1bc0,0xb4360a2,0x703e9072,0x55514f03,0xde1fc207,0x162540d7,0x68cb5901,0x3f9c6ef0,0xb7652e28,
0x292785de,0x402de3a8,0xfca1d0ec,0x4b938824,0x9aa52f0b,0x514e8174,0x1f1e84a5,0x68a63354,0x9ffc1551,0x56cf249f,0x9c2586a0,0x5ad113c5,0x45da309e,0xdb69f998,0x22ca5549,0x1a3b60e0,
0x7ff4a754,0xfc3c0440,0x25190594,0x69427d8d,0x982a2f46,0xa46771d0,0x7f60b835,0x672a5184,0x7a18d4f5,0xac15aaa2,0x53b5d2b6,0xe5016745,0xa56e44e3,0xadf4e5cb,0x853e6344,0x7a864ecc,
0x1653322,0x9c8574d0,0x788043f2,0x9976dfee,0xde002423,0x21f76d1d,0xb9e5e627,0x236b78bc,0xaeca2a4f,0x6ffdb3ea,0x3be7de65,0x311bd648,0xfbebcd6f,0x59246a7,0x3188a7f6,0xf958ad39,
0xc02c8440,0x38b35c6d,0x809be2ce,0xa920c483,0xd9415781,0xe689aca9,0x23f68714,0x791b7bee,0xaf971ccd,0xe1c7b672,0x534cdf58,0x2f7e2346,0x81b9ad95,0x77d48867,0x8e62d45d,0xdee26905,
0xd50f16f5,0x5b41baa7,0x4845cd4e,0x481a9a80,0x4ee6709c,0xd6f56ec,0x200af653,0xcb79e620,0x129735d8,0x3769c8ea,0xfc56f2f4,0x98721181,0xb0a7ff09,0xd6305c60,0x3b1740bf,0x13db8d94,
0x6f3d96ae,0x54a77d76,0xcf5e13cf,0x1e67a2cb,0xd59bed6,0x70ac5f15,0xffdae99a,0xaf68be33,0xaf652726,0x722df8d6,0x97dffd09,0xac2b8d10,0x3f29c35c,0x9c4a819f,0x8379e5f6,0xdffb59e7,
0xcedf8f1a,0x32f83d7c,0xeed75ac9,0xf6cda72a,0xa488b946,0xa7420e,0xc56922e,0xe30bcb8a,0xcaa4ad4a,0xed575faa,0xe622006d,0xae8b80ad,0x61eaff41,0x4f933a32,0x456f8699,0x945b1109,
0xc13d6bd3,0xbc9132af,0x1ebcd8e4,0x15927e96,0x3aefad7,0x1bbc79ba,0x754e306d,0xf2027205,0xdeeed3e,0x81c34277,0xbbfb4851,0x93fe53ce,0x78cbb4df,0x66fc52ce,0xa1cc634e,0xbeec1b6a,
0xec46b6b8,0x16785306,0xcf8cafb8,0x953a6c61,0x2b780eb8,0x3ab02fe9,0xd97c0bfe,0xd2a8a662,0x27eb0b66,0xff25edb5,0x4dc589a1,0xbfb34fd9,0xf1f71b37,0x8e24e42a,0x7beeb845,0xc4009350,
0x4b18cd52,0xb763af8f,0x14ccb4cd,0x1cc90215,0x321107e2,0xc305c9f2,0x1f9a0f47,0x4307257e,0x5c04babc,0x96f59bca,0x5c9c8ae4,0xecb3904d,0x6aedee95,0x94a7cf9b,0x70376a97,0xb8f6fb19,
0x5e3aa072,0x73875837,0x9023dfbd,0xf33d4cf9,0x1cf10975,0xc0a738f2,0x11792e79,0x24d22d8b,0x15b1a4a5,0x33f7b7f3,0xe66a6d47,0x7a94b047,0x43cc3a83,0x2fe7ed02,0x3f4f05e5,0xa55518dc,
0xd04830b0,0x606a9dc5,0xf4be2922,0xc008590,0xf1ba5f2f,0xbdcdbe0,0xd13055b9,0x21cd1515,0xd071f6c7,0xab4830b6,0x4bf2290d,0x18e62d1b,0x38589294,0xf80db2ff,0xf688ecc2,0x58cc6bad,
0x39315722,0x73b23ba7,0x1a6c42a9,0x7acda71f,0x8e2e3598,0x27dc1bf,0x1f3a680c,0x8a0e2e39,0xb447c1f5,0x1d8e6087,0x3f5cb49a,0x9845eb6b,0x6d13013e,0x40335704,0x31863963,0x29257f5a,
0xb266dea,0x9940be27,0x8be14ab9,0x333f2335,0x2fa33051,0xe2154063,0x4b4eeb89,0xffc96855,0x368a94a5,0x2f9994e9,0xafd1d482,0xccc8a4a,0x41b920c2,0xc73e7a79,0x453454eb,0xb3480623,
0xee87ae3,0xe6694969,0x54019491,0x98e2f7f3,0x275b77fa,0x68cf4b58,0xe3116176,0xc78bbf14,0x5169817e,0x9fc92ff1,0xe6e86591,0x152bfa98,0x44b492c6,0x286b5efd,0xcbcbcdab,0x1dff2e5d,
0xec4941e4,0xfffe608f,0xb041d8b3,0x1834c739,0xc7404f90,0xb7039b0,0x62879d92,0x7bbe58e6,0x26d99cf0,0x48e7a12c,0x3a4d1aec,0x14d4f972,0xff284beb,0x617c1038,0x41e1ebbd,0xa5739aba,
0x2f68be92,0x9ec01adc,0xd0e0c4e0,0x9d869c85,0x287718c3,0xfa8c6635,0x1180c25e,0xf4521189,0x3253fd7b,0xfd7af5e1,0xfbb52ec3,0xd1272b2a,0xa65fdcd,0xbf2a1a4c,0x1c94c2c9,0xf7893a6f,
0xccb5b9e9,0x199e527c,0x923ded3d,0x5c501aeb,0xcbfa3920,0xf70741f2,0xbc09dcf6,0xc4af5366,0x4b6d4404,0x681d3266,0x51c9063d,0x904a5a56,0x73762f42,0x97ed4e28,0x9cb01282,0x9e795de,
0xb3e4a060,0xf36a0d31,0x73ad394a,0x6903c04d,0x7bd2e8d5,0xa931a00d,0xd0464883,0x96d39763,0x6c47ba9a,0x1c8b2e8f,0x45cffff,0x449e4956,0x5d394c8d,0x317e27c6,0xaa3c8331,0xe3385390,
0xff915b55,0x1d4e194b,0x81b0dba5,0xb450b1a2,0x6f23c20d,0x4c6cd15f,0xf6fd313,0xb5088c6a,0x4932ef06,0xdad0e5e,0x313dfc60,0x3e9a2a70,0x30d0186c,0x14b9499d,0x63b41966,0xea116e4a,
0x85f6ac5d,0xb8e58069,0x88ce3a3c,0xfeb060de,0xc5ec6459,0xd9b79a09,0x6357894c,0xfaf11b4a,0x11563a71,0x51887b96,0x6f0fe9a1,0xca337a77,0x9571aaf3,0x47ff4158,0xb8517f8d,0xd45a4d28,
0x2cde2939,0x473d4e36,0xa056140e,0xec8713c2,0x98d27989,0x859feafa,0xb0f11c81,0xc21fc059,0xb2211c14,0x3594a500,0xbd3702a6,0xf22ded63,0x7d7346d6,0xc5b34388,0xecb0548b,0xa9e625dd,
0x21b5e177,0x88e664df,0x337407a,0x8364814e,0x585db875,0x697e43c3,0xb56be716,0xfe706586,0x7ec62ef9,0x9f9229b4,0xf77f21b7,0xa5210a15,0xcfac071c,0x36f5131c,0x57c7a5b0,0xb80c763a,
0xeff18a9e,0xc02aeb81,0xdee7fa6f,0xa3315be6,0xd3a97a1d,0xabf653e5,0xb33aa478,0x6a13570,0xe28e63e8,0xc0853e1f,0xf07a46da,0x366b92cf,0x194b1957,0x70760972,0xf45136a0,0x82bc66b8,
0x22bcfa6a,0xa4bce0e2,0x639be2e3,0xa543f41d,0xf2522630,0x31ac9a1a,0x73d25bb2,0x7e48a41c,0x8dfbfb8b,0xfc5b47c7,0xb344bfb0,0xf39563ac,0x3c766e81,0xea7e6fab,0xd3971137,0xf23b8e3b,
0xf93932fa,0x9cd6e732,0x39945445,0xd1b8688c,0xa89c1e72,0x5599e62d,0xc0d08cb4,0x7686c4c6,0x40ed2165,0xf05bf58,0xc9c97881,0xde8fbbd5,0x27738048,0xbd322782,0xbfb73751,0xd88a37ed,
0x427a997c,0xabaae5fa,0x74920850,0x396cda7a,0x92eec18e,0x482bcc87,0x230fcd47,0xf39df5e1,0x79c18697,0xa963735d,0x4568ddfc,0xb9b551,0x8dbf940d,0x4e4b2e1b,0x3aeb05bb,0xf097f3c7,
0xba18cf18,0xb3794a77,0xf8111674,0x96ada053,0x3d50a449,0x3c276094,0x5a70b522,0xbf91e5e7,0x2822f7bc,0x9d9719df,0x28af1597,0xba4ef8e2,0x178457f8,0xce4c9144,0xad3f2b8e,0xbdf84b77,
0x61f88e40,0xf881dbe3,0x56dbdbde,0x29dda7d2,0x2ac8f29c,0xe674ea71,0x921ed161,0x83ca3783,0x281b341f,0x12725484,0xa9048f35,0x6e28bc82,0x93129a9,0x9fb31377,0xacac94bf,0x78c3276a,
0xfbd4abd4,0xe92dfd0e,0x792eda6e,0xd4edfa18,0x5506d4a3,0x608335e5,0x76c1043d,0x51297953,0x56c7191e,0x3d73610f,0xb4da2845,0x36deaf6e,0xa63c8a8d,0x48a19f80,0x945dfa4a,0x309d589,
0x5fa2e0fc,0x4489954f,0xd15bd7e2,0x5890c00e,0xb7d7a09f,0x5a9a11a4,0x22d6741b,0xa501cd35,0xb09ff717,0x520e547c,0x9ad246a6,0xd95ac63b,0xfaf4751f,0x7aa230c3,0xecd838d9,0x17ddb8ef,
0xe21523fd,0x9d62a62d,0x8654c6a9,0x63e11959,0xabc8f678,0xc02f7568,0x38c7bad7,0x5ece0f0e,0xa6a999c6,0xb207582c,0xf29ad1d5,0xcc8c4815,0x69675e2b,0x88363cd2,0xbed347a5,0x7eedcfef,
0x3f2cd0d1,0x1dd14bd8,0xa435e6e9,0x998824a5,0xa61defb4,0x64644bd,0x3ca27c24,0xa61a9614,0xed9a23d0,0xc39d22b5,0xe896f088,0x38765b9a,0xef6a77a1,0xc60188ff,0xee843661,0x76ada85c,
0x417feb33,0xf32be6cf,0x6fe8a0c1,0x9fe56213,0x941bb3e1,0x19808704,0xb3257b84,0x378654d2,0x5a4a1e03,0x8e119a2f,0xcb15e99f,0xbcbfc803,0xf90f0926,0xdc83a552,0x7390c68a,0x7af3b0b,
0xa0f16dc8,0x920df95d,0x9e9c3421,0xaddf2a32,0x7d0e196a,0xcbe8c240,0x3c99f299,0xa3e7b9e2,0xd5734ec2,0x5682236b,0xd898ea8,0xca6d0727,0x845f0f0f,0x2cb746fc,0xbbfe4680,0x30f388e7,
0x41fe9f70,0xc7665cbe,0xa582fe5e,0xcf1fa3d1,0xe7087edf,0xd9c883e7,0x77565c24,0x947102a7,0xda6dc0e5,0xc86d9a56,0x8959f2b5,0x674060a8,0x18af9e99,0x1285ad0f,0x870ed377,0x66679bba,
0xaba1725f,0x50a02dc,0x8cac87fd,0x7362e71a,0xb4e71dda,0xe987b880,0xd4c2fe4f,0x3c5e593d,0x49ad75fa,0x7c0fc27d,0x5dc7b013,0x82bdb039,0x3499746a,0x3c3b0959,0x50893489,0xcc843ea7,
0x669c5c33,0xe68bc89a,0xec2ff7f7,0x4fbe0c58,0x8235c552,0xd8c732a2,0xc165a9bb,0xde74b27d,0xc9e4df16,0x6f88d9de,0xa8c310f1,0x448df110,0x6434ff30,0x5882ee2a,0x263309fc,0x6f872bc1,
0xb80c2f8d,0xe3b86bd9,0xf18390d3,0x1b880746,0x6f231f8a,0x5981cafd,0xd6565ea6,0x4d6ace16,0x108a99c8,0x25d0d43a,0x3597eacd,0x13491a29,0xbf585ea3,0x3140d657,0x66c81997,0xf1e90d54,
0xd74d8cb5,0xd912e082,0xebb231cb,0xd3bb264b,0x5f6e64b5,0x7ce68abf,0xc4df08c1,0x75f7844f,0x13eba52c,0x393ccb39,0x26d8af0d,0x5ca15705,0x2b844c6a,0x37960ee7,0x5e81d4c1,0xef02496a,
0x1b978283,0xbeccd5f1,0x6cf1d776,0xa3812f31,0x83ea367a,0xd332b223,0x623c4dc6,0xaf41acc6,0x4bd297e1,0xf33357c,0xd27256fb,0xe01013b9,0x2b9dd41f,0x3f0f7d06,0x86db6edf,0x8d919c58,
0xef09b9d8,0x6e482b8b,0x240d878e,0x4a227547,0xf918d9a0,0xed5aca40,0x559ede7d,0xa62ef5ca,0x3c8ab9b2,0xe98067e0,0x52fc0b53,0xc7a2529,0x5931e04c,0x616f8773,0xc1db3475,0x7323ffa7,
0x8636bf62,0x3ab0a911,0x16c71a79,0x7437fa0b,0xc404070b,0xec6e7361,0xda55d2ff,0x6effdfee,0x1559b7d7,0x16eda8e,0x7da67c39,0x29327a83,0xbd92499a,0x6a03170b,0x340c02b9,0x929f12cb,
0xa726820,0x64b76a32,0x84f9d351,0xd9a51720,0x7127ce52,0xb4d1c8f8,0xaa87a16d,0x199169f0,0x460c16f5,0x89b63861,0xf8a56bfc,0xba205f4a,0x79c37d1b,0x208fa2eb,0xc822e94c,0xa87c1a2f,
0x5336838,0x80c4e85f,0x5fa774ba,0x3d7d1330,0x41a25915,0x137550e1,0xce81c1d8,0xa1e0a5d5,0x487b5fdd,0xf3e617e1,0x7e5786b9,0x8f55ce3f,0x888004bd,0x78f4462f,0xb91dbebf,0xa63ccf2b,
0x156807c8,0x6f1eda8d,0xa125a2d5,0x9ce049e0,0x65ba15c5,0x3cfcab33,0xe2579482,0x71c8cda5,0x46f6f398,0x4232c05,0xf8906c21,0xada6d459,0x8f14acc7,0xf03bd4ac,0x9bd4115,0xf6f4d045,
0x2f18305d,0xf250036d,0x225c7eb8,0xf6ea00e4,0x780db5d7,0x6931ebb6,0x8375029c,0xdf7eefdc,0x4a083bcb,0xfbc26813,0x24ed2067,0x49d84362,0xabb923b7,0x739cc079,0x5af5f7e0,0xb5edeb23,
0x8ddca8ef,0x313c061e,0xf1532987,0xbc659899,0x659b7536,0xe079754e,0x169a58e7,0xad5ebfc4,0x2cef354b,0x2f1742b8,0x9f130714,0xd4a747ca,0x13afc5a,0xbf4d33a4,0xfaf6634a,0x80c344d4,
0x3e4c1e2a,0x92bca3f,0x3472fbd7,0x5aea8e13,0x8b5866fe,0xb56c4163,0x1b2451b8,0x8965ba8a,0xa1925339,0x6877cf45,0x427d3b62,0x21c5fbc2,0x6a7d9b85,0xecf95e93,0xe399acdf,0x7be7fe34,
0x12aa60b6,0x97dceb48,0xbf7cb22c,0x25089a7a,0x628e3e7a,0x34ec72a5,0xd5100384,0xd250b323,0xda8d9963,0x51024ef5,0x91e5e107,0x6d5d4d97,0xa55aed17,0xf81190de,0xcfe4fb86,0xb32cc225,
0x407c5bf6,0xeab09dfb,0x5b3c9148,0x29c52779,0x55293b57,0x45716939,0x67206818,0xeafa70ad,0xb88c4a13,0x46b22b49,0x37d32e48,0xf9bfa4cd,0x7fd04d7a,0x4bb2556d,0x4f5a2675,0x40f25b45,
0x8720c6b,0xff6255ec,0x7700b380,0x87ec36a2,0xfea876d,0x3c11643f,0x68aace2d,0x20ac3c36,0x1fd8c9f3,0x51b6ff8e,0x6b025520,0x41b600ce,0xa82b0970,0x96ebbead,0x10caffcf,0x5e811dd7,
0x75189e9b,0x68b899b3,0x4ede5d01,0xb27aa67a,0x1a0b86ee,0xf19df629,0x52048b2f,0x5e09004a,0x1a84c24e,0x605d5efd,0x3db5ba2e,0x29f9c0f4,0x6ff1c063,0x38408a5a,0x24d103c0,0xa108093a,
0x61949f64,0x58fb20c8,0xf3c69312,0xacf7e894,0x7cc9beaf,0x2c5d7da9,0xbb419a1b,0x3e86466e,0x3dff87b9,0x7c7361a1,0xdf2ead05,0x51254195,0x746aaa6b,0x850795b5,0xc72465d0,0x23061243,
0xa221aa6f,0x2794d364,0x1d179591,0x9c584ac6,0xbee0b806,0x5a22c644,0x2174623a,0x97bb7848,0x333d6b77,0x69833b2c,0xc2919776,0x22e4ee62,0x5f4a802d,0x310bf87d,0xefc62eba,0xd3182064,
0x4dc08a88,0x68e7e81f,0x64ef4d67,0x429d1279,0x6fb99108,0x42807b62,0xe2e2be4,0x1d2e69b7,0x40ff108c,0x393b302a,0x9ea28db5,0x5f69f73e,0x53e1bc27,0x59e0d7d0,0xe565616d,0x34b6a6f8,
0x89bc3607,0x65a2cdb1,0x394b4045,0x170943ec,0x48de70d,0x4a3216e1,0x472234a9,0xe33954d9,0xc6e2a1c3,0x1e8b8fe7,0xd959f156,0x7e0b1da5,0x4e270c3e,0x8027e501,0xc4714668,0x3063832,
0x8618694a,0xee822f43,0x536da45e,0x6cdfe1c5,0xa694553c,0x17830c34,0xac5e0f4b,0xc247b017,0xa99a61d4,0xf0f29a20,0x19c13dc6,0x1a2ff879,0x7535e657,0xfd2294f,0xf4f2a2ac,0x88cc6150,
0x25b40857,0x39fa2d1d,0x2b0627f,0xa0faefbf,0x40c901e9,0x605c1d2c,0x16cea415,0xdb12780d,0xbea84540,0xe0c8e810,0x6b6027f9,0x43af44d9,0xb38e7f43,0xfa29007d,0x7db1a72f,0x90cb75cf,
0x9187a2fc,0xc0659b48,0x460c5e7c,0x14d8b51a,0x64c4d903,0xaf4292a,0x7130c2d4,0x9e3f7b6e,0xbebbafe9,0xbfe9f6e7,0x444904f1,0x9c95135,0xa86fa51d,0x9d971345,0x69e01410,0xea93868d,
0x45ba22fe,0xf3d7bb3e,0x4c72ad70,0xd720dfa,0xddc52e5e,0x560c84b0,0xe3e27a53,0x3951d927,0x85055b91,0xdcb26fae,0x39dc2994,0xdca62d7b,0x422dde76,0xb30ab751,0x3723ce61,0x881e7e80,
0x1cc77932,0x1db4ed0b,0x9264ee95,0x8b63d9a2,0x5fe32fb2,0x77909e6,0xb3863278,0xc8b1ef3f,0xf32951d3,0x84a49e9b,0x20e2110e,0xd03441b,0xf8491787,0xeb45f910,0x75221798,0xc867b3a2,
0x6546dfe3,0xf9379548,0xf663c7f3,0x9b4815e5,0xc5070dbb,0x942ee1e6,0x7c44ead,0x8f3258ce,0xd0338a,0x429b3fac,0xf7ce7151,0xa69ad008,0x8325c96b,0x39dada5f,0x10231a0a,0xb6fb7132,
0x8853bb32,0xa80c0b43,0xa997746b,0xe9e2ce6d,0xd39dfb17,0x931fa4de,0x29f6abb4,0x9be64d33,0xa8c7c4c,0x8c267e06,0xefccb1c3,0xf69297cc,0x277556c,0xb146723,0x3f99cfa7,0xaea3ddc9,
0xa86aaa82,0x4ef40ee1,0x309df92c,0x99513d29,0x8b658973,0xe0e5b290,0x7978c864,0x781cc53a,0xb6a399f9,0xe2c05a37,0x55625e9d,0xd1178ac5,0x84e9a04,0x95357907,0x5da90865,0x5cabccaa,
0x5f457186,0x140e4103,0x3ecd3874,0x1baf14cd,0x11ea0cfc,0xae8150f0,0x455f8e93,0xdcc89040,0xe3e9e5e0,0xb48d022,0x34e27ef6,0x3c82b88,0xbba2d7cd,0x10b9364e,0x139664d0,0xa7e12c82,
0x15f2119c,0x9aca96b,0xad90650a,0x999134c0,0x253cccf3,0xbb343fdd,0x31cef94e,0x90a58a09,0x67a9d043,0x5ac67a30,0x8f3a5ed9,0xa040ff51,0xbc276211,0x6f4639a2,0x34f2cf24,0x15d7365f,
0x380035e6,0x8f2bcb38,0x72bff79a,0x9154bbed,0xe2f7d7ec,0x889518f9,0xba8a51c3,0x2a8dad10,0x87acdcc2,0xc58f75f8,0x876be04,0x15e79c15,0xaa24248,0xe7ed7a81,0x3705add6,0x428928be,
0x42c9,0xe8df9954,0x2009c5fd,0xe9621ae8,0x53105c4c,0x5f35659,0xd4df35c5,0x3036bda4,0x70f1eead,0xc38b46f9,0xa686f42b,0xe1829c6c,0x74775a26,0x81b9acb1,0xee79c53c,0x1c103a4d,
0xce103fa0,0xb0fc01a9,0x1a835712,0x10c8cf5,0xf91b06b9,0xd3450c7c,0xf013c2dd,0x6420c56a,0xbbd8d378,0xf30e9bf6,0x405f54ca,0x1cb9b6cf,0xc191a38a,0x268a47b,0xf3900553,0x1561a79e,
0x4aa09ace,0x697da012,0xc797139f,0xd21a5f7f,0xcc763f7e,0x24b03c1c,0x79ff745f,0xfa355373,0x4cc27246,0x2015eb06,0x6ba9db37,0x72056659,0x9d51ace4,0x4c31e145,0xa72f4bc6,0x6e132b3,
0xef07c65b,0xdd88665d,0x616dd51d,0x5e53a49,0x8fa4241f,0x84067fbd,0x616b39a2,0x48108386,0x170e719f,0x8fc7ad23,0x61fb3535,0xa61388c3,0x70b6881b,0x25eed556,0x8044a25d,0xcbdc161e,
0xd239b8f5,0x474bcf0f,0xd308c1e,0x18d6361f,0x371cd31f,0xbe649ea7,0xcc8b5180,0xe1ca0a62,0x53c2467,0xd2b4163c,0x592535cf,0x56008ca7,0xf7b8361,0xc4d3be29,0xe96b43a8,0xd881171d,
0x38845a01,0x97bd3ff2,0xa36e7c1a,0xdb89be98,0xe97eba54,0xe22a3da2,0x581ddb6e,0x5f56be4f,0xdf68a774,0x843740f3,0xfb7d6493,0x7d15543,0x16307075,0xe24b497,0x85ea2a72,0xeca0eb78,
0x19c5d32b,0x7ce65e61,0x3a9604ff,0x12a56732,0xa301c845,0x7074b231,0x59f223d1,0xc36da950,0xa20e9a0a,0x91b93ab3,0x14cff21c,0x5340ee31,0xa98f5254,0x6e180c83,0x7cf4a9f8,0x83cc4ff8,
0x4e6fca7c,0x54c69fc3,0x3dbc80c5,0x24636fa1,0x54f941c5,0x93dd3b3a,0x1cf860b5,0x9b6f38cf,0xee737a11,0x55fbbd9,0x5a36dd9e,0x47baab73,0x76b60068,0xe4b368c4,0xc20c9466,0x6cd09c65,
0xf6fe3c16,0x43e4f6a3,0x4ce64d4b,0xe9f9a5a,0xbcfe3edf,0x19eb654,0xb66a3066,0xa57ed7b8,0x5134be41,0xc477d096,0xbf02e3f9,0x6643feb7,0x9310919,0x2be33545,0x5b11c047,0x226df00b,
0x60bde7ef,0xaf98646,0xb9a2d982,0x87defbec,0x60f91cf5,0xb9d8aebf,0xe9815a4e,0x2fca11a6,0x88eee8e,0x1b2696bb,0xa5d99e18,0xc4037bb6,0x97932535,0xe7e36fc5,0xde136875,0x6a2ecf32,
0xa8fbe665,0x37958a51,0x1a952ec5,0x52f34c74,0x64316a10,0x657f37b9,0x6a802bbd,0xdfeaa52f,0xf08b1455,0x48625ce9,0x94229ed1,0x9bdcb99f,0xf167b2bb,0x7b52b0a9,0xcf793fec,0x81a54049,
0xce31f9cd,0x53e33929,0xfebf738c,0xdc8405e4,0x473ea355,0xcddc7b74,0xcd6f2d5a,0x1799171b,0x4c8d4443,0x563595e1,0x64c3f087,0x311b154b,0xd9d6ab50,0x35dfac7e,0x93aaf9e9,0xc3eabc7c,
0xc4be0b28,0xa0016851,0x89fe594f,0x35fb44d4,0xa90e0277,0x7f028505,0x9a28d4e1,0x509f5edb,0xb43153d4,0x21458e44,0x6bd10ff3,0xfba07b9d,0xcc63f006,0x54eaf018,0x8f82b82d,0xb70e5aef,
0x58b4ecab,0xba228a3,0xc8b49287,0x8465c05b,0xce044157,0x794fb1e,0x3ab3ac56,0xa89ee944,0xb05cabf6,0x6cd86556,0x983a403b,0x513dc651,0x4755251b,0x235cf88f,0x4522dd38,0x5da084d6,
0x8b118579,0x5b0efc01,0x81a62773,0x97164434,0x22bca87e,0x56da80f7,0xbea412ff,0xee48d234,0x91e09934,0xe7b565c7,0xaa0f192d,0x9d165cd8,0x78b2ec4d,0x93896a06,0xb3a23f06,0xd65c8d5,
0x1fccae31,0xdf1d4161,0x90e5ef2d,0x1ff4581,0xd9fccbba,0xc160bc91,0x581751a6,0x5191e212,0x79758a53,0x5fb03ca4,0xc5d0458a,0xd9437cba,0xabcc02a4,0x8289b048,0x4d54f2d1,0x5092f110,
0x6652c230,0xe488233f,0xecd8107,0x7d3555ff,0x4f6bd699,0x6295d009,0x75aad597,0x2828c230,0x4879cf81,0xce333472,0xd0705bff,0xd1445d,0x2001d881,0xb3e1e73e,0xa1409fe9,0xbf305325,
0x110f0c1c,0x129fee6d,0x18aa048a,0xec39007e,0xc6b0701d,0xfc6f2700,0x806dc8c2,0x3f0f4019,0x7429fb6b,0xf8c2a221,0x8e1042c5,0xc6c3e200,0xe2058819,0x93e99f2b,0xf363dc49,0x1a7857ef,
0xc0a6da2,0x48aea086,0xa3690572,0x8bab7948,0x45ca2675,0x7c1eb6f1,0xa4d67c2c,0xebe2a664,0xb216e0ae,0xdea25645,0xdb3a7018,0xaa0591fa,0x917a9c7,0xdd0da863,0x4d8edb4,0xeff15135,
0xe94c0fe7,0xcb59ab04,0xf0a362e6,0xb91a861d,0xc36bd926,0x5bf0df47,0xe6d1a2f1,0x63dd4d61,0xb5a970de,0x58ff36b2,0xd41f0108,0xc27e59b8,0x65b802e3,0xf349fa2e,0xfd6204de,0xde162ec9,
0x5a7315c8,0xbd1a1fe0,0xafbe9ae9,0xb6ab0c0b,0xc59e77bf,0xb54a5f03,0x48250b7d,0x3164b4d0,0x6a4fa1c6,0x5dea87c3,0x7a75f8a3,0x4ce3e0ac,0x23487697,0x2cb39b68,0xec3bf364,0x2fb8bfe5,
0x8a71a00c,0x3806fb5e,0xd0743d1,0xf6d4483b,0xaca0f8fa,0x38347cf3,0x9b4a23ed,0xa2d6f03b,0xdbdea9f6,0x54cbab6e,0x6e939291,0xf94a5f2d,0x8352ce22,0x612c7e46,0xcdd0ef24,0xf1afbaa6,
0x3e22fb7d,0x257308d2,0xf6fe9a61,0xf3b2b9fd,0x338df398,0x1ef11106,0x16634871,0x11df02bd,0x35e353dc,0x2db108fb,0xcaf64bfb,0xc5082907,0x1e1feb9a,0x609ba13b,0xa9e6829b,0x4e2ef249,
0x4ff3bf66,0xd7c45a8e,0x972de6d5,0x6e2df892,0xdad7884e,0xd7959cca,0xb2a1d1f2,0x6dd383ff,0x79a5960d,0xdb3ef651,0x72e91d5e,0xa6fce0bd,0xd535f393,0x41d7d816,0xde5a6f0e,0x3b8561cd,
0xd17e3d51,0x7743aea2,0x8e5363c6,0x999018b2,0x7d07b025,0x5e9eddf3,0x2a45a68c,0xfb76c159,0xf8066d5c,0xa92d258c,0xc3eee65c,0xd09aafcc,0xec5a6cb4,0xbf7a5d0e,0x73daec6d,0xcb322325,
0xd4a0cf8e,0x118dc5b8,0x97c7d99f,0x20c33d5f,0x2a1965d4,0x97cc521d,0x332f5a69,0x9b6ed38c,0x28bc3c19,0x8bb6fce7,0xa6cd5b7c,0xa6366d14,0x9f5cf8ee,0x7ef41a97,0x7b9f1749,0x1258ad1c,
0x63917477,0xe1287f55,0xfa62b298,0x917c8cfd,0xe68a4054,0x7ca780d2,0x285acf68,0xdf9bb2a2,0xf40e5451,0x43148150,0x218f5cff,0x532ca1e8,0x388c3bc,0xf2d9ddb0,0x9c4af89f,0x36183213,
0x43bda87d,0x2bec4999,0xc3333e6,0x34007692,0xc95f3290,0xcce35228,0xc0f6d4f4,0x34e32dbb,0x5cbf2b08,0x1c805897,0xe55fea5,0xbc1c0466,0xe80d5310,0x31125ead,0x36554e2e,0x97e6cab7,
0x11c0c50d,0x283dcc31,0xe16ad18a,0x7f66fbc1,0x88708ee3,0x407fd650,0x8e8658c0,0x820543c8,0x514cd0e7,0x7b94646d,0x4fb3d1c3,0x374cd910,0xeb3e1337,0x738e62da,0x7f8c617c,0x80972ee7,
0xdd8baa74,0xa71ffaf2,0x92922ccf,0x4ac007e2,0xcf28f6c0,0x36576bde,0xc8f2e48a,0xd9e67ba1,0xf2b402d4,0x86d08d56,0x95fcb545,0xa5de2ee8,0xfb6d2b15,0x61ab650a,0xdacf5d6a,0xff654f02,
0x6f3ad050,0x1ff888a9,0x24034b42,0x6946108b,0xc621cbd4,0xe9731cd5,0xc08a33c7,0xd688c986,0xf5b9ef30,0x9263f467,0x5cdccbe8,0xff671525,0xffa77c2e,0x5364bb84,0x352c9194,0x5673d055,
0xd8a8fb7a,0xf0484a2b,0x10d63d91,0x97b43a31,0xce4a2e19,0x6572bdcc,0xa30001df,0xc860be90,0x7cd838db,0x8be0e551,0xb3598a7,0xf5978c2c,0x1c06468,0x1fd45b2e,0x49093c89,0x67923a9c,
0xc32650dc,0x5c970826,0xb53da3d4,0x81d136b1,0x26a48e7a,0x6681e8d4,0x13b39903,0xadbad61f,0xbb8485a0,0x62501ff8,0x94bbc389,0x58aeb4f,0xc22825b0,0x59c613a4,0x5aeff709,0x19a74ba5,
0x99c95bda,0x5d09fd49,0x9028439,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x70000,0x0,0x0,0x0,0x401,0x134f72cb,0x8a1438c,0x3cd96b0c,0xffc4cfec,0xa7667d12,
0x20d910b0,0x6bd3b16a,0x124aecf,0x193cec6b,0x5d293afb,0x8ceb6f9,0x52a418da,0x4410048,0xfa046073,0x29992c11,0xf92a6d0b,0x7a9d36ce,0x602709f7,0x6f2b6499,0xfbd99350,0xfc26ad2a,
0x8eb52ce,0xe1e7f328,0x6de52081,0xbffdbf1b,0x242d83e3,0x5f9077be,0xf14baac0,0xcc7096c3,0xa5124b31,0x9e97f0e2,0x263f7e34,0x529d045c,0x3391d977,0x6d1cfbae,0xdae14015,0x6a3c4b25,
0xb4d0806f,0x9bee1860,0x4daa633e,0x62430e5e,0x68863c91,0xd9207816,0xd6b64963,0x8ae0839c,0xbe200b43,0x21dafda0,0x68130604,0xe1b1d2eb,0xa6e3632e,0xc1bd86ee,0x4452834c,0xaebd4204,
0x9f1427f6,0x61b2f4a8,0x13c1d9e1,0xc29720db,0x4d6b1b81,0x6c73a9e0,0x2391d104,0xf0fb6e30,0xc279ef05,0x57e23649,0x8b6b69a6,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70000,0x0,0x0,0x0,0x401,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x70000,0x0,0x0,0x0,0x401,0x8d3df0ad,0x42b3319f,0xbcd21b5,0x4915a238,0xa4bd5f0e,0x30c5928c,0x7c652b1e,0xc11d0e61,0xe76b371c,0xa007ed3f,0x96d2b28d,
0xcd9e0822,0xb0f1af11,0x8236ca50,0x97b0355a,0xb67ff36e,0x794f5fd3,0xd38833df,0x3edd7373,0xb52a4ed9,0x47cc31c,0x96c0ac1b,0xa8208e0d,0x9e520aab,0x93ce7686,0x14ee3bfa,0xa046e30c,
0x11654a1c,0x2dea2f66,0x8b51ea2f,0xffa753b0,0x88d812cf,0xe1cd74ff,0xb8d1fb88,0xca353b8f,0xb6131d1f,0xb021b970,0x8e565171,0x6974e186,0x4c780d95,0xafe2902b,0x30d1578f,0xc34a75fa,
0x4ad71706,0xe1d14f65,0x14890940,0xa1e3712c,0xb3ea71e1,0x5f3e79ae,0xcf6dfa4b,0x5fdc3751,0x6abec6e8,0x96f7d74f,0xa5fc1f0c,0x2ed9f6d4,0x5d10bb42,0xd8242886,0xa401f492,0x7b66a51a,
0x924870f1,0xbfdb05ab,0x2fb82ed1,0xde62efd2,0x9bb94f61,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
const uint32_t BDSP_ARM_IMG_idle_process_header [2] = {sizeof(BDSP_ARM_IMG_idle_process_array1), 1};
const void * const BDSP_ARM_IMG_idle_process [2] = {BDSP_ARM_IMG_idle_process_header, BDSP_ARM_IMG_idle_process_array1};
/* End of File */
