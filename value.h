/** @file */
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
/** @brief The generic function object for callable objects.

instances whose class implements this generic function is callable.
for example,
~~~{.c}
voba_gf_add_class(voba_gf_apply,voba_cls_array,voba_make_func(apply_array));

VOBA_FUNC static voba_value_t apply_array(voba_value_t self, voba_value_t args)
{
    ....
}
~~~

then \a array becomes callable. 

~~~{.c}
voba_value_t a = voba_array_from_tuple(tuple);
voba_value_t args[] = { 1, voba_make_i8(0) };
voba_value_t r = voba_apply(a, args); // invoke apply_array 
~~~

*/
extern voba_value_t voba_gf_apply;
/** The generic function used to define callable objects. see gf.h 

Objects whose class implements this generic function are callable.
for example,
~~~{.c}
voba_gf_add_class(voba_gf_apply,voba_cls_array,voba_make_func(apply_array));

VOBA_FUNC static voba_value_t apply_array(voba_value_t self, voba_value_t args)
{
    ....
}
~~~

then \a array becomes callable. 

~~~{.c}
voba_value_t a = voba_array_from_tuple(tuple);
voba_value_t args[] = { 1, voba_make_i8(0) };
voba_value_t r = voba_apply(a, args); // invoke apply_array 
~~~

*/
extern voba_value_t voba_gf_apply;
/** @brief The generic function object used to define iterative object.

`(voba_gf_iter obj)` should return a callable object. This callable
object is called iterator. The iterator can be invoked many time until
it returns ::VOBA_DONE, indicating the end of iteration. For example,
function object(see ::voba_cls_fun) returns itself.

 */
extern voba_value_t voba_gf_iter;
/** @brief The generic function object for `match`

If a class implements this generic function, the class can be used for
pattern matching. It is used by `match` statement of voba language.

@todo explain `match` statement in detail

 */
extern voba_value_t voba_gf_match;    
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
