#pragma once
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
void voba_throw_exception(voba_value_t v);
voba_value_t voba_try_catch(voba_value_t fun_body, voba_value_t fun_catch);

#define VOBA_THROW(...) voba_throw_exception(                           \
        voba_make_string(                                               \
            VOBA_STRCAT(                                                \
                VOBA_CONST_CHAR(__FILE__),                              \
                VOBA_CONST_CHAR(":"),                                   \
                voba_str_fmt_uint32_t(__LINE__,10),                     \
                VOBA_CONST_CHAR(": error: "),                           \
                __VA_ARGS__)))


