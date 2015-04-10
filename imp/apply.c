#include "apply.h"

INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args,voba_value_t* next_fun, voba_value_t next_args[])
{
    return f(voba_make_func(f),args,next_fun,next_args);
}
INLINE voba_value_t voba_apply_gf(voba_value_t gf, voba_value_t args,voba_value_t* next_fun, voba_value_t next_args[])
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t a1 = voba_tuple_at(args,0);
    voba_value_t cls = voba_get_class(a1);
    voba_func_t vf = voba_gf_lookup(gf,cls);
    voba_value_t ret = VOBA_NIL;
    if(vf){
	ret = vf(gf,args,next_fun,next_args);
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
INLINE voba_value_t voba_apply_user_object(voba_value_t f, voba_value_t a1,voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t ret = VOBA_NIL;
    voba_value_t self  = f;
    voba_value_t cls = voba_get_class(self);
    voba_func_t vf = voba_gf_lookup(voba_gf_apply,cls);
    if(vf){
	ret = vf(f,a1,next_fun,next_args);
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
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t args)
{
    voba_value_t ret = VOBA_TAIL_CALL;
    voba_value_t next_fun = VOBA_NIL;
    voba_value_t next_args[VOBA_MAX_NUM_OF_TAIL_CALL_ARGS + 1];
    next_args[0] = -1;
    assert(voba_is_a(args,voba_cls_tuple));
    do{
	switch(voba_get_type1(f)){
	case VOBA_TYPE_FUNC:
	    ret = voba_direct_apply(voba_value_to_func(f),args,&next_fun,next_args); break;
	case VOBA_TYPE_CLOSURE:
	    ret = voba_closure_func(f)(voba_closure_tuple(f),args,&next_fun,next_args); break;
	case VOBA_TYPE_TUPLE:
	    ret = apply_tuple(f,args); break;
	case VOBA_TYPE_SMALL:
	    switch(voba_get_type2(f)){
	    case VOBA_TYPE_GENERIC_FUNCTION:
		ret = voba_apply_gf(f,args,&next_fun,next_args); break;
	    default:
		assert(0);
	    }
	    break;
	case VOBA_TYPE_USER:
	    ret = voba_apply_user_object(f,args,&next_fun,next_args); break;
	default:
	    assert(0);
	}
	f = next_fun;
	args = voba_make_tuple(next_args);
    }while(ret == VOBA_TAIL_CALL);
    return ret;
}

INLINE voba_value_t apply_tuple(voba_value_t fun, voba_value_t args)
{
    voba_value_t ret = VOBA_NIL;
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t index1 = voba_tuple_at(args,0);
    VOBA_ASSERT_ARG_ISA(index1,voba_cls_i32,0);
    int64_t index = (index1 >> 8);
    int64_t len = voba_tuple_len(fun);
    if(index >= len){
	voba_throw_exception(voba_make_string(VOBA_CONST_CHAR("out of range")));
    }
    ret = voba_tuple_at(fun,index);
    return ret;
}
