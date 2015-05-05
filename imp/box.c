VOBA_VALUE_INLINE voba_value_t  voba_box(voba_value_t value)
{
    voba_value_t * ret = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t));
    *ret = value;
    return voba_pointer_to_box(ret);
}
VOBA_VALUE_INLINE voba_value_t  voba_unbox(voba_value_t box)
{
    assert(voba_is_a(box,voba_cls_box));
    return *(voba_box_to_pointer(box));
}
VOBA_VALUE_INLINE voba_value_t voba_pointer_to_box(voba_value_t * p)
{
    return voba_from_pointer((void*)p,VOBA_TYPE_BOX);
}
VOBA_VALUE_INLINE voba_value_t* voba_box_to_pointer(voba_value_t value)
{
    return voba_to_pointer(voba_value_t*,value);
}
