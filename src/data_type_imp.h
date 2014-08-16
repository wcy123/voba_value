#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <gc.h>
// pointer to a function pointer can be casted voba_value_t directly
// macro definitions for type1
#define    VOBA_TYPE_FUNC    0
#define    VOBA_TYPE_SYMBOL  1
#define    VOBA_TYPE_ARRAY   2
#define    VOBA_TYPE_CLOSURE 3
#define    VOBA_TYPE_PAIR    4
#define    VOBA_TYPE_USER    5
#define    VOBA_TYPE_STRING  6
#define    VOBA_TYPE_SMALL   7
#define    VOBA_TYPE_NIL     8 // virtual type, only one value
// 3bits for type1
#define    VOBA_TYPE_MASK    7 // 3 bits


// voba_value_t is 64bits.
/*
+-----------------------------------------------------------+-------+
| data                                                      | type  |
+-----------------------------------------------------------+-------+
| 61bits                                                    | 3bits |
+-----------------------------------------------------------+-------+
3bits type could be:
VOBA_FUNC, VOBA_STRING, VOBA_CLOSURE, VOBA_ARRAY, VOBA_HASH, VOBA_SMALL
`voba_get_type1` return 3-bits type.

when it is VOBA_SMALL,

+-----------------------------------------------------------+-------+
| data                                           |    type2 | type  |
+-----------------------------------------------------------+-------+
| 48bits                                         |    5bits | 3bits |
+-----------------------------------------------------------+-------+
`voba_get_type2` return 5-bits type.
*/
// macro definitions for type2, i.e. when type1 == VOBA_TYPE_SMALL
#define    VOBA_TYPE_BOOL         1
#define    VOBA_TYPE_I8           2
#define    VOBA_TYPE_U8           3
#define    VOBA_TYPE_I16          4
#define    VOBA_TYPE_U16          5
#define    VOBA_TYPE_I32          6
#define    VOBA_TYPE_U32          7
#define    VOBA_TYPE_FLOAT        8
#define    VOBA_TYPE_SHORT_SYMBOL 9
#define    VOBA_TYPE_SECRET       10   // internal used, which is not visible

typedef int64_t voba_value_t;
typedef struct vclass_s {
    size_t size;
    const char * name;
} voba_class_t;
extern voba_value_t voba_cls_func;
extern voba_value_t voba_cls_generic_function;
extern voba_value_t voba_cls_symbol;
extern voba_value_t voba_cls_array;
extern voba_value_t voba_cls_closure;
extern voba_value_t voba_cls_pair;
extern voba_value_t voba_cls_user;
extern voba_value_t voba_cls_str;
extern voba_value_t voba_cls_nil;
extern voba_value_t voba_cls_bool;
extern voba_value_t voba_cls_u8;
extern voba_value_t voba_cls_i8;
extern voba_value_t voba_cls_u16;
extern voba_value_t voba_cls_i16;
extern voba_value_t voba_cls_u32;
extern voba_value_t voba_cls_i32;
extern voba_value_t voba_cls_float;
extern voba_value_t voba_cls_short_symbol;
/*
  any valid VOBA function prototype. `args` is an array of arguments.
  `self` is determined by the first argument of `apply`

     - when it is a function, `self` is the function itself. We can
     invoke recursive call as below.

         self(self, args)
         or apply(self,args)

     - when it is a closure, `self` is the closure itself.
        - we can access captured variables

     - when it is user defined class, `self` is the object.
        - we can

*/
typedef voba_value_t (*voba_func_t)(voba_value_t self, voba_value_t args);
// common utilis
INLINE int64_t  voba_get_type1(voba_value_t v);
INLINE int64_t  voba_get_type2(voba_value_t v);
INLINE int      voba_eq(voba_value_t a,voba_value_t b);
INLINE int      voba_is_nil(voba_value_t p);
// it is defined as a macro, because the return type is unknown.
#define voba_to_pointer(type,p) ((type)((p) & (~VOBA_TYPE_MASK)))
INLINE voba_value_t voba_from_pointer(void* p, int type);
/* string  */
//
INLINE voba_value_t  voba_make_string(voba_str_t* s);
INLINE voba_value_t  voba_make_cstr(const char * s);
INLINE int      voba_is_string(voba_value_t s);
INLINE voba_str_t* voba_value_to_str(voba_value_t s);
// func
INLINE voba_value_t  voba_make_func(voba_func_t);
INLINE voba_func_t  voba_value_to_func(voba_value_t v);
/* array

there are two kinds of array. fixed size and variable size. For the
fixed sized array, voba_value_t point to a below structure.

+----------------+------------+
|         size(64bits)        |
+----------------+------------+
|         voba_value_t a1          |
+-----------------------------+
|         voba_value_t a2          |
+-----------------------------+
|            ....             |
+-----------------------------+

When `size` is -1, it is variable size of array. voba_value_t point to the
following structure.

+----------------+------------+
|     0xFFFFFFFFFFFFFFFF      |
+----------------+------------+
|   .     voba_value_t *p          |
+---|-------------------------+
    |
    |
    |
    |     +----------------+------------+
    \---> |capacity(32bits)|size(32bits)|
          +----------------+------------+
          |         voba_value_t a1          |
          +-----------------------------+
          |         voba_value_t a2          |
          +-----------------------------+
          |            ....             |
          +-----------------------------+

*/
INLINE voba_value_t  voba_make_array(voba_value_t* p);
INLINE voba_value_t  voba_make_array_n(int64_t n,...);
INLINE voba_value_t  voba_make_array_nv(int64_t n,va_list s);
INLINE voba_value_t* voba_value_to_c_array(voba_value_t v);
INLINE int64_t  voba_array_capacity(voba_value_t v);
INLINE int64_t  voba_array_len(voba_value_t v);
INLINE voba_value_t* voba_array_base(voba_value_t v);
INLINE voba_value_t  voba_array_at(voba_value_t v,int64_t i);
INLINE voba_value_t  voba_array_set(voba_value_t a,int64_t i,voba_value_t v);
// create an array from another array
INLINE voba_value_t  voba_array_copy(voba_value_t v);
INLINE voba_value_t  voba_array_push(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_pop(voba_value_t a);
INLINE voba_value_t  voba_array_shift(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_unshift(voba_value_t a);
INLINE int      voba_is_array(voba_value_t v);
INLINE int      voba_is_fixed_size_array(voba_value_t v);
/* closure */
// closure is as below
/*
+----------------+------------+
|         funcp(64bits)       |
+----------------+------------+
|         size(64bits)        |
+-----------------------------+
|         voba_value_t a1          |
+-----------------------------+
|            ....             |
+-----------------------------+
*/
INLINE voba_value_t  voba_make_closure(voba_value_t * p);
// we can ONLY create a closure with raw voba_func_t.  because of the
// definition of `self`. `self` must be the closure itself.  if f is
// other type, e.g. an array, what would be `self`? the array or the
// closure?
INLINE voba_value_t  voba_make_closure_f_a(voba_func_t f, voba_value_t array);
//INLINE voba_value_t  make_closure_f_n(voba_func_t f, size_t n);
// closure array return a fixed size array.
INLINE voba_value_t  voba_closure_array(voba_value_t c);
INLINE voba_func_t  voba_closure_func(voba_value_t c);
/* pair */
// closure is as below
/*
+----------------+------------+
|        head(64bits)         |
+----------------+------------+
|        tail(64bits)         |
+-----------------------------+
for historical reason,`cons`,`car` and `cdr` should be used.
for readability, `voba_make_pair`, `head`, and `tail` should be used.
for catering non-lisp programmer, the later is chosen.
*/
INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t);
INLINE voba_value_t  voba_is_pair(voba_value_t p);
INLINE voba_value_t  voba_head(voba_value_t p);
INLINE voba_value_t  voba_tail(voba_value_t p);
INLINE void voba_set_head(voba_value_t p,voba_value_t v);
INLINE void voba_set_tail(voba_value_t p,voba_value_t v);
/* user data */
/*
+----------------+------------+
|  class_function(64bits)     |
+----------------+------------+
|       data ...  unknow size |
+-----------------------------+
|     but 8 bytes alignment   |
+-----------------------------+

the user data is garbage collected when unused, this is not a problem
for c, but might be a problem for C++, because deconstructur will
automatic release the memory. a solution is needed. TODO.

when class_function is NIL, the default class function
`vclass_user_data` is used. this function regards user data as a
opaque data area.
*/
// `size` of user_data, not including the room for cls function. E.g.
//  struct point { int x; int y};
//  make_user_data(NIL, sizeof(struct point))
//  ((struct point*) user_data_base(v))->x = 100;
//  ((struct point*) user_data_base(v))->y = 100;
INLINE voba_value_t voba_make_class(voba_class_t * vclass); // class is a unique symbol, typically un-interned.
INLINE voba_value_t voba_make_user_data(voba_value_t vclass, size_t size);
INLINE voba_value_t voba_user_data_class(voba_value_t v);
INLINE void*   voba_user_data_base(voba_value_t v);
#define VOBA_USER_DATA_AS(type,v) ((type)(voba_user_data_base(v)))
/* symbol */
INLINE voba_value_t voba_make_symbol_table();
INLINE voba_value_t voba_make_symbol_internal(voba_value_t symbol_name, voba_value_t symbol_value);
INLINE voba_value_t voba_make_symbol(voba_str_t * symbol_name, voba_value_t symbol_table);
INLINE voba_value_t voba_make_symbol_cstr(const char * symbol_name, voba_value_t symbol_table);
INLINE voba_value_t voba_is_symbol(voba_value_t v);
INLINE voba_value_t voba_symbol_name(voba_value_t v);
INLINE voba_value_t voba_symbol_value(voba_value_t v);
INLINE voba_value_t*voba_symbol_value_ptr(voba_value_t v);
INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v);

/* generic function */
// generic function is a user class, whose value is a hash table
INLINE voba_value_t voba_make_generic_function();
INLINE voba_value_t voba_gf_hash_table(voba_value_t gf);
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func);
INLINE voba_value_t voba_gf_lookup(voba_value_t gf, voba_value_t cls);
extern voba_value_t voba_gf_apply;
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a1);
INLINE voba_value_t voba_get_class_internal(voba_value_t v);
// small type
#define VOBA_SMALL_TYPES(XX)                                   \
    XX(VOBA_TYPE_I8,i8,int8_t)                                         \
    XX(VOBA_TYPE_U8,u8,uint8_t)                                        \
    XX(VOBA_TYPE_I16,i16,int16_t)                                      \
    XX(VOBA_TYPE_U16,u16,uint16_t)                                     \
    XX(VOBA_TYPE_I32,i32,int32_t)                                      \
    XX(VOBA_TYPE_U32,u32,uint32_t)                                     \
    XX(VOBA_TYPE_FLOAT,float,float)                                      
#define DECLARE_SMALL_TYPE(tag,name,type)       \
    INLINE voba_value_t voba_make_##name (type v);        \
    INLINE type voba_value_to_##name (voba_value_t v);     

VOBA_SMALL_TYPES(DECLARE_SMALL_TYPE)

// constants
// NIL don't have any class associated with it.
#define VOBA_NIL 0
#define VOBA_TRUE                      (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  1 * 256)
#define VOBA_FALSE                     (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  0 * 256)
#define VOBA_SECRET_VIRTUAL_TABLE (VOBA_TYPE_SMALL + VOBA_SECRET * 8 +  0 * 256)
INLINE int voba_is_true(voba_value_t v);
INLINE int voba_is_false(voba_value_t v);


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
