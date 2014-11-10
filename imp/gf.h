#pragma once
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
        |           default implementation(func)                |
        +------------------------------+------------------------+

*/
#define VOBA_MAX_NUM_OF_CLS 1000
#define VOBA_MAX_NUM_OF_GF  1000
typedef struct voba_gf_s voba_gf_t;
struct voba_gf_s {
    voba_value_t hash;
    voba_func_t  fun;
    const char * name;
    uint32_t     id;
    voba_func_t  cls[VOBA_MAX_NUM_OF_CLS];
};
#define VOBA_GF(s)  VOBA_USER_DATA_AS(voba_gf_t *,s)

extern voba_value_t voba_cls_generic_function;
extern uint32_t voba_cls_generic_function_next_id;
INLINE voba_value_t voba_make_generic_function(const char * name, voba_func_t default_imp);
// register a method for a class.
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func);
// look up the hash table and find the implemention for a class.
INLINE voba_func_t voba_gf_lookup(voba_value_t gf, voba_value_t cls);


