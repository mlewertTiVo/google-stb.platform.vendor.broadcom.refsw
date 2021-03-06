/******************************************************************************
 *    (c)2008-2015 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
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
 *****************************************************************************/
#include "nexus_platform_priv.h"
#include <stdio.h>

#define PRINT_HEAP_FIELD(heap,i,field,fmt) printf("\t\tNEXUS_HEAP_FIELD(%u,%s," fmt ")\n", i, #field, heap[i].field);
#define HEAP_NAME(name,NAME) if(i==NAME) {name=#NAME;break;}
void print_heaps(unsigned boxmode)
{
    NEXUS_PlatformSettings settings;
    const NEXUS_PlatformHeapSettings *heap = settings.heap;
    unsigned i;
    BKNI_Init();
    BDBG_Init();

    BKNI_Memset(&settings, 0, sizeof(settings));
    NEXUS_Platform_P_GetDefaultHeapSettings(&settings, boxmode);
    printf("BOX_MODE_BEGIN(%u)\n", boxmode);
    for(i=0;i<NEXUS_MAX_HEAPS;i++) {
        const char *name = NULL;
        do {
            /* generated by running "grep -roh 'NEXUS_.*_HEAP ' nexus/platforms/ | sort -u" */
#ifdef NEXUS_MEMC0_DRIVER_HEAP
            HEAP_NAME(name,NEXUS_MEMC0_DRIVER_HEAP);
#endif
#ifdef NEXUS_MEMC0_GRAPHICS_HEAP
            HEAP_NAME(name,NEXUS_MEMC0_GRAPHICS_HEAP);
#endif
#ifdef NEXUS_MEMC0_MAIN_HEAP
            HEAP_NAME(name,NEXUS_MEMC0_MAIN_HEAP);
#endif
#ifdef NEXUS_MEMC0_PICTURE_BUFFER_HEAP
            HEAP_NAME(name,NEXUS_MEMC0_PICTURE_BUFFER_HEAP);
#endif
#ifdef NEXUS_MEMC1_DRIVER_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_DRIVER_HEAP);
#endif
#ifdef NEXUS_MEMC1_GRAPHICS_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_GRAPHICS_HEAP);
#endif
#ifdef NEXUS_MEMC1_HIGH_MEMORY_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_HIGH_MEMORY_HEAP);
#endif
#ifdef NEXUS_MEMC1_MAIN_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_MAIN_HEAP);
#endif
#ifdef NEXUS_MEMC1_PICTURE_BUFFER_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_PICTURE_BUFFER_HEAP);
#endif
#ifdef NEXUS_MEMC1_RDC_HEAP
            HEAP_NAME(name,NEXUS_MEMC1_RDC_HEAP);
#endif
#ifdef NEXUS_MEMC2_DRIVER_HEAP
            HEAP_NAME(name,NEXUS_MEMC2_DRIVER_HEAP);
#endif
#ifdef NEXUS_MEMC2_GRAPHICS_HEAP
            HEAP_NAME(name,NEXUS_MEMC2_GRAPHICS_HEAP);
#endif
#ifdef NEXUS_MEMC2_PICTURE_BUFFER_HEAP
            HEAP_NAME(name,NEXUS_MEMC2_PICTURE_BUFFER_HEAP);
#endif
#ifdef NEXUS_MEMCx_PICTURE_BUFFER_HEAP
            HEAP_NAME(name,NEXUS_MEMCx_PICTURE_BUFFER_HEAP);
#endif
#ifdef NEXUS_PLATFORM_DEFAULT_HEAP
            HEAP_NAME(name,NEXUS_PLATFORM_DEFAULT_HEAP);
#endif
#ifdef NEXUS_SAGE_SECURE_HEAP
            HEAP_NAME(name,NEXUS_SAGE_SECURE_HEAP);
#endif
#ifdef NEXUS_VIDEO_SECURE_HEAP
            HEAP_NAME(name,NEXUS_VIDEO_SECURE_HEAP);
#endif
        } while(0);
        if(heap[i].size==0 && name==NULL) {
           continue;
        }
        if(name==NULL) {
            name = "UNKNOWN";
        }
        printf("\tNEXUS_HEAP_BEGIN(%u,%s)\n", i,name);
        PRINT_HEAP_FIELD(heap,i,memcIndex,"%u");
        PRINT_HEAP_FIELD(heap,i,size,"%d");
        PRINT_HEAP_FIELD(heap,i,alignment,"%u");
        PRINT_HEAP_FIELD(heap,i,memoryType,"%#x");
        PRINT_HEAP_FIELD(heap,i,heapType,"%#x");
        PRINT_HEAP_FIELD(heap,i,placement.first,"%u");
        PRINT_HEAP_FIELD(heap,i,placement.sage,"%u");
        printf("\tNEXUS_HEAP_END(%u)\n", i);
    }
    printf("BOX_MODE_END(%u)\n", boxmode);

    BDBG_Uninit();
    BKNI_Uninit();
    return;
}
