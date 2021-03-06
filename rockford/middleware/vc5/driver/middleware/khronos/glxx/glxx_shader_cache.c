/*=============================================================================
Copyright (c) 2010 Broadcom Europe Limited.
All rights reserved.

Project  :  khronos
Module   :  Header file

FILE DESCRIPTION
Creates GLES2.0 shaders as dataflow graphs and passes them to the compiler
backend.
=============================================================================*/

#include "middleware/khronos/glsl/glsl_ir_program.h"
#include "middleware/khronos/glxx/glxx_shader.h"
#include "middleware/khronos/glxx/glxx_hw.h"


void glxx_binary_cache_invalidate(GLXX_BINARY_CACHE_T *cache)
{
   for (unsigned i = 0; i < GLXX_BINARY_CACHE_SIZE; i++) {
      if (cache->entry[i].used) {
         glxx_hw_cleanup_shaders(&cache->entry[i].data);
         cache->entry[i].used = false;
      }
   }
}

GLXX_LINK_RESULT_DATA_T *glxx_binary_cache_get_shaders(GLXX_BINARY_CACHE_T *cache,
                                                       GLXX_LINK_RESULT_KEY_T *key)
{
   for (unsigned i = 0; i < cache->used; i++) {
      if (cache->entry[i].used && !memcmp(&cache->entry[i].key, key, sizeof(GLXX_LINK_RESULT_KEY_T)))
         return &cache->entry[i].data;
   }

   return NULL;
}

GLXX_LINK_RESULT_DATA_T *glxx_get_shaders_and_cache(
   GLXX_BINARY_CACHE_T *cache,
   IR_PROGRAM_T *ir,
   GLXX_LINK_RESULT_KEY_T *key)
{
   /* Compile new version of this shader and add it to the cache. */
   int i = cache->next;
   cache->next = (i + 1) % GLXX_BINARY_CACHE_SIZE;
   if (cache->used < GLXX_BINARY_CACHE_SIZE) cache->used++;

   GLXX_BINARY_CACHE_ENTRY_T *entry = &cache->entry[i];
   if (entry->used) glxx_hw_cleanup_shaders(&entry->data);

   entry->key = *key;
   entry->used = true;

   if (!glxx_hw_emit_shaders(&entry->data, &entry->key, ir)) {
      entry->used = false;
      return NULL;
   }

   return &cache->entry[i].data;
}
