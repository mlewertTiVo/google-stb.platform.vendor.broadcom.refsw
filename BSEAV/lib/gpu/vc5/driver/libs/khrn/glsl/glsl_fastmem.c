/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include "glsl_common.h"
#include "glsl_fastmem.h"

#define DEFAULT_ALLOC_SIZE (64*1024)

ArenaAlloc *fastmem_arena = NULL;

void glsl_fastmem_init() {
   fastmem_arena = glsl_arena_create(DEFAULT_ALLOC_SIZE);
}

void glsl_fastmem_term() {
   glsl_arena_free(fastmem_arena);
   fastmem_arena = NULL;
}
