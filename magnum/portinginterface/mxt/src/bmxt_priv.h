/******************************************************************************
 *  Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
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

#ifndef BMXT_PRIV_H__
#define BMXT_PRIV_H__

#include "breg_mem.h"
#include "bchp.h"
#include "bmxt.h"
#include "bmxt_dcbg.h"

#define BMXT_NOREG 0xffffffff

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BMXT_P_PlatformType
{
    BMXT_P_PlatformType_eHab,
    BMXT_P_PlatformType_eRpc,
    BMXT_P_PlatformType_eReg
} BMXT_P_PlatformType;

typedef struct BMXT_P_TransportData
{
    BCHP_Handle hChp;
    BREG_Handle hReg;
    BMXT_Settings settings;
    void* hHab;
    void* hRpc;
    BMXT_PowerSaverSettings powerSettings;
    unsigned dumpIndex;

    struct {
        unsigned *num, *numDcbg;
        uint32_t regbase, regbaseWakeup, regbaseDcbg;
        uint32_t *regoffsets, *regoffsetsWakeup, *regoffsetsDcbg;
        unsigned *stepsize, *stepsizeDcbg;
        BMXT_P_PlatformType type;
    } platform;

    unsigned numDcbg; /* total number of DCBG channels opened */
    void *dcbg[BMXT_MAX_NUM_DCBG];
} BMXT_P_TransportData;

void BMXT_P_SetPlatform(BMXT_Handle mxt, BMXT_Chip chip, BMXT_ChipRev rev, unsigned **ptr);

uint32_t BMXT_RegRead32_common(BMXT_Handle handle, uint32_t addr);
void BMXT_RegWrite32_common(BMXT_Handle handle, uint32_t addr, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef BMXT_PRIV_H__ */

