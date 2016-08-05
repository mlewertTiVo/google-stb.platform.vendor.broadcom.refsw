/*=============================================================================
Broadcom Proprietary and Confidential. (c)2016 Broadcom.
All rights reserved.
=============================================================================*/
/* Auto-generated by running gen_hook_tables.py
 * DO NOT EDIT!
 */
#include <string.h>
#include "libs/core/v3d/v3d_ver.h"
#include <EGL/eglext.h>
#include <EGL/eglext_brcm.h>
#define EGL_DISPLAY_EXTS_STR_MAX_SIZE 452
static inline char *egl_display_exts_str(char *s_in)
{
   char *s = s_in;
#if EGL_BRCM_event_monitor
   memcpy(s, "EGL_BRCM_event_monitor", 22);
   s += 22;
   *(s++) = ' ';
#endif
#if EGL_BRCM_gl_framebuffer_image
   memcpy(s, "EGL_BRCM_gl_framebuffer_image", 29);
   s += 29;
   *(s++) = ' ';
#endif
#if EGL_BRCM_performance_counters
   memcpy(s, "EGL_BRCM_performance_counters", 29);
   s += 29;
   *(s++) = ' ';
#endif
#if EGL_EXT_create_context_robustness
   memcpy(s, "EGL_EXT_create_context_robustness", 33);
   s += 33;
   *(s++) = ' ';
#endif
#if EGL_EXT_protected_content
   memcpy(s, "EGL_EXT_protected_content", 25);
   s += 25;
   *(s++) = ' ';
#endif
#if EGL_EXT_protected_surface
   memcpy(s, "EGL_EXT_protected_surface", 25);
   s += 25;
   *(s++) = ' ';
#endif
   memcpy(s, "EGL_KHR_cl_event2", 17);
   s += 17;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_create_context", 22);
   s += 22;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_fence_sync", 18);
   s += 18;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_get_all_proc_addresses", 30);
   s += 30;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_gl_colorspace", 21);
   s += 21;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_gl_renderbuffer_image", 29);
   s += 29;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_gl_texture_2D_image", 27);
   s += 27;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_gl_texture_cubemap_image", 32);
   s += 32;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_image", 13);
   s += 13;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_image_base", 18);
   s += 18;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_surfaceless_context", 27);
   s += 27;
   *(s++) = ' ';
   memcpy(s, "EGL_KHR_wait_sync", 17);
   s += 17;
   *(s++) = ' ';
   if (s != s_in)
      --s;
   *s = '\0';
   return s;
}
#define EGL_DISPLAY_MAX_EXTS 18
static inline unsigned egl_display_exts(const char **e_in)
{
   const char **e = e_in;
#if EGL_BRCM_event_monitor
   *(e++) = "EGL_BRCM_event_monitor";
#endif
#if EGL_BRCM_gl_framebuffer_image
   *(e++) = "EGL_BRCM_gl_framebuffer_image";
#endif
#if EGL_BRCM_performance_counters
   *(e++) = "EGL_BRCM_performance_counters";
#endif
#if EGL_EXT_create_context_robustness
   *(e++) = "EGL_EXT_create_context_robustness";
#endif
#if EGL_EXT_protected_content
   *(e++) = "EGL_EXT_protected_content";
#endif
#if EGL_EXT_protected_surface
   *(e++) = "EGL_EXT_protected_surface";
#endif
   *(e++) = "EGL_KHR_cl_event2";
   *(e++) = "EGL_KHR_create_context";
   *(e++) = "EGL_KHR_fence_sync";
   *(e++) = "EGL_KHR_get_all_proc_addresses";
   *(e++) = "EGL_KHR_gl_colorspace";
   *(e++) = "EGL_KHR_gl_renderbuffer_image";
   *(e++) = "EGL_KHR_gl_texture_2D_image";
   *(e++) = "EGL_KHR_gl_texture_cubemap_image";
   *(e++) = "EGL_KHR_image";
   *(e++) = "EGL_KHR_image_base";
   *(e++) = "EGL_KHR_surfaceless_context";
   *(e++) = "EGL_KHR_wait_sync";
   return e - e_in;
}
