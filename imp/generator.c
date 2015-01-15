#include "generator.h"

INLINE voba_value_t voba_make_generator(voba_func_t f, voba_value_t self, voba_value_t args)
{
    voba_value_t ret = voba_make_user_data(voba_cls_generator);
    cg_init(VOBA_GENERATOR(ret), (void*) f, self,args);
    return ret;
}
