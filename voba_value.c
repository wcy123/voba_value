#include <stdarg.h>
#include "value.h"
#define EXEC_ONCE_TU_NAME "voba_value"
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
VOBA_FUNC static voba_value_t apply_generator(voba_value_t self, voba_value_t args);
EXEC_ONCE_PROGN{
    voba_gf_apply = voba_make_generic_function("apply",NULL);
    voba_gf_add_class(voba_gf_apply,voba_cls_generator, voba_make_func(apply_generator));
}
voba_value_t voba_make_tuple_1_x(voba_value_t x) 
{
    return voba_make_tuple_n(1,x);
}
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

