#include "usr.h"

VOBA_VALUE_INLINE voba_value_t* voba_allocate_user_data(size_t s)
{
    size_t size = (s+sizeof(voba_value_t));
    if(size % 16 !=0){
        // size must be 16 bytes alignment.
        size = (size/16 + 1) * 16;
    }
    voba_value_t * p = (voba_value_t*)GC_MALLOC(size);
    assert(p);
    return p;
}
VOBA_VALUE_INLINE voba_value_t voba_make_user_data(voba_value_t cls)
{
    // size doesn't include the cls function, so allocate a room for it.
    assert(voba_cls_size(cls) > 0);
    voba_value_t * p = voba_allocate_user_data(voba_cls_size(cls));
    p[0] = cls;
    return voba_from_pointer(p,VOBA_TYPE_USER);
}
VOBA_VALUE_INLINE voba_value_t voba_user_data_class(voba_value_t v)
{
  return (voba_to_pointer(voba_value_t*, v)[0]);
}
VOBA_VALUE_INLINE void*  voba_user_data_base(voba_value_t v)
{
  return (void*)(&(voba_to_pointer(voba_value_t*, v)[1]));
}

