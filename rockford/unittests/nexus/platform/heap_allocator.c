/***************************************************************************
*     (c)2004-2013 Broadcom Corporation
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
*
* Revision History:
*
* $brcm_Log: $
*
***************************************************************************/
#include "bstd.h"
#include "bkni.h"
#include "bkni_multi.h"
#include "bmma.h"
#include "bmma_range.h"
#include "bdbg_app.h"
#include <string.h>
BDBG_MODULE(heap_allocator);

#include "base/include/public/nexus_base_types.h"

typedef struct brcmstb_range {
    BMMA_DeviceOffset addr;
    size_t size;  /* 0 means no region */
} nexus_p_memory_range;

#define MAX_RANGES        8
#define NEXUS_NUM_MEMC  3

#define NEXUS_NUM_MEMC_REGIONS 3
#define NEXUS_CMA_MAX_REGIONS NEXUS_MAX_HEAPS
#define NEXUS_CMA_MAX_INDEX (NEXUS_NUM_MEMC*NEXUS_NUM_MEMC_REGIONS)

typedef struct brcmstb_memory {
    struct {
        struct brcmstb_range range[MAX_RANGES];
        int count;
    } memc[NEXUS_NUM_MEMC];
    /* fixed map space */
    struct {
        struct brcmstb_range range[MAX_RANGES];
        int count;
    } lowmem;
    /* bmem carveout regions */
    struct {
        struct brcmstb_range range[MAX_RANGES];
        int count;
    } bmem;
    /* CMA carveout regions */
    struct {
        struct brcmstb_range range[MAX_RANGES];
        int count;
    } cma;
    /* regions that nexus cannot recommend for bmem or CMA */
    struct {
        struct brcmstb_range range[MAX_RANGES];
        int count;
    } reserved;
} nexus_p_memory_info;

typedef struct NEXUS_PlatformOsRegion {
    NEXUS_Addr base; /* physical address */
    uint64_t length; /* no region if 0 */

    /* correlation of bmem to MEMC */
    unsigned memcIndex;
    unsigned subIndex;
    bool cma;
} NEXUS_PlatformOsRegion;

typedef struct NEXUS_PlatformHeapSettings {
    unsigned memcIndex; /* memory controller (MEMC) index. */
    unsigned subIndex;  /* addressing region within the MEMC. The only use is subIndex = 1 for MEMC0 memory above
                           the 256MB register hole or 760MB CMA barrier. Always 0 for MEMC1 and 2. */
    int size;           /* If size is >0, a fixed sized heap is created.
                           If size is -1, all remaining device memory in a matching bmem/cma region is allocated to that heap.
                           If size is 0 (default), no heap is created. */
    unsigned alignment; /* Minimum alignment of heap. In units of bytes. */
    bool guardBanding;  /* [deprecated] If true, add debug guard bands around each allocation.
                           Only applies to CPU-accessible heaps.
                           Set to false if your heap size calculations are precise with no extra space. */
    unsigned memoryType; /* see NEXUS_MEMORY_TYPE bitmasks and NEXUS_MemoryType macros in nexus_types.h */
    unsigned heapType;   /* see NEXUS_HEAP_TYPE bitmasks in nexus_types.h */
    bool optional;      /* if true, then allow nexus to init even if memory for this heap cannot be found */
    struct {
        bool first; /* if set to true, then this HEAP would be placed at beginning of the first region available on a given MEMC */
        bool sage; /* if set to true, then this HEAP should be placed to confirm SAGE requirements */
    } placement;
} NEXUS_PlatformHeapSettings;

struct nexus_cma_map {
    int unused;
};

struct nexus_linux_cma_memory_info {
    int unused;
};

typedef struct NEXUS_PlatformMemcMemory {
    uint64_t length; /* no MEMC if 0 */
    struct {
        NEXUS_Addr base; /* physical address. 0 is valid. */
        uint64_t length; /* no region if 0 */
    } region[NEXUS_NUM_MEMC_REGIONS];
} NEXUS_PlatformMemcMemory;

typedef struct NEXUS_PlatformMemory
{
    /* DDR sizes detected on the board, per MEMC, per physical addressing region */
    NEXUS_PlatformMemcMemory memc[NEXUS_MAX_MEMC];

    /* OS's report of usable memory by physical address */
    NEXUS_PlatformOsRegion osRegion[NEXUS_MAX_HEAPS];

    unsigned max_dcache_line_size; /* reported by OS. BMEM alignment must be >= this to avoid cache coherency bugs. */
} NEXUS_PlatformMemory;

#define NEXUS_MEMORY_TYPE_HIGH_MEMORY           0x0100
#define NEXUS_MEMORY_TYPE_DRIVER_UNCACHED       0x0001
#define NEXUS_MEMORY_TYPE_DRIVER_CACHED         0x0002

typedef struct NEXUS_Core_MemoryRegion
{
    unsigned memcIndex;     /* memory controller index */
    void *pvAddr;           /* virtual uncached address of memory region */
    NEXUS_Addr offset;      /* physical device offset (address) of memory region */
    size_t length;          /* length in bytes of the memory region */
    void *pvAddrCached;     /* virtual cached address of memory region, optional */
    unsigned memoryType;    /* bitmasdk of NEXUS_MEMORY_TYPE_XXX macros, found in nexus_types.h */
    unsigned heapType;      /* bitmask of NEXUS_HEAP_TYPE_XXX macros, found in nexus_types.h */
    unsigned alignment;     /* required alignment (in bytes) of allocations in this region */
    bool locked;            /* nexus does not allow new allocations from a locked heap. */
    bool guardBanding;      /* [deprecated] if true, use guard bands if possible. if false, force no guard banding for higher performance. */
} NEXUS_Core_MemoryRegion;

typedef struct NEXUS_Core_Settings
{
    NEXUS_Core_MemoryRegion heapRegion[NEXUS_MAX_HEAPS]; /* heaps to allocate. set heapRegion[].length == 0 to terminate. */
} NEXUS_Core_Settings;

struct heap_config {
    const char *name;
    NEXUS_Error (*heap_config)(unsigned boxMode, unsigned heapNo, NEXUS_PlatformHeapSettings *heap);
    int (*heap_byName)(const char *heapName);
    size_t (*boxModes)(unsigned *boxModes, size_t maxModes);
};

struct memc_memory_range {
    unsigned memc;
    BMMA_DeviceOffset addr;
    size_t size;
};

struct board {
    const char *name;
    const struct memc_memory_range *memc;
    unsigned nRanges;
};

struct heap_size_entry {
    const char *name;
    size_t size;
};

struct picture_buffers {
    const char *name;
    const struct heap_size_entry *buffers;
};

struct platform {
    const struct board *board;
    const struct heap_config * const *heap_config;
    const struct picture_buffers *pictures_buffers;
};



#define BCHP_REGISTER_END   0

#define vmalloc_size(a,b,c)     0

#include "platforms/common/src/linuxuser/nexus_platform_heaps.c"

#define BOX_MODE_BEGIN(x) case x: switch(heapNo) {
#define NEXUS_HEAP_BEGIN(x, name) case x:
#define NEXUS_HEAP_FIELD(n, field, value) heap->field = value;
#define NEXUS_HEAP_END(x)   return NEXUS_SUCCESS;
#define BOX_MODE_END(z)  default: return NEXUS_NOT_SUPPORTED;}


#define HEAP_CONFIG_BEGIN(name) static NEXUS_Error heap_config_##name (unsigned boxMode, unsigned heapNo, NEXUS_PlatformHeapSettings *heap) { switch(boxMode) {
#define HEAP_CONFIG_END(name)  default: break; } return BERR_TRACE(NEXUS_NOT_SUPPORTED); }


HEAP_CONFIG_BEGIN(97445_SAGE_y)
#include "heap_configs/heap_config_97445_SAGE_y.txt"
HEAP_CONFIG_END(97445_SAGE_y)

HEAP_CONFIG_BEGIN(97445_SAGE_n)
#include "heap_configs/heap_config_97445_SAGE_n.txt"
HEAP_CONFIG_END(97445_SAGE_n)

HEAP_CONFIG_BEGIN(97250_SAGE_y)
#include "heap_configs/heap_config_97250_SAGE_y.txt"
HEAP_CONFIG_END(97250_SAGE_y)

HEAP_CONFIG_BEGIN(97250_SAGE_n)
#include "heap_configs/heap_config_97250_SAGE_n.txt"
HEAP_CONFIG_END(97250_SAGE_n)

HEAP_CONFIG_BEGIN(97364_SAGE_y)
#include "heap_configs/heap_config_97364_SAGE_y.txt"
HEAP_CONFIG_END(97364_SAGE_y)

HEAP_CONFIG_BEGIN(97364_SAGE_n)
#include "heap_configs/heap_config_97364_SAGE_n.txt"
HEAP_CONFIG_END(97364_SAGE_n)

HEAP_CONFIG_BEGIN(97366_SAGE_y)
#include "heap_configs/heap_config_97366_SAGE_y.txt"
HEAP_CONFIG_END(97366_SAGE_y)

HEAP_CONFIG_BEGIN(97366_SAGE_n)
#include "heap_configs/heap_config_97366_SAGE_n.txt"
HEAP_CONFIG_END(97366_SAGE_n)

HEAP_CONFIG_BEGIN(97439_SAGE_y)
#include "heap_configs/heap_config_97439_SAGE_y.txt"
HEAP_CONFIG_END(97439_SAGE_y)

HEAP_CONFIG_BEGIN(97439_SAGE_n)
#include "heap_configs/heap_config_97439_SAGE_n.txt"
HEAP_CONFIG_END(97439_SAGE_n)

#undef BOX_MODE_BEGIN
#undef NEXUS_HEAP_BEGIN
#undef NEXUS_HEAP_FIELD
#undef NEXUS_HEAP_END
#undef BOX_MODE_END

#define BOX_MODE_BEGIN(x)
#define NEXUS_HEAP_BEGIN(x, name) if(strcmp(heapName, #name)==0) { return x;}
#define NEXUS_HEAP_FIELD(n, field, value)
#define NEXUS_HEAP_END(x)
#define BOX_MODE_END(z)

#define HEAP_BYNAME_BEGIN(name) static int heap_byName_##name(const char *heapName) {
#define HEAP_BYNAME_END(name)     return -1; }

HEAP_BYNAME_BEGIN(97445_SAGE_y)
#include "heap_configs/heap_config_97445_SAGE_y.txt"
HEAP_BYNAME_END(97445_SAGE_y)

HEAP_BYNAME_BEGIN(97445_SAGE_n)
#include "heap_configs/heap_config_97445_SAGE_y.txt"
HEAP_BYNAME_END(97445_SAGE_n)

HEAP_BYNAME_BEGIN(97250_SAGE_y)
#include "heap_configs/heap_config_97250_SAGE_y.txt"
HEAP_BYNAME_END(97250_SAGE_y)

HEAP_BYNAME_BEGIN(97250_SAGE_n)
#include "heap_configs/heap_config_97250_SAGE_n.txt"
HEAP_BYNAME_END(97250_SAGE_n)

HEAP_BYNAME_BEGIN(97364_SAGE_y)
#include "heap_configs/heap_config_97364_SAGE_y.txt"
HEAP_BYNAME_END(97364_SAGE_y)

HEAP_BYNAME_BEGIN(97364_SAGE_n)
#include "heap_configs/heap_config_97364_SAGE_n.txt"
HEAP_BYNAME_END(97364_SAGE_n)

HEAP_BYNAME_BEGIN(97366_SAGE_y)
#include "heap_configs/heap_config_97366_SAGE_y.txt"
HEAP_BYNAME_END(97366_SAGE_y)

HEAP_BYNAME_BEGIN(97366_SAGE_n)
#include "heap_configs/heap_config_97366_SAGE_n.txt"
HEAP_BYNAME_END(97366_SAGE_n)

HEAP_BYNAME_BEGIN(97439_SAGE_y)
#include "heap_configs/heap_config_97439_SAGE_y.txt"
HEAP_BYNAME_END(97439_SAGE_y)

HEAP_BYNAME_BEGIN(97439_SAGE_n)
#include "heap_configs/heap_config_97439_SAGE_n.txt"
HEAP_BYNAME_END(97439_SAGE_n)



#undef NEXUS_HEAP_BEGIN
#undef BOX_MODE_BEGIN

#define NEXUS_HEAP_BEGIN(x, name)
#define BOX_MODE_BEGIN(x) if(i>=maxModes) { return i;} boxModes[i]=x;i++;

#define BOXMODES_BEGIN(name) static size_t boxModes_##name(unsigned *boxModes, size_t maxModes) { size_t i=0;
#define BOXMODES_END(name)  return i; }


BOXMODES_BEGIN(97445_SAGE_y)
#include "heap_configs/heap_config_97445_SAGE_y.txt"
BOXMODES_END(97445_SAGE_y)

BOXMODES_BEGIN(97445_SAGE_n)
#include "heap_configs/heap_config_97445_SAGE_n.txt"
BOXMODES_END(97445_SAGE_n)

BOXMODES_BEGIN(97250_SAGE_y)
#include "heap_configs/heap_config_97250_SAGE_y.txt"
BOXMODES_END(97250_SAGE_y)

BOXMODES_BEGIN(97250_SAGE_n)
#include "heap_configs/heap_config_97250_SAGE_n.txt"
BOXMODES_END(97250_SAGE_n)

BOXMODES_BEGIN(97364_SAGE_y)
#include "heap_configs/heap_config_97364_SAGE_y.txt"
BOXMODES_END(97364_SAGE_y)

BOXMODES_BEGIN(97364_SAGE_n)
#include "heap_configs/heap_config_97364_SAGE_n.txt"
BOXMODES_END(97364_SAGE_n)

BOXMODES_BEGIN(97366_SAGE_y)
#include "heap_configs/heap_config_97366_SAGE_y.txt"
BOXMODES_END(97366_SAGE_y)

BOXMODES_BEGIN(97366_SAGE_n)
#include "heap_configs/heap_config_97366_SAGE_n.txt"
BOXMODES_END(97366_SAGE_n)

BOXMODES_BEGIN(97439_SAGE_y)
#include "heap_configs/heap_config_97439_SAGE_y.txt"
BOXMODES_END(97439_SAGE_y)

BOXMODES_BEGIN(97439_SAGE_n)
#include "heap_configs/heap_config_97439_SAGE_n.txt"
BOXMODES_END(97439_SAGE_n)

static const struct heap_config _97445_SAGE_y = {
    "97445_SAGE_y",
    heap_config_97445_SAGE_y,
    heap_byName_97445_SAGE_y,
    boxModes_97445_SAGE_y
};

static const struct heap_config _97445_SAGE_n = {
    "97445_SAGE_n",
    heap_config_97445_SAGE_n,
    heap_byName_97445_SAGE_n,
    boxModes_97445_SAGE_n
};

static const struct heap_config _97250_SAGE_n = {
    "97250_SAGE_n",
    heap_config_97250_SAGE_n,
    heap_byName_97250_SAGE_n,
    boxModes_97250_SAGE_n
};

static const struct heap_config _97250_SAGE_y = {
    "97250_SAGE_y",
    heap_config_97250_SAGE_y,
    heap_byName_97250_SAGE_y,
    boxModes_97250_SAGE_y
};

static const struct heap_config _97364_SAGE_n = {
    "97364_SAGE_n",
    heap_config_97364_SAGE_n,
    heap_byName_97364_SAGE_n,
    boxModes_97364_SAGE_n
};

static const struct heap_config _97364_SAGE_y = {
    "97364_SAGE_y",
    heap_config_97364_SAGE_y,
    heap_byName_97364_SAGE_y,
    boxModes_97364_SAGE_y
};

static const struct heap_config _97366_SAGE_n = {
    "97366_SAGE_n",
    heap_config_97366_SAGE_n,
    heap_byName_97366_SAGE_n,
    boxModes_97366_SAGE_n
};

static const struct heap_config _97366_SAGE_y = {
    "97366_SAGE_y",
    heap_config_97366_SAGE_y,
    heap_byName_97366_SAGE_y,
    boxModes_97366_SAGE_y
};

static const struct heap_config _97439_SAGE_n = {
    "97439_SAGE_n",
    heap_config_97439_SAGE_n,
    heap_byName_97439_SAGE_n,
    boxModes_97439_SAGE_n
};

static const struct heap_config _97439_SAGE_y = {
    "97439_SAGE_y",
    heap_config_97439_SAGE_y,
    heap_byName_97439_SAGE_y,
    boxModes_97439_SAGE_y
};

static const struct memc_memory_range bcm97445_3GB_memc[] = {
    {0, 0x001000000, 1056964608},
    {1, 0x040000000, 1073741824},
    {2, 0x080000000, 1073741824}
};

static const struct board bcm97445_3GB = {
    "BCM97445 3GB", bcm97445_3GB_memc, sizeof(bcm97445_3GB_memc)/sizeof(bcm97445_3GB_memc[0])
};

static const struct heap_size_entry high_7445[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x86d0000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0x114ab000},
    {"NEXUS_MEMC2_PICTURE_BUFFER_HEAP", 0x7876000},
    {NULL,0}
};

static const struct heap_size_entry medium_7445[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x1322000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0x114ab000},
    {"NEXUS_MEMC2_PICTURE_BUFFER_HEAP", 0x7876000},
    {NULL,0}
};

static const struct heap_size_entry low_7445[] = {
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0x55c8000},
    {"NEXUS_MEMC2_PICTURE_BUFFER_HEAP", 0x58be000},
    {NULL,0}
};

static const struct picture_buffers pictures_7445[] = {
    {"high",high_7445},
    {"medium",medium_7445},
    {"low",low_7445},
    {NULL,NULL}
};

static const struct heap_config const *_97445_configs[] = {
    &_97445_SAGE_y,
    &_97445_SAGE_n,
    NULL
};

static const struct platform _97445 = {
    &bcm97445_3GB,
    _97445_configs,
    pictures_7445
};

static const struct memc_memory_range bcm97250_1GB_memc[] = {
    {0, 0x001000000, 1056964608},
};

static const struct board bcm97250_1GB = {
    "BCM97250 1GB", bcm97250_1GB_memc, sizeof(bcm97250_1GB_memc)/sizeof(bcm97250_1GB_memc[0])
};

static const struct heap_size_entry default_7250[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x877e000},
    {NULL,0}
};

static const struct picture_buffers pictures_7250[] = {
    {"default",default_7250},
    {NULL,NULL}
};

static const struct heap_config const *_97250_configs[] = {
    &_97250_SAGE_y,
    &_97250_SAGE_n,
    NULL
};

static const struct platform _97250 = {
    &bcm97250_1GB,
    _97250_configs,
    pictures_7250
};

static const struct memc_memory_range bcm97364_1GB_memc[] = {
    {0, 0x001000000, 1056964608},
};

static const struct board bcm97364_1GB = {
    "BCM97420 1GB", bcm97364_1GB_memc, sizeof(bcm97364_1GB_memc)/sizeof(bcm97364_1GB_memc[0])
};

static const struct heap_size_entry default_7364[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x98f7000},
    {NULL,0}
};

static const struct picture_buffers pictures_7364[] = {
    {"default",default_7364},
    {NULL,NULL}
};

static const struct heap_config const *_97364_configs[] = {
    &_97364_SAGE_y,
    &_97364_SAGE_n,
    NULL
};

static const struct platform _97364 = {
    &bcm97364_1GB,
    _97364_configs,
    pictures_7364
};

static const struct memc_memory_range bcm97366_2GB_memc[] = {
    {0, 0x001000000, 1056964608},
    {1, 0x080000000, 1073741824},
};

static const struct board bcm97366_2GB = {
    "BCM97420 2GB", bcm97366_2GB_memc, sizeof(bcm97366_2GB_memc)/sizeof(bcm97366_2GB_memc[0])
};

static const struct heap_size_entry default_7366[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x94a0000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0xce6d000},
    {NULL,0}
};

static const struct picture_buffers pictures_7366[] = {
    {"default",default_7366},
    {NULL,NULL}
};

static const struct heap_config const *_97366_configs[] = {
    &_97366_SAGE_y,
    &_97366_SAGE_n,
    NULL
};

static const struct platform _97366 = {
    &bcm97366_2GB,
    _97366_configs,
    pictures_7366
};

static const struct memc_memory_range bcm97439_4GB_memc[] = {
    {0,  0x001000000, 2130706432u},
    {1,  0x080000000, 1073741824},
    {1,  (BMMA_DeviceOffset)0x300000000ull, 1073741824},
};

static const struct board bcm97439_4GB = {
    "BCM97420 4GB", bcm97439_4GB_memc, sizeof(bcm97439_4GB_memc)/sizeof(bcm97439_4GB_memc[0])
};

static const struct heap_size_entry high_7439[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0xf001000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0x10816000},
    {NULL,0}
};

static const struct heap_size_entry medium_7439[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x7b68000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0x10816000},
    {NULL,0}
};

static const struct heap_size_entry low_7439[] = {
    {"NEXUS_MEMC0_PICTURE_BUFFER_HEAP", 0x766000},
    {"NEXUS_MEMC1_PICTURE_BUFFER_HEAP", 0xa864000},
    {NULL,0}
};

static const struct picture_buffers pictures_7439[] = {
    {"high",high_7439},
    {"medium",medium_7439},
    {"low",low_7439},
    {NULL,NULL}
};

static const struct heap_config const *_97439_configs[] = {
    &_97439_SAGE_y,
    &_97439_SAGE_n,
    NULL
};

static const struct platform _97439 = {
    &bcm97439_4GB,
    _97439_configs,
    pictures_7439
};

static const struct platform  const *platforms[] = {
    &_97445,
    &_97250,
    &_97364,
    &_97366,
    &_97439,
    NULL
};


static void nexus_p_memory_info_init(nexus_p_memory_info *info, const struct board *board)
{
    unsigned i;
    BKNI_Memset(info,0,sizeof(*info));
    for(i=0;i<board->nRanges;i++) {
        unsigned memc = board->memc[i].memc;
        info->memc[memc].range[info->memc[memc].count].addr = board->memc[i].addr;
        info->memc[memc].range[info->memc[memc].count].size = board->memc[i].size;
        info->memc[memc].count++;
    }
    return;
}


int main(int argc, const char *argv[])
{
    NEXUS_Error rc;
    unsigned max_dcache_line_size = 1024;
    unsigned platformIndex;
    BSTD_UNUSED(argc);
    BSTD_UNUSED(argv);
    BKNI_Init();
    BDBG_Init();

    if(0) {
        BDBG_SetModuleLevel("heap_allocator", BDBG_eMsg);
        if(0) BDBG_SetModuleLevel("BMMA_Range", BDBG_eMsg);
    }

    for(platformIndex=0;;platformIndex++) {
        const struct platform *platform = platforms[platformIndex];
        const struct board *board;
        unsigned heap_config;

        if(platform==NULL) {
            break;
        }

        board = platform->board;
        BDBG_LOG(("Testing %s", board->name));
        for(heap_config=0;;heap_config++) {
            const struct heap_config *config = platform->heap_config[heap_config];
            unsigned picture_config;

            if(config==NULL) {
                break;
            }
            BDBG_LOG(("Using %s", config->name));
            for(picture_config=0;;picture_config++) {
                size_t nBoxModes;
                unsigned i;
                unsigned boxModes[64];
                const struct picture_buffers *picture_buffers = &platform->pictures_buffers[picture_config];
                const struct heap_size_entry *picture_heaps = picture_buffers->buffers;

                if(picture_buffers->name==NULL) {
                    break;
                }

                nBoxModes = config->boxModes(boxModes, sizeof(boxModes)/sizeof(boxModes[0]));
                BDBG_LOG(("Testing %s with %u boxmodes", picture_buffers->name, nBoxModes));
                for(i=0;i<nBoxModes;i++) {
                    unsigned boxMode = boxModes[i];
                    unsigned j;
                    int heapNo;
                    struct NEXUS_Platform_P_AllocatorState state;
                    NEXUS_PlatformHeapSettings heaps[NEXUS_MAX_HEAPS];
                    NEXUS_PlatformMemory memory;
                    NEXUS_Core_Settings coreSettings;

                    BDBG_MSG(("Using box MODE %u (%u/%u)", boxMode, i, nBoxModes));
                    BKNI_Memset(heaps,0,sizeof(heaps));
                    for(heapNo=0;heapNo<NEXUS_MAX_HEAPS;heapNo++) {
                        rc = config->heap_config(boxMode, heapNo, &heaps[heapNo]);
                    }
                    for(j=0;;j++) {
                        if(picture_heaps[j].name==NULL) {
                            break;
                        }
                        heapNo = config->heap_byName(picture_heaps[j].name);
                        if(heapNo<0) {
                            continue;
                        }
                        heaps[heapNo].size = picture_heaps[j].size;
                    }

                    BKNI_Memset(&state, 0, sizeof(state));
                    nexus_p_memory_info_init(&state.bmem_hint.info, board);
                    NEXUS_Platform_P_CalculateBootParams(&state, heaps, max_dcache_line_size);
                    NEXUS_Platform_P_GetVmallocSize(heaps, &state.bmem_hint.info);

                    BKNI_Memset(&memory, 0, sizeof(memory));
                    BDBG_CASSERT(sizeof(memory.osRegion) == sizeof(state.bmem_hint.osMemory));
                    BKNI_Memcpy(memory.osRegion, state.bmem_hint.osMemory, sizeof(memory.osRegion));
                    memory.max_dcache_line_size = max_dcache_line_size;
                    NEXUS_Platform_P_PrintBmemBootOptions(state.bmem_hint.osMemory, "suggested", "", state.bmem_hint.buf, sizeof(state.bmem_hint.buf));
                    NEXUS_Platform_P_SetCoreCmaSettings_allocateBmem(&state, heaps, &memory, &coreSettings);
                }
            }
        }
    }

    BDBG_Uninit();
    BKNI_Uninit();
    return 0;
}
