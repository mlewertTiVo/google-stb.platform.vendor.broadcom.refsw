//#define DRAW_TEX_LOGGING
/*=============================================================================
Copyright (c) 2008 Broadcom Europe Limited.
All rights reserved.

Project  :  khronos
Module   :  Header file

FILE DESCRIPTION
Hardware abstraction layer API declaration.
Functions common to OpenGL ES 1.1 and OpenGL ES 2.0
=============================================================================*/
#ifndef GLXX_HW_H
#define GLXX_HW_H

#include "middleware/khronos/glxx/glxx_server.h"


typedef struct
{
   MEM_HANDLE_T mh_vcode;
   MEM_HANDLE_T mh_ccode;
   MEM_HANDLE_T mh_fcode;
   MEM_HANDLE_T mh_vuniform_map;
   MEM_HANDLE_T mh_cuniform_map;
   MEM_HANDLE_T mh_funiform_map;
   uint32_t num_varyings;
   bool threaded;
   bool has_point_size;
#ifdef XXX_OFFLINE
   bool use_offline;
#endif
   uint32_t cattribs_order[GLXX_CONFIG_MAX_VERTEX_ATTRIBS*2];
   uint32_t vattribs_order[GLXX_CONFIG_MAX_VERTEX_ATTRIBS*2];
} GLXX_LINK_RESULT_DATA_T;

static INLINE GLXX_HW_BLEND_T glxx_hw_blend(
   bool blend,
   GLenum equation, GLenum equation_alpha,
   GLenum src_function, GLenum src_function_alpha,
   GLenum dst_function, GLenum dst_function_alpha,
   GLenum sample_alpha_to_coverage,
   GLenum sample_coverage,
   GLXX_SAMPLE_COVERAGE_T sample_coverage_v,
   bool ms, GLenum logic_op, bool *color_mask)
{
   GLXX_HW_BLEND_T result;

   result.equation           = equation;
   result.equation_alpha     = equation_alpha;
   result.src_function       = src_function;
   result.src_function_alpha = src_function_alpha;
   result.dst_function       = dst_function;
   result.dst_function_alpha = dst_function_alpha;
   result.sample_alpha_to_coverage = sample_alpha_to_coverage;
   result.sample_coverage    = sample_coverage;
   result.sample_coverage_v  = sample_coverage_v;
   result.ms                 = ms;
   result.logic_op           = logic_op;
   result.color_mask	= 0;

   if (!blend)
   {
      result.equation = GL_FUNC_ADD;
      result.equation_alpha = GL_FUNC_ADD;
      result.src_function = GL_ONE;
      result.src_function_alpha = GL_ONE;
      result.dst_function = GL_ZERO;
      result.dst_function_alpha = GL_ZERO;
   }

   if(!result.sample_coverage)
   {
      result.sample_coverage_v.value = 0.0f;
      result.sample_coverage_v.invert = GL_FALSE;
   }

   if (color_mask[0]) result.color_mask |= 0x000000ff;
   if (color_mask[1]) result.color_mask |= 0x0000ff00;
   if (color_mask[2]) result.color_mask |= 0x00ff0000;
   if (color_mask[3]) result.color_mask |= 0xff000000;
   if (!blend && logic_op == 0 && result.color_mask == 0xffffffff
      && !result.sample_alpha_to_coverage &&!result.sample_coverage) result.ms = false;
   return result;
}
static INLINE bool glxx_hw_blend_enabled(GLXX_HW_BLEND_T blend)
{
   return
      blend.equation != GL_FUNC_ADD || blend.equation_alpha != GL_FUNC_ADD ||
      blend.src_function != GL_ONE || blend.src_function_alpha != GL_ONE ||
      blend.dst_function != GL_ZERO || blend.dst_function_alpha != GL_ZERO;
}

extern bool glxx_hw_clear(bool color, bool depth, bool stencil, GLXX_SERVER_STATE_T *state);
extern void glxx_hw_finish(void);
extern void glxx_hw_finish_context(GLXX_SERVER_STATE_T *state, bool wait);
extern void glxx_hw_term(void);
extern bool glxx_hw_draw_triangles(GLsizei count, GLenum type, uint32_t indices_offset,
                                   GLXX_SERVER_STATE_T *state,
                                   GLXX_ATTRIB_T *attrib,
                                   MEM_HANDLE_T indices_handle,
                                   MEM_HANDLE_T *attrib_handles,
                                   uint32_t max_index,
                                   MEM_HANDLE_OFFSET_T *interlocks,
                                   uint32_t interlock_count);
extern void glxx_hw_invalidate_frame(GLXX_SERVER_STATE_T *state, bool color, bool depth, bool stencil, bool multisample,
                                     bool preserveBuf, bool main_buffer);
extern bool glxx_hw_get_attr_live(GLXX_SERVER_STATE_T *state, GLenum primitive_mode, GLXX_ATTRIB_T *attrib);
extern bool glxx_hw_draw_tex(GLXX_SERVER_STATE_T *state, float Xs, float Ys, float Zw, float Ws, float Hs);

extern bool glxx_schedule_during_link(GLXX_SERVER_STATE_T *state, void *prog);

#endif
