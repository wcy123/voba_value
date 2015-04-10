#include "tuple.h"
// ------------- tuple ---------------------
INLINE voba_value_t voba_make_tuple(voba_value_t* p)
{
    return voba_from_pointer(p,VOBA_TYPE_TUPLE);
}
INLINE voba_value_t voba_make_tuple_nv(int64_t n,va_list ap)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * (n + 1));
    assert(p);
    p[0] = n;
    for(int i = 0 ; i < n; ++i){
        p[i+1] = va_arg(ap,voba_value_t);
    }
    return voba_make_tuple(p);
}
INLINE voba_value_t voba_make_tuple_n(int64_t n,...) 
{
    voba_value_t ret = VOBA_NIL;
    va_list ap;
    va_start(ap,n);
    ret = voba_make_tuple_nv(n,ap);
    va_end(ap);
    return ret;
}
INLINE int64_t voba_tuple_len(voba_value_t v)
{
    return voba_to_pointer(voba_value_t*, v)[0];
}
INLINE voba_value_t* voba_tuple_base(voba_value_t v)
{
    return voba_to_pointer(voba_value_t*, v) + 1;
}
INLINE voba_value_t voba_tuple_at(voba_value_t v,int64_t i)
{
    return voba_tuple_base(v)[i];
}
INLINE voba_value_t voba_tuple_set(voba_value_t a,int64_t i,voba_value_t v)
{
    return voba_tuple_base(a)[i] = v;
}

INLINE voba_value_t* voba_alloc(size_t n_of_elt)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * n_of_elt);
    if(!p){abort();};
    return p;
}
INLINE voba_value_t voba_tuple_copy(voba_value_t tuple)
{
    voba_value_t * ret = voba_alloc(voba_tuple_len(tuple)+1);
    memcpy((void*)ret,voba_to_pointer(void*,tuple),
	   (voba_tuple_len(tuple)+1) * sizeof(voba_value_t));
    return voba_make_tuple(ret);
}
