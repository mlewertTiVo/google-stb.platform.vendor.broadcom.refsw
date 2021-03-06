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
 * @ingroup libsyschip
 * @brief Target Buffer chip-specific declarations.
 *
 * Shared control structures used by services are declared in a chip-specific way.
 * Also, macros describing chip requirements and constraints are defined here.
 *
 * Note: this file gets included both by Firepath and Host code, so avoid dependencies
 *       to SDK headers as much as possible and macro guard DSP-specific definitions.
 *       Also, this header is included from linker script, so keep the macro definitions
 *       section clean from C code and use only C-style comments.
 */

#ifndef _TBUF_CHIPS_H_
#define _TBUF_CHIPS_H_

/* NOTE: this file gets exported into the Raaga Magnum host library and so it
 *       must abide by a specific strict set of rules. Please use only ANSI C
 *       constructs and include only FPSDK headers which do as well. After any
 *       edit, always make sure that the Raaga Magnum build succeeds without
 *       any warning.
 */


/* -----------------------------------------------------------
 * Macros describing chip constraints regarding Target Buffers
 * ----------------------------------------------------------- */
/**
 * TB_HAS_SHARED_STRUCT is defined if a TB_shared structure lives
 * somewhere in shared memory for each Target Buffer.
 */
#if !(defined(MCPHY) || defined(WHITNEY))
#  define TB_HAS_SHARED_STRUCT
#endif

/**
 * TB_ASYNC_TRANSFERS: the function issuing the data transfer returns
 *                     immediately; a callback function is then invoked
 *                     when the transfer completes.
 * TB_SYNC_TRANSFERS: the function issuing the data transfer returns only
 *                    when all data has been transferred.
 */
#if defined(CELIVERO) || defined(RAAGA) || defined(MCPHY)
#  define TB_ASYNC_TRANSFERS
#elif defined(DUNA) || defined(PIKE) || defined(WHITNEY)
#  define TB_SYNC_TRANSFERS
#elif defined(__COMPILE_HEADER__)
#  define TB_ASYNC_TRANSFERS    /* fallback choice to avoid the build fail */
#else
#  error "Couldn't derive if TB transfers are synchronous or asynchronous"
#endif

/**
 * TB_KNOWS_TRANSFERS_DEST is defined if, upon transferring toward the
 * Host, the destination buffers are known to the DSP.
 */
#if !(defined(MCPHY) || defined(WHITNEY))
#  define TB_KNOWS_TRANSFERS_DEST
#endif

/**
 * TB_FRAMES_HAVE_TRAILER is defined if frames can have a trailer
 */
#ifdef TB_ASYNC_TRANSFERS
#  define TB_FRAMES_HAVE_TRAILER
#endif

/**
 * TB_TRANSFER_ALIGNMENT: when transferring data, the source and destination
 *                        addresses must be this number of bytes-aligned.
 * TB_TRANSFER_SIZE_MULTIPLE: the number of transferred bytes must be a multiple
 *                            of this number.
 * TB_TRANSFER_SIZE_MAX: the biggest amount of data that can be moved in a
 *                       single transfer.
 */
#define TB_TRANSFER_UNLIMITED_SIZE      (1024 * 1024 * 1024)    /* 1 GiB, just a random high-enough value */
#if defined(CELIVERO)
#  define TB_TRANSFER_ALIGNMENT         8
#  define TB_TRANSFER_SIZE_MULTIPLE     64
#  define TB_TRANSFER_SIZE_MAX          (64 * 1024)
#elif defined(RAAGA)
#  define TB_TRANSFER_ALIGNMENT         1
#  define TB_TRANSFER_SIZE_MULTIPLE     1
#  define TB_TRANSFER_SIZE_MAX          ((64 * 1024) - 1)
#elif defined(MCPHY)
#  define TB_TRANSFER_ALIGNMENT         8               /* both are actually 4, but the GBUS bridge   */
#  define TB_TRANSFER_SIZE_MULTIPLE     8               /* ignores the 3 LSB and complicates our life */
#  define TB_TRANSFER_SIZE_MAX          (8 * 1024)      /* 2048 words */
#elif defined(WHITNEY)                                  /* FIXME: mock details, just to make the SDK    */
#  define TB_TRANSFER_ALIGNMENT         1               /* build; modify once info about this chip will */
#  define TB_TRANSFER_SIZE_MULTIPLE     1               /* become available */
#  define TB_TRANSFER_SIZE_MAX          TB_TRANSFER_UNLIMITED_SIZE
#elif defined(DUNA)
#  define TB_TRANSFER_ALIGNMENT         1
#  define TB_TRANSFER_SIZE_MULTIPLE     1
#  define TB_TRANSFER_SIZE_MAX          TB_TRANSFER_UNLIMITED_SIZE
#elif defined(PIKE)                                     /* We are currently using a Duna-like approach,        */
#  define TB_TRANSFER_ALIGNMENT         1               /* where the DSP stores TB data into a shared memory.  */
#  define TB_TRANSFER_SIZE_MULTIPLE     1               /* TODO: change once we switch to DMA-based transfers. */
#  define TB_TRANSFER_SIZE_MAX          TB_TRANSFER_UNLIMITED_SIZE
#elif defined(__COMPILE_HEADER__)
#  define TB_TRANSFER_ALIGNMENT         1               /* fallback values    */
#  define TB_TRANSFER_SIZE_MULTIPLE     1               /* to avoid the build */
#  define TB_TRANSFER_SIZE_MAX          1               /* system complain    */
#else
#  error "Couldn't derive TB transfers constraints (alignment and size granularity)"
#endif


#if defined(__FIREPATH__) && !defined(__LINKER_SCRIPT__)
/* -------------
 * Helper macros
 * ------------- */
#ifdef TB_HAS_SHARED_STRUCT
#  define IF_TB_HAS_SHARED_STRUCT(...)      __VA_ARGS__
#  define IF_TB_HASNT_SHARED_STRUCT(...)
#else
#  define IF_TB_HAS_SHARED_STRUCT(...)
#  define IF_TB_HASNT_SHARED_STRUCT(...)    __VA_ARGS__
#endif

#ifdef TB_ASYNC_TRANSFERS
#  define IF_TB_ASYNC_TRANSFERS(...)        __VA_ARGS__
#else
#  define IF_TB_ASYNC_TRANSFERS(...)
#endif

#ifdef TB_SYNC_TRANSFERS
#  define IF_TB_SYNC_TRANSFERS(...)         __VA_ARGS__
#else
#  define IF_TB_SYNC_TRANSFERS(...)
#endif

#ifdef TB_KNOWS_TRANSFERS_DEST
#  define IF_TB_KNOWS_TRANSFERS_DEST(...)   __VA_ARGS__
#  define IF_TB_IGNORES_TRANSFERS_DEST(...)
#else
#  define IF_TB_KNOWS_TRANSFERS_DEST(...)
#  define IF_TB_IGNORES_TRANSFERS_DEST(...) __VA_ARGS__
#endif

#ifdef TB_FRAMES_HAVE_TRAILER
#  define IF_TB_FRAMES_HAVE_TRAILER(...)    __VA_ARGS__
#else
#  define IF_TB_FRAMES_HAVE_TRAILER(...)
#endif

#ifdef TARGET_BUFFER_MUX_SERVICES
#  define IF_TB_MUX_SERVICES(...)           __VA_ARGS__
#  define IF_TB_NOMUX_SERVICES(...)
#else
#  define IF_TB_MUX_SERVICES(...)
#  define IF_TB_NOMUX_SERVICES(...)         __VA_ARGS__
#endif



#include "libfp/c_utils.h"

#include "libsyschip/tbuf_services.h"

#include "tbuf.h"


#ifdef __cplusplus
extern "C" {
#endif


/* ---------------------------------- *
 * Shared structures used by services *
 * ---------------------------------- */
#ifdef DUNA
#  define TB_SHARED_ATTRS __pid

#  if !defined(TARGET_BUFFER_MUX_SERVICES)
__got extern char Buffer_TargetPrint[];
__got extern char BufSize_TargetPrint;
__got extern char Buffer_StatProf[];
__got extern char BufSize_StatProf;
__got extern char Buffer_Instrumentation[];
__got extern char BufSize_Instrumentation;
#  else
__got extern char Buffer_TB_Common[];
__got extern char BufSize_TB_Common;
#  endif

#else
#  define TB_SHARED_ATTRS
#endif  /* DUNA */



#ifdef TB_HAS_SHARED_STRUCT
#  ifndef TARGET_BUFFER_MUX_SERVICES
#    if !defined(RAAGA)


/**
 * Common declaration for supported services TB_shared elements.
 * Each of these will be defined in a platform-specific way in tbuf_chips.c
 * @{
 */
TB_SHARED_ATTRS extern TB_shared * const TB_shared_TargetPrint;
TB_SHARED_ATTRS extern TB_shared * const TB_shared_StatProf;
TB_SHARED_ATTRS extern TB_shared * const TB_shared_CoreDump;
TB_SHARED_ATTRS extern TB_shared * const TB_shared_Instrumentation;
/** @} */

#    else

/* Raaga introduced the possibility to move TB_shared around (specifically,
 * to custom FIFOs) before initialising services. */

/** Holds pointers to TB_shared structure in shared memory */
extern TB_shared * TB_shared_services[];
/** @{
 * Macros for uniform access across chips */
#define TB_shared_TargetPrint     (TB_shared_services[TB_SERVICE_TARGET_PRINT])
#define TB_shared_StatProf        (TB_shared_services[TB_SERVICE_STAT_PROF])
#define TB_shared_Instrumentation (TB_shared_services[TB_SERVICE_INSTRUMENTATION])
#define TB_shared_CoreDump        (TB_shared_services[TB_SERVICE_CORE_DUMP])
/** @} */

#    endif  /* !defined(RAAGA) */
#  else  /* TARGET_BUFFER_MUX_SERVICES */


/*
 * Raaga and Celivero core dump code doesn't use the Target Buffer
 * implementation in libsyschip. These chip just dump their raw memory
 * content in at a specific DDR location and eventually update only
 * the proper TB_shared write pointer. Thus, is not currently possible
 * to properly multiplex core dump data in these chips.
 */
#    if defined(CELIVERO)

TB_SHARED_ATTRS extern TB_shared * const TB_shared_CoreDump;
TB_SHARED_ATTRS extern TB_shared * const TB_shared_Common;

#    elif defined(RAAGA)

/* Raaga TBs are not const as they can be moved at runtime,
 * see TB_set_*_fifo_addr functions. */
TB_SHARED_ATTRS extern TB_shared * TB_shared_CoreDump;
TB_SHARED_ATTRS extern TB_shared * TB_shared_Common;

#    elif defined(DUNA) || defined(PIKE)

TB_SHARED_ATTRS extern TB_shared * const TB_shared_Common;

#    endif


/** @{
 * Macros to ease accesses from services code */
#define TB_shared_TargetPrint     TB_shared_Common
#define TB_shared_StatProf        TB_shared_Common
#define TB_shared_Instrumentation TB_shared_Common
#if !(defined(CELIVERO) || defined(RAAGA))
#  define TB_shared_CoreDump      TB_shared_Common
#endif
/** @} */


/* Forward declaration - we don't want to include tbuf_shared.h here */
struct TB_shared_mgt_t;

/**
 * See the chip-specific declaration in tbuf_chips.c
 */
extern struct TB_shared_mgt_t TB_shared_mgt_Common;


/** @{
 * Macros to ease accesses from services code */
#define TB_shared_mgt_TargetPrint     TB_shared_mgt_Common
#define TB_shared_mgt_StatProf        TB_shared_mgt_Common
#define TB_shared_mgt_Instrumentation TB_shared_mgt_Common
#if !(defined(CELIVERO) || defined(RAAGA))
#  define TB_shared_mgt_CoreDump      TB_shared_mgt_Common
#endif
/** @} */


#  endif  /* !defined(TARGET_BUFFER_MUX_SERVICES) */
#endif  /* TB_HAS_SHARED_STRUCT */


#undef TB_SHARED_ATTRS  /* don't propagate this macro */



/* --------- *
 * Functions *
 * --------- */
#if defined(RAAGA)

/* Define which DMA channel is used for TB transfers */
#define TB_RAAGA_DMA_CHANNEL    DMA_Q_CODE_BG


#  if !defined(TARGET_BUFFER_MUX_SERVICES)

/**
 * Choose on which FIFO to allocate the TB_shared structure that will be used
 * by the specified service Target Buffer. Call this function before invoking
 * the corresponding service initialisation function.
 *
 * @param[in] service   the service to set the FIFO for
 * @param[in] fifo_addr address of the FIFO used for the service
 */
void TB_set_fifo_addr(TB_service_id service, void *fifo_addr);


/**
 * Return the address of the FIFO holding a specific service TB_shared
 * structure, or NULL if not set.
 *
 * @param[in] service the service to query
 * @return            address of the FIFO associated with this service,
 *                    NULL if the specified service FIFO has not been set
 *                    (TB_set_fifo_addr has not been called)
 */
void *TB_get_fifo_addr(TB_service_id service);


#  else  /* !defined(TARGET_BUFFER_MUX_SERVICES) */

/**
 * Choose on which FIFO to allocate the TB_shared structure that will be used
 * for core dump data evacuation.
 *
 * @param[in] fifo_addr address of the FIFO used for core dumps
 */
void TB_set_core_dump_fifo_addr(void *fifo_addr);


/**
 * Choose on which FIFO to allocate the TB_shared structure that will be used
 * for all the services except core dumps.
 *
 * @param[in] fifo_addr address of the 'common' Target Buffer FIFO
 */
void TB_set_common_fifo_addr(void *fifo_addr);


/**
 * Return the address of the FIFO holding the core dump TB_shared
 * structure, or NULL if not set.
 *
 * @return address of the FIFO associated with core dumps, NULL if the FIFO
 *         has not been set (TB_set_core_dump_fifo_addr has not been called)
 */
void *TB_get_core_dump_fifo_addr(void);


/**
 * Return the address of the FIFO holding the 'common' TB_shared
 * structure, or NULL if not set.
 *
 * @return address of the FIFO associated with all services except core dumps, NULL
 *         if such FIFO has not been set (TB_set_common_fifo_addr has not been called)
 */
void *TB_get_common_fifo_addr(void);


#  endif  /* !defined(TARGET_BUFFER_MUX_SERVICES) */
#endif  /* defined(RAAGA) */


#ifdef __cplusplus
}
#endif

#endif  /* defined(__FIREPATH__) && !defined(__LINKER_SCRIPT__) */


#endif  /* _TBUF_CHIPS_H_ */
