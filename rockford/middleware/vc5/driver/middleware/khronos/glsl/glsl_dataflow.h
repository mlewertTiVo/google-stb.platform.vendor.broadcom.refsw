/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.

Project  :  glsl
Module   :

FILE DESCRIPTION
=============================================================================*/

#ifndef GLSL_DATAFLOW_H
#define GLSL_DATAFLOW_H

#include "glsl_const_types.h"
#include "glsl_fastmem.h"
#include "glsl_list.h"

typedef struct _Dataflow Dataflow;

//
// Dataflow graph where all operations act on scalars.
//

#define SAMPLER_LOCATION_UNDEFINED        (~0u)
#define LINKABLE_VALUE_ROW_UNDEFINED      (~0u)

void glsl_dataflow_reset_count(void);
int glsl_dataflow_get_count(void);
int glsl_dataflow_get_next_id(void);

// Set up age which will be applied to dataflow. This might not be a good thing.
void glsl_dataflow_reset_age(void);
void glsl_dataflow_set_age(int age);


void glsl_dataflow_begin_construction();
void glsl_dataflow_end_construction();
void *glsl_dataflow_malloc(size_t);

// Dataflow chain structure, for efficient appending.
DECLARE_NODE_LIST(Dataflow,glsl_dataflow_malloc)

typedef bool (*DataflowFilter)(Dataflow* dataflow, void* data);

void glsl_dataflow_chain_init(DataflowChain* chain);
DataflowChain* glsl_dataflow_chain_append(DataflowChain* chain, Dataflow* dataflow);
DataflowChain* glsl_dataflow_chain_remove(DataflowChain *chain, Dataflow *dataflow);
DataflowChain* glsl_dataflow_chain_replace(DataflowChain *chain, Dataflow *dataflow_old, Dataflow *dataflow_new);
bool glsl_dataflow_chain_contains(DataflowChain *chain, Dataflow *dataflow);
// If node in chain, removes node from chain.
// Else, behaviour undefined.
DataflowChain* glsl_dataflow_chain_remove_node(DataflowChain* chain, DataflowChainNode* node);
DataflowChain *glsl_dataflow_chain_filter(DataflowChain *dst, DataflowChain *src, void *data, DataflowFilter filter);

typedef enum
{
   DATAFLOW_CONST,

   DATAFLOW_LOAD,
   DATAFLOW_PHI,
   DATAFLOW_EXTERNAL,

   DATAFLOW_LOGICAL_NOT,
   DATAFLOW_CONST_SAMPLER,
   DATAFLOW_FTOI_TRUNC,
   DATAFLOW_FTOI_NEAREST,
   DATAFLOW_FTOU,

   DATAFLOW_BITWISE_NOT,
   DATAFLOW_BITWISE_AND,
   DATAFLOW_BITWISE_OR,
   DATAFLOW_BITWISE_XOR,

   DATAFLOW_SHL,
   DATAFLOW_SHR,

   DATAFLOW_ADDRESS_LOAD,
   DATAFLOW_VECTOR_LOAD,
   DATAFLOW_UNIFORM,
   DATAFLOW_UNIFORM_BUFFER,
   DATAFLOW_STORAGE_BUFFER,
   DATAFLOW_ATOMIC_COUNTER,
   DATAFLOW_IN,
   DATAFLOW_MUL,
   DATAFLOW_DIV,
   DATAFLOW_REM,
   DATAFLOW_ADD,
   DATAFLOW_SUB,
   DATAFLOW_ARITH_NEGATE,
   DATAFLOW_LESS_THAN,
   DATAFLOW_LESS_THAN_EQUAL,
   DATAFLOW_GREATER_THAN,
   DATAFLOW_GREATER_THAN_EQUAL,
   DATAFLOW_EQUAL,
   DATAFLOW_NOT_EQUAL,
   DATAFLOW_LOGICAL_AND,
   DATAFLOW_LOGICAL_XOR,
   DATAFLOW_LOGICAL_OR,
   DATAFLOW_CONDITIONAL,

   // Intrinsic, hardware supported operators.
   DATAFLOW_RSQRT,
   DATAFLOW_RCP,
   DATAFLOW_LOG2,
   DATAFLOW_EXP2,
   DATAFLOW_SIN,
   DATAFLOW_COS,
   DATAFLOW_TAN,
   DATAFLOW_MIN,
   DATAFLOW_MAX,
   DATAFLOW_TRUNC,
   DATAFLOW_NEAREST,
   DATAFLOW_CEIL,
   DATAFLOW_FLOOR,
   DATAFLOW_ABS,
   DATAFLOW_FDX,
   DATAFLOW_FDY,
   DATAFLOW_REINTERP,
   DATAFLOW_FPACK,
   DATAFLOW_FUNPACKA,
   DATAFLOW_FUNPACKB,

   DATAFLOW_ITOF,
   DATAFLOW_UTOF,
   DATAFLOW_CLZ,

   // IR texture gadget
   DATAFLOW_VEC4,
   DATAFLOW_TEXTURE,
   DATAFLOW_TEXTURE_SIZE,
   DATAFLOW_GET_VEC4_COMPONENT,

   // Fragment colour get (used with GET_VEC4_COMPONENT)
   DATAFLOW_FRAG_GET_COL,

   // Fragment coordinate retrieval.
   DATAFLOW_FRAG_GET_X,
   DATAFLOW_FRAG_GET_Y,
   DATAFLOW_FRAG_GET_X_UINT,
   DATAFLOW_FRAG_GET_Y_UINT,
   DATAFLOW_FRAG_GET_Z,
   DATAFLOW_FRAG_GET_W,
   DATAFLOW_FRAG_GET_FF,

   // TIDX
   DATAFLOW_GET_THREAD_INDEX,

   DATAFLOW_IS_HELPER,

   DATAFLOW_GET_VERTEX_ID,
   DATAFLOW_GET_INSTANCE_ID,
   DATAFLOW_GET_POINT_COORD_X,
   DATAFLOW_GET_POINT_COORD_Y,
   DATAFLOW_GET_LINE_COORD,
   DATAFLOW_GET_DEPTHRANGE_NEAR,
   DATAFLOW_GET_DEPTHRANGE_FAR,
   DATAFLOW_GET_DEPTHRANGE_DIFF,

   // Buffer loads and stores
   DATAFLOW_ADDRESS,

   DATAFLOW_FLAVOUR_COUNT
} DataflowFlavour;

#define DATAFLOW_MAX_DEPENDENCIES 6

/* These are deliberately in a different order from PRIM_* */
typedef enum {
   DF_BOOL,
   DF_INT,
   DF_UINT,
   DF_FLOAT,
   DF_FSAMPLER,
   DF_ISAMPLER,
   DF_USAMPLER,
   DF_ATOMIC,
   DF_FIMAGE,
   DF_IIMAGE,
   DF_UIMAGE,
   DF_VOID,
   DF_TYPE_COUNT,
   DF_INVALID,
} DataflowType;

struct _Dataflow
{
   int id;
   DataflowFlavour flavour;
   DataflowType type;
   int dependencies_count;

   union {
      Dataflow *dependencies[DATAFLOW_MAX_DEPENDENCIES];
      int reloc_deps[DATAFLOW_MAX_DEPENDENCIES];

      struct {
         Dataflow *operand;
      } unary_op;

      struct {
         Dataflow *left;
         Dataflow *right;
      } binary_op;

      struct {
         Dataflow *cond;
         Dataflow *true_value;
         Dataflow *false_value;
      } cond_op;

      struct {
         Dataflow *coords;
         Dataflow *d;       /* may be NULL */
         Dataflow *b;       /* may be NULL */
         Dataflow *sampler; /* may be NULL */
      } texture;

      struct {
         Dataflow *sampler;
      } texture_size;
   } d;

   union {
      // DATAFLOW_CONST
      struct {
         const_value value;
      } constant;

      // DATAFLOW_CONST_SAMPLER
      struct {
         // location set to SAMPLER_LOCATION_UNDEFINED before samplers have been resolved at link time.
         const_value location;
         bool is_32bit;
      } const_sampler;

      // DATAFLOW_UNIFORM, DATAFLOW_IN
      struct {
         // row set to LINKABLE_VALUE_ROW_UNDEFINED before linkable values have been resolved at link time.
         const_value row;
      } linkable_value;

      // DATAFLOW_UNIFORM_BUFFER
      struct {
         // row set to LINKABLE_VALUE_ROW_UNDEFINED before linkable values have been resolved at link time.
         const_value index;
      } uniform_buffer;

      struct {
         uint32_t binding;
         uint32_t offset;
      } atomic_counter;

      struct {
         uint32_t required_components; /* bitmask - which of r,g,b,a are actually needed. Filled in by copy(). In ES1.1, assume we need all components. */
         uint32_t bits;
      } texture;

      struct {
         uint32_t required_components; /* bitmask - which of r,g,b,a are actually needed. Filled in by copy(). In ES1.1, assume we need all components. */
         int render_target;
      } get_col;

      struct {
         uint32_t required_components;
      } vector_load;

      struct {
         uint32_t component_index;
      } get_vec4_component;

      struct {
         int block;
         int output;
      } external;
   } u;

   uint32_t age; /* determines scheduling order */
};

// On failure, these functions return NULL.
Dataflow *glsl_dataflow_construct_const_value(DataflowType type, const_value value);
Dataflow *glsl_dataflow_construct_const_bool(bool value);
Dataflow *glsl_dataflow_construct_const_int(int value);
Dataflow *glsl_dataflow_construct_const_uint(unsigned value);
Dataflow *glsl_dataflow_construct_const_float(float value);
Dataflow *glsl_dataflow_construct_const_sampler(DataflowType type, bool is_32bit);
Dataflow *glsl_dataflow_construct_linkable_value(DataflowFlavour flavour, DataflowType type);
Dataflow *glsl_dataflow_construct_address_load(DataflowType type, Dataflow *address);
Dataflow *glsl_dataflow_construct_vector_load(DataflowType type, Dataflow *address);
Dataflow *glsl_dataflow_construct_load(DataflowType type);
Dataflow *glsl_dataflow_construct_nullary_op(DataflowFlavour flavour);
Dataflow *glsl_dataflow_construct_unary_op(DataflowFlavour flavour, Dataflow* operand);
Dataflow *glsl_dataflow_construct_binary_op(DataflowFlavour flavour, Dataflow* left, Dataflow* right);
Dataflow *glsl_dataflow_construct_ternary_op(DataflowFlavour flavour, Dataflow* first, Dataflow* second, Dataflow* third);
Dataflow *glsl_dataflow_construct_op(DataflowFlavour flavour, int num_args, Dataflow **arguments);
Dataflow *glsl_dataflow_construct_reinterp(Dataflow *operand, DataflowType new_type);
Dataflow *glsl_dataflow_construct_address(Dataflow *operand);

Dataflow *glsl_dataflow_construct_vec4(Dataflow *r, Dataflow *g, Dataflow *b, Dataflow *a);
Dataflow *glsl_dataflow_construct_get_vec4_component(uint32_t component_index, Dataflow *param, DataflowType type);

void glsl_dataflow_construct_frag_get_col(Dataflow **out, DataflowType type,
                                          uint32_t required_components,
                                          int render_target);
void glsl_dataflow_construct_texture_gadget(Dataflow **r_out, Dataflow **g_out,
                                            Dataflow **b_out, Dataflow **a_out,
                                            uint32_t bits, Dataflow *sampler,
                                            Dataflow *coords,
                                            Dataflow *d, Dataflow *b, uint32_t required_components,
                                            DataflowType component_type);
Dataflow *glsl_dataflow_construct_texture_size(Dataflow *sampler);
Dataflow *glsl_dataflow_construct_external(DataflowType t);
Dataflow *glsl_dataflow_construct_phi(Dataflow *a, Dataflow *b);

const char* glsl_dataflow_info_get_name(DataflowFlavour flavour);

// Returns a dataflow node that converts input to the given type.
Dataflow *glsl_dataflow_convert_type(Dataflow *input, DataflowType out_type);

static inline bool glsl_dataflow_is_integral_type(const Dataflow *df) {
   return df->type == DF_INT || df->type == DF_UINT;
}

#endif // DATAFLOW_H
