#pragma once
/** @file fun.h
 * Basic type of function object

 * A function object is a function pointer, the assocated 3-bit type
 * tag is 000.

\verbatim
+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |
  `----> address of a function.
\endverbatim

The type of function pointer is typedef as ::voba_func_t
@code{.c} typedef voba_value_t (*voba_func_t)(voba_value_t self, voba_value_t args); \endcode

The function must be 16 aligned, usually use ::VOBA_FUNC
For example:
@code{.c}
VOBA_FUNC static voba_value_t a_function(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[])
{
   .......
}
@endcode

For an s-exp, 
@code{.el}
(f a1 a2 a3 ...)
@endcode

where \a self is \a f and \a args is a tuple \a [ \a a1, \a a2, \a a4 , .... \a ]

  \a f could be any callable object, like, a closure, a generic
  function, a function, an tuple, a hash table, or any other user
  defined objects whose class method implements the ::voba_gf_apply

*/

#define VOBA_FUNC __attribute__((aligned(16))) /*!< a voba function must be 16-byte aligned*/
/** @brief the class object associated with function type. */
extern voba_value_t voba_cls_func;
/** @brief the type of function pointer */
typedef voba_value_t (*voba_func_t)(voba_value_t fun, voba_value_t args, voba_value_t* next_fun, voba_value_t next_args[]);
/** @brief create a new function object from a function pointer */
INLINE voba_value_t  voba_make_func(voba_func_t);
/** @brief convert a voba value to a function pointer */
INLINE voba_func_t  voba_value_to_func(voba_value_t v);

/** useful macros for a function */

/** assert that nth argument must be available
 *  @param args the second argument of function object, a tuple
 *  @param n index of the tuple \a args
 */
#ifdef NDEBUG
#define VOBA_ASSERT_N_ARG(args,n)
#else
#define VOBA_ASSERT_N_ARG(args,n)                                       \
if(voba_tuple_len(args) <= n) {                                         \
    VOBA_THROW(VOBA_CONST_CHAR("unexpected number of argument: "),      \
               voba_str_fmt_uint32_t(n+1,10),                           \
               VOBA_CONST_CHAR(" expected, but given "),                \
               voba_str_fmt_uint32_t(voba_tuple_len(args),10));         \
}                                                                       
#endif

/** assert \a n th argument is a class of \a cls
 * @param name a variable name
 * @param cls a class object
 * @param n index of the tuple, for error reporting. */
#ifdef NDEBUG
#define VOBA_ASSERT_ARG_ISA(name,cls,n)
#else
#define VOBA_ASSERT_ARG_ISA(name,cls,n)                                     \
    if(!voba_is_a(name,cls)){                                           \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #cls "` expected, "            \
                                   "but given value is 0x"),            \
                   voba_str_fmt_uint64_t(name,16));                     \
    }
#endif

/** assert applying \a f to \a n th argument returns true
 * @param name a variable name
 * @param f a function pointer
 * @param n index of the tuple, for error reporting. 
 */
#ifdef NDEBUG
#define VOBA_ASSERT_ARG_FUN(name,f,n)
#else
#define VOBA_ASSERT_ARG_FUN(name,f,n)                                   \
    if(!(f)(name)){                                                    \
        VOBA_THROW(VOBA_CONST_CHAR("wrong type of argument #") ,        \
                   voba_str_fmt_uint32_t(n,10),                         \
                   VOBA_CONST_CHAR(": `" #f "` expected, "              \
                                   "but given value is 0x"),            \
                   voba_str_fmt_uint64_t(name,16));                     \
    }
#endif
