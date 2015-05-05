#include "str.h"

// ------------- string -----------------
VOBA_VALUE_INLINE voba_value_t voba_make_string(voba_str_t* s)
{
    if(s == NULL) return VOBA_NIL;
    return voba_from_pointer(s,VOBA_TYPE_USER);
}
VOBA_VALUE_INLINE voba_value_t voba_make_cstr(const char * s)
{
  return voba_make_string(voba_str_from_cstr(s));
}
VOBA_VALUE_INLINE voba_str_t* voba_value_to_str(voba_value_t s)
{
    assert(voba_is_a(s,voba_cls_str));
    return voba_to_pointer(voba_str_t*,s);
}

