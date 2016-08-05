/***************************************************************************
 * Copyright (C) 2016 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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
 *
* Module Description:
*
***************************************************************************/
/***************************************************************
*
* This file is auto-generated by generate_chp_pwr.pl, based on
* bchp_pwr_resources.txt.
*
* This file contains a list of private power resource IDs that
* represent HW clocks, and function prototypes for controlling
* them.
*
***************************************************************/

#ifndef BCHP_PWR_RESOURCES_PRIV_H__
#define BCHP_PWR_RESOURCES_PRIV_H__

#include "bchp_pwr.h"

/* Private power resource IDs */
#define BCHP_PWR_HW_HVD0_CORE_CLK          0xff000001
#define BCHP_PWR_HW_HVD0_CPU_CLK           0xff000002
#define BCHP_PWR_HW_HVD0_GISB_CLK          0xff000003
#define BCHP_PWR_HW_HVD0_SCB_54_108_CLK    0xff000004
#define BCHP_PWR_HW_HVD0_SRAM              0xff000005
#define BCHP_PWR_HW_HVD1_CORE_CLK          0xff000006
#define BCHP_PWR_HW_HVD1_CPU_CLK           0xff000007
#define BCHP_PWR_HW_HVD1_GISB_CLK          0xff000008
#define BCHP_PWR_HW_HVD1_54_CLK            0xff000009
#define BCHP_PWR_HW_HVD1_SCB_108_CLK       0xff00000a
#define BCHP_PWR_HW_HVD1_SRAM              0xff00000b
#define BCHP_PWR_HW_HVD2_CORE_CLK          0xff00000c
#define BCHP_PWR_HW_HVD2_CPU_CLK           0xff00000d
#define BCHP_PWR_HW_HVD2_GISB_CLK          0xff00000e
#define BCHP_PWR_HW_HVD2_SCB_54_108_CLK    0xff00000f
#define BCHP_PWR_HW_HVD2_SRAM              0xff000010
#define BCHP_PWR_HW_VICE0_CORE_CLK         0xff000011
#define BCHP_PWR_HW_VICE0_GISB_CLK         0xff000012
#define BCHP_PWR_HW_VICE0_CLK              0xff000013
#define BCHP_PWR_HW_VICE0_SRAM             0xff000014
#define BCHP_PWR_HW_VICE1_CORE_CLK         0xff000015
#define BCHP_PWR_HW_VICE1_GISB_CLK         0xff000016
#define BCHP_PWR_HW_VICE1_CLK              0xff000017
#define BCHP_PWR_HW_VICE1_SRAM             0xff000018
#define BCHP_PWR_HW_AIO                    0xff000019
#define BCHP_PWR_HW_AIO_SRAM               0xff00001a
#define BCHP_PWR_HW_RAAGA0_GISB_CLK        0xff00001b
#define BCHP_PWR_HW_RAAGA0_CLK             0xff00001c
#define BCHP_PWR_HW_RAAGA0_DSP             0xff00001d
#define BCHP_PWR_HW_VEC_108                0xff00001e
#define BCHP_PWR_HW_AUD_DAC                0xff00001f
#define BCHP_PWR_HW_RAAGA0_SRAM            0xff000020
#define BCHP_PWR_HW_RAAGA1_GISB_CLK        0xff000021
#define BCHP_PWR_HW_RAAGA1_CLK             0xff000022
#define BCHP_PWR_HW_RAAGA1_DSP             0xff000023
#define BCHP_PWR_HW_RAAGA1_SRAM            0xff000024
#define BCHP_PWR_HW_BVN                    0xff000025
#define BCHP_PWR_HW_BVN_BVB_GISB           0xff000026
#define BCHP_PWR_HW_BVN_SRAM               0xff000027
#define BCHP_PWR_HW_VDC_DAC                0xff000028
#define BCHP_PWR_HW_VEC                    0xff000029
#define BCHP_PWR_HW_VEC_SRAM               0xff00002a
#define BCHP_PWR_HW_VDC_656_OUT            0xff00002b
#define BCHP_PWR_HW_HDMI_TX0_PHY           0xff00002c
#define BCHP_PWR_HW_HDMI_RX0_CLK           0xff00002d
#define BCHP_PWR_HW_XPT_RMX                0xff00002e
#define BCHP_PWR_HW_XPT_CORE_GISB          0xff00002f
#define BCHP_PWR_HW_XPT_XMEMIF             0xff000030
#define BCHP_PWR_HW_SECTOP_XPT             0xff000031
#define BCHP_PWR_HW_XPT_WAKEUP             0xff000032
#define BCHP_PWR_HW_HDMI_TX_CLK            0xff000033
#define BCHP_PWR_HW_HDMI_TX_108M           0xff000034
#define BCHP_PWR_HW_HDMI_RX0_RBUS          0xff000035
#define BCHP_PWR_HW_HDMI_RX0_PHY           0xff000036
#define BCHP_PWR_HW_HDMI_RX0_SRAM          0xff000037
#define BCHP_PWR_HW_M2MC0                  0xff000038
#define BCHP_PWR_HW_M2MC1                  0xff000039
#define BCHP_PWR_HW_M2MC0_SRAM             0xff00003a
#define BCHP_PWR_HW_M2MC1_SRAM             0xff00003b
#define BCHP_PWR_HW_V3D                    0xff00003c
#define BCHP_PWR_HW_V3D_SRAM               0xff00003d
#define BCHP_PWR_HW_SCD0                   0xff00003e
#define BCHP_PWR_HW_SCD1                   0xff00003f
#define BCHP_PWR_HW_TEMP_MONITOR           0xff000040
#define BCHP_PWR_HW_SID                    0xff000041
#define BCHP_PWR_HW_SID_SRAM               0xff000042
#define BCHP_PWR_HW_RFM                    0xff000043
#define BCHP_PWR_HW_RFM_PHY                0xff000044
#define BCHP_PWR_HW_PLL_HVD_CH0            0xff000045
#define BCHP_PWR_HW_PLL_HVD_CH1            0xff000046
#define BCHP_PWR_HW_PLL_HVD_CH2            0xff000047
#define BCHP_PWR_HW_PLL_HVD_CH3            0xff000048
#define BCHP_PWR_HW_PLL_HVD                0xff000049
#define BCHP_PWR_HW_AUD_PLL0               0xff00004a
#define BCHP_PWR_HW_AUD_PLL1               0xff00004b
#define BCHP_PWR_HW_AUD_PLL2               0xff00004c
#define BCHP_PWR_HW_PLL_VCXO_PLL0_CH0      0xff00004d
#define BCHP_PWR_HW_PLL_VCXO_PLL0_CH2      0xff00004e
#define BCHP_PWR_HW_PLL_VCXO_PLL0          0xff00004f
#define BCHP_PWR_HW_PLL_VCXO_PLL1_CH0      0xff000050
#define BCHP_PWR_HW_PLL_VCXO_PLL1_CH2      0xff000051
#define BCHP_PWR_HW_PLL_VCXO_PLL1          0xff000052
#define BCHP_PWR_HW_PLL_VCXO_PLL2_CH0      0xff000053
#define BCHP_PWR_HW_PLL_VCXO_PLL2_CH2      0xff000054
#define BCHP_PWR_HW_PLL_VCXO_PLL2          0xff000055
#define BCHP_PWR_HW_PLL_MOCA_CH3           0xff000056
#define BCHP_PWR_HW_PLL_MOCA_CH4           0xff000057
#define BCHP_PWR_HW_PLL_SCD0_CH0           0xff000058
#define BCHP_PWR_HW_PLL_SCD0               0xff000059
#define BCHP_PWR_HW_PLL_SCD1_CH0           0xff00005a
#define BCHP_PWR_HW_PLL_SCD1               0xff00005b
#define BCHP_PWR_HW_PLL_RAAGA_PLL_CH0      0xff00005c
#define BCHP_PWR_HW_PLL_RAAGA_PLL_CH1      0xff00005d
#define BCHP_PWR_HW_PLL_RAAGA_PLL_CH3      0xff00005e
#define BCHP_PWR_HW_PLL_RAAGA_PLL_CH2      0xff00005f
#define BCHP_PWR_HW_PLL_RAAGA              0xff000060
#define BCHP_PWR_DV_PLL_HVD_CH0            0xff000061
#define BCHP_PWR_DV_PLL_HVD_CH1            0xff000062
#define BCHP_PWR_DV_PLL_HVD_CH2            0xff000063
#define BCHP_PWR_DV_PLL_HVD_CH3            0xff000064
#define BCHP_PWR_DV_PLL_MOCA_CH3           0xff000065
#define BCHP_PWR_DV_PLL_MOCA_CH4           0xff000066
#define BCHP_PWR_DV_PLL_RAAGA_PLL_CH0      0xff000067
#define BCHP_PWR_DV_PLL_RAAGA_PLL_CH1      0xff000068
#define BCHP_PWR_DV_PLL_RAAGA_PLL_CH2      0xff000069
#define BCHP_PWR_DV_PLL_RAAGA_PLL_CH3      0xff00006a

/* This is the link between the public and private interface */
void BCHP_PWR_P_HW_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, bool activate);
void BCHP_PWR_P_HW_ControlId(BCHP_Handle handle, unsigned id, bool activate);
void BCHP_PWR_P_MUX_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, unsigned *mux, bool set);
void BCHP_PWR_P_DIV_Control(BCHP_Handle handle, const BCHP_PWR_P_Resource *resource, unsigned *mult, unsigned *prediv, unsigned *postdiv, bool set);

#define BCHP_PWR_P_NUM_NONLEAFS   78
#define BCHP_PWR_P_NUM_NONLEAFSHW 45
#define BCHP_PWR_P_NUM_LEAFS      51
#define BCHP_PWR_P_NUM_MUXES      0
#define BCHP_PWR_P_NUM_DIVS       10
#define BCHP_PWR_P_NUM_ALLNODES   184

#endif
