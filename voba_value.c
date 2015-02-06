/** @file */
#include <stdarg.h>
#include "value.h"
#define EXEC_ONCE_TU_NAME "voba.value"
#include "exec_once.h"
uint32_t voba_cls_generic_function_next_id = 0;
uint32_t voba_cls_next_id = 0;
// it is defined in a special way, to avoid chicken-egg problem
voba_value_t voba_cls_cls = VOBA_UNDEF;
VOBA_DEF_CLS(0,func)
VOBA_DEF_CLS(0,symbol)
VOBA_DEF_CLS(0,tuple)
VOBA_DEF_CLS(0,closure)
VOBA_DEF_CLS(0,pair)
VOBA_DEF_CLS(0,str)
VOBA_DEF_CLS(0,nil)
VOBA_DEF_CLS(0,bool)
VOBA_DEF_CLS(0,u8)
VOBA_DEF_CLS(0,i8)
VOBA_DEF_CLS(0,u16)
VOBA_DEF_CLS(0,i16)
VOBA_DEF_CLS(0,u32)
VOBA_DEF_CLS(0,i32)
VOBA_DEF_CLS(0,float)
VOBA_DEF_CLS(0,short_symbol)
VOBA_DEF_CLS(0,undef);
VOBA_DEF_CLS(0,done);
VOBA_DEF_CLS(voba_sizeof_hashtable(),hashtable)
VOBA_DEF_CLS(voba_sizeof_symbol_table(),symbol_table)
VOBA_DEF_CLS(sizeof(voba_array_t),array)
VOBA_DEF_CLS(sizeof(voba_gf_t),generic_function)
VOBA_DEF_CLS(sizeof(voba_la_t),la)
VOBA_DEF_CLS(sizeof(cg_t),generator)

voba_value_t voba_gf_apply = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_apply = voba_make_generic_function("apply",NULL);
}
voba_value_t voba_gf_iter = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_iter = voba_make_generic_function("iter",NULL);
}
voba_value_t voba_gf_match = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_match = voba_make_generic_function("match",NULL);
}


/** @brief Objects of ::voba_cls_generator are callable objects
 @todo explain generator in details
 */
VOBA_FUNC static voba_value_t apply_generator(voba_value_t self, voba_value_t args)
{
    voba_value_t g = self;
    voba_value_t ret = VOBA_NIL;
    if(cg_is_done(VOBA_GENERATOR(g))){
        ret = VOBA_DONE;
    }else{
        VOBA_ASSERT_ARG_ISA(g,voba_cls_generator,0);
        int64_t len = voba_tuple_len(args);
        voba_value_t v = VOBA_UNDEF;
        if(len > 0){
            v = voba_tuple_at(args,0);
        }
        ret = cg_invoke(VOBA_GENERATOR(g),v);
    }
    return ret;
}
EXEC_ONCE_PROGN {
    voba_gf_add_class(voba_gf_apply,voba_cls_generator, voba_make_func(apply_generator));
}

/** @brief An array is callable.
    
`(array_obj index)` returns the element in the array at `index`.

  */
VOBA_FUNC static voba_value_t apply_array(voba_value_t self, voba_value_t args)
{
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t index1 = voba_tuple_at(args,0);
    VOBA_ASSERT_ARG_FUN(index1,voba_is_int,0);
    int64_t index = (index1 >> 8);
    return voba_array_at(self,index);
}
/** @brief The closure for iterator of an array */
VOBA_FUNC static voba_value_t iter_array_real(voba_value_t self, voba_value_t args)
{
    voba_value_t a = voba_tuple_at(self,0);
    voba_value_t i = voba_tuple_at(self,1);
    int64_t len = voba_array_len(a);
    voba_value_t ret = VOBA_DONE;
    if(i < len){
        ret = voba_array_at(a, i);
        i++;
        voba_tuple_set(self,1,i);
    }
    return ret;
}
/** @brief Array is iterable */
VOBA_FUNC static voba_value_t iter_array(voba_value_t self, voba_value_t args)
{
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t a = voba_tuple_at(args,0);
    VOBA_ASSERT_ARG_ISA(a,voba_cls_array,0);
    return voba_make_closure_2(iter_array_real,a,0);
}
EXEC_ONCE_PROGN {
    voba_gf_add_class(voba_gf_apply,voba_cls_array, voba_make_func(apply_array));
    voba_gf_add_class(voba_gf_iter, voba_cls_array, voba_make_func(iter_array));
}
/** @brief function and closure are iterable */
VOBA_FUNC voba_value_t iter_func (voba_value_t self, voba_value_t args)
{
    return voba_tuple_at(args,0);
}
EXEC_ONCE_PROGN {
    voba_gf_add_class(voba_gf_iter, voba_cls_func, voba_make_func(iter_func));
    voba_gf_add_class(voba_gf_iter, voba_cls_closure, voba_make_func(iter_func));
}


/** @brief match for builtin classes
 @todo add document for this function.
 */
VOBA_FUNC voba_value_t match_single(voba_value_t self, voba_value_t args) 
{
    voba_value_t ret = VOBA_FALSE;
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t cls = voba_tuple_at(args,0);
    VOBA_ASSERT_N_ARG(args,1);
    voba_value_t v = voba_tuple_at(args,1);
    VOBA_ASSERT_N_ARG(args,2);
    voba_value_t index = voba_tuple_at(args,2);
    VOBA_ASSERT_N_ARG(args,3);
    voba_value_t len = voba_tuple_at(args,3);
    int32_t index1 = voba_value_to_i32(index);
    int32_t len1 = voba_value_to_i32(len);
    switch(index1){
    case -1:
        if(len1 == 1 && voba_is_a(v,cls)){
            ret = VOBA_TRUE;
        }
        break;
    case 0:
        assert(len1 == 1);
        ret = v;
        break;
    default:
        assert(0&&"never goes here");
    }
    return ret;
}
EXEC_ONCE_PROGN {
    voba_gf_add_class(voba_gf_match,voba_cls_i8,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_i16,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_i32,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_u8,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_u16,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_u32,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_float,voba_make_func(match_single));
    voba_gf_add_class(voba_gf_match,voba_cls_str,voba_make_func(match_single));
}

/** 
 @todo add match for ::voba_cls_pair
 */

