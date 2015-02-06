/** @file
Exception handle
================

Exception handle is supported and implemented via C++ exception mechanism.


*/
#pragma once
#ifdef __cplusplus
#include <exception>
namespace voba {
    /** @brief The exception class for C++, this is only for implemention, C cannot see this part.
        
        voba::exception is a simple wrapper around ::voba_value_t
     */
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
/** @brief Throw an exception.*/
void voba_throw_exception(voba_value_t v);
/** @brief Exception handle.

    Evaluate the function body, when an excaption is throw, invoke the
    exception handler, `fun_catch`

    @param fun_body The function body, it can be any callable object.
    @param fun_catch The exception handle, it can be any callable
    object. `fun_catch` with a single argument, i.e. the exception object,
    any arbitrary ::voba_value_t object could be an exception object.

    @return The return value of `fun_body` is returned if no
    excpetion, otherwise the return value of `fun_catch`.

    @todo This is rather a low level implemention, pattern match need
    to implemented on top of it.
    @todo Do we need a generic exception class, the super class of all exceptions/?
 */
voba_value_t voba_try_catch(voba_value_t fun_body, voba_value_t fun_catch);

/** @brief Throw a string object as an exception 
    
    `VOBA_THROW(s1,s2,...,sn)` where `s_n` is a ::voba_str_t
    @todo how voba source code throw an exception?

*/
#define VOBA_THROW(...) voba_throw_exception(                           \
        voba_make_string(                                               \
            VOBA_STRCAT(                                                \
                VOBA_CONST_CHAR(__FILE__),                              \
                VOBA_CONST_CHAR(":"),                                   \
                voba_str_fmt_uint32_t(__LINE__,10),                     \
                VOBA_CONST_CHAR(": error: "),                           \
                __VA_ARGS__)))


