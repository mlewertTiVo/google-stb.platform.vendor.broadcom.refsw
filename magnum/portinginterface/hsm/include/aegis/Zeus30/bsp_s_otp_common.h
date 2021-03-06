/***************************************************************************
 *  Copyright (c) 2005-2014, Broadcom Corporation
 *  All Rights Reserved
 *  Highly Confidential Property of Broadcom Corporation
 *
 *  THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED SOFTWARE LICENSE
 *  AGREEMENT  BETWEEN THE USER AND BROADCOM.  YOU HAVE NO RIGHT TO USE OR
 *  EXPLOIT THIS MATERIAL EXCEPT SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 ***************************************************************************/


#ifndef BSP_S_OTP_COMMON_H__
#define BSP_S_OTP_COMMON_H__

typedef enum BCMD_Otp_CmdMsp_e
{
  BCMD_Otp_CmdMsp_eReserved0  = 0,
  BCMD_Otp_CmdMsp_eReserved1_0  = 1,
  BCMD_Otp_CmdMsp_eReserved2_0  = 2,
  BCMD_Otp_CmdMsp_eReserved1  = 1,
  BCMD_Otp_CmdMsp_eReserved2  = 2,
  BCMD_Otp_CmdMsp_eReserved3  = 3,
  BCMD_Otp_CmdMsp_eReserved4  = 4,
  BCMD_Otp_CmdMsp_eCrLockEnable  = 5,
  BCMD_Otp_CmdMsp_eReserved6  = 6,
  BCMD_Otp_CmdMsp_ePciHostProtect  = 7,
  BCMD_Otp_CmdMsp_ePciClientProtect  = 8,
  BCMD_Otp_CmdMsp_ePcieHostProtect  = 9,
  BCMD_Otp_CmdMsp_ePcieClientProtect  = 10,
  BCMD_Otp_CmdMsp_eReserved11  = 11,
  BCMD_Otp_CmdMsp_eReserved12  = 12,
  BCMD_Otp_CmdMsp_eReserved13  = 13,
  BCMD_Otp_CmdMsp_eReserved14  = 14,
  BCMD_Otp_CmdMsp_eReserved15  = 15,
  BCMD_Otp_CmdMsp_eReserved16  = 16,
  BCMD_Otp_CmdMsp_eReserved17  = 17,
  BCMD_Otp_CmdMsp_eForceDramScrambler  = 18,
  BCMD_Otp_CmdMsp_eReserved19  = 19,
  BCMD_Otp_CmdMsp_eReserved20  = 20,
  BCMD_Otp_CmdMsp_eReserved21  = 21,
  BCMD_Otp_CmdMsp_eReserved22  = 22,
  BCMD_Otp_CmdMsp_eReserved23  = 23,
  BCMD_Otp_CmdMsp_eReserved24  = 24,
  BCMD_Otp_CmdMsp_eReserved25  = 25,
  BCMD_Otp_CmdMsp_eReserved26  = 26,
  BCMD_Otp_CmdMsp_eReserved27  = 27,
  BCMD_Otp_CmdMsp_eReserved28  = 28,
  BCMD_Otp_CmdMsp_eReserved29  = 29,
  BCMD_Otp_CmdMsp_eReserved30  = 30,
  BCMD_Otp_CmdMsp_eReserved31  = 31,
  BCMD_Otp_CmdMsp_eReserved32  = 32,
  BCMD_Otp_CmdMsp_eReserved33  = 33,
  BCMD_Otp_CmdMsp_eReserved34  = 34,
  BCMD_Otp_CmdMsp_eDataSectionLockByte  = 35,
  BCMD_Otp_CmdMsp_eDataSectionReadProtectBits  = 36,
  BCMD_Otp_CmdMsp_eDestinationDisallowKeyA  = 37,
  BCMD_Otp_CmdMsp_eDestinationDisallowKeyB  = 38,
  BCMD_Otp_CmdMsp_eDestinationDisallowKeyC  = 39,
  BCMD_Otp_CmdMsp_eReserved40  = 40,
  BCMD_Otp_CmdMsp_eReserved41  = 41,
  BCMD_Otp_CmdMsp_eReserved42  = 42,
  BCMD_Otp_CmdMsp_eAskmStbOwnerId  = 43,
  BCMD_Otp_CmdMsp_eReserved44  = 44,
  BCMD_Otp_CmdMsp_eCaControlBits  = 45,
  BCMD_Otp_CmdMsp_eCpM2MControlBits  = 46,
  BCMD_Otp_CmdMsp_eReserved47  = 47,
  BCMD_Otp_CmdMsp_eReserved48  = 48,
  BCMD_Otp_CmdMsp_eCustKeytoCaKeyLadderDisable  = 49,
  BCMD_Otp_CmdMsp_eCustKeytoCpKeyLadderDisable  = 50,
  BCMD_Otp_CmdMsp_eReserved51  = 51,
  BCMD_Otp_CmdMsp_eReserved52  = 52,
  BCMD_Otp_CmdMsp_eReserved53  = 53,
  BCMD_Otp_CmdMsp_eReserved54  = 54,
  BCMD_Otp_CmdMsp_eReserved55  = 55,
  BCMD_Otp_CmdMsp_eReserved56  = 56,
  BCMD_Otp_CmdMsp_eReserved57  = 57,
  BCMD_Otp_CmdMsp_eReserved58  = 58,
  BCMD_Otp_CmdMsp_eCaSoftwareKeyDisable  = 59,
  BCMD_Otp_CmdMsp_eReserved60  = 60,
  BCMD_Otp_CmdMsp_eCpM2mDesSoftwareKeyDisable  = 61,
  BCMD_Otp_CmdMsp_eCpM2m3DesSoftwareKeyDisable  = 62,
  BCMD_Otp_CmdMsp_eCpM2mAesSoftwareKeyDisable  = 63,
  BCMD_Otp_CmdMsp_eCpM2mAesCounterSoftwareKeyDisable  = 64,
  BCMD_Otp_CmdMsp_eM2mC2CssSoftwareKeyDisable  = 65,
  BCMD_Otp_CmdMsp_eM2mM6SoftwareKeyDisable  = 66,
  BCMD_Otp_CmdMsp_eReserved67  = 67,
  BCMD_Otp_CmdMsp_eReserved68  = 68,
  BCMD_Otp_CmdMsp_eReserved69  = 69,
  BCMD_Otp_CmdMsp_eReserved70  = 70,
  BCMD_Otp_CmdMsp_eReserved71  = 71,
  BCMD_Otp_CmdMsp_eAskmCAVendorIdSelect  = 72,
  BCMD_Otp_CmdMsp_eReserved73  = 73,
  BCMD_Otp_CmdMsp_eSecureBootEnable  = 74,
  BCMD_Otp_CmdMsp_eReserved75  = 75,
  BCMD_Otp_CmdMsp_eReserved76  = 76,
  BCMD_Otp_CmdMsp_eReserved77  = 77,
  BCMD_Otp_CmdMsp_eReserved78  = 78,
  BCMD_Otp_CmdMsp_eHostBootCodeHideEnable  = 79,
  BCMD_Otp_CmdMsp_eHostBootCodeDecryptionSelect  = 80,
  BCMD_Otp_CmdMsp_eReserved81  = 81,
  BCMD_Otp_CmdMsp_eReserved82  = 82,
  BCMD_Otp_CmdMsp_eReserved83  = 83,
  BCMD_Otp_CmdMsp_eReserved84  = 84,
  BCMD_Otp_CmdMsp_eReserved85  = 85,
  BCMD_Otp_CmdMsp_eReserved86  = 86,
  BCMD_Otp_CmdMsp_eSystemEpoch  = 87,
  BCMD_Otp_CmdMsp_eReserved88  = 88,
  BCMD_Otp_CmdMsp_eBseckEnable  = 89,
  BCMD_Otp_CmdMsp_eReserved90  = 90,
  BCMD_Otp_CmdMsp_eReserved91  = 91,
  BCMD_Otp_CmdMsp_eReserved92  = 92,
  BCMD_Otp_CmdMsp_eReserved93  = 93,
  BCMD_Otp_CmdMsp_eMarketId  = 94,
  BCMD_Otp_CmdMsp_eReserved95  = 95,
  BCMD_Otp_CmdMsp_eReserved96  = 96,
  BCMD_Otp_CmdMsp_eReserved97  = 97,
  BCMD_Otp_CmdMsp_eReserved98  = 98,
  BCMD_Otp_CmdMsp_eReserved99  = 99,
  BCMD_Otp_CmdMsp_eReserved100  = 100,
  BCMD_Otp_CmdMsp_eReserved101  = 101,
  BCMD_Otp_CmdMsp_eReserved102  = 102,
  BCMD_Otp_CmdMsp_eReserved103  = 103,
  BCMD_Otp_CmdMsp_eReserved104  = 104,
  BCMD_Otp_CmdMsp_eReserved105  = 105,
  BCMD_Otp_CmdMsp_eReserved106  = 106,
  BCMD_Otp_CmdMsp_eReserved107  = 107,
  BCMD_Otp_CmdMsp_eReserved108  = 108,
  BCMD_Otp_CmdMsp_eReserved109_0  = 109,
  BCMD_Otp_CmdMsp_eReserved109  = 109,
  BCMD_Otp_CmdMsp_eReserved110  = 110,
  BCMD_Otp_CmdMsp_eReserved111  = 111,
  BCMD_Otp_CmdMsp_eReserved112  = 112,
  BCMD_Otp_CmdMsp_eReserved112_0  = 112,
  BCMD_Otp_CmdMsp_eReserved113  = 113,
  BCMD_Otp_CmdMsp_eReserved114  = 114,
  BCMD_Otp_CmdMsp_eReserved115  = 115,
  BCMD_Otp_CmdMsp_eReserved116  = 116,
  BCMD_Otp_CmdMsp_eReserved117  = 117,
  BCMD_Otp_CmdMsp_eReserved118  = 118,
  BCMD_Otp_CmdMsp_eReserved119  = 119,
  BCMD_Otp_CmdMsp_eReserved120  = 120,
  BCMD_Otp_CmdMsp_eReserved121  = 121,
  BCMD_Otp_CmdMsp_eReserved122  = 122,
  BCMD_Otp_CmdMsp_eReserved123  = 123,
  BCMD_Otp_CmdMsp_eReserved124  = 124,
  BCMD_Otp_CmdMsp_eReserved125  = 125,
  BCMD_Otp_CmdMsp_eReserved126  = 126,
  BCMD_Otp_CmdMsp_eReserved127  = 127,
  BCMD_Otp_CmdMsp_eReserved128  = 128,
  BCMD_Otp_CmdMsp_eReserved129  = 129,
  BCMD_Otp_CmdMsp_eReserved130  = 130,
  BCMD_Otp_CmdMsp_eReserved131  = 131,
  BCMD_Otp_CmdMsp_ePciGwinDisable  = 132,
  BCMD_Otp_CmdMsp_ePcieGwinDisable  = 133,
  BCMD_Otp_CmdMsp_ePciMwinDisable  = 134,
  BCMD_Otp_CmdMsp_ePciMwinRestrictEnable  = 135,
  BCMD_Otp_CmdMsp_ePcieMwinDisable  = 136,
  BCMD_Otp_CmdMsp_ePcieMwinRestrictEnable  = 137,
  BCMD_Otp_CmdMsp_eReserved138  = 138,
  BCMD_Otp_CmdMsp_eReserved139  = 139,
  BCMD_Otp_CmdMsp_eReserved140  = 140,
  BCMD_Otp_CmdMsp_eReserved141  = 141,
  BCMD_Otp_CmdMsp_eReserved142  = 142,
  BCMD_Otp_CmdMsp_eReserved143  = 143,
  BCMD_Otp_CmdMsp_eReserved144  = 144,
  BCMD_Otp_CmdMsp_eReserved145  = 145,
  BCMD_Otp_CmdMsp_eReserved146  = 146,
  BCMD_Otp_CmdMsp_eReserved147  = 147,
  BCMD_Otp_CmdMsp_eReserved148  = 148,
  BCMD_Otp_CmdMsp_eReserved149  = 149,
  BCMD_Otp_CmdMsp_eReserved150  = 150,
  BCMD_Otp_CmdMsp_eReserved151  = 151,
  BCMD_Otp_CmdMsp_eReserved152  = 152,
  BCMD_Otp_CmdMsp_eMMCBootDisable  = 153,
  BCMD_Otp_CmdMsp_eReserved154  = 154,
  BCMD_Otp_CmdMsp_eReserved155  = 155,
  BCMD_Otp_CmdMsp_eEbiCsSwapDisable  = 156,
  BCMD_Otp_CmdMsp_eReserved157  = 157,
  BCMD_Otp_CmdMsp_eReserved158  = 158,
  BCMD_Otp_CmdMsp_eReserved159  = 159,
  BCMD_Otp_CmdMsp_eReserved160  = 160,
  BCMD_Otp_CmdMsp_eReserved161  = 161,
  BCMD_Otp_CmdMsp_eReserved162  = 162,
  BCMD_Otp_CmdMsp_eReserved163  = 163,
  BCMD_Otp_CmdMsp_eReserved164  = 164,
  BCMD_Otp_CmdMsp_eReserved165  = 165,
  BCMD_Otp_CmdMsp_eReserved166  = 166,
  BCMD_Otp_CmdMsp_eReserved167  = 167,
  BCMD_Otp_CmdMsp_eReserved168  = 168,
  BCMD_Otp_CmdMsp_eReserved169  = 169,
  BCMD_Otp_CmdMsp_eReserved170  = 170,
  BCMD_Otp_CmdMsp_eReserved171  = 171,
  BCMD_Otp_CmdMsp_eReserved172  = 172,
  BCMD_Otp_CmdMsp_eReserved173  = 173,
  BCMD_Otp_CmdMsp_eReserved174  = 174,
  BCMD_Otp_CmdMsp_eReserved175  = 175,
  BCMD_Otp_CmdMsp_eReserved176  = 176,
  BCMD_Otp_CmdMsp_eReserved177  = 177,
  BCMD_Otp_CmdMsp_eReserved178  = 178,
  BCMD_Otp_CmdMsp_eReserved179  = 179,
  BCMD_Otp_CmdMsp_eReserved180  = 180,
  BCMD_Otp_CmdMsp_eReserved181  = 181,
  BCMD_Otp_CmdMsp_eReserved182  = 182,
  BCMD_Otp_CmdMsp_eReserved183  = 183,
  BCMD_Otp_CmdMsp_eReserved184  = 184,
  BCMD_Otp_CmdMsp_eReserved185  = 185,
  BCMD_Otp_CmdMsp_eReserved186  = 186,
  BCMD_Otp_CmdMsp_eReserved187  = 187,
  BCMD_Otp_CmdMsp_eReserved188  = 188,
  BCMD_Otp_CmdMsp_eReserved189  = 189,
  BCMD_Otp_CmdMsp_eDualCoreAcpuMode  = 190,
  BCMD_Otp_CmdMsp_eSecureSandboxEnable  = 191,
  BCMD_Otp_CmdMsp_eWebCpuVerifyEnable  = 192,
  BCMD_Otp_CmdMsp_eReserved193  = 193,
  BCMD_Otp_CmdMsp_eReserved194  = 194,
  BCMD_Otp_CmdMsp_eReserved195  = 195,
  BCMD_Otp_CmdMsp_eReserved196  = 196,
  BCMD_Otp_CmdMsp_eReserved197  = 197,
  BCMD_Otp_CmdMsp_eReserved198  = 198,
  BCMD_Otp_CmdMsp_eReserved199  = 199,
  BCMD_Otp_CmdMsp_eSecureRsa2Sha1TruncateDisable  = 200,
  BCMD_Otp_CmdMsp_eReserved201  = 201,
  BCMD_Otp_CmdMsp_eReserved202  = 202,
  BCMD_Otp_CmdMsp_eReserved203  = 203,
  BCMD_Otp_CmdMsp_eReserved204  = 204,
  BCMD_Otp_CmdMsp_eReserved205  = 205,
  BCMD_Otp_CmdMsp_eReserved206  = 206,
  BCMD_Otp_CmdMsp_eReserved144_0  = 207,
  BCMD_Otp_CmdMsp_eReserved208  = 208,
  BCMD_Otp_CmdMsp_eReserved209  = 209,
  BCMD_Otp_CmdMsp_eReserved210  = 210,
  BCMD_Otp_CmdMsp_eReserved211  = 211,
  BCMD_Otp_CmdMsp_eReserved212  = 212,
  BCMD_Otp_CmdMsp_eReserved213  = 213,
  BCMD_Otp_CmdMsp_eReserved214  = 214,
  BCMD_Otp_CmdMsp_eReserved215  = 215,
  BCMD_Otp_CmdMsp_eReserved216  = 216,
  BCMD_Otp_CmdMsp_eAesCounterGenericModeDisable  = 217,
  BCMD_Otp_CmdMsp_eCaCpAesCounterAsfModeDisable  = 218,
  BCMD_Otp_CmdMsp_eAesCounterWmdrmNdModeDisable  = 219,
  BCMD_Otp_CmdMsp_eCaCpAesCounterHdcp21ModeDisable  = 220,
  BCMD_Otp_CmdMsp_eReserved221  = 221,
  BCMD_Otp_CmdMsp_eReserved222  = 222,
  BCMD_Otp_CmdMsp_eReserved223  = 223,
  BCMD_Otp_CmdMsp_eReserved224  = 224,
  BCMD_Otp_CmdMsp_eReserved225  = 225,

  BCMD_Otp_CmdMsp_eReserved226  = 226,
  BCMD_Otp_CmdMsp_eReserved227  = 227,
  BCMD_Otp_CmdMsp_eReserved228  = 228,
  BCMD_Otp_CmdMsp_eReserved229  = 229,

  BCMD_Otp_CmdMsp_eReserved230  = 230,

  BCMD_Otp_CmdMsp_eReserved231  = 231,

  BCMD_Otp_CmdMsp_eReserved232  = 232,
  BCMD_Otp_CmdMsp_eReserved233  = 233,
  BCMD_Otp_CmdMsp_eReserved234  = 234,

  BCMD_Otp_CmdMsp_eM2mAesGHddvdBlurayDisable  = 235,

  BCMD_Otp_CmdMsp_eReserved236 = 236,
  BCMD_Otp_CmdMsp_eReserved237  = 237,

  BCMD_Otp_CmdMsp_eAsymmetricCrLockEnable  = 238,
  BCMD_Otp_CmdMsp_eReserved239  = 239,
  BCMD_Otp_CmdMsp_eReserved240  = 240,
  BCMD_Otp_CmdMsp_eReserved241  = 241,
  BCMD_Otp_CmdMsp_eReserved242  = 242,
  BCMD_Otp_CmdMsp_eReserved243  = 243,

  BCMD_Otp_CmdMsp_eReserved244  = 244,
  BCMD_Otp_CmdMsp_eReserved245  = 245,

  BCMD_Otp_CmdMsp_eSize
} BCMD_Otp_CmdMsp_e ;

#endif
