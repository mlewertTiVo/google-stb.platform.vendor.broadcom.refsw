/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include <stdlib.h>
#include <setjmp.h>
#include <limits.h>

#include "glsl_backend.h"
#include "glsl_backend_uniforms.h"
#include "glsl_backflow.h"
#include "glsl_binary_shader.h"

#include "glsl_sched_node_helpers.h"

#include "libs/util/gfx_util/gfx_util.h"
#include "libs/core/v3d/v3d_qpu_instr.h"

/*********************** HOW THIS STUFF ALL WORKS ****************************/
/* The register points to the node and the node to the register.             */
/* The last time at which a register was read is stored in reg_available.    */
/* The fit_* functions find a place for the instruction(s) and write into the*/
/*    instruction stream. They return the time of the instruction.           */
/* move_result moves something, leaving the state completely consistent so   */
/*    other stuff doesn't have to worry.                                     */

typedef struct {
   INSTR_T *instructions;
   unsigned n_instructions;

   REG_T    reg[REG_MAX];

   jmp_buf  error_abort;

   uint64_t register_blackout;

   uint32_t regfile_max;            /* Determined by threadability. */
   uint32_t regfile_usable;         /* regfile_max minus number of registers in blackout. */
   uint32_t thrsw_remaining;
   bool     lthrsw;

   Map *backend_states;
} GLSL_SCHEDULER_STATE_T;

typedef enum {
   PIPE_ADD = 0,
   PIPE_MUL = 1,
} AluPipe;

static void backend_error(jmp_buf error_abort, const char *description) {
   longjmp(error_abort, 1);
}

static void translate_write(uint32_t *waddr_out, bool *is_magic, backend_reg reg)
{
   if (reg == REG_UNDECIDED) return;

   assert( IS_RRF(reg) || IS_MAGIC(reg) );

   *is_magic = !IS_RF(reg);

   if (IS_RF(reg))     *waddr_out = reg - REG_RF0;
   else if (IS_R(reg)) *waddr_out = reg - REG_R0;
   else                *waddr_out = reg - REG_MAGIC_BASE;
}

static v3d_qpu_in_source_t translate_mux(backend_reg reg, const backend_reg *reads) {
   if (reg == REG_UNDECIDED) return V3D_QPU_IN_SOURCE_R0;

   assert(IS_R(reg) || (IS_RF(reg) && (reg == reads[0] || reg == reads[1])));

   if (IS_R(reg))            return reg - REG_R0;
   else if (reg == reads[0]) return V3D_QPU_IN_SOURCE_A;
   else                      return V3D_QPU_IN_SOURCE_B;
}

static v3d_qpu_setf_t convert_setf(uint32_t cond_setf) {
   return cs_is_setf(cond_setf) ? cond_setf : V3D_QPU_SETF_NONE;
}

static v3d_qpu_cond_t convert_cond(uint32_t cond_setf) {
   switch(cond_setf) {
      case COND_IFA:  return V3D_QPU_COND_IFA;
      case COND_IFB:  return V3D_QPU_COND_IFB;
      case COND_IFNA: return V3D_QPU_COND_IFNA;
      case COND_IFNB: return V3D_QPU_COND_IFNB;
      default:        return V3D_QPU_COND_ALWAYS;
   }
}

static void gen_wr(struct v3d_qpu_instr_write *wr, const GLSL_OP_T *op) {
   const static struct v3d_qpu_instr_write no_wr = { .addr = V3D_QPU_MAGIC_WADDR_NOP, .magic = true,
                                                     .setf = V3D_QPU_SETF_NONE,
                                                     .cond = V3D_QPU_COND_ALWAYS };
   if (!op->used) *wr = no_wr;
   else {
      translate_write(&wr->addr, &wr->magic, op->output);
      wr->setf = convert_setf(op->cond_setf);
      wr->cond = convert_cond(op->cond_setf);
   }
}

static inline void add_read(backend_reg *read, backend_reg reg) {
   assert(IS_RF(reg));
   assert(read[1] == REG_UNDECIDED || read[0] == reg || read[1] == reg);
   /* No need to record this, it's already here */
   if (read[0] == reg || read[1] == reg) return;

   if (read[0] == REG_UNDECIDED) read[0] = reg;
   else                          read[1] = reg;
}

static void instr_get_reads(const INSTR_T *ci, backend_reg *read) {
   read[0] = read[1] = REG_UNDECIDED;
   for (int p=0; p<2; p++) {
      if (ci->op[p].used && IS_RF(ci->op[p].input_a)) add_read(read, ci->op[p].input_a);
      if (ci->op[p].used && IS_RF(ci->op[p].input_b)) add_read(read, ci->op[p].input_b);
   }
}

static v3d_qpu_unpack_t get_v3d_unpack(SchedNodeUnpack u) {
   switch (u) {
      case UNPACK_ABS:        return V3D_QPU_UNPACK_32F_ABS;
      case UNPACK_NONE:       return V3D_QPU_UNPACK_NONE;
      case UNPACK_F16_A:      return V3D_QPU_UNPACK_16F_LOW_TO_32F;
      case UNPACK_F16_B:      return V3D_QPU_UNPACK_16F_HIGH_TO_32F;
      default: unreachable(); return V3D_QPU_UNPACK_INVALID;
   }
}

static void gen_op(struct v3d_qpu_op *out, const GLSL_OP_T *op_struct, bool add, const backend_reg *instr_reads) {
   const static struct v3d_qpu_op op_nop =
         { .a = { .unpack = V3D_QPU_UNPACK_NONE }, .b = { .unpack = V3D_QPU_UNPACK_NONE },
           .opcode = V3D_QPU_OP_NOP, .pack = V3D_QPU_PACK_NONE };
   const static v3d_qpu_opcode_t flag_op[2][2] = { { V3D_QPU_OP_VFLA, V3D_QPU_OP_VFLB },
                                                   { V3D_QPU_OP_VFLNA, V3D_QPU_OP_VFLNB } };

   if (!op_struct->used) *out = op_nop;
   else {
      if (op_struct->op == V3D_QPU_OP_VFLA || op_struct->op == V3D_QPU_OP_VFLNA) {
         assert(op_struct->input_a == REG_FLAG_A || op_struct->input_a == REG_FLAG_B);
         out->opcode = flag_op[ (op_struct->op == V3D_QPU_OP_VFLNA) ][ (op_struct->input_a == REG_FLAG_B) ];
         out->a.unpack = V3D_QPU_UNPACK_NONE;
         out->b.unpack = V3D_QPU_UNPACK_NONE;
         out->pack = V3D_QPU_PACK_NONE;
      } else {
         out->a.source = translate_mux(op_struct->input_a, instr_reads);
         out->b.source = translate_mux(op_struct->input_b, instr_reads);
         out->a.unpack = get_v3d_unpack(op_struct->unpack[0]);
         out->b.unpack = get_v3d_unpack(op_struct->unpack[1]);
         out->opcode = op_struct->op;
         out->pack = V3D_QPU_PACK_NONE;

         if (add && (out->opcode == V3D_QPU_OP_MOV || out->opcode == V3D_QPU_OP_FMOV)) {
            out->b = out->a;
            out->opcode = (out->opcode == V3D_QPU_OP_MOV) ? V3D_QPU_OP_OR : V3D_QPU_OP_FMIN;
         }
      }
   }
}

static bool sig_requires_read_b(v3d_qpu_sigbits_t sigbits) {
   return sigbits & (V3D_QPU_SIG_SMALL_IMM | V3D_QPU_SIG_ROTATE);
}

#if V3D_VER_AT_LEAST(4,1,34,0)
static void gen_sig(struct v3d_qpu_sig *sig, v3d_qpu_sigbits_t sigbits, backend_reg sig_waddr) {
   sig->sigbits = sigbits;

   if (v3d_qpu_sig_has_result_write(sigbits)) {
# if V3D_VER_AT_LEAST(4,1,34,0)
      translate_write(&sig->waddr, &sig->magic, sig_waddr);
# else
      assert(IS_RF(sig_waddr) || sig_waddr == REG_R(v3d_qpu_sig_default_reg(sigbits)));

      bool magic = false;
      translate_write(&sig->waddr, &magic, sig_waddr);
      sig->sig_reg = !magic;
# endif
   }
}
#endif

static uint64_t pack_instr(const struct v3d_qpu_instr *in) {
   uint64_t ret;
   bool ok = v3d_qpu_instr_try_pack(in, &ret);
   assert(ok);
   return ret;
}

static uint64_t generate(const INSTR_T *ci) {
   struct v3d_qpu_instr in = { .type = V3D_QPU_INSTR_TYPE_ALU };

   gen_wr(&in.add_wr, &ci->op[PIPE_ADD]);
   gen_wr(&in.mul_wr, &ci->op[PIPE_MUL]);

   backend_reg read[2];
   instr_get_reads(ci, read);

   /* Certain instructions must use the first read slot */
   if (ci->op[PIPE_ADD].used && v3d_qpu_op_requires_read_a(ci->op[PIPE_ADD].op)) {
      if (ci->op[PIPE_ADD].input_a == read[1]) {
         uint32_t r = read[0];
         read[0] = read[1];
         read[1] = r;
      }
      assert(ci->op[PIPE_ADD].input_a == read[0]);
   }

   uint32_t raddr[2] = {0, 0};
   for (int i=0; i<2; i++) {
      if (read[i] != REG_UNDECIDED) {
         assert(IS_RF(read[i]));
         raddr[i] = read[i] - REG_RF0;
      }
   }
   in.u.alu.raddr_a = raddr[0];
   in.u.alu.raddr_b = raddr[1];

   if (sig_requires_read_b(ci->sigbits)) {
      assert(in.u.alu.raddr_b == REG_UNDECIDED);
      in.u.alu.raddr_b = ci->rotate_or_small_imm;
   }

   gen_op(&in.u.alu.add, &ci->op[PIPE_ADD], true,  read);
   gen_op(&in.u.alu.mul, &ci->op[PIPE_MUL], false, read);

#if V3D_VER_AT_LEAST(4,1,34,0)
   gen_sig(&in.u.alu.sig, ci->sigbits, ci->sig_waddr);
#else
   in.u.alu.sig.sigbits = ci->sigbits;
#endif

   return pack_instr(&in);
}

void generate_all_instructions(const INSTR_T *instructions, uint32_t instr_count,
                               GENERATED_SHADER_T *gen)
{
   uint32_t unif_count = 0;
   uint32_t varying_count = 0;

   for (unsigned i = 0; i < instr_count; i++) {
      const INSTR_T *ci = &instructions[i];

      gen->instructions[i] = generate(ci);

      /* uniform */
      if (ci->unif != UNIF_NONE) {
         assert(unif_count < instr_count);   /* The uniform buffer is also sized to instr_count */

         assert((ci->conflicts & CONFLICT_UNIF) != CONFLICT_NONE);
         gen->unifs[2*unif_count]   = (uint32_t)ci->unif;
         gen->unifs[2*unif_count+1] = (uint32_t)(ci->unif >> 32);
         unif_count++;
      }

      /* varying */
      if (ci->sigbits & V3D_QPU_SIG_LDVARY && ci->varying < VARYING_ID_HW_0) {
         assert(varying_count < V3D_MAX_VARYING_COMPONENTS);
         gen->varyings[varying_count] = ci->varying;
         varying_count++;
      }
   }

   gen->instruction_count = instr_count;
   gen->unif_count        = unif_count;
   gen->varying_count     = varying_count;
}

static bool conflicts_ok(QBEConflict conflicts_a, QBEConflict conflicts_b) {

#if V3D_VER_AT_LEAST(4,1,34,0) && !V3D_VER_AT_LEAST(4,2,13,0)
   if ((conflicts_a|conflicts_b) & CONFLICT_ADJ_UNIF && (conflicts_a|conflicts_b) & CONFLICT_UNIFA) return false;
   if ((conflicts_a|conflicts_b) & CONFLICT_ADJ_UNIFA && (conflicts_a|conflicts_b) & CONFLICT_UNIF) return false;
#endif

   static const QBEConflict peripherals = CONFLICT_TMU_W | CONFLICT_TMU_C | CONFLICT_TMU_R | CONFLICT_TMUWT |
                                          CONFLICT_TLB_R | CONFLICT_TLB_W | CONFLICT_VPM |
#if V3D_VER_AT_LEAST(4,1,34,0)
                                          CONFLICT_VPMWT |
#endif
                                          CONFLICT_SFU;
#if V3D_VER_AT_LEAST(4,1,34,0)
   static const QBEConflict cond_reg_conflict = CONFLICT_TLB_R   | CONFLICT_TMU_R   | CONFLICT_VARY | CONFLICT_UNIFRF |
                                                CONFLICT_ADDFLAG | CONFLICT_MULFLAG | CONFLICT_SETFLAG;
#else
   static const QBEConflict cond_reg_conflict = CONFLICT_NONE;
#endif
#if V3D_VER_AT_LEAST(4,1,34,0)
   static const QBEConflict tlb_acc = CONFLICT_TLB_R | CONFLICT_TLB_W | CONFLICT_FIRST_INSTR;
#else
   static const QBEConflict tlb_acc = CONFLICT_TLB_R | CONFLICT_TLB_W | CONFLICT_FIRST_INSTR | CONFLICT_POST_THRSW;
#endif

   QBEConflict bad_accesses = conflicts_a;
   if (conflicts_a & tlb_acc)           bad_accesses |= tlb_acc;
   if (conflicts_a & cond_reg_conflict) bad_accesses |= cond_reg_conflict;
   if (conflicts_a & peripherals) {
      QBEConflict p = peripherals;
      if (conflicts_a & CONFLICT_TMU_W) p &= ~CONFLICT_TMU_C;
      if (conflicts_a & CONFLICT_TMU_C) p &= ~CONFLICT_TMU_W;
#if V3D_VER_AT_LEAST(4,1,34,0)
      if (conflicts_a & CONFLICT_VPM)   p &= ~CONFLICT_TMU_R;
      if (conflicts_a & CONFLICT_TMU_R) p &= ~CONFLICT_VPM;
#endif
      bad_accesses |= p;
   }

   // Writes to unifa take additional 3 cycles, and must not span a thrsw.
   // Conflicts between multiple nodes using unifa/ldunifa are handled with IO deps.
   static const QBEConflict thrsw_conflict = CONFLICT_UNIFA_W | CONFLICT_THRSW_SIGNAL | CONFLICT_THRSW_DELAY_1 | CONFLICT_THRSW_DELAY_2;
   if (conflicts_a & thrsw_conflict) bad_accesses |= thrsw_conflict;

   static const QBEConflict sfu_conflict = CONFLICT_SFU | CONFLICT_THRSW_DELAY_2;
   if (conflicts_a & sfu_conflict) bad_accesses |= sfu_conflict;

#if V3D_VER_AT_LEAST(4,1,34,0)
   // Uniform stream and unifa reads conflict.
   static const QBEConflict unif_conflict = CONFLICT_UNIF | CONFLICT_UNIFA;
   if (conflicts_a & unif_conflict) bad_accesses |= unif_conflict;
#endif

   return (conflicts_b & bad_accesses) == 0;
}

static bool can_add_sigbits(v3d_qpu_sigbits_t a, v3d_qpu_sigbits_t b) {
   uint32_t ignored;
   return (a & b) == 0 && v3d_qpu_try_encode_sigbits((a | b), &ignored);
}

static QBEConflict sig_conflicts(v3d_qpu_sigbits_t sigbits) {
   switch (sigbits)
   {
#if V3D_VER_AT_LEAST(4,1,34,0)
   case V3D_QPU_SIG_WRTMUC: return CONFLICT_TMU_C | CONFLICT_UNIF;
#else
   case V3D_QPU_SIG_LDVPM:  return CONFLICT_VPM;
#endif
   case V3D_QPU_SIG_LDUNIF: return CONFLICT_UNIF;
   case V3D_QPU_SIG_LDTLBU: return CONFLICT_TLB_R | CONFLICT_UNIF;
   case V3D_QPU_SIG_LDTLB:  return CONFLICT_TLB_R;
   case V3D_QPU_SIG_LDTMU:  return CONFLICT_TMU_R;
   case V3D_QPU_SIG_LDVARY: return CONFLICT_VARY;
#if V3D_VER_AT_LEAST(4,1,34,0)
   case V3D_QPU_SIG_LDUNIFA:   return CONFLICT_UNIFA;
   case V3D_QPU_SIG_LDUNIFRF:  return CONFLICT_UNIFRF | CONFLICT_UNIF;
   case V3D_QPU_SIG_LDUNIFARF: return CONFLICT_UNIFRF | CONFLICT_UNIFA;
#endif
   default: return 0;
   }
}

static QBEConflict get_output_conflicts(uint32_t output)
{
   if (IS_MAGIC(output))
   {
      switch (output)
      {
      case REG_MAGIC_TLBU:
      case REG_MAGIC_TLB:
         return CONFLICT_TLB_W;
      case REG_MAGIC_TMUA:
      case REG_MAGIC_TMUAU:
      case REG_MAGIC_TMUD:
         return CONFLICT_TMU_W;
#if V3D_VER_AT_LEAST(4,1,34,0)
      case REG_MAGIC_TMUC:
         return CONFLICT_TMU_W | CONFLICT_TMU_C;
      case REG_MAGIC_TMUT:
      case REG_MAGIC_TMUR:
      case REG_MAGIC_TMUI:
      case REG_MAGIC_TMUB:
      case REG_MAGIC_TMUDREF:
      case REG_MAGIC_TMUOFF:
      case REG_MAGIC_TMUS:
      case REG_MAGIC_TMUSCM:
      case REG_MAGIC_TMUSF:
      case REG_MAGIC_TMUSLOD:
      case REG_MAGIC_TMUHS:
      case REG_MAGIC_TMUHSCM:
      case REG_MAGIC_TMUHSF:
      case REG_MAGIC_TMUHSLOD:
         return CONFLICT_TMU_W;
#else
      case REG_MAGIC_VPM:
      case REG_MAGIC_VPMU:
         return CONFLICT_VPM;

      case REG_MAGIC_TMU:
      case REG_MAGIC_TMUL:
         return CONFLICT_TMU_W;
#endif
      case REG_MAGIC_RECIP:
      case REG_MAGIC_RSQRT:
      case REG_MAGIC_RSQRT2:
      case REG_MAGIC_EXP:
      case REG_MAGIC_LOG:
      case REG_MAGIC_SIN:
         return CONFLICT_SFU;
#if V3D_VER_AT_LEAST(4,1,34,0)
      case REG_MAGIC_UNIFA:
         return CONFLICT_UNIFA_W;
#endif
      }
   }

   return 0;
}

static void ensure_instructions(GLSL_SCHEDULER_STATE_T *sched, uint32_t time) {
   if (time >= sched->n_instructions) {
      sched->instructions = glsl_safemem_realloc(sched->instructions, 2*sched->n_instructions*sizeof(INSTR_T));
      memset(sched->instructions + sched->n_instructions, 0, sched->n_instructions * sizeof(INSTR_T));
      sched->n_instructions *= 2;
   }
   assert(time < sched->n_instructions);
}

#if V3D_VER_AT_LEAST(4,1,34,0) && !V3D_VER_AT_LEAST(4,2,13,0)
static void add_unif_read_conflicts(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, QBEConflict conflicts)
{
   if (!(conflicts & (CONFLICT_UNIF | CONFLICT_UNIFA)))
      return;

   assert(!(conflicts & CONFLICT_UNIF) ^ !(conflicts & CONFLICT_UNIFA));

   QBEConflict adj_conflict = conflicts & CONFLICT_UNIFA ? CONFLICT_ADJ_UNIFA : CONFLICT_ADJ_UNIF;
   if (time > 0)
      sched->instructions[time-1].conflicts |= adj_conflict;

   ensure_instructions(sched, time+1);
   sched->instructions[time+1].conflicts |= adj_conflict;
}
#endif

static uint32_t fit_sig(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, v3d_qpu_sigbits_t sigbits, uint64_t unif
#if V3D_VER_AT_LEAST(4,1,34,0)
      , backend_reg sig_waddr
#endif
      )
{
   QBEConflict sig = sig_conflicts(sigbits);
#if V3D_VER_AT_LEAST(4,1,34,0)
   QBEConflict out = get_output_conflicts(sig_waddr);
   assert(conflicts_ok(sig, out));
   QBEConflict conflicts = sig | out;
#else
   QBEConflict conflicts = sig;
#endif

   /* Advance current instruction until we find one which doesn't conflict */
   INSTR_T *ci;
   while (true) {
      ensure_instructions(sched, time);

      ci = &sched->instructions[time];
      if (can_add_sigbits(ci->sigbits, sigbits) && conflicts_ok(ci->conflicts, conflicts))
         break;

      time++;
   }

   /* Fill in instruction */
   assert( (conflicts & CONFLICT_UNIF) || unif == UNIF_NONE);

   ci->sigbits   |= sigbits;
   ci->conflicts |= conflicts;
   if (unif != UNIF_NONE) ci->unif = unif;
#if V3D_VER_AT_LEAST(4,1,34,0)
   if (v3d_qpu_sig_has_result_write(sigbits)) {
      assert(ci->sig_waddr == REG_UNDECIDED);
      ci->sig_waddr = sig_waddr;
   }
#endif

#if V3D_VER_AT_LEAST(4,1,34,0) && !V3D_VER_AT_LEAST(4,2,13,0)
   add_unif_read_conflicts(sched, time, conflicts);
#endif

   return time;
}

static inline bool reg_is_sfu(uint32_t output) {
   return output == REG_MAGIC_RECIP  ||
          output == REG_MAGIC_RSQRT  ||
          output == REG_MAGIC_RSQRT2 ||
          output == REG_MAGIC_EXP    ||
          output == REG_MAGIC_LOG    ||
          output == REG_MAGIC_SIN;
}

static uint32_t get_output_delay(uint32_t output) {
   if (reg_is_sfu(output)) return 2;
   else return 1;
}

static QBEConflict get_cond_setf_conflicts(uint32_t cond_setf, AluPipe pipe)
{
   QBEConflict conflict = CONFLICT_NONE;
   assert(is_cond_setf(cond_setf));
   if (cs_is_none(cond_setf)) return CONFLICT_NONE;

   switch (pipe)
   {
   case PIPE_ADD: conflict |= CONFLICT_ADDFLAG; break;
   case PIPE_MUL: conflict |= CONFLICT_MULFLAG; break;
   default: unreachable();
   }

   if (cs_is_setf(cond_setf)) conflict |= CONFLICT_SETFLAG;

   /* Special Case: there isn't encoding space for muf + an add thing */
   if (cs_is_updt(cond_setf) && pipe == PIPE_MUL) conflict |= CONFLICT_ADDFLAG;

   return conflict;
}

#if V3D_VER_AT_LEAST(4,1,34,0)
static bool is_vpm_load(v3d_qpu_opcode_t op) {
   return op == V3D_QPU_OP_LDVPMV_IN  || op == V3D_QPU_OP_LDVPMD_IN  ||
          op == V3D_QPU_OP_LDVPMV_OUT || op == V3D_QPU_OP_LDVPMD_OUT ||
          op == V3D_QPU_OP_LDVPMG_IN  || op == V3D_QPU_OP_LDVPMG_OUT ||
          op == V3D_QPU_OP_LDVPMP;
}
static bool is_vpm_store(v3d_qpu_opcode_t op) {
   return op == V3D_QPU_OP_STVPMV || op == V3D_QPU_OP_STVPMD || op == V3D_QPU_OP_STVPMP;
}
#endif

static QBEConflict get_special_instruction_conflicts(v3d_qpu_opcode_t op) {
   if (op == V3D_QPU_OP_SETMSF || op == V3D_QPU_OP_SETREVF) return CONFLICT_MSF;

   if (op == V3D_QPU_OP_TMUWT) return CONFLICT_TMUWT;

#if V3D_VER_AT_LEAST(4,1,34,0)
   if (v3d_qpu_op_is_sfu(op)) return CONFLICT_SFU;
#endif

#if V3D_VER_AT_LEAST(4,1,34,0)
   if (op == V3D_QPU_OP_VPMWT) return CONFLICT_VPMWT;
   if (is_vpm_load(op) || is_vpm_store(op))  return CONFLICT_VPM;
#else
   if (op == V3D_QPU_OP_VPMSETUP) return CONFLICT_VPM;
#endif
   return 0;
}

static void read_register(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg, uint32_t time) {
   assert(reg < countof(sched->reg));
   sched->reg[reg].available = gfx_umax(sched->reg[reg].available, time);
}

static void clean_register(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg) {
   assert(IS_RRF(reg) || IS_FLAG(reg));
   if (sched->reg[reg].user != NULL && sched->reg[reg].user->remaining_dependents == 0)
      sched->reg[reg].user = NULL;
}

static void read_and_clean_register(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg, uint32_t time) {
   assert(IS_RRF(reg) || IS_FLAG(reg));

   read_register(sched, reg, time);
   clean_register(sched, reg);
}

static void invalidate_register(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg, uint32_t time)
{
   assert(reg < countof(sched->reg));

   assert(sched->reg[reg].user == NULL);
   assert(sched->reg[reg].available <= time);
   sched->reg[reg].available = time;
}

static void write_register(GLSL_SCHEDULER_STATE_T *sched, BackendNodeState *node, backend_reg reg, uint32_t time)
{
   assert(reg < countof(sched->reg));

   /* node may be NULL, in which case we are writing throw-away data */
   if (node != NULL) node->reg = reg;
   sched->reg[reg].user = node;
   sched->reg[reg].written = time;

   assert(sched->reg[reg].available <= time);
   sched->reg[reg].available = time;
}

static inline bool no_read_clashes(unsigned reads_allowed, const backend_reg *old_read, const backend_reg *new_read)
{
   unsigned n_reads = (old_read[0] != REG_UNDECIDED) + (old_read[1] != REG_UNDECIDED);
   for (int i=0; i<2; i++)
      if (new_read[i] != REG_UNDECIDED && new_read[i] != old_read[0] && new_read[i] != old_read[1])
         n_reads++;

   return n_reads <= reads_allowed;
}

static inline bool can_place_alu(const INSTR_T *ci, AluPipe pipe, bool rot, QBEConflict conflicts, const backend_reg *read)
{
   backend_reg instr_read[2];
   instr_get_reads(ci, instr_read);

   unsigned max_reads = (sig_requires_read_b(ci->sigbits) || rot) ? 1 : 2;
   bool can_place = !rot || can_add_sigbits(ci->sigbits, V3D_QPU_SIG_ROTATE);
   can_place &= !ci->op[pipe].used;
   can_place &= conflicts_ok(ci->conflicts, conflicts);
   can_place &= no_read_clashes(max_reads, instr_read, read);

   return can_place;
}

QBEConflict get_conflicts(backend_reg output, uint32_t cond_setf, AluPipe pipe,
                          v3d_qpu_opcode_t op, uint64_t unif)
{
   QBEConflict conflicts[4];
   conflicts[0] = get_output_conflicts(output);
   conflicts[1] = get_cond_setf_conflicts(cond_setf, pipe);
   conflicts[2] = get_special_instruction_conflicts(op);
   conflicts[3] = (unif != UNIF_NONE ? CONFLICT_UNIF : CONFLICT_NONE);
   for (int i=1; i<4; i++) {
      /* If individual conflict words aren't ok then instruction conflicts with itself */
      assert(conflicts_ok(conflicts[0], conflicts[i]));
      conflicts[0] |= conflicts[i];
   }
   return conflicts[0];
}

static uint32_t time_alu(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, AluPipe pipe, bool rot,
                         v3d_qpu_opcode_t op, backend_reg output,
                         backend_reg input_a, backend_reg input_b, uint64_t unif, uint32_t cond_setf)
{
   assert(!rot || pipe == PIPE_MUL);

   /* Determine conflicts */
   QBEConflict conflicts = get_conflicts(output, cond_setf, pipe, op, unif);

   /* See which regfiles we're reading from */
   backend_reg read[2] = { REG_UNDECIDED, REG_UNDECIDED };
   if (IS_RF(input_a)) add_read(read, input_a);
   if (IS_RF(input_b)) add_read(read, input_b);

   /* Advance current instruction until we find one which doesn't conflict */
   INSTR_T *ci;
   while (true) {
      ensure_instructions(sched, time);

      ci = &sched->instructions[time];
      if (can_place_alu(ci, pipe, rot, conflicts, read))
         return time;

      time++;
   }
}

static void place_alu(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, AluPipe pipe,
                      v3d_qpu_opcode_t op, SchedNodeUnpack ul, SchedNodeUnpack ur,
                      backend_reg output, backend_reg input_a, backend_reg input_b,
                      uint64_t unif, uint32_t cond_setf, MOV_EXCUSE mov_excuse)
{
   INSTR_T *ci = &sched->instructions[time];

   if (unif != UNIF_NONE) {
      assert(ci->unif == UNIF_NONE);
      ci->unif = unif;
   }

   /* Fill in instruction */
   assert(!ci->op[pipe].used);
   ci->op[pipe].used       = true;
   ci->op[pipe].op         = op;
   ci->op[pipe].unpack[0]  = ul;
   ci->op[pipe].unpack[1]  = ur;
   ci->op[pipe].output     = output;
   ci->op[pipe].input_a    = input_a;
   ci->op[pipe].input_b    = input_b;
   ci->op[pipe].cond_setf  = cond_setf;
   ci->op[pipe].mov_excuse = mov_excuse;

   QBEConflict conflicts = get_conflicts(output, cond_setf, pipe, op, unif);
   assert(conflicts_ok(ci->conflicts, conflicts));
   ci->conflicts |= conflicts;

#if V3D_VER_AT_LEAST(4,1,34,0) && !V3D_VER_AT_LEAST(4,2,13,0)
   add_unif_read_conflicts(sched, time, conflicts);
#endif
}

static bool pipe_valid_for_op(AluPipe p, v3d_qpu_opcode_t op) {
   bool mul_only = (op == V3D_QPU_OP_FMUL || op == V3D_QPU_OP_VFMUL || op == V3D_QPU_OP_SMUL24 || op == V3D_QPU_OP_UMUL24 || op == V3D_QPU_OP_MULTOP);
   bool both     = (op == V3D_QPU_OP_ADD  || op == V3D_QPU_OP_SUB   || op == V3D_QPU_OP_FMOV || op == V3D_QPU_OP_MOV);

   return p == PIPE_MUL ? (mul_only || both) : (!mul_only);
}

static uint32_t fit_alu(GLSL_SCHEDULER_STATE_T *sched, uint32_t time,
                        v3d_qpu_opcode_t op, SchedNodeUnpack ul, SchedNodeUnpack ur,
                        backend_reg output, backend_reg input_a, backend_reg input_b,
                        uint64_t unif, uint32_t cond_setf, MOV_EXCUSE mov_excuse)
{
   uint32_t add_time = ~0u;
   uint32_t mul_time = ~0u;
   if (pipe_valid_for_op(PIPE_ADD, op))
      add_time = time_alu(sched, time, PIPE_ADD, false, op, output, input_a, input_b, unif, cond_setf);
   if (pipe_valid_for_op(PIPE_MUL, op))
      mul_time = time_alu(sched, time, PIPE_MUL, false, op, output, input_a, input_b, unif, cond_setf);

   if (mul_time <= add_time) {
      place_alu(sched, mul_time, PIPE_MUL, op, ul, ur, output, input_a, input_b, unif, cond_setf, mov_excuse);
      return mul_time;
   } else {
      place_alu(sched, add_time, PIPE_ADD, op, ul, ur, output, input_a, input_b, unif, cond_setf, mov_excuse);
      return add_time;
   }
}

static uint32_t fit_mov(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, backend_reg output, backend_reg input, uint64_t unif, uint32_t cond_setf, MOV_EXCUSE mov_excuse)
{
   assert(IS_FLAG(input) || IS_RRF(input));
   v3d_qpu_opcode_t op = (IS_FLAG(input) ? V3D_QPU_OP_VFLNA : V3D_QPU_OP_MOV);
   return fit_alu(sched, time, op, UNPACK_NONE, UNPACK_NONE, output, input, REG_UNDECIDED, unif, cond_setf, mov_excuse);
}

/* No op may write rf* or read rf[0,2] on or after thrend */
static bool op_valid_for_thrend(const GLSL_OP_T *op) {
   if (!op->used) return true;

#if V3D_HAS_GFXH1678_FIX
   if (IS_RF(op->output) && v3d_qpu_res_type_from_opcode(op->op) != V3D_QPU_RES_TYPE_DIRECT_RF_WRITE) return false;
#else
   if (IS_RF(op->output)) return false;
#endif
   if (op->input_a == REG_RF0 || op->input_a == REG_RF1 || op->input_a == REG_RF2) return false;
   if (op->input_b == REG_RF0 || op->input_b == REG_RF1 || op->input_b == REG_RF2) return false;
   return true;
}

static bool instr_valid_for_thrend(const INSTR_T *i) {
   QBEConflict conflicts = V3D_VER_AT_LEAST(4,1,34,0) ? CONFLICT_NONE : (CONFLICT_MSF | CONFLICT_VPM);
   return op_valid_for_thrend(&i->op[PIPE_ADD]) && op_valid_for_thrend(&i->op[PIPE_MUL]) &&
          !(i->conflicts & conflicts) && !(i->sigbits & V3D_QPU_SIG_LDVARY)
#if V3D_VER_AT_LEAST(4,1,34,0)
          && !IS_RF(i->sig_waddr)
#endif
          ;
}

static const QBEConflict thrsw_conflicts[4] = {
   CONFLICT_THRSW_SIGNAL,
   CONFLICT_THRSW_DELAY_1,
   CONFLICT_THRSW_DELAY_2,
   CONFLICT_POST_THRSW
};

static bool can_add_thrsw(const GLSL_SCHEDULER_STATE_T *sched, uint32_t time, bool lthrsw, bool thrend)
{
   if (time < 3) return false;

   const INSTR_T *i = &sched->instructions[time-3];

   if (!can_add_sigbits(i[0].sigbits, V3D_QPU_SIG_THRSW)) return false;
   if (lthrsw && !can_add_sigbits(i[1].sigbits, V3D_QPU_SIG_THRSW)) return false;

   for (unsigned c = 0; c != countof(thrsw_conflicts); ++c) {
      if (!conflicts_ok(i[c].conflicts, thrsw_conflicts[c]))
         return false;
   }

   if (thrend) {
#if !V3D_HAS_GFXH1678_FIX
      if (time > 3 && i[-1].op[PIPE_ADD].used && v3d_qpu_op_is_sfu(i[-1].op[PIPE_ADD].op)) return false;
#endif

      for (int j=0; j<3; j++)
         if (!instr_valid_for_thrend(&i[j])) return false;

#if !V3D_VER_AT_LEAST(4,1,34,0)
#  if !V3D_VER_AT_LEAST(3,3,0,0)
      if (i[0].conflicts & CONFLICT_UNIF) return false;
#  endif
      if (i[1].conflicts & CONFLICT_UNIF) return false;
      if (i[2].conflicts & CONFLICT_UNIF) return false;
#endif

      /* Z-writes are not allowed on the final instruction */
      /* Any z-write in the IR that may schedule last is required to load the
       * config on that instruction. (This is very likely to be the case because
       * z-writes have to come before other writes anyway). If this weren't
       * the case then we'd have to determine the config here more cleverly */
      for (int pipe = 0; pipe < 2; pipe++) {
         if (i[2].op[pipe].used && i[2].op[pipe].output == REG_MAGIC_TLBU) {
            uint8_t cfg = (i[2].unif >> 32) & 0xFF;
            if ((cfg & 0xF0) == 0x80) return false;
         }
      }

      // GFXH-1625
      if (i[2].op[PIPE_ADD].used && ((i[2].op[PIPE_ADD].op == V3D_QPU_OP_TMUWT)
#if V3D_VER_AT_LEAST(4,1,34,0)
            || (i[2].op[PIPE_ADD].op == V3D_QPU_OP_VPMWT)
#endif
            ))
         return false;
   }
   return true;
}

/* Place thrsw. Returns the instruction which triggers the actual switch.
 * The first instruction of the new thread is the instruction after that */
static uint32_t fit_thrsw(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, bool thrend) {
   bool lthrsw = (sched->thrsw_remaining == 1 && sched->lthrsw);

   if (thrend) assert(sched->thrsw_remaining == 0);
   else {
      assert(sched->thrsw_remaining >= 1);
      sched->thrsw_remaining--;
   }

   while (true) {
      ensure_instructions(sched, time);

      if (can_add_thrsw(sched, time, lthrsw, thrend)) break;

      time++;
   }

   // Add thrsw related conflicts (starting from signal).
   for (unsigned c = 0; c != countof(thrsw_conflicts); ++c) {
      sched->instructions[time-3+c].conflicts |= thrsw_conflicts[c];
   }

   sched->instructions[time-3].sigbits |= V3D_QPU_SIG_THRSW;
   if (lthrsw)
      sched->instructions[time-2].sigbits |= V3D_QPU_SIG_THRSW;

   /* Make sure writes to accumulators don't get scheduled *before* the thrsw */
   for (int i = 0; i < 6; i++) {
      invalidate_register(sched, REG_R(i), time);
      invalidate_register(sched, REG_RTOP, time);
   }

   return time-1;
}

/* Insert code to calculate the varying value.
 * The dependencies determine where the "ldvary" may be placed. The return value
 * is the instruction that writes the final result.  */
static uint32_t fit_varying(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, backend_reg output, uint64_t unif,
                            backend_reg temp_reg, backend_reg w_reg, uint32_t varying_id, VaryingType varying_type)
{
   assert((varying_type == VARYING_FLAT                             ) ||
          (varying_type == VARYING_NOPERSP && w_reg == REG_UNDECIDED) ||
          (varying_type == VARYING_DEFAULT && w_reg != REG_UNDECIDED)    );
   assert(varying_type != VARYING_FLAT || temp_reg == REG_UNDECIDED);

   GLSL_OP_T v_ops[3][2] = {
      /* Default */
      { { true, V3D_QPU_OP_FMUL, { UNPACK_NONE, UNPACK_NONE }, temp_reg, REG_R3,   w_reg,  SETF_NONE},
        { true, V3D_QPU_OP_FADD, { UNPACK_NONE, UNPACK_NONE }, output,   temp_reg, REG_R5, SETF_NONE} },
      /* Line Coord */
      { { true, V3D_QPU_OP_MOV,  { UNPACK_NONE, UNPACK_NONE }, temp_reg, REG_R3,   0,      SETF_NONE},
        { true, V3D_QPU_OP_FADD, { UNPACK_NONE, UNPACK_NONE }, output,   temp_reg, REG_R5, SETF_NONE} },
      /* Flat */
      { { false, 0,                                                                              },
        { true,  V3D_QPU_OP_OR, { UNPACK_NONE, UNPACK_NONE }, output, REG_R5, REG_R5, SETF_NONE} } };
   GLSL_OP_T *v = v_ops[varying_type-1];

#if V3D_VER_AT_LEAST(4,1,34,0)
   time = fit_sig(sched, time, V3D_QPU_SIG_LDVARY, UNIF_NONE, REG_R3);
#else
   time = fit_sig(sched, time, V3D_QPU_SIG_LDVARY, UNIF_NONE);
#endif

   sched->instructions[time].varying = varying_id;

   uint32_t t[2] = { 0, 0 };
   for (int i=0; i<2; i++) {
      AluPipe  p[2] = { PIPE_MUL,  PIPE_ADD };
      uint64_t u[2] = { UNIF_NONE, unif };
      if (v[i].used) {
         t[i] = time_alu(sched, gfx_umax(time+i+1, t[0]+1), p[i], false, v[i].op, v[i].output,
                                                                  v[i].input_a, v[i].input_b, u[i], v[i].cond_setf);

         place_alu(sched, t[i], p[i], v[i].op, v[i].unpack[0], v[i].unpack[1], v[i].output,
                                      v[i].input_a, v[i].input_b, u[i], v[i].cond_setf, v[i].mov_excuse);
      }
   }

   /* Update our temporary registers. Main code updates inputs/outputs */
   assert(sched->reg[REG_R3].user == NULL);
   assert(sched->reg[REG_R5].user == NULL);
   write_register(sched, NULL, REG_R3, time + 1);
   write_register(sched, NULL, REG_R5, time + 2);
   if (varying_type != VARYING_FLAT) {
      assert(sched->reg[temp_reg].user == NULL);
      write_register(sched, NULL, temp_reg, t[0] + 1);
      read_register(sched, REG_R3,   t[0]);
      read_register(sched, temp_reg, t[1]);
   }
   read_register(sched, REG_R5, t[1]);

   return t[1];
}

static uint32_t fit_imul32(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, backend_reg output,
                           backend_reg left, backend_reg right, uint64_t unif)
{
   uint32_t multop_time = time_alu(sched, time, PIPE_MUL, false, V3D_QPU_OP_MULTOP, REG_MAGIC_NOP,
                                                                 left, right, UNIF_NONE, SETF_NONE);
   place_alu(sched, multop_time, PIPE_MUL, V3D_QPU_OP_MULTOP, UNPACK_NONE, UNPACK_NONE, REG_MAGIC_NOP,
                                           left, right, UNIF_NONE, SETF_NONE, MOV_EXCUSE_NONE);

   time = time_alu(sched, multop_time+1, PIPE_MUL, false, V3D_QPU_OP_UMUL24, output, left, right, unif, SETF_NONE);
   place_alu(sched, time, PIPE_MUL, V3D_QPU_OP_UMUL24, UNPACK_NONE, UNPACK_NONE, output, left, right, unif, SETF_NONE, MOV_EXCUSE_NONE);

   /* Update our temporary registers. Main code updates inputs/outputs */
   read_register(sched, REG_RTOP, time);
   return time;
}

static uint32_t fit_rotate(GLSL_SCHEDULER_STATE_T *sched, uint32_t time, backend_reg output,
                           backend_reg input_a, uint32_t amount)
{
   time = time_alu(sched, time, PIPE_MUL, true, V3D_QPU_OP_MOV, output, input_a, REG_UNDECIDED, UNIF_NONE, SETF_NONE);

   sched->instructions[time].sigbits |= V3D_QPU_SIG_ROTATE;
   sched->instructions[time].rotate_or_small_imm = amount;
   place_alu(sched, time, PIPE_MUL, V3D_QPU_OP_MOV, UNPACK_NONE, UNPACK_NONE, output, input_a, REG_UNDECIDED,
                                    UNIF_NONE, SETF_NONE, MOV_EXCUSE_NONE);
   return time;
}

/* May or may not choose a register in r0-r4.
   If insist is true then it will always choose something.  */
static backend_reg choose_acc(const GLSL_SCHEDULER_STATE_T *sched, bool insist, uint32_t time)
{
   uint32_t oldest = ~0u;
   backend_reg oldest_r = REG_UNDECIDED;

   /* Choose a free register, failing that one that wasn't used recently */
   for (int i = 0; i < 5; i++) {
      if ( sched->reg[REG_R(i)].user == NULL ) return REG_R(i);

      if (sched->reg[REG_R(i)].written < oldest) {
         oldest = sched->reg[REG_R(i)].written;
         oldest_r = REG_R(i);
      }
   }
   assert(oldest_r != REG_UNDECIDED);
   if (oldest + 4 < time) return oldest_r;

   /* If we've insisted, then return the least recently used */
   if (insist) return oldest_r;
   else return REG_UNDECIDED;
}

static backend_reg choose_regfile(GLSL_SCHEDULER_STATE_T *sched, uint32_t time)
{
   uint32_t best_time = ~0u;
   backend_reg best = REG_UNDECIDED;
   for (unsigned j = 0; j != sched->regfile_max; ++j)
   {
      // permute priority of regfile to avoid likelyhood of rf0-2 near thrend
      unsigned i = (j + 3) % sched->regfile_max;

      if (sched->register_blackout & (1ull << i)) continue;

      if (sched->reg[REG_RF(i)].user == NULL) {
         if (sched->reg[REG_RF(i)].available < best_time) {
            best_time = sched->reg[REG_RF(i)].available;
            best = REG_RF(i);
         }
      }
      if (best_time <= time) break;
   }

   if (best == REG_UNDECIDED) backend_error(sched->error_abort, "No regfile space");

   return best;
}

/*
   Move a value in a register into a different register. We update the register
   tracking structures so that everything is consistent in the new state
*/
static void move_to_register(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg,
                             backend_reg output, uint32_t mov_excuse)
{
   /* Validate input and output choices */
   assert(IS_RRF(reg) || IS_FLAG(reg));
   assert(sched->reg[reg].user != NULL);
   assert(IS_RRF(output));
   assert(sched->reg[output].user == NULL);
   assert(output != reg);

   BackendNodeState *node = sched->reg[reg].user;
   assert(node->reg == reg);

   assert(node->remaining_dependents > 0);

   /* Place after the result has been written and the output reg is free */
   uint32_t mov_timestamp = sched->reg[reg].written;
   mov_timestamp = gfx_umax(mov_timestamp, sched->reg[output].available);
   mov_timestamp = fit_mov(sched, mov_timestamp, output, reg, UNIF_NONE, SETF_NONE, mov_excuse);
   uint32_t delay = get_output_delay(output);

   write_register(sched, node, output, mov_timestamp + delay);

   sched->reg[reg].user = NULL;
   read_register(sched, reg, mov_timestamp);
}

static void move_result(GLSL_SCHEDULER_STATE_T *sched, backend_reg reg, uint32_t mov_excuse)
{
   if (sched->reg[reg].user == NULL) return;    /* Nothing to move */
   uint32_t timestamp_guess = sched->reg[reg].written;
   move_to_register(sched, reg, choose_regfile(sched, timestamp_guess), mov_excuse);
}

static uint64_t node_get_unif(const Backflow *node)
{
   assert(node->unif_type >= 0 &&
          node->unif_type <= BACKEND_UNIFORM_LAST_ELEMENT);

   if (node->unif_type == BACKEND_UNIFORM_UNASSIGNED) {
      assert(node->unif == 0);
      return UNIF_NONE;
   }

   return UNIF_PAIR(node->unif_type, node->unif);
}

static backend_reg find_output_reg_sig(v3d_qpu_sigbits_t sigbit) {
   switch (sigbit) {
      case V3D_QPU_SIG_LDUNIF: return REG_R5;
#if !V3D_VER_AT_LEAST(4,1,34,0)
      case V3D_QPU_SIG_LDTMU:  return REG_R4;
      case V3D_QPU_SIG_LDTLB:  return REG_R3;
      case V3D_QPU_SIG_LDTLBU: return REG_R3;
      case V3D_QPU_SIG_LDVPM:  return REG_R3;
#else
      case V3D_QPU_SIG_LDTMU:  return REG_UNDECIDED;
      case V3D_QPU_SIG_LDTLB:  return REG_UNDECIDED;
      case V3D_QPU_SIG_LDTLBU: return REG_UNDECIDED;
#endif
#if V3D_VER_AT_LEAST(4,1,34,0)
      case V3D_QPU_SIG_LDUNIFA:   return REG_R5;
      case V3D_QPU_SIG_LDUNIFRF:  return REG_UNDECIDED;
      case V3D_QPU_SIG_LDUNIFARF: return REG_UNDECIDED;
#endif
      default:            return REG_MAGIC_NOP; /* Other SIGS have no output */
   }
}

static backend_reg find_output_reg_magic(backend_reg waddr) {
   switch (waddr) {
      /* Node genuinely has no output */
      case REG_MAGIC_NOP:   return REG_MAGIC_NOP;
      /* Output to the SFU comes back in r4 */
      case REG_MAGIC_RECIP:
      case REG_MAGIC_RSQRT:
      case REG_MAGIC_RSQRT2:
      case REG_MAGIC_LOG:
      case REG_MAGIC_EXP:
      case REG_MAGIC_SIN:   return REG_R4;
#if V3D_VER_AT_LEAST(4,1,34,0)
      case REG_MAGIC_R5REP:
#endif
      case REG_R5:          return REG_R5;
      /* These are shader output so the results aren't returned */
      case REG_MAGIC_TMUA:
      case REG_MAGIC_TMUAU:
      case REG_MAGIC_TMUD:
#if V3D_VER_AT_LEAST(4,1,34,0)
      case REG_MAGIC_TMUC:
      case REG_MAGIC_TMUT:
      case REG_MAGIC_TMUR:
      case REG_MAGIC_TMUI:
      case REG_MAGIC_TMUB:
      case REG_MAGIC_TMUDREF:
      case REG_MAGIC_TMUOFF:
      case REG_MAGIC_TMUS:
      case REG_MAGIC_TMUSCM:
      case REG_MAGIC_TMUSF:
      case REG_MAGIC_TMUSLOD:
      case REG_MAGIC_TMUHS:
      case REG_MAGIC_TMUHSCM:
      case REG_MAGIC_TMUHSF:
      case REG_MAGIC_TMUHSLOD:
#else
      case REG_MAGIC_TMU:
      case REG_MAGIC_TMUL:
      case REG_MAGIC_VPM:
      case REG_MAGIC_VPMU:
#endif
#if V3D_VER_AT_LEAST(4,1,34,0)
      case REG_MAGIC_UNIFA:
#endif
      case REG_MAGIC_TLBU:
      case REG_MAGIC_TLB:
         return REG_MAGIC_NOP;
      default:
         unreachable();
         return REG_MAGIC_NOP;
   }
}

static inline void validate_reg(const GLSL_SCHEDULER_STATE_T *sched, const BackendNodeState *node) {
   if (node == NULL) return;

   assert(IS_RRF(node->reg) || IS_FLAG(node->reg));
   assert(sched->reg[node->reg].user == node);
   assert(node->remaining_dependents >= 1);
}

static void flagify(GLSL_SCHEDULER_STATE_T *sched, BackendNodeState *node, bool require_a)
{
   if (node->reg != REG_FLAG_A && (node->reg != REG_FLAG_B || require_a)) {
      if (node->reg != REG_FLAG_B)
         move_result(sched, REG_FLAG_B, MOV_EXCUSE_FLAG_B_OUT_OF_THE_WAY);

      /* Going to clobber both flag registers so wait until after last read */
      uint32_t timestamp = sched->reg[REG_FLAG_A].available;
      timestamp = gfx_umax(timestamp, sched->reg[REG_FLAG_B].available);
      timestamp = gfx_umax(timestamp, sched->reg[node->reg].written);
      timestamp = fit_mov(sched, timestamp, REG_MAGIC_NOP, node->reg, UNIF_NONE, SETF_PUSHZ, MOV_EXCUSE_FLAGIFY);
      uint32_t delay = get_output_delay(REG_MAGIC_NOP);
      read_register(sched, node->reg, timestamp);
      sched->reg[node->reg].user = NULL;

      /* Push the a-flag down to b and write the result to a */
      write_register(sched, sched->reg[REG_FLAG_A].user, REG_FLAG_B, timestamp+delay);
      write_register(sched, node,                        REG_FLAG_A, timestamp+delay);
   }
}

static uint32_t dereference(const GLSL_SCHEDULER_STATE_T *sched, BackendNodeState *node)
{
   assert(node->remaining_dependents >= 1);
   node->remaining_dependents--;

   /*
   Don't update sched->reg_user[reg] or sched->reg_available[reg] yet.

   reg_available holds the latest instruction to read from node, excluding the current one.
   (The current instruction isn't going to interfere with itself).
   We update reg_available once we know where the current instruction will get put.

   We don't update reg_user yet because we want to avoid this register from being allocated to
   something else (i.e. move_result) until we're really finished with it.
   */

   return node->reg;
}

static uint32_t specialise_cond(uint32_t cond_setf, uint32_t reg) {
   assert(IS_FLAG(reg));
   if (cond_setf == COND_IFFLAG)
      return reg == REG_FLAG_A ? COND_IFA  : COND_IFB;
   if (cond_setf == COND_IFNFLAG)
      return reg == REG_FLAG_A ? COND_IFNA : COND_IFNB;
   return cond_setf; /* Other conditions don't need specialising */
}

static BackendNodeState *get_state(Map *map, const Backflow *node) {
   if (node == NULL) return NULL;
   return glsl_map_get(map, node);
}

static void schedule_node(GLSL_SCHEDULER_STATE_T *sched, const Backflow *node, BackendNodeState *node_state)
{
   backend_reg input_a  = REG_UNDECIDED;
   backend_reg input_b  = REG_UNDECIDED;
   backend_reg flag_reg = REG_UNDECIDED;

   v3d_qpu_sigbits_t sigbits = node->type == SIG ? node->u.sigbits : 0;
#if V3D_VER_AT_LEAST(4,1,34,0)
   if (sigbits == V3D_QPU_SIG_LDUNIF && (sched->reg[REG_R5].user != NULL || node->magic_write != REG_UNDECIDED))
      sigbits = V3D_QPU_SIG_LDUNIFRF;
   else if (sigbits == V3D_QPU_SIG_LDUNIFA)
      sigbits = V3D_QPU_SIG_LDUNIFARF; // always prefer LDUNIFARF
#endif

   /* If the result comes to a fixed register (sig, sfu, etc) fill in which */
   if (node->type == SIG)
      node_state->reg = find_output_reg_sig(sigbits);

   if (node->magic_write != REG_UNDECIDED)
      node_state->reg = find_output_reg_magic(node->magic_write);

   BackendNodeState *flag_dep     = get_state(sched->backend_states, node->dependencies[0]);  /* cond_setf determines usage */
   BackendNodeState *output_dep   = get_state(sched->backend_states, node->dependencies[3]);  /* NULL if not overwriting another node */
   BackendNodeState *input_dep[2] = { get_state(sched->backend_states, node->dependencies[1]),
                                      get_state(sched->backend_states, node->dependencies[2]) };

#ifndef NDEBUG
   validate_reg(sched, flag_dep);
   validate_reg(sched, output_dep);
   for (int i=0; i<2; i++) validate_reg(sched, input_dep[i]);
#endif

   /* If we need flags as inputs(/outputs) sort them out first */
   uint32_t cond_setf = node->cond_setf;
   if (flag_dep != NULL) {
      bool requires_a = cs_is_updt(node->cond_setf)
#if V3D_VER_AT_LEAST(4,1,34,0)
               || (node->type == ALU && node->u.alu.op == V3D_QPU_OP_FLAFIRST)
#endif
#if V3D_HAS_FLNAFIRST
               || (node->type == ALU && node->u.alu.op == V3D_QPU_OP_FLNAFIRST)
#endif
               ;
      flagify(sched, flag_dep, requires_a);
      flag_reg = dereference(sched, flag_dep);

      if (flag_dep->remaining_dependents != 0 && cs_is_updt(node->cond_setf)) {
         move_result(sched, flag_reg, MOV_EXCUSE_COPY_ON_WRITE);
      }
      /* XXX We only do a copy on write above, not a move out of the way */

      /* If we need to specialise based on which flag, do that now */
      cond_setf = specialise_cond(node->cond_setf, flag_reg);
      assert(!requires_a || flag_reg == REG_FLAG_A);
   }

   /* Now sort out the dependencies of our outputs, starting with flags */
   /* Push clobbers b-flag */
   if (cs_is_push(node->cond_setf)) {
      move_result(sched, REG_FLAG_B, MOV_EXCUSE_FLAG_B_OUT_OF_THE_WAY);
   }

   if (output_dep != NULL) {
      assert(node->cond_setf == COND_IFFLAG || node->cond_setf == COND_IFNFLAG);
      assert(node->magic_write == REG_UNDECIDED);
      assert(node_state->reg == REG_UNDECIDED);

      /* We can't write to r5 */
      uint32_t cur_reg = output_dep->reg;
      if (cur_reg == REG_R5) move_result(sched, cur_reg, MOV_EXCUSE_R5_WRITE);
      if (IS_RF(cur_reg) && ((1ull << (cur_reg - REG_RF0)) & sched->register_blackout) != 0)
         move_result(sched, cur_reg, MOV_EXCUSE_COPY_ON_WRITE);
   }

   if (node->type == SPECIAL_ROTATE) {
      if (node->u.rotate.amount == 0 && input_dep[1]->reg != REG_R5) {
         move_result(sched, REG_R5, MOV_EXCUSE_OUT_OF_THE_WAY);
         move_to_register(sched, input_dep[1]->reg, REG_R5, MOV_EXCUSE_R_RF_INPUT);
      }

      if (!node->u.rotate.quad && !IS_R(input_dep[0]->reg)) {
         backend_reg r = choose_acc(sched, true, 0);
         move_to_register(sched, input_dep[0]->reg, r, MOV_EXCUSE_R_RF_INPUT);
      }
      if (node->u.rotate.quad && !IS_RF(input_dep[0]->reg)) {
         move_result(sched, input_dep[0]->reg, MOV_EXCUSE_R_RF_INPUT);
      }
   }

   if (node->type == ALU && v3d_qpu_op_requires_read_a(node->u.alu.op) &&
       !IS_RF(input_dep[0]->reg) )
   {
      move_result(sched, input_dep[0]->reg, MOV_EXCUSE_R_RF_INPUT);
   }

   /* Decrement reference counts. Don't retire them, lest we overwrite the
      register with a move later.  */
   if (output_dep != NULL) node_state->reg = dereference(sched, output_dep);
   if (input_dep[0] != NULL) input_a = dereference(sched, input_dep[0]);
   if (input_dep[1] != NULL) input_b = dereference(sched, input_dep[1]);

   if (output_dep != NULL) {
      if (output_dep->remaining_dependents != 0)
         move_result(sched, node_state->reg, MOV_EXCUSE_COPY_ON_WRITE);

      assert(IS_RRF(node_state->reg));  /* Should be writing to a reg not a flag */
   }

   backend_reg waddr = node->magic_write;
   if (cs_is_setf(cond_setf)) {
      /* We can't currently write to both a register and a flag */
      assert(node->magic_write == REG_UNDECIDED);
      assert(node_state->reg == REG_UNDECIDED);
      node_state->reg = REG_FLAG_A;
      waddr = REG_MAGIC_NOP;
   }

   if (node->type == ALU && (node->u.alu.op == V3D_QPU_OP_VFLA || node->u.alu.op == V3D_QPU_OP_VFLNA)) {
      assert(input_a == REG_UNDECIDED && IS_FLAG(flag_reg));
      input_a = flag_reg;
      flag_reg = REG_UNDECIDED;
   }

   /*
   Special register allocation for VARYING.
   The boilerplate code requires a temporary register and also makes use of r3 and r5.

   Note that it *is* safe for the temporary and output registers to be the same. It's also
   safe to use r3 as the temporary register (though it makes stacking less efficient).
   So we're sloppy about making sure the same thing doesn't get allocated for two different
   purposes, but it won't actually break anything.
   */
   backend_reg temp_reg = REG_UNDECIDED;
   if (node->type == SPECIAL_VARYING) {
      move_result(sched, REG_R5, MOV_EXCUSE_OUT_OF_THE_WAY);
      move_result(sched, REG_R3, MOV_EXCUSE_OUT_OF_THE_WAY);
      if (node->u.varying.type != VARYING_FLAT) {
         temp_reg = choose_acc(sched, true, 0);   /* Insist on acc. Not sure why. Time doesn't matter when insisting */
         move_result(sched, temp_reg, MOV_EXCUSE_OUT_OF_THE_WAY);
      }
   }

   /* Accumulators aren't preserved over a thrsw */
   if (node->type == SPECIAL_THRSW)
      for (int i = 0; i < 6; i++) move_result(sched, REG_R(i), MOV_EXCUSE_THRSW);

   /* First guess the final scheduling time ... */
   uint32_t timestamp = 0, delay;

   if (node->type == SPECIAL_VARYING) {
      timestamp = gfx_umax(timestamp, sched->reg[REG_R3].available);
      timestamp = gfx_smax(timestamp, sched->reg[REG_R5].available-1);
      if (node->u.varying.type != VARYING_FLAT)
         timestamp = gfx_smax(timestamp, sched->reg[temp_reg].available-1);
   }
   if (node->type == SPECIAL_THRSW) {
      /* offset==1 because timestamp wants to be *after* the mov, not on the same line. */
      for (int i=0; i<6; i++) timestamp = gfx_umax(timestamp, sched->reg[REG_R(i)].available+1);
      timestamp = gfx_umax(timestamp, sched->reg[REG_RTOP].available + 1);
   }

   if (node->type == SPECIAL_ROTATE) {
      if (node->u.rotate.amount == 0)
         timestamp = gfx_umax(timestamp, sched->reg[REG_R5].written + 1);
      if (!node->u.rotate.quad)
         timestamp = gfx_umax(timestamp, sched->reg[input_a].written + 1);
   }

   if (node->type == SPECIAL_IMUL32)
      timestamp = gfx_umax(timestamp, sched->reg[REG_RTOP].available);

   /* Wait until the inputs we need to read have been written */
   if (flag_reg  != REG_UNDECIDED) timestamp = gfx_umax(timestamp, sched->reg[flag_reg ].written);
   if (input_a   != REG_UNDECIDED) timestamp = gfx_umax(timestamp, sched->reg[input_a  ].written);
   if (input_b   != REG_UNDECIDED) timestamp = gfx_umax(timestamp, sched->reg[input_b  ].written);
   /* If we'll update flags then wait for the last read of the previous flag value */
   if (cs_is_push(cond_setf)) timestamp = gfx_umax(timestamp, sched->reg[REG_FLAG_B].available);

   /* Wait for our iodependencies */
   for (BackflowIODepChainNode *n=node->io_dependencies.head; n; n=n->next) {
      BackendNodeState *s = glsl_map_get(sched->backend_states, n->val.dep);
      timestamp = gfx_smax(timestamp, (int)s->io_timestamp + n->val.io_timestamp_offset);
   }

   /* Register allocation. If not needed then REG_MAGIC_NOP should already be set */
   if (node_state->reg == REG_UNDECIDED) {
      /* Try an accumulator first then fallback to regfile */
      if (!glsl_sched_node_requires_regfile(node)) {
         if (node_state->remaining_dependents == 0) node_state->reg = REG_MAGIC_NOP;
         else if (node_state->remaining_dependents == 1 && node->type != SPECIAL_VARYING)
            node_state->reg = choose_acc(sched, false, timestamp);
      }
      if (node_state->reg == REG_UNDECIDED) node_state->reg = choose_regfile(sched, timestamp);
   }
   assert(node_state->reg != REG_UNDECIDED);
   /* If this node isn't a magic write, set waddr to the output. */
   if (waddr == REG_UNDECIDED) waddr = node_state->reg;

   if (node_state->reg != REG_MAGIC_NOP) {
      /* Insert movs if we're writing to something which is already in use */
      assert(IS_RRF(node_state->reg) || node_state->reg == REG_FLAG_A);

      if (sched->reg[node_state->reg].user && sched->reg[node_state->reg].user->remaining_dependents != 0) {
         // Pushing to flag A preserves it's value in flag B. This state is updated below.
         if (!cs_is_push(cond_setf))
            move_result(sched, node_state->reg, MOV_EXCUSE_OUT_OF_THE_WAY);
      }
   }

   /* Wait until any register we'll write to has been read for the last time */
   if (node_state->reg != REG_MAGIC_NOP) timestamp = gfx_umax(timestamp, sched->reg[node_state->reg].available);

   uint64_t unif = node_get_unif(node);
   switch (node->type)
   {
   case SIG:
      timestamp = fit_sig(sched, timestamp, sigbits, unif
#if V3D_VER_AT_LEAST(4,1,34,0)
                           , waddr
#endif
                          );
      delay = get_output_delay(waddr);
      break;
   case SPECIAL_THRSW:
      timestamp = fit_thrsw(sched, timestamp, false);
      delay = 0;
      break;
   case SPECIAL_VARYING:
      /* For a varying input_a contains our w value */
      timestamp = fit_varying(sched, timestamp, waddr, unif, temp_reg, input_a, node->u.varying.row, node->u.varying.type);
      delay = get_output_delay(waddr);
      break;
   case SPECIAL_IMUL32:
      timestamp = fit_imul32(sched, timestamp, waddr, input_a, input_b, unif);
      delay = get_output_delay(waddr);
      break;
   case ALU:
      if (node->u.alu.op == V3D_QPU_OP_MOV) {
         timestamp = fit_mov(sched, timestamp, waddr, input_a, unif, cond_setf, MOV_EXCUSE_IR);
      } else {
         uint32_t mov_excuse = node->u.alu.op == V3D_QPU_OP_FMOV ? MOV_EXCUSE_IR : MOV_EXCUSE_NONE;
         timestamp = fit_alu(sched, timestamp, node->u.alu.op,
                             node->u.alu.unpack[0], node->u.alu.unpack[1],
                             waddr, input_a, input_b, unif, cond_setf, mov_excuse);
      }
      delay = (v3d_qpu_op_is_sfu(node->u.alu.op) ? 1 : 0) + get_output_delay(waddr);
      break;
   case SPECIAL_VOID:
      /* timestamp = timestamp; */
      delay = 0;
      break;
   case SPECIAL_ROTATE:
      timestamp = fit_rotate(sched, timestamp, waddr, input_a, node->u.rotate.amount);
      delay = get_output_delay(waddr);
      break;
   default:
      unreachable();
   }

   node_state->io_timestamp = timestamp + 1;  /* We must go *after* the actual timestamp */
   if (node->type == SPECIAL_VOID)
      node_state->io_timestamp--;

   /* Mark reads on all the registers and clean them if done with */
   if (flag_reg != REG_UNDECIDED) read_and_clean_register(sched, flag_reg, timestamp);
   if (input_a  != REG_UNDECIDED) read_and_clean_register(sched, input_a,  timestamp);
   if (input_b  != REG_UNDECIDED) read_and_clean_register(sched, input_b,  timestamp);

   if (cs_is_setf(cond_setf)) {
      /* TODO: how to generate instructions which both write somewhere and update flags? */
      assert(waddr == REG_MAGIC_NOP);
      assert(node_state->reg == REG_FLAG_A);
   }

   if (cs_is_push(cond_setf)) {
      /* Update flag B with the pushed value */
      assert(sched->reg[REG_FLAG_B].user == NULL);
      write_register(sched, sched->reg[REG_FLAG_A].user, REG_FLAG_B, timestamp+delay);
      sched->reg[REG_FLAG_A].user = NULL;
   }

   if (node_state->reg != REG_MAGIC_NOP) {
      /* Should write to somewhere "normal" that noone else is using */
      assert(IS_RRF(node_state->reg) || IS_FLAG(node_state->reg));
      assert(sched->reg[node_state->reg].user == NULL || sched->reg[node_state->reg].user == output_dep ||
                                                   sched->reg[node_state->reg].user == flag_dep);

      write_register(sched, node_state, node_state->reg, timestamp + delay);
      /* Sometimes a node has iodependencies only, e.g. an attrib */
      if (node_state->remaining_dependents == 0) sched->reg[node_state->reg].user = NULL;
   }
}

#ifdef KHRN_SHADER_STATS
#include <stdio.h>

const char *mov_excuse_strings[] = {
   "",
   "   # unfolded output       ",
   "   # mov present in IR     ",
   "   # move out of the way   ",
   "   # move flag B for a push",
   "   # move acc due to thrsw ",
   "   # copy on write         ",
   "   # can't write to r5     ",
   "   # flagify               ",
   "   # r/rf input required   ",
};

static void print_shader_stats(const GLSL_SCHEDULER_STATE_T *sched, int n_instrs, int n_threads, ShaderFlavour flavour, bool bin_mode) {
   int valid[2] = { 0, 0 }, nop[2] = { 0, 0 }, mov[2] = { 0, 0 };
   int unifs = 0;
   bool tmu = false;
   int n_mov_excuses[MOV_EXCUSE_COUNT] = { 0,  };

   for (int i=0; i<n_instrs; i++) {
      for (int p = 0; p < 2; p++) {
         if (sched->instructions[i].op[p].used) valid[p]++;
         else nop[p]++;

         if (sched->instructions[i].op[p].mov_excuse != MOV_EXCUSE_NONE) {
            n_mov_excuses[sched->instructions[i].op[p].mov_excuse]++;
            mov[p]++;
         }
      }

      if (sched->instructions[i].unif != UNIF_NONE) unifs++;
      if (sched->instructions[i].sigbits & V3D_QPU_SIG_LDTMU) tmu = true;
   }

   for (int p=0; p<2; p++) valid[p] -= mov[p];

   int reg_used = sched->regfile_max;
   for (int i=sched->regfile_max-1; i>=0; i--) {
      if (sched->reg[REG_RF(i)].available != 0) break;
      reg_used = i;
   }

#if V3D_VER_AT_LEAST(4,1,34,0)
   tmu = true;
#endif
   printf("Shader stats:\n");
   printf("   Shader Type: %s%s, N threads: %d/%d\n",
          glsl_shader_flavour_name(flavour), bin_mode ? " (bin)": "",
          n_threads, tmu ? 4 : 1);
   printf("   Total instructions: %d\n", n_instrs);
   printf("   Uniform reads:      %d\n", unifs);
   printf("   Registers used:     %d\n", reg_used);
   printf("                         a    m\n");
   printf("   Valid Instructions: %3d  %3d\n", valid[PIPE_ADD], valid[PIPE_MUL]);
   printf("   Movs:               %3d  %3d\n", mov[PIPE_ADD], mov[PIPE_MUL]);
   printf("   Nops:               %3d  %3d\n", nop[PIPE_ADD], nop[PIPE_MUL]);
   printf("   Utilisation (%%):    %3d  %3d\n", (int)((100.0f*valid[PIPE_ADD])/n_instrs), (int)((100.0f*valid[PIPE_MUL])/n_instrs));
   printf("\n");
   printf("   Mov excuses:\n");
   for (int i=1; i<MOV_EXCUSE_COUNT; i++) {
      if (n_mov_excuses[i] > 0) {
         printf("  %s   %3d\n", mov_excuse_strings[i], n_mov_excuses[i]);
      }
   }
   printf("\n");
}
#endif

typedef struct {
   Backflow *nodes[MAX_STACK];
   int depth;
} SchedStack;

static bool stack_push(SchedStack *stack, Backflow *d, BackendNodeState *s)
{
   assert(s->phase == PHASE_UNVISITED);
   if (stack->depth == MAX_STACK-1) return false;

   s->phase = PHASE_STACKED;
   stack->depth++;
   stack->nodes[stack->depth] = d;
   return true;
}

static Backflow *get_unvisited_dep(Map *backend_states, Backflow *node) {
   for (BackflowIODepChainNode *n=node->io_dependencies.head; n; n=n->next) {
      BackendNodeState *s = glsl_map_get(backend_states, n->val.dep);
      assert(s->phase != PHASE_STACKED);
      if (s->phase == PHASE_UNVISITED)
         return n->val.dep;
   }

#if V3D_VER_AT_LEAST(4,1,34,0)
   v3d_qpu_sigbits_t defer_unifs = V3D_QPU_SIG_LDUNIF | V3D_QPU_SIG_LDUNIFRF | V3D_QPU_SIG_LDUNIFA | V3D_QPU_SIG_LDUNIFARF;
#else
   v3d_qpu_sigbits_t defer_unifs = V3D_QPU_SIG_LDUNIF;
#endif
   for (unsigned pass = 0; pass != 2; ++pass) {
      for (unsigned i = 0; i < BACKFLOW_DEP_COUNT; i++) {
         Backflow* dep = node->dependencies[i];
         if (dep == NULL) continue;
         if (dep->type == SIG && (dep->u.sigbits & defer_unifs)) continue;

         BackendNodeState *s = glsl_map_get(backend_states, dep);
         assert(s->phase != PHASE_STACKED);
         if (s->phase == PHASE_UNVISITED)
            return dep;
      }
      defer_unifs = 0;
   }
   return NULL;
}

static bool all_deps_ready(Map *backend_states, const Backflow *node) {
   for (int i=0; i<BACKFLOW_DEP_COUNT; i++) {
      if (node->dependencies[i] != NULL) {
         BackendNodeState *s = glsl_map_get(backend_states, node->dependencies[i]);
         if (s->phase != PHASE_COMPLETE) return false;
      }
   }
   for (BackflowIODepChainNode *n=node->io_dependencies.head; n; n=n->next) {
      BackendNodeState *s = glsl_map_get(backend_states, n->val.dep);
      if (s->phase != PHASE_COMPLETE) return false;
   }
   return true;
}

static void visit_recursive(SchedStack *stack, GLSL_SCHEDULER_STATE_T *sched, Backflow *root)
{
   BackendNodeState *s = glsl_map_get(sched->backend_states, root);
   if (s->phase == PHASE_COMPLETE) return;

   if (!stack_push(stack, root, s))
      backend_error(sched->error_abort, "Stack overflow");

   while(stack->depth >= 0) {
      /* Hunt for a dependency that we haven't visited yet. */
      Backflow *node = stack->nodes[stack->depth];
      Backflow *dep = get_unvisited_dep(sched->backend_states, node);
      if (dep != NULL) {
         if (!stack_push(stack, dep, glsl_map_get(sched->backend_states, dep)))
            backend_error(sched->error_abort, "Stack overflow");
         continue;               /* Recurse */
      }

      BackendNodeState *node_state = glsl_map_get(sched->backend_states, node);
      /* Already visited all dependencies so deal with this node. */
      schedule_node(sched, node, node_state);
      assert(node_state->phase == PHASE_STACKED);
      node_state->phase = PHASE_COMPLETE;
      stack->depth--;

      /* Determine how many regfile locations are in use */
      int threshold = sched->regfile_usable - ((sched->regfile_max * 14) / 64);

      int p = 0;
      for (unsigned i=0; i<sched->regfile_max; i++) {
         if (sched->reg[REG_RF(i)].user != NULL) p++;
      }

      /* If this exceeds a threshold (arbitrarily set) then search for
       * nodes that free up locations and schedule them next.           */
      if (p > threshold) {
         for (unsigned i=0; i<sched->regfile_max; i++) {
            if (sched->reg[REG_RF(i)].user) {
               BackendNodeState *node = sched->reg[REG_RF(i)].user;
               /* Having only 1 dependent means that scheduling it will free the register */
               if (node->remaining_dependents != 1) continue;

               /* Search for the dependent */
               BackflowChainNode *n;
               BackendNodeState *ns;
               for (n = node->data_dependents.head; n; n = n->next) {
                  ns = glsl_map_get(sched->backend_states, n->val);
                  /* n->val may not be needed for the shader, so check if anything will use it */
                  if (ns->phase != PHASE_COMPLETE) break;
               }
               /* If the 1 remaining_dependent is a block output then there may be no node */
               if (n == NULL) continue;

               /* This is only helpful (or correct) if we don't have to schedule any other things first */
               if (ns->phase == PHASE_UNVISITED && all_deps_ready(sched->backend_states, n->val))
                  if (!stack_push(stack, n->val, ns))
                     backend_error(sched->error_abort, "Stack overflow");
            }
         }
      }
   }
}

static void prescheduled(GLSL_SCHEDULER_STATE_T *sched, Backflow *node, uint32_t reg) {
   /* TODO: Decide which of these conditions means 'not used' */
   if (node == NULL) return;

   BackendNodeState *node_state = glsl_map_get(sched->backend_states, node);

   if (node_state->remaining_dependents > 0) {
      assert(sched->reg[reg].user == NULL);
      write_register(sched, node_state, reg, 0);

      /* Remove this from consideration. We don't need to schedule it */
      node_state->phase = PHASE_COMPLETE;
   }
}

#if !V3D_VER_AT_LEAST(4,1,34,0)
static void break_alias(GLSL_SCHEDULER_STATE_T *sched) {
   if (sched->reg[REG_RF0].user != NULL && sched->reg[REG_RF1].user != NULL)
   {
      uint32_t time = fit_mov(sched, 0, REG_RF0, REG_RF0, UNIF_NONE,
                              SETF_NONE, MOV_EXCUSE_OUT_OF_THE_WAY);
      assert(time == 0);
   }
}
#endif

GENERATED_SHADER_T *glsl_backend(BackflowChain  *iodeps,
                                 ShaderFlavour   flavour,
                                 bool            bin_mode,
                                 uint32_t        thrsw_count,
                                 uint32_t        threadability,
                                 Backflow      **sched_queue,
                                 unsigned        sched_count,
                                 RegList        *presched,
                                 RegList        *outputs,
                                 Map            *state_map,
                                 Backflow       *branch_cond,
                                 uint64_t        blackout,
                                 bool            last,
                                 bool            lthrsw)
{
   GENERATED_SHADER_T *gen = NULL;

   SchedStack *stack = glsl_safemem_malloc(sizeof(SchedStack));
   stack->depth = -1;

   GLSL_SCHEDULER_STATE_T *sched = glsl_safemem_calloc(1, sizeof(GLSL_SCHEDULER_STATE_T));
   sched->instructions   = glsl_safemem_calloc(1000, sizeof(INSTR_T));
   sched->n_instructions = 1000;

   if(setjmp(sched->error_abort) != 0) {
      glsl_safemem_free(stack);
      glsl_safemem_free(sched->instructions);
      glsl_safemem_free(sched);
      if (gen) {
         glsl_safemem_free(gen->instructions);
         glsl_safemem_free(gen->unifs);
      }
      glsl_safemem_free(gen);
      return NULL;
   }

   sched->register_blackout = blackout;
   sched->regfile_max       = get_max_regfile(threadability);
   sched->regfile_usable    = sched->regfile_max;

   for (unsigned i=0; i<sched->regfile_max; i++)
      if (blackout & (1ull<<i)) sched->regfile_usable--;

   sched->backend_states = state_map;
   /* If data starts in the regfile record that now */
   for ( ; presched; presched = presched->next)
      prescheduled(sched, presched->node, presched->reg);

   /* Remove any register that we output to from the blackout */
   for (RegList *l = outputs; l!=NULL; l=l->next) {
      uint64_t reg_bit = (1ull << (l->reg - REG_RF0));
      sched->register_blackout &= ~reg_bit;
      sched->regfile_usable += 1;
   }

#if !V3D_VER_AT_LEAST(4,1,34,0)
   /* Break any potential aliasing between w and w_c immediately */
   break_alias(sched);
#endif

   sched->lthrsw          = lthrsw;
   sched->thrsw_remaining = thrsw_count;

   /* TLB access is not allowed in the first instruction */
   sched->instructions[0].conflicts = CONFLICT_FIRST_INSTR;

   for (int i = 0; i != sched_count; ++i)
      visit_recursive(stack, sched, sched_queue[i]);

   for (RegList *r = outputs; r; r=r->next)
      visit_recursive(stack, sched, r->node);

   for (BackflowChainNode *n=iodeps->head; n; n=n->next)
      visit_recursive(stack, sched, n->val);

   if (branch_cond)
      visit_recursive(stack, sched, branch_cond);

   uint32_t instr_count = 0;
   if (!last) {
      /* Restore the complete blackout here. A single output value that is being
       * placed in multiple registers goes wrong otherwise. It will be moved out
       * of its first register to the second, and the first could be reused. */
      for (RegList *r = outputs; r; r=r->next) {
         uint64_t reg_bit = (1ull << (r->reg - REG_RF0));
         sched->register_blackout |= reg_bit;
      }

      /* Place the outputs in their assigned registers */
      for (RegList *r = outputs; r; r=r->next) {
         BackendNodeState *r_node_state = glsl_map_get(sched->backend_states, r->node);
         if (r_node_state->reg != r->reg) {
            if (sched->reg[r->reg].user != NULL) {
               move_result(sched, r->reg, MOV_EXCUSE_OUT_OF_THE_WAY);
            }
            move_to_register(sched, r_node_state->reg, r->reg, MOV_EXCUSE_OUTPUT);
         }
         /* The write is done one instruction before the data will be available */
         instr_count = gfx_smax(instr_count, sched->reg[r->reg].available);
      }

      if (branch_cond != NULL) {
         flagify(sched, glsl_map_get(sched->backend_states, branch_cond), true);      /* Need this in flag a */
         instr_count = gfx_smax(instr_count, sched->reg[REG_FLAG_A].available);
      }
   }

   for (BackflowChainNode *n=iodeps->head; n; n=n->next) {
      BackendNodeState *s = glsl_map_get(sched->backend_states, n->val);
      instr_count = gfx_umax(instr_count, s->io_timestamp);
   }

#if !V3D_VER_AT_LEAST(3,3,0,0)
   /* Workaround GFXH-1276 for points. Insert an extra varying load
    * somewhere if we have read a hardware varying, but no user varyings */
   bool extra_vary = false;
   uint32_t hw_vary_time;
   for (uint32_t i=0; i<instr_count; i++) {
      if (!(sched->instructions[i].sigbits & V3D_QPU_SIG_LDVARY)) continue;

      /* Only look for points. These must read hw_1 and it must come after hw_0. */
      if (sched->instructions[i].varying == VARYING_ID_HW_1) {
         extra_vary = true;
         hw_vary_time = i;
      } else if (sched->instructions[i].varying != VARYING_ID_HW_0) {
         extra_vary = false;
         break;
      }
   }

   /* Add an extra varying. Lie and say that it is hw_1 so that it doesn't get included
    * in the varyings map (that might break the vertex shader). The compiler doesn't use
    * these values for anything else, so the lie is safe. */
   if (extra_vary) {
      uint32_t t = fit_varying(sched, gfx_umax(hw_vary_time+1, sched->reg[REG_R5].available), REG_MAGIC_NOP,
                               UNIF_NONE, REG_UNDECIDED, REG_UNDECIDED, VARYING_ID_HW_1, VARYING_FLAT);
      instr_count = gfx_umax(instr_count, t);
   }
#endif

   if (last)
      instr_count = fit_thrsw(sched, instr_count, true) + 1;

   /* ldvary has delayed effects that could affect the next block, so wait */
   if (instr_count > 0 && sched->instructions[instr_count-1].sigbits & V3D_QPU_SIG_LDVARY) instr_count++;

   assert(!last || !outputs);
   assert(instr_count <= sched->n_instructions);

#ifndef NDEBUG
   /* Ensure that all registers are free at the end, otherwise we've leaked */
   /* First clean up the outputs */
   for (RegList *r = outputs; r; r=r->next) {
      BackendNodeState *s = glsl_map_get(sched->backend_states, r->node);
      dereference(sched, s);
      clean_register(sched, s->reg);
   }
   if (branch_cond) {
      BackendNodeState *s = glsl_map_get(sched->backend_states, branch_cond);
      dereference(sched, s);
      clean_register(sched, s->reg);
   }
   for (int i=0; i<countof(sched->reg); i++) assert(sched->reg[i].user == NULL);
#endif

#ifdef KHRN_SHADER_STATS
   print_shader_stats(sched, instr_count, threadability, flavour, bin_mode);
#endif

   gen               = glsl_safemem_calloc(1, sizeof(GENERATED_SHADER_T));
   gen->instructions = glsl_safemem_malloc(  instr_count * sizeof(uint64_t));
   gen->unifs        = glsl_safemem_malloc(2*instr_count * sizeof(uint32_t));
   /* Fills in sched->gen with the actual code! */
   generate_all_instructions(sched->instructions, instr_count, gen);

   glsl_safemem_free(stack);
   glsl_safemem_free(sched->instructions);
   glsl_safemem_free(sched);

   return gen;
}
