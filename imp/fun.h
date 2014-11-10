#pragma once
/* class function
   ==============

+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |
  `----> address of a function.

  for an s-exp, e.g. (f a1 a2 a3 ...)
  `self` is `f` and `args` is an tuple , [ a1, a2, a4 , ....]

  `f` could be any callable object, like, a closure, a generic
  function, a function, an tuple, a hash table, or any other user
  defined objects whose class method implements the `voba_gf_apply`

*/
#define VOBA_FUNC __attribute__((aligned(16)))
extern voba_value_t voba_cls_func;
typedef voba_value_t (*voba_func_t)(voba_value_t self, voba_value_t args);
INLINE voba_value_t  voba_make_func(voba_func_t);
INLINE voba_func_t  voba_value_to_func(voba_value_t v);


#ifdef NDEBUG
#define VOBA_ASSERT_N_ARG(args,n)
#else
#define VOBA_ASSERT_N_ARG(args,n)                                       \
if(voba_array_len(args) <= n) {                                         \
    VOBA_THROW(VOBA_CONST_CHAR("unexpected number of argument: "),      \
               voba_str_fmt_uint32_t(n+1,10),                           \
               VOBA_CONST_CHAR(" expected, but given "),                \
               voba_str_fmt_uint32_t(voba_array_len(args),10));         \
}                                                                       
#endif
#ifdef NDEBUG
#define VOBA_ASSERT_CLS(name,cls,n)
#else
#define VOBA_ASSERT_CLS(name,cls,n)                                     \
    if(!voba_is_a(name,cls)){                                           \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #cls "` expected, "            \
                                   "but given value is 0x"),            \
                   voba_str_fmt_uint64_t(name,16));                     \
    }
#endif

#ifdef NDEBUG
#define VOBA_ASSERT_IS(name,is,n)
#else
#define VOBA_ASSERT_IS(name,is,n)                                      \
    if(!(is)(name)){                                                    \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #is "` expected, "             \
                                   "but given value is 0x"),            \
                   voba_str_fmt_uint64_t(name,16));                     \
    }
#endif
