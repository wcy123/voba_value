#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <gc.h>
#include "voba_for_each.h"
// 3bits for type1

/* OVERVIEW */
// voba_value_t is 64bits.
/*
+--------------+-------+
| data         | type1 |
+--------------+-------+
| 61bits       | 3bits |
+--------------+-------+
3bits type could be:

    VOBA_TYPE_FUNC, ..., and  VOBA_TYPE_SMALL

`voba_get_type1` return 3-bits type.

when it is VOBA_TYPE_SMALL,

+--------+-------+-------+
| data   | type2 | type1 |
+--------------+---------+
| 48bits | 5bits | 3bits |
+--------+-------+-------|
`voba_get_type2` return 5-bits type.
*/
// macro definitions for type1
#define    VOBA_TYPE_FUNC    0 // pointer to a function pointer can be casted voba_value_t directly
#define    VOBA_TYPE_SYMBOL  1
#define    VOBA_TYPE_ARRAY   2
#define    VOBA_TYPE_CLOSURE 3
#define    VOBA_TYPE_PAIR    4
#define    VOBA_TYPE_USER    5
#define    VOBA_TYPE_STRING  6
#define    VOBA_TYPE_SMALL   7
#define    VOBA_TYPE_NIL     8 // virtual type, get_type1 return 8 when value is NIL.
#define    VOBA_TYPE_MASK    7 // 3 bits
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
INLINE int64_t  voba_get_type1(voba_value_t v);
INLINE int64_t  voba_get_type2(voba_value_t v);
INLINE int      voba_eq(voba_value_t a,voba_value_t b);
// it is defined as a macro, because the return type is unknown.
#define voba_to_pointer(type,p) ((type)((p) & (~VOBA_TYPE_MASK)))
INLINE voba_value_t voba_from_pointer(void* p, int type);

/*
        class nil
        =========

+---------------------------+------------------------+
|                    0                               |
+---------------------------+------------------------+

This class only contain one value, VOBA_NIL. The least significant
3-bits is 000, which is as same as VOBA_TYPE_FUNC, but no function
address is zero, so it is safe to share the same type tag with
VOBA_TYPE_FUNC.

 */
extern voba_value_t voba_cls_nil;
#define VOBA_NIL 0
INLINE int voba_is_nil(voba_value_t p);

/*
     class bool
     =========

+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                  1 (TRUE) or 0 (FALS)             | 00001 |  111  |
+-----------------------------------------------------------+-------+


It has two values, VOBA_TRUE and VOBA_FALSE

 */
extern voba_value_t voba_cls_bool;
#define VOBA_TRUE                      (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  1 * 256)
#define VOBA_FALSE                     (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  0 * 256)
INLINE int voba_is_true(voba_value_t v);
INLINE int voba_is_false(voba_value_t v);

/*
    class integer
    ============
+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                 value                             | 00010 |  111  |
+-----------------------------------------------------------+-------+

voba_make_i8, voba_make_u8, voba_make_i16,voba_make_i32

voba_value_to_i8, voba_value_to_u8, voba_value_to_i16,
voba_value_to_u16, voba_value_to_i32, voba_value_to_u32,

These functions are trivival so that they are implemented by macro
VOBA_SMALL_TYPES(DECLARE_SMALL_TYPE)

 */
extern voba_value_t voba_cls_u8;
extern voba_value_t voba_cls_i8;
extern voba_value_t voba_cls_u16;
extern voba_value_t voba_cls_i16;
extern voba_value_t voba_cls_u32;
extern voba_value_t voba_cls_i32;
/*
    class float
    ============

+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                 value (32bits)                    | 01000 |  111  |
+-----------------------------------------------------------+-------+

voba_make_float, voba_value_to_float, similar to `class integer`.

 */
extern voba_value_t voba_cls_float;

/*
    class short symbol
    ==================
NOT IMPLEMENTED YET.

 */
extern voba_value_t voba_cls_short_symbol;

/* class function
   ==============

+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |
  `----> address of a function.

  for an s-exp, e.g. (f a1 a2 a3 ...)
  `self` is `f` and `args` is an array , [ a1, a2, a4 , ....]

  `f` could be any callable object, like, a closure, a generic
  function, a function, an array, a hash table, or any other user
  defined objects whose class method implements the `voba_gf_apply`

*/
extern voba_value_t voba_cls_func;
typedef voba_value_t (*voba_func_t)(voba_value_t self, voba_value_t args);
INLINE voba_value_t  voba_make_func(voba_func_t);
INLINE voba_func_t  voba_value_to_func(voba_value_t v);

/* class generic_function
   ======================

generic function is a user class, whose value is a hash table.

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  `-->  +------------------------------+------------------------+
        |           voba_cls_generic_function                   |
        +------------------------------+------------------------+
        |           a hash table                                |
        +------------------------------+------------------------+

*/
extern voba_value_t voba_cls_generic_function;
INLINE voba_value_t voba_make_generic_function();
// return the hash table
INLINE voba_value_t voba_gf_hash_table(voba_value_t gf);
// register a method for a class.
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func);
// look up the hash table and find the implemention for a class.
INLINE voba_value_t voba_gf_lookup(voba_value_t gf, voba_value_t cls);

/*   symbol
     ======

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  `-->  +----------------------------+------------------------+
        |           symbol name ( a string )                  |
        +----------------------------+------------------------+
        |           symbol value (any voba value)             |
        +----------------------------+------------------------+

`symbol` is similar to `pair`, but the address of the `pair` is
important. `voba_make_symbol` always returns the same address for the
same symbol name in a symbol table.

`symbol_table` is a user defined class as below

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  `-->  +----------------------------+------------------------+
        |  VOBA_NIL (TODO: it should have a class)            |
        +----------------------------+------------------------+
        |  voba_symbol_table_c(see voba_value_cpp.cc)         |
        +----------------------------+------------------------+
        |             ... ...                                 |
        +----------------------------+------------------------+

When`symbol_table is nil, `voba_make_symbol` return a uniqu symbol, an
un-interned symbol.

To intern a symbol, `voba_make_symbol(voba_symbol_name(sym),
the_symbol_table)`, but it returns a new symbol. The old symbol `sym`
is always un-interned.

TODO: voba_make_symbol_with_value, voba_make_symbol_cstr_with_value
*/
extern voba_value_t voba_cls_symbol;
voba_value_t voba_make_symbol(voba_str_t * symbol_name, voba_value_t symbol_table); // implemented in voba_value.cc
INLINE voba_value_t voba_make_symbol_cstr(const char * symbol_name, voba_value_t symbol_table);
INLINE voba_value_t voba_make_symbol_data(const char * data, uint32_t len, voba_value_t symbol_table);
#define VOBA_SYMBOL(s,table) voba_make_symbol_data(#s,sizeof(#s) -1 , table)
INLINE voba_value_t voba_is_symbol(voba_value_t v);
INLINE voba_value_t voba_symbol_name(voba_value_t v);
INLINE voba_value_t voba_symbol_value(voba_value_t v);
INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v);



/* array
   =====

there are two kinds of array. fixed size and variable size.


For fixed sized arrays, `voba_value_t` point to a below structure.


+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  |       +----------------+------------+
  `---->  |         size(64bits)        | when size = -1, it is a variable size array
          +----------------+------------+
          |         voba_value_t a1     |
          +-----------------------------+
          |         voba_value_t a2     |
          +-----------------------------+
          |            ....             |
          +-----------------------------+


+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  |       +----------------+------------+
  `---->  |     0xFFFFFFFFFFFFFFFF      | tag for variable size array
          +----------------+------------+
          |   .  voba_value_t *p        |
          +---|-------------------------+
              |
              |
              |
              |     +----------------+------------+
              \---> |capacity(32bits)|size(32bits)|
                    +----------------+------------+
                    |         voba_value_t a1     |
                    +-----------------------------+
                    |         voba_value_t a2     |
                    +-----------------------------+
                    |            ....             |
                    +-----------------------------+


Why two level for variable size?

   when resizing, the array object's address might be changed, so all
   other reference could point to an invalid address.



*/
extern voba_value_t voba_cls_array;
INLINE voba_value_t  voba_make_array(voba_value_t* p);
INLINE voba_value_t  voba_make_array_n(int64_t n,...);
INLINE voba_value_t  voba_make_array_nv(int64_t n,va_list s);
INLINE voba_value_t* voba_value_to_c_array(voba_value_t v);
INLINE int64_t  voba_array_capacity(voba_value_t v);
INLINE int64_t  voba_array_len(voba_value_t v);
INLINE voba_value_t* voba_array_base(voba_value_t v);
INLINE voba_value_t  voba_array_at(voba_value_t v,int64_t i);
INLINE voba_value_t  voba_array_set(voba_value_t a,int64_t i,voba_value_t v);
INLINE voba_value_t  voba_array_copy(voba_value_t v);
INLINE voba_value_t  voba_array_push(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_pop(voba_value_t a);
INLINE voba_value_t  voba_array_shift(voba_value_t a, voba_value_t v);
INLINE voba_value_t  voba_array_unshift(voba_value_t a);
INLINE int      voba_is_array(voba_value_t v);
INLINE int      voba_is_fixed_size_array(voba_value_t v);

/* closure
   =======

+---------------------------+-------+
| data                      | type1 |
+---------------------------+-------+
| .    value (61 bits)      |  011  |
+-|-------------------------+-------+
  |
  |      +----------------+------------+
  `----> |         funcp(64bits)       |
         +----------------+------------+
         |         size(64bits)        |
         +-----------------------------+
         |         voba_value_t a1     |
         +-----------------------------+
         |            ....             |
         +-----------------------------+

*/
extern voba_value_t voba_cls_closure;
INLINE voba_value_t  voba_make_closure_f_a(voba_func_t f, voba_value_t array);
INLINE voba_value_t  voba_closure_array(voba_value_t c);
INLINE voba_func_t   voba_closure_func(voba_value_t c);

/* pair
   ====

+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |      +----------------+------------+
  `----> |        head(64bits)         |
         +----------------+------------+
         |        tail(64bits)         |
         +-----------------------------+
*/
extern voba_value_t voba_cls_pair;
#define voba_cons voba_make_pair
#define voba_car  voba_head
#define voba_cdr voba_tail
#define voba_setcar voba_set_head
#define voba_setcdr voba_setcdr
INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t);
INLINE voba_value_t  voba_is_pair(voba_value_t p);
INLINE voba_value_t  voba_head(voba_value_t p);
INLINE voba_value_t  voba_tail(voba_value_t p);
INLINE void voba_set_head(voba_value_t p,voba_value_t v);
INLINE void voba_set_tail(voba_value_t p,voba_value_t v);
/* string
   ======

+--------------------+------------------------+
| .     61 bits value                   | 100 |
+-|------------------+------------------------+
  |      voba_str_t, see voba_str for detail
  |      +----------------+------------+
  `----> |  const char * data          |
         +----------------+------------+
         | capcity(32bits)| len(32bit) |
         +-----------------------------+
 */
//
extern voba_value_t voba_cls_str;
INLINE voba_value_t  voba_make_string(voba_str_t* s);
INLINE voba_value_t  voba_make_cstr(const char * s);
INLINE int      voba_is_string(voba_value_t s);
INLINE voba_str_t* voba_value_to_str(voba_value_t s);
/* user data */
/*
+--------------------+------------------------+
| .     61 bits value                   | 100 |
+-|------------------+------------------------+
  |    +----------------+------------+          +----------------+------------+
  `->  |  voba_class_t  *        .---|--------> |    .      size_t size       |
       +----------------+------------+          +----------------+------------+
       |       data ...  unknow size | \        |  const char * name          |
       +-----------------------------+  |       +-----------------------------+
       |     but 8 bytes alignment   |  |  (size)
       +-----------------------------+ /


the user data is garbage collected when unused, this is not a problem
for c, but might be a problem for C++, because deconstructor is
automatic invoked. a solution is needed. TODO.

when voba_class_t is NIL, the default user class `vova_cls_user_data`
is used. this function regards user data as a opaque data area.

`size` of user_data, not including the room for cls function. E.g.
 
       struct point { int x; int y}; 
       voba_make_user_data(NIL, sizeof(struct point)) 
       ((struct point*) user_data_base(v))->x = 100; 
       ((struct point*) user_data_base(v))->y = 100;

*/
extern voba_value_t voba_cls_user;
typedef struct vclass_s {
    size_t size;
    const char * name;
} voba_class_t;
INLINE voba_value_t voba_make_class(voba_class_t * vclass); // class is a unique symbol, typically un-interned.
INLINE voba_value_t voba_make_user_data(voba_value_t vclass, size_t size);
INLINE voba_value_t voba_user_data_class(voba_value_t v);
INLINE void*   voba_user_data_base(voba_value_t v);
#define VOBA_USER_DATA_AS(type,v) ((type)(voba_user_data_base(v)))

/* hashtable
   =========

A hashtable is a user define class.

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  |     +------------------------------+--------------------+
  `->   |           voba_cls_hashtable                      |
        +------------------------------+--------------------+
        |           hash_table_c ...                        |
        +------------------------------+--------------------+

voba_hash_insert returns a pair if succesfull, NIL otherwise.

voba_hash_find returns a pair if successful, NIL otherwise.

head is the key and tail is the value. 


 */
extern voba_value_t voba_cls_hashtable;
voba_value_t voba_make_hash();
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v);
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k);
/*  symbol table
    ============

A symbol table is a user define class.

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  |     +------------------------------+--------------------+
  `->   |           voba_cls_hashtable                      |
        +------------------------------+--------------------+
        |           symbol_table_c ...                      |
        +------------------------------+--------------------+

 */
extern voba_value_t voba_cls_symbol_table;
voba_value_t voba_make_symbol_table();  // implemented in voba_value.cc
/* listarray
   =========

   In LISP, we often have the `push and reverse` idiom, `list array`
   is the optimization (pre-mature, sorry) for it. Using `array` to
   represent `list` is more space efficient.

   `listarray` essentially is a pair, it does not have it own type,
   for efficiency purpuse, so `is_pair(la)` returns true;

interface:

    - VOBA_NIL is the empty `listarray`
    - `la_cons(LA, a)`: append a to the end of LA
         this implements `push without reverse` because appending is fast for array.
    - `la_car(LA)`
    - `la_cdr(LA)`

internal implementation

    for non-empty `listarray`;

+--------------------+------------------------+
| .     61 bits value                   | 010 |
+-|------------------+------------------------+
  |      +----------------+----------------------+
  `----> |  uint32_t cur  | uint32_t capacity    |
         +----------------+----------------------+
         |   storage: cls_array                  |
         +---------------------------------------+

for efficiency purpose, `la_cons`, and `la_cdr` modify `la` in-place,
and returns `la`. to save `la`, use `la_copy`.

*/
INLINE voba_value_t voba_la_from_array(voba_value_t array, uint32_t start, uint32_t len);
INLINE voba_value_t voba_la_cons(voba_value_t la, voba_value_t a);
INLINE voba_value_t voba_la_car(voba_value_t la);
INLINE voba_value_t voba_la_cdr(voba_value_t la);
INLINE voba_value_t voba_la_copy(voba_value_t la);
/* apply
   =====
 It is the only core function of voba_value library.
 voba_apply(f, args) evaluates s-exp (f a1 a2 ....)
 where args is the argument array (a1 a2 ...)

 `f` is any type which is callable.

 1. voba_cls_func: call it directly
 2. voba_cls_generic_function: use `voba_gf_lookup(f, voba_get_class(a1))` to find the actual voba_cls_func.
 3. otherwise, use `voba_gf_lookup(voba_gf_apply, voba_get_class(f))` to find the actual 

*/

extern voba_value_t voba_gf_apply;
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a);
INLINE voba_value_t voba_get_class(voba_value_t v);
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
#define VOBA_SECRET_VIRTUAL_TABLE (VOBA_TYPE_SMALL + VOBA_SECRET * 8 +  0 * 256)

#define COMMA  ,
#define SEMI_COMMA ;
#define SPACE
#define arg(n) voba_value_t a##n
#define DECLARE_VOBA_MAKE_ARRAY_N(n)                                    \
    INLINE voba_value_t voba_make_array_##n (VOBA_FOR_EACH_N(n)(arg, COMMA))
VOBA_FOR_EACH(DECLARE_VOBA_MAKE_ARRAY_N,SEMI_COMMA);

/* Local Variables: */
/* mode:c */
/* coding: utf-8-unix */
/* End: */
