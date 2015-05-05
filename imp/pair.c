#include "pair.h"

// ---------- pair -----------------------
VOBA_VALUE_INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * 2);
    assert(p);
    p[0] = h;
    p[1] = t;
    return voba_from_pointer(p,VOBA_TYPE_PAIR);
}
VOBA_VALUE_INLINE voba_value_t  voba_head(voba_value_t p)
{
    return voba_to_pointer(voba_value_t*,p)[0];
}
VOBA_VALUE_INLINE voba_value_t  voba_tail(voba_value_t p)
{
    return voba_to_pointer(voba_value_t*,p)[1];
}
VOBA_VALUE_INLINE void voba_set_head(voba_value_t p,voba_value_t v)
{
    voba_to_pointer(voba_value_t*,p)[0] = v;
}
VOBA_VALUE_INLINE void voba_set_tail(voba_value_t p,voba_value_t v)
{
    voba_to_pointer(voba_value_t*,p)[1] = v;
}

