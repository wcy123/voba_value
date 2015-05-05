#pragma once
/**@file
cls
===

The first user defined data type is a cls itself.

here is a table of all built-in class

| var name               |  note                                |
|------------------------|--------------------------------------|
| voba_cls_func          |  function object, see fun.h          |
| voba_cls_symbol        |  symbol, see symbol.h                |
| voba_cls_tuple         |  tuple, see tuple.h                  |
| voba_cls_closure       | closure, see closure.h               |
| voba_cls_pair          | pair, see pair.h                     |
| voba_cls_str           | str, see str.h                       |
| voba_cls_nil           | nil, see small.h                     |
| voba_cls_bool          | bool, see small.h                    |
| voba_cls_u8            | unsigned 8-bit integer, see small.h  |
| voba_cls_i8            | signed 8-bit integer, see small.h    |
| voba_cls_u16           | unsigned 16-bit integer, see small.h |
| voba_cls_i16           | signed 16-bit integer, see small.h   |
| voba_cls_u32           | unsigned 32-bit integer, see small.h |
| voba_cls_i32           | sigend 32-bit integer, see small.h   |
| voba_cls_float         | float, see small.h                   |
| voba_cls_short_symbol  | short symbol, see small.h            |
| voba_cls_undef         | VOBA_UNDEF, see small.h              |
| voba_cls_done          | VOBA_DONE, see small.h               |
| voba_cls_hashtable     | hash table, see hash.h               |
| voba_cls_symbol_table  | symbol table, see symbol_table.h     |
| voba_cls_voba_array    | array, see array.h                   |
| voba_cls_voba_gf       | generic function, see gf.h           |
| voba_cls_voba_la_t     | list view, see la.h                  |
| voba_cls_cg_t          | generator, see generator.h           |

*/
extern voba_value_t voba_cls_cls; /*!< the class object associated with \a cls itself */
typedef struct voba_cls_s voba_cls_t;
/** */
struct voba_cls_s {
    size_t size; /*!< the size of user defined object*/
    const char * name; /*!< the name of class */
};
extern voba_cls_t * the_voba_class_table;
extern int32_t      the_voba_class_table_length;
/** @return a \a cls obejct which represents the class of the object \a v*/
VOBA_VALUE_INLINE voba_value_t voba_get_class(voba_value_t v);
/** @return the name of the class object */
VOBA_VALUE_INLINE const char * voba_get_class_name(voba_value_t v);
#define VOBA_DEF_CLS(xsize,xname)                                       \
    voba_value_t voba_cls_##xname = VOBA_UNDEF;                         \
    EXEC_ONCE_PROGN                                                     \
    {                                                                   \
        voba_cls_##xname = voba_make_cls(xsize,#xname);                 \
    }
VOBA_VALUE_INLINE int32_t voba_class_id(voba_value_t cls);
VOBA_VALUE_INLINE const char* voba_cls_name(voba_value_t cls);
VOBA_VALUE_INLINE size_t voba_cls_size(voba_value_t cls);
