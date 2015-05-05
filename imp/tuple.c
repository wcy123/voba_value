#include "tuple.h"
// ------------- tuple ---------------------
VOBA_VALUE_INLINE voba_value_t voba_make_tuple(voba_value_t* p)
{
    assert(p[p[0] + 1] == VOBA_BOX_END);
    return voba_from_pointer(p,VOBA_TYPE_TUPLE);
}
VOBA_VALUE_INLINE voba_value_t voba_make_tuple_nv(int64_t n,va_list ap)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * (n + 2));
    assert(p);
    p[0] = n;
    int i;
    for(i = 0 ; i < n; ++i){
        p[i+1] = va_arg(ap,voba_value_t);
    }
    p[i+1] = VOBA_BOX_END;
    return voba_make_tuple(p);
}
VOBA_VALUE_INLINE voba_value_t voba_make_tuple_n(int64_t n,...) 
{
    voba_value_t ret = VOBA_NIL;
    va_list ap;
    va_start(ap,n);
    ret = voba_make_tuple_nv(n,ap);
    va_end(ap);
    return ret;
}
VOBA_VALUE_INLINE int32_t voba_tuple_len(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_tuple));
    return (int32_t)(voba_to_pointer(voba_value_t*, v)[0]);
}
VOBA_VALUE_INLINE voba_value_t* voba_tuple_base(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_tuple));
    return voba_to_pointer(voba_value_t*, v) + 1;
}
VOBA_VALUE_INLINE voba_value_t voba_tuple_at(voba_value_t v,int64_t i)
{
    assert(voba_is_a(v,voba_cls_tuple));
    return voba_tuple_base(v)[i];
}
VOBA_VALUE_INLINE voba_value_t voba_tuple_set(voba_value_t a,int64_t i,voba_value_t v)
{
    assert(voba_is_a(a,voba_cls_tuple));
    return voba_tuple_base(a)[i] = v;
}

VOBA_VALUE_INLINE voba_value_t* voba_alloc(size_t n_of_elt)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * n_of_elt);
    if(!p){abort();};
    return p;
}
VOBA_VALUE_INLINE voba_value_t voba_tuple_copy(voba_value_t tuple)
{
    voba_value_t * ret = voba_alloc(voba_tuple_len(tuple)+1);
    memcpy((void*)ret,voba_to_pointer(void*,tuple),
	   (voba_tuple_len(tuple)+2) * sizeof(voba_value_t));
    return voba_make_tuple(ret);
}
