/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#pragma once

#include "glsl_common.h"
#include "glsl_symbol_table.h"
#include "glsl_layout.h"

QualList *qual_list_new(Qualifier *head);
QualList *qual_list_append(QualList *list, Qualifier *q);

Qualifier *new_qual_storage(StorageQualifier q);
Qualifier *new_qual_auxiliary(AuxiliaryQualifier q);
Qualifier *new_qual_memory(MemoryQualifier q);
Qualifier *new_qual_layout(LayoutIDList *q);
Qualifier *new_qual_prec(PrecisionQualifier q);
Qualifier *new_qual_interp(InterpolationQualifier q);
Qualifier *new_qual_invariant(void);
Qualifier *new_qual_precise(void);

void qualifiers_from_list(Qualifiers *q, QualList *l);
void param_quals_from_list(Qualifiers *q, ParamQualifier *param_qual, QualList *l);
void qualifiers_process_default(QualList *q_in, SymbolTable *s, DeclDefaultState *dflt);
