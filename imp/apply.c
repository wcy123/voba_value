#include "apply.h"

INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args)
{
    return f(voba_make_func(f),args);
}
INLINE voba_value_t voba_apply_gf(voba_value_t gf, voba_value_t args)
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t a1 = voba_tuple_at(args,0);
    voba_value_t cls = voba_get_class(a1);
    voba_func_t vf = voba_gf_lookup(gf,cls);
    voba_value_t ret = VOBA_NIL;
    if(vf){
	ret = vf(gf,args);
    }else{
	voba_throw_exception(
	    voba_make_string(
		VOBA_STRCAT(
		    VOBA_CONST_CHAR("vfunc `"),
		    voba_str_from_cstr(voba_gf_name(gf)),
		    VOBA_CONST_CHAR("' is not found for cls `"),
		    voba_str_from_cstr(voba_cls_name(cls)),
		    VOBA_CONST_CHAR("'")
		    )));
    }
    return ret;
}
INLINE voba_value_t voba_apply_user_object(voba_value_t f, voba_value_t a1)
{
    voba_value_t ret = VOBA_NIL;
    voba_value_t self  = f;
    voba_value_t cls = voba_get_class(self);
    voba_func_t vf = voba_gf_lookup(voba_gf_apply,cls);
    if(vf){
	ret = vf(f,a1);
    }else{
	voba_throw_exception(
	    voba_make_string(
		VOBA_STRCAT(
		    VOBA_CONST_CHAR("object of `"),
		    voba_str_from_cstr(voba_cls_name(cls)),
		    VOBA_CONST_CHAR("' is not callable`")
		    )));
    }
    return ret;
}
/*
The callable objects are

1. function

   call it directly. no memory access.

2. closure

   closure is a pointer to a function, call it indirectly immediately. one memory access.
   
3. generic function.

   It is a index of the generic function table.
   1. access the first argument
   2. access the class id of first argument
   3. access the global table.

   Old method, no detail, too slow.

4. user defined class.

 */
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a1)
{
    voba_value_t ret = VOBA_NIL;
    assert(voba_is_a(a1,voba_cls_tuple));
    switch(voba_get_type1(f)){
    case VOBA_TYPE_FUNC:
        ret = voba_direct_apply(voba_value_to_func(f),a1); break;
    case VOBA_TYPE_CLOSURE:
        ret = voba_closure_func(f)(voba_closure_tuple(f),a1); break;
    case VOBA_TYPE_TUPLE:
        ret = apply_tuple(f,a1); break;
    case VOBA_TYPE_SMALL:
	switch(voba_get_type2(f)){
	case VOBA_TYPE_GENERIC_FUNCTION:
	    ret = voba_apply_gf(f,a1); break;
	default:
	    assert(0);
	}
	break;
    case VOBA_TYPE_USER:
	ret = voba_apply_user_object(f,a1); break;
    default:
	assert(0);
    }
    return ret;
}

INLINE voba_value_t apply_tuple(voba_value_t self, voba_value_t args)
{
    voba_value_t ret = VOBA_NIL;
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t index1 = voba_tuple_at(args,0);
    VOBA_ASSERT_ARG_ISA(index1,voba_cls_i32,0);
    int64_t index = (index1 >> 8);
    int64_t len = voba_tuple_len(self);
    if(index >= len){
	voba_throw_exception(voba_make_string(VOBA_CONST_CHAR("out of range")));
    }
    ret = voba_tuple_at(self,index);
    return ret;
}
