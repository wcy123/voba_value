#pragma once
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
*/
// macro definitions for type1
#define    VOBA_TYPE_FUNC    0 // pointer to a function pointer can be casted voba_value_t directly
#define    VOBA_TYPE_STRING  1
#define    VOBA_TYPE_SYMBOL  2
#define    VOBA_TYPE_TUPLE   3
#define    VOBA_TYPE_CLOSURE 4
#define    VOBA_TYPE_PAIR    5
#define    VOBA_TYPE_USER    6
#define    VOBA_TYPE_SMALL   7
#define    VOBA_TYPE_NIL     8 // virtual type, get_type1 return 8 when value is NIL.
#define    VOBA_TYPE_MASK    7 // 3 bits
typedef int64_t voba_value_t;
INLINE int64_t  voba_get_type1(voba_value_t v);
INLINE int      voba_eq(voba_value_t a,voba_value_t b);
INLINE int      voba_eql(voba_value_t a,voba_value_t b);
// it is defined as a macro, because the return type is unknown.
#define voba_to_pointer(type,p) ((type)((p) & (~VOBA_TYPE_MASK)))
INLINE voba_value_t voba_from_pointer(void* p, int type);


