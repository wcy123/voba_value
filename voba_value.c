#define INLINE
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
VOBA_DEF_CLS(0,array)
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
VOBA_DEF_CLS(sizeof(voba_gf_t),generic_function)
VOBA_DEF_CLS(sizeof(voba_la_t),la)

voba_value_t voba_gf_apply = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_apply = voba_make_generic_function("apply",NULL);
}

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
        assert(voba_is_a(s,voba_cls_array));
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
voba_value_t voba_direct_apply_n(voba_func_t f,size_t n, ...)
{
    voba_value_t args[n] ; //= {(voba_value_t)n , 0 };
    size_t i = 0;
    args[i++] = (voba_value_t) n;
    va_list ap;
    va_start(ap, n);
    for(; i < n + 1; ++i){
        args[i] = va_arg(ap,voba_value_t);
    }
    va_end(ap);
    return f(voba_make_func(f),voba_make_array(args));
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




