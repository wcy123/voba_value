#pragma once
/*! \file overview.h 
 *
 */


/**
 * voba_value_t is a 64-bit value as below

\verbatim 
+--------------+-----------+
| data         | type tag  |
+--------------+-----------+
| 61bits       | 3bits     |
+--------------+-----------+
\endverbatim

*/
typedef int64_t voba_value_t;
/// type tag of function pointer, see fun.h
#define    VOBA_TYPE_FUNC    0
/// type tag of box, see box.h
#define    VOBA_TYPE_BOX     1
/// type tag of symbol, see symbol.h
#define    VOBA_TYPE_SYMBOL  2
/// type tag of tuple, see tuple.h
#define    VOBA_TYPE_TUPLE   3
/// type tag of closure, see closure.h
#define    VOBA_TYPE_CLOSURE 4
/// type tag of pair, see pair.h
#define    VOBA_TYPE_PAIR    5
/// type tag of user defined object, see user.h
#define    VOBA_TYPE_USER    6
/// type tag of small types, e.g. int8, int16, etc, see small.h
#define    VOBA_TYPE_SMALL   7


/** special type tag for value NIL
*/
/*  TODO: why we have it?  

    because it has the same type tag as VOBA_TYPE_FUNC, but it is not
    a function.
*/
/// virtual type, voba_get_type1() return VOBA_TYPE_NIL when value is NIL.
#define    VOBA_TYPE_NIL     8 
#define    VOBA_TYPE_MASK    7 /*!< 3 bits */
/** @return the 3-bit type tag associated with the value @param v
 */
VOBA_VALUE_INLINE int64_t  voba_get_type1(voba_value_t v);
/** strong comparison.
 * 
 * test equal or not by comparing the 64-bit value directly
 */
VOBA_VALUE_INLINE int      voba_eq(voba_value_t a,voba_value_t b);
/** eql comparison
 *
 * In addition to `voba_eq`, if two integer with difference types are
 * eql. 
 * Two strings with same string sequence are eql.
 */
VOBA_VALUE_INLINE int      voba_eql(voba_value_t a,voba_value_t b);
/// it is defined as a macro, because the return type is unknown.
#define voba_to_pointer(type,p) ((type)((p) & (~VOBA_TYPE_MASK)))
/** convert a pointer to a voba_value_t by attaching a tag type \a type
 *
 * @param type the 3-bit type tag
 * @param p the pointer
 * @return a voba_value_t with least significant 3-bit as type tag.
 *
 */
VOBA_VALUE_INLINE voba_value_t voba_from_pointer(void* p, int type);

