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

voba_value_t voba_gf_apply = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_apply = voba_make_generic_function("apply",NULL);
}




