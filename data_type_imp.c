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
INLINE int voba_is_nil(voba_value_t p) 
{ 
  return voba_eq(p,VOBA_NIL);
}
// ------------- string -----------------
INLINE voba_value_t voba_make_string(voba_str_t* s)
{
    if(s == NULL) return VOBA_NIL;
    return voba_from_pointer(s,VOBA_TYPE_STRING);
}
INLINE voba_value_t voba_make_cstr(const char * s)
{
  return voba_make_string(voba_str_from_cstr(s));
}
INLINE int      voba_is_string(voba_value_t v)
{
    return voba_get_type1(v) == VOBA_TYPE_STRING;
}
INLINE voba_str_t* voba_value_to_str(voba_value_t s)
{
    assert(voba_is_string(s));
    return voba_to_pointer(voba_str_t*,s);
}
// ------------ funcp ----------------------  
INLINE  voba_value_t voba_make_func(voba_func_t p)
{
  return voba_from_pointer((void*)((intptr_t)p),VOBA_TYPE_FUNC);
}
INLINE int voba_is_fun(voba_value_t v)
{
    return voba_get_type1(v) == VOBA_TYPE_FUNC;
}
INLINE  voba_func_t voba_value_to_func(voba_value_t v)
{
  return voba_to_pointer(voba_func_t,v);
}
// ------------- array ---------------------
INLINE voba_value_t voba_make_array(voba_value_t* p)
{
  return voba_from_pointer(p,VOBA_TYPE_ARRAY);
}
INLINE voba_value_t voba_make_var_array(voba_value_t *v)
{
  voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * 2);
  assert(p);
  p[0] = -1;
  // we don't need to attach type info here, so don't call
  // voba_from_pointer
  p[1] = (int64_t)(v);
  return voba_from_pointer(p,VOBA_TYPE_ARRAY);
}
INLINE voba_value_t voba_make_array_nv(int64_t n,va_list ap)
{
  voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * (n + 1));
  p[0] = (n<<32) + n;
  assert(p);
  for(int i = 0 ; i < n; ++i){
    p[i+1] = va_arg(ap,voba_value_t);
  }
  return voba_make_var_array(p);
}
INLINE voba_value_t voba_make_array_n(int64_t n,...) 
{
  voba_value_t ret = VOBA_NIL;
  va_list ap;
  va_start(ap,n);
  ret = voba_make_array_nv(n,ap);
  va_end(ap);
  return ret;
}
INLINE voba_value_t * voba_value_to_c_array(voba_value_t v)
{
  return voba_is_fixed_size_array(v)?
    voba_to_pointer(voba_value_t*, v)
    :
    voba_to_pointer(voba_value_t*, (voba_to_pointer(voba_value_t*, v)[1]))
    ;
}
INLINE int64_t voba_array_capacity(voba_value_t v)
{
  // for fixed size array, it not meaningful and return 0xFFFFFFFF
  return (voba_value_to_c_array(v)[0] >> 32);
}
INLINE int64_t voba_array_len(voba_value_t v)
{
    assert(voba_is_array(v));
  return voba_is_fixed_size_array(v)?
    (voba_value_to_c_array(v)[0])
    :
    ((voba_value_to_c_array(v)[0]) & 0xFFFFFFFF)
    ;
}
INLINE voba_value_t* voba_array_base(voba_value_t v)
{
  return voba_value_to_c_array(v) + 1;
}
INLINE voba_value_t voba_array_at(voba_value_t v,int64_t i)
{
  return voba_array_base(v)[i];
}
INLINE voba_value_t  voba_array_set(voba_value_t a,int64_t i,voba_value_t v)
{
  return voba_array_base(a)[i] = v;
}
INLINE int voba_is_array(voba_value_t v)
{
  return voba_get_type1(v) == VOBA_TYPE_ARRAY;
}
INLINE int voba_is_fixed_size_array(voba_value_t v)
{
    return voba_is_array(v) && voba_to_pointer(voba_value_t*,v)[0] != -1;
}
INLINE int voba_is_var_size_array(voba_value_t v)
{
    return voba_is_array(v) && voba_to_pointer(voba_value_t*,v)[0] == -1;    
}
INLINE int64_t  v__find_next_capacity(int64_t c)
{
  for(int i = 0; i < 64; i ++){
    if((1l << i) > (c+1)){
      return 1l<< i;
    }
  }
  assert(0 && "never reach here");
  return 0;
}
INLINE voba_value_t  voba_array_copy(voba_value_t v)
{
  int64_t c1 = (voba_is_fixed_size_array(v)?
                (voba_array_len(v)+1):
                (voba_array_capacity(v)));
  int64_t capacity = v__find_next_capacity(c1);
  voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * capacity);
  assert(p);
  memcpy((void*)p,voba_value_to_c_array(v),(size_t)(c1*sizeof(voba_value_t)));
  p[0] = (capacity << 32) + voba_array_len(v);
  return voba_make_var_array(p);
}
INLINE void voba_array__enlarge(voba_value_t a, int64_t inc)
{
  if(voba_is_fixed_size_array(a)){
    //throw_exception(a);
    assert(0 && "cannot push to a fixed size array.");
  }
  int64_t len = voba_array_len(a);
  int64_t capacity = voba_array_capacity(a);
  voba_value_t * p = voba_value_to_c_array(a);
  if(len + inc + 1 > capacity){ // one more room for `capacity+size`
    capacity = capacity * 2;
    p = (voba_value_t*) GC_REALLOC((void*)p, capacity*sizeof(voba_value_t));
    assert(p);
    p[0] = (capacity<<32) + len;
    /*
      for example: when capcity is 8  and len is 3

                 /------- from ^p[4]
      3 a0 a1 a2 n0 n1 n2 n3
                 \___ 4 ___/

     */
    bzero(&p[len+1], sizeof(voba_value_t)*(capacity - len - 1));
    voba_to_pointer(voba_value_t *, a)[1] = (voba_value_t)p; // update the pointer
  }
  return;
}
INLINE voba_value_t voba_array_push(voba_value_t a,voba_value_t v)
{
  voba_array__enlarge(a,1);
  voba_value_t * p = voba_value_to_c_array(a);
  int64_t len = voba_array_len(a);
  p[len+1] = v;
  assert(len < UINT32_MAX);
  p[0] = p[0] + 1; // capacity won't be updated. hope so.
  return a;
}
INLINE voba_value_t voba_array_shift(voba_value_t a,voba_value_t v)
{
  voba_array__enlarge(a,1);
  voba_value_t * p = voba_value_to_c_array(a);
  int64_t len = voba_array_len(a);
  memmove(&p[2], &p[1], len * sizeof(voba_value_t));
  p[1] = v;
  assert(len < UINT32_MAX);
  p[0] = p[0] + 1; // capacity won't be updated. hope so.
  return a;
}
INLINE voba_value_t voba_array_pop(voba_value_t a)
{
  int64_t len = voba_array_len(a);
  if(len == 0){
    //throw_exception(a);
    assert(0 && "cannot pop empty array.");
  }
  voba_value_t * p = voba_value_to_c_array(a);
  voba_value_t ret = p[len];
  p[0] = p[0] - 1; // capacity won't be changed.
  p[len] = VOBA_NIL ; // clear it, for gc
  return ret;
}
INLINE voba_value_t voba_array_unshift(voba_value_t a)
{
  int64_t len = voba_array_len(a);
  if(len == 0){
    //throw_exception(a);
    assert(0 && "cannot pop empty array.");
  }
  voba_value_t * p = voba_value_to_c_array(a);
  voba_value_t ret = p[1];
  p[0] = p[0] - 1; // capacity won't be changed.
  memmove(&p[1], &p[2], len * sizeof(voba_value_t));
  return ret;
}
INLINE voba_value_t voba_array_concat(voba_value_t a, voba_value_t b)
{
    assert(voba_is_var_size_array(a));
    assert(voba_is_array(b));
    // TODO, use memcpy
    int64_t len = voba_array_len(b);
    for(int64_t i = 0 ; i < len; ++i){
        voba_array_push(a,voba_array_at(b,i));
    }
    return a;
}
// -- closure
/* INLINE voba_value_t  voba_make_closure_f_a(voba_func_t f, voba_value_t array) */
/* { */
/*   voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t)*(voba_array_len(array)+2)); */
/*   assert(p); */
/*   p[0] = voba_make_func(f); */
/*   memcpy((void*)&p[1], (void*)voba_value_to_c_array(array), sizeof(voba_value_t)*(voba_array_len(array)+1)); */
/*   p[1] = p[1] & 0xFFFFFFFF; // remove capacity if any. */
/*   return voba_from_pointer((void*)p,VOBA_TYPE_CLOSURE); */
/* } */
INLINE int voba_is_closure(voba_value_t v)
{
    return voba_get_type1(v) == VOBA_TYPE_CLOSURE;
}
// return int64_t because voba_value_t is int64_t
INLINE int64_t voba_closure_len(voba_value_t c)
{
    assert(voba_is_closure(c));
    return voba_to_pointer(voba_value_t*, c)[1];
 
}
INLINE voba_value_t voba_closure_at(voba_value_t c,uint32_t i)
{
    assert(voba_is_closure(c));
    assert(((int64_t)i) < voba_closure_len(c));
    return voba_to_pointer(voba_value_t*, c)[i + 2]; // +2, skip func and size.
}
INLINE voba_value_t voba_closure_array(voba_value_t c)
{
    assert(voba_is_closure(c));
    return voba_make_array(&(voba_to_pointer(voba_value_t*, c)[1])); // +1, only skip func
}
INLINE voba_func_t voba_closure_func(voba_value_t c)
{
    assert(voba_is_closure(c));
    return (voba_func_t)voba_to_pointer(voba_value_t*,c)[0];
}
// ---------- pair -----------------------
INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t)
{
  voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * 2);
  assert(p);
  p[0] = h;
  p[1] = t;
  return voba_from_pointer(p,VOBA_TYPE_PAIR);
}
INLINE voba_value_t  voba_is_pair(voba_value_t p)
{
  return voba_get_type1(p) == VOBA_TYPE_PAIR;
}
INLINE voba_value_t  voba_head(voba_value_t p)
{
  return voba_to_pointer(voba_value_t*,p)[0];
}
INLINE voba_value_t  voba_tail(voba_value_t p)
{
  return voba_to_pointer(voba_value_t*,p)[1];
}
INLINE void voba_set_head(voba_value_t p,voba_value_t v)
{
    voba_to_pointer(voba_value_t*,p)[0] = v;
}
INLINE void voba_set_tail(voba_value_t p,voba_value_t v)
{
    voba_to_pointer(voba_value_t*,p)[1] = v;
}
INLINE voba_value_t voba_make_class(voba_class_t * vclass)
{
    voba_value_t ret = voba_make_user_data(VOBA_NIL,sizeof(voba_class_t*));
    *(VOBA_USER_DATA_AS(voba_class_t**,ret)) = vclass;
    return ret;
}
INLINE voba_value_t voba_make_user_data(voba_value_t cls, size_t size)
{
  // size doesn't include the cls function, so allocate a room for it.
  size = (size+sizeof(voba_value_t));
  if(size % 16 !=0){
    // size must be 16 bytes alignment.
    size = (size/16 + 1) * 16;
  }
  voba_value_t * p = (voba_value_t*)GC_MALLOC(size);
  assert(p);
  p[0] = cls;
  return voba_from_pointer(p,VOBA_TYPE_USER);
}
INLINE voba_value_t voba_user_data_class(voba_value_t v)
{
  return (voba_to_pointer(voba_value_t*, v)[0]);
}
INLINE void*  voba_user_data_base(voba_value_t v)
{
  return (void*)(&(voba_to_pointer(voba_value_t*, v)[1]));
}
INLINE voba_value_t voba__make_symbol_lowlevel(voba_value_t symbol_name, voba_value_t symbol_value)
{
    voba_value_t * p = (voba_value_t*)GC_MALLOC(sizeof(voba_value_t) * 2);
    assert(p);
    p[0] = symbol_name;
    p[1] = symbol_value;
    return voba_from_pointer(p,VOBA_TYPE_SYMBOL);
}
INLINE voba_value_t voba_is_symbol(voba_value_t v)
{
  return voba_get_type1(v) == VOBA_TYPE_SYMBOL;
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
    assert(voba_is_symbol(v));
    return voba_head(v);
}
INLINE voba_value_t voba_symbol_value(voba_value_t v)
{
    assert(voba_is_symbol(v));
    return voba_tail(v);
}
INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v)
{
    assert(voba_is_symbol(s));
    voba_set_tail(s,v);
    return v;
}
INLINE int voba_is_hashtable(voba_value_t v)
{
    return voba_cls_hashtable == voba_get_class(v);
}
INLINE int voba_is_symbol_table(voba_value_t v)
{
    return voba_cls_symbol_table == voba_get_class(v);
}
INLINE voba_value_t voba_make_generic_function()
{
    voba_value_t ret = voba_make_user_data(voba_cls_generic_function,sizeof(voba_value_t));
    *(VOBA_USER_DATA_AS(voba_value_t*,ret)) = voba_make_hash();
    return ret;
}
INLINE voba_value_t voba_gf_hash_table(voba_value_t gf)
{
    return *(VOBA_USER_DATA_AS(voba_value_t*,gf));
}
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func)
{
    return voba_hash_insert(voba_gf_hash_table(gf),cls,func);
}
INLINE voba_value_t voba_gf_lookup(voba_value_t gf, voba_value_t cls)
{
    voba_value_t ret = voba_hash_find(voba_gf_hash_table(gf),cls);
    if(!voba_is_nil(ret)){
        ret = voba_tail(ret);
    }
    return ret;
}
INLINE voba_value_t voba__make_la(uint32_t cur, uint32_t end,voba_value_t array)
{
    voba_value_t r = voba_make_user_data(voba_cls_la,sizeof(voba_la_t));
    VOBA_LA(r)->cur = cur;
    VOBA_LA(r)->end = end;
    VOBA_LA(r)->array = array;
    return r;
}
INLINE voba_value_t voba_la_from_array0(voba_value_t array)
{
    uint32_t a_len = ((uint32_t) voba_array_len(array));
    return voba__make_la(0,a_len,array);
}
INLINE voba_value_t voba_la_from_array1(voba_value_t array, uint32_t cur)
{
    uint64_t a_len = ((uint64_t) voba_array_len(array));
    assert(cur <= a_len);
    uint32_t end = (uint32_t)a_len;
    return voba__make_la(cur,end,array);
}
INLINE voba_value_t voba_la_from_array2(voba_value_t array, uint32_t cur,uint32_t len)
{
    uint64_t a_len = ((uint64_t) voba_array_len(array));
    assert(cur + len <= a_len);
    uint32_t end = (uint32_t) (cur + len);
    return voba__make_la(cur,end,array);
}
INLINE voba_value_t voba_la_cons(voba_value_t la, voba_value_t x)
{
    assert(voba_is_la(la));
    uint32_t end = VOBA_LA(la)->end;
    voba_value_t a = VOBA_LA(la)->array;
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
    end ++;
    if(end < a_len){
        voba_array_set(a, end, x);
    }else{
        voba_array_push(a,x);
    }
    VOBA_LA(la)->end = end;
    return la;
}
INLINE voba_value_t voba_la_car(voba_value_t la)
{
    assert(voba_is_la(la));
    uint32_t cur = VOBA_LA(la)->cur;
    voba_value_t a = VOBA_LA(la)->array;
#ifndef NDEBUG
    uint32_t end = VOBA_LA(la)->end;
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
#endif
    assert(cur <= a_len && cur <= end && end <= a_len);
    return voba_array_at(a,(int64_t)cur);
}
INLINE voba_value_t voba_la_cdr(voba_value_t la)
{
    assert(voba_is_la(la));
    uint32_t cur = VOBA_LA(la)->cur;
    uint32_t end = VOBA_LA(la)->end;
#ifndef NDEBUG
    voba_value_t a = VOBA_LA(la)->array;
    uint32_t a_len = ( (uint32_t) voba_array_len(a));
#endif
    assert(cur <= a_len && cur <= end && end <= a_len);
    if(cur < end) {
        cur ++;
    }
    VOBA_LA(la)->cur = cur;
    return la;
}
INLINE voba_value_t voba_la_nil()
{
    return voba__make_la(0,0,voba_make_array_0());
}
INLINE voba_value_t voba_la_copy(voba_value_t la)
{
    assert(voba_is_la(la));
    return voba__make_la(VOBA_LA(la)->cur,VOBA_LA(la)->end,VOBA_LA(la)->array);
}
INLINE uint32_t voba_la_len(voba_value_t la)
{
    assert(voba_is_la(la));
    assert(VOBA_LA(la)->end >= VOBA_LA(la)->cur);
    return VOBA_LA(la)->end - VOBA_LA(la)->cur;
}
INLINE voba_value_t voba_la_concat(voba_value_t la1,voba_value_t la2)
{
    voba_value_t ret = voba_la_nil();
    voba_value_t x = voba_la_copy(la1);
    while(!voba_la_is_nil(x)){
        ret = voba_la_cons(ret,voba_la_car(x));
        x = voba_la_cdr(x);
    }
    x = voba_la_copy(la2);
    while(!voba_la_is_nil(x)){
        ret = voba_la_cons(ret,voba_la_car(x));
        x = voba_la_cdr(x);
    }
    return ret;
}
INLINE int voba_la_is_nil(voba_value_t la)
{
    assert(voba_is_la(la));
    return voba_la_len(la) == 0;
}
INLINE int voba_is_la(voba_value_t la)
{
    return (voba_cls_la == voba_get_class(la));
}
INLINE voba_func_t voba__apply_find_func(voba_value_t f, voba_value_t a1)
{
    voba_func_t ret = NULL;
    if(voba_get_type1(f) == VOBA_TYPE_USER &&
       voba_user_data_class(f) == voba_cls_generic_function &&
       voba_array_len(a1) >= 1){
            voba_value_t vf = voba_gf_lookup(f,voba_get_class(voba_array_at(a1,0)));
            if(voba_get_type1(vf) == VOBA_TYPE_FUNC){
                ret = voba_value_to_func(vf);
            }else{
                voba_throw_exception(voba_make_string(voba_str_from_cstr("vfunc is not found")));
            }
    } else {
        voba_value_t vf = voba_gf_lookup(voba_gf_apply,voba_get_class(f));
        switch(voba_get_type1(vf)){
        case VOBA_TYPE_FUNC:
            ret = voba_value_to_func(vf);
            break;
        case VOBA_TYPE_CLOSURE:
            voba_throw_exception(voba_make_string(voba_str_from_cstr("closure is not allowed")));
            ret = NULL;
        default:
            ret = voba__apply_find_func(voba_gf_apply,vf);
        }
    }
    return ret;
}
INLINE voba_value_t voba_direct_apply(voba_func_t f,voba_value_t args)
{
    return f(voba_make_func(f),args);
}
INLINE voba_value_t voba_apply(voba_value_t f, voba_value_t a1)
{
    switch(voba_get_type1(f)){
    case VOBA_TYPE_FUNC:
        return voba_direct_apply(voba_value_to_func(f),a1);
    case VOBA_TYPE_CLOSURE:
        return voba_closure_func(f)(voba_closure_array(f),a1);
    }
    voba_func_t func = voba__apply_find_func(f,a1);
    if(func != NULL){
        return func(f,a1);
    }else{
        voba_throw_exception(
            voba_make_string(voba_str_from_cstr("cannot apply")));
    }
    return VOBA_NIL;
}
// get_class
#define VOBA_GET_CLASS_SMALL(tag,name,type) case tag:   return voba_cls_##name;
INLINE voba_value_t voba_get_class(voba_value_t v)
{
    switch(voba_get_type1(v)){
    case VOBA_TYPE_SMALL:
        switch(voba_get_type2(v)){
            VOBA_SMALL_TYPES(VOBA_GET_CLASS_SMALL)
        default:
            assert(0); // not implemented.
        }
    case VOBA_TYPE_SYMBOL:
        return voba_cls_symbol;
    case VOBA_TYPE_FUNC:
        return voba_cls_func;
    case VOBA_TYPE_ARRAY:
        return voba_cls_array;
    case VOBA_TYPE_CLOSURE:
        return voba_cls_closure;
    case VOBA_TYPE_PAIR:
        return voba_cls_pair;
    case VOBA_TYPE_USER:
        return voba_user_data_class(v)?voba_user_data_class(v):voba_cls_user;
    case VOBA_TYPE_STRING:
        return voba_cls_str;
    case VOBA_TYPE_NIL:
        return voba_cls_nil;
    default:
        assert(0);
    }
    return 0;
}
INLINE  const char * voba_get_class_name(voba_value_t v1)
{
    voba_value_t s  = voba_get_class(v1);
    voba_value_t v = voba_symbol_value(s);
    voba_class_t ** x = VOBA_USER_DATA_AS(voba_class_t**, v);
    return (*x)->name;
}
// small type
// ----------- boolean ------------------
INLINE int voba_is_true(voba_value_t v)
{
  return v == VOBA_TRUE;
}
INLINE int voba_is_false(voba_value_t v)
{
  return v == VOBA_FALSE;
}

// ------------- get type and get class ----
INLINE int64_t voba_get_type1(voba_value_t v)
{
    return voba_is_nil(v)?VOBA_TYPE_NIL:(v&VOBA_TYPE_MASK);
}
INLINE int64_t voba_get_type2(voba_value_t v)
{
  return (v&0xff)>>3;
}

#define DEFINE_SMALL_TYPE(tag,name,type)        \
INLINE voba_value_t voba_make_##name (type v)             \
{                                               \
    union {                                     \
        voba_value_t v1;                        \
        type v2;                                \
    } r;                                        \
    r.v1 = 0;                                   \
    r.v2 = v;                                   \
    return (VOBA_TYPE_SMALL + tag * 8 + r.v1 * 256);    \
}                                               \
INLINE type voba_value_to_##name (voba_value_t v)         \
{                                               \
    union {                                     \
        voba_value_t v1;                        \
        type v2;                                \
    } r;                                        \
    r.v1 = v;                                   \
    r.v1 = r.v1 >> 8;                           \
    return r.v2;                                \
}

VOBA_SMALL_TYPES(DEFINE_SMALL_TYPE)





static inline voba_value_t clz_long(uint32_t s) 
{
    int a = 0;
#if ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
    a = __builtin_clz(s);
#else
#error("TODO implement clzl")
#endif
    return (1l<<(32 - a));
}

// I really hate to write these macros, but I hate more duplicate
// code. I used to use python or M4 generate the following code, but
// it isn't more readable and it is not so good to depend on tools
// other than standard compilers.

#define DEFINE_VOBA_MAKE_ARRAY_N(n)                                     \
    INLINE voba_value_t                                                 \
    voba_make_array_##n (VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG, COMMA)) {              \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * clz_long(n+1));            \
        p[0] = (clz_long(n+1)<<32) + (n);                               \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_ARRAY_ASSIGN,SEMI_COMMA);   \
        return voba_make_var_array(p);                                  \
    }
#define DEFINE_VOBA_MAKE_ARRAY_ASSIGN(n) p[n + 1] = a##n


VOBA_FOR_EACH(DEFINE_VOBA_MAKE_ARRAY_N,SPACE)

#define DEFINE_VOBA_MAKE_CLOSURE_N(n)                                   \
    INLINE voba_value_t voba_make_closure_##n                           \
    (voba_func_t f VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG2, SPACE))          \
    {                                                                   \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * (n+2));                    \
        p[0] = voba_make_func(f);                                       \
        p[1] = n;                                                       \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_CLOSURE_ASSIGN,SEMI_COMMA); \
        return voba_from_pointer((void*)p,VOBA_TYPE_CLOSURE);           \
    }
#define DEFINE_VOBA_MAKE_CLOSURE_ASSIGN(n) p[n + 2] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_CLOSURE_N,SPACE)

