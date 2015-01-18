#pragma once
/**@file
cls
=== 

The first user defined data type is a cls itself. 

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


