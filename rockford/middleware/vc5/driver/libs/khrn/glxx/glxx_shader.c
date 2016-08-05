/*=============================================================================
Broadcom Proprietary and Confidential. (c)2009 Broadcom.
All rights reserved.

Project  :  khronos
Module   :  Header file

FILE DESCRIPTION
common GL ES 1.1 and 2.0 code for
shaders as dataflow graphs and passing them to the compiler backend.
=============================================================================*/

#include "../glsl/glsl_binary_program.h"
#include "glxx_shader.h"
#include "glxx_server.h"
#include "libs/core/v3d/v3d_shadrec.h"

static GLXX_UNIFORM_MAP_T *format_uniform_map(uint32_t *uniform_map,
                                              size_t    uniform_count)
{
   const unsigned entries_size = sizeof(*uniform_map) * (2*uniform_count);
   GLXX_UNIFORM_MAP_T *our_uniform_map;

   our_uniform_map = malloc(offsetof(GLXX_UNIFORM_MAP_T, entry) + entries_size);
   if (our_uniform_map == NULL) return NULL;

   our_uniform_map->count = uniform_count;
   memcpy(our_uniform_map->entry, uniform_map, entries_size);

   return our_uniform_map;
}

static void write_cv_shader_data(GLXX_LINK_RESULT_DATA_T  *data,
                                 int c_shader_varys,
                                 const BINARY_PROGRAM_T   *prog)
{
   bool c_point_size = prog->vstages[SHADER_VERTEX][MODE_BIN   ]->u.vertex.has_point_size;
   bool v_point_size = prog->vstages[SHADER_VERTEX][MODE_RENDER]->u.vertex.has_point_size;
   assert((c_point_size && v_point_size) || (!c_point_size && !v_point_size));
   bool point_size_included = c_point_size;
   if(point_size_included)
      data->flags |= GLXX_SHADER_FLAGS_POINT_SIZE_SHADED_VERTEX_DATA;

   for (unsigned m = 0; m != MODE_COUNT; ++m)
   {
      unsigned inputs = prog->vstages[SHADER_VERTEX][m]->u.vertex.reads_total;
      unsigned outputs = point_size_included + (m == MODE_BIN ? 6 + c_shader_varys : 4 + data->num_varys);

      // Use separate IO blocks for TNG.
     #if GLXX_HAS_TNG
      if (  prog->vstages[SHADER_TESS_EVALUATION][MODE_RENDER] == NULL
         && prog->vstages[SHADER_GEOMETRY][MODE_RENDER] == NULL   )
     #endif
      {
         outputs = gfx_umax(inputs, outputs);
         inputs = 0;
      }
      data->vs_input_words[m] = inputs;
      data->vs_output_words[m] = outputs;

      if (inputs != 0)
         data->flags |= GLXX_SHADER_FLAGS_VS_SEPARATE_I_O_VPM_BLOCKS << m;
      if (prog->vstages[SHADER_VERTEX][m]->u.vertex.attribs.vertexid_used)
         data->flags |= GLXX_SHADER_FLAGS_VS_READS_VERTEX_ID << m;
      if (prog->vstages[SHADER_VERTEX][m]->u.vertex.attribs.instanceid_used)
         data->flags |= GLXX_SHADER_FLAGS_VS_READS_INSTANCE_ID << m;
#if V3D_HAS_BASEINSTANCE
      if (prog->vstages[SHADER_VERTEX][m]->u.vertex.attribs.baseinstance_used)
         data->flags |= GLXX_SHADER_FLAGS_VS_READS_BASE_INSTANCE << m;
#endif
   }
}

static bool write_common_data(GLXX_SHADER_DATA_T    *data,
                              const BINARY_SHADER_T *bin)
{
   data->res_i       = khrn_res_interlock_from_data(bin->code, bin->code_size,
                                                    V3D_MAX_QPU_INSTRS_READAHEAD,
                                                    8, GMEM_USAGE_CPU_WRITE | GMEM_USAGE_V3D_READ,
                                                    "Shader");
   data->uniform_map = format_uniform_map(bin->unif, bin->unif_count);
   data->threading   = v3d_translate_threading(bin->n_threads);

   return (data->res_i != NULL) && (data->uniform_map != NULL);
}

static bool write_link_result_data(GLXX_LINK_RESULT_DATA_T  *data,
                                   const IR_PROGRAM_T       *ir,
                                   const BINARY_PROGRAM_T   *prog,
                                   bool                      z_prepass)
{
   memset(data, 0, sizeof(*data));

   /* Convert all the common data to the format expected by the driver */
   bool uses_control_flow[MODE_COUNT] = { false,  };
   for (unsigned m = 0; m != MODE_COUNT; ++m)
   {
      // Map from vertex pipe stages to GLSL shader flavours.
      const ShaderFlavour flavours[GLXX_SHADER_VPS_COUNT] =
      {
         SHADER_VERTEX,             // GLXX_SHADER_VPS_VS
        #if GLXX_HAS_TNG
         SHADER_GEOMETRY,           // GLXX_SHADER_VPS_GS
         SHADER_TESS_CONTROL,       // GLXX_SHADER_VPS_TCS
         SHADER_TESS_EVALUATION,    // GLXX_SHADER_VPS_TES
        #endif
      };

      for (unsigned s = 0; s != GLXX_SHADER_VPS_COUNT; ++s)
      {
         BINARY_SHADER_T* bs = prog->vstages[flavours[s]][m];
         if (bs != NULL)
         {
            if (!write_common_data(&data->vps[s][m], bs))
               return false;
            uses_control_flow[m] |= bs->uses_control_flow;
         }
      }
   }

   if (!write_common_data(&data->fs, prog->fshader))
      return false;
   uses_control_flow[MODE_RENDER] |= prog->fshader->uses_control_flow;

   // Bin-mode shaders runs during render-mode if z-prepass is enabled.
   uses_control_flow[MODE_RENDER] |= (uses_control_flow[MODE_BIN] & z_prepass);

   data->bin_uses_control_flow = uses_control_flow[MODE_BIN];
   data->render_uses_control_flow = uses_control_flow[MODE_RENDER];

   assert(prog->vary_map.n <= GLXX_CONFIG_MAX_VARYING_SCALARS);
   data->num_varys = prog->vary_map.n;
   for (unsigned i = 0; i != prog->vary_map.n; ++i)
   {
      data->vary_map[i] = prog->vary_map.entries[i];
   }

   /* Write out the LINK_RESULT_DATA based on the fshader compiler output */
   data->flags = GLXX_SHADER_FLAGS_ENABLE_CLIPPING; /* write_cv_shader_data may add more */

   if (prog->fshader->u.fragment.writes_z)
      data->flags |= GLXX_SHADER_FLAGS_FS_WRITES_Z;

   if (prog->fshader->u.fragment.ez_disable)
      data->flags |= GLXX_SHADER_FLAGS_FS_EARLY_Z_DISABLE;

   if (prog->fshader->u.fragment.tlb_wait_first_thrsw)
      data->flags |= GLXX_SHADER_FLAGS_TLB_WAIT_FIRST_THRSW;

   if (ir->varyings_per_sample || prog->fshader->u.fragment.per_sample)
      data->flags |= GLXX_SHADER_FLAGS_PER_SAMPLE;

   if (prog->vstages[SHADER_VERTEX][MODE_RENDER])
   {
      /* Now do the same for the coordinate and vertex shaders */
      write_cv_shader_data(data, ir->tf_vary_map.n, prog);

      data->attr_count = 0;
      for (int i = 0; i < GLXX_CONFIG_MAX_VERTEX_ATTRIBS; i++) {
         if (prog->vstages[SHADER_VERTEX][MODE_BIN]->u.vertex.attribs.scalars_used[i]    > 0 ||
             prog->vstages[SHADER_VERTEX][MODE_RENDER]->u.vertex.attribs.scalars_used[i] > 0   )
         {
            int a = data->attr_count++;
            data->attr[a].idx = i;
            data->attr[a].c_scalars_used = prog->vstages[SHADER_VERTEX][MODE_BIN]->u.vertex.attribs.scalars_used[i];
            data->attr[a].v_scalars_used = prog->vstages[SHADER_VERTEX][MODE_RENDER]->u.vertex.attribs.scalars_used[i];
         }
      }

      /* Data for bin load balancing, todo T+G */
      data->num_bin_qpu_instructions = prog->vstages[SHADER_VERTEX][MODE_BIN]->code_size/8;
   }

   for (unsigned i = 0; i < data->num_varys; i++) {
      const VARYING_INFO_T *vary = &ir->varying[prog->vary_map.entries[i]];
      if (vary->centroid) {
         data->varying_centroid[i/24] |= 1 << (i % 24);
      }
      if (vary->flat) {
         data->varying_flat[i/24] |= 1 << (i % 24);
      }
   }

   #if GLXX_HAS_TNG
   {
      if (prog->vstages[SHADER_TESS_CONTROL][MODE_RENDER] != NULL)
      {
         assert(prog->vstages[SHADER_TESS_CONTROL][MODE_BIN] != NULL);
         assert(prog->vstages[SHADER_TESS_EVALUATION][MODE_RENDER] != NULL);
         assert(prog->vstages[SHADER_TESS_EVALUATION][MODE_BIN] != NULL);

         data->tcs_output_vertices_per_patch = ir->tess_vertices;

         v3d_cl_tess_type_t tess_type;
         switch (ir->tess_mode)
         {
         case TESS_ISOLINES:  tess_type = V3D_CL_TESS_TYPE_ISOLINES; break;
         case TESS_TRIANGLES: tess_type = V3D_CL_TESS_TYPE_TRIANGLE; break;
         case TESS_QUADS:     tess_type = V3D_CL_TESS_TYPE_QUAD;     break;
         default: unreachable();
         }

         v3d_cl_tess_edge_spacing_t tess_spacing;
         switch (ir->tess_spacing)
         {
         case TESS_SPACING_EQUAL:      tess_spacing = V3D_CL_TESS_EDGE_SPACING_EQUAL;           break;
         case TESS_SPACING_FRACT_EVEN: tess_spacing = V3D_CL_TESS_EDGE_SPACING_FRACTIONAL_EVEN; break;
         case TESS_SPACING_FRACT_ODD:  tess_spacing = V3D_CL_TESS_EDGE_SPACING_FRACTIONAL_ODD;  break;
         default: unreachable();
         }

         data->tess_type = tess_type;
         data->tess_edge_spacing = tess_spacing;
         data->tess_point_mode = ir->tess_point_mode;
         data->tess_clockwise = ir->tess_cw;

         for (unsigned m = 0; m != MODE_COUNT; ++m)
         {
            // todo: actual value rather than maximum
            data->tcs_output_words_per_patch[m] = GLXX_CONFIG_MAX_TESS_PATCH_COMPONENTS;
            data->tcs_output_words[m] = GLXX_CONFIG_MAX_TESS_CONTROL_OUTPUT_COMPONENTS;
            data->tes_output_words[m] = GLXX_CONFIG_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS;
         }
      }

      if (prog->vstages[SHADER_GEOMETRY][MODE_RENDER] != NULL)
      {
         assert(prog->vstages[SHADER_GEOMETRY][MODE_BIN] != NULL);
         switch (ir->gs_out) {
            case GS_OUT_POINTS:     data->geom_prim_type = V3D_CL_GEOM_PRIM_TYPE_POINTS;         break;
            case GS_OUT_LINE_STRIP: data->geom_prim_type = V3D_CL_GEOM_PRIM_TYPE_LINE_STRIP;     break;
            case GS_OUT_TRI_STRIP:  data->geom_prim_type = V3D_CL_GEOM_PRIM_TYPE_TRIANGLE_STRIP; break;
            default: unreachable();
         }

         data->geom_invocations = ir->gs_n_invocations;

         for (unsigned m = 0; m != MODE_COUNT; ++m)
         {
            // todo: actual value rather than maximum
            data->gs_output_words[m] = GLXX_CONFIG_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS;
         }
      }
   }
   #endif

   return true;
}

/* Create shaders for a program's dataflow */
bool glxx_hw_emit_shaders(GLXX_LINK_RESULT_DATA_T      *data,
                          const GLXX_LINK_RESULT_KEY_T *key,
                          IR_PROGRAM_T                 *ir)
{
   BINARY_PROGRAM_T *prog;
   bool success = false;

   /* Hackily randomise the centroid-ness of the varyings */
   for (int i=0; i<V3D_MAX_VARYING_COMPONENTS; i++) {
      if (!ir->varying[i].flat && khrn_options_make_centroid()) ir->varying[i].centroid = true;
   }

   prog = glsl_binary_program_from_dataflow(ir, key);
   if(!prog) {
      goto cleanup;
   }

   if(!write_link_result_data(data, ir, prog, (key->backend & GLXX_Z_ONLY_WRITE))) {
      glxx_hw_cleanup_shaders(data);
      goto cleanup;
   }

   success = true;

 cleanup:
   /* Everything in the binary gets copied away into internal driver datastructures,
      and the binary itself can be freed */
   glsl_binary_program_free(prog);

   return success;
}

static void clear_shader(GLXX_SHADER_DATA_T *data)
{
   khrn_res_interlock_refdec(data->res_i);
   free(data->uniform_map);

   data->res_i       = NULL;
   data->uniform_map = NULL;
}

/* Clean up shaders previously emitted by glxx_hw_emit_shaders */
void glxx_hw_cleanup_shaders(GLXX_LINK_RESULT_DATA_T *data)
{
   for (unsigned m = 0; m != MODE_COUNT; ++m)
   {
      for (unsigned s = 0; s != GLXX_SHADER_VPS_COUNT; ++s)
      {
         clear_shader(&data->vps[s][m]);
      }
   }
   clear_shader(&data->fs);
}
