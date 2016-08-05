/*=============================================================================
Broadcom Proprietary and Confidential. (c)2016 Broadcom.
All rights reserved.
=============================================================================*/
/* Auto-generated by running gen_hook_tables.py
 * DO NOT EDIT!
 */
#include <string.h>
#include "libs/core/v3d/v3d_ver.h"
#include "../glxx/gl_public_api.h"
#include "../glxx/glxx_int_config.h"
#include "../common/khrn_process.h"
#define GL11_EXTS_STR_MAX_SIZE 620
static inline char *gl11_exts_str(char *s_in)
{
   char *s = s_in;
   memcpy(s, "GL_EXT_debug_marker", 19);
   s += 19;
   *(s++) = ' ';
   memcpy(s, "GL_EXT_discard_framebuffer", 26);
   s += 26;
   *(s++) = ' ';
   memcpy(s, "GL_EXT_multisampled_render_to_texture", 37);
   s += 37;
   *(s++) = ' ';
   memcpy(s, "GL_EXT_robustness", 17);
   s += 17;
   *(s++) = ' ';
#if GL_EXT_texture_filter_anisotropic
   memcpy(s, "GL_EXT_texture_filter_anisotropic", 33);
   s += 33;
   *(s++) = ' ';
#endif
   memcpy(s, "GL_EXT_texture_format_BGRA8888", 30);
   s += 30;
   *(s++) = ' ';
   memcpy(s, "GL_KHR_debug", 12);
   s += 12;
   *(s++) = ' ';
   memcpy(s, "GL_OES_EGL_image", 16);
   s += 16;
   *(s++) = ' ';
   memcpy(s, "GL_OES_EGL_image_external", 25);
   s += 25;
   *(s++) = ' ';
   memcpy(s, "GL_OES_EGL_sync", 15);
   s += 15;
   *(s++) = ' ';
   memcpy(s, "GL_OES_compressed_ETC1_RGB8_texture", 35);
   s += 35;
   *(s++) = ' ';
   memcpy(s, "GL_OES_compressed_paletted_texture", 34);
   s += 34;
   *(s++) = ' ';
   memcpy(s, "GL_OES_depth24", 14);
   s += 14;
   *(s++) = ' ';
   memcpy(s, "GL_OES_draw_texture", 19);
   s += 19;
   *(s++) = ' ';
   memcpy(s, "GL_OES_framebuffer_object", 25);
   s += 25;
   *(s++) = ' ';
   memcpy(s, "GL_OES_mapbuffer", 16);
   s += 16;
   *(s++) = ' ';
   memcpy(s, "GL_OES_packed_depth_stencil", 27);
   s += 27;
   *(s++) = ' ';
   memcpy(s, "GL_OES_point_size_array", 23);
   s += 23;
   *(s++) = ' ';
   memcpy(s, "GL_OES_query_matrix", 19);
   s += 19;
   *(s++) = ' ';
   memcpy(s, "GL_OES_rgb8_rgba8", 17);
   s += 17;
   *(s++) = ' ';
   memcpy(s, "GL_OES_stencil8", 15);
   s += 15;
   *(s++) = ' ';
   memcpy(s, "GL_OES_stencil_wrap", 19);
   s += 19;
   *(s++) = ' ';
   memcpy(s, "GL_OES_surfaceless_context", 26);
   s += 26;
   *(s++) = ' ';
   memcpy(s, "GL_OES_texture_mirrored_repeat", 30);
   s += 30;
   *(s++) = ' ';
   memcpy(s, "GL_OES_texture_npot", 19);
   s += 19;
   *(s++) = ' ';
   memcpy(s, "GL_OES_vertex_array_object", 26);
   s += 26;
   *(s++) = ' ';
   if (s != s_in)
      --s;
   *s = '\0';
   return s;
}
#define GL11_MAX_EXTS 26
static inline unsigned gl11_exts(const char **e_in)
{
   const char **e = e_in;
   *(e++) = "GL_EXT_debug_marker";
   *(e++) = "GL_EXT_discard_framebuffer";
   *(e++) = "GL_EXT_multisampled_render_to_texture";
   *(e++) = "GL_EXT_robustness";
#if GL_EXT_texture_filter_anisotropic
   *(e++) = "GL_EXT_texture_filter_anisotropic";
#endif
   *(e++) = "GL_EXT_texture_format_BGRA8888";
   *(e++) = "GL_KHR_debug";
   *(e++) = "GL_OES_EGL_image";
   *(e++) = "GL_OES_EGL_image_external";
   *(e++) = "GL_OES_EGL_sync";
   *(e++) = "GL_OES_compressed_ETC1_RGB8_texture";
   *(e++) = "GL_OES_compressed_paletted_texture";
   *(e++) = "GL_OES_depth24";
   *(e++) = "GL_OES_draw_texture";
   *(e++) = "GL_OES_framebuffer_object";
   *(e++) = "GL_OES_mapbuffer";
   *(e++) = "GL_OES_packed_depth_stencil";
   *(e++) = "GL_OES_point_size_array";
   *(e++) = "GL_OES_query_matrix";
   *(e++) = "GL_OES_rgb8_rgba8";
   *(e++) = "GL_OES_stencil8";
   *(e++) = "GL_OES_stencil_wrap";
   *(e++) = "GL_OES_surfaceless_context";
   *(e++) = "GL_OES_texture_mirrored_repeat";
   *(e++) = "GL_OES_texture_npot";
   *(e++) = "GL_OES_vertex_array_object";
   return e - e_in;
}
