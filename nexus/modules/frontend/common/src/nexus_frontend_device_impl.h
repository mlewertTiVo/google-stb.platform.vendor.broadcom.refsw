/***************************************************************************
*     (c)2014 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
*
* $brcm_Workfile: $
* $brcm_Revision: $
* $brcm_Date: $
*
* API Description:
*   API name: Frontend SAT
*    Generic routines to control any SAT satellite frontend
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/
#ifndef NEXUS_FRONTEND_DEVICE_IMPL_H__
#define NEXUS_FRONTEND_DEVICE_IMPL_H__

#include "nexus_frontend_module.h"

/* Key off of both decimal and hex versions of CHIP.
 * A direct read of the family ID/chip ID usually returns hexadecimal.
 * New five-digit chips and non-numeric chip identifiers might read differently in the future. */
#define NEXUS_PLATFORM_P_FRONTEND_SUPPORT(CHIP) {CHIP, NEXUS_FrontendDevice_P_Open##CHIP, NEXUS_Frontend_P_Open##CHIP},\
                                                {0x##CHIP, NEXUS_FrontendDevice_P_Open##CHIP, NEXUS_Frontend_P_Open##CHIP},

#define NEXUS_PLATFORM_P_FRONTEND_SUPPORT_FAMILY(CHIP,FE_CHIP) {CHIP, NEXUS_FrontendDevice_P_Open##FE_CHIP, NEXUS_Frontend_P_Open##FE_CHIP},\
                                                {0x##CHIP, NEXUS_FrontendDevice_P_Open##FE_CHIP, NEXUS_Frontend_P_Open##FE_CHIP},

#ifdef NEXUS_FRONTEND_3128
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open3128(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open3128(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_3461
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open3461(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open3461(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_4538
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open4538(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open4538(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_45216
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open45216(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open45216(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_45308
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open45308(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open45308(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_7364
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open7364(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open7364(const NEXUS_FrontendChannelSettings *pSettings);
#endif
#ifdef NEXUS_FRONTEND_7366
    NEXUS_FrontendDeviceHandle NEXUS_FrontendDevice_P_Open7366(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle NEXUS_Frontend_P_Open7366(const NEXUS_FrontendChannelSettings *pSettings);
#endif


static struct {
    unsigned chipid;
    NEXUS_FrontendDeviceHandle (*openDevice)(unsigned index, const NEXUS_FrontendDeviceOpenSettings *pSettings);
    NEXUS_FrontendHandle (*openChannel)(const NEXUS_FrontendChannelSettings *pSettings);
} g_frontends[] = {
#ifdef NEXUS_FRONTEND_3128
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(3128)
#endif
#ifdef NEXUS_FRONTEND_3461
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(3461)
#endif
#ifdef NEXUS_FRONTEND_4538
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(4538)
#endif
#ifdef NEXUS_FRONTEND_45216
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(45216)
#endif
#ifdef NEXUS_FRONTEND_45308
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(45308)
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT_FAMILY(45316,45308)
#endif
#ifdef NEXUS_FRONTEND_7364
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(7364)
#endif
#ifdef NEXUS_FRONTEND_7366
    NEXUS_PLATFORM_P_FRONTEND_SUPPORT(7366)
#endif
    {0, NULL, NULL}
};

#endif /* #ifndef NEXUS_FRONTEND_DEVICE_IMPL_H__ */
