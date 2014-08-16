#define INLINE
#include "voba_value.h"
#include "exec_once.h"

#define DEFINE_CLS(xsize,xname)                                           \
    vtype_t voba_cls_##xname             = NIL;                          \
    static void voba_cls_##xname##_init()                                \
    {                                                                   \
        static vclass_t cls = { xsize, #xname  };                         \
        voba_cls_##xname = make_symbol(voba_str_from_cstr(#xname),NIL);   \
        symbol_set_value(voba_cls_##xname, make_class(&cls));            \
        fprintf(stderr,__FILE__ ":%d:[%s] %p,%s(%ld)\n", __LINE__, __FUNCTION__, \
                &cls,cls.name, cls.size);                               \
                                                                        \
        return;                                                         \
    }                                                                   \
    EXEC_ONCE(voba_cls_##xname##_init)                                   \



DEFINE_CLS(0,func)
DEFINE_CLS(sizeof(vtype_t),generic_function)
DEFINE_CLS(0,symbol)
DEFINE_CLS(0,array)
DEFINE_CLS(0,closure)
DEFINE_CLS(0,pair)
DEFINE_CLS(sizeof(vclass_t),user)
DEFINE_CLS(0,str)
DEFINE_CLS(0,nil)
DEFINE_CLS(0,bool)
DEFINE_CLS(0,u8)
DEFINE_CLS(0,i8)
DEFINE_CLS(0,u16)
DEFINE_CLS(0,i16)
DEFINE_CLS(0,u32)
DEFINE_CLS(0,i32)
DEFINE_CLS(0,float)
DEFINE_CLS(0,short_symbol)

vtype_t gf_apply = NIL;
EXEC_ONCE_DO(gf_apply = make_generic_function(););

EXEC_ONCE_START



