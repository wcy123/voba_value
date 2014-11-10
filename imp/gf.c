#include "gf.h"
INLINE voba_value_t voba_make_generic_function(const char * name, voba_func_t default_imp)
{
    voba_value_t ret = voba_make_user_data(voba_cls_generic_function);
    VOBA_GF(ret)->hash = voba_make_hash();
    VOBA_GF(ret)->fun = default_imp;
    VOBA_GF(ret)->name = name;
    VOBA_GF(ret)->id = voba_cls_generic_function_next_id++;
    if(voba_cls_generic_function_next_id >= VOBA_MAX_NUM_OF_GF){
        voba_throw_exception(
            voba_make_string(VOBA_CONST_CHAR("too many generic functions")));
        abort();
    }
    for(uint32_t i = 0; i < VOBA_MAX_NUM_OF_CLS; ++i){
        VOBA_GF(ret)->cls[i] = default_imp;
    }
    return ret;
}
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func)
{
    //assert(voba_is_a(gf,voba_cls_generic_function));
    //return voba_hash_insert(VOBA_GF(gf)->hash,cls,func);
    if(0){
        fprintf(stderr,__FILE__ ":%d:[%s] looking gf= %s cls=%s f= %p\n", __LINE__, __FUNCTION__
                ,VOBA_GF(gf)->name
                ,VOBA_CLS(cls)->name
                ,voba_value_to_func(func));
    }
    VOBA_GF(gf)->cls[VOBA_CLS(cls)->id] = voba_value_to_func(func);
    return func;
}
INLINE voba_func_t voba_gf_lookup(voba_value_t gf, voba_value_t cls)
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    if(0){
        fprintf(stderr,__FILE__ ":%d:[%s] looking gf= %s cls=%s f= %p\n", __LINE__, __FUNCTION__
                ,VOBA_GF(gf)->name
                ,VOBA_CLS(cls)->name
                ,VOBA_GF(gf)->cls[VOBA_CLS(cls)->id]);

    }
    return VOBA_GF(gf)->cls[VOBA_CLS(cls)->id];
}

