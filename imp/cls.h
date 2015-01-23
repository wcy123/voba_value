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
extern uint32_t voba_cls_next_id; /*!< every class has a unique id, which is used for generic function.*/
typedef struct voba_cls_s voba_cls_t;
/** */
struct voba_cls_s {
    size_t size; /*!< the size of user defined object*/
    const char * name; /*!< the name of class */
    uint32_t id; /*!< the id of classs*/
};
/** @return a \a cls obejct which represents the class of the object \a v*/
INLINE voba_value_t voba_get_class(voba_value_t v);
/** @return the name of the class object */
INLINE const char * voba_get_class_name(voba_value_t v);

#define VOBA_CLS(s)  VOBA_USER_DATA_AS(voba_cls_t *,s)
#define VOBA_DEF_CLS(xsize,xname)                                       \
    voba_value_t voba_cls_##xname = VOBA_UNDEF;                         \
    EXEC_ONCE_PROGN                                                     \
    {                                                                   \
        if(voba_eq(voba_cls_cls,VOBA_UNDEF)){                           \
            /*                                                          \
               voba_cls_cls itself is a user data too. but we need a    \
               voba_cls to create a user data. so there is a            \
               chicken-egg problem.  voba_allocate_user_data is used    \
               to break the recursive, i.e. `voba_cls_cls` is created   \
               in a special way.                                        \
            */                                                          \
            voba_value_t * p =                                          \
                voba_allocate_user_data(sizeof(voba_cls_t));            \
            voba_cls_cls = voba_from_pointer(p,VOBA_TYPE_USER);         \
            VOBA_CLS(voba_cls_cls)->size = sizeof(voba_cls_t);          \
            VOBA_CLS(voba_cls_cls)->name = "cls";                       \
            p[0] = voba_cls_cls;                                        \
        }                                                               \
        voba_cls_##xname = voba_make_cls(xsize,#xname);                 \
    }
