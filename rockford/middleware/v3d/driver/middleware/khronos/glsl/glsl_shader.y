%{
   #include "middleware/khronos/glsl/glsl_common.h"

   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>

   #include "middleware/khronos/glsl/glsl_symbols.h"
   #include "middleware/khronos/glsl/glsl_ast.h"
   #include "middleware/khronos/glsl/glsl_dataflow.h"
   #include "middleware/khronos/glsl/glsl_trace.h"
   #include "middleware/khronos/glsl/glsl_errors.h"
   #include "middleware/khronos/glsl/glsl_intern.h"
   #include "middleware/khronos/glsl/glsl_globals.h"
   #include "middleware/khronos/glsl/glsl_builders.h"
   #include "middleware/khronos/glsl/glsl_extensions.h"

   #include "middleware/khronos/glsl/prepro/glsl_prepro_expand.h"
   #include "middleware/khronos/glsl/prepro/glsl_prepro_directive.h"

   extern TokenData pptoken;

   extern void ppunput(int c);
   extern int pplex(void);

   static TokenSeq *seq;

   static void yyerror (void *top_level_statement, const char *s) {
      if (strcmp(s, "syntax error") == 0) {
         // Catch syntax errors and redirect them.
         glsl_compile_error(ERROR_LEXER_PARSER, 1, g_LineNumber, NULL);
      } else {
         glsl_compile_error(ERROR_UNKNOWN, 0, g_LineNumber, "%s", s);
      }
   }

   static int yylex(void)
   {
      V3DTokenType type = 0;
      TokenData data;

      do {
         if (!seq) {
            seq = glsl_expand(NULL, false);
            if (!seq)
               return 0;
         }

         type = seq->token->type;
         data = seq->token->data;
         seq = seq->next;
      } while (type == WHITESPACE);

      /*
         detect uses of reserved keywords
      */

      if (type >= ASM && type <= USING)
         glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL);

      if (type == UNKNOWN)
         glsl_compile_error(ERROR_UNKNOWN, 0, g_LineNumber, "unknown character");

      /*
         detokenize tokens which were meaningful to the preprocessor
      */

      if (type >= DEFINE && type <= DISABLE)
         type = IDENTIFIER;

      /*
         the idea here is to return CANDIDATE_TYPE_NAME if it could be a type,
         and otherwise to return IDENTIFIER
      */

      switch (type) {
      case IDENTIFIER:
      {
         Symbol *sym = glsl_symbol_table_lookup(g_SymbolTable, data.s, true);

         yylval.lookup.symbol = sym;
         yylval.lookup.name = data.s;

         if (sym)
            switch (sym->flavour) {
            case SYMBOL_TYPE:
               type = CANDIDATE_TYPE_NAME;
               break;
            case SYMBOL_VAR_INSTANCE:
            case SYMBOL_PARAM_INSTANCE:
            case SYMBOL_FUNCTION_INSTANCE:
               break;
            default:
               /*
                  we shouldn't see anything else in the main symbol table
               */
               UNREACHABLE();
               break;
            }
         break;
      }
      case PPNUMBERI:
         yylval.i = data.i;
         break;
      case PPNUMBERF:
         yylval.f = data.f;
         break;
      case PPNUMBERU:
         yylval.s = data.s;
         break;
      default:
         break;
      }

      return type;
   }

   // This allows yyparse to take an argument of type void* with the given name.
   #define YYPARSE_PARAM top_level_statement

   #define YYMALLOC yymalloc
   #define YYFREE yyfree

   void *yymalloc(size_t bytes)
   {
      return glsl_fastmem_malloc(bytes, true);
   }

   void yyfree(void *ptr)
   {
      UNUSED(ptr);
   }

   void enter_loop(void)
   {
      vcos_assert(g_LoopDepth < INT_MAX);
      g_LoopDepth++;
   }
   static void exit_loop(void)
   {
      vcos_assert(g_LoopDepth > 0);
      g_LoopDepth--;
   }
   static int in_loop(void)
   {
      return (g_LoopDepth > 0);
   }
%}

%{
   void glsl_init_parser(void)
   {
      g_DataflowSources = glsl_dataflow_sources_new();
      g_SymbolTable = glsl_symbol_table_populate(glsl_symbol_table_new());
      g_InGlobalScope = true;

      glsl_sb_invalidate(&g_StringBuilder);

      g_StructBuilderMembers = NULL;
      g_FunctionBuilderParams = NULL;

      g_NextAnonParam = 0;

      seq = NULL;

      g_LoopDepth = 0;
   }

   static const char *generate_anon_param(void)
   {
      char c[16];

      sprintf(c, "$$anon%d\n", g_NextAnonParam++);

      return glsl_intern(c, true);
   }
%}

// Just one if-else shift/reduce conflict, for which the default behaviour is fine.
%expect 1

%no-lines
%parse-param { void *top_level_statement }

// The rule that will derive a correct program.
%start translation_unit

// Yacc's value stack type.
%union
{
   const_int i; const_float f; const char* s;
   struct { const char* name; Symbol* symbol; } lookup;
   SymbolType* type;
   Symbol* symbol;
   CallContext call_context;
   Expr* expr; ExprFlavour expr_flavour;
   Statement* statement;
   StatementChain* statement_chain;
   struct { Statement* a; Statement* b; } statement2;
}

%token<lookup> IDENTIFIER
%token<i> PPNUMBERI
%token<f> PPNUMBERF
%token<s> PPNUMBERU
%token<s> UNKNOWN

%token HASH
%token WHITESPACE
%token NEWLINE

%token LEFT_OP
%token RIGHT_OP
%token INC_OP
%token DEC_OP
%token LE_OP
%token GE_OP
%token EQ_OP
%token NE_OP
%token LOGICAL_AND_OP
%token LOGICAL_OR_OP
%token LOGICAL_XOR_OP
%token MUL_ASSIGN
%token DIV_ASSIGN
%token ADD_ASSIGN
%token MOD_ASSIGN
%token LEFT_ASSIGN
%token RIGHT_ASSIGN
%token AND_ASSIGN
%token XOR_ASSIGN
%token OR_ASSIGN
%token SUB_ASSIGN
%token LEFT_PAREN
%token RIGHT_PAREN
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_BRACE
%token RIGHT_BRACE
%token DOT
%token COMMA
%token COLON
%token EQUAL
%token SEMICOLON
%token BANG
%token DASH
%token TILDE
%token PLUS
%token STAR
%token SLASH
%token PERCENT
%token LEFT_ANGLE
%token RIGHT_ANGLE
%token BITWISE_OR_OP
%token BITWISE_XOR_OP
%token BITWISE_AND_OP
%token QUESTION

%token INTRINSIC_TEXTURE_2D_BIAS
%token INTRINSIC_TEXTURE_2D_LOD
%token INTRINSIC_TEXTURE_CUBE_BIAS
%token INTRINSIC_TEXTURE_CUBE_LOD
%token INTRINSIC_RSQRT
%token INTRINSIC_RCP
%token INTRINSIC_LOG2
%token INTRINSIC_EXP2
%token INTRINSIC_CEIL
%token INTRINSIC_FLOOR
%token INTRINSIC_SIGN
%token INTRINSIC_TRUNC
%token INTRINSIC_NEAREST
%token INTRINSIC_MIN
%token INTRINSIC_MAX
%token INTRINSIC_MINABS
%token INTRINSIC_MAXABS

%token DEFINE
%token UNDEF
%token IFDEF
%token IFNDEF
%token ELIF
%token ENDIF
%token _ERROR
%token PRAGMA
%token EXTENSION
%token VERSION
%token LINE

%token ALL
%token REQUIRE
%token ENABLE
%token WARN
%token DISABLE

%token ATTRIBUTE
%token _CONST
%token _BOOL
%token _FLOAT
%token _INT
%token BREAK
%token CONTINUE
%token DO
%token ELSE
%token FOR
%token IF
%token DISCARD
%token RETURN
%token BVEC2
%token BVEC3
%token BVEC4
%token IVEC2
%token IVEC3
%token IVEC4
%token VEC2
%token VEC3
%token VEC4
%token _MAT2
%token MAT3
%token MAT4
%token _IN
%token _OUT
%token INOUT
%token UNIFORM
%token VARYING
%token SAMPLER2D
%token SAMPLEREXTERNAL
%token SAMPLERCUBE
%token STRUCT
%token _VOID
%token WHILE
%token INVARIANT
%token HIGH_PRECISION
%token MEDIUM_PRECISION
%token LOW_PRECISION
%token PRECISION

%token ASM
%token CLASS
%token UNION
%token ENUM
%token TYPEDEF
%token TEMPLATE
%token THIS
%token PACKED
%token GOTO
%token SWITCH
%token DEFAULT
%token _INLINE
%token NOINLINE
%token VOLATILE
%token PUBLIC
%token STATIC
%token EXTERN
%token EXTERNAL
%token INTERFACE
%token FLAT
%token _LONG
%token _SHORT
%token DOUBLE
%token HALF
%token _FIXED
%token _UNSIGNED
%token SUPERP
%token _INPUT
%token OUTPUT
%token HVEC2
%token HVEC3
%token HVEC4
%token DVEC2
%token DVEC3
%token DVEC4
%token FVEC2
%token FVEC3
%token FVEC4
%token SAMPLER1D
%token SAMPLER3D
%token SAMPLER1DSHADOW
%token SAMPLER2DSHADOW
%token SAMPLER2DRECT
%token SAMPLER3DRECT
%token SAMPLER2DRECTSHADOW
%token SIZEOF
%token CAST
%token NAMESPACE
%token USING

%token _TRUE
%token _FALSE

%token<lookup> CANDIDATE_TYPE_NAME

%type<lookup> variable_identifier
%type<expr> primary_expression
%type<expr> postfix_expression
%type<expr> integer_expression

%type<expr> function_call
%type<call_context> function_call_generic
%type<call_context> function_call_header_no_parameters
%type<call_context> function_call_header_with_parameters
%type<call_context> function_call_header
%type<call_context> function_identifier
%type<call_context> intrinsic_identifier
%type<call_context> constructor_identifier

%type<expr> unary_expression
%type<expr> multiplicative_expression
%type<expr> additive_expression
%type<expr> shift_expression
%type<expr> relational_expression
%type<expr> equality_expression
%type<expr> and_expression
%type<expr> exclusive_or_expression
%type<expr> inclusive_or_expression
%type<expr> logical_and_expression
%type<expr> logical_xor_expression
%type<expr> logical_or_expression
%type<expr> conditional_expression
%type<expr> assignment_expression
%type<expr_flavour> assignment_operator
%type<expr> expression
%type<expr> constant_expression

%type<statement> declaration

%type<symbol> function_prototype

%type<statement_chain> init_declarator_list
%type<statement_chain> init_declarator

%type<expr> initializer

%type<statement> declaration_statement
%type<statement> statement
%type<statement> simple_statement
%type<statement> compound_statement
%type<statement> statement_with_scope
%type<statement> statement_no_new_scope
%type<statement> compound_statement_no_new_scope
%type<statement_chain> statement_list
%type<statement> expression_statement
%type<statement> selection_statement
%type<statement2> selection_rest_statement
%type<statement> condition

%type<statement> iteration_statement

%type<statement> for_init
%type<statement> for_test
%type<expr> for_loop

%type<statement> jump_statement

%type<statement> translation_unit
%type<statement> external_declaration
%type<statement> function_definition

%type<s> identifier_or_typename

%%

variable_identifier
      : IDENTIFIER { $$ = $1; }
      ;

primary_expression
      : variable_identifier               { $$ = glsl_expr_construct_instance($1.symbol); }
      | PPNUMBERF                         { $$ = glsl_expr_construct_value_float($1); }
      | PPNUMBERI                         { $$ = glsl_expr_construct_value_int($1); }
      | _TRUE                             { $$ = glsl_expr_construct_value_bool(CONST_BOOL_TRUE); }
      | _FALSE                            { $$ = glsl_expr_construct_value_bool(CONST_BOOL_FALSE); }
      | LEFT_PAREN expression RIGHT_PAREN { $$ = $2; }
      ;

postfix_expression
      : primary_expression                                               { $$ = $1; }
      | postfix_expression LEFT_BRACKET integer_expression RIGHT_BRACKET { $$ = glsl_expr_construct_subscript($1, $3); }
      | function_call                                                    { $$ = $1; }
      | postfix_expression DOT identifier_or_typename                    { $$ = glsl_expr_construct_field_selector($1, $3); }
      | postfix_expression INC_OP                                        { $$ = glsl_expr_construct_unary_op_arithmetic(EXPR_POST_INC, $1); }
      | postfix_expression DEC_OP                                        { $$ = glsl_expr_construct_unary_op_arithmetic(EXPR_POST_DEC, $1); }
      ;

integer_expression
      : expression { $$ = $1; }
      ;

function_call
      : function_call_generic {
                                 switch($1.flavour)
                                 {
                                    case CALL_CONTEXT_FUNCTION:
                                       $$ = glsl_expr_construct_function_call($1.u.function.symbol, $1.args);
                                       break;
                                    case CALL_CONTEXT_PRIM_CONSTRUCTOR:
                                       $$ = glsl_expr_construct_prim_constructor_call($1.u.prim_constructor.index, $1.args);
                                       break;
                                    case CALL_CONTEXT_TYPE_CONSTRUCTOR:
                                       $$ = glsl_expr_construct_type_constructor_call($1.u.type_constructor.symbol->type, $1.args);
                                       break;
                                    case CALL_CONTEXT_INTRINSIC:
                                       $$ = glsl_expr_construct_intrinsic($1.u.intrinsic.flavour, $1.args);
                                       break;
                                    default:
                                       UNREACHABLE();
                                       break;
                                 }
                              }
      ;

function_call_generic
      : function_call_header_with_parameters RIGHT_PAREN { $$ = $1; }
      | function_call_header_no_parameters RIGHT_PAREN   { $$ = $1; }
      ;

function_call_header_no_parameters
      : function_call_header _VOID { $$.args = glsl_expr_chain_create(); }
      | function_call_header       { $$.args = glsl_expr_chain_create(); }
      ;

function_call_header_with_parameters
      : function_call_header assignment_expression                       { $$.args = glsl_expr_chain_append(glsl_expr_chain_create(), $2); }
      | function_call_header_with_parameters COMMA assignment_expression { $$.args = glsl_expr_chain_append($1.args, $3); }
      ;

function_call_header
      : function_identifier LEFT_PAREN   { $$ = $1; }
      ;

function_identifier
      : constructor_identifier { $$ = $1; }
      | intrinsic_identifier   { $$ = $1; }
      | IDENTIFIER             { $$.flavour = CALL_CONTEXT_FUNCTION; $$.u.function.symbol = $1.symbol; }
      ;

// These are language extensions.
intrinsic_identifier
      : INTRINSIC_TEXTURE_2D_BIAS   { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_TEXTURE_2D_BIAS; }
      | INTRINSIC_TEXTURE_2D_LOD    { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_TEXTURE_2D_LOD; }
      | INTRINSIC_TEXTURE_CUBE_BIAS { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_TEXTURE_CUBE_BIAS; }
      | INTRINSIC_TEXTURE_CUBE_LOD  { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_TEXTURE_CUBE_LOD; }
      | INTRINSIC_RSQRT             { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_RSQRT; }
      | INTRINSIC_RCP               { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_RCP; }
      | INTRINSIC_LOG2              { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_LOG2; }
      | INTRINSIC_EXP2              { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_EXP2; }
      | INTRINSIC_CEIL              { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_CEIL; }
      | INTRINSIC_FLOOR             { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_FLOOR; }
      | INTRINSIC_SIGN              { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_SIGN; }
      | INTRINSIC_TRUNC             { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_TRUNC; }
      | INTRINSIC_NEAREST           { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_NEAREST; }
      | INTRINSIC_MIN               { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_MIN; }
      | INTRINSIC_MAX               { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_MAX; }
      | INTRINSIC_MINABS            { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_MINABS; }
      | INTRINSIC_MAXABS            { $$.flavour = CALL_CONTEXT_INTRINSIC; $$.u.intrinsic.flavour = EXPR_INTRINSIC_MAXABS; }
      ;

constructor_identifier
      : _FLOAT              { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_FLOAT; }
      | _INT                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_INT; }
      | _BOOL               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_BOOL; }
      | VEC2                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_VEC2; }
      | VEC3                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_VEC3; }
      | VEC4                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_VEC4; }
      | BVEC2               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_BVEC2; }
      | BVEC3               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_BVEC3; }
      | BVEC4               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_BVEC4; }
      | IVEC2               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_IVEC2; }
      | IVEC3               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_IVEC3; }
      | IVEC4               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_IVEC4; }
      | _MAT2               { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_MAT2; }
      | MAT3                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_MAT3; }
      | MAT4                { $$.flavour = CALL_CONTEXT_PRIM_CONSTRUCTOR; $$.u.prim_constructor.index = PRIM_MAT4; }
      | CANDIDATE_TYPE_NAME { $$.flavour = CALL_CONTEXT_TYPE_CONSTRUCTOR; $$.u.type_constructor.symbol = $1.symbol; }
      ;

unary_expression
      : postfix_expression      { $$ = $1; }
      | INC_OP unary_expression { $$ = glsl_expr_construct_unary_op_arithmetic(EXPR_PRE_INC, $2); }
      | DEC_OP unary_expression { $$ = glsl_expr_construct_unary_op_arithmetic(EXPR_PRE_DEC, $2); }
      | PLUS unary_expression   { $$ = $2; }
      | DASH unary_expression   { $$ = glsl_expr_construct_unary_op_arithmetic(EXPR_ARITH_NEGATE, $2); }
      | BANG unary_expression   { $$ = glsl_expr_construct_unary_op_logical(EXPR_LOGICAL_NOT, $2); }
      | TILDE unary_expression  { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

// Grammar Note: No traditional style type casts.
// Grammar Note: No '*' or '&' unary ops. Pointers are not supported.

multiplicative_expression
      : unary_expression                                   { $$ = $1; }
      | multiplicative_expression STAR unary_expression    { $$ = glsl_expr_construct_binary_op_arithmetic(EXPR_MUL, $1, $3); }
      | multiplicative_expression SLASH unary_expression   { $$ = glsl_expr_construct_binary_op_arithmetic(EXPR_DIV, $1, $3); }
      | multiplicative_expression PERCENT unary_expression { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

additive_expression
      : multiplicative_expression                          { $$ = $1; }
      | additive_expression PLUS multiplicative_expression { $$ = glsl_expr_construct_binary_op_arithmetic(EXPR_ADD, $1, $3); }
      | additive_expression DASH multiplicative_expression { $$ = glsl_expr_construct_binary_op_arithmetic(EXPR_SUB, $1, $3); }
      ;

shift_expression
      : additive_expression                           { $$ = $1; }
      | shift_expression LEFT_OP additive_expression  { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | shift_expression RIGHT_OP additive_expression { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

relational_expression
      : shift_expression                                   { $$ = $1; }
      | relational_expression LEFT_ANGLE shift_expression  { $$ = glsl_expr_construct_binary_op_relational(EXPR_LESS_THAN, $1, $3); }
      | relational_expression RIGHT_ANGLE shift_expression { $$ = glsl_expr_construct_binary_op_relational(EXPR_GREATER_THAN, $1, $3); }
      | relational_expression LE_OP shift_expression       { $$ = glsl_expr_construct_binary_op_relational(EXPR_LESS_THAN_EQUAL, $1, $3); }
      | relational_expression GE_OP shift_expression       { $$ = glsl_expr_construct_binary_op_relational(EXPR_GREATER_THAN_EQUAL, $1, $3); }
      ;

equality_expression
      : relational_expression                           { $$ = $1; }
      | equality_expression EQ_OP relational_expression { $$ = glsl_expr_construct_binary_op_equality(EXPR_EQUAL, $1, $3); }
      | equality_expression NE_OP relational_expression { $$ = glsl_expr_construct_binary_op_equality(EXPR_NOT_EQUAL, $1, $3); }
      ;

and_expression
      : equality_expression                               { $$ = $1; }
      | and_expression BITWISE_AND_OP equality_expression { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

exclusive_or_expression
      : and_expression                                        { $$ = $1; }
      | exclusive_or_expression BITWISE_XOR_OP and_expression { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

inclusive_or_expression
      : exclusive_or_expression                                       { $$ = $1; }
      | inclusive_or_expression BITWISE_OR_OP exclusive_or_expression { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

logical_and_expression
      : inclusive_or_expression                                       { $$ = $1; }
      | logical_and_expression LOGICAL_AND_OP inclusive_or_expression { $$ = glsl_expr_construct_binary_op_logical(EXPR_LOGICAL_AND, $1, $3); }
      ;

logical_xor_expression
      : logical_and_expression                                       { $$ = $1; }
      | logical_xor_expression LOGICAL_XOR_OP logical_and_expression { $$ = glsl_expr_construct_binary_op_logical(EXPR_LOGICAL_XOR, $1, $3); }
      ;

logical_or_expression
      : logical_xor_expression                                     { $$ = $1; }
      | logical_or_expression LOGICAL_OR_OP logical_xor_expression { $$ = glsl_expr_construct_binary_op_logical(EXPR_LOGICAL_OR, $1, $3); }
      ;

conditional_expression
      : logical_or_expression                                                 { $$ = $1; }
      | logical_or_expression QUESTION expression COLON assignment_expression { $$ = glsl_expr_construct_cond_op($1, $3, $5); }
      ;

assignment_expression
      : conditional_expression                                     { $$ = $1; }
      | unary_expression EQUAL assignment_expression               { $$ = glsl_expr_construct_assign_op($1, $3); }
      | unary_expression assignment_operator assignment_expression { $$ = glsl_expr_construct_assign_op($1, glsl_expr_construct_binary_op_arithmetic($2, $1, $3)); }
      ;

assignment_operator
      : MUL_ASSIGN   { $$ = EXPR_MUL; }
      | DIV_ASSIGN   { $$ = EXPR_DIV; }
      | ADD_ASSIGN   { $$ = EXPR_ADD; }
      | SUB_ASSIGN   { $$ = EXPR_SUB; }
      | MOD_ASSIGN   { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | LEFT_ASSIGN  { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | RIGHT_ASSIGN { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | AND_ASSIGN   { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | XOR_ASSIGN   { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      | OR_ASSIGN    { /* RESERVED */ glsl_compile_error(ERROR_LEXER_PARSER, 3, g_LineNumber, NULL); }
      ;

expression
      : assignment_expression                  { $$ = $1; }
      | expression COMMA assignment_expression { $$ = glsl_expr_construct_sequence($1, $3); }
      ;

constant_expression
      : conditional_expression { $$ = $1; }
      ;

declaration
      : function_prototype SEMICOLON                                   {
                                                                          if (!g_InGlobalScope)
                                                                             glsl_compile_error(ERROR_CUSTOM, 39, g_LineNumber, NULL);
                                                                          // Don't store these in the AST.
                                                                          $$ = glsl_statement_construct_decl_list(glsl_statement_chain_create());
                                                                       }
      | init_declarator_list SEMICOLON                                 {
                                                                          $$ = glsl_statement_construct_decl_list($1);
                                                                       }
      | PRECISION precision_qualifier type_specifier_no_prec SEMICOLON {
                                                                          // "Precision not supported" warning already issued.
                                                                          // Don't store these in the AST.
                                                                          $$ = glsl_statement_construct_decl_list(glsl_statement_chain_create());
                                                                       }
      ;

function_prototype
      : function_declarator RIGHT_PAREN { glsl_build_function_type(); glsl_commit_singleton_function_declaration(g_FunctionBuilderName); $$ = g_LastInstance; }
      ;

// We cannot find (void) functions here due to shift/reduce conflicts with nameless parameters in function declarations.
// Instead we will convert (void) to the canonical form () in glsl_build_function_type().

function_declarator
      : function_header                 { /* Nothing to do here. */ }
      | function_header_with_parameters { /* Nothing to do here. */ }
      ;

function_header_with_parameters
      : function_header parameter_declaration                       { /* Nothing to do here. */ }
      | function_header_with_parameters COMMA parameter_declaration { /* Nothing to do here. */ }
      ;

function_header
      : fully_specified_type identifier_or_typename LEFT_PAREN
         {
            g_FunctionBuilderReturnType = g_TypeBuilder;
            if (g_TypeQual != TYPE_QUAL_DEFAULT)
               glsl_compile_error(ERROR_CUSTOM, 40, g_LineNumber, NULL);
            g_FunctionBuilderName = $2;
            glsl_reinit_function_builder();
         }
      | fully_specified_type LEFT_BRACKET constant_expression RIGHT_BRACKET identifier_or_typename LEFT_PAREN { glsl_compile_error(ERROR_SEMANTIC, 41, g_LineNumber, NULL); }
      ;

parameter_declaration
      : type_and_parameter_qualifier type_specifier identifier_or_typename                                                { glsl_commit_singleton_function_param($3); }
      | type_and_parameter_qualifier type_specifier identifier_or_typename LEFT_BRACKET constant_expression RIGHT_BRACKET { glsl_commit_array_function_param($3, $5); }
      | type_and_parameter_qualifier type_specifier                                                                       { glsl_commit_singleton_function_param(generate_anon_param()); }
      | type_and_parameter_qualifier type_specifier LEFT_BRACKET constant_expression RIGHT_BRACKET                        { glsl_commit_array_function_param(generate_anon_param(), $4); }
      ;

type_and_parameter_qualifier
      : type_qualifier parameter_qualifier { /* Nothing to do here. */ }
      | parameter_qualifier                { g_TypeQual = TYPE_QUAL_DEFAULT; }
      ;

parameter_qualifier
      : /* empty */ { g_ParamQual = PARAM_QUAL_DEFAULT; }
      | _IN          { g_ParamQual = PARAM_QUAL_IN; }
      | _OUT         { g_ParamQual = PARAM_QUAL_OUT; }
      | INOUT       { g_ParamQual = PARAM_QUAL_INOUT; }
      ;

// This rule should return a StatementChain* of STATEMENT_VAR_DECL.
init_declarator_list
      : init_declarator                                                                                  {
                                                                                                            $$ = $1;
                                                                                                         }
      | init_declarator_list COMMA identifier_or_typename                                                {
                                                                                                            glsl_commit_singleton_variable_instance($3, NULL);
                                                                                                            $$ = glsl_statement_chain_append($1, glsl_statement_construct_var_decl(g_LastInstance, NULL));
                                                                                                         }
      | init_declarator_list COMMA identifier_or_typename LEFT_BRACKET constant_expression RIGHT_BRACKET {
                                                                                                            glsl_commit_array_instance($3, $5);
                                                                                                            $$ = glsl_statement_chain_append($1, glsl_statement_construct_var_decl(g_LastInstance, NULL));
                                                                                                         }
      | init_declarator_list COMMA identifier_or_typename EQUAL initializer                              {
                                                                                                            glsl_commit_singleton_variable_instance($3, $5);
                                                                                                            $$ = glsl_statement_chain_append($1, glsl_statement_construct_var_decl(g_LastInstance, $5));
                                                                                                         }
      ;

// This rule should return a StatementChain* of STATEMENT_VAR_DECL to init_declarator_list, possibly to insert more definitions.
init_declarator
      : fully_specified_type                                                                       {
                                                                                                      // This is to match struct declarations, but unfortunately it also admits rubbish like "int , x".
                                                                                                      $$ = glsl_statement_chain_create();
                                                                                                   }
      | fully_specified_type identifier_or_typename                                                {
                                                                                                      glsl_commit_singleton_variable_instance($2, NULL);
                                                                                                      $$ = glsl_statement_chain_append(glsl_statement_chain_create(), glsl_statement_construct_var_decl(g_LastInstance, NULL));
                                                                                                   }
      | fully_specified_type identifier_or_typename LEFT_BRACKET constant_expression RIGHT_BRACKET {
                                                                                                      glsl_commit_array_instance($2, $4);
                                                                                                      $$ = glsl_statement_chain_append(glsl_statement_chain_create(), glsl_statement_construct_var_decl(g_LastInstance, NULL));
                                                                                                   }
      | fully_specified_type identifier_or_typename EQUAL initializer                              {
                                                                                                      glsl_commit_singleton_variable_instance($2, $4);
                                                                                                      $$ = glsl_statement_chain_append(glsl_statement_chain_create(), glsl_statement_construct_var_decl(g_LastInstance, $4));
                                                                                                   }
      | INVARIANT identifier_or_typename                                                           {
                                                                                                      $$ = glsl_statement_chain_create();
                                                                                                   }
      ;

// Grammar Note: No 'enum', or 'typedef'.

fully_specified_type
      : type_specifier                { g_TypeQual = TYPE_QUAL_DEFAULT; }
      | type_qualifier type_specifier
      ;

type_qualifier
      : _CONST            { g_TypeQual = TYPE_QUAL_CONST; }
      | ATTRIBUTE         { g_TypeQual = TYPE_QUAL_ATTRIBUTE; }
      | VARYING           { g_TypeQual = TYPE_QUAL_VARYING; }
      | INVARIANT VARYING { g_TypeQual = TYPE_QUAL_INVARIANT_VARYING; }
      | UNIFORM           { g_TypeQual = TYPE_QUAL_UNIFORM; }
      ;

type_specifier
      : type_specifier_no_prec
      | precision_qualifier type_specifier_no_prec
      ;

type_specifier_no_prec
      : _VOID               { g_TypeBuilder = &primitiveTypes[PRIM_VOID]; }
      | _FLOAT              { g_TypeBuilder = &primitiveTypes[PRIM_FLOAT]; }
      | _INT                { g_TypeBuilder = &primitiveTypes[PRIM_INT]; }
      | _BOOL               { g_TypeBuilder = &primitiveTypes[PRIM_BOOL]; }
      | VEC2                { g_TypeBuilder = &primitiveTypes[PRIM_VEC2]; }
      | VEC3                { g_TypeBuilder = &primitiveTypes[PRIM_VEC3]; }
      | VEC4                { g_TypeBuilder = &primitiveTypes[PRIM_VEC4]; }
      | BVEC2               { g_TypeBuilder = &primitiveTypes[PRIM_BVEC2]; }
      | BVEC3               { g_TypeBuilder = &primitiveTypes[PRIM_BVEC3]; }
      | BVEC4               { g_TypeBuilder = &primitiveTypes[PRIM_BVEC4]; }
      | IVEC2               { g_TypeBuilder = &primitiveTypes[PRIM_IVEC2]; }
      | IVEC3               { g_TypeBuilder = &primitiveTypes[PRIM_IVEC3]; }
      | IVEC4               { g_TypeBuilder = &primitiveTypes[PRIM_IVEC4]; }
      | _MAT2               { g_TypeBuilder = &primitiveTypes[PRIM_MAT2]; }
      | MAT3                { g_TypeBuilder = &primitiveTypes[PRIM_MAT3]; }
      | MAT4                { g_TypeBuilder = &primitiveTypes[PRIM_MAT4]; }
      | SAMPLER2D           { g_TypeBuilder = &primitiveTypes[PRIM_SAMPLER2D]; }
      | SAMPLEREXTERNAL     {
        switch (glsl_ext_status(GLSL_EXT_IMAGE_EXTERNAL)) {
        case GLSL_ENABLED_WARN:
            glsl_compile_error(WARNING, 2, g_LineNumber, NULL);
        case GLSL_ENABLED:
            g_TypeBuilder = &primitiveTypes[PRIM_SAMPLEREXTERNAL];
            break;
        case GLSL_DISABLED:
            glsl_compile_error(_ERROR, 1, g_LineNumber, NULL);
            break;
        }
      }
      | SAMPLERCUBE         { g_TypeBuilder = &primitiveTypes[PRIM_SAMPLERCUBE]; }
      | struct_specifier    { /* g_TypeBuilder already set. */ }
      | CANDIDATE_TYPE_NAME { g_TypeBuilder = $1.symbol->type; }
      ;

// Ignore - everything is high precision.
precision_qualifier
      : HIGH_PRECISION   {  }
      | MEDIUM_PRECISION {  }
      | LOW_PRECISION    {  }
      ;

// Note the struct grammar has been changed from the spec to match in the same way as the normal declarations.
// This allows the type to be known before identifiers are introduced.
struct_specifier
      : STRUCT { glsl_reinit_struct_builder(); } identifier_or_typename LEFT_BRACE struct_declaration_list RIGHT_BRACE { glsl_build_struct_type(); glsl_commit_struct_type($3); }
      | STRUCT { glsl_reinit_struct_builder(); } LEFT_BRACE struct_declaration_list RIGHT_BRACE                        { glsl_build_struct_type(); }
      ;

struct_declaration_list
      : struct_declaration
      | struct_declaration_list struct_declaration
      ;

struct_declaration
      : struct_declarator_list SEMICOLON
      ;

struct_declarator_list
      : struct_declarator
      | struct_declarator_list COMMA identifier_or_typename                                                { glsl_commit_singleton_struct_member($3); }
      | struct_declarator_list COMMA identifier_or_typename LEFT_BRACKET constant_expression RIGHT_BRACKET { glsl_commit_array_struct_member($3, $5); }
      ;

struct_declarator
      : type_specifier identifier_or_typename                                                { glsl_commit_singleton_struct_member($2); }
      | type_specifier identifier_or_typename LEFT_BRACKET constant_expression RIGHT_BRACKET { glsl_commit_array_struct_member($2, $4); }
      ;

initializer
      : assignment_expression { $$ = $1; }
      ;

declaration_statement
      : declaration { $$ = $1; }
      ;

statement
      : compound_statement { $$ = $1; }
      | simple_statement   { $$ = $1; }
      ;

// Grammar Note: No labeled statements; 'goto' is not supported.

simple_statement
      : declaration_statement { $$ = $1; }
      | expression_statement  { $$ = $1; }
      | selection_statement   { $$ = $1; }
      | iteration_statement   { $$ = $1; }
      | jump_statement        { $$ = $1; }
      ;

compound_statement
      : LEFT_BRACE RIGHT_BRACE                                   { $$ = glsl_statement_construct_compound(glsl_statement_chain_create()); }
      | LEFT_BRACE { glsl_enter_scope(); } statement_list RIGHT_BRACE { glsl_exit_scope(); $$ = glsl_statement_construct_compound($3); }
      ;

statement_with_scope
      : { glsl_enter_scope(); } compound_statement_no_new_scope { $$ = $2; glsl_exit_scope(); }
      | { glsl_enter_scope(); } simple_statement                { $$ = $2; glsl_exit_scope(); }

statement_no_new_scope
      : compound_statement_no_new_scope { $$ = $1; }
      | simple_statement                { $$ = $1; }
      ;

compound_statement_no_new_scope
      : LEFT_BRACE RIGHT_BRACE                { $$ = glsl_statement_construct_compound(glsl_statement_chain_create()); }
      | LEFT_BRACE statement_list RIGHT_BRACE { $$ = glsl_statement_construct_compound($2); }
      ;

statement_list
      : statement                { $$ = glsl_statement_chain_append(glsl_statement_chain_create(), $1); }
      | statement_list statement { $$ = glsl_statement_chain_append($1, $2); }
      ;

expression_statement
      : SEMICOLON            { $$ = glsl_statement_construct(STATEMENT_NULL); }
      | expression SEMICOLON { $$ = glsl_statement_construct_expr($1); }
      ;

selection_statement
      : IF LEFT_PAREN expression RIGHT_PAREN selection_rest_statement { $$ = glsl_statement_construct_selection($3, $5.a, $5.b); }
      ;

selection_rest_statement
      : statement_with_scope ELSE statement_with_scope { $$.a = $1; $$.b = $3; }
      | statement_with_scope                           { $$.a = $1; $$.b = NULL; }
      ;

// Grammar Note: No 'switch'. Switch statements not supported.

condition
      : expression                                                    { $$ = glsl_statement_construct_expr($1); }
      | fully_specified_type identifier_or_typename EQUAL initializer { glsl_commit_singleton_variable_instance($2, $4); $$ = glsl_statement_construct_var_decl(g_LastInstance, $4); }
      ;

iteration_statement
      : WHILE LEFT_PAREN { glsl_enter_scope(); } condition RIGHT_PAREN { enter_loop(); } statement_no_new_scope { exit_loop(); glsl_exit_scope(); $$ = glsl_statement_construct_iterator_while($4, $7); }
      | DO { enter_loop(); } statement_with_scope { exit_loop(); } WHILE LEFT_PAREN expression RIGHT_PAREN SEMICOLON { $$ = glsl_statement_construct_iterator_do_while($3, $7); }
      | FOR LEFT_PAREN { glsl_enter_scope(); } for_init SEMICOLON for_test SEMICOLON for_loop RIGHT_PAREN { enter_loop(); } statement_no_new_scope { exit_loop(); glsl_exit_scope(); $$ = glsl_statement_construct_iterator_for($4, $6, $8, $11); }
      ;

induction_type_specifier
      : induction_type_specifier_no_prec
      | precision_qualifier induction_type_specifier_no_prec
      ;

induction_type_specifier_no_prec
      : _FLOAT { g_TypeBuilder = &primitiveTypes[PRIM_FLOAT]; }
      | _INT  { g_TypeBuilder = &primitiveTypes[PRIM_INT]; }
      ;

for_init
      : induction_type_specifier identifier_or_typename EQUAL initializer {
                                                                             g_TypeQual = TYPE_QUAL_LOOP_INDEX;

                                                                             glsl_commit_singleton_variable_instance($2, $4);
                                                                             $$ = glsl_statement_construct_var_decl(g_LastInstance, $4);
                                                                          }
      | expression                                                    { $$ = glsl_statement_construct_expr($1); }

      | /* empty */                                                   { $$ = NULL; }
      ;

for_test
      : condition

      | /* empty */                                                   { $$ = NULL; }

      ;

for_loop
      : expression
      | /* empty */                                                   { $$ = NULL; }
      ;

jump_statement
      : CONTINUE SEMICOLON          { if (!in_loop())
                                         glsl_compile_error(ERROR_CUSTOM, 29, g_LineNumber, NULL);
                                      $$ = glsl_statement_construct(STATEMENT_CONTINUE); }
      | BREAK SEMICOLON             { if (!in_loop())
                                         glsl_compile_error(ERROR_CUSTOM, 29, g_LineNumber, NULL);
                                      $$ = glsl_statement_construct(STATEMENT_BREAK); }
      | RETURN SEMICOLON            { $$ = glsl_statement_construct(STATEMENT_RETURN); }
      | RETURN expression SEMICOLON { $$ = glsl_statement_construct_return_expr($2); }
      | DISCARD SEMICOLON           { if (SHADER_FRAGMENT != g_ShaderFlavour) glsl_compile_error(ERROR_CUSTOM, 12, g_LineNumber, NULL); $$ = glsl_statement_construct(STATEMENT_DISCARD); }
      ;

// Grammar Note: No 'goto'. Gotos are not supported.

translation_unit
      : external_declaration                  {
                                                 StatementChain* chain = glsl_statement_chain_create();
                                                 glsl_statement_chain_append(chain, $1);
                                                 $$ = glsl_statement_construct_ast(chain);
                                                 *(Statement**)top_level_statement = $$; // Save for calling function.
                                              }
      | translation_unit external_declaration {
                                                 StatementChain* chain = $1->u.ast.decls;
                                                 glsl_statement_chain_append(chain, $2);
                                                 $$ = $1;
                                                 *(Statement**)top_level_statement = $$; // Save for calling function.
                                              }
      ;

external_declaration
      : function_definition { $$ = $1; }
      | declaration         { $$ = $1; }
      ;

function_definition
      : function_prototype { glsl_enter_scope(); g_InGlobalScope = false; glsl_instantiate_function_params(g_TypeBuilder); } compound_statement_no_new_scope { glsl_exit_scope(); g_InGlobalScope = true; $$ = glsl_statement_construct_function_def($1, $3); glsl_insert_function_definition($$); }
      ;

// This is a bit of a hack, but it accounts for over-eager Lex matching as CANDIDATE_TYPE_NAME instead of IDENTIFIER.
identifier_or_typename
      : IDENTIFIER    { $$ = $1.name; }
      | CANDIDATE_TYPE_NAME { $$ = $1.name; }
      ;
