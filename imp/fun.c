#include "fun.h"

// ------------ funcp ----------------------  
VOBA_VALUE_INLINE  voba_value_t voba_make_func(voba_func_t p)
{
  return voba_from_pointer((void*)((intptr_t)p),VOBA_TYPE_FUNC);
}
VOBA_VALUE_INLINE  voba_func_t voba_value_to_func(voba_value_t v)
{
  return voba_to_pointer(voba_func_t,v);
}

