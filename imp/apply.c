#include "apply.h"

INLINE voba_func_t voba__apply_find_func(voba_value_t f, voba_value_t a1)
{
    voba_func_t ret = NULL;
    if(voba_is_a(f,voba_cls_generic_function) &&
       voba_tuple_len(a1) >= 1){
        voba_value_t cls = voba_get_class(voba_tuple_at(a1,0));
        voba_func_t vf = voba_gf_lookup(f,cls);
        if(!(vf == NULL)){
            ret = vf;
        }else{
            voba_throw_exception(
                voba_make_string(
                    VOBA_STRCAT(
                        VOBA_CONST_CHAR("vfunc `"),
                        voba_str_from_cstr(VOBA_GF(f)->name),
                        VOBA_CONST_CHAR("' is not found for cls `"),
                        voba_str_from_cstr(VOBA_CLS(cls)->name),
                        VOBA_CONST_CHAR("'")
                        )));
        }
    } else {
        voba_value_t self  = f;
        voba_value_t cls = voba_get_class(self);
        voba_func_t vf = voba_gf_lookup(voba_gf_apply,cls);
        if(!(vf == NULL)){
            ret = vf;
        }else{
            voba_throw_exception(
                voba_make_string(
                    VOBA_STRCAT(
                        VOBA_CONST_CHAR("object of `"),
                        voba_str_from_cstr(VOBA_CLS(cls)->name),
                        VOBA_CONST_CHAR("' is not callable`")
                        )));
        }
    }
    return ret;
}
INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args)
{
    return f(voba_make_func(f),args);
}
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a1)
{
    assert(voba_is_a(a1,voba_cls_tuple));
    switch(voba_get_type1(f)){
    case VOBA_TYPE_FUNC:
        return voba_direct_apply(voba_value_to_func(f),a1);
    case VOBA_TYPE_CLOSURE:
        return voba_closure_func(f)(voba_closure_tuple(f),a1);
    }
    voba_func_t func = voba__apply_find_func(f,a1);
    if(func != NULL){
        return func(f,a1);
    }else{
        voba_throw_exception(
            voba_make_string(voba_str_from_cstr("cannot apply")));
    }
    return VOBA_NIL;
}

