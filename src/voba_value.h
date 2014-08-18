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
void    voba_throw_exception(voba_value_t v);
#include "data_type_imp.c"
#ifdef __cplusplus
}
#endif



// some useful macros
#define VOBA_FUNC __attribute__((aligned(16)))


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
