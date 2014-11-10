#include "small.h"

INLINE int voba_is_nil(voba_value_t p) 
{ 
  return voba_eq(p,VOBA_NIL);
}
// ----------- boolean ------------------
INLINE int voba_is_true(voba_value_t v)
{
  return v == VOBA_TRUE;
}
INLINE int voba_is_false(voba_value_t v)
{
  return v == VOBA_FALSE;
}
INLINE voba_value_t voba_not(voba_value_t v)
{
    voba_value_t ret = VOBA_FALSE;
    if(voba_is_false(v)){
        ret = VOBA_TRUE;
    }else{
        ret = VOBA_FALSE;
    }
    return ret;
}
// ------------- get type and get class ----
INLINE int64_t voba_get_type1(voba_value_t v)
{
    return voba_is_nil(v)?VOBA_TYPE_NIL:(v&VOBA_TYPE_MASK);
}
INLINE int64_t voba_get_type2(voba_value_t v)
{
  return (v&0xff)>>3;
}

#define DEFINE_SMALL_TYPE(tag,name,type)                \
    INLINE voba_value_t voba_make_##name (type v)       \
    {                                                   \
        voba_value_t ret = (voba_value_t) v;            \
        return (VOBA_TYPE_SMALL + tag * 8 + (ret<<8));  \
    }                                                   \
    INLINE type voba_value_to_##name (voba_value_t v)   \
    {                                                   \
        return ((type)(v >> 8));                        \
    }

VOBA_SMALL_TYPES(DEFINE_SMALL_TYPE)











INLINE int64_t voba_int_value_to_i32(voba_value_t a)
{
    assert(voba_is_int(a));
    return (int32_t)(a>>8);
}
INLINE int voba_is_int(voba_value_t a )
{
    voba_value_t cls = voba_get_class(a);
    if(voba_eq(cls,voba_cls_i8) ||
       voba_eq(cls,voba_cls_i16) ||
       voba_eq(cls,voba_cls_i32) ||
       voba_eq(cls,voba_cls_u8) ||
       voba_eq(cls,voba_cls_u16) ||
       voba_eq(cls,voba_cls_u32))
    {
        return 1;
    }
    return 0;
}
INLINE voba_value_t i64_to_voba_int_value(int64_t a)
{
    voba_value_t ret = voba_make_i32(0);
    if(a >= INT8_MIN && a <= INT8_MAX){
        int8_t x = (int8_t) a;
        ret = voba_make_i8(x);
    }else if(a >= 0 && a <= UINT8_MAX){
        uint8_t x = (uint8_t) a;
        ret = voba_make_u8(x);
    }else if(a >= INT16_MIN && a <= INT16_MAX){
        int16_t x = (int16_t) a;
        ret = voba_make_i16(x);
    }else if(a >= 0 && a <= UINT16_MAX){
        uint16_t x = (uint16_t) a;
        ret = voba_make_u16(x);
    }else if(a >= INT32_MIN && a <= INT32_MAX){
        int32_t x = (int32_t) a;
        ret = voba_make_i32(x);
    }else if(a >= 0 && a <= UINT32_MAX){
        uint32_t x = (uint32_t) a;
        ret = voba_make_u32(x);
    }else{
        voba_throw_exception(voba_make_string(VOBA_CONST_CHAR("32-bit integer overflow")));
    }
    return ret;
}

