/*=============================================================================
Copyright (c) 2010 Broadcom Europe Limited.
All rights reserved.

Project  :  khronos
Module   :  Control per-frame memory allocator

FILE DESCRIPTION
Handles allocation of memory for control lists and associated data that will be
generated each frame as HW input.
=============================================================================*/
#ifndef KHRN_FMEM_4_H
#define KHRN_FMEM_4_H

#include "middleware/khronos/common/khrn_fmem_pool.h"
#include "middleware/khronos/common/khrn_fmem_debug_info.h"
#include "middleware/khronos/common/khrn_uintptr_vector.h"
#include "middleware/khronos/common/khrn_res_interlock.h"
#include "middleware/khronos/common/khrn_fence.h"
#include "middleware/khronos/common/khrn_synclist.h"
#include "helpers/v3d/v3d_align.h"
#include "v3d_scheduler.h"
#include "vcos.h"

VCOS_EXTERN_C_BEGIN

#define KHRN_CLE_BRANCH_SIZE 5   /* for linking control list blocks */

typedef uint64_t job_t;

typedef struct
{
   uint8_t *start;
   size_t end;
   size_t current;
} KHRN_FMEM_BLOCK_T;

struct glxx_query;

/* Do not change this unless you know what you are doing. See the comments
 * above the definition of V3D_QUERY_COUNTER_SINGLE_CORE_CACHE_LINE_COUNTERS
 * for why. */
#define KHRN_CLE_QUERY_COUNT V3D_QUERY_COUNTER_SINGLE_CORE_CACHE_LINE_COUNTERS

struct khrn_query_block
{
   /* query_values must be KHRN_CLE_QUERY_BLOCK_ALIGN aligned. So it's at the
    * start of the structure and we make sure the structure is sufficiently
    * aligned when allocating. */
#define KHRN_CLE_QUERY_BLOCK_ALIGN V3D_QUERY_COUNTER_FIRST_CORE_CACHE_LINE_ALIGN
   uint32_t query_values[V3D_MAX_CORES * KHRN_CLE_QUERY_COUNT];

   struct
   {
      struct glxx_query* obj;
      uint64_t instance;
   }query [KHRN_CLE_QUERY_COUNT];

   unsigned count;

   struct khrn_query_block* prev;
};

typedef struct khrn_query_block KHRN_QUERY_BLOCK_T;

/* Precious things of the FMEM that need to live longer than the render state
   reside in khrn_fmem_common_persist, which is cleaned up when the
   completion callbacks for bin/render jobs have bin signalled */
typedef struct
{
   KHRN_FMEM_POOL_T pool;                 // memory pool for control lists
   /* Debug info */
#ifdef KHRN_GEOMD
   struct fmem_debug_info_vector debug_info;
#endif
   KHRN_UINTPTR_VECTOR_T client_handles;  // client-allocated handles
   KHRN_QUERY_BLOCK_T *query_list; // allocation for this list comes form fmem_data

   gmem_handle_t bin_tile_state[V3D_MAX_CORES];
   unsigned num_bin_tile_states;

}khrn_fmem_common_persist;

typedef struct
{
   khrn_fmem_common_persist *common;
   struct gmem_lock_list lock_list;
   struct khrn_synclist sync_list;
} khrn_fmem_stage_persist;

enum
{
   BIN_STAGE,
   RENDER_STAGE,
   COUNT_STAGES
};

struct khrn_fmem_tmu_cfg_alloc
{
#if V3D_HAS_NEW_TMU_CFG
   void *spare_8; /* 8-byte aligned, 8-byte size. Valid if not NULL. */
   void *spare_16; /* 16-byte aligned, 16-byte size. Valid if not NULL. */
   void *spare_32; /* 32-byte aligned */
   unsigned num_32; /* Number of 32-byte blocks at spare_32 */
#else
   uint8_t* spare;
   unsigned num_spare;
#endif
};

typedef struct khrn_fmem
{
   KHRN_FMEM_BLOCK_T cle;
   KHRN_FMEM_BLOCK_T data;
   uint8_t* cle_first;
   struct khrn_fmem_tmu_cfg_alloc tmu_cfg_alloc;
   khrn_fmem_stage_persist* persist[COUNT_STAGES];

   V3D_BIN_RENDER_INFO_T br_info;

   KHRN_RENDER_STATE_T *render_state;

   KHRN_UINTPTR_VECTOR_T res_interlocks;
   KHRN_UINTPTR_VECTOR_T fences_to_signal;
   KHRN_UINTPTR_VECTOR_T fences_to_depend_on;

#ifndef NDEBUG
   bool in_begin_clist;
   uint8_t *in_begin_cle_start;
   uint8_t *in_begin_cle_end;
   uint8_t *in_begin_data_start;
   uint8_t *in_begin_data_end;
#endif
} KHRN_FMEM_T, khrn_fmem;

extern uint32_t khrn_fmem_frame_i;

/* when the number of free fmems and number of submitted buffers in the client
 * pool goes below this threshold, start flushing other render states except the
 * current one, till we get over this threshold or there are no other render
 * states to flush */
#define KHRN_FMEM_THRESHOLD_FLUSH_OTHER_RS 4
static_assrt(KHRN_FMEM_THRESHOLD_FLUSH_OTHER_RS <= (KHRN_FMEM_MAX_BLOCKS/2));

/* when a rs is using  KHRN_FMEM_MAX_PER_RS fmems, if the rs gets installed as
 * current, we are going to flush this rs before installing it */
#define KHRN_FMEM_MAX_PER_RS (KHRN_FMEM_MAX_BLOCKS - 4)
static_assrt(KHRN_FMEM_MAX_PER_RS > 0);

extern bool khrn_fmem_init(KHRN_FMEM_T *fmem, KHRN_RENDER_STATE_T *render_state);
extern bool khrn_fmem_reset(KHRN_FMEM_T *fmem, KHRN_RENDER_STATE_T *render_state);
extern void khrn_fmem_discard(KHRN_FMEM_T *fmem);
extern uint32_t *khrn_fmem_data(KHRN_FMEM_T *fmem, unsigned size, unsigned align);
extern uint8_t *khrn_fmem_cle(KHRN_FMEM_T *fmem, unsigned size);
extern bool khrn_fmem_check_flush(KHRN_FMEM_T *fmem);

// required to call begin_clist before any calls to khrn_fmem_*_cle
// begin_clist is called automatically in khrn_fmem_init
extern uint8_t *khrn_fmem_begin_clist(KHRN_FMEM_T *fmem);
extern uint8_t *khrn_fmem_end_clist(KHRN_FMEM_T *fmem);

#if V3D_HAS_NEW_TMU_CFG
extern v3d_addr_t khrn_fmem_add_tmu_tex_state(KHRN_FMEM_T *fmem,
   const void *tex_state, bool extended);
extern v3d_addr_t khrn_fmem_add_tmu_sampler(KHRN_FMEM_T *fmem,
   const void *sampler, bool extended);
#else
extern v3d_addr_t khrn_fmem_add_tmu_indirect(KHRN_FMEM_T *fmem, uint32_t const *tmu_indirect);
#endif

//! Return number of bytes left in this fmem buffer.
static inline unsigned khrn_fmem_cle_buffer_remaining(KHRN_FMEM_T *fmem)
{
   return fmem->cle.end - fmem->cle.current;
}

//! Begin writing to the cle specifying the maximum number of bytes to write.
static inline uint8_t *khrn_fmem_begin_cle(KHRN_FMEM_T *fmem, unsigned max_size)
{
   // khrn_fmem_begin_cle must be closed with khrn_fmem_end_cle
   assert(!fmem->in_begin_cle_start);

   uint8_t *ptr = khrn_fmem_cle(fmem, max_size);
#ifndef NDEBUG
   fmem->in_begin_cle_start = ptr;
   fmem->in_begin_cle_end = fmem->cle.start + fmem->cle.current;
#endif
   return ptr;
}

//! Close a previous call to khrn_fmem_begin_cle.
static inline void khrn_fmem_end_cle(KHRN_FMEM_T *fmem, uint8_t *end)
{
   // khrn_fmem_end_cle must be preceeded by successful khrn_fmem_begin_cle
   // and end pointer must be within the allocation
   assert(end >= fmem->in_begin_cle_start && end <= fmem->in_begin_cle_end);
   assert((fmem->cle.start + fmem->cle.current) == fmem->in_begin_cle_end);

   fmem->cle.current = end - fmem->cle.start;
#ifndef NDEBUG
   fmem->in_begin_cle_start = NULL;
   fmem->in_begin_cle_end = NULL;
#endif
}

//! Close a previous call to khrn_fmem_begin_cle exactly.
static inline void khrn_fmem_end_cle_exact(KHRN_FMEM_T *fmem, uint8_t *end)
{
   // khrn_fmem_end_cle must be preceeded by successful khrn_fmem_begin_cle
   // and end pointer must match the number of bytes allocated
   assert(end == fmem->in_begin_cle_end);
   assert((fmem->cle.start + fmem->cle.current) == fmem->in_begin_cle_end);

#ifndef NDEBUG
   fmem->in_begin_cle_start = NULL;
   fmem->in_begin_cle_end = NULL;
#endif
}

//! Begin writing to the data specifying the maximum number of bytes to write.
static inline void *khrn_fmem_begin_data(KHRN_FMEM_T *fmem, unsigned max_size, unsigned align)
{
   // khrn_fmem_begin_data must be closed with khrn_fmem_end_data
   assert(!fmem->in_begin_data_start);

   void *ptr = khrn_fmem_data(fmem, max_size, align);
#ifndef NDEBUG
   fmem->in_begin_data_start = (uint8_t*)ptr;
   fmem->in_begin_data_end = fmem->data.start + fmem->data.current;
#endif
   return ptr;
}

//! Close a previous call to khrn_fmem_begin_data.
static inline void khrn_fmem_end_data(KHRN_FMEM_T *fmem, void *end)
{
   // khrn_fmem_end_data must be preceeded by successful khrn_fmem_begin_data
   // and end pointer must be within the allocation
   assert((uint8_t*)end >= fmem->in_begin_data_start && (uint8_t*)end <= fmem->in_begin_data_end);
   assert((fmem->data.start + fmem->data.current) == fmem->in_begin_data_end);

   fmem->data.current = (uint8_t*)end - fmem->data.start;
#ifndef NDEBUG
   fmem->in_begin_data_start = NULL;
   fmem->in_begin_data_end = NULL;
#endif
}

//! Close a previous call to khrn_fmem_begin_data exactly.
static inline void khrn_fmem_end_data_exact(KHRN_FMEM_T *fmem, void *end)
{
   // khrn_fmem_end_data must be preceeded by successful khrn_fmem_begin_data
   // and end pointer must match the number of bytes allocated
   assert(end == fmem->in_begin_data_end);
   assert((fmem->data.start + fmem->data.current) == fmem->in_begin_data_end);

#ifndef NDEBUG
   fmem->in_begin_data_start = NULL;
   fmem->in_begin_data_end = NULL;
#endif
}

/* Add handle to fmem's sync lists, and return a corresponding v3d_addr_t */
extern v3d_addr_t khrn_fmem_lock_and_sync(KHRN_FMEM_T *fmem,
      gmem_handle_t handle, uint32_t bin_rw_flags, uint32_t render_rw_flags);

extern v3d_addr_t khrn_fmem_tile_state_alloc(KHRN_FMEM_T *fmem, size_t size);

/* Flush out any binning and/or rendering */
extern void khrn_fmem_flush(KHRN_FMEM_T *fmem);

//extern bool khrn_fmem_special(KHRN_FMEM_T *fmem, uint32_t *location, uint32_t special_i, uint32_t offset);
extern bool khrn_fmem_is_here(KHRN_FMEM_T *fmem, uint8_t *p);

extern v3d_addr_t khrn_fmem_hw_address(KHRN_FMEM_T *fmem, void *p);

/*
 * mark the fact that fmem is using this res_interlock for read or write during
 * binning/rendering/etc;
 */
extern bool khrn_fmem_record_res_interlock(KHRN_FMEM_T *fmem,
      KHRN_RES_INTERLOCK_T *res_i, bool write, khrn_interlock_action_t actions);

/*
 * Mark the fact that fmem is using this res_interlock for transform feedback writes
 * during binning. Returns false if this operation failed; either because the
 * render state requires a flush (already reading the interlock), or ran out of memory.
 */
extern bool khrn_fmem_record_res_interlock_tf_write(KHRN_FMEM_T *fmem,
      KHRN_RES_INTERLOCK_T *res_i, bool* requires_flush);

extern bool khrn_fmem_record_handle(KHRN_FMEM_T *fmem, gmem_handle_t handle);

extern bool khrn_fmem_record_fence_to_signal(KHRN_FMEM_T *fmem,
      KHRN_FENCE_T *fence);
extern bool khrn_fmem_record_fence_to_depend_on(KHRN_FMEM_T *fmem,
      KHRN_FENCE_T *fence);

void khrn_fmem_new_query_entry(KHRN_FMEM_T *fmem,
      KHRN_QUERY_BLOCK_T **block, unsigned *index);

static inline khrn_fmem_common_persist* khrn_fmem_get_common_persist(KHRN_FMEM_T *fmem)
{
   return fmem->persist[BIN_STAGE]->common;
}

VCOS_EXTERN_C_END

#endif
