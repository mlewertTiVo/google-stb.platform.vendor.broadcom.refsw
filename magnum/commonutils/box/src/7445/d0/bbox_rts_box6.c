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

#include "bstd.h"                /* standard types */
#include "bkni.h"
#include "bdbg.h"                /* Debug message */
#include "bbox.h"

BDBG_MODULE(BBOX_RTS_BOX6);
BDBG_OBJECT_ID(BBOX_RTS_BOX6);

extern BBOX_Rts stBoxRts_7252_1u2t_box6;

/* Memc Index for box mode 6. BBOX_MemcIndex_Invalid means it's not used */
static const BBOX_MemConfig stBoxMemConfig_7252_box6 =
{
   {
      BBOX_MK_RDC_MEMC_IDX(0),       /* RDC */
      {
         BBOX_MK_WIN_MEMC_IDX(1,       Invalid, 1,       Invalid, 0      ),  /* disp  0 */
         BBOX_MK_WIN_MEMC_IDX(1,       Invalid, Invalid, Invalid, 1      ),  /* disp  1 */
         BBOX_MK_WIN_MEMC_IDX(Invalid, Invalid, Invalid, Invalid, Invalid),  /* disp  2 */
         BBOX_MK_WIN_MEMC_IDX(1,       Invalid, 1,       Invalid, 1      ),  /* disp  3 */
         BBOX_MK_WIN_MEMC_IDX(1,       Invalid, 1,       Invalid, 1      ),  /* disp  4 */
         BBOX_MK_WIN_MEMC_IDX(Invalid, Invalid, Invalid, Invalid, Invalid),  /* disp  5 */
         BBOX_MK_WIN_MEMC_IDX(Invalid, Invalid, Invalid, Invalid, Invalid),  /* disp  6 */
      }
   },
   BBOX_INVALID_NUM_MEMC
};
void BBOX_P_GetBox6MemConfig
    ( BBOX_MemConfig                *pBoxMemConfig )
{
    *pBoxMemConfig = stBoxMemConfig_7252_box6;
    pBoxMemConfig->ulNumMemc = stBoxRts_7252_1u2t_box6.ulNumMemc;
}

void BBOX_P_GetBox6Rts
    ( BBOX_Rts *pBoxRts )
{
    *pBoxRts = stBoxRts_7252_1u2t_box6;
}
