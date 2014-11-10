#include "array.h"
INLINE uint32_t voba_array_len_to_capacity(uint32_t len)
{
    return (uint32_t)(clz_long(len + 1));
}
INLINE voba_value_t voba_make_array(uint32_t capacity, uint32_t len, voba_value_t* p)
{
    voba_value_t ret = voba_make_user_data(voba_cls_array);
    voba_array_t * p1 = VOBA_ARRAY(ret);
    p1->capacity = capacity;
    p1->len = len;
    p1->data = p;
    return ret;
}
INLINE voba_value_t voba_make_array_nv(uint32_t n,va_list ap)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * n);
    assert(n>0);
    assert(p);
    for(int i = 0 ; i < n; ++i){
        p[i] = va_arg(ap,voba_value_t);
    }
    return voba_make_array(n,n,p);
}
INLINE voba_value_t voba_make_array_n(uint32_t n,...) 
{
    voba_value_t ret = VOBA_NIL;
    va_list ap;
    va_start(ap,n);
    ret = voba_make_array_nv(n,ap);
    va_end(ap);
    return ret;
}
INLINE voba_value_t * voba_array_base(voba_value_t v)
{
    return VOBA_ARRAY(v)->data;
}
INLINE uint32_t voba_array_capacity(voba_value_t v)
{
    return VOBA_ARRAY(v)->capacity;
}
INLINE uint32_t voba_array_len(voba_value_t v)
{
    return VOBA_ARRAY(v)->len;
}
INLINE voba_value_t voba_array_at(voba_value_t v,uint32_t i)
{
    return voba_array_base(v)[i];
}
INLINE voba_value_t  voba_array_set(voba_value_t a,uint32_t i,voba_value_t v)
{
    return voba_array_base(a)[i] = v;
}
INLINE voba_value_t voba_array_copy(voba_value_t v)
{
    uint32_t capacity = voba_array_capacity(v);
    uint32_t len = voba_array_len(v);
    size_t c1 = sizeof(voba_value_t) * capacity;
    voba_value_t * p = (voba_value_t*)GC_MALLOC(c1);
    assert(p);
    memcpy((void*)p,voba_array_base(v),c1);
    return voba_make_array(capacity,len,p);
}
INLINE void voba_array__enlarge(voba_value_t a, uint32_t inc)
{
    uint32_t len = voba_array_len(a);
    uint32_t capacity = voba_array_capacity(a);
    voba_value_t * p = voba_array_base(a);
    if(len + inc + 1 > capacity){ // one more room for `capacity+size`
        capacity = capacity * 2;
        p = (voba_value_t*) GC_REALLOC((void*)p, capacity*sizeof(voba_value_t));
        assert(p);
        VOBA_ARRAY(a)->capacity = capacity;
        /*
          for example: when capcity is 8  and len is 3

          /------- from ^p[4]
          3 a0 a1 a2 n0 n1 n2 n3
          \___ 4 ___/

        */
        bzero(&p[len+1], sizeof(voba_value_t)*(capacity - len - 1));
        VOBA_ARRAY(a)->data = p;
    }
    return;
}
INLINE voba_value_t voba_array_push(voba_value_t a,voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_array));
    voba_array__enlarge(a,1);
    voba_value_t * p = voba_array_base(a);
    uint32_t len = voba_array_len(a);
    p[len] = v;
    assert(len < UINT32_MAX);
    VOBA_ARRAY(a)->len ++;
    return a;
}
INLINE voba_value_t voba_array_shift(voba_value_t a,voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_array));
    voba_array__enlarge(a,1);
    voba_value_t * p = voba_array_base(a);
    uint32_t len = voba_array_len(a);
    memmove(&p[1], &p[0], len * sizeof(voba_value_t));
    p[0] = v;
    assert(len < UINT32_MAX);
    VOBA_ARRAY(a)->len ++;
    return a;
}
INLINE voba_value_t voba_array_pop(voba_value_t a)
{
    // no shrink?
    assert(voba_is_a(a,voba_cls_array));
    uint32_t len = voba_array_len(a);
    if(len == 0){
        //throw_exception(a);
        assert(0 && "cannot pop empty array.");
    }
    voba_value_t * p = voba_array_base(a);
    voba_value_t ret = p[len];
    VOBA_ARRAY(a)->len --;
    return ret;
}
INLINE voba_value_t voba_array_unshift(voba_value_t a)
{
    // no shrink?
    assert(voba_is_a(a,voba_cls_array));
    uint32_t len = voba_array_len(a);
    if(len == 0){
        //throw_exception(a);
        assert(0 && "cannot unshift empty array.");
    }
    voba_value_t * p = voba_array_base(a);
    voba_value_t ret = p[0];
    memmove(&p[0], &p[1], len * sizeof(voba_value_t));
    VOBA_ARRAY(a)->len --;
    return ret;
}
INLINE voba_value_t voba_array_concat(voba_value_t a, voba_value_t b)
{
    assert(voba_is_a(b,voba_cls_array));
    uint32_t len = voba_array_len(b);
    voba_array__enlarge(a,len);
    memmove(voba_array_base(a) + voba_array_len(b),
            voba_array_base(b),
            len * sizeof(voba_value_t));
    return a;
}
