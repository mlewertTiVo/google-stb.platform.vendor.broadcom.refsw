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
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* Module Description:
*
* Revision History:
*
* $brcm_Log: $
*
******************************************************************************/
/*******************************************************************
*               Do Not Edit Directly
* Auto-Generated from RTS environment:
*   at: Mon Feb  2 22:38:21 2015 GMT
*   by: chuck
*   for: Box 4ku0t
*         MemC 0 (32-bit DDR3@1067MHz) w/324MHz clock
*         MemC 1 (32-bit DDR3@1067MHz) w/324MHz clock
*******************************************************************/

#include "bchp_memc_gen_0.h"  /* located in /TBD directory */
#include "bchp_memc_gen_1.h"  /* located in /TBD directory */

#include "bbox.h"  /* located in /magnum/commonutils/box/include directory */

static const uint32_t aulMemc0_20150202223821_4ku0t[] = {
           0x001b0006,  /*   0: XPT_WR_RS 1340ns */
           0x805da04c,  /*   1: XPT_WR_XC RR 4630ns */
           0x80655022,  /*   2: XPT_WR_CDB RR 5010ns */
           0x81406053,  /*   3: XPT_WR_ITB_MSG RR 15830ns */
           0x8068f023,  /*   4: XPT_RD_RS RR 5190ns */
           0x85f2a03e,  /*   5: XPT_RD_XC_RMX_MSG RR 75200ns */
           0x80655021,  /*   6: XPT_RD_XC_RAVE RR 5010ns */
           0x84a0b05a,  /*   7: XPT_RD_PB RR 58510ns */
           0x833d5059,  /*   8: XPT_WR_MEMDMA RR 40960ns */
           0x833d5058,  /*   9: XPT_RD_MEMDMA RR 40960ns */
           0x802fa00d,  /*  10: GENET0_WR RR 2360ns */
           0x80871051,  /*  11: GENET0_RD RR 7079ns */
           0x8052d01a,  /*  12: GENET1_WR RR 4096ns */
           0x81a1c056,  /*  13: GENET1_RD RR 21875ns */
           0x8052d01b,  /*  14: GENET2_WR RR 4096ns */
           0x81a1c057,  /*  15: GENET2_RD RR 21875ns */
           0x8431203d,  /*  16: MOCA_MIPS RR 53000ns */
           0x8029b042,  /*  17: SATA RR 2190ns */
           0x8029b043,  /*  18: SATA_1 RR 2190ns */
           0x814e3055,  /*  19: HIF_PCIe1 RR 17500ns */
           0x3ffff0ff,  /*  20: UNASSIGNED off */
           0xbffff0ff,  /*  21: LEAP RR */
           0x83f4603c,  /*  22: BSP RR 50000ns */
           0x8082204f,  /*  23: SAGE RR 6820ns */
           0x84b3605b,  /*  24: FLASH_DMA RR 63000ns */
           0x814e3054,  /*  25: HIF_PCIe RR 17500ns */
           0x84b3605d,  /*  26: SDIO_EMMC RR 63000ns */
           0x84b3605c,  /*  27: SDIO_CARD RR 63000ns */
           0xbffff0ff,  /*  28: TPCAP RR */
           0x02ed2037,  /*  29: MCIF_RD_0 37000ns */
           0x02ed2039,  /*  30: MCIF_WR_0 37000ns */
           0xbffff0ff,  /*  31: UART_DMA_RD RR */
           0xbffff0ff,  /*  32: UART_DMA_WR RR */
           0x80371045,  /*  33: USB_HI_0 RR 2890ns */
           0xbffff0ff,  /*  34: USB_LO_0 RR */
           0x806e404e,  /*  35: USB_X_WRITE_0 RR 5780ns */
           0x806e404d,  /*  36: USB_X_READ_0 RR 5780ns */
           0x810d302e,  /*  37: USB_X_CTRL_0 RR 13300ns */
           0x80371046,  /*  38: USB_HI_1 RR 2890ns */
           0xbffff0ff,  /*  39: USB_LO_1 RR */
           0x02ed2038,  /*  40: MCIF_RD_1 37000ns */
           0x02ed203a,  /*  41: MCIF_WR_1 37000ns */
           0x0030700f,  /*  42: RAAGA 2400ns */
           0x00142003,  /*  43: RAAGA_1 1000ns */
           0x008c6024,  /*  44: AUD_AIO 6940ns */
           0x80261041,  /*  45: VICE_CME_RMB_CMB RR 2000ns */
           0x81318052,  /*  46: VICE_CME_CSC RR 16000ns */
           0x80460049,  /*  47: VICE_FME_CSC RR 3670ns */
           0x8046004b,  /*  48: VICE_FME_Luma_CMB RR 3670ns */
           0x8046004a,  /*  49: VICE_FME_Chroma_CMB RR 3670ns */
           0x80a5702d,  /*  50: VICE_SG RR 8176.66666666667ns */
           0x80075063,  /*  51: VICE_DBLK RR 0ns */
           0x81d5a035,  /*  52: VICE_CABAC0 RR 23200ns */
           0x833e203b,  /*  53: VICE_CABAC1 RR 41000ns */
           0x803a2047,  /*  54: VICE_ARCSS0 RR 3050ns */
           0x80255007,  /*  55: VICE_VIP0_INST0 RR 1850ns */
           0x8095e02a,  /*  56: VICE_VIP1_INST0 RR 7410ns */
           0x80255008,  /*  57: VICE_VIP0_INST1 RR 1850ns */
           0x8095e02b,  /*  58: VICE_VIP1_INST1 RR 7410ns */
           0x3ffff0ff,  /*  59: UNASSIGNED off */
           0x3ffff0ff,  /*  60: UNASSIGNED off */
           0x3ffff0ff,  /*  61: UNASSIGNED off */
           0x3ffff0ff,  /*  62: UNASSIGNED off */
           0x3ffff0ff,  /*  63: UNASSIGNED off */
           0x3ffff0ff,  /*  64: UNASSIGNED off */
           0x3ffff0ff,  /*  65: UNASSIGNED off */
           0x3ffff0ff,  /*  66: UNASSIGNED off */
           0x3ffff0ff,  /*  67: UNASSIGNED off */
           0x3ffff0ff,  /*  68: UNASSIGNED off */
           0x3ffff0ff,  /*  69: UNASSIGNED off */
           0x3ffff0ff,  /*  70: UNASSIGNED off */
           0x3ffff0ff,  /*  71: UNASSIGNED off */
           0x3ffff0ff,  /*  72: UNASSIGNED off */
           0x8001d05f,  /*  73: HVD_DBLK_Ch_0 RR 0ns */
           0x8001d060,  /*  74: HVD_DBLK_Ch_1 RR 0ns */
           0x801d4040,  /*  75: HVD_ILCPU RR 1451ns */
           0x80838050,  /*  76: HVD_OLCPU RR 6893ns */
           0x004aa018,  /*  77: HVD_CAB 3694ns */
           0x0055c01c,  /*  78: HVD_ILSI 4242ns */
           0x803a9048,  /*  79: HVD_ILCPU_p2 RR 2900ns */
           0x0056401d,  /*  80: HVD_ILSI_p2 4268ns */
           0x3ffff0ff,  /*  81: UNASSIGNED off */
           0x3ffff0ff,  /*  82: UNASSIGNED off */
           0x3ffff0ff,  /*  83: UNASSIGNED off */
           0x3ffff0ff,  /*  84: UNASSIGNED off */
           0x3ffff0ff,  /*  85: UNASSIGNED off */
           0x3ffff0ff,  /*  86: UNASSIGNED off */
           0xbffff0ff,  /*  87: SID RR */
           0x3ffff0ff,  /*  88: UNASSIGNED off */
           0x3ffff0ff,  /*  89: UNASSIGNED off */
           0x3ffff0ff,  /*  90: UNASSIGNED off */
           0x3ffff0ff,  /*  91: UNASSIGNED off */
           0x3ffff0ff,  /*  92: UNASSIGNED off */
           0x3ffff0ff,  /*  93: UNASSIGNED off */
           0x3ffff017,  /*  94: MADR_RD off */
           0x3ffff020,  /*  95: MADR_QM off */
           0x3ffff026,  /*  96: MADR_WR off */
           0x3ffff016,  /*  97: MADR1_RD off */
           0x3ffff01f,  /*  98: MADR1_QM off */
           0x3ffff025,  /*  99: MADR1_WR off */
           0x3ffff0ff,  /* 100: UNASSIGNED off */
           0x3ffff0ff,  /* 101: UNASSIGNED off */
           0x3ffff0ff,  /* 102: UNASSIGNED off */
           0x3ffff0ff,  /* 103: UNASSIGNED off */
           0x3ffff0ff,  /* 104: UNASSIGNED off */
           0x3ffff0ff,  /* 105: UNASSIGNED off */
           0x0095d027,  /* 106: BVNF_MFD0_0 7406ns */
           0x0025500a,  /* 107: BVNF_MFD0_1 1851ns */
           0x0095d028,  /* 108: BVNF_MFD1_0 7406ns */
           0x0025500b,  /* 109: BVNF_MFD1_1 1851ns */
           0x0095d029,  /* 110: BVNF_MFD2_0 7406ns */
           0x0025500c,  /* 111: BVNF_MFD2_1 1851ns */
           0x3ffff0ff,  /* 112: UNASSIGNED off */
           0x3ffff0ff,  /* 113: UNASSIGNED off */
           0x3ffff0ff,  /* 114: UNASSIGNED off */
           0x3ffff0ff,  /* 115: UNASSIGNED off */
           0x3ffff0ff,  /* 116: UNASSIGNED off */
           0x3ffff0ff,  /* 117: UNASSIGNED off */
           0x00165004,  /* 118: BVNF_VFD0 1110ns */
           0x3ffff00e,  /* 119: BVNF_VFD1 off */
           0x3ffff02f,  /* 120: BVNF_VFD2 off */
           0x3ffff030,  /* 121: BVNF_VFD3 off */
           0x3ffff0ff,  /* 122: UNASSIGNED off */
           0x3ffff0ff,  /* 123: UNASSIGNED off */
           0x3ffff0ff,  /* 124: UNASSIGNED off */
           0x3ffff0ff,  /* 125: UNASSIGNED off */
           0x00165005,  /* 126: BVNF_CAP0 1110ns */
           0x3ffff01e,  /* 127: BVNF_CAP1 off */
           0x3ffff031,  /* 128: BVNF_CAP2 off */
           0x3ffff032,  /* 129: BVNF_CAP3 off */
           0x3ffff0ff,  /* 130: UNASSIGNED off */
           0x3ffff0ff,  /* 131: UNASSIGNED off */
           0x3ffff0ff,  /* 132: UNASSIGNED off */
           0x3ffff0ff,  /* 133: UNASSIGNED off */
           0x3ffff009,  /* 134: BVNF_GFD0 off */
           0x3ffff033,  /* 135: BVNF_GFD1 off */
           0x3ffff034,  /* 136: BVNF_GFD2 off */
           0x3ffff0ff,  /* 137: UNASSIGNED off */
           0x3ffff0ff,  /* 138: UNASSIGNED off */
           0x3ffff0ff,  /* 139: BVNF_GFD0_1 off */
           0x3ffff0ff,  /* 140: BVNF_GFD1_1 off */
           0x00387014,  /* 141: MCVP0 2794ns */
           0x00387013,  /* 142: MCVP1 2794ns */
           0x009ed02c,  /* 143: MCVP_QM 7850ns */
           0x00389015,  /* 144: BVNF_RDC 2800ns */
           0x027dc036,  /* 145: VEC_VBI_ENC 31500ns */
           0x3ffff0ff,  /* 146: UNASSIGNED off */
           0xbffff0ff,  /* 147: M2MC_0 RR */
           0xbffff0ff,  /* 148: M2MC_1 RR */
           0xbffff0ff,  /* 149: M2MC_2 RR */
           0x3ffff0ff,  /* 150: UNASSIGNED off */
           0x3ffff0ff,  /* 151: UNASSIGNED off */
           0x3ffff0ff,  /* 152: UNASSIGNED off */
           0x3ffff0ff,  /* 153: UNASSIGNED off */
           0x3ffff0ff,  /* 154: UNASSIGNED off */
           0x8001d061,  /* 155: HVD_DBLK_p2_Ch_0 RR 0ns */
           0x8001d062,  /* 156: HVD_DBLK_p2_Ch_1 RR 0ns */
           0x3ffff0ff,  /* 157: UNASSIGNED off */
           0x3ffff0ff,  /* 158: UNASSIGNED off */
           0x3ffff0ff,  /* 159: UNASSIGNED off */
           0xbffff0ff,  /* 160: M2MC1_0 RR */
           0xbffff0ff,  /* 161: M2MC1_1 RR */
           0xbffff0ff,  /* 162: M2MC1_2 RR */
           0x3ffff0ff,  /* 163: UNASSIGNED off */
           0x3ffff0ff,  /* 164: UNASSIGNED off */
           0x3ffff0ff,  /* 165: UNASSIGNED off */
           0x3ffff0ff,  /* 166: UNASSIGNED off */
           0x3ffff0ff,  /* 167: UNASSIGNED off */
           0x3ffff0ff,  /* 168: UNASSIGNED off */
           0x3ffff0ff,  /* 169: UNASSIGNED off */
           0x3ffff0ff,  /* 170: UNASSIGNED off */
           0x3ffff0ff,  /* 171: UNASSIGNED off */
           0x3ffff0ff,  /* 172: UNASSIGNED off */
           0x3ffff0ff,  /* 173: UNASSIGNED off */
           0x3ffff0ff,  /* 174: UNASSIGNED off */
           0x3ffff0ff,  /* 175: UNASSIGNED off */
           0x3ffff0ff,  /* 176: UNASSIGNED off */
           0x3ffff0ff,  /* 177: UNASSIGNED off */
           0x3ffff0ff,  /* 178: UNASSIGNED off */
           0x3ffff0ff,  /* 179: UNASSIGNED off */
           0x3ffff0ff,  /* 180: UNASSIGNED off */
           0x3ffff0ff,  /* 181: UNASSIGNED off */
           0x3ffff0ff,  /* 182: UNASSIGNED off */
           0x3ffff0ff,  /* 183: UNASSIGNED off */
           0x3ffff0ff,  /* 184: UNASSIGNED off */
           0x3ffff0ff,  /* 185: UNASSIGNED off */
           0x3ffff0ff,  /* 186: UNASSIGNED off */
           0x3ffff0ff,  /* 187: UNASSIGNED off */
           0x3ffff0ff,  /* 188: UNASSIGNED off */
           0x3ffff0ff,  /* 189: UNASSIGNED off */
           0x3ffff0ff,  /* 190: UNASSIGNED off */
           0x3ffff0ff,  /* 191: UNASSIGNED off */
           0x3ffff0ff,  /* 192: UNASSIGNED off */
           0x3ffff0ff,  /* 193: UNASSIGNED off */
           0x3ffff0ff,  /* 194: UNASSIGNED off */
           0x3ffff0ff,  /* 195: UNASSIGNED off */
           0x3ffff0ff,  /* 196: UNASSIGNED off */
           0x3ffff0ff,  /* 197: UNASSIGNED off */
           0x3ffff0ff,  /* 198: UNASSIGNED off */
           0x3ffff0ff,  /* 199: UNASSIGNED off */
           0x801c803f,  /* 200: HOST_CPU_MCP_RD_HIGH RR 1500ns */
           0x8000006e,  /* 201: HOST_CPU_MCP_RD_LOW RR */
           0x802ad044,  /* 202: HOST_CPU_MCP_WR_HIGH RR 2250ns */
           0x8000006f,  /* 203: HOST_CPU_MCP_WR_LOW RR */
           0xbffff0ff,  /* 204: V3D_MCP_R_HI RR */
           0xbffff0ff,  /* 205: V3D_MCP_W_HI RR */
           0xbffff0ff,  /* 206: V3D_MCP_R_LO RR */
           0xbffff0ff,  /* 207: V3D_MCP_W_LO RR */
           0x3ffff0ff,  /* 208: UNASSIGNED off */
           0x3ffff0ff,  /* 209: UNASSIGNED off */
           0x3ffff0ff,  /* 210: UNASSIGNED off */
           0x3ffff0ff,  /* 211: UNASSIGNED off */
           0x3ffff0ff,  /* 212: UNASSIGNED off */
           0x3ffff0ff,  /* 213: UNASSIGNED off */
           0x3ffff0ff,  /* 214: UNASSIGNED off */
           0x3ffff0ff,  /* 215: UNASSIGNED off */
           0x80000069,  /* 216: HVD_PFRI_Ch RR 0ns */
           0x80000068,  /* 217: VICE_PFRI RR 0ns */
           0x3ffff0ff,  /* 218: UNASSIGNED off */
           0x3ffff0ff,  /* 219: UNASSIGNED off */
           0x3ffff0ff,  /* 220: UNASSIGNED off */
           0x3ffff0ff,  /* 221: UNASSIGNED off */
           0x3ffff0ff,  /* 222: UNASSIGNED off */
           0x3ffff0ff,  /* 223: UNASSIGNED off */
           0x3ffff0ff,  /* 224: UNASSIGNED off */
           0x3ffff0ff,  /* 225: UNASSIGNED off */
           0x3ffff0ff,  /* 226: UNASSIGNED off */
           0x3ffff0ff,  /* 227: UNASSIGNED off */
           0x3ffff0ff,  /* 228: UNASSIGNED off */
           0x3ffff0ff,  /* 229: UNASSIGNED off */
           0x3ffff0ff,  /* 230: UNASSIGNED off */
           0x3ffff0ff,  /* 231: UNASSIGNED off */
           0x3ffff0ff,  /* 232: UNASSIGNED off */
           0x3ffff0ff,  /* 233: UNASSIGNED off */
           0x3ffff0ff,  /* 234: UNASSIGNED off */
           0x3ffff0ff,  /* 235: UNASSIGNED off */
           0x3ffff0ff,  /* 236: UNASSIGNED off */
           0x3ffff0ff,  /* 237: UNASSIGNED off */
           0x3ffff0ff,  /* 238: UNASSIGNED off */
           0x3ffff0ff,  /* 239: UNASSIGNED off */
           0x3ffff0ff,  /* 240: UNASSIGNED off */
           0x3ffff0ff,  /* 241: UNASSIGNED off */
           0x3ffff0ff,  /* 242: UNASSIGNED off */
           0x3ffff0ff,  /* 243: UNASSIGNED off */
           0x3ffff0ff,  /* 244: UNASSIGNED off */
           0x3ffff0ff,  /* 245: UNASSIGNED off */
           0x3ffff0ff,  /* 246: UNASSIGNED off */
           0x3ffff0ff,  /* 247: UNASSIGNED off */
           0xbffff0ff,  /* 248: MEMC_TRACELOG RR */
           0x3ffff0ff,  /* 249: UNASSIGNED off */
           0xbfffe05e,  /* 250: MEMC_ZQCS RR 1000000ns */
           0xbffff0ff,  /* 251: MEMC_MSA RR */
           0xbffff0ff,  /* 252: MEMC_DIS0 RR */
           0xbffff0ff,  /* 253: MEMC_DIS1 RR */
           0xbffff0ff,  /* 254: MEMC_DRAM_INIT_ZQCS RR */
           0x004ef019,  /* 255: REFRESH 3906.25ns */
         };
static const uint32_t aulMemc1_20150202223821_4ku0t[] = {
           0x3ffff006,  /*   0: XPT_WR_RS off */
           0x3ffff04c,  /*   1: XPT_WR_XC off */
           0x3ffff022,  /*   2: XPT_WR_CDB off */
           0x3ffff053,  /*   3: XPT_WR_ITB_MSG off */
           0x3ffff023,  /*   4: XPT_RD_RS off */
           0x3ffff03e,  /*   5: XPT_RD_XC_RMX_MSG off */
           0x3ffff021,  /*   6: XPT_RD_XC_RAVE off */
           0x3ffff05a,  /*   7: XPT_RD_PB off */
           0x833d5059,  /*   8: XPT_WR_MEMDMA RR 40960ns */
           0x833d5058,  /*   9: XPT_RD_MEMDMA RR 40960ns */
           0x802fa00d,  /*  10: GENET0_WR RR 2360ns */
           0x80871051,  /*  11: GENET0_RD RR 7079ns */
           0x8052d01a,  /*  12: GENET1_WR RR 4096ns */
           0x81a1c056,  /*  13: GENET1_RD RR 21875ns */
           0x8052d01b,  /*  14: GENET2_WR RR 4096ns */
           0x81a1c057,  /*  15: GENET2_RD RR 21875ns */
           0x8431203d,  /*  16: MOCA_MIPS RR 53000ns */
           0x8029b042,  /*  17: SATA RR 2190ns */
           0x8029b043,  /*  18: SATA_1 RR 2190ns */
           0x814e3055,  /*  19: HIF_PCIe1 RR 17500ns */
           0x3ffff0ff,  /*  20: UNASSIGNED off */
           0xbffff0ff,  /*  21: LEAP RR */
           0x83f4603c,  /*  22: BSP RR 50000ns */
           0x8082204f,  /*  23: SAGE RR 6820ns */
           0x84b3605b,  /*  24: FLASH_DMA RR 63000ns */
           0x814e3054,  /*  25: HIF_PCIe RR 17500ns */
           0x84b3605d,  /*  26: SDIO_EMMC RR 63000ns */
           0x84b3605c,  /*  27: SDIO_CARD RR 63000ns */
           0x3ffff0ff,  /*  28: TPCAP off */
           0x3ffff037,  /*  29: MCIF_RD_0 off */
           0x3ffff039,  /*  30: MCIF_WR_0 off */
           0x3ffff0ff,  /*  31: UART_DMA_RD off */
           0x3ffff0ff,  /*  32: UART_DMA_WR off */
           0x80371045,  /*  33: USB_HI_0 RR 2890ns */
           0xbffff0ff,  /*  34: USB_LO_0 RR */
           0x806e404e,  /*  35: USB_X_WRITE_0 RR 5780ns */
           0x806e404d,  /*  36: USB_X_READ_0 RR 5780ns */
           0x810d302e,  /*  37: USB_X_CTRL_0 RR 13300ns */
           0x80371046,  /*  38: USB_HI_1 RR 2890ns */
           0xbffff0ff,  /*  39: USB_LO_1 RR */
           0x3ffff038,  /*  40: MCIF_RD_1 off */
           0x3ffff03a,  /*  41: MCIF_WR_1 off */
           0x3ffff00f,  /*  42: RAAGA off */
           0x3ffff003,  /*  43: RAAGA_1 off */
           0x008c6024,  /*  44: AUD_AIO 6940ns */
           0x3ffff041,  /*  45: VICE_CME_RMB_CMB off */
           0x3ffff052,  /*  46: VICE_CME_CSC off */
           0x3ffff049,  /*  47: VICE_FME_CSC off */
           0x3ffff04b,  /*  48: VICE_FME_Luma_CMB off */
           0x3ffff04a,  /*  49: VICE_FME_Chroma_CMB off */
           0x3ffff02d,  /*  50: VICE_SG off */
           0x3ffff063,  /*  51: VICE_DBLK off */
           0x3ffff035,  /*  52: VICE_CABAC0 off */
           0x3ffff03b,  /*  53: VICE_CABAC1 off */
           0x3ffff047,  /*  54: VICE_ARCSS0 off */
           0x3ffff007,  /*  55: VICE_VIP0_INST0 off */
           0x3ffff02a,  /*  56: VICE_VIP1_INST0 off */
           0x3ffff008,  /*  57: VICE_VIP0_INST1 off */
           0x3ffff02b,  /*  58: VICE_VIP1_INST1 off */
           0x3ffff0ff,  /*  59: UNASSIGNED off */
           0x3ffff0ff,  /*  60: UNASSIGNED off */
           0x3ffff0ff,  /*  61: UNASSIGNED off */
           0x3ffff0ff,  /*  62: UNASSIGNED off */
           0x3ffff0ff,  /*  63: UNASSIGNED off */
           0x3ffff0ff,  /*  64: UNASSIGNED off */
           0x3ffff0ff,  /*  65: UNASSIGNED off */
           0x3ffff0ff,  /*  66: UNASSIGNED off */
           0x3ffff0ff,  /*  67: UNASSIGNED off */
           0x3ffff0ff,  /*  68: UNASSIGNED off */
           0x3ffff0ff,  /*  69: UNASSIGNED off */
           0x3ffff0ff,  /*  70: UNASSIGNED off */
           0x3ffff0ff,  /*  71: UNASSIGNED off */
           0x3ffff0ff,  /*  72: UNASSIGNED off */
           0x8001d064,  /*  73: HVD_DBLK_0 RR 0ns */
           0x8001d065,  /*  74: HVD_DBLK_1 RR 0ns */
           0x3ffff040,  /*  75: HVD_ILCPU off */
           0x3ffff050,  /*  76: HVD_OLCPU off */
           0x3ffff018,  /*  77: HVD_CAB off */
           0x3ffff01c,  /*  78: HVD_ILSI off */
           0x3ffff048,  /*  79: HVD_ILCPU_p2 off */
           0x3ffff01d,  /*  80: HVD_ILSI_p2 off */
           0x3ffff0ff,  /*  81: UNASSIGNED off */
           0x3ffff0ff,  /*  82: UNASSIGNED off */
           0x3ffff0ff,  /*  83: UNASSIGNED off */
           0x3ffff0ff,  /*  84: UNASSIGNED off */
           0x3ffff0ff,  /*  85: UNASSIGNED off */
           0x3ffff0ff,  /*  86: UNASSIGNED off */
           0xbffff0ff,  /*  87: SID RR */
           0x3ffff0ff,  /*  88: UNASSIGNED off */
           0x3ffff0ff,  /*  89: UNASSIGNED off */
           0x3ffff0ff,  /*  90: UNASSIGNED off */
           0x3ffff0ff,  /*  91: UNASSIGNED off */
           0x3ffff0ff,  /*  92: UNASSIGNED off */
           0x3ffff0ff,  /*  93: UNASSIGNED off */
           0x0046f017,  /*  94: MADR_RD 3511ns */
           0x0063d020,  /*  95: MADR_QM 4938ns */
           0x008e1026,  /*  96: MADR_WR 7023ns */
           0x0046f016,  /*  97: MADR1_RD 3511ns */
           0x0063d01f,  /*  98: MADR1_QM 4938ns */
           0x008e1025,  /*  99: MADR1_WR 7023ns */
           0x3ffff0ff,  /* 100: UNASSIGNED off */
           0x3ffff0ff,  /* 101: UNASSIGNED off */
           0x3ffff0ff,  /* 102: UNASSIGNED off */
           0x3ffff0ff,  /* 103: UNASSIGNED off */
           0x3ffff0ff,  /* 104: UNASSIGNED off */
           0x3ffff0ff,  /* 105: UNASSIGNED off */
           0x0031d010,  /* 106: BVNF_MFD0_0Y 2469ns */
           0x00129000,  /* 107: BVNF_MFD0_1Y 925.5ns */
           0x0031d011,  /* 108: BVNF_MFD1_0Y 2469ns */
           0x00129001,  /* 109: BVNF_MFD1_1Y 925.5ns */
           0x0031d012,  /* 110: BVNF_MFD2_0Y 2469ns */
           0x00129002,  /* 111: BVNF_MFD2_1Y 925.5ns */
           0x3ffff0ff,  /* 112: UNASSIGNED off */
           0x3ffff0ff,  /* 113: UNASSIGNED off */
           0x3ffff0ff,  /* 114: UNASSIGNED off */
           0x3ffff0ff,  /* 115: UNASSIGNED off */
           0x3ffff0ff,  /* 116: UNASSIGNED off */
           0x3ffff0ff,  /* 117: UNASSIGNED off */
           0x3ffff004,  /* 118: BVNF_VFD0 off */
           0x002fd00e,  /* 119: BVNF_VFD1 2370ns */
           0x013fd02f,  /* 120: BVNF_VFD2 15800ns */
           0x013fd030,  /* 121: BVNF_VFD3 15800ns */
           0x3ffff0ff,  /* 122: UNASSIGNED off */
           0x3ffff0ff,  /* 123: UNASSIGNED off */
           0x3ffff0ff,  /* 124: UNASSIGNED off */
           0x3ffff0ff,  /* 125: UNASSIGNED off */
           0x3ffff005,  /* 126: BVNF_CAP0 off */
           0x005fe01e,  /* 127: BVNF_CAP1 4741ns */
           0x013fd031,  /* 128: BVNF_CAP2 15800ns */
           0x013fd032,  /* 129: BVNF_CAP3 15800ns */
           0x3ffff0ff,  /* 130: UNASSIGNED off */
           0x3ffff0ff,  /* 131: UNASSIGNED off */
           0x3ffff0ff,  /* 132: UNASSIGNED off */
           0x3ffff0ff,  /* 133: UNASSIGNED off */
           0x00255009,  /* 134: BVNF_GFD0 1851ns */
           0x01414033,  /* 135: BVNF_GFD1 15873ns */
           0x01414034,  /* 136: BVNF_GFD2 15873ns */
           0x3ffff0ff,  /* 137: UNASSIGNED off */
           0x3ffff0ff,  /* 138: UNASSIGNED off */
           0x3ffff0ff,  /* 139: BVNF_GFD0_1 off */
           0x3ffff0ff,  /* 140: BVNF_GFD1_1 off */
           0x3ffff014,  /* 141: MCVP0 off */
           0x3ffff013,  /* 142: MCVP1 off */
           0x3ffff02c,  /* 143: MCVP_QM off */
           0x3ffff015,  /* 144: BVNF_RDC off */
           0x3ffff036,  /* 145: VEC_VBI_ENC off */
           0x3ffff0ff,  /* 146: UNASSIGNED off */
           0xbffff0ff,  /* 147: M2MC_0 RR */
           0xbffff0ff,  /* 148: M2MC_1 RR */
           0xbffff0ff,  /* 149: M2MC_2 RR */
           0x3ffff0ff,  /* 150: UNASSIGNED off */
           0x3ffff0ff,  /* 151: UNASSIGNED off */
           0x3ffff0ff,  /* 152: UNASSIGNED off */
           0x3ffff0ff,  /* 153: UNASSIGNED off */
           0x3ffff0ff,  /* 154: UNASSIGNED off */
           0x8001d066,  /* 155: HVD_DBLK_p2_0 RR 0ns */
           0x8001d067,  /* 156: HVD_DBLK_p2_1 RR 0ns */
           0x3ffff0ff,  /* 157: UNASSIGNED off */
           0x3ffff0ff,  /* 158: UNASSIGNED off */
           0x3ffff0ff,  /* 159: UNASSIGNED off */
           0xbffff0ff,  /* 160: M2MC1_0 RR */
           0xbffff0ff,  /* 161: M2MC1_1 RR */
           0xbffff0ff,  /* 162: M2MC1_2 RR */
           0x3ffff0ff,  /* 163: UNASSIGNED off */
           0x3ffff0ff,  /* 164: UNASSIGNED off */
           0x3ffff0ff,  /* 165: UNASSIGNED off */
           0x3ffff0ff,  /* 166: UNASSIGNED off */
           0x3ffff0ff,  /* 167: UNASSIGNED off */
           0x3ffff0ff,  /* 168: UNASSIGNED off */
           0x3ffff0ff,  /* 169: UNASSIGNED off */
           0x3ffff0ff,  /* 170: UNASSIGNED off */
           0x3ffff0ff,  /* 171: UNASSIGNED off */
           0x3ffff0ff,  /* 172: UNASSIGNED off */
           0x3ffff0ff,  /* 173: UNASSIGNED off */
           0x3ffff0ff,  /* 174: UNASSIGNED off */
           0x3ffff0ff,  /* 175: UNASSIGNED off */
           0x3ffff0ff,  /* 176: UNASSIGNED off */
           0x3ffff0ff,  /* 177: UNASSIGNED off */
           0x3ffff0ff,  /* 178: UNASSIGNED off */
           0x3ffff0ff,  /* 179: UNASSIGNED off */
           0x3ffff0ff,  /* 180: UNASSIGNED off */
           0x3ffff0ff,  /* 181: UNASSIGNED off */
           0x3ffff0ff,  /* 182: UNASSIGNED off */
           0x3ffff0ff,  /* 183: UNASSIGNED off */
           0x3ffff0ff,  /* 184: UNASSIGNED off */
           0x3ffff0ff,  /* 185: UNASSIGNED off */
           0x3ffff0ff,  /* 186: UNASSIGNED off */
           0x3ffff0ff,  /* 187: UNASSIGNED off */
           0x3ffff0ff,  /* 188: UNASSIGNED off */
           0x3ffff0ff,  /* 189: UNASSIGNED off */
           0x3ffff0ff,  /* 190: UNASSIGNED off */
           0x3ffff0ff,  /* 191: UNASSIGNED off */
           0x3ffff0ff,  /* 192: UNASSIGNED off */
           0x3ffff0ff,  /* 193: UNASSIGNED off */
           0x3ffff0ff,  /* 194: UNASSIGNED off */
           0x3ffff0ff,  /* 195: UNASSIGNED off */
           0x3ffff0ff,  /* 196: UNASSIGNED off */
           0x3ffff0ff,  /* 197: UNASSIGNED off */
           0x3ffff0ff,  /* 198: UNASSIGNED off */
           0x3ffff0ff,  /* 199: UNASSIGNED off */
           0x801c803f,  /* 200: HOST_CPU_MCP_RD_HIGH RR 1500ns */
           0x8000006e,  /* 201: HOST_CPU_MCP_RD_LOW RR */
           0x802ad044,  /* 202: HOST_CPU_MCP_WR_HIGH RR 2250ns */
           0x8000006f,  /* 203: HOST_CPU_MCP_WR_LOW RR */
           0xbffff0ff,  /* 204: V3D_MCP_R_HI RR */
           0xbffff0ff,  /* 205: V3D_MCP_W_HI RR */
           0xbffff0ff,  /* 206: V3D_MCP_R_LO RR */
           0xbffff0ff,  /* 207: V3D_MCP_W_LO RR */
           0x3ffff0ff,  /* 208: UNASSIGNED off */
           0x3ffff0ff,  /* 209: UNASSIGNED off */
           0x3ffff0ff,  /* 210: UNASSIGNED off */
           0x3ffff0ff,  /* 211: UNASSIGNED off */
           0x3ffff0ff,  /* 212: UNASSIGNED off */
           0x3ffff0ff,  /* 213: UNASSIGNED off */
           0x3ffff0ff,  /* 214: UNASSIGNED off */
           0x3ffff0ff,  /* 215: UNASSIGNED off */
           0x8000006a,  /* 216: HVD_PFRI RR 0ns */
           0x3ffff068,  /* 217: VICE_PFRI off */
           0x3ffff0ff,  /* 218: UNASSIGNED off */
           0x3ffff0ff,  /* 219: UNASSIGNED off */
           0x3ffff0ff,  /* 220: UNASSIGNED off */
           0x3ffff0ff,  /* 221: UNASSIGNED off */
           0x3ffff0ff,  /* 222: UNASSIGNED off */
           0x3ffff0ff,  /* 223: UNASSIGNED off */
           0x3ffff0ff,  /* 224: UNASSIGNED off */
           0x3ffff0ff,  /* 225: UNASSIGNED off */
           0x3ffff0ff,  /* 226: UNASSIGNED off */
           0x3ffff0ff,  /* 227: UNASSIGNED off */
           0x3ffff0ff,  /* 228: UNASSIGNED off */
           0x3ffff0ff,  /* 229: UNASSIGNED off */
           0x3ffff0ff,  /* 230: UNASSIGNED off */
           0x3ffff0ff,  /* 231: UNASSIGNED off */
           0x3ffff0ff,  /* 232: UNASSIGNED off */
           0x3ffff0ff,  /* 233: UNASSIGNED off */
           0x3ffff0ff,  /* 234: UNASSIGNED off */
           0x3ffff0ff,  /* 235: UNASSIGNED off */
           0x3ffff0ff,  /* 236: UNASSIGNED off */
           0x3ffff0ff,  /* 237: UNASSIGNED off */
           0x3ffff0ff,  /* 238: UNASSIGNED off */
           0x3ffff0ff,  /* 239: UNASSIGNED off */
           0x3ffff0ff,  /* 240: UNASSIGNED off */
           0x3ffff0ff,  /* 241: UNASSIGNED off */
           0x3ffff0ff,  /* 242: UNASSIGNED off */
           0x3ffff0ff,  /* 243: UNASSIGNED off */
           0x3ffff0ff,  /* 244: UNASSIGNED off */
           0x3ffff0ff,  /* 245: UNASSIGNED off */
           0x3ffff0ff,  /* 246: UNASSIGNED off */
           0x3ffff0ff,  /* 247: UNASSIGNED off */
           0xbffff0ff,  /* 248: MEMC_TRACELOG RR */
           0x3ffff0ff,  /* 249: UNASSIGNED off */
           0xbfffe05e,  /* 250: MEMC_ZQCS RR 1000000ns */
           0xbffff0ff,  /* 251: MEMC_MSA RR */
           0xbffff0ff,  /* 252: MEMC_DIS0 RR */
           0xbffff0ff,  /* 253: MEMC_DIS1 RR */
           0xbffff0ff,  /* 254: MEMC_DRAM_INIT_ZQCS RR */
           0x004ef019,  /* 255: REFRESH 3906.25ns */
         };


static const BBOX_Rts_PfriClient stBoxRts_PfriClient_20150202223821_4ku0t[] = {
  {BCHP_MEMC_GEN_0_PFRI_0_THROTTLE_CONFIG,      0x806c0802},
  {BCHP_MEMC_GEN_0_PFRI_0_THROTTLE_FILTER_CTRL, 0x4000060d},
  {BCHP_MEMC_GEN_0_PFRI_0_THROTTLE_THRESH0,     0x00000d40},
  {BCHP_MEMC_GEN_0_PFRI_0_THROTTLE_THRESH1,     0x000007f3},
  {BCHP_MEMC_GEN_0_PFRI_1_THROTTLE_CONFIG,      0x806b0803},
  {BCHP_MEMC_GEN_0_PFRI_1_THROTTLE_FILTER_CTRL, 0x400001ea},
  {BCHP_MEMC_GEN_0_PFRI_1_THROTTLE_THRESH0,     0x000001a0},
  {BCHP_MEMC_GEN_0_PFRI_1_THROTTLE_THRESH1,     0x000000f9},
  {BCHP_MEMC_GEN_1_PFRI_0_THROTTLE_CONFIG,      0x806d0803},
  {BCHP_MEMC_GEN_1_PFRI_0_THROTTLE_FILTER_CTRL, 0x4000060d},
  {BCHP_MEMC_GEN_1_PFRI_0_THROTTLE_THRESH0,     0x00001642},
  {BCHP_MEMC_GEN_1_PFRI_0_THROTTLE_THRESH1,     0x00000d5a},
};

static const uint32_t * const paulMemc_box5[] = { &aulMemc0_20150202223821_4ku0t[0], &aulMemc1_20150202223821_4ku0t[0],};

const BBOX_Rts stBoxRts_4ku0t_box5 = {
  "20150202223821_4ku0t_box5",
  7366,
  5,
  2,
  256,
  (const uint32_t **)&paulMemc_box5[0],
  12,
  stBoxRts_PfriClient_20150202223821_4ku0t,
};
