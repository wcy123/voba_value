#define INLINE
#include "value.h"
#include "exec_once.h"
#ifdef __cplusplus
extern "C" {
#endif
size_t voba_hashtable_size();
size_t voba_symbol_table_size();
#ifdef __cplusplus
}
#endif



DEFINE_CLS(0,func)
DEFINE_CLS(sizeof(voba_value_t),generic_function)
DEFINE_CLS(0,symbol)
DEFINE_CLS(0,array)
DEFINE_CLS(0,closure)
DEFINE_CLS(0,pair)
DEFINE_CLS(sizeof(voba_class_t),user)
DEFINE_CLS(0,str)
DEFINE_CLS(0,nil)
DEFINE_CLS(0,bool)
DEFINE_CLS(0,u8)
DEFINE_CLS(0,i8)
DEFINE_CLS(0,u16)
DEFINE_CLS(0,i16)
DEFINE_CLS(0,u32)
DEFINE_CLS(0,i32)
DEFINE_CLS(0,float)
DEFINE_CLS(0,short_symbol)
DEFINE_CLS(voba_hashtable_size(),hashtable)
DEFINE_CLS(voba_hashtable_size(),symbol_table)
DEFINE_CLS(sizeof(voba_la_t),la)

voba_value_t voba_gf_apply = VOBA_NIL;
EXEC_ONCE_DO(voba_gf_apply = voba_make_generic_function(););

EXEC_ONCE_START



