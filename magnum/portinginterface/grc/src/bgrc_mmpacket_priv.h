/***************************************************************************
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
 *
 * Module Description: GRC Packet private header
 *
 ***************************************************************************/

#ifndef BGRC_MMPACKET_PRIV_H__
#define BGRC_MMPACKET_PRIV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bchp.h"
#include "bchp_common.h"
#include "bdbg.h"
#include "bint.h"
#include "bkni.h"
#include "bgrc.h"
#include "bgrc_packet_priv.h"
#include "bgrc_private.h"
#include "bgrc_packet.h"
#include "bgrc_errors.h"

#if (BGRC_P_VER >= BGRC_P_VER_3)

#ifdef BCHP_MM_M2MC0_REG_START
#include "bchp_mm_m2mc0.h"
#endif


#define BGRC_MM_M2MC(val)   BCHP_MM_M2MC0_##val



/* register settings */
typedef enum BGRC_Swizzling
{
    BGRC_Swizzling_eUIF       = 0, /* Unified image format, no XORing */
    BGRC_Swizzling_eUIF_XOR   = 1, /* XOR in odd columns */
    BGRC_Swizzling_eUBLINEAR2 = 2,  /* UIF-blocks in raster order 2 UIF per column*/
    BGRC_Swizzling_eUBLINEAR1 = 3,  /* UIF-blocks in raster order, 1UIF per column */
    BGRC_Swizzling_eLT        = 4, /* Lineartile */
    BGRC_Swizzling_eMax
} BGRC_Swizzling;

typedef struct BGRC_SwizzlingConvertInfo {
    BPXL_Uif_Swizzling pxl;
    BGRC_Swizzling grc;
} BGRC_SwizzlingConvertInfo;

BGRC_Swizzling BGRC_P_PixelFormat_FromMagnum(BPXL_Uif_Swizzling ePxlSwizzling);

#endif
#ifdef __cplusplus
}
#endif

#endif /* #ifndef BGRC_PACKET_PRIV_H__ */

/* end of file */
