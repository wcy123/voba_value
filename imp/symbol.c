#include "symbol.h"

INLINE voba_value_t voba__make_symbol_lowlevel(voba_value_t symbol_name, voba_value_t symbol_value)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * 2);
    assert(p);
    p[0] = symbol_name;
    p[1] = symbol_value;
    return voba_from_pointer(p,VOBA_TYPE_SYMBOL);
}
INLINE voba_value_t voba_make_symbol_cstr(const char * symbol_name, voba_value_t symbol_table)
{
    return voba_make_symbol_data(symbol_name,(uint32_t)strlen(symbol_name), symbol_table);
}
INLINE voba_value_t voba_make_symbol_cstr_with_value(const char * symbol_name, voba_value_t symbol_table, voba_value_t v)
{
    voba_value_t ret = voba_make_symbol_data(symbol_name,(uint32_t)strlen(symbol_name), symbol_table);
    voba_symbol_set_value(ret,v);
    return ret;
}
INLINE voba_value_t voba_make_symbol_data(const char * data, uint32_t len, voba_value_t symbol_table)
{
    voba_str_t tmp;
    tmp.data = (char*)data;
    tmp.capacity = 0;
    tmp.len = len;
    voba_value_t ret = voba_make_symbol(&tmp,symbol_table);
    return ret;
}
INLINE voba_value_t voba_symbol_name(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_symbol));
    return voba_head(v);
}
INLINE voba_value_t voba_symbol_value(voba_value_t v)
{
    assert(voba_is_a(v,voba_cls_symbol));
    return voba_tail(v);
}
INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v)
{
    assert(voba_is_a(s,voba_cls_symbol));
    voba_set_tail(s,v);
    return v;
}

