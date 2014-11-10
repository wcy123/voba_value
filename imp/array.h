#pragma once
typedef struct voba_array_s voba_array_t;
struct voba_array_s {
    uint32_t capacity;
    uint32_t len;
    voba_value_t * data;
};
#define VOBA_ARRAY(s)  VOBA_USER_DATA_AS(voba_array_t *,s)
extern voba_value_t voba_cls_array;
INLINE voba_value_t voba_make_array(uint32_t capacity, uint32_t len, voba_value_t* p);
INLINE voba_value_t voba_make_array_n(uint32_t n,...);
INLINE voba_value_t * voba_array_base(voba_value_t v);
INLINE uint32_t voba_array_capacity(voba_value_t v);
INLINE uint32_t voba_array_len(voba_value_t v);
INLINE voba_value_t voba_array_at(voba_value_t v,uint32_t i);
INLINE voba_value_t  voba_array_set(voba_value_t a,uint32_t i,voba_value_t v);    
INLINE voba_value_t  voba_array_push(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_pop(voba_value_t a);
INLINE voba_value_t  voba_array_shift(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_unshift(voba_value_t a);
// a is changed, b is not changed.
INLINE voba_value_t  voba_array_concat(voba_value_t a, voba_value_t b);

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
            GC_MALLOC(sizeof(voba_value_t) * len);                      \
        if(!p){abort();}                                                \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_ARRAY_ASSIGN,SEMI_COMMA);   \
        return voba_make_array(capacity,len,p);                         \
    }
#define DEFINE_VOBA_MAKE_ARRAY_ASSIGN(n) p[n] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_ARRAY_N,SPACE)
