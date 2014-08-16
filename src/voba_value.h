#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <gc.h>
#include "inline.h"
#include "voba_str.h"
#include "data_type_imp.h"
vtype_t make_hash();
vtype_t hash_insert(vtype_t h, vtype_t k, vtype_t v);
vtype_t hash_find(vtype_t h, vtype_t k);
vtype_t make_symbol_cpp(voba_str_t * symbol_name, vtype_t symbol_table);
vtype_t make_symbol_table_cpp();
void    throw_exception(vtype_t v);


#include "data_type_imp.c"
#ifdef __cplusplus
}
#endif

/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
