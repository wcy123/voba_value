#include "cls.h"

INLINE voba_value_t voba_make_cls(size_t size, const char * name)
{
    voba_value_t ret = voba_make_user_data(voba_cls_cls);
    VOBA_CLS(ret)->name = name;
    VOBA_CLS(ret)->size = size;
    VOBA_CLS(ret)->id = voba_cls_next_id ++;
    if(voba_cls_next_id >= VOBA_MAX_NUM_OF_CLS){
        voba_throw_exception(
            voba_make_string(VOBA_CONST_CHAR("too many classes")));
        abort();
    }
    return ret;
}

// get_class
#define VOBA_GET_CLASS_SMALL(tag,name,type) case tag:   return voba_cls_##name;
static inline voba_value_t voba_get_class(voba_value_t v)
{
    switch(v){
    case VOBA_UNDEF:
        return voba_cls_undef;
        break;
    case VOBA_DONE:
        return voba_cls_done;
        break;
    case VOBA_NIL:
        return voba_cls_nil;
    case VOBA_TRUE:
    case VOBA_FALSE:
        return voba_cls_bool;
    }
    switch(voba_get_type1(v)){
    case VOBA_TYPE_SMALL:
        switch(voba_get_type2(v)){
            VOBA_SMALL_TYPES(VOBA_GET_CLASS_SMALL)
        default:
            assert(0); // not implemented.
        }
    case VOBA_TYPE_SYMBOL:
        return voba_cls_symbol;
    case VOBA_TYPE_FUNC:
        return voba_cls_func;
    case VOBA_TYPE_TUPLE:
        return voba_cls_tuple;
    case VOBA_TYPE_CLOSURE:
        return voba_cls_closure;
    case VOBA_TYPE_PAIR:
        return voba_cls_pair;
    case VOBA_TYPE_USER:
        return voba_user_data_class(v);
    case VOBA_TYPE_STRING:
        return voba_cls_str;
    default:
        assert(0);
    }
    return 0;
}
INLINE int voba_is_a(voba_value_t v, voba_value_t cls)
{
    return voba_get_class(v) == cls;
}
INLINE  const char * voba_get_class_name(voba_value_t v1)
{
    voba_value_t s  = voba_get_class(v1);
    voba_cls_t * p = VOBA_CLS(s);
    return p->name;
}
