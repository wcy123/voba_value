#include "array.h"
INLINE uint32_t voba_array_len_to_capacity(uint32_t len)
{
    return (uint32_t)(clz_long(len + 2));
}
INLINE voba_value_t voba_make_array(uint32_t capacity, uint32_t len, voba_value_t* p)
{
    voba_value_t ret = voba_make_user_data(voba_cls_array);
    voba_array_t * p1 = VOBA_ARRAY(ret);
    p1->capacity = capacity;
    p1->len = len;
    p1->data = p;
    assert(p[len] == VOBA_BOX_END);
    return ret;
}
INLINE voba_value_t voba_array_from_tuple(voba_value_t tuple)
{
    assert(voba_is_a(tuple,voba_cls_tuple));
    uint32_t len = (uint32_t)voba_tuple_len(tuple);
    uint32_t capacity = len == 0?2:(len+1); /* minimun capacity is 2 */
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * capacity);
    if(!p){abort();};
    // len + 1 to include the VOBA_BOX_END
    memcpy(p,voba_tuple_base(tuple), sizeof(voba_value_t)*(len+1));
    return voba_make_array(capacity,len,p);
}
INLINE voba_value_t voba_make_array_nv(uint32_t n,va_list ap)
{
    uint32_t len = n;
    uint32_t capacity = len + 1; // include the space for VOBA_BOX_END
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * capacity);
    assert(n>0);
    assert(p);
    int i;
    for(i = 0 ; i < n; ++i){
        p[i] = va_arg(ap,voba_value_t);
    }
    p[n] = VOBA_BOX_END;
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
    assert(voba_is_a(v,voba_cls_array));
    return VOBA_ARRAY(v)->data;
}
INLINE uint32_t voba_array_capacity(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_array));
    return VOBA_ARRAY(v)->capacity;
}
INLINE uint32_t voba_array_len(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_array));
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
    // c1 should be larger than len at least by 1 to include VOBA_BOX_END
    assert(c1 > len + 1);
    memcpy((void*)p,voba_array_base(v),c1);
    return voba_make_array(capacity,len,p);
}
INLINE void voba_array__enlarge(voba_value_t a, uint32_t inc)
{
    uint32_t len = voba_array_len(a);
    uint32_t capacity = voba_array_capacity(a);
    voba_value_t * p = voba_array_base(a);
    if(len + inc + 2 > capacity){ // one more room for `capacity+size`
        capacity = capacity * 2;
        p = (voba_value_t*) GC_REALLOC((void*)p, capacity*sizeof(voba_value_t));
        if(!p){abort();}
        VOBA_ARRAY(a)->capacity = capacity;
        /*
          for example: when capcity is 8  and len is 3

          /------- from ^p[4]
          3 a0 a1 a2 n0 n1 n2 n3
          \___ 4 ___/

        */
        bzero(&p[len+1], sizeof(voba_value_t)*(capacity - len - 1));
	p[len] = VOBA_BOX_END;
        VOBA_ARRAY(a)->data = p;
    }
    return;
}
INLINE voba_value_t voba_array_push(voba_value_t a,voba_value_t v)
{
    assert(voba_is_a(a,voba_cls_array));
    voba_array__enlarge(a,1);
    voba_value_t * p = voba_array_base(a);
    uint32_t len = voba_array_len(a);
    p[len] = v;
    p[len + 1] = VOBA_BOX_END;
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
    p[len+1] = VOBA_BOX_END;
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
    p[len] = VOBA_BOX_END;
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
    p[len] = VOBA_BOX_END;
    VOBA_ARRAY(a)->len --;
    return ret;
}
INLINE voba_value_t voba_array_concat(voba_value_t a, voba_value_t b)
{
    assert(voba_is_a(b,voba_cls_array));
    uint32_t len = voba_array_len(b);
    voba_array__enlarge(a,len);
    memmove(voba_array_base(a) + voba_array_len(a),
            voba_array_base(b),
            (len+1) * sizeof(voba_value_t));
    VOBA_ARRAY(a)->len += len;
    return a;
}
// I really hate to write these macros, but I hate more duplicate
// code. I used to use python or M4 generate the following code, but
// it isn't more readable and it is not so good to depend on tools
// other than standard compilers.
#define DEFINE_VOBA_MAKE_ARRAY_N(n)                                     \
    INLINE voba_value_t                                                 \
    voba_make_array_##n (VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG, COMMA)) {   \
        uint32_t len = n;                                               \
        uint32_t capacity = clz_long(len);                              \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * capacity);                 \
        if(!p){abort();}                                                \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_ARRAY_ASSIGN,SEMI_COMMA);   \
        p[n] = VOBA_BOX_END;						\
	return voba_make_array(capacity,len,p);				\
    }
#define DEFINE_VOBA_MAKE_ARRAY_ASSIGN(n) p[n] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_ARRAY_N,SPACE)
