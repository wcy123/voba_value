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
voba_value_t voba_make_hash();
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v);
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k);
voba_value_t voba_make_symbol_cpp(voba_str_t * symbol_name, voba_value_t symbol_table);
voba_value_t voba_make_symbol_table_cpp();
void    voba_throw_exception(voba_value_t v);


#include "data_type_imp.c"
#ifdef __cplusplus
}
#endif

/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
