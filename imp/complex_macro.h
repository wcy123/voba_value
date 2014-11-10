#pragma once


VOBA_FOR_EACH(DEFINE_VOBA_MAKE_TUPLE_N,SPACE)

#define DEFINE_VOBA_MAKE_CLOSURE_N(n)                                   \
    INLINE voba_value_t voba_make_closure_##n                           \
    (voba_func_t f VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG2, SPACE))          \
    {                                                                   \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * (n+2));                    \
        p[0] = voba_make_func(f);                                       \
        p[1] = n;                                                       \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_CLOSURE_ASSIGN,SEMI_COMMA); \
        return voba_from_pointer((void*)p,VOBA_TYPE_CLOSURE);           \
    }
#define DEFINE_VOBA_MAKE_CLOSURE_ASSIGN(n) p[n + 2] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_CLOSURE_N,SPACE)


