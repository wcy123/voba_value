#pragma once
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
        if(voba_is_int(a) && voba_is_int(b)){
            int64_t a1 = voba_int_value_to_i32(a);
            int64_t b1 = voba_int_value_to_i32(b);
            if(a1 == b1){
                ret = 1;
            }
        }else if(voba_eq(cls1,cls2)){
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
INLINE int voba_is_nil(voba_value_t p) 
{ 
  return voba_eq(p,VOBA_NIL);
}


