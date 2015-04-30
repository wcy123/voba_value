INLINE voba_value_t  voba_box(voba_value_t value)
{
    voba_value_t * ret = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t));
    *ret = value;
    return voba_from_pointer((void*)ret,VOBA_TYPE_BOX);
}
INLINE voba_value_t  voba_unbox(voba_value_t box)
{
    assert(voba_is_a(box,voba_cls_box));
    return *(voba_to_pointer(voba_value_t*,box));
}
