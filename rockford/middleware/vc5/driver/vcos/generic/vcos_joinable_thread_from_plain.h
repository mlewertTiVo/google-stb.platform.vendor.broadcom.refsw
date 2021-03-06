/*
 * Copyright (c) 2010-2011 Broadcom. All rights reserved.
 *
 * This program is the proprietary software of Broadcom Corporation and/or
 * its licensors, and may only be used, duplicated, modified or distributed
 * pursuant to the terms and conditions of a separate, written license
 * agreement executed between you and Broadcom (an "Authorized License").
 * Except as set forth in an Authorized License, Broadcom grants no license
 * (express or implied), right to use, or waiver of any kind with respect to
 * the Software, and Broadcom expressly reserves all rights in and to the
 * Software and all intellectual property rights therein.  IF YOU HAVE NO
 * AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
 * WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF
 * THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 * 1. This program, including its structure, sequence and organization,
 *    constitutes the valuable trade secrets of Broadcom, and you shall use
 *    all reasonable efforts to protect the confidentiality thereof, and to
 *    use this information only in connection with your use of Broadcom
 *    integrated circuit products.
 * 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 *    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
 *    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
 *    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
 *    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 *    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
 *    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
 *    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
 *    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
 *    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 */

/*=============================================================================
VideoCore OS Abstraction Layer - implementation: joinable thread from plain
=============================================================================*/

/** \file
  *
  * Header file for platforms creating the joinable thread from a lowlevel
  * thread.
  *
  * In addition to the actual thread, the following are also created:
  *
  * - a semaphore to wait on when joining the thread
  * - a semaphore to support counted suspend/resume (used by event group)
  * - a per-thread timer (used by event group, but could be removed)
  */

#ifndef VCOS_JOINABLE_THREAD_FROM_PLAIN_H
#define VCOS_JOINABLE_THREAD_FROM_PLAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VCOS_SEMAPHORE_H
#include "vcos_semaphore.h"
#endif
#ifndef VCOS_LOWLEVEL_THREAD_H
#include "vcos_lowlevel_thread.h"
#endif
#ifndef VCOS_TIMER_H
#include "vcos_timer.h"
#endif

#ifdef VCOS_WANT_TLS_EMULATION
#include "generic/vcos_generic_tls.h"
#endif

#define VCOS_THREAD_MAGIC 0x56436a74

#define VCOS_THREAD_VALID(t) (t->magic == VCOS_THREAD_MAGIC)
#define VCOS_HAVE_THREAD_AT_EXIT        1

/** Thread attribute structure. Clients should not manipulate this directly, but
  * should instead use the provided functions.
  */
typedef struct VCOS_THREAD_ATTR_T
{
   void *ta_stackaddr;
   VCOS_UNSIGNED ta_stacksz;
   VCOS_UNSIGNED ta_priority;
   VCOS_UNSIGNED ta_affinity;
   VCOS_UNSIGNED ta_timeslice;
   VCOS_UNSIGNED legacy;
   VCOS_UNSIGNED ta_autostart;
} VCOS_THREAD_ATTR_T;

/** Each thread gets a timer, which is for internal VCOS use.
  */
typedef struct _VCOS_THREAD_TIMER_T
{
   VCOS_TIMER_T timer;
   void (*pfn)(void *);
   void *cxt;
} _VCOS_THREAD_TIMER_T;

typedef void (*VCOS_THREAD_EXIT_HANDLER_T)(void *);
/** Called at thread exit.
  */
typedef struct VCOS_THREAD_EXIT_T
{
   VCOS_THREAD_EXIT_HANDLER_T pfn;
   void *cxt;
} VCOS_THREAD_EXIT_T;
#define VCOS_MAX_EXIT_HANDLERS  8

/* The name field isn't used for anything, so we can just copy the
 * the pointer. Nucleus makes its own copy.
 */
typedef const char *          VCOS_LLTHREAD_T_NAME;
#define _VCOS_LLTHREAD_NAME(dst,src) (dst)=(src)

/*
 * Simulated TLS support
 */


/** Thread structure.
  *
  * \warning Do not access the members of this structure directly!
  */
typedef struct VCOS_THREAD_T
{
   VCOS_LLTHREAD_T  thread;      /**< The underlying thread */
   char name[16];                /**< The name */
   unsigned int     magic;       /**< For debug */
   void            *exit_data;   /**< Exit data passed out in vcos_joinable_thread_exit() */
   void            *stack;       /**< Stack, if not supplied by caller */
   VCOS_SEMAPHORE_T wait;        /**< Semaphore to wait on at join */
   VCOS_SEMAPHORE_T suspend;     /**< Semaphore to wait on for counted suspend */
   int16_t          joined;      /**< Joined yet? For debug. */
   VCOS_UNSIGNED    legacy;      /**< Use (argc,argv) for entry point arguments */
   void *(*entry)(void*);        /**< Entry point */
   void             *arg;        /**< Argument passed to entry point */
   void *(*term)(void*);         /**< Termination function, used by reaper */
   void             *term_arg;   /**< Argument passed to termination function */
   _VCOS_THREAD_TIMER_T _timer;  /**< Internal timer, mainly for event groups */
#ifdef VCOS_WANT_TLS_EMULATION
   VCOS_TLS_THREAD_T   _tls;     /**< TLS data when native TLS not available, or NULL */
#endif
   /** Array of functions to call at thread exit */
   VCOS_THREAD_EXIT_T at_exit[VCOS_MAX_EXIT_HANDLERS];

   struct VCOS_THREAD_T *next;   /**< For linked lists of threads */
} VCOS_THREAD_T;

static inline void vcos_thread_attr_setstack(VCOS_THREAD_ATTR_T *attrs, void *addr, VCOS_UNSIGNED stacksz) {
   attrs->ta_stackaddr = addr;
   attrs->ta_stacksz = stacksz;
}

static inline void vcos_thread_attr_setstacksize(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED stacksz) {
   attrs->ta_stacksz = stacksz;
}

static inline void vcos_thread_attr_setpriority(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED pri) {
   attrs->ta_priority = pri;
}

static inline void vcos_thread_attr_setaffinity(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED affinity) {
   attrs->ta_affinity = affinity;
}

static inline void vcos_thread_attr_settimeslice(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED ts) {
   attrs->ta_timeslice = ts;
}

static inline void _vcos_thread_attr_setlegacyapi(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED legacy) {
   attrs->legacy = legacy;
}

static inline void vcos_thread_attr_setautostart(VCOS_THREAD_ATTR_T *attrs, VCOS_UNSIGNED autostart) {
   attrs->ta_autostart = autostart;
}

static inline VCOS_THREAD_T *vcos_thread_current(void) {
   VCOS_THREAD_T *ret =  (VCOS_THREAD_T*)vcos_llthread_current();
   /*If we're called from a non-vcos thread, this assert will fail.
    *XXX FIXME why is this commented out?
    *assert(ret->magic == VCOS_THREAD_MAGIC);
    */
   return ret;
}

/**
  * \brief Create a VCOS_THREAD_T for the current thread. This is so we can have
  * VCOS_THREAD_Ts even for threads not originally created by VCOS (eg the
  * thread that calls vcos_init)
  */
extern VCOS_STATUS_T _vcos_thread_create_attach(VCOS_THREAD_T *thread,
                                                const char *name);

/**
  * \brief Deletes the VCOS_THREAD_T, but does not wait for the underlying
  * thread to exit. This will cleanup everything created by
  * _vcos_thread_create_attach
  */
extern void _vcos_thread_delete(VCOS_THREAD_T *thread);

/** Register a function to be called when the current thread exits.
  */
extern VCOS_STATUS_T vcos_thread_at_exit(void (*pfn)(void*), void *cxt);

/** Deregister a previously registered at-exit function.
  */
extern void vcos_thread_deregister_at_exit(void (*pfn)(void*), void *cxt);

#ifdef __cplusplus
}
#endif
#endif /* VCOS_JOINABLE_THREAD_FROM_PLAIN_H */
