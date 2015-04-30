#include "cls.h"

INLINE voba_value_t voba_make_cls(size_t size, const char * name)
{
    assert(the_voba_class_table != NULL);
    the_voba_class_table = (struct voba_cls_s*)
	GC_REALLOC(the_voba_class_table,
		   sizeof(struct voba_cls_s)*(the_voba_class_table_length+1));
    if(!the_voba_class_table) abort(); // out-of-memory
    the_voba_class_table[the_voba_class_table_length].size = size;
    the_voba_class_table[the_voba_class_table_length].name = name;
    voba_value_t ret = VOBA_TYPE_SMALL + (VOBA_TYPE_CLASS*8) + (the_voba_class_table_length * 256);
    the_voba_class_table_length ++;
    if(the_voba_class_table_length >= VOBA_MAX_NUM_OF_CLS){
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
	case VOBA_TYPE_BOOL: return voba_cls_bool;
	case VOBA_TYPE_I32: return voba_cls_i32;
	case VOBA_TYPE_FLOAT: return voba_cls_float;
	case VOBA_TYPE_SHORT_SYMBOL: return voba_cls_short_symbol;
	case VOBA_TYPE_CLASS: return voba_cls_cls;
	case VOBA_TYPE_GENERIC_FUNCTION: return voba_cls_generic_function;
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
    case VOBA_TYPE_BOX:
        return voba_cls_box;
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
    voba_value_t cls = voba_get_class(v1);
    return voba_cls_name(cls);
}
INLINE int32_t voba_class_id(voba_value_t cls)
{
    assert((cls%256) == VOBA_TYPE_SMALL + (VOBA_TYPE_CLASS*8));
    return cls / 256;
}
INLINE size_t voba_cls_size(voba_value_t cls)
{
    int32_t index = voba_class_id(cls);
    assert(index < the_voba_class_table_length);
    return the_voba_class_table[index].size;
}
INLINE const char* voba_cls_name(voba_value_t cls)
{
    int32_t index = voba_class_id(cls);
    assert(index < the_voba_class_table_length);
    return the_voba_class_table[index].name;
}
