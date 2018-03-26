/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
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

 ******************************************************************************/
#ifndef _BWFE_45308_PRIV_H__
#define _BWFE_45308_PRIV_H__

#include "bchp_leap_l1.h"
#include "bchp_aif_wb_sat_core0.h"
#include "bchp_aif_wb_sat_core_intr2_0.h"
#include "bchp_aif_wb_sat_ana.h"


#define BWFE_BYPASS_DGS
#define BWFE_BYPASS_LIC
/*#define BWFE_BYPASS_LIC2*/
#define BWFE_BYPASS_INL
#define BWFE_HYBRID_ADC

#define BWFE_G3_BUILD_VERSION 0x01
#define BWFE_NUM_CHANNELS     2     /* 1 input ADC x 2 */
#define BWFE_CHANNELS_MASK    0x0F
#define BWFE_NUM_REF_CHANNELS 0
#define BWFE_NUM_SLICES       2
#define BWFE_NUM_REF_SLICES   1
#define BWFE_NUM_LANES        2
#define BWFE_NUM_DMX_LANES    4
#define BWFE_NUM_MDACS        1
#define BWFE_NUM_LIC_TAPS     16    /* TBD 16 taps per lane, no DCO tap */
#define BWFE_NUM_EQ_TAPS      4
#define BWFE_NUM_ADC_PHASES   8     /* TBD */
#define BWFE_XTAL_FREQ_KHZ    54000

#define BWFE_LIC_L            (BWFE_NUM_SLICES * BWFE_NUM_LANES * BWFE_NUM_DMX_LANES)
#define BWFE_LIC_S            8  /* TBD */

#define BWFE_DEF_FS_ADC_KHZ   4968000  /* Fs_adc = 4968MHz */
#define BWFE_DEF_FC_DPM_KHZ   2400800  /* Fc_dpm = 2400.8MHz */


/* wfe channel 0 interrupts */
#define BCHP_INT_ID_WFE_TIMER0_0          BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_TIMER0_INTR_SHIFT)
#define BCHP_INT_ID_WFE_TIMER1_0          BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_TIMER1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CORRDONE_0        BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CORR_CNT_DONE_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PO_SLC1_0  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PONG_SLC1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PO_SLC0_0  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PONG_SLC0_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PI_SLC1_0  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PING_SLC1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PI_SLC0_0  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_0_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PING_SLC0_INTR_SHIFT)

/* wfe channel 1 interrupts */
#define BCHP_INT_ID_WFE_TIMER0_1          BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_TIMER0_INTR_SHIFT)
#define BCHP_INT_ID_WFE_TIMER1_1          BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_TIMER1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CORRDONE_1        BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CORR_CNT_DONE_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PO_SLC1_1  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PONG_SLC1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PO_SLC0_1  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PONG_SLC0_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PI_SLC1_1  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PING_SLC1_INTR_SHIFT)
#define BCHP_INT_ID_WFE_CLPDTR_PI_SLC0_1  BINT_LEAP_INT_ID_CREATE(BCHP_LEAP_L1_INTR_W1_STATUS_AIF_WB_SAT_TOP_1_INST_INTR_SHIFT+32, BCHP_AIF_WB_SAT_CORE_INTR2_0_CPU_STATUS_CLPDTR_PING_SLC0_INTR_SHIFT)


/* WFE base registers */
#define BCHP_WFE_CORE_RSTCTL                       BCHP_AIF_WB_SAT_CORE0_RSTCTL
#define BCHP_WFE_CORE_HDOFFCTL0                    BCHP_AIF_WB_SAT_CORE0_HDOFFCTL0
#define BCHP_WFE_CORE_HDOFFCTL1                    BCHP_AIF_WB_SAT_CORE0_HDOFFCTL1
#define BCHP_WFE_CORE_HDOFFCTL2                    BCHP_AIF_WB_SAT_CORE0_HDOFFCTL2
#define BCHP_WFE_CORE_HDOFFSTS                     BCHP_AIF_WB_SAT_CORE0_HDOFFSTS
#define BCHP_WFE_CORE_DGSCTL2                      BCHP_AIF_WB_SAT_CORE0_DGSCTL2
#define BCHP_WFE_CORE_MDACSASTS                    BCHP_AIF_WB_SAT_CORE0_MDACSASTS
#define BCHP_WFE_CORE_DOUTCTL                      BCHP_AIF_WB_SAT_CORE0_DOUTCTL
#define BCHP_WFE_CORE_CORRINSEL                    BCHP_AIF_WB_SAT_CORE0_CORRINSEL
#define BCHP_WFE_CORE_CORRCTL                      BCHP_AIF_WB_SAT_CORE0_CORRCTL
#define BCHP_WFE_CORE_CORRFCW                      BCHP_AIF_WB_SAT_CORE0_CORRFCW
#define BCHP_WFE_CORE_CORRTHR                      BCHP_AIF_WB_SAT_CORE0_CORRTHR
#define BCHP_WFE_CORE_CORRFCWEN                    BCHP_AIF_WB_SAT_CORE0_CORRFCWEN
#define BCHP_WFE_CORE_CORRLEN0                     BCHP_AIF_WB_SAT_CORE0_CORRLEN0
#define BCHP_WFE_CORE_CORRLEN1                     BCHP_AIF_WB_SAT_CORE0_CORRLEN1
#define BCHP_WFE_CORE_TIMERCTL0                    BCHP_AIF_WB_SAT_CORE0_TIMERCTL0
#define BCHP_WFE_CORE_TIMERCTL1                    BCHP_AIF_WB_SAT_CORE0_TIMERCTL1
#define BCHP_WFE_CORE_AGCCTL2                      BCHP_AIF_WB_SAT_CORE0_AGCCTL2
#define BCHP_WFE_CORE_AGCDECRATE                   BCHP_AIF_WB_SAT_CORE0_AGCDECRATE
#define BCHP_WFE_CORE_AGCCTL1                      BCHP_AIF_WB_SAT_CORE0_AGCCTL1
#define BCHP_WFE_CORE_AGCTHRA1                     BCHP_AIF_WB_SAT_CORE0_AGCTHRA1
#define BCHP_WFE_CORE_REG_AGC_LF_INT_WDATA         BCHP_AIF_WB_SAT_CORE0_REG_AGC_LF_INT_WDATA
#define BCHP_WFE_CORE_REG_AGC_LA_INT_WDATA         BCHP_AIF_WB_SAT_CORE0_REG_AGC_LA_INT_WDATA
#define BCHP_WFE_CORE_REG_AGC_CTRL_LF_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_AGC_CTRL_LF_INT_WDATA
#define BCHP_WFE_CORE_REG_AGC_CTRL_LA_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_AGC_CTRL_LA_INT_WDATA
#define BCHP_WFE_CORE_AGCTHRA2                     BCHP_AIF_WB_SAT_CORE0_AGCTHRA2
#define BCHP_WFE_CORE_PGACLKCTL                    BCHP_AIF_WB_SAT_CORE0_PGACLKCTL
#define BCHP_WFE_CORE_PGALUTD                      BCHP_AIF_WB_SAT_CORE0_PGALUTD
#define BCHP_WFE_CORE_PGALUTA                      BCHP_AIF_WB_SAT_CORE0_PGALUTA
#define BCHP_WFE_CORE_NRNOTCHCTL                   BCHP_AIF_WB_SAT_CORE0_NRNOTCHCTL
#define BCHP_WFE_CORE_NRAGCTHR                     BCHP_AIF_WB_SAT_CORE0_NRAGCTHR
#define BCHP_WFE_CORE_NRDCOCTL_THR                 BCHP_AIF_WB_SAT_CORE0_NRDCOCTL_THR
#define BCHP_WFE_CORE_REG_NR_DCO_INT_WDATA_THR     BCHP_AIF_WB_SAT_CORE0_REG_NR_DCO_INT_WDATA_THR
#define BCHP_WFE_CORE_CORE_SW_SPARE0               BCHP_AIF_WB_SAT_CORE0_CORE_SW_SPARE0
#define BCHP_WFE_CORE_CORE_SW_SPARE1               BCHP_AIF_WB_SAT_CORE0_CORE_SW_SPARE1

/* WFE slice registers */
#define BCHP_WFE_CORE_DGSCTL_PI_SLC          BCHP_AIF_WB_SAT_CORE0_DGSCTL_PI_SLC0
#define BCHP_WFE_CORE_DGSCTL_PO_SLC          BCHP_AIF_WB_SAT_CORE0_DGSCTL_PO_SLC0
#define BCHP_WFE_CORE_DGSCLP_PI_SLC          BCHP_AIF_WB_SAT_CORE0_DGSCLP_PI_SLC0
#define BCHP_WFE_CORE_DGSCLP_PO_SLC          BCHP_AIF_WB_SAT_CORE0_DGSCLP_PO_SLC0
#define BCHP_WFE_CORE_DGSHIST_PI_SLC         BCHP_AIF_WB_SAT_CORE0_DGSHIST_PI_SLC0
#define BCHP_WFE_CORE_DGSHIST_PO_SLC         BCHP_AIF_WB_SAT_CORE0_DGSHIST_PO_SLC0
#define BCHP_WFE_CORE_DGSCLPCNT_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSCLPCNT_PI_SLC0
#define BCHP_WFE_CORE_DGSCLPCNT_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSCLPCNT_PO_SLC0
#define BCHP_WFE_CORE_DGSACCUM_PI_SLC        BCHP_AIF_WB_SAT_CORE0_DGSACCUM_PI_SLC0
#define BCHP_WFE_CORE_DGSACCUM_PO_SLC        BCHP_AIF_WB_SAT_CORE0_DGSACCUM_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT011_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT011_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT010_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT010_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT001_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT001_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT000_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT000_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT111_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT111_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT110_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT110_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT101_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT101_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT100_PI_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT100_PI_SLC0
#define BCHP_WFE_CORE_DGSLUT011_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT011_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT010_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT010_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT001_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT001_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT000_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT000_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT111_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT111_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT110_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT110_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT101_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT101_PO_SLC0
#define BCHP_WFE_CORE_DGSLUT100_PO_SLC       BCHP_AIF_WB_SAT_CORE0_DGSLUT100_PO_SLC0
#define BCHP_WFE_CORE_DGSLMS_SLC             BCHP_AIF_WB_SAT_CORE0_DGSLMS_SLC0
#define BCHP_WFE_CORE_DGSBGLMS_SLC           BCHP_AIF_WB_SAT_CORE0_DGSBGLMS_SLC0
#define BCHP_WFE_CORE_DGSLMSMU_SLC           BCHP_AIF_WB_SAT_CORE0_DGSLMSMU_SLC0
#define BCHP_WFE_CORE_DGSCOEFD_SLC           BCHP_AIF_WB_SAT_CORE0_DGSCOEFD_SLC0
#define BCHP_WFE_CORE_DGSCOEFA_SLC           BCHP_AIF_WB_SAT_CORE0_DGSCOEFA_SLC0
#define BCHP_WFE_CORE_DGSCOEFEN_SLC          BCHP_AIF_WB_SAT_CORE0_DGSCOEFEN_SLC0
#define BCHP_WFE_CORE_MDACSA_SLC             BCHP_AIF_WB_SAT_CORE0_MDACSA_SLC0
#define BCHP_WFE_CORE_MDACSADU_SLC           BCHP_AIF_WB_SAT_CORE0_MDACSADU_SLC0
#define BCHP_WFE_CORE_MDACSAOUT_SLC          BCHP_AIF_WB_SAT_CORE0_MDACSAOUT_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRI0_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI0_DMX3_PO_SLC0
#define BCHP_WFE_CORE_CORRI1_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRI1_DMX3_PO_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRQ0_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ0_DMX3_PO_SLC0
#define BCHP_WFE_CORE_CORRQ1_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRQ1_DMX3_PO_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX0_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX0_PI_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX0_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX0_PO_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX1_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX1_PI_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX1_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX1_PO_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX2_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX2_PI_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX2_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX2_PO_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX3_PI_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX3_PI_SLC0
#define BCHP_WFE_CORE_CORRP0_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP0_DMX3_PO_SLC0
#define BCHP_WFE_CORE_CORRP1_DMX3_PO_SLC     BCHP_AIF_WB_SAT_CORE0_CORRP1_DMX3_PO_SLC0
#define BCHP_WFE_CORE_DGSEPCTL_SLC           BCHP_AIF_WB_SAT_CORE0_DGSEPCTL_SLC0
#define BCHP_WFE_CORE_REG_DGSEP_DMX0_PI_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX0_PI_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX0_PO_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX0_PO_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX1_PI_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX1_PI_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX1_PO_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX1_PO_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX2_PI_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX2_PI_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX2_PO_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX2_PO_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX3_PI_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX3_PI_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_DMX3_PO_SLC_INT_WDATA    BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_DMX3_PO_SLC0_INT_WDATA
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX0_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX0_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX0_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX0_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX0_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX0_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX0_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX0_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX1_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX1_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX1_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX1_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX1_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX1_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX1_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX1_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX2_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX2_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX2_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX2_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX2_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX2_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX2_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX2_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX3_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX3_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX3_PI_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX3_PI_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S1_DMX3_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S1_DMX3_PO_SLC0_ERRP
#define BCHP_WFE_CORE_REG_DGSEP_S2_DMX3_PO_SLC_ERRP      BCHP_AIF_WB_SAT_CORE0_REG_DGSEP_S2_DMX3_PO_SLC0_ERRP
#define BCHP_WFE_CORE_NRDCOCTL_PI_SLC                    BCHP_AIF_WB_SAT_CORE0_NRDCOCTL_PI_SLC0
#define BCHP_WFE_CORE_REG_NR_DCO_INT_WDATA_PI_SLC        BCHP_AIF_WB_SAT_CORE0_REG_NR_DCO_INT_WDATA_PI_SLC0
#define BCHP_WFE_CORE_NRDCOCTL_PO_SLC                    BCHP_AIF_WB_SAT_CORE0_NRDCOCTL_PO_SLC0
#define BCHP_WFE_CORE_REG_NR_DCO_INT_WDATA_PO_SLC        BCHP_AIF_WB_SAT_CORE0_REG_NR_DCO_INT_WDATA_PO_SLC0
#define BCHP_WFE_CORE_NRNOTCHCTL_PI_SLC                  BCHP_AIF_WB_SAT_CORE0_NRNOTCHCTL_PI_SLC0
#define BCHP_WFE_CORE_REG_NR_NOTCH_INT_WDATA_PI_SLC      BCHP_AIF_WB_SAT_CORE0_REG_NR_NOTCH_INT_WDATA_PI_SLC0
#define BCHP_WFE_CORE_NRNOTCHCTL_PO_SLC                  BCHP_AIF_WB_SAT_CORE0_NRNOTCHCTL_PO_SLC0
#define BCHP_WFE_CORE_REG_NR_NOTCH_INT_WDATA_PO_SLC      BCHP_AIF_WB_SAT_CORE0_REG_NR_NOTCH_INT_WDATA_PO_SLC0
#define BCHP_WFE_CORE_NRAGCCTL_PI_SLC                    BCHP_AIF_WB_SAT_CORE0_NRAGCCTL_PI_SLC0
#define BCHP_WFE_CORE_REG_NR_AGC_LF_INT_WDATA_PI_SLC     BCHP_AIF_WB_SAT_CORE0_REG_NR_AGC_LF_INT_WDATA_PI_SLC0
#define BCHP_WFE_CORE_REG_NR_AGC_LA_INT_WDATA_PI_SLC     BCHP_AIF_WB_SAT_CORE0_REG_NR_AGC_LA_INT_WDATA_PI_SLC0
#define BCHP_WFE_CORE_NRAGCCTL_PO_SLC                    BCHP_AIF_WB_SAT_CORE0_NRAGCCTL_PO_SLC0
#define BCHP_WFE_CORE_REG_NR_AGC_LF_INT_WDATA_PO_SLC     BCHP_AIF_WB_SAT_CORE0_REG_NR_AGC_LF_INT_WDATA_PO_SLC0
#define BCHP_WFE_CORE_REG_NR_AGC_LA_INT_WDATA_PO_SLC     BCHP_AIF_WB_SAT_CORE0_REG_NR_AGC_LA_INT_WDATA_PO_SLC0

/* WFE analog base registers */
#define BCHP_WFE_ANA_RFFE_WRITER01     BCHP_AIF_WB_SAT_ANA_RFFE0_WRITER01
#define BCHP_WFE_ANA_RFFE_WRITER02     BCHP_AIF_WB_SAT_ANA_RFFE0_WRITER02
#define BCHP_WFE_ANA_RFFE_WRITER03     BCHP_AIF_WB_SAT_ANA_RFFE0_WRITER03
#define BCHP_WFE_ANA_ADC_CNTL0         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL0
#define BCHP_WFE_ANA_ADC_CNTL1         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL1
#define BCHP_WFE_ANA_ADC_CNTL2         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL2
#define BCHP_WFE_ANA_ADC_CNTL3         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL3
#define BCHP_WFE_ANA_ADC_CNTL4         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL4
#define BCHP_WFE_ANA_ADC_CNTL5         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL5
#define BCHP_WFE_ANA_ADC_CNTL6         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL6
#define BCHP_WFE_ANA_ADC_CNTL7         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL7
#define BCHP_WFE_ANA_ADC_CNTL8         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL8
#define BCHP_WFE_ANA_ADC_CNTL9         BCHP_AIF_WB_SAT_ANA_ADC0_CNTL9
#define BCHP_WFE_ANA_ADC_CNTL10        BCHP_AIF_WB_SAT_ANA_ADC0_CNTL10
#define BCHP_WFE_ANA_ADC_CNTL11        BCHP_AIF_WB_SAT_ANA_ADC0_CNTL11
#define BCHP_WFE_ANA_ADC_CNTL12        BCHP_AIF_WB_SAT_ANA_ADC0_CNTL12
#define BCHP_WFE_ANA_ADC_CNTL13        BCHP_AIF_WB_SAT_ANA_ADC0_CNTL13
#define BCHP_WFE_ANA_PLL_CNTL0         BCHP_AIF_WB_SAT_ANA_PLL_CNTL0
#define BCHP_WFE_ANA_PLL_CNTL1         BCHP_AIF_WB_SAT_ANA_PLL_CNTL1
#define BCHP_WFE_ANA_PLL_CNTL2         BCHP_AIF_WB_SAT_ANA_PLL_CNTL2
#define BCHP_WFE_ANA_PLL_CNTL3         BCHP_AIF_WB_SAT_ANA_PLL_CNTL3
#define BCHP_WFE_ANA_PLL_CNTL4         BCHP_AIF_WB_SAT_ANA_PLL_CNTL4
#define BCHP_WFE_ANA_PLL_CNTL5         BCHP_AIF_WB_SAT_ANA_PLL_CNTL5
#define BCHP_WFE_ANA_PLL_CNTL6         BCHP_AIF_WB_SAT_ANA_PLL_CNTL6
#define BCHP_WFE_ANA_PLL_CNTL7         BCHP_AIF_WB_SAT_ANA_PLL_CNTL7
#define BCHP_WFE_ANA_SYS_CNTL          BCHP_AIF_WB_SAT_ANA_SYS_CNTL0
#define BCHP_WFE_ANA_PLL_STAT          BCHP_AIF_WB_SAT_ANA_PLL_STAT
#define BCHP_WFE_ANA_DPM_DAC_R00       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R00     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R01       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R01     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R02       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R02     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R03       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R03     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R04       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R04     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R05       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R05     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R06       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R06     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_DAC_R07       BCHP_AIF_WB_SAT_ANA_DPM_DAC_R07     /* new in 45308 */
#define BCHP_WFE_ANA_DPM_CNTL          BCHP_AIF_WB_SAT_ANA_DPM_CNTL        /* new in 45308 */
#define BCHP_WFE_ANA_DPM_STAT_OUT0     BCHP_AIF_WB_SAT_ANA_DPM_STAT_OUT0   /* new in 45308 */
#define BCHP_WFE_ANA_DPM_STAT_OUT1     BCHP_AIF_WB_SAT_ANA_DPM_STAT_OUT1   /* new in 45308 */
#define BCHP_WFE_ANA_CLK_CTRL          BCHP_AIF_WB_SAT_ANA_CLK_CTRL_IN0
#define BCHP_WFE_ANA_PGA_GAIN          BCHP_AIF_WB_SAT_ANA_PGA_GAIN_IN0
#define BCHP_WFE_ANA_SW_SPARE0         BCHP_AIF_WB_SAT_ANA_SW_SPARE0
#define BCHP_WFE_ANA_SW_SPARE1         BCHP_AIF_WB_SAT_ANA_SW_SPARE1

#endif /* BWFE_45308_PRIV_H__ */
