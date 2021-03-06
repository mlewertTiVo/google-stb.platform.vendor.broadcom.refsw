/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.

Project  :  helpers
Module   :

FILE DESCRIPTION
=============================================================================*/

#ifndef GFX_LFMT_TRANSLATE_V3D_H
#define GFX_LFMT_TRANSLATE_V3D_H

#include "helpers/gfx/gfx_lfmt.h"
#include "helpers/v3d/v3d_common.h"
#include "helpers/v3d/v3d_cl.h"
#include "helpers/v3d/v3d_tmu.h"
#include "helpers/v3d/v3d_tfu.h"

VCOS_EXTERN_C_BEGIN

extern v3d_index_type_t gfx_lfmt_translate_index_type(GFX_LFMT_T lfmt);

/** TLB/TFU memory format */

/* These functions return dims (=2D) & swizzling */
extern GFX_LFMT_T gfx_lfmt_translate_from_memory_format(
   v3d_memory_format_t memory_format);
extern GFX_LFMT_T gfx_lfmt_translate_from_tfu_iformat(
   v3d_tfu_iformat_t tfu_iformat);
extern GFX_LFMT_T gfx_lfmt_translate_from_tfu_oformat(
   v3d_tfu_oformat_t tfu_oformat);

/** TLB pixel format (for color buffers) */

/* These only pay attention to format, ignoring premultipliedness */
extern v3d_pixel_format_t gfx_lfmt_maybe_translate_pixel_format(GFX_LFMT_T lfmt);
extern v3d_pixel_format_t gfx_lfmt_translate_pixel_format(GFX_LFMT_T lfmt);

/* Returns just format */
extern GFX_LFMT_T gfx_lfmt_translate_from_pixel_format(v3d_pixel_format_t pixel_format);

/* Returns dims, layout, and format */
static inline GFX_LFMT_T gfx_lfmt_translate_from_memory_pixel_format_and_flipy(
   v3d_memory_format_t memory_format, v3d_pixel_format_t pixel_format, bool flipy)
{
   GFX_LFMT_T lfmt = gfx_lfmt_set_format(
      gfx_lfmt_translate_from_memory_format(memory_format),
      gfx_lfmt_translate_from_pixel_format(pixel_format));
   if (flipy)
   {
      gfx_lfmt_set_base(&lfmt, gfx_lfmt_yflip_base(gfx_lfmt_get_base(&lfmt)));
      gfx_lfmt_set_yflip(&lfmt, GFX_LFMT_YFLIP_YFLIP);
   }
   return lfmt;
}

/* Returns dims & format */
extern GFX_LFMT_T gfx_lfmt_translate_internal_raw_mode(GFX_LFMT_T lfmt);

/** TLB depth format */

/* Note that when the TLB loads/stores only stencil with a depth-only depth
 * format, it assumes an 8bpp stencil format, ignoring the actual depth format.
 * This case needs to be handled by the calling code */

/* These only pay attention to format */
extern v3d_depth_format_t gfx_lfmt_maybe_translate_depth_format(GFX_LFMT_T lfmt);
extern v3d_depth_format_t gfx_lfmt_translate_depth_format(GFX_LFMT_T lfmt);
extern v3d_depth_type_t gfx_lfmt_translate_depth_type(GFX_LFMT_T lfmt);

/* Returns just format */
extern GFX_LFMT_T gfx_lfmt_translate_from_depth_format(v3d_depth_format_t depth_format);

/* Returns dims, layout, and format */
static inline GFX_LFMT_T gfx_lfmt_translate_from_memory_and_depth_format(
   v3d_memory_format_t memory_format, v3d_depth_format_t depth_format)
{
   return gfx_lfmt_set_format(
      gfx_lfmt_translate_from_memory_format(memory_format),
      gfx_lfmt_translate_from_depth_format(depth_format));
}

/** TMU */

typedef enum
{
   GFX_LFMT_TMU_DEPTH_ALWAYS, /* Caller requires v3d_tmu_is_depth_type(t->type) */
   GFX_LFMT_TMU_DEPTH_NEVER, /* Caller requires !v3d_tmu_is_depth_type(t->type) */
   GFX_LFMT_TMU_DEPTH_DONT_CARE
} gfx_lfmt_tmu_depth_pref_t;

/* format of data returned from tmu. the tmu word enables can limit the number
 * of words returned */
typedef enum
{
   /* tmu returns packed data */
   GFX_LFMT_TMU_RET_8,        /* 4x8-bit per word  */
   GFX_LFMT_TMU_RET_16,       /* 2x16-bit per word */
   GFX_LFMT_TMU_RET_32,       /* 1x32-bit per word */
   GFX_LFMT_TMU_RET_1010102,  /* 1010102 as one word */

   /* tmu returns data as 2x16 or 1x32, switchable */
   GFX_LFMT_TMU_RET_AUTO,

   GFX_LFMT_TMU_RET_INVALID
} gfx_lfmt_tmu_ret_t;

typedef struct
{
   v3d_tmu_type_t type;
   bool srgb;

   gfx_lfmt_tmu_ret_t ret;

   /* If shader_swizzle is true, swizzling needs to be done in the shader --
    * the TMU swizzles should just be set to R,G,B,A. Otherwise, ie if
    * shader_swizzle is false, swizzles should be passed to the TMU and the
    * shader doesn't need to do any swizzling. */
   bool shader_swizzle;
   v3d_tmu_swizzle_t swizzles[4];
} GFX_LFMT_TMU_TRANSLATION_T;

/* These only pay attention to format, ignoring premultipliedness.
 * false is returned on failure. */
extern bool gfx_lfmt_maybe_translate_tmu(GFX_LFMT_TMU_TRANSLATION_T *t,
   GFX_LFMT_T lfmt, gfx_lfmt_tmu_depth_pref_t depth_pref,
   int v3d_version);
extern void gfx_lfmt_translate_tmu(GFX_LFMT_TMU_TRANSLATION_T *t,
   GFX_LFMT_T lfmt, gfx_lfmt_tmu_depth_pref_t depth_pref,
   int v3d_version);

/* Returns just format */
extern GFX_LFMT_T gfx_lfmt_translate_from_tmu_type(
   v3d_tmu_type_t tmu_type, bool srgb, int v3d_version);

/* Returns just dims */
extern GFX_LFMT_DIMS_T gfx_lfmt_translate_from_tmu_ltype(v3d_tmu_ltype_t ltype);

/* Returns dims and format */
static inline GFX_LFMT_T gfx_lfmt_translate_from_tmu_type_and_ltype(
   v3d_tmu_type_t tmu_type, bool srgb, v3d_tmu_ltype_t ltype, int v3d_version)
{
   GFX_LFMT_T dims_only = GFX_LFMT_NONE;
   gfx_lfmt_set_dims(&dims_only, gfx_lfmt_translate_from_tmu_ltype(ltype));
   return gfx_lfmt_set_format(
      dims_only,
      gfx_lfmt_translate_from_tmu_type(tmu_type, srgb, v3d_version));
}

/** TFU */

/* Returns just formats */
extern void gfx_lfmt_translate_from_tfu_type(
   GFX_LFMT_T *fmts, uint32_t *num_planes, v3d_tfu_yuv_col_space_t *yuv_col_space,
   v3d_tfu_type_t tfu_type, v3d_tfu_rgbord_t rgbord, bool srgb, int v3d_version,
   bool is_sand);

/* Pays attention to format *and layout*
 * Returns false on failure to translate */
extern bool gfx_lfmt_maybe_translate_to_tfu_type(
   v3d_tfu_type_t *tfu_type, bool *srgb, v3d_tfu_rgbord_t *rgbord,
   const GFX_LFMT_T *src_lfmts, uint32_t src_num_planes,
   v3d_tfu_yuv_col_space_t src_yuv_col_space, /* Ignored if src not YUV. TODO Should this be in lfmt? */
   GFX_LFMT_T dst_lfmt, int v3d_version);

extern void gfx_lfmt_translate_to_tfu_type(
   v3d_tfu_type_t *tfu_type, bool *srgb, v3d_tfu_rgbord_t *rgbord,
   const GFX_LFMT_T *src_lfmts, uint32_t src_num_planes,
   v3d_tfu_yuv_col_space_t src_yuv_col_space, /* Ignored if src not YUV */
   GFX_LFMT_T dst_lfmt, int v3d_version);

VCOS_EXTERN_C_END

#endif
