#pragma once
#include "for_each.h"

/*

when it is VOBA_TYPE_SMALL,

+--------+-------+-------+
| data   | type2 | type1 |
+--------------+---------+
| 48bits | 5bits | 3bits |
+--------+-------+-------|
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
#define    VOBA_TYPE_SPECIAL_VALUES 11   // internal used, which is not visible
#define    VOBA_TYPE_SECRET       10   // internal used, which is not visible
INLINE int64_t  voba_get_type2(voba_value_t v);
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
INLINE voba_value_t voba_not(voba_value_t v);
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
    class short symbol
    ==================
NOT IMPLEMENTED YET.

 */
extern voba_value_t voba_cls_short_symbol;
extern voba_value_t voba_cls_undef;
extern voba_value_t voba_cls_done;

// constants
// NIL don't have any class associated with it.
// #define VOBA_SECRET_VIRTUAL_TABLE (VOBA_TYPE_SMALL + VOBA_SECRET * 8 +  0 * 256)
#define VOBA_UNDEF (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  1 * 256)
INLINE int voba_is_undef(voba_value_t v) { return v == VOBA_UNDEF; }
#define VOBA_DONE (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  2 * 256)
INLINE int voba_is_done(voba_value_t v) { return v == VOBA_DONE; }
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
// small type
#define VOBA_SMALL_TYPES(XX)                                           \
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


INLINE int64_t voba_int_value_to_i32(voba_value_t a);
INLINE int voba_is_int(voba_value_t a );
INLINE voba_value_t i64_to_voba_int_value(int64_t a);
