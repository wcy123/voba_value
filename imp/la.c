#include "la.h"

VOBA_VALUE_INLINE voba_value_t voba__make_la(uint32_t cur, uint32_t end,voba_value_t array)
{
    voba_value_t r = voba_make_user_data(voba_cls_la);
    VOBA_LA(r)->cur = cur;
    VOBA_LA(r)->end = end;
    VOBA_LA(r)->array = array;
    return r;
}
VOBA_VALUE_INLINE voba_value_t voba_la_from_array0(voba_value_t array)
{
    uint32_t a_len = ((uint32_t) voba_array_len(array));
    return voba__make_la(0,a_len,array);
}
VOBA_VALUE_INLINE voba_value_t voba_la_from_array1(voba_value_t array, uint32_t cur)
{
    uint64_t a_len = ((uint64_t) voba_array_len(array));
    assert(cur <= a_len);
    uint32_t end = (uint32_t)a_len;
    return voba__make_la(cur,end,array);
}
VOBA_VALUE_INLINE voba_value_t voba_la_from_array2(voba_value_t array, uint32_t cur,uint32_t len)
{
#ifndef NDEBUG
    uint64_t a_len = ((uint64_t) voba_array_len(array));
#endif
    assert(cur + len <= a_len);
    uint32_t end = (uint32_t) (cur + len);
    return voba__make_la(cur,end,array);
}
VOBA_VALUE_INLINE voba_value_t voba_la_car(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    uint32_t cur = VOBA_LA(la)->cur;
    voba_value_t a = VOBA_LA(la)->array;
#ifndef NDEBUG
    uint32_t end = VOBA_LA(la)->end;
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
#endif
    assert(cur <= a_len && cur <= end && end <= a_len);
    return voba_array_at(a,(int64_t)cur);
}
VOBA_VALUE_INLINE voba_value_t voba_la_cdr(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    uint32_t cur = VOBA_LA(la)->cur;
    uint32_t end = VOBA_LA(la)->end;
#ifndef NDEBUG
    voba_value_t a = VOBA_LA(la)->array;
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
#endif
    assert(cur <= a_len && cur <= end && end <= a_len);
    if(cur < end) {
        cur ++;
    }
    VOBA_LA(la)->cur = cur;
    return la;
}
VOBA_VALUE_INLINE voba_value_t voba_la_last(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
#ifndef NDEBUG    
    uint32_t cur = VOBA_LA(la)->cur;
#endif	
    voba_value_t a = VOBA_LA(la)->array;
    uint32_t end = VOBA_LA(la)->end;
#ifndef NDEBUG
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
#endif
    assert(cur <= a_len && cur <= end && end <= a_len);
    return voba_array_at(a,(int64_t)(end-1));
}
VOBA_VALUE_INLINE voba_value_t voba_la_copy(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    return voba__make_la(VOBA_LA(la)->cur,VOBA_LA(la)->end,VOBA_LA(la)->array);
}
VOBA_VALUE_INLINE uint32_t voba_la_len(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    assert(VOBA_LA(la)->end >= VOBA_LA(la)->cur);
    return VOBA_LA(la)->end - VOBA_LA(la)->cur;
}
VOBA_VALUE_INLINE int voba_la_is_nil(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    return voba_la_len(la) == 0;
}
VOBA_VALUE_INLINE voba_value_t voba_la_to_array(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    voba_value_t ret = voba_make_array_0();
    voba_value_t xs = voba_la_copy(la);
    while(!voba_la_is_nil(xs)){
        voba_value_t x = voba_la_car(xs);
        ret = voba_array_push(ret,x);
        xs = voba_la_cdr(xs);
    }
    return ret;
}

