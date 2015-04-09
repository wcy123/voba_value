
INLINE voba_value_t voba_from_pointer(void* p, int type)
{
  // it is good to apply &(~VOBA_TYPE_MASK), so that link time
  // optimizator is able to figure out the type automatically.  it
  // is safe to assume any pointer is 8 bytes aligned.
  assert((((int64_t)p)&VOBA_TYPE_MASK) == 0);
  return (((voba_value_t)p) & (~VOBA_TYPE_MASK)) | type;
}
INLINE int voba_eq(voba_value_t a,voba_value_t b) 
{
  return a == b;
}
INLINE int voba_eql(voba_value_t a,voba_value_t b)
{
    voba_value_t ret = 0;
    if(voba_eq(a,b)){
        ret = 1;
    }else{
        voba_value_t cls1 = voba_get_class(a);
        voba_value_t cls2 = voba_get_class(b);
	if(voba_eq(cls1,cls2)){
            if(voba_eq(cls1, voba_cls_str)){
                ret = voba_strcmp(voba_value_to_str(a),
                                  voba_value_to_str(b)) == 0;
            }else{
                ret = 0;
            }
        }else{
            ret = 0;
        }
    }
    return ret;
}

