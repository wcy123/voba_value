#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <gc.h>
// pointer to a function pointer can be casted vtype_t directly
// macro definitions for type1
#define    V_FUNC    0
#define    V_SYMBOL  1
#define    V_ARRAY   2
#define    V_CLOSURE 3
#define    V_PAIR    4
#define    V_USER    5
#define    V_STRING  6
#define    V_SMALL   7
#define    V_NIL     8 // virtual type, only one value
// 3bits for type1
#define    V_TYPE_MASK    7 // 3 bits

#define    VOBA_FUNC __attribute__((aligned (16)))
// vtype_t is 64bits.
/*
+-----------------------------------------------------------+-------+
| data                                                      | type  |
+-----------------------------------------------------------+-------+
| 61bits                                                    | 3bits |
+-----------------------------------------------------------+-------+
3bits type could be:
V_FUNC, V_STRING, V_CLOSURE, V_ARRAY, V_HASH, V_SMALL
`get_type1` return 3-bits type.

when it is V_SMALL,

+-----------------------------------------------------------+-------+
| data                                           |    type2 | type  |
+-----------------------------------------------------------+-------+
| 48bits                                         |    5bits | 3bits |
+-----------------------------------------------------------+-------+
`get_type2` return 5-bits type.
*/
// macro defininitions for type2, i.e. when type1 == V_SMALL
#define    V_BOOLEAN      1
#define    V_I8           2
#define    V_U8           3
#define    V_I16          4
#define    V_U16          5
#define    V_I32          6
#define    V_U32          7
#define    V_FLOAT        8
#define    V_SHORT_SYMBOL 9
#define    V_SECRET       10   // internal used, which is not visiable

typedef int64_t vtype_t;
typedef struct vclass_s {
    size_t size;
    const char * name;
} vclass_t;
extern vtype_t voba_cls_func;
extern vtype_t voba_cls_generic_function;
extern vtype_t voba_cls_symbol;
extern vtype_t voba_cls_array;
extern vtype_t voba_cls_closure;
extern vtype_t voba_cls_pair;
extern vtype_t voba_cls_user;
extern vtype_t voba_cls_str;
extern vtype_t voba_cls_nil;
extern vtype_t voba_cls_bool;
extern vtype_t voba_cls_u8;
extern vtype_t voba_cls_i8;
extern vtype_t voba_cls_u16;
extern vtype_t voba_cls_i16;
extern vtype_t voba_cls_u32;
extern vtype_t voba_cls_i32;
extern vtype_t voba_cls_float;
extern vtype_t voba_cls_short_symbol;

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
typedef vtype_t (*vfunc_t)(vtype_t self, vtype_t args);
// common utilis
INLINE int64_t  get_type1(vtype_t v);
INLINE int64_t  get_type2(vtype_t v);
INLINE int      eq(vtype_t a,vtype_t b);
INLINE int      is_nil(vtype_t p);
// it idefined as a macro, because the return type is unknown.
#define to_pointer(type,p) ((type)((p) & (~V_TYPE_MASK)))
INLINE vtype_t from_pointer(void* p, int type);
/* string  */
//
INLINE vtype_t  make_string(voba_str_t* s);
INLINE vtype_t  make_cstr(const char * s);
INLINE int      is_string(vtype_t s);
INLINE voba_str_t* vtype_to_str(vtype_t s);
// func
INLINE vtype_t  make_func(vfunc_t);
INLINE vfunc_t  vtype_to_func(vtype_t v);
/* array

there are two kinds of array. fixed size and variable size. For the
fixed sized array, vtype_t point to a below structure.

+----------------+------------+
|         size(64bits)        |
+----------------+------------+
|         vtype_t a1          |
+-----------------------------+
|         vtype_t a2          |
+-----------------------------+
|            ....             |
+-----------------------------+

When `size` is -1, it is variable size of array. vtype_t point to the
following structure.

+----------------+------------+
|     0xFFFFFFFFFFFFFFFF      |
+----------------+------------+
|   .     vtype_t *p          |
+---|-------------------------+
    |
    |
    |
    |     +----------------+------------+
    \---> |capacity(32bits)|size(32bits)|
          +----------------+------------+
          |         vtype_t a1          |
          +-----------------------------+
          |         vtype_t a2          |
          +-----------------------------+
          |            ....             |
          +-----------------------------+

*/
INLINE vtype_t  make_array(vtype_t* p);
INLINE vtype_t  make_array_n(int64_t n,...);
INLINE vtype_t  make_array_nv(int64_t n,va_list s);
INLINE vtype_t* vtype_to_c_array(vtype_t v);
INLINE int64_t  array_capacity(vtype_t v);
INLINE int64_t  array_len(vtype_t v);
INLINE vtype_t* array_base(vtype_t v);
INLINE vtype_t  array_at(vtype_t v,int64_t i);
INLINE vtype_t  array_set(vtype_t a,int64_t i,vtype_t v);
// create an array from another array
INLINE vtype_t  array_copy(vtype_t v);
INLINE vtype_t  array_push(vtype_t a, vtype_t v);
INLINE vtype_t  array_pop(vtype_t a);
INLINE vtype_t  array_shift(vtype_t a, vtype_t v);
INLINE vtype_t  array_unshift(vtype_t a);
INLINE int      is_array(vtype_t v);
INLINE int      is_fixed_size_array(vtype_t v);
/* closure */
// closure is as below
/*
+----------------+------------+
|         funcp(64bits)       |
+----------------+------------+
|         size(64bits)        |
+-----------------------------+
|         vtype_t a1          |
+-----------------------------+
|            ....             |
+-----------------------------+
*/
INLINE vtype_t  make_closure(vtype_t * p);
// we can ONLY create a closure with raw vfunc_t.  because of the
// definition of `self`. `self` must be the closure itself.  if f is
// other type, e.g. an array, what would be `self`? the array or the
// closure?
INLINE vtype_t  make_closure_f_a(vfunc_t f, vtype_t array);
//INLINE vtype_t  make_closure_f_n(vfunc_t f, size_t n);
// closure array return a fixed size array.
INLINE vtype_t  closure_array(vtype_t c);
INLINE vfunc_t  closure_func(vtype_t c);
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
INLINE vtype_t  voba_make_pair(vtype_t h,vtype_t t);
INLINE vtype_t  is_pair(vtype_t p);
INLINE vtype_t  head(vtype_t p);
INLINE vtype_t  tail(vtype_t p);
INLINE void set_head(vtype_t p,vtype_t v);
INLINE void set_tail(vtype_t p,vtype_t v);
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
INLINE vtype_t make_class(vclass_t * vclass); // class is a unique symbol, typically un-interned.
INLINE vtype_t make_user_data(vtype_t vclass, size_t size);
INLINE vtype_t user_data_class(vtype_t v);
INLINE void*   user_data_base(vtype_t v);
#define USER_DATA_AS(type,v) ((type)(user_data_base(v)))
/* symbol */
INLINE vtype_t make_symbol_table();
INLINE vtype_t make_symbol_internal(vtype_t symbol_name, vtype_t symbol_value);
INLINE vtype_t make_symbol(voba_str_t * symbol_name, vtype_t symbol_table);
INLINE vtype_t make_symbol_cstr(const char * symbol_name, vtype_t symbol_table);
INLINE vtype_t is_symbol(vtype_t v);
INLINE vtype_t symbol_name(vtype_t v);
INLINE vtype_t symbol_value(vtype_t v);
INLINE vtype_t*symbol_value_ptr(vtype_t v);
INLINE vtype_t symbol_set_value(vtype_t s,vtype_t v);

/* generic function */
// generic function is a user class, whose value is a hash table
INLINE vtype_t make_generic_function();
INLINE vtype_t gf_hash_table(vtype_t gf);
INLINE vtype_t gf_add_class(vtype_t gf, vtype_t cls, vtype_t func);
INLINE vtype_t gf_lookup(vtype_t gf, vtype_t cls);
extern vtype_t gf_apply;
INLINE vtype_t apply(vtype_t f, vtype_t a1);
INLINE vtype_t get_class_internal(vtype_t v);
// small type
#define VOBA_SMALL_TYPES(XX)                                   \
    XX(V_I8,i8,int8_t)                                         \
    XX(V_U8,u8,uint8_t)                                        \
    XX(V_I16,i16,int16_t)                                      \
    XX(V_U16,u16,uint16_t)                                     \
    XX(V_I32,i32,int32_t)                                      \
    XX(V_U32,u32,uint32_t)                                     \
    XX(V_FLOAT,float,float)                                      
#define DECLARE_SMALL_TYPE(tag,name,type)       \
    INLINE vtype_t make_##name (type v);        \
    INLINE type vtype_to_##name (vtype_t v);     

VOBA_SMALL_TYPES(DECLARE_SMALL_TYPE)

// constants
// NIL don't have any class associated with it.
#define NIL 0
#define TRUE                      (V_SMALL + V_BOOLEAN * 8 +  1 * 256)
#define FALSE                     (V_SMALL + V_BOOLEAN * 8 +  0 * 256)
#define VOBA_SECRET_VIRTUAL_TABLE (V_SMALL + V_SECRET * 8 +  0 * 256)
INLINE int is_true(vtype_t v);
INLINE int is_false(vtype_t v);


/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
