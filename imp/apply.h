#pragma once
/** @file
apply
=====

It is the only core function of voba_value library.  

~~~{.c}
voba_apply(f, args) 
~~~

evaluates s-exp 
~~~
(f a1 a2 ....)
~~~

where \a args is the argument tuple \a (a1 a2 ...)

\a f is any type which is callable.

1. \a voba_cls_func: call it directly
2. \a voba_cls_generic_function: use `voba_gf_lookup(f, voba_get_class(a1))` to find the actual voba_cls_func.
3. \a otherwise, use `voba_gf_lookup(voba_gf_apply, voba_get_class(f))` to find the actual 

*/
/** @brief invoke a callable object `f`

@code{.lisp}
(f a0 a1 a3 ...)
@endcode
   @param f The callable object
   @param args The tuple which represents arguments, `a0`, ... , `an`
 */
VOBA_VALUE_INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t args);
VOBA_VALUE_INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args,voba_value_t* next_fun, voba_value_t next_args[]);
VOBA_VALUE_INLINE voba_value_t voba_apply_gf(voba_value_t gf, voba_value_t args,voba_value_t* next_fun, voba_value_t next_args[]);
VOBA_VALUE_INLINE voba_value_t voba_apply_user_object(voba_value_t f, voba_value_t a1,voba_value_t* next_fun, voba_value_t next_args[]);
#ifndef VOBA_MAX_NUM_OF_TAIL_CALL_ARGS
#define VOBA_MAX_NUM_OF_TAIL_CALL_ARGS 20
#endif

/** @brief apply macros
 */

#define VOBA_APPLY(r,f,...) {                                   \
    voba_value_t tmpargs [] = {0, __VA_ARGS__, VOBA_BOX_END};	\
    tmpargs[0] = sizeof(tmpargs) / sizeof(voba_value_t) -2 ;	\
    r = voba_apply(f, voba_make_tuple(tmpargs));                \
    }                                                           

VOBA_VALUE_INLINE voba_value_t apply_tuple(voba_value_t self, voba_value_t args);
