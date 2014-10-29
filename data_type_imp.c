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
        if(voba_is_int(a) && voba_is_int(b)){
            int64_t a1 = voba_int_value_to_i64(a);
            int64_t b1 = voba_int_value_to_i64(b);
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
INLINE voba_str_t* voba_value_to_str(voba_value_t s)
{
    assert(voba_is_a(s,voba_cls_str));
    return voba_to_pointer(voba_str_t*,s);
}
// ------------ funcp ----------------------  
INLINE  voba_value_t voba_make_func(voba_func_t p)
{
  return voba_from_pointer((void*)((intptr_t)p),VOBA_TYPE_FUNC);
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
    assert(voba_is_a(v,voba_cls_array));
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
INLINE int voba_is_fixed_size_array(voba_value_t v)
{
    return voba_is_a(v,voba_cls_array) && voba_to_pointer(voba_value_t*,v)[0] != -1;
}
INLINE int voba_is_var_size_array(voba_value_t v)
{
    return voba_is_a(v,voba_cls_array) && voba_to_pointer(voba_value_t*,v)[0] == -1;    
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
    assert(voba_is_a(b,voba_cls_array));
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
// return int64_t because voba_value_t is int64_t
INLINE int64_t voba_closure_len(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
    return voba_to_pointer(voba_value_t*, c)[1];
 
}
INLINE voba_value_t voba_closure_at(voba_value_t c,uint32_t i)
{
    assert(voba_is_a(c,voba_cls_closure));
    assert(((int64_t)i) < voba_closure_len(c));
    return voba_to_pointer(voba_value_t*, c)[i + 2]; // +2, skip func and size.
}
INLINE voba_value_t voba_closure_array(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
    return voba_make_array(&(voba_to_pointer(voba_value_t*, c)[1])); // +1, only skip func
}
INLINE voba_func_t voba_closure_func(voba_value_t c)
{
    assert(voba_is_a(c,voba_cls_closure));
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
INLINE voba_value_t voba_make_cls(size_t size, const char * name)
{
    voba_value_t ret = voba_make_user_data(voba_cls_cls);
    VOBA_CLS(ret)->name = name;
    VOBA_CLS(ret)->size = size;
    return ret;
}
INLINE voba_value_t* voba_allocate_user_data(size_t s)
{
    size_t size = (s+sizeof(voba_value_t));
    if(size % 16 !=0){
        // size must be 16 bytes alignment.
        size = (size/16 + 1) * 16;
    }
    voba_value_t * p = (voba_value_t*)GC_MALLOC(size);
    assert(p);
    return p;
}
INLINE voba_value_t voba_make_user_data(voba_value_t cls)
{
    // size doesn't include the cls function, so allocate a room for it.
    assert(VOBA_CLS(cls)->size > 0);
    voba_value_t * p = voba_allocate_user_data(VOBA_CLS(cls)->size);
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
INLINE int voba_is_symbol_table(voba_value_t v)
{
    return voba_cls_symbol_table == voba_get_class(v);
}
INLINE voba_value_t voba_make_generic_function(const char * name)
{
    voba_value_t ret = voba_make_user_data(voba_cls_generic_function);
    VOBA_GF(ret)->hash = voba_make_hash();
    VOBA_GF(ret)->fun = (voba_func_t)0;
    VOBA_GF(ret)->name = name;
    return ret;
}
INLINE voba_value_t voba_gf_add_class(voba_value_t gf, voba_value_t cls, voba_value_t func)
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    return voba_hash_insert(VOBA_GF(gf)->hash,cls,func);
}
INLINE voba_value_t voba_gf_lookup(voba_value_t gf, voba_value_t cls)
{
    assert(voba_is_a(gf,voba_cls_generic_function));
    voba_value_t ret = voba_hash_find(VOBA_GF(gf)->hash,cls);
    if(!voba_is_nil(ret)){
        ret = voba_tail(ret);
    }else{
        if(VOBA_GF(gf)->fun){
            ret = voba_make_func(VOBA_GF(gf)->fun);
        }
    }
    return ret;
}
INLINE voba_value_t voba__make_la(uint32_t cur, uint32_t end,voba_value_t array)
{
    voba_value_t r = voba_make_user_data(voba_cls_la);
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
#ifndef NDEBUG
    uint64_t a_len = ((uint64_t) voba_array_len(array));
#endif
    assert(cur + len <= a_len);
    uint32_t end = (uint32_t) (cur + len);
    return voba__make_la(cur,end,array);
}
INLINE voba_value_t voba_la_car(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
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
    assert(voba_is_a(la,voba_cls_la));
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
    assert(voba_is_a(la,voba_cls_la));
    return voba__make_la(VOBA_LA(la)->cur,VOBA_LA(la)->end,VOBA_LA(la)->array);
}
INLINE uint32_t voba_la_len(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    assert(VOBA_LA(la)->end >= VOBA_LA(la)->cur);
    return VOBA_LA(la)->end - VOBA_LA(la)->cur;
}
INLINE int voba_la_is_nil(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    return voba_la_len(la) == 0;
}
INLINE voba_value_t voba_la_to_array(voba_value_t la)
{
    assert(voba_is_a(la,voba_cls_la));
    voba_value_t ret = voba_make_array_0();
    voba_value_t xs = voba_la_copy(la);
    while(voba_is_nil(xs)){
        voba_value_t x = voba_la_car(xs);
        voba_array_push(ret,x);
        xs = voba_la_cdr(xs);
    }
    return ret;
}
INLINE voba_func_t voba__apply_find_func(voba_value_t f, voba_value_t a1)
{
    voba_func_t ret = NULL;
    if(voba_is_a(f,voba_cls_generic_function) &&
       voba_array_len(a1) >= 1){
        voba_value_t cls = voba_get_class(voba_array_at(a1,0));
        voba_value_t vf = voba_gf_lookup(f,cls);
        if(voba_get_type1(vf) == VOBA_TYPE_FUNC){
            ret = voba_value_to_func(vf);
        }else{
            voba_throw_exception(
                voba_make_string(
                    VOBA_STRCAT(
                        VOBA_CONST_CHAR("vfunc `"),
                        voba_str_from_cstr(VOBA_GF(f)->name),
                        VOBA_CONST_CHAR("' is not found for cls `"),
                        voba_str_from_cstr(VOBA_CLS(cls)->name),
                        VOBA_CONST_CHAR("'")
                        )));
        }
    } else {
        voba_value_t self  = f;
        voba_value_t cls = voba_get_class(self);
        voba_value_t vf = voba_gf_lookup(voba_gf_apply,cls);
        int64_t type1 = voba_get_type1(vf);
        if(!voba_is_nil(vf) && type1 == VOBA_TYPE_FUNC){
            ret = voba_value_to_func(vf);
        }else{
            voba_throw_exception(
                voba_make_string(
                    VOBA_STRCAT(
                        VOBA_CONST_CHAR("object of `"),
                        voba_str_from_cstr(VOBA_CLS(cls)->name),
                        VOBA_CONST_CHAR("' is not callable`")
                        )));
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
    switch(v){
    case VOBA_UNDEF:
        return voba_cls_undef;
        break;
    case VOBA_NIL:
        return voba_cls_nil;
    case VOBA_TRUE:
    case VOBA_FALSE:
        return voba_cls_bool;
    }
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
        return voba_user_data_class(v);
    case VOBA_TYPE_STRING:
        return voba_cls_str;
    default:
        assert(0);
    }
    return 0;
}
INLINE int voba_is_a(voba_value_t v, voba_value_t cls)
{
    return voba_get_class(v) == cls;
}
INLINE  const char * voba_get_class_name(voba_value_t v1)
{
    voba_value_t s  = voba_get_class(v1);
    voba_cls_t * p = VOBA_CLS(s);
    return p->name;
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
INLINE voba_value_t voba_not(voba_value_t v)
{
    voba_value_t ret = VOBA_FALSE;
    if(voba_is_false(v)){
        ret = VOBA_TRUE;
    }else{
        ret = VOBA_FALSE;
    }
    return ret;
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





INLINE voba_value_t clz_long(uint32_t s) 
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





INLINE int64_t voba_int_value_to_i64(voba_value_t a)
{
    assert(voba_is_int(a));
    return (a>>8);
}
INLINE int voba_is_int(voba_value_t a )
{
    voba_value_t cls = voba_get_class(a);
    if(voba_eq(cls,voba_cls_i8) ||
       voba_eq(cls,voba_cls_i16) ||
       voba_eq(cls,voba_cls_i32) ||
       voba_eq(cls,voba_cls_u8) ||
       voba_eq(cls,voba_cls_u16) ||
       voba_eq(cls,voba_cls_u32))
    {
        return 1;
    }
    return 0;
}
INLINE voba_value_t i64_to_voba_int_value(int64_t a)
{
    voba_value_t ret = voba_make_i32(0);
    if(a >= INT8_MIN && a <= INT8_MAX){
        int8_t x = (int8_t) a;
        ret = voba_make_i8(x);
    }else if(a >= 0 && a <= UINT8_MAX){
        uint8_t x = (uint8_t) a;
        ret = voba_make_u8(x);
    }else if(a >= INT16_MIN && a <= INT16_MAX){
        int16_t x = (int16_t) a;
        ret = voba_make_i16(x);
    }else if(a >= 0 && a <= UINT16_MAX){
        uint16_t x = (uint16_t) a;
        ret = voba_make_u16(x);
    }else if(a >= INT32_MIN && a <= INT32_MAX){
        int32_t x = (int32_t) a;
        ret = voba_make_i32(x);
    }else if(a >= 0 && a <= UINT32_MAX){
        uint32_t x = (uint32_t) a;
        ret = voba_make_u32(x);
    }else{
        voba_throw_exception(voba_make_string(VOBA_CONST_CHAR("32-bit integer overflow")));
    }
    return ret;
}
