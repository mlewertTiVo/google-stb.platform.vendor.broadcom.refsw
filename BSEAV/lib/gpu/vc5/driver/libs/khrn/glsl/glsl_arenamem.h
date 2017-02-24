/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#pragma once

#include <stddef.h>

#include "glsl_safemem.h"

EXTERN_C_BEGIN

typedef struct _ArenaAlloc ArenaAlloc;

/* This is an arena allocator which provides fast allocations; it is a
   generalisation of the older glsl_fastmem. You must create an arena
   allocator (ArenaAlloc) and use it to obtain memory. Note that this
   is built on the safemem module, and therefore also cannot leak, and
   cannot fail. There is no free and no realloc, only the entire arena
   can be discarded.
*/

#ifndef GLSL_MEMORY_DEBUG

ArenaAlloc *glsl_arena_create(size_t hunk_size);
void       *glsl_arena_malloc(ArenaAlloc *a, size_t size);
void       *glsl_arena_calloc(ArenaAlloc *a, size_t nmemb, size_t size);
char       *glsl_arena_strdup(ArenaAlloc *a, const char *s);

#else

ArenaAlloc *glsl_arena_create_debug(size_t hunk_size, const char *file, int line);
void       *glsl_arena_malloc_debug(ArenaAlloc *a, size_t size, const char *file, int line);
void       *glsl_arena_calloc_debug(ArenaAlloc *a, size_t nmemb, size_t size, const char *file, int line);
char       *glsl_arena_strdup_debug(ArenaAlloc *a, const char *s, const char *file, int line);
void        glsl_arena_print       (const ArenaAlloc *a);

#define glsl_arena_create(hunk_size)      glsl_arena_create_debug((hunk_size),          __FILE__, __LINE__)
#define glsl_arena_malloc(a, size)        glsl_arena_malloc_debug((a), (size),          __FILE__, __LINE__)
#define glsl_arena_calloc(a, nmemb, size) glsl_arena_calloc_debug((a), (nmemb), (size), __FILE__, __LINE__)
#define glsl_arena_strdup(a,s)            glsl_arena_strdup_debug((a), (s),             __FILE__, __LINE__)

#endif

void glsl_arena_free (ArenaAlloc *a);

EXTERN_C_END
