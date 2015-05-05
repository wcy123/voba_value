/** @file */
#include <stdarg.h>
#include "value.h"
#define EXEC_ONCE_TU_NAME "voba.value"
#include "exec_once.h"
uint32_t voba_cls_generic_function_next_id = 0;
uint32_t voba_cls_next_id = 0;
// it is defined in a special way, to avoid chicken-egg problem
voba_value_t voba_cls_cls = VOBA_UNDEF;
voba_cls_t * the_voba_class_table = NULL;
int32_t the_voba_class_table_length = 1; // reserver types
EXEC_ONCE_PROGN{
    /// reserve the first user defined object is the string class.
    the_voba_class_table = (struct voba_cls_s*)
	GC_MALLOC(sizeof(struct voba_cls_s)*the_voba_class_table_length);
    the_voba_class_table[0].size = sizeof(voba_str_t) - sizeof(voba_value_t);
    the_voba_class_table[0].name = "string";
}
VOBA_DEF_CLS(0,func)
VOBA_DEF_CLS(0,box)
VOBA_DEF_CLS(0,symbol)
VOBA_DEF_CLS(0,tuple)
VOBA_DEF_CLS(0,closure)
VOBA_DEF_CLS(0,pair)
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
voba_value_t voba_gf_to_string = VOBA_NIL;
EXEC_ONCE_PROGN{
    voba_gf_to_string = voba_make_generic_function("to_string",NULL);
}

/** @brief Objects of ::voba_cls_generator are callable objects
 @todo explain generator in details
 */
VOBA_FUNC static voba_value_t apply_generator(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t g = fun;
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
VOBA_FUNC static voba_value_t apply_array(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t ret = VOBA_NIL;
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t index1 = voba_tuple_at(args,0);
    VOBA_ASSERT_ARG_ISA(index1,voba_cls_i32,0);
    int64_t index = (index1 >> 8);
    ret = voba_array_at(fun,index);
    return ret;
}
EXEC_ONCE_PROGN {
    voba_gf_add_class(voba_gf_apply,voba_cls_array, voba_make_func(apply_array));
}
/* VOBA_FUNC static voba_value_t apply_tuple(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[]) */
/* { */
/*     int64_t len = voba_tuple_len(args); */
/*     voba_value_t ret = VOBA_NIL; */
/*     if(len == 0){ */
/*         VOBA_ASSERT_ARG_ISA(self,voba_cls_tuple,0); */
/*         ret = voba_make_closure_2(iter_tuple_real,self,0); */
/*     }else{ */
/*         voba_value_t index1 = voba_tuple_at(args,0); */
/*         VOBA_ASSERT_ARG_ISA(index1,voba_cls_i32,0); */
/*         int64_t index = (index1 >> 8); */
/*         ret = voba_tuple_at(self,index); */
/*     } */
/*     return ret; */
/* } */
/** @brief The closure for iterator of an tuple */
/* VOBA_FUNC static voba_value_t iter_tuple_real(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[]) */
/* { */
/*     voba_value_t a = voba_tuple_at(self,0); */
/*     voba_value_t i = voba_tuple_at(self,1); */
/*     int64_t len = voba_tuple_len(a); */
/*     voba_value_t ret = VOBA_DONE; */
/*     if(i < len){ */
/*         ret = voba_tuple_at(a, i); */
/*         i++; */
/*         voba_tuple_set(self,1,i); */
/*     } */
/*     return ret; */
/* } */
/** 
 @todo add match for ::voba_cls_pair
 */

VOBA_FUNC static voba_value_t string_to_string(voba_value_t fun,voba_value_t v, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t* ret = voba_str_from_char('"',1);
    ret = voba_strcat(ret,voba_value_to_str(voba_tuple_at(v,0)));
    ret = voba_strcat_char(ret,'"');
    return voba_make_string(ret);
}
/** @brief to_string implementation of arrays.
 */
VOBA_FUNC static  voba_value_t array_to_string(voba_value_t fun,voba_value_t vs, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t v = voba_tuple_at(vs,0);
    int64_t len = voba_array_len(v);
    voba_str_t* ret = voba_str_empty();
    ret = voba_strcat_char(ret,'[');
    for(int i = 0; i < len ; ++i){
        if(i!=0){
            ret = voba_strcat_char(ret,',');
        }
        voba_value_t args[] = {1,voba_array_at(v,i),VOBA_BOX_END};
        ret = voba_strcat(ret,
                          voba_value_to_str(voba_apply(voba_gf_to_string,voba_make_tuple(args))));
    }
    ret = voba_strcat_char(ret,']');
    return voba_make_string(ret);
}
VOBA_FUNC static  voba_value_t tuple_to_string(voba_value_t fun,voba_value_t vs, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t v = voba_tuple_at(vs,0);
    int64_t len = voba_tuple_len(v);
    voba_str_t* ret = voba_str_empty();
    ret = voba_strcat_char(ret,'[');
    for(int i = 0; i < len ; ++i){
        if(i!=0){
            ret = voba_strcat_char(ret,',');
        }
        voba_value_t args[] = {1,voba_tuple_at(v,i),VOBA_BOX_END};
        ret = voba_strcat(ret,
                          voba_value_to_str(voba_apply(voba_gf_to_string,voba_make_tuple(args))));
    }
    ret = voba_strcat_char(ret,']');
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t int_to_string (voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    VOBA_ASSERT_N_ARG(args,0);
    voba_value_t a1 = voba_tuple_at(args,0);
    int64_t a2  = voba_value_to_i32(a1);
    voba_str_t* a3 = voba_str_fmt_int64_t(a2,10);
    voba_value_t ret = voba_make_string(a3);
    return ret;
}
VOBA_FUNC static voba_value_t funcp_to_string(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t * ret = voba_str_empty();
    ret = voba_strcat_cstr(ret,"<funcp ");
    ret = voba_strcat(ret,voba_str_fmt_pointer(((void*)voba_tuple_at(args,0))));
    ret = voba_strcat_cstr(ret," >");    
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t closure_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t *ret = voba_str_empty();
    ret = voba_strcat_cstr(ret,"<closure ");
    ret = voba_strcat(ret,voba_str_fmt_pointer((void*) (intptr_t)voba_closure_func(voba_tuple_at(args,0))));
    ret = voba_strcat_char(ret,',');
    ret = voba_strcat(ret,voba_str_fmt_uint32_t(voba_tuple_len(voba_closure_tuple(voba_tuple_at(args,0))),10));
    ret = voba_strcat_char(ret,'>');
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t pair_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t tmp_args[] = {1, VOBA_NIL,VOBA_BOX_END};
    voba_value_t v = voba_tuple_at(args,0);
    voba_str_t* ret = voba_str_empty();
    ret = voba_strcat_char(ret,'(');
    int i = 0; 
    voba_value_t x = v;
    while(1){ // circular pair would cause infinite loop;
        if(i!=0){
            ret = voba_strcat_char(ret,',');
        }
        tmp_args[1] = voba_head(x);
        ret = voba_strcat(ret,
                          voba_value_to_str(
                              voba_apply(voba_gf_to_string,voba_make_tuple(tmp_args))));
        x = voba_tail(x);
        if(voba_is_nil(x)){
            break;
        }else if(!voba_is_a(x,voba_cls_pair)){
            ret = voba_strcat_char(ret,'.');
            tmp_args[1] = x;
            ret = voba_strcat(ret,
                              voba_value_to_str(
                                  voba_apply(voba_gf_to_string,voba_make_tuple(tmp_args))));
            break;
        }
        i ++;
    }
    ret = voba_strcat_char(ret,')');
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t la_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t tmp_args[] = {1, VOBA_NIL,VOBA_BOX_END};
    voba_value_t v = voba_tuple_at(args,0);
    voba_str_t* ret = voba_str_empty();
    ret = voba_strcat_char(ret,'[');
    int i = 0; 
    voba_value_t x = voba_la_copy(v);
    while(1){ // circular la would cause infinite loop;
        if(i!=0){
            ret = voba_strcat_char(ret,',');
        }
        tmp_args[1] = voba_la_car(x);
        ret = voba_strcat(ret,
                          voba_value_to_str(
                              voba_apply(voba_gf_to_string,voba_make_tuple(tmp_args))));
        x = voba_la_cdr(x);
        if(voba_la_is_nil(x)){
            break;
        }
        i ++;
    }
    ret = voba_strcat_char(ret,']');
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t nil_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t *ret = voba_str_from_cstr("nil");
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t undef_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t *ret = voba_str_from_cstr("undef");
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t done_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_str_t *ret = voba_str_from_cstr("done");
    return voba_make_string(ret);
}

VOBA_FUNC static voba_value_t boolean_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t v = voba_tuple_at(args,0);
    if(voba_is_true(v)){
        voba_str_t *ret = voba_str_from_cstr("TRUE");
        return voba_make_string(ret);
    }
    if(voba_is_false(v)){
        voba_str_t *ret = voba_str_from_cstr("FALSE");
        return voba_make_string(ret);
    }
    assert(0);
    voba_str_t *ret = voba_str_from_cstr("unknown");
    return voba_make_string(ret);
}
VOBA_FUNC static voba_value_t symbol_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t s = voba_tuple_at(args,0);
    return voba_make_string(voba_vstrcat(
                                voba_strdup(voba_value_to_str(voba_symbol_name(s))),
                                VOBA_CONST_CHAR("@0x"),
                                voba_str_fmt_uint64_t(s,16),NULL));
}
VOBA_FUNC static voba_value_t box_to_string(voba_value_t fun,voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
    voba_value_t box = voba_tuple_at(args,0);
    voba_value_t tmp_args[] = {1, voba_unbox(box),VOBA_BOX_END};
    return voba_make_string(voba_vstrcat(
                                voba_str_from_cstr("#b["),
				voba_value_to_str(
				    voba_apply(voba_gf_to_string,voba_make_tuple(tmp_args))),
				voba_str_from_cstr("]"),NULL));
}
EXEC_ONCE_PROGN{
    voba_gf_add_class(voba_gf_to_string,voba_cls_str,voba_make_func(string_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_array,voba_make_func(array_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_tuple,voba_make_func(tuple_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_i8,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_i16,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_i32,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_u8,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_u16,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_u32,voba_make_func(int_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_func,voba_make_func(funcp_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_closure,voba_make_func(closure_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_nil,voba_make_func(nil_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_pair,voba_make_func(pair_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_la,voba_make_func(la_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_undef,voba_make_func(undef_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_done,voba_make_func(done_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_bool,voba_make_func(boolean_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_symbol,voba_make_func(symbol_to_string));
    voba_gf_add_class(voba_gf_to_string,voba_cls_box,voba_make_func(box_to_string));
}
static voba_value_t print1(voba_value_t x)
{
    voba_value_t args[] = {1,x,VOBA_BOX_END};
    voba_value_t s = voba_apply(voba_gf_to_string,voba_make_tuple(args));
    fwrite((const void*)voba_str_to_cstr(voba_value_to_str(s)),1,voba_strlen(voba_value_to_str(s)),stdout);
    fflush(stdout);
    return x;
}
VOBA_FUNC voba_value_t voba_print(voba_value_t fun, voba_value_t a1, voba_value_t* next_fun, voba_value_t next_args[]) 
{
    int64_t len = voba_tuple_len(a1);
    for(int i = 0; i < len; ++i){
        print1(voba_tuple_at(a1,i));
        if(i!=0){
             fputc(' ',stdout);
        }
    }
    fputc('\n',stdout);
    return VOBA_NIL;
}


voba_value_t voba_default_symbol_table;
voba_value_t voba_constant_symbol_table;
EXEC_ONCE_PROGN {
    voba_default_symbol_table = voba_make_symbol_table();
    voba_constant_symbol_table = voba_make_symbol_table();
}

voba_gf_t * the_voba_gf_table = NULL;
int32_t the_voba_gf_table_length = 0;
