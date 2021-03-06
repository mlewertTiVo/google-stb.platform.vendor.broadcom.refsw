/****************************************************************************
 *                Copyright (c) 2013 Broadcom Corporation                   *
 *                                                                          *
 *      This material is the confidential trade secret and proprietary      *
 *      information of Broadcom Corporation. It may not be reproduced,      *
 *      used, sold or transferred to any third party without the prior      *
 *      written consent of Broadcom Corporation. All rights reserved.       *
 *                                                                          *
 ****************************************************************************/

/**
 * @file
 * @ingroup libdspcontrol
 * @brief DSP module chip-specific functions and data structures.
 */

#ifndef _DSP_CHIP_H_
#define _DSP_CHIP_H_

/* NOTE: this file gets exported into the Raaga Magnum host library and so it
 *       must abide by a specific strict set of rules. Please use only ANSI C
 *       constructs and include only FPSDK headers which do as well. After any
 *       edit, always make sure that the Raaga Magnum build succeeds without
 *       any warning.
 */

#include "fp_sdk_config.h"

#include "libdspcontrol/CHIP.h"

#if !(FEATURE_IS(SW_HOST, RAAGA_MAGNUM) || FEATURE_IS(SW_HOST, RAAGA_ROCKFORD))
#  include <stdbool.h>
#  include <stdint.h>
#else
#  include "bstd.h"
#  include "bmem.h"
#endif
#if defined(MCPHY)
#  include "DMA_Drv.h"
#  if IS_HOST(BM)
#    include "FreeRTOS.h"
#    include "semphr.h"
#  endif
#endif
#if defined(PIKE) && IS_HOST(SILICON)
#  include "umrbus.h"
#endif



#if !defined(_DSP_H_)
#  error "Don't include this header directly, include DSP.h instead"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#if defined(DUNA) && IS_HOST(SILICON)
/* Known to work parameters */
#  define DSP_DUNA_DEFAULT_READ_BLOCK_SIZE      64
#  define DSP_DUNA_DEFAULT_WRITE_BLOCK_SIZE     64
#  define DSP_DUNA_DEFAULT_SPI_FREQUENCY        4000000
#endif


#if defined(CELIVERO) && IS_HOST(SILICON)

/* Default memory allocation figures */
#  define DSP_CELIVERO_DEFAULT_TRANSFERS_MEM_SIZE               (1024 * 1024)
#  if !defined(TARGET_BUFFER_MUX_SERVICES)
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_TARGETPRINT        (512 * 1024)
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_STATPROF           (512 * 1024)
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_INSTRUMENTATION    (512 * 1024)
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_COREDUMP           (768 * 1024)
#  else
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_COMMON             (1024 * 1024)
#    define DSP_CELIVERO_DEFAULT_TB_MEM_SIZE_COREDUMP           (768 * 1024)
#  endif

/* Memory management types definition */
typedef struct
{
    void *vDdr;                     /**< virtual address, NULL by default */
    uint32_t pDdr_raw;              /**< physical address in low-mem, NULL by default */
    uint32_t pDdr_cached;           /**< cached access to the physical address, NULL by default */
    uint32_t pDdr_uncached;         /**< uncached access to the physical address, NULL by default */
    uint32_t cDdr;                  /**< block size, 0 by default */
} DSP_CELIVERO_MEMORY_BLOCK;

typedef struct
{
    bool initialised;                       /**< false if not initialised, true after the call to CELIVERO_allocateMemory */
    DSP_CELIVERO_MEMORY_BLOCK overlays;     /**< memory assigned to the overlay system */
    DSP_CELIVERO_MEMORY_BLOCK vom;          /**< memory assigned to VOM code */
    DSP_CELIVERO_MEMORY_BLOCK mem_transfers;/**< memory assigned to the CELIVERO_read/write_*mem functions */
    DSP_CELIVERO_MEMORY_BLOCK services;     /**< memory assigned to services TBs */
#  if !defined(TARGET_BUFFER_MUX_SERVICES)
    unsigned tb_targetprint_size;           /**< size assigned to the Target Print TB */
    unsigned tb_statprof_size;              /**< size assigned to the Statistical Profiling TB */
    unsigned tb_instrumentation_size;       /**< size assigned to the Instrumentation TB */
#  else
    unsigned tb_common_size;                /**< size assigned to the Common TB */
#  endif
    unsigned tb_coredump_size;              /**< size assigned to the Core Dump TB */
} DSP_CELIVERO_MEMORY_LAYOUT;

#endif


#if IS_TARGET(Pike_haps)
/** The (device, bus, address) coordinates describing an HAPS CAPIM */
typedef struct
{
    unsigned device;    /**< device no. */
    unsigned bus;       /**< bus no. */
    unsigned address;   /**< address */
} DSP_HAPS_COORDS;

#  define DSP_HAPS_INVALID_COORD    (~(0U))
#  define DSP_HAPS_INVALID_COORDS   { DSP_HAPS_INVALID_COORD, DSP_HAPS_INVALID_COORD, DSP_HAPS_INVALID_COORD }
#endif


typedef struct
{
#if IS_HOST(BM)
    /** The port on localhost where the SCP server (BM targets) is listening. */
    int port;
#endif

#if defined(CELIVERO) && IS_HOST(SILICON)
    /** call brcm_mmap_shared_flush before each brcm_read_physical invocation */
    bool     flush_cache_before_read     : 1;
    /** call brcm_mmap_shared_flush after each brcm_write_physical invocation */
    bool     flush_cache_after_write     : 1;
    /** true if memory used for Target Buffers and data transfers (code loading) should overlap */
    bool     mem_transfer_mem_overlap_tb : 1;
    /** true if memory should be allocated from CPUH/CPUL shared area of memory (brcm_mmap_ipc_*) */
    bool     mem_use_shared_dram         : 1;
    /** memory assigned to the CELIVERO_read/write_*mem functions, used for example for code loading */
    unsigned mem_transfers_size;
    /** memory assigned to the overlay system */
    unsigned mem_overlays_size;
    /** memory assigned to VOM code */
    unsigned mem_vom_size;
#  if !defined(TARGET_BUFFER_MUX_SERVICES)
    /** size assigned to the Target Print TB shared buffer */
    unsigned mem_tb_targetprint_size;
    /** size assigned to the Statistical Profiling TB shared buffer */
    unsigned mem_tb_statprof_size;
    /** size assigned to the Instrumentation TB shared buffer */
    unsigned mem_tb_instrumentation_size;
#  else
    /** size assigned to the Common TB shared buffer */
    unsigned mem_tb_common_size;
#  endif
    /** size assigned to the Core Dump TB shared buffer */
    unsigned mem_tb_coredump_size;
#endif

#if defined(RAAGA) && IS_HOST(SILICON)
    /** Magnum BREG memory mapped registers handle */
    BREG_Handle hReg;
    /** Magnum BMEM device memory handle */
    BMEM_Heap_Handle hMem;
#endif

#if defined(DUNA) && IS_HOST(SILICON)
    /** Memory will be read in blocks this big from the SPI interface */
    uint32_t read_block_size;
    /** Memory will be written in blocks this big from the SPI interface */
    uint32_t write_block_size;
    /** Frequency to run the SPI interface at */
    uint32_t spi_frequency;
    /** Name of the SPI dongle to use - NULL to use the first one */
    char *dongle_id;
    /** Whether to read back and verify each write */
    bool write_verify;
#endif

#if defined(MCPHY)
    /** which subsystem to control */
    DSP_SUBSYS dsp_subsystem;
    /** ban writes to IMEM if the DSP is enabled */
    bool check_dsp_enabled_before_write_imem    : 1;
    /** whether to initialise the mcphy2imem source side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in mcphy2imem_src_dma_handle */
    bool init_mcphy2imem_src_dma                : 1;
    /** whether to initialise the mcphy2imem destination side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in mcphy2imem_dst_dma_handle */
    bool init_mcphy2imem_dst_dma                : 1;
    /** whether to initialise the mcphy2dmem source side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in mcphy2dmem_src_dma_handle */
    bool init_mcphy2dmem_src_dma                : 1;
    /** whether to initialise the mcphy2dmem destination side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in mcphy2dmem_dst_dma_handle */
    bool init_mcphy2dmem_dst_dma                : 1;
    /** whether to initialise the dmem2mcphy source side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in dmem2mcphy_src_dma_handle */
    bool init_dmem2mcphy_src_dma                : 1;
    /** whether to initialise the dmem2mcphy destination side DMA; if false,
     * the DSP module assumes that the DMA is already configured with
     * base_address=0, block_size=2048, wr_status_or_skip_rd=no,
     * addr_alignment=4,double_command=true, status_en=true and a valid
     * handle is passed in dmem2mcphy_dst_dma_handle */
    bool init_dmem2mcphy_dst_dma                : 1;
    /** mcphy2imem source side DMA driver handle, ignored if init_mcphy2imem_src_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *mcphy2imem_src_dma_handle;
    /** mcphy2imem destination side DMA driver handle, ignored if init_mcphy2imem_dst_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *mcphy2imem_dst_dma_handle;
    /** mcphy2dmem source side DMA driver handle, ignored if init_mcphy2dmem_src_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *mcphy2dmem_src_dma_handle;
    /** mcphy2dmem destination side DMA driver handle, ignored if init_mcphy2dmem_dst_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *mcphy2dmem_dst_dma_handle;
    /** dmem2mcphy source side DMA driver handle, ignored if init_dmem2mcphy_src_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *dmem2mcphy_src_dma_handle;
    /** dmem2mcphy destination side DMA driver handle, ignored if init_dmem2mcphy_dst_dma is true;
     *  set to NULL to disable transfers in this direction */
    DmaHandle_T *dmem2mcphy_dst_dma_handle;
#endif

#if IS_TARGET(Pike_haps)
    /** CAPIM to drive the 'data' bus */
    DSP_HAPS_COORDS data_capim;
    /** CAPIM to drive the 'control' bus */
    DSP_HAPS_COORDS control_capim;
    /** Reset the UMRBUS - OBUS bridge when calling DSP_init */
    bool reset_bridge_on_init;
    /** Reset the UMRBUS - OBUS bridge if a transaction ends with an error */
    bool reset_bridge_on_error;
    /** Reset the UMRBUS - OBUS bridge after the end of every transaction */
    bool reset_bridge_on_transaction;
#endif
} DSP_PARAMETERS;


typedef struct
{
#if (defined(CELIVERO) && IS_HOST(SILICON)) || \
    defined(MCPHY)                          || \
    (defined(DUNA) && IS_HOST(SILICON))
    /** Local copy of the user-provided DSP_PARAMETERS */
    DSP_PARAMETERS parameters;
#endif
#if defined(__FP2012__)
    /** On FP2012 the debug console is emulated by a software circular buffer.
     *  We have to keep track if the structure has been initialised. */
    bool debug_console_initialised;
#endif
#if defined(CELIVERO) && IS_HOST(SILICON)
    /** Layout of the allocated DRAM, in virtual and physical forms. */
    DSP_CELIVERO_MEMORY_LAYOUT memory_layout;
#endif
#if defined(DUNA) && IS_HOST(SILICON)
    /** Actually a "Tspi *", but we can't declare a C++ class
     *  pointer in a C context. */
    void *spi;
#endif
#if defined(RAAGA) && IS_HOST(SILICON)
    /** Magnum BREG memory mapped registers handle */
    BREG_Handle hReg;
    /** Magnum BMEM device memory handle */
    BMEM_Heap_Handle hMem;
#endif
#if defined(MCPHY) && IS_HOST(BM)
    /** This mutex will be used to protect the SCP request/response socket I/O.
     *  FreeRTOS mutex API should't be called from interrupts, but we don't care
     *  as we are in a simulated world on RHEL, not on silicon. */
    xSemaphoreHandle scp_mutex;
#endif
#if IS_TARGET(Pike_haps)
    UMR_HANDLE data_capim;
    UMR_HANDLE control_capim;
    bool reset_bridge_on_init;
    bool reset_bridge_on_error;
    bool reset_bridge_on_transaction;
#endif
} DSP;


typedef struct
{
#if defined(RAAGA) && !defined(__FP4014__)
    uint32_t dsp_inth_host_status;
    uint32_t dsp_fw_inth_host_status;
    uint32_t dsp_mem_subsystem_memsub_error_status;
#endif
#if defined(__FP4014__) && !defined(RAAGA)
    uint32_t host_intc_host_irq;
#endif
} DSP_INTERRUPTS;


/**
 * Retrieve the number of FP cores in the DSP subsystem.
 *
 * @param[in] dsp  the DSP instance
 * @return         the number of FP cores
 */
#if defined(DUNA)
#  define DSP_numCores(dsp) (2)
#else
#  define DSP_numCores(dsp) (1)
#endif


/**
 * Return the size of the DSP instruction memory.
 * FIXME: this has to go!
 *
 * @param[in] dsp  the DSP instance
 * @return         the size of the DSP instruction memory, in bytes
 */
#if defined(CELIVERO)
#  define DSP_imemSize(dsp)   (256*1024)
#elif defined(RAAGA)
#  define DSP_imemSize(dsp)   (192*1024)
#elif defined(DUNA)
#  define DSP_imemSize(dsp)   (1536*1024)
#elif defined(MCPHY)
#  define DSP_imemSize(dsp)   (128*1024)
#elif defined(PIKE)
#  define DSP_imemSize(dsp)   (192*1024)
#else
#  error "Please define DSP_imemSize for this chip"
#endif


#ifdef __cplusplus
}
#endif


#endif  /* _DSP_CHIP_H_ */
