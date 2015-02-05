#pragma once
#include "for_each.h"

/** @file
small types
===========

when it is VOBA_TYPE_SMALL,
@verbatim
+--------------------------------------+-------+-------+
| data                                 | type2 | type1 |
+--------------------------------------+-------+-------+
| 48bits                               | 5bits | 3bits |
+--------------------------------------+-------+-------|
@endverbatim
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
/** @brief nil
constant value nil
==================

@verbatim
+---------------------------+------------------------+
|                    0                               |
+---------------------------+------------------------+
@endverbatim

This class only contain one value, VOBA_NIL. The least significant
3-bits is 000, which is as same as VOBA_TYPE_FUNC, but no function
address is zero, so it is safe to share the same type tag with
VOBA_TYPE_FUNC.

::VOBA_NIL is usually represents
 - end of a list
 - empty value
 - it has the special value of zero.
*/
#define VOBA_NIL 0
/** @brief the class object for ::VOBA_NIL*/
extern voba_value_t voba_cls_nil;
/** @brief return 0 if `p` is not ::VOBA_NIL*/
INLINE int voba_is_nil(voba_value_t p);
/** @brief constant value undef

It represents an unintialized value.

*/
#define VOBA_UNDEF (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  1 * 256)
/** @brief the class object for ::VOBA_UNDEF */
extern voba_value_t voba_cls_undef;
/** @brief return 0 if `p` is not ::VOBA_UNDEF*/
INLINE int voba_is_undef(voba_value_t v) { return v == VOBA_UNDEF; }

/** @brief constant VOBA_DONE

It repensent the end of iteration.

*/
#define VOBA_DONE (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  2 * 256)
/** @brief the class object for ::VOBA_DONE */
extern voba_value_t voba_cls_done;
/** @brief return 0 if `p` is not ::VOBA_DONE*/
INLINE int voba_is_done(voba_value_t v) { return v == VOBA_DONE; }
/** @brief class bool
@verbatim
+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                  1 (TRUE) or 0 (FALS)             | 00001 |  111  |
+-----------------------------------------------------------+-------+
@endverbatim
It has two values, ::VOBA_TRUE and ::VOBA_FALSE
*/
extern voba_value_t voba_cls_bool;
/// @brief true value
#define VOBA_TRUE                      (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  1 * 256)
/// @brief false value
#define VOBA_FALSE                     (VOBA_TYPE_SMALL + VOBA_TYPE_BOOL * 8 +  0 * 256)
/// @brief return 1 if true
INLINE int voba_is_true(voba_value_t v);
/// @brief return 1 if false
INLINE int voba_is_false(voba_value_t v);
/// @brief return ::VOBA_TRUE if ::VOBA_FALSE, otherwise return ::VOBA_FALSE.
INLINE voba_value_t voba_not(voba_value_t v);
/** @brief class short symbol
NOT IMPLEMENTED YET.
 */
extern voba_value_t voba_cls_short_symbol;
/** @brief class integer

@verbatim
+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                 value                             | 00010 |  111  |
+-----------------------------------------------------------+-------+
@endverbatim

voba_make_i8, voba_make_u8, voba_make_i16,voba_make_i32

voba_value_to_i8, voba_value_to_u8, voba_value_to_i16,
voba_value_to_u16, voba_value_to_i32, voba_value_to_u32,

These functions are trivival so that they are implemented by macro
VOBA_SMALL_TYPES(DECLARE_SMALL_TYPE)

 */
extern voba_value_t voba_cls_u8;
/** @brief class object for i8 */
extern voba_value_t voba_cls_i8;
/** @brief class object for u16 */
extern voba_value_t voba_cls_u16;
/** @brief class object for i16 */
extern voba_value_t voba_cls_i16;
/** @brief class object for u32 */
extern voba_value_t voba_cls_u32;
/** @brief class object for i32 */
extern voba_value_t voba_cls_i32;
/** @brief class float

@verbatim
+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                 value (32bits)                    | 01000 |  111  |
+-----------------------------------------------------------+-------+
@endverbatim

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
