/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#ifndef KHRN_FMEM_4_H
#define KHRN_FMEM_4_H

#include "khrn_fmem_pool.h"
#include "khrn_tile_state.h"
#include "khrn_fmem_debug_info.h"
#include "khrn_uintptr_vector.h"
#include "khrn_res_interlock.h"
#include "khrn_fence.h"
#include "libs/core/v3d/v3d_align.h"
#include "libs/platform/v3d_scheduler.h"
#include "vcos.h"

#if KHRN_DEBUG
#include "khrn_memaccess.h"
#endif

VCOS_EXTERN_C_BEGIN

typedef uint64_t job_t;

typedef struct
{
   uint8_t *start;
   size_t end;
   size_t current;
} KHRN_FMEM_BLOCK_T;

typedef struct glxx_query_block glxx_query_block;
typedef struct glxx_ustream_job glxx_ustream_job;
typedef struct glxx_ustream_job_block glxx_ustream_job_block;

/* Precious things of the FMEM that need to live longer than the render state
   reside in khrn_fmem_persist, which is cleaned up when the
   completion callbacks for bin/render jobs have bin signalled */
typedef struct khrn_fmem_persist
{
   KHRN_FMEM_POOL_T pool;                 // memory pool for control lists
   /* Debug info */
#ifdef KHRN_GEOMD
   struct fmem_debug_info_vector debug_info;
#endif
#if KHRN_DEBUG
   void* gmp_tables[2];
#endif

   KHRN_UINTPTR_VECTOR_T client_handles;  // client-allocated handles

   glxx_query_block *occlusion_query_list;
#if V3D_VER_AT_LEAST(4,0,2,0)
   glxx_query_block *prim_counts_query_list;
#endif

   gmem_handle_t bin_tile_state[V3D_MAX_CORES];
   unsigned num_bin_tile_states;
   khrn_shared_tile_state* bin_shared_tile_state;

   khrn_interlock_stages_t delete_stage;

   glxx_ustream_job_block* ustream_jobs;
   glxx_ustream_job_block* ustream_jobs_tail;
   uint32_t num_tail_ustream_jobs;

} khrn_fmem_persist;

struct khrn_fmem_tmu_cfg_alloc
{
#if V3D_VER_AT_LEAST(4,0,2,0)
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
   khrn_fmem_persist* persist;

   V3D_BIN_RENDER_INFO_T br_info;
   v3d_barrier_flags bin_rw_flags;
   v3d_barrier_flags render_rw_flags;

   KHRN_RENDER_STATE_T *render_state;

   KHRN_UINTPTR_VECTOR_T res_interlocks;
   KHRN_UINTPTR_VECTOR_T fences_to_signal;
   KHRN_UINTPTR_VECTOR_T fences_to_depend_on;

#if KHRN_DEBUG
   khrn_memaccess* memaccess;
#endif
#ifndef NDEBUG
   bool in_begin_clist;
   uint8_t *in_begin_cle_start;
   uint8_t *in_begin_cle_end;
   uint8_t *in_begin_data_start;
   uint8_t *in_begin_data_end;
#endif
} KHRN_FMEM_T, khrn_fmem;

extern uint32_t khrn_fmem_frame_i;

extern bool khrn_fmem_init(KHRN_FMEM_T *fmem, KHRN_RENDER_STATE_T *render_state);
extern bool khrn_fmem_reset(KHRN_FMEM_T *fmem, KHRN_RENDER_STATE_T *render_state);
extern void khrn_fmem_discard(KHRN_FMEM_T *fmem);
extern uint32_t *khrn_fmem_data(KHRN_FMEM_T *fmem, unsigned size, unsigned align);
extern uint8_t *khrn_fmem_cle(KHRN_FMEM_T *fmem, unsigned size);

static inline bool khrn_fmem_should_flush(KHRN_FMEM_T *fmem)
{
   return khrn_fmem_pool_should_flush(&fmem->persist->pool);
}

// required to call begin_clist before any calls to khrn_fmem_*_cle
// begin_clist is called automatically in khrn_fmem_init
extern v3d_addr_t khrn_fmem_begin_clist(KHRN_FMEM_T *fmem);
extern v3d_addr_t khrn_fmem_end_clist(KHRN_FMEM_T *fmem);

#if V3D_VER_AT_LEAST(4,0,2,0)
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
extern bool khrn_fmem_record_res_interlock_read(KHRN_FMEM_T *fmem,
      KHRN_RES_INTERLOCK_T *res_i, khrn_interlock_stages_t stages);
extern bool khrn_fmem_record_res_interlock_write(KHRN_FMEM_T *fmem,
      KHRN_RES_INTERLOCK_T *res_i, khrn_interlock_stages_t stages,
      khrn_interlock_parts_t parts, bool *invalid);

/*
 * Returns false if we're already reading the interlock (*requires_flush will
 * be true), or if we run out of memory (*requires_flush will be false).
 */
extern bool khrn_fmem_record_res_interlock_self_read_conflicting_write(KHRN_FMEM_T *fmem,
      KHRN_RES_INTERLOCK_T *res_i, khrn_interlock_stages_t stages,
      khrn_interlock_parts_t parts, bool* requires_flush);

extern bool khrn_fmem_record_handle(KHRN_FMEM_T *fmem, gmem_handle_t handle);

extern bool khrn_fmem_record_fence_to_signal(KHRN_FMEM_T *fmem,
      KHRN_FENCE_T *fence);
extern bool khrn_fmem_record_fence_to_depend_on(KHRN_FMEM_T *fmem,
      KHRN_FENCE_T *fence);

static inline bool khrn_fmem_has_queries(KHRN_FMEM_T *fmem)
{
   bool res =  fmem->persist->occlusion_query_list != NULL;
#if V3D_VER_AT_LEAST(4,0,2,0)
   res = res || (fmem->persist->prim_counts_query_list != NULL);
#endif
   return res;
}

glxx_ustream_job* khrn_fmem_add_ustream_job(khrn_fmem* fmem);

VCOS_EXTERN_C_END

#endif
