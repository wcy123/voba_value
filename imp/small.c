#include "small.h"

VOBA_VALUE_INLINE int voba_is_nil(voba_value_t p) 
{ 
  return voba_eq(p,VOBA_NIL);
}
// ----------- boolean ------------------
VOBA_VALUE_INLINE int voba_is_true(voba_value_t v)
{
  return v == VOBA_TRUE;
}
VOBA_VALUE_INLINE int voba_is_false(voba_value_t v)
{
  return v == VOBA_FALSE;
}
VOBA_VALUE_INLINE voba_value_t voba_not(voba_value_t v)
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
VOBA_VALUE_INLINE int64_t voba_get_type1(voba_value_t v)
{
    return voba_is_nil(v)?VOBA_TYPE_NIL:(v&VOBA_TYPE_MASK);
}
VOBA_VALUE_INLINE int64_t voba_get_type2(voba_value_t v)
{
  return (v&0xff)>>3;
}
VOBA_VALUE_INLINE voba_value_t voba_make_i32 (int32_t v)
{
    return (VOBA_TYPE_SMALL + VOBA_TYPE_I32 * 8 + (v<<8));
}
VOBA_VALUE_INLINE int32_t voba_value_to_i32 (voba_value_t v)
{
    return ((int32_t)(v >> 8));
}
VOBA_VALUE_INLINE voba_value_t voba_make_float (float v)
{
    union {
	float v;
	voba_value_t x;
    } vv;
    vv.v = v;
    return (VOBA_TYPE_SMALL + VOBA_TYPE_FLOAT * 8 + (vv.x<<8));
}
VOBA_VALUE_INLINE float voba_value_to_float (voba_value_t v)
{
    assert(v % 256 == VOBA_TYPE_SMALL + VOBA_TYPE_FLOAT*8);
    union {
	float v;
	voba_value_t x;
    } vv;
    vv.x = (v/256);
    return vv.v;
}


