/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#pragma once

#include "glsl_symbols.h"
#include "glsl_map.h"

EXTERN_C_BEGIN

typedef struct _ShaderInterfaces {
   SymbolList *uniforms;
   SymbolList *ins;
   SymbolList *outs;
   SymbolList *buffers;
   SymbolList *shared;
} ShaderInterfaces;

ShaderInterfaces *glsl_shader_interfaces_new(void);
void glsl_shader_interfaces_update(ShaderInterfaces *shader_interfaces, Symbol *symbol);

Dataflow **glsl_shader_interface_create_uniform_dataflow(const Symbol *s, int *ids);
Dataflow **glsl_shader_interface_create_buffer_dataflow(const Symbol *s, int *ids);

Map *glsl_shader_interfaces_id_map(const ShaderInterfaces *interfaces);
Map *glsl_shader_interfaces_create_dataflow(const ShaderInterfaces *interfaces,
                                            Map *symbol_ids);

EXTERN_C_END
