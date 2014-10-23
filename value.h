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

#define VOBA_DEF_ARG(name,args,n,type)                                  \
if(voba_array_len(args) <= n) {                                         \
    VOBA_THROW(VOBA_CONST_CHAR("unexpected number of argument: "),      \
               voba_str_fmt_uint32_t(n+1,10),                             \
               VOBA_CONST_CHAR(" expected, but given "),                 \
               voba_str_fmt_uint32_t(voba_array_len(args),10));         \
}                                                                       \
voba_value_t name = voba_array_at(args,n);                              \
if(!type(name)){                                                        \
    VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,            \
               voba_str_fmt_uint32_t(n,10),                             \
               VOBA_CONST_CHAR(": `" #type "` expected, but given value is 0x"), \
               voba_str_fmt_uint64_t(name,10));                         \
}

#define VOBA_DEF_CVAR(name,self,n)                                      \
    voba_value_t name = voba_array_at(self,n)

#define DEFINE_CLS(xsize,xname)                                         \
    voba_value_t voba_cls_##xname             = VOBA_NIL;               \
    EXEC_ONCE_PROGN                                                     \
    {                                                                   \
        static voba_class_t cls = { 0, #xname  };                       \
        cls.size = xsize;                                               \
        voba_cls_##xname =                                              \
            voba_make_symbol(voba_str_from_cstr(#xname),VOBA_NIL);      \
        voba_symbol_set_value(voba_cls_##xname,                         \
                              voba_make_class(&cls));                   \
                                                                        \
        return;                                                         \
    }                                                                   


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
