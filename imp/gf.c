#include "gf.h"
VOBA_VALUE_INLINE voba_value_t voba_make_generic_function(const char * name, voba_func_t default_imp)
{
    if(the_voba_gf_table == NULL){
	the_voba_gf_table = (struct voba_gf_s*)
	    GC_MALLOC(sizeof(struct voba_gf_s));
    }else{
	the_voba_gf_table = (struct voba_gf_s*)
	    GC_REALLOC(the_voba_gf_table,
		       sizeof(struct voba_gf_s)*(the_voba_gf_table_length+1));
    }
    if(!the_voba_gf_table) abort(); // out-of-memory
    the_voba_gf_table[the_voba_gf_table_length].header.name = name;
    for(int i = 0; i < VOBA_MAX_NUM_OF_CLS; ++i){
	the_voba_gf_table[the_voba_gf_table_length].cls[i] = default_imp;
    }
    voba_value_t ret = VOBA_TYPE_SMALL + (VOBA_TYPE_GENERIC_FUNCTION*8) + (the_voba_gf_table_length * 256);
    the_voba_gf_table_length ++;
    return ret;
}
VOBA_VALUE_INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func)
{
    the_voba_gf_table[voba_gf_id(gf)].cls[voba_class_id(cls)] = voba_value_to_func(func);
    return func;
}
VOBA_VALUE_INLINE voba_func_t voba_gf_lookup(voba_value_t gf, voba_value_t cls)
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    if(0){
        fprintf(stderr,__FILE__ ":%d:[%s] looking gf= %s cls=%s f= %p\n", __LINE__, __FUNCTION__
                ,voba_gf_name(gf)
                ,voba_get_class_name(cls)
                ,voba_gf_cls_func_pointer(gf,cls));

    }
    return voba_gf_cls_func_pointer(gf,cls);
}
VOBA_VALUE_INLINE int32_t voba_gf_id(voba_value_t gf)
{
    assert((gf%256) == VOBA_TYPE_SMALL + (VOBA_TYPE_GENERIC_FUNCTION*8));
    return gf / 256;
}
VOBA_VALUE_INLINE const char * voba_gf_name(voba_value_t gf)
{
    int32_t index = voba_gf_id(gf);
    assert(index < the_voba_gf_table_length);
    return the_voba_gf_table[index].header.name;
}
VOBA_VALUE_INLINE voba_func_t voba_gf_cls_func_pointer(voba_value_t gf, voba_value_t cls)
{
    int32_t gf_index = voba_gf_id(gf);
    int32_t cls_index = voba_class_id(cls);
    assert(gf_index < the_voba_gf_table_length);
    assert(cls_index < the_voba_class_table_length);
    return the_voba_gf_table[gf_index].cls[cls_index];
}
