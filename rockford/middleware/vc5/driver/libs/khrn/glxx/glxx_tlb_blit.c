/*=============================================================================
Broadcom Proprietary and Confidential. (c)2016 Broadcom.
All rights reserved.
=============================================================================*/

#include "glxx_tlb_blit.h"
#include "glxx_hw.h"
#include "glxx_framebuffer.h"

#if !V3D_HAS_NEW_TLB_CFG
#include "libs/core/lfmt/lfmt_translate_v3d.h"
#endif

#if !V3D_HAS_NEW_TLB_CFG
static bool store_general_compatible(GFX_LFMT_T lfmt)
{
   return !gfx_lfmt_get_yflip(&lfmt) && (gfx_lfmt_is_uif(lfmt) || gfx_lfmt_is_uif_xor(lfmt));
}
#endif

/* If can do blit with TLB, src_hw_fb & blit inited and true returned.
 * Otherwise, false returned. */
static bool try_init_blit(
   GLXX_HW_FRAMEBUFFER_T *src_hw_fb, GLXX_BLIT_T *blit,
   const GLXX_SERVER_STATE_T *state,
   GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
   GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
   GLbitfield mask)
{
   const GLXX_FRAMEBUFFER_T *src_fb = state->bound_read_framebuffer;
   if (!glxx_init_hw_framebuffer(src_fb, src_hw_fb))
      return false;

   const GLXX_FRAMEBUFFER_T *dst_fb = state->bound_draw_framebuffer;
   if (!glxx_init_hw_framebuffer(dst_fb, &blit->dst_fb))
   {
      glxx_destroy_hw_framebuffer(src_hw_fb);
      return false;
   }

   /* glBlitFramebuffer() does not support MS dest. Should not get this far if
    * dest FB is MS... */
   assert(!blit->dst_fb.ms);

   /* Can only handle full frame to full frame blit with no flipping & no
    * scaling (we *do* support multisample decimation though) */
   if ((srcX0 != dstX0) || (srcX1 != dstX1) ||
      (srcY0 != dstY0) || (srcY1 != dstY1) ||
      (src_hw_fb->width != blit->dst_fb.width) || (src_hw_fb->height != blit->dst_fb.height) ||
      (gfx_smin(srcX0, srcX1) != 0) ||
      (gfx_smax(srcX0, srcX1) != (int)src_hw_fb->width) ||
      (gfx_smin(srcY0, srcY1) != 0) ||
      (gfx_smax(srcY0, srcY1) != (int)src_hw_fb->height))
      goto not_supported;

   /* Scissoring not supported */
   if (state->caps.scissor_test &&
      (state->scissor.x != 0 || state->scissor.y != 0 ||
      state->scissor.width != blit->dst_fb.width ||
      state->scissor.height != blit->dst_fb.height))
      goto not_supported;

   blit->color = !!(mask & GL_COLOR_BUFFER_BIT);
   if (blit->color)
   {
      blit->color_read_buffer = src_fb->read_buffer - GLXX_COLOR0_ATT;
      blit->color_draw_to_buf = glxx_fb_get_valid_draw_buf_mask(dst_fb);

#if !V3D_HAS_NEW_TLB_CFG
      GFX_LFMT_T src_nonms_lfmt = khrn_image_plane_lfmt_maybe(
         &src_hw_fb->color[blit->color_read_buffer]);
      v3d_pixel_format_t pixel_format = src_nonms_lfmt ?
         gfx_lfmt_translate_pixel_format(src_nonms_lfmt) : V3D_PIXEL_FORMAT_INVALID;
#endif

      for (uint32_t b = 0, mask = blit->color_draw_to_buf; mask; ++b, mask >>= 1)
      {
         if (mask & 1)
         {
            /* Destination format must be compatible with internal TLB type */
            if (blit->dst_fb.color_internal_type[b] != src_hw_fb->color_internal_type[blit->color_read_buffer])
               goto not_supported;

#if !V3D_HAS_NEW_TLB_CFG
            GFX_LFMT_T dst_lfmt = khrn_image_plane_lfmt(&blit->dst_fb.color[b]);
            v3d_pixel_format_t dst_pixel_format = gfx_lfmt_translate_pixel_format(dst_lfmt);

            /* We always use STORE_GENERAL instructions to do TLB blits.
             * STORE_GENERAL isn't actually fully general... */

            /* Only UIF & UIF XOR swizzlings are supported.
             * Y-flip is not supported. */
            if (!store_general_compatible(dst_lfmt))
               goto not_supported;

            /* Output format is not explicitly provided; the output format
             * specified in the rendering mode config for the specified buffer
             * is used.
             *
             * In the case where the source framebuffer has no downsampled
             * buffer, the output format in the rendering mode config is not
             * used for normal loads/stores so can be used solely for blits.
             * The blits still have to agree though; this is checked here
             * (agreement within a single GLXX_BLIT_T) and in blit_compatible()
             * (agreement between GLXX_BLIT_Ts). */
            if (pixel_format == V3D_PIXEL_FORMAT_INVALID)
               pixel_format = dst_pixel_format;
            else if (!v3d_pixel_format_equiv_for_store(dst_pixel_format, pixel_format))
               goto not_supported;

            /* Decimate mode is not explicitly provided; it comes from the
             * rendering mode config for the specified buffer.
             * glBlitFramebuffer() only supports non-MS destination. When in
             * multisample mode the color decimate mode in the rendering mode
             * config is always set to 4X, which is what we want for blits, so
             * no need for any extra restrictions here... */
#endif
         }
      }
   }

   blit->depth = !!(mask & GL_DEPTH_BUFFER_BIT);
#if !V3D_HAS_NEW_TLB_CFG
   if (blit->depth)
   {
      GFX_LFMT_T src_lfmt = khrn_image_plane_lfmt(&src_hw_fb->depth);
      GFX_LFMT_T dst_lfmt = khrn_image_plane_lfmt(&blit->dst_fb.depth);

      /* We always use STORE_GENERAL instructions to do TLB blits.
       * STORE_GENERAL isn't actually fully general... */

      /* Only UIF & UIF XOR swizzlings are supported.
       * Y-flip is not supported. */
      if (!store_general_compatible(dst_lfmt))
         goto not_supported;

      /* Output format is not explicitly provided; the output format specified
       * in the rendering mode config for the specified buffer is used */
      if (gfx_lfmt_translate_depth_format(dst_lfmt) != gfx_lfmt_translate_depth_format(src_lfmt))
         goto not_supported;

      /* Decimate mode is not explicitly provided; it comes from the rendering
       * mode config for the specified buffer. For MS --> non-MS blit we would
       * want SAMPLE0 decimate mode, but ALL_SAMPLES is needed for the normal
       * depth load/store. So we can't support MS --> non-MS blit of depth... */
      if (src_hw_fb->ms)
         goto not_supported;
   }
#endif

   // TODO Could probably support this
   if (mask & GL_STENCIL_BUFFER_BIT)
      goto not_supported;

   return true;

not_supported:
   glxx_destroy_hw_framebuffer(&blit->dst_fb);
   glxx_destroy_hw_framebuffer(src_hw_fb);
   return false;
}

#if !V3D_HAS_NEW_TLB_CFG
static v3d_pixel_format_t blit_pixel_format(const GLXX_BLIT_T *blit)
{
   /* try_init_blit() checks all dest color buffers have equivalent pixel
    * formats, just pick one... */
   unsigned b = gfx_msb(blit->color_draw_to_buf);
   assert(b < blit->dst_fb.rt_count);
   return gfx_lfmt_translate_pixel_format(khrn_image_plane_lfmt(&blit->dst_fb.color[b]));
}

static bool blit_compatible(const GLXX_HW_RENDER_STATE_T *rs, const GLXX_BLIT_T *blit)
{
   if (!blit->color || !blit->color_draw_to_buf ||
      /* If there is a downsampled source buffer we will already have checked
       * all blits against that (see try_init_blit) */
      rs->installed_fb.color[blit->color_read_buffer].image)
      return true;

   /* Find the first blit in rs with the same color_read_buffer (all blits in
    * rs with the same color_read_buffer should be compatible with each other
    * and compatibility is transitive, so we only need to check against one of
    * them) */
   unsigned i;
   const GLXX_BLIT_T *rs_blit = NULL;
   for (i = 0; i != rs->num_blits; ++i)
   {
      rs_blit = &rs->tlb_blits[i];
      if (rs_blit->color && rs_blit->color_draw_to_buf &&
         (rs_blit->color_read_buffer == blit->color_read_buffer))
         break;
   }
   if (i == rs->num_blits)
      return true; /* No blits in rs with same color_read_buffer */

   /* Check the required pixel formats are equivalent */
   return v3d_pixel_format_equiv_for_store(
      blit_pixel_format(blit), blit_pixel_format(rs_blit));
}
#endif

static bool record_interlock_writes(GLXX_HW_RENDER_STATE_T *rs, const GLXX_BLIT_T *blit, bool *requires_flush)
{
   if (blit->color)
   {
      for (uint32_t b = 0, mask = blit->color_draw_to_buf; mask; ++b, mask >>= 1)
      {
         if (mask & 1)
         {
            KHRN_RES_INTERLOCK_T *res_i = khrn_image_get_res_interlock(blit->dst_fb.color[b].image);
            if (!khrn_fmem_record_res_interlock_self_read_conflicting_write(
                  &rs->fmem, res_i, ACTION_RENDER, requires_flush))
               return false;
         }
      }
   }

   if (blit->depth)
   {
      KHRN_RES_INTERLOCK_T *res_i = khrn_image_get_res_interlock(blit->dst_fb.depth.image);
      if (!khrn_fmem_record_res_interlock_self_read_conflicting_write(
            &rs->fmem, res_i, ACTION_RENDER, requires_flush))
         return false;
   }

   return true;
}

static bool add_blit_to_rs(GLXX_SERVER_STATE_T *state,
   const GLXX_HW_FRAMEBUFFER_T *src_hw_fb, const GLXX_BLIT_T *blit)
{
   for (;;)
   {
      GLXX_HW_RENDER_STATE_T *rs = glxx_install_rs(state, src_hw_fb, /*for_tlb_blit=*/true);
      if (!rs)
         return false;

#if V3D_HAS_NEW_TLB_CFG
      if (rs->num_blits == GLXX_RS_MAX_BLITS)
#else
      if ((rs->num_blits == GLXX_RS_MAX_BLITS) || !blit_compatible(rs, blit))
#endif
      {
         glxx_hw_render_state_flush(rs);
         continue;
      }

      bool requires_flush;
      if (!record_interlock_writes(rs, blit, &requires_flush))
      {
         if (requires_flush)
         {
            glxx_hw_render_state_flush(rs);
            continue;
         }
         else
            return false;
      }

      if (blit->color)
         glxx_bufstate_read(&rs->color_buffer_state[blit->color_read_buffer]);
      if (blit->depth)
         glxx_bufstate_read(&rs->depth_buffer_state);

      assert(rs->num_blits < GLXX_RS_MAX_BLITS);
      rs->tlb_blits[rs->num_blits++] = *blit;
      return true;
   }
}

bool glxx_try_tlb_blit_framebuffer(GLXX_SERVER_STATE_T *state,
   GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
   GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
   GLbitfield mask)
{
   GLXX_HW_FRAMEBUFFER_T src_hw_fb;
   GLXX_BLIT_T blit;
   if (!try_init_blit(&src_hw_fb, &blit,
      state, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask))
      return false;

   if (!add_blit_to_rs(state, &src_hw_fb, &blit))
   {
      glxx_server_state_set_error(state, GL_OUT_OF_MEMORY);
      glxx_destroy_hw_framebuffer(&blit.dst_fb);
   }
   glxx_destroy_hw_framebuffer(&src_hw_fb);
   return true;
}
