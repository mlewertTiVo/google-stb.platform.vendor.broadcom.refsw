/*=============================================================================
Copyright (c) 2015 Broadcom Europe Limited.
All rights reserved.
=============================================================================*/
#pragma once

#include "helpers/gfx/gfx_lfmt.h"
#include "helpers/gfx/gfx_buffer_slow_conv.h"
#include "helpers/v3d/v3d_gen.h"
#include "helpers/v3d/v3d_limits.h"

VCOS_EXTERN_C_BEGIN

static inline v3d_tmu_min_filt_t v3d_tmu_mag_filt_to_min_filt(v3d_tmu_mag_filt_t mag_filt)
{
   switch (mag_filt)
   {
   case V3D_TMU_MAG_FILT_LINEAR:    return V3D_TMU_MIN_FILT_LINEAR;
   case V3D_TMU_MAG_FILT_NEAREST:   return V3D_TMU_MIN_FILT_NEAREST;
   default:                         unreachable(); return V3D_TMU_MIN_FILT_INVALID;
   }
}

static inline bool v3d_tmu_min_filt_does_mipmapping(v3d_tmu_min_filt_t min_filt)
{
   return (min_filt != V3D_TMU_MIN_FILT_LINEAR) &&
          (min_filt != V3D_TMU_MIN_FILT_NEAREST);
}

static inline v3d_tmu_min_filt_t v3d_tmu_min_filt_disable_mipmapping(v3d_tmu_min_filt_t min_filt)
{
   switch (min_filt)
   {
   case V3D_TMU_MIN_FILT_LINEAR:
   case V3D_TMU_MIN_FILT_LIN_MIP_NEAR:
   case V3D_TMU_MIN_FILT_LIN_MIP_LIN:
      return V3D_TMU_MIN_FILT_LINEAR;
   case V3D_TMU_MIN_FILT_NEAREST:
   case V3D_TMU_MIN_FILT_NEAR_MIP_NEAR:
   case V3D_TMU_MIN_FILT_NEAR_MIP_LIN:
      return V3D_TMU_MIN_FILT_NEAREST;
   default:
      unreachable();
      return V3D_TMU_MIN_FILT_INVALID;
   }
}

static inline bool v3d_tmu_is_depth_type(v3d_tmu_type_t type)
{
   switch (type)
   {
   case V3D_TMU_TYPE_DEPTH_COMP16:
   case V3D_TMU_TYPE_DEPTH_COMP24:
   case V3D_TMU_TYPE_DEPTH_COMP32F:
   case V3D_TMU_TYPE_DEPTH24_X8:
      return true;
   default:
      return false;
   }
}

typedef enum
{
   V3D_TMU_BLEND_TYPE_FLOAT16,
   V3D_TMU_BLEND_TYPE_UNORM16,
   V3D_TMU_BLEND_TYPE_SNORM16,
   V3D_TMU_BLEND_TYPE_SNORM15, /* For SNORM8 with output_32 */
   V3D_TMU_BLEND_TYPE_INVALID
} v3d_tmu_blend_type_t;

/* V3D_TMU_BLEND_TYPE_INVALID is returned in cases where blending is not
 * possible */
extern v3d_tmu_blend_type_t v3d_maybe_get_tmu_blend_type(
   v3d_tmu_type_t type, bool srgb, bool shadow, bool output_32,
   int v3d_version);

static inline v3d_tmu_blend_type_t v3d_get_tmu_blend_type(
   v3d_tmu_type_t type, bool srgb, bool shadow, bool output_32,
   int v3d_version)
{
   v3d_tmu_blend_type_t blend_type = v3d_maybe_get_tmu_blend_type(
      type, srgb, shadow, output_32, v3d_version);
   assert(blend_type != V3D_TMU_BLEND_TYPE_INVALID);
   return blend_type;
}

static inline bool v3d_tmu_can_blend(
   v3d_tmu_type_t type, bool shadow, bool output_32,
   int v3d_version)
{
   /* sRGBness does not affect blendability */
   return v3d_maybe_get_tmu_blend_type(type, /*srgb=*/false, shadow, output_32, v3d_version)
      != V3D_TMU_BLEND_TYPE_INVALID;
}

extern GFX_LFMT_T v3d_tmu_blend_fmt_from_type(v3d_tmu_blend_type_t blend_type);

/* Returns the format that we should have through the blend pipeline stage.
 * Note that we will return a format for all types, even those that cannot be
 * blended (blend must be disabled for such types, and the blend stage will
 * just pass values through unmodified). */
extern GFX_LFMT_T v3d_get_tmu_blend_fmt(
   v3d_tmu_type_t type, bool srgb, bool shadow, bool output_32,
   int v3d_version);

extern uint32_t v3d_tmu_get_num_channels(v3d_tmu_type_t type);

#if !V3D_HAS_NEW_TMU_CFG

/* Return the output type (true=32, false=16) that is used when
 * !V3D_MISCCFG.ovrtmuout or when (cfg=1 & ltype!=CHILD_IMAGE &
 * output_type=V3D_TMU_OUTPUT_TYPE_AUTO) */
extern bool v3d_tmu_auto_output_32(v3d_tmu_type_t type, bool shadow, bool coefficient);

static inline bool v3d_tmu_output_type_32(v3d_tmu_output_type_t output_type,
   v3d_tmu_type_t type, bool shadow, bool coefficient)
{
   switch (output_type)
   {
   case V3D_TMU_OUTPUT_TYPE_16:     return false;
   case V3D_TMU_OUTPUT_TYPE_32:     return true;
   case V3D_TMU_OUTPUT_TYPE_AUTO:   return v3d_tmu_auto_output_32(type, shadow, coefficient);
   default:                         unreachable();
   }
}

/* Get the number of words returned by the TMU for the specified type in cfg=0
 * mode */
extern uint32_t v3d_tmu_get_word_read_default(
   v3d_tmu_type_t type, bool misccfg_ovrtmuout);

#endif

/* Get the maximum number of words that can be returned by the TMU with the
 * specified configuration */
extern uint32_t v3d_tmu_get_word_read_max(
   v3d_tmu_type_t type, bool coefficient, bool gather,
   bool output_32);

extern bool v3d_tmu_type_supports_srgb(v3d_tmu_type_t type, int v3d_version);

/* Does not consider array size to be a dimension, so eg returns 2 for 2D_ARRAY */
extern uint32_t v3d_tmu_ltype_num_dims(v3d_tmu_ltype_t ltype);

extern bool v3d_tmu_ltype_is_array(v3d_tmu_ltype_t ltype);

struct v3d_tmu_cfg
{
   /* Derived from written registers */
   v3d_tmu_ltype_t ltype;
   bool shadow;
   bool fetch;

   /* From params */
   bool word_en[4];
   bool output_32;
   bool unnorm;
   bool pix_mask;
#if V3D_HAS_NEW_TMU_CFG
   bool tmuoff_per_sample;
#endif
   bool bslod;
   bool coefficient;
   uint32_t coeff_sample;
   bool gather;
   int32_t logical_tex_off_s;
   int32_t logical_tex_off_t;
   int32_t tex_off_r;

   /* From texture state */
   bool logical_flipx;
   bool logical_flipy;
   bool swapst;
   bool srgb; /* Will be false if sampler->srgb_override set */
   bool ahdr;
   v3d_addr_t l0_addr;
   uint32_t arr_str;
   uint32_t logical_width;
   uint32_t logical_height;
   uint32_t logical_depth;
   uint32_t num_array_elems;
   v3d_tmu_type_t type;
   v3d_tmu_swizzle_t swizzles[4];
   uint32_t base_level;
   uint32_t ub_pad;
   bool ub_xor;
   bool uif_top;
   bool xor_dis;

   /* From sampler */
   uint32_t aniso_level; /* Number of samples = 2^aniso_level. 0 means no anisotropic filtering. */
   v3d_tmu_min_filt_t minfilt;
   v3d_tmu_mag_filt_t magfilt;
   v3d_compare_func_t compare_func;
   uint32_t min_lod;
   uint32_t max_lod;
   int32_t fixed_bias;
   v3d_tmu_wrap_t logical_wrap_s;
   v3d_tmu_wrap_t logical_wrap_t;
   v3d_tmu_wrap_t wrap_r;
   GFX_LFMT_BLOCK_T bcolour; /* Border colour. For non-depth types will be in blend format. */

#if !V3D_HAS_NEW_TMU_CFG
   /* Child image */
   uint32_t logical_cwidth;
   uint32_t logical_cheight;
   uint32_t cxoff;
   uint32_t cyoff;

   /* Just indicates whether or not we expect a bias value to be written */
   bool bias;
#endif

   /* Derived from rest of config */
   v3d_addr_t base_addr; /* Offsets in mip_levels are relative to this */
   GFX_BUFFER_DESC_T mip_levels[V3D_MAX_MIP_COUNT];
   /* The range of mip levels the hardware might access. Note that in
    * coefficient mode, the hardware will never actually access any memory, but
    * we still set minlvl/miplvls as if it would. */
   uint32_t minlvl, miplvls;
   /* The format we should have through the blend pipeline stage */
   GFX_LFMT_T blend_fmt;
};

#if V3D_HAS_NEW_TMU_CFG
struct v3d_tmu_reg_flags
{
   bool s, t, r, i, b, dref, off, scm, sfetch, slod;
};

extern void v3d_tmu_cfg_collect(struct v3d_tmu_cfg *cfg,
   const struct v3d_tmu_reg_flags *written,
   const V3D_TMU_PARAM0_T *p0,
   const V3D_TMU_PARAM1_T *p1, /* May be NULL */
   const V3D_TMU_PARAM2_T *p2, /* May be NULL */
   const V3D_TMU_TEX_STATE_T *tex_state,
   const V3D_TMU_TEX_EXTENSION_T *tex_extension, /* May be NULL */
   const V3D_TMU_SAMPLER_T *sampler, /* May be NULL */
   const uint32_t bcolour[4]); /* May be NULL */
#else
extern void v3d_tmu_cfg_collect(struct v3d_tmu_cfg *cfg,
   const V3D_MISCCFG_T *misccfg,
   const V3D_TMU_PARAM0_T *p0,
   const V3D_TMU_PARAM1_CFG0_T *p1_cfg0, const V3D_TMU_PARAM1_CFG1_T *p1_cfg1,
   const V3D_TMU_INDIRECT_T *ind);
#endif

VCOS_EXTERN_C_END
