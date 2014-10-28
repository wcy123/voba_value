#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <gc.h>
#ifndef INLINE
#define INLINE static inline 
#endif

#include "voba_str.h"
#include "data_type_imp.h"

#ifdef __cplusplus
#include <exception>
namespace voba {
  class exception : std::exception{
  public:
    explicit exception(voba_value_t v):value(v){}
    virtual const char* what() const throw();
    voba_value_t value;
  };
  const char* exception::what() const throw()
  {
    static char buf[128];
    snprintf(buf,128,"exception %ld(0x%lx)",value,value);
    return buf;
  }
}
#endif    
void    voba_throw_exception(voba_value_t v);
voba_value_t voba_try_catch(voba_value_t fun_body, voba_value_t fun_catch);
#include "data_type_imp.c"
#ifdef __cplusplus
}
#endif



// some useful macros
#define VOBA_FUNC __attribute__((aligned(16)))

#define VOBA_THROW(...) voba_throw_exception(                           \
        voba_make_string(                                               \
            VOBA_STRCAT(                                                \
                VOBA_CONST_CHAR(__FILE__),                              \
                VOBA_CONST_CHAR(":"),                                   \
                voba_str_fmt_uint32_t(__LINE__,10),                     \
                VOBA_CONST_CHAR(": error: "),                           \
                __VA_ARGS__)))

#define VOBA_DEF_ARG3(name,args,n)                                      \
if(voba_array_len(args) <= n) {                                         \
    VOBA_THROW(VOBA_CONST_CHAR("unexpected number of argument: "),      \
               voba_str_fmt_uint32_t(n+1,10),                           \
               VOBA_CONST_CHAR(" expected, but given "),                \
               voba_str_fmt_uint32_t(voba_array_len(args),10));         \
}                                                                       \
voba_value_t name = voba_array_at(args,n);                              

#define VOBA_DEF_ARG4(type,name,args,n)                                 \
    VOBA_DEF_ARG3(name,args,n);                                         \
    if(!voba_is_a(name,type)){                                          \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #type "` expected, but given value is 0x"), \
                   voba_str_fmt_uint64_t(name,16));                     \
    }

#define VOBA_DEF_OPTIONAL_ARG3(name,args,n,default_value)               \
    voba_value_t name = VOBA_UNDEF;                                     \
    if(n < voba_array_len(args) ) {                                     \
        name = voba_array_at(args,n);                                   \
    }else{                                                              \
        name = (default_value);                                         \
    }                                                                   \

#define VOBA_DEF_OPTIONAL_ARG4(type,name,args,n,default_value)          \
    VOBA_DEF_OPTIONAL_ARG3(name,args,n,default_value);                  \
    if(!voba_is_a(name,type)){                                          \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #type "` expected, but given value is 0x"), \
                   voba_str_fmt_uint64_t(name,16));                     \
    }

#define VOBA_DEF_CVAR(name,self,n)                                      \
    voba_value_t name = voba_array_at(self,n)

#define DEFINE_CLS(xsize,xname)                                         \
    voba_value_t voba_cls_##xname = VOBA_UNDEF;                         \
    EXEC_ONCE_PROGN                                                     \
    {                                                                   \
        if(voba_eq(voba_cls_cls,VOBA_UNDEF)){                           \
            /*                                                          \
               voba_cls_cls itself is a user data too. but we need a    \
               voba_cls to create a user data. so there is a            \
               chicken-egg problem.  voba_allocate_user_data is used    \
               to break the recursive, i.e. `voba_cls_cls` is created   \
               in a special way.                                        \
            */                                                          \
            voba_value_t * p =                                          \
                voba_allocate_user_data(sizeof(voba_cls_t));            \
            voba_cls_cls = voba_from_pointer(p,VOBA_TYPE_USER);         \
            VOBA_CLS(voba_cls_cls)->size = sizeof(voba_cls_t);          \
            VOBA_CLS(voba_cls_cls)->name = "cls";                       \
            p[0] = voba_cls_cls;                                        \
        }                                                               \
        voba_cls_##xname = voba_make_cls(xsize,#xname);                 \
    }                                                                


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
