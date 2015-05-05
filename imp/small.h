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
#define    VOBA_TYPE_BOOL		1
#define    VOBA_TYPE_I32		2
#define    VOBA_TYPE_FLOAT		3
#define    VOBA_TYPE_SHORT_SYMBOL	4
#define    VOBA_TYPE_GENERIC_FUNCTION	5
#define    VOBA_TYPE_CLASS		6
#define    VOBA_TYPE_SPECIAL_VALUES	100	// internal use, true, false, done, undef
#define    VOBA_TYPE_SECRET		101	// internal use, I like secret, it is not used at all.
VOBA_VALUE_INLINE int64_t  voba_get_type2(voba_value_t v);
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
VOBA_VALUE_INLINE int voba_is_nil(voba_value_t p);
/** @brief constant value undef

It represents an unintialized value.

*/
#define VOBA_UNDEF (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  1 * 256)
/** @brief the class object for ::VOBA_UNDEF */
extern voba_value_t voba_cls_undef;
/** @brief return 0 if `p` is not ::VOBA_UNDEF*/
VOBA_VALUE_INLINE int voba_is_undef(voba_value_t v) { return v == VOBA_UNDEF; }

/** @brief constant VOBA_DONE

It repensent the end of iteration.

*/
#define VOBA_DONE (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  2 * 256)
/** @brief the class object for ::VOBA_DONE */
extern voba_value_t voba_cls_done;
/** @brief return 0 if `p` is not ::VOBA_DONE*/
VOBA_VALUE_INLINE int voba_is_done(voba_value_t v) { return v == VOBA_DONE; }


/** @brief constant VOBA_TAIL_CALL

It repensent the end of iteration.

*/
#define VOBA_TAIL_CALL (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  3 * 256)
/** @brief return 0 if `p` is not ::VOBA_TAIL_CALL*/
VOBA_VALUE_INLINE int voba_is_tail_call(voba_value_t v) { return v == VOBA_TAIL_CALL; }
/** @brief contant VOBA_BOX_END
    it is the end mark for a sequence of voba value, e.g. tuple or array.
 */
#define VOBA_BOX_END (VOBA_TYPE_SMALL + VOBA_TYPE_SPECIAL_VALUES * 8 +  4 * 256)

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
VOBA_VALUE_INLINE int voba_is_true(voba_value_t v);
/// @brief return 1 if false
VOBA_VALUE_INLINE int voba_is_false(voba_value_t v);
/// @brief return ::VOBA_TRUE if ::VOBA_FALSE, otherwise return ::VOBA_FALSE.
VOBA_VALUE_INLINE voba_value_t voba_not(voba_value_t v);
/** @brief class short symbol
NOT IMPLEMENTED YET.
 */
extern voba_value_t voba_cls_short_symbol;
/** @brief class int32_t

@verbatim
+-----------------------------------------------------------+-------+
| data                                              | type2 | type1 |
+-----------------------------------------------------------+-------+
|                 value                             | 00010 |  111  |
+-----------------------------------------------------------+-------+
@endverbatim

 */
/** @brief class object for i32 */
extern voba_value_t voba_cls_i32;
VOBA_VALUE_INLINE voba_value_t voba_make_i32(int32_t a);
VOBA_VALUE_INLINE int32_t voba_value_to_i32(voba_value_t a);
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
VOBA_VALUE_INLINE voba_value_t voba_make_float (float v);
VOBA_VALUE_INLINE float voba_value_to_float (voba_value_t v);
