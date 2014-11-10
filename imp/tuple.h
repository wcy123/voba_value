#pragma once
/* tuple
   =====

there are two kinds of tuple. fixed size and variable size.


For fixed sized tuples, `voba_value_t` point to a below structure.


+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  |       +----------------+------------+
  `---->  |         size(64bits)        | 
          +----------------+------------+
          |         voba_value_t a1     |
          +-----------------------------+
          |         voba_value_t a2     |
          +-----------------------------+
          |            ....             |
          +-----------------------------+
*/
extern voba_value_t  voba_cls_tuple;
INLINE voba_value_t  voba_make_tuple(voba_value_t* p);
INLINE voba_value_t  voba_make_tuple_n(int64_t n,...);
INLINE voba_value_t  voba_make_tuple_nv(int64_t n,va_list s);
INLINE int64_t       voba_tuple_len(voba_value_t v);
INLINE voba_value_t* voba_tuple_base(voba_value_t v);
INLINE voba_value_t  voba_tuple_at(voba_value_t v,int64_t i);
INLINE voba_value_t  voba_tuple_set(voba_value_t a,int64_t i,voba_value_t v);
// I really hate to write these macros, but I hate more duplicate
// code. I used to use python or M4 generate the following code, but
// it isn't more readable and it is not so good to depend on tools
// other than standard compilers.
#define DEFINE_VOBA_MAKE_TUPLE_N(n)                                     \
    INLINE voba_value_t                                                 \
    voba_make_tuple_##n (VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG, COMMA)) {   \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * clz_long(n+1));            \
        p[0] =  (n);                                                    \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_TUPLE_ASSIGN,SEMI_COMMA);   \
        return voba_make_tuple(p);                                      \
    }
#define DEFINE_VOBA_MAKE_TUPLE_ASSIGN(n) p[n + 1] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_TUPLE_N,SPACE)

