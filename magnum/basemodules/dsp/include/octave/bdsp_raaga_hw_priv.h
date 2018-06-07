/******************************************************************************
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
 *****************************************************************************/

#ifndef BDSP_RAAGA_HW_PRIV_H_
#define BDSP_RAAGA_HW_PRIV_H_

#include "bdsp_raaga_priv_include.h"

#define BDSP_RAAGA_DEFAULT_WATCHDOG_COUNT   (0x3FFFFFFF)
/* We should program only upper 12 bit in 40-bit addressing
 * In 7278 this is 40 bit addressing and hence (40-12 = 28)
 * For a given chip, make sure to set the value correct */
#define SCB_REG_ADDR_WRITE_SHIFT_VALUE 28

BERR_Code BDSP_Raaga_P_ResetHardware_isr(
	BDSP_Raaga *pRaaga,
	unsigned uiDspIndex
);
BERR_Code BDSP_Raaga_P_Reset(
    BDSP_Raaga *pRaaga
);
BERR_Code BDSP_Raaga_P_Boot(
    BDSP_Raaga *pDevice
);
void BDSP_Raaga_P_EnableDspWatchdogTimer (
    BDSP_Raaga *pDevice,
    uint32_t    dspIndex,
    bool        bEnable
);
BERR_Code BDSP_Raaga_P_WriteAtuEntry(
    BREG_Handle hRegister,
    BDSP_Raaga_P_ATUInfo *psATUInfo
);
BERR_Code BDSP_Raaga_P_ProgramAtuEntries(
    BDSP_Raaga *pDevice,
    unsigned dspindex
);
void BDSP_Raaga_P_DirectRaagaUartToPort(
    BREG_Handle regHandle
);
void BDSP_Raaga_P_EnableAllPwrResource(
	void *pDeviceHandle,
	bool bEnable
);
BERR_Code BDSP_Raaga_P_PowerStandby(
	void 					*pDeviceHandle,
	BDSP_StandbySettings    *pSettings
);
BERR_Code BDSP_Raaga_P_PowerResume(
	void *pDeviceHandle
);
BERR_Code BDSP_Raaga_P_Initialize(
	void  *pDeviceHandle
);
void BDSP_Raaga_P_SetDspClkRate(
	BDSP_Raaga *pRaaga,
	unsigned expectedDspClkRate,
	unsigned dspIndex );

BERR_Code BDSP_Raaga_P_GetDefaultClkRate(
	BDSP_Raaga *pRaaga,
	unsigned dspIndex,
	unsigned *pDefaultDspClkRate );

void    BDSP_Raaga_P_GetLowerDspClkRate(
	unsigned dspClkRate,
	unsigned *lowerDspClkRate);

#endif /*BDSP_RAAGA_HW_PRIV_H_*/
