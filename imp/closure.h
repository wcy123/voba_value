#pragma once
/* closure
   =======

+---------------------------+-------+
| data                      | type1 |
+---------------------------+-------+
| .    value (61 bits)      |  011  |
+-|-------------------------+-------+
  |
  |      +----------------+------------+
  `----> |         funcp(64bits)       |
         +----------------+------------+
         |         size(64bits)        |
         +-----------------------------+
         |         voba_value_t a1     |
         +-----------------------------+
         |            ....             |
         +-----------------------------+

*/
extern voba_value_t voba_cls_closure;
// this function is a little bit misleading, we might have thought a
//closure is a func plus an tuple.

//INLINE voba_value_t voba_make_closure_f_a(voba_func_t f, voba_value_t tuple);
// see DEFINE_VOBA_MAKE_CLOSURE_N
//INLINE voba_value_t voba_make_closure_N(voba_value_t f, voba_value_t a0,...);
INLINE voba_value_t voba_closure_at(voba_value_t c,uint32_t i);
INLINE voba_func_t  voba_closure_func(voba_value_t c);
INLINE voba_value_t voba_closure_tuple(voba_value_t c);
INLINE voba_value_t voba_closure_len(voba_value_t c);


#define VOBA_MACRO_ARG2(n) ,voba_value_t a##n
#define DECLARE_VOBA_MAKE_CLOSURE_N(n)                                  \
    INLINE voba_value_t voba_make_closure_##n                           \
    (voba_func_t f VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG2, SPACE))
VOBA_FOR_EACH(DECLARE_VOBA_MAKE_CLOSURE_N,SEMI_COMMA);
