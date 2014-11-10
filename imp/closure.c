#include "closure.h"

// -- closure
/* INLINE voba_value_t  voba_make_closure_f_a(voba_func_t f, voba_value_t array) */
/* { */
/*   voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t)*(voba_array_len(array)+2)); */
/*   assert(p); */
/*   p[0] = voba_make_func(f); */
/*   memcpy((void*)&p[1], (void*)voba_value_to_c_array(array), sizeof(voba_value_t)*(voba_array_len(array)+1)); */
/*   p[1] = p[1] & 0xFFFFFFFF; // remove capacity if any. */
/*   return voba_from_pointer((void*)p,VOBA_TYPE_CLOSURE); */
/* } */
// return int64_t because voba_value_t is int64_t
INLINE int64_t voba_closure_len(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
    return voba_to_pointer(voba_value_t*, c)[1];
 
}
INLINE voba_value_t voba_closure_at(voba_value_t c,uint32_t i)
{
    assert(voba_is_a(c,voba_cls_closure));
    assert(((int64_t)i) < voba_closure_len(c));
    return voba_to_pointer(voba_value_t*, c)[i + 2]; // +2, skip func and size.
}
INLINE voba_value_t voba_closure_tuple(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
    return voba_make_tuple(&(voba_to_pointer(voba_value_t*, c)[1])); // +1, only skip func
}
INLINE voba_func_t voba_closure_func(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
    return (voba_func_t)voba_to_pointer(voba_value_t*,c)[0];
}
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

