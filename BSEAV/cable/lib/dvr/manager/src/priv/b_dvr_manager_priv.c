/***************************************************************************
 *     (c)2013-2015 Broadcom Corporation
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
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 * 
 ****************************************************************************/

#include "b_dvr_manager.h"
#include "b_dvr_manager_priv.h"

BDBG_MODULE(b_dvr_manager_priv);

void B_DVR_Manager_P_UpdateSystemMemoryUsage(
    B_DVR_MemoryType memoryType,
    unsigned long size,
    bool alloc,
    const char *func,
    unsigned line)
{
    B_DVR_ManagerHandle manager = B_DVR_Manager_GetHandle();
    B_Mutex_Lock(manager->memoryUsageMutex);
    if (memoryType == eB_DVR_MemoryType_System) 
    {
        if(alloc) 
        {
            manager->systemMemoryUsed += size;
        }
        else
        {
            manager->systemMemoryUsed -= size;
        }
        BDBG_MSG(("\n DVR SystemMemoryUpdate: %s%u bytes total %u bytes func %s line %u \n",
         alloc?"+":"-",size,manager->systemMemoryUsed,func,line));
    }
    else
    {
        if(memoryType == eB_DVR_MemoryType_Device) 
        {
            if(alloc) 
            {
                manager->deviceMemoryUsed += size;
             }
             else
             {
                 manager->deviceMemoryUsed -= size;
             }
             BDBG_MSG(("DVR DeviceMemoryUpdate: %s%u total %u func %s line %u",
              alloc?"+":"-",size,manager->deviceMemoryUsed,func,line));
        }
    }
    B_Mutex_Unlock(manager->memoryUsageMutex);
    return;
}


