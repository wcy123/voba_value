#define INLINE
#include <stdarg.h>
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

voba_value_t voba_array_fixed_vconcat(voba_value_t a,...)
{
    int n = 0;
    va_list ap;
    va_start(ap, a);
    voba_value_t args[100];
    voba_value_t s = 0;
    args[0] = a;
    for(n = 1, s = va_arg(ap,voba_value_t);
        n < 100 && !voba_is_nil(s);
        n++, s = va_arg(ap,voba_value_t)){
        assert(voba_is_array(s));
        args[n] = s;
    }
    assert(n!=100);
    va_end(ap);
    int64_t len = 0;
    for(int i = 0; i < n; i++){
        len += voba_array_len(args[i]);
    }
    voba_value_t * p = GC_MALLOC(sizeof(voba_value_t)*(len+1));
    p[0] = len;
    voba_value_t * p2 = &p[1];
    for(int i = 0; i < n; i++){
        int64_t len_i = voba_array_len(args[i]);
#ifndef NDEBUG        
        void * tmp =
#endif
            memcpy(p2, voba_array_base(args[i]), len_i * sizeof(voba_value_t));
        assert(tmp);
        p2 += len_i;
    }
    assert(p2 - p == (len + 1));
    return voba_make_array(p);
}
/* voba_value_t voba_la_vconcat(voba_value_t la1,...) */
/* { */
/*     int n = 0; */
/*     va_list ap; */
/*     va_start(ap, la1); */
/*     voba_value_t args[100]; */
/*     voba_value_t s = 0; */
/*     args[0] = a; */
/*     for(n = 1, s = va_arg(ap,voba_value_t); */
/*         n < 100 && !voba_is_nil(s); */
/*         n++, s = va_arg(ap,voba_value_t)){ */
/*         assert(voba_is_array(s)); */
/*         args[n] = s; */
/*     } */
/*     assert(n!=100); */
/*     va_end(ap); */
 
/* } */
