#pragma once
/** @file

closure
=======

\verbatim
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
\endverbatim

A \a closure is a pointer which points to an array . 

*/
extern voba_value_t voba_cls_closure; /*!< the class object associated with \a closure */
INLINE voba_value_t voba_closure_at(voba_value_t c,uint32_t i); ///
INLINE voba_func_t  voba_closure_func(voba_value_t c); ///
INLINE voba_value_t voba_closure_tuple(voba_value_t c); /// 
INLINE uint64_t voba_closure_len(voba_value_t c); ///


#define VOBA_MACRO_ARG2(n) ,voba_value_t a##n
/** @brief create a closure
    \a f function pointer
    \a a0 captured argument 0
    @return a closure object

    it is a set of functions created by a complicated macro, as below
@code{.c}
voba_value_t voba_make_closure_0(voba_value_t f);
voba_value_t voba_make_closure_1(voba_value_t f, voba_value_t a0);
voba_value_t voba_make_closure_2(voba_value_t f, voba_value_t a0,voba_value_t a1);
//...
voba_value_t voba_make_closure_20(voba_value_t f, voba_value_t a0,voba_value_t a1, ..., voba_value_t a19);
@endcode

 */
#define DECLARE_VOBA_MAKE_CLOSURE_N(n)                                  \
    INLINE voba_value_t voba_make_closure_##n                           \
    (voba_func_t f VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG2, SPACE))
VOBA_FOR_EACH(DECLARE_VOBA_MAKE_CLOSURE_N,SEMI_COMMA);
