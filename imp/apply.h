#pragma once
/* apply
   =====
 It is the only core function of voba_value library.
 voba_apply(f, args) evaluates s-exp (f a1 a2 ....)
 where args is the argument tuple (a1 a2 ...)

 `f` is any type which is callable.

 1. voba_cls_func: call it directly
 2. voba_cls_generic_function: use `voba_gf_lookup(f, voba_get_class(a1))` to find the actual voba_cls_func.
 3. otherwise, use `voba_gf_lookup(voba_gf_apply, voba_get_class(f))` to find the actual 

*/

extern voba_value_t voba_gf_apply;
INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args);
voba_value_t voba_direct_apply_n(voba_func_t f,size_t n, ...); // defined in voba_value.c
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a);


