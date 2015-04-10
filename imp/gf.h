#pragma once
/** @file

generic_function
======================

generic function is a user class, whose value is an array.

@verbatim
+------------------------------------------------+-------+
| data                                   | type2 | type1 |
+------------------------------------------------+-------+
|      unused (38 bits)    index(10bits) | 1010  |  111  |
+------------------------------------------------+-------+
@endverbatim

With traditional object oriented programming languages, e.g. C++,
Java, etc, a virtual function is associated with a class. In lisp
CLOS, a virtual function is associated with the type of all its
arguments. Generic function is in-between the two. A generic function
is a virtual function which is associated with its first argument,

`index` is an index of the global generic function table.

*/
typedef struct voba_gf_s voba_gf_t;
/** data structure for generic function objects*/
struct voba_gf_header_s{
    const char * name;
};
#define VOBA_MAX_NUM_OF_CLS (1024 - sizeof(struct voba_gf_header_s)/sizeof(voba_func_t))
struct voba_gf_s {
    struct voba_gf_header_s header;
    voba_func_t  cls[VOBA_MAX_NUM_OF_CLS];
};
#define VOBA_GF(s)  VOBA_USER_DATA_AS(voba_gf_t *,s)
extern struct voba_gf_s * the_voba_gf_table;
extern int32_t the_voba_gf_table_length;
extern voba_value_t voba_cls_generic_function;
/** @brief create a generic function object */
INLINE voba_value_t voba_make_generic_function(const char * name, voba_func_t default_imp);
/** register a method for a class. 
 *  @return return value is not useful, return \a func anyway.
 */
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func);
/** this is the key concept of generic function, given a generic
 * function object and a class object, we are able to find the registered implementation.
 * @code{.c}
 *    voba_value_t gf_to_string; // create a generic function, named to_string
 *    voba_value_t obj; // any object
 *    voba_gf_add_class(gf_to_string, voba_get_class(obj),func);
 *    voba_apply(gf_to_string, obj); // similiar to invoke func(gf_to_string,obj);
 * @endcode
 */
INLINE voba_func_t voba_gf_lookup(voba_value_t gf, voba_value_t cls);

/* @brief return the index to the global generic function table
 */
INLINE int32_t voba_gf_id(voba_value_t gf) __attribute__((always_inline));
INLINE const char * voba_gf_name(voba_value_t gf);
INLINE voba_func_t voba_gf_cls_func_pointer(voba_value_t gf, voba_value_t cls) __attribute__((always_inline));
