#pragma once
typedef struct voba_array_s voba_array_t;
/// todo
struct voba_array_s {
    uint32_t capacity;
    uint32_t len;
    voba_value_t * data;
};
#define VOBA_ARRAY(s)  VOBA_USER_DATA_AS(voba_array_t *,s)
extern voba_value_t voba_cls_array;
INLINE voba_value_t voba_make_array(uint32_t capacity, uint32_t len, voba_value_t* p);
INLINE voba_value_t voba_array_from_tuple(voba_value_t tuple);
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

