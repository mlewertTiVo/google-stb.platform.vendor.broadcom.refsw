/*=============================================================================
Copyright (c) 2014 Broadcom Europe Limited.
All rights reserved.

Project  :  prepro
Module   :

FILE DESCRIPTION
=============================================================================*/

#include "glsl_common.h"
#include "glsl_globals.h"

#include "prepro/glsl_prepro_eval.h"
#include "prepro/glsl_prepro_expand.h"
#include "prepro/glsl_prepro_directive.h"

#include "glsl_extensions.h"
#include "glsl_errors.h"
#include "glsl_symbol_table.h"

#include <string.h>
#include <stdio.h>

#if defined(_WIN32)
#define strcasecmp _stricmp
#endif

//#define DEBUG_PREPRO

extern TokenData pptoken;
extern int pplex(void);

MacroList *directive_macros;

static bool allow_directive;
static bool allow_extension;
static bool allow_version;

#define MAX_DEPTH 32

static struct {
   bool active;
   bool sticky;
   bool seen_else;
} if_stack[MAX_DEPTH];

static int depth;

void glsl_init_preprocessor(void)
{
   directive_macros = glsl_macrolist_construct_initial();

   allow_directive = true;
   allow_extension = true;
   allow_version = true;
   depth = 0;
}

void glsl_directive_reset_macros()
{
   directive_macros = NULL;
}

static void push(bool active)
{
   if (depth == MAX_DEPTH) {
      /* implementation-specific error */
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "excessive nesting of #if directives");
   }

   if_stack[depth].active = active;
   if_stack[depth].sticky = active;
   if_stack[depth].seen_else = false;

#ifdef DEBUG_PREPRO
   printf("push: depth = %d, active = %s\n", depth, active ? "true" : "false");
#endif

   depth++;
}

static void pop(void)
{
   if (depth == 0)
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "unexpected #endif");

   depth--;

#ifdef DEBUG_PREPRO
   printf("pop: depth = %d, active = %s, sticky = %s, seen_else = %s\n", depth, if_stack[depth].active ? "true" : "false", if_stack[depth].sticky ? "true" : "false", if_stack[depth].seen_else ? "true" : "false");
#endif
}

static bool is_active(int delta)
{
   int i;

   for (i = 0; i < depth - delta; i++)
      if (!if_stack[i].active)
         return false;

   return true;
}

static bool allow_else(void)
{
   return depth > 0 && !if_stack[depth - 1].seen_else;
}

static bool has_sticky(void)
{
   return if_stack[depth - 1].sticky;
}

static void seen_else(void)
{
   assert(depth > 0);

   if_stack[depth - 1].seen_else = true;
}

static void set_active(bool active)
{
   bool sticky = if_stack[depth - 1].sticky;

   assert(depth > 0);

   if_stack[depth - 1].active = !sticky && active;
   if_stack[depth - 1].sticky = sticky || active;
}

static TokenType get_type(bool skip)
{
   TokenType t;
   do {
      t = (TokenType)pplex();
   } while (t == WHITESPACE && skip);
   return t;
}

static Token *get_token(bool skip)
{
   TokenType t = get_type(skip);          // TODO: end of file handling

   return glsl_token_construct(t, pptoken);
}

static Token *get_identifier(void)
{
   Token *token = get_token(true);

   if (!is_pp_identifier(token))
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "expected identifier");

   return token;
}

static TokenList *get_identifier_list(void)
{
   TokenList *list = NULL;

   Token *token = get_token(true);

   if (!is_rparen(token)) {
      TokenType type;

      if (!is_pp_identifier(token))
         glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "expected identifier");

      list = glsl_tokenlist_construct(token, list);

      for (type = get_type(true); type != RIGHT_PAREN; type = get_type(true)) {
         if (type != COMMA)
            glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "expected comma");

         token = get_token(true);

         if (!is_pp_identifier(token))
            glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "expected identifier");

         if (glsl_tokenlist_contains(list, token))
            glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "duplicate formal parameter in macro definition");

         list = glsl_tokenlist_construct(token, list);
      }
   }

   return list;
}

static TokenSeq *get_remainder(bool skip)
{
   TokenSeq *seq = NULL;
   Token *token;

   // skip initial whitespace:
   for ( token = get_token(true); !is_newline(token); token = get_token(skip)) {
      // merge consequtive whitespaces into single whitespace token
      if (seq != NULL && seq->token->type == WHITESPACE && token->type == WHITESPACE)
         continue;
      seq = glsl_tokenseq_construct(token, NULL, seq);
   }

   seq = glsl_tokenseq_destructive_reverse(seq, NULL);

   return seq;
}

static void skip_remainder(void)
{
   while (get_type(true) != NEWLINE);           // TODO: end of file handling
}

static void check_no_remainder(void)
{
   if (get_type(true) != NEWLINE)
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "garbage at end of preprocessor directive");
}

/*
   # define identifier replacement-list new-line
   # define identifier ( identifier-listopt ) replacement-list new-line
   # define identifier ( lparen ... ) replacement-list new-line
   # define identifier ( lparen identifier-list , ... ) replacement-list new-line

   function-like macro definitions may not have whitespace between the name and left parenthesis
*/
static void check_valid_name(const char *s)
{
   if (strlen(s) >= 3 && !strncmp(s, "GL_", 3))
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "reserved macro name: %s", s);
}

static void parse_define(void)
{
   Token *name = get_identifier();

   Macro *prev = glsl_macrolist_find(directive_macros, name);
   Macro *curr = NULL;

   if (name->type == IDENTIFIER) { check_valid_name(name->data.s); }

   Token *first = get_token(false);
   switch (first->type) {
   case WHITESPACE:
      curr = glsl_macro_construct_object(name, get_remainder(false)); // presence of whitespace matters, hence don't skip it
      break;
   case NEWLINE:
      curr = glsl_macro_construct_object(name, NULL);
      break;
   case LEFT_PAREN:
   {
      TokenList *args = get_identifier_list();
      TokenSeq *body = get_remainder(false); // presence of whitespace matters, hence don't skip it

      curr = glsl_macro_construct_function(name, args, body);
      break;
   }
   default:
      {
         TokenSeq *rem = get_remainder(false);
         rem = glsl_tokenseq_construct(first, NULL, rem);
         curr = glsl_macro_construct_object(name, rem);
         break;
      }
   }

   if (!prev || glsl_macro_equals(prev, curr))
      directive_macros = glsl_macrolist_construct(curr, directive_macros);
   else
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "inconsistent macro redefinition");
}

/* # undef identifier new-line */
static void parse_undef(void)
{
   Token *name = get_identifier();

   /* Check that this doesn't undef a builtin */
   Macro *prev = glsl_macrolist_find(directive_macros, name);
   if (prev != NULL && glsl_macro_is_builtin(prev))
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "Cannot undef builtin: %s", name->data.s);

   directive_macros = glsl_macrolist_construct(glsl_macro_construct_undef(name), directive_macros);

   check_no_remainder();
}

/* # if constant-expression new-line */
static void parse_if(void)
{
   if (is_active(0)) {
      TokenSeq *seq = get_remainder(true);
      TokenSeq *rem;

      g_LineNumber--;         // compensate for the fact that we've now fetched the line feed

      push(glsl_eval_evaluate(glsl_expand(glsl_remove_defined(seq), true), &rem));

      if (rem)
         glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "garbage at end of preprocessor directive");

      g_LineNumber++;
   } else {
      skip_remainder();

      push(false);
   }
}

/* # ifdef identifier new-line */
static void parse_ifdef(void)
{
   if (is_active(0)) {
      push(glsl_is_defined(directive_macros, get_identifier()));

      check_no_remainder();
   } else {
      push(false);

      skip_remainder();
   }
}

/* # ifndef identifier new-line */
static void parse_ifndef(void)
{
   if (is_active(0)) {
      push(!glsl_is_defined(directive_macros, get_identifier()));

      check_no_remainder();
   } else {
      push(false);

      skip_remainder();
   }
}

/* # else new-line */
static void parse_else(void)
{
   if (is_active(1)) {
      if (!allow_else())
         glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "unexpected #else");

      seen_else();

      if (has_sticky()) {
         set_active(false);
      } else {
         set_active(true);
      }

      check_no_remainder();
   } else
      skip_remainder();
}

/* # elif constant-expression new-line */
static void parse_elif(void)
{
   if (is_active(1)) {
      TokenSeq *seq = get_remainder(true);

      if (!allow_else())
         glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "unexpected #elif");

      if (has_sticky())
         set_active(false);
      else {
         TokenSeq *rem;
         g_LineNumber--;         // compensate for the fact that we've now fetched the line feed

         set_active(glsl_eval_evaluate(glsl_expand(glsl_remove_defined(seq), true), &rem));

         if (rem)
            glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "garbage at end of preprocessor directive");

         g_LineNumber++;
      }
   } else
      skip_remainder();
}

/* # endif new-line */
static void parse_endif(void)
{
   pop();

   if (is_active(0))
      check_no_remainder();
   else
      skip_remainder();
}

static void parse_pragma(void)
{
   /* This implementation defines no pragmas, so ignore them all */
   skip_remainder();
}

static void expect_colon(void)
{
   TokenType type = get_type(true);

   if (type != COLON)
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "expected colon");
}

extern void glsl_include(const char *s);

static void parse_extension(void)
{
    Token *tok;
    enum glsl_ext ext = GLSL_EXT_NOT_SUPPORTED;
    const char *contents;
    bool enabled = false;

    if (!allow_extension)
       glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "Invalid extension enable");

    tok = get_token(true);

    switch (tok->type) {
    case IDENTIFIER:
        ext = glsl_ext_lookup(tok->data.s);
        break;
    case ALL:
        ext = GLSL_EXT_ALL;
        break;
    default:
        glsl_compile_error(ERROR_PREPROCESSOR,
                1, g_LineNumber, "expected identifier");
    }

    expect_colon();

    switch (get_type(true)) {
    case REQUIRE:
        if (!glsl_ext_enable(ext, false)) {
            glsl_compile_error(ERROR_PREPROCESSOR, 3, g_LineNumber, NULL);
            break;
        }
        enabled = true;
        break;
    case ENABLE:
        if (ext == GLSL_EXT_ALL) {
            glsl_compile_error(ERROR_PREPROCESSOR, 3, g_LineNumber, NULL);
            break;
        }
        else if (!glsl_ext_enable(ext, false))
            glsl_compile_error(WARNING, 1, g_LineNumber, "%s", tok->data.s);
        enabled = true;
        break;
    case WARN:
        if (ext == GLSL_EXT_ALL) {
            glsl_ext_enable(ext, true);
            glsl_compile_error(WARNING, 1, g_LineNumber, NULL);
        }
        else if (!glsl_ext_enable(ext, true))
            glsl_compile_error(WARNING, 1, g_LineNumber, NULL);
        enabled = true;
        break;
    case DISABLE:
        if (!glsl_ext_disable(ext))
            glsl_compile_error(WARNING, 1, g_LineNumber, NULL);
        break;
    default:
        glsl_compile_error(ERROR_PREPROCESSOR,
                1, g_LineNumber, "expected require, enable, disable or warn");
        break;
    }

    check_no_remainder();

    if (enabled) {
        contents = glsl_ext_contents(ext);
        if (contents) glsl_include(contents);
    }
}

static void parse_line(void)
{
   TokenSeq *seq = get_remainder(true);
   TokenSeq *rem;

   g_LineNumber--;         // compensate for the fact that we've now fetched the line feed

   int line = glsl_eval_evaluate(glsl_expand(glsl_remove_defined(seq), true), &rem);

   if (rem)
      g_FileNumber = glsl_eval_evaluate(rem, &rem);

   if (rem)
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "garbage at end of preprocessor directive");

   g_LineNumber = line;
}

static bool is_conditional_directive(TokenType t) {
   return t == IF || t == IFDEF || t == IFNDEF || t == ELSE || t == ELIF || t == ENDIF;
}

static void parse_conditional_directive(TokenType t) {
   switch (t) {
   case IF:     parse_if();     return;
   case IFDEF:  parse_ifdef();  return;
   case IFNDEF: parse_ifndef(); return;
   case ELSE:   parse_else();   return;
   case ELIF:   parse_elif();   return;
   case ENDIF:  parse_endif();  return;
   default:
      unreachable();
      break;
   }
}

static void parse_directive()
{
   TokenType t = get_type(true);

   if (is_conditional_directive(t)) {
      parse_conditional_directive(t);
      return;
   }

   if (!is_active(0)) {
      skip_remainder();
      return;
   }

   switch (t) {
   case ERROR:
      glsl_compile_error(ERROR_PREPROCESSOR, 2, g_LineNumber, NULL);
      break;
   case DEFINE:
      parse_define();
      break;
   case UNDEF:
      parse_undef();
      break;
   case PRAGMA:
      parse_pragma();
      break;
   case EXTENSION:
      parse_extension();
      break;
   case VERSION:
      /* If declaring here is valid then we've already checked it, otherwise it's an error */
      if (!allow_version)
         glsl_compile_error(ERROR_PREPROCESSOR, 5, g_LineNumber, NULL);
      skip_remainder();
      break;
   case LINE:
      parse_line();
      break;
   case NEWLINE:
      break;
   default:
      glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "invalid preprocessor directive");
      break;
   }
}

TokenSeq *glsl_directive_next_token(void)
{
   while (true) {
      TokenType type = get_type(true);

      if (type == 0) {
         if (depth)
            glsl_compile_error(ERROR_PREPROCESSOR, 1, g_LineNumber, "unexpected end of file");

         return NULL;
      }

      // Eagerly expand __LINE__ to line number in effort to get accurate answer.
      // If __LINE__ is part of a macro, this code path will not be hit, and we will get the line number during macro expansion.
      if (type == IDENTIFIER && strcmp(pptoken.s, "__LINE__") == 0)
      {
         type = INTCONSTANT;
         pptoken.v = g_LineNumber;
      }

      if (allow_directive && type == HASH) {
         parse_directive();
         allow_version = false;
      } else {
         if (type == NEWLINE)
            allow_directive = true;
         else {
            allow_directive = false;
            allow_extension = false;
            allow_version   = false;

            if (is_active(0))
               return glsl_tokenseq_construct(glsl_token_construct(type, pptoken), NULL, NULL);
         }
      }
   }
}
