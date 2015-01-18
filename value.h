/** @file */
/** \mainpage Voba value system, the basic data structure

 * \section Overview
 *
 * This is a rich value system to representing a lot of useful data
 * structure, e.g. list, array, hash etc.  This is the corner stone
 * for the language `voba`, but it could be used independently. see
 * overview.h
 * 
 * There are 8 basic types:
 *   - function, see fun.h
 *   - string, see str.h
 *   - symbol, see symbol.h
 *   - tuple, see tuple.h
 *   - closure, see closure.h
 *   - pair, see pair.h
 *   - user defined, see usr.h
 *      - cls, see cls.h
 *      - symbol table, see symbol_table.h
 *      - generic function, see gf.h
 *      - list array view, la, see la.h
 *      - generator, generator.h
 *      - symbol table, see symbol.h
 *      - other user defined object
 *   - small types, see small.h
 * 
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#ifdef USE_BDW_GC
#include <gc.h>
#else
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_REALLOC realloc
#define GC_FREE free
#define GC_strdup strdup
#define GC
#endif
#ifndef INLINE
#define INLINE static inline 
#endif
#include "imp/overview.h"
/* FOR_EACH macro used by tuple and array */
#include "imp/for_each.h"
#include "imp/fun.h"
#include "imp/str.h"
#include "imp/symbol.h"
#include "imp/closure.h"
#include "imp/tuple.h"
#include "imp/pair.h"
#include "imp/usr.h"
#include "imp/small.h"
/* user defined class */
#include "imp/cls.h"
#include "imp/symbol_table.h"
#include "imp/gf.h"
#include "imp/hash.h"
//#include "imp/array.h"
#include "imp/la.h"
#include "imp/apply.h"
#include "imp/exception.h"
#include "imp/generator.h"

#include "imp/overview.c"
#include "imp/fun.c"
#include "imp/str.c"
#include "imp/symbol.c"
#include "imp/closure.c"
#include "imp/tuple.c"
#include "imp/pair.c"
#include "imp/usr.c"
#include "imp/small.c"
/* user defined class */
#include "imp/cls.c"
//#include "imp/symbol_table.c"
#include "imp/gf.c"
#include "imp/array.c"
#include "imp/la.c"
#include "imp/apply.c"
//#include "imp/exception.c"
#include "imp/generator.c"    
#ifdef __cplusplus
}
#endif


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
