INLINE vtype_t from_pointer(void* p, int type)
{
  // it is good to apply &(~V_TYPE_MASK), so that link time
  // optimizator is able to figure out the type automatically.  it
  // is safe to assume any pointer is 8 bytes aligned.
  assert((((int64_t)p)&V_TYPE_MASK) == 0);
  return (((vtype_t)p) & (~V_TYPE_MASK)) | type;
}
INLINE int eq(vtype_t a,vtype_t b) 
{
  return a == b;
}
INLINE int is_nil(vtype_t p) 
{ 
  return eq(p,NIL);
}
// ------------- string -----------------
INLINE vtype_t make_string(voba_str_t* s)
{
    if(s == NULL) return NIL;
    return from_pointer(s,V_STRING);
}
INLINE vtype_t make_cstr(const char * s)
{
  return make_string(voba_str_from_cstr(s));
}
INLINE int      is_string(vtype_t v)
{
    return get_type1(v) == V_STRING;
}
INLINE voba_str_t* vtype_to_str(vtype_t s)
{
    return to_pointer(voba_str_t*,s);
}
// ------------ funcp ----------------------  
INLINE  vtype_t make_func(vfunc_t p)
{
  return from_pointer((void*)((intptr_t)p),V_FUNC);
}
INLINE  vfunc_t vtype_to_func(vtype_t v)
{
  return to_pointer(vfunc_t,v);
}
// ------------- array ---------------------
INLINE vtype_t make_array(vtype_t* p)
{
  return from_pointer(p,V_ARRAY);
}
INLINE vtype_t make_var_array(vtype_t *v)
{
  vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t) * 2);
  assert(p);
  p[0] = -1;
  // we don't need to attach type info here, so don't call
  // from_pointer
  p[1] = (int64_t)(v);
  return from_pointer(p,V_ARRAY);
}
INLINE vtype_t make_array_nv(int64_t n,va_list ap)
{
  vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t) * (n + 1));
  p[0] = (n<<32) + n;
  assert(p);
  for(int i = 0 ; i < n; ++i){
    p[i+1] = va_arg(ap,vtype_t);
  }
  return make_var_array(p);
}
INLINE vtype_t make_array_n(int64_t n,...) 
{
  vtype_t ret = NIL;
  va_list ap;
  va_start(ap,n);
  ret = make_array_nv(n,ap);
  va_end(ap);
  return ret;
}
INLINE vtype_t * vtype_to_c_array(vtype_t v)
{
  return is_fixed_size_array(v)?
    to_pointer(vtype_t*, v)
    :
    to_pointer(vtype_t*, (to_pointer(vtype_t*, v)[1]))
    ;
}
INLINE int64_t array_capacity(vtype_t v)
{
  // for fixed size array, it not meaningful and return 0xFFFFFFFF
  return (vtype_to_c_array(v)[0] >> 32);
}
INLINE int64_t array_len(vtype_t v)
{
  return is_fixed_size_array(v)?
    (vtype_to_c_array(v)[0])
    :
    ((vtype_to_c_array(v)[0]) & 0xFFFFFFFF)
    ;
}
INLINE vtype_t* array_base(vtype_t v)
{
  return vtype_to_c_array(v) + 1;
}
INLINE vtype_t array_at(vtype_t v,int64_t i)
{
  return array_base(v)[i];
}
INLINE vtype_t  array_set(vtype_t a,int64_t i,vtype_t v)
{
  return array_base(a)[i] = v;
}
INLINE int is_array(vtype_t v)
{
  return get_type1(v) == V_ARRAY;
}
INLINE int is_fixed_size_array(vtype_t v)
{
  return is_array(v) && to_pointer(vtype_t*,v)[0] != -1;
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
INLINE vtype_t  array_copy(vtype_t v)
{
  int64_t c1 = (is_fixed_size_array(v)?
                (array_len(v)+1):
                (array_capacity(v)));
  int64_t capacity = v__find_next_capacity(c1);
  vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t) * capacity);
  assert(p);
  memcpy((void*)p,vtype_to_c_array(v),(size_t)(c1*sizeof(vtype_t)));
  p[0] = (capacity << 32) + array_len(v);
  return make_var_array(p);
}
INLINE void array__enlarge(vtype_t a, int64_t inc)
{
  if(is_fixed_size_array(a)){
    //throw_exception(a);
    assert(0 && "cannot push to a fixed size array.");
  }
  int64_t len = array_len(a);
  int64_t capacity = array_capacity(a);
  vtype_t * p = vtype_to_c_array(a);
  if(len + inc + 1 > capacity){ // one more room for `capacity+size`
    capacity = capacity * 2;
    p = (vtype_t*) GC_REALLOC((void*)p, capacity*sizeof(vtype_t));
    assert(p);
    p[0] = (capacity<<32) + len;
    /*
      for example: when capcity is 8  and len is 3

                 /------- from ^p[4]
      3 a0 a1 a2 n0 n1 n2 n3
                 \___ 4 ___/

     */
    bzero(&p[len+1], sizeof(vtype_t)*(capacity - len - 1));
    to_pointer(vtype_t *, a)[1] = (vtype_t)p; // update the pointer
  }
  return;
}
INLINE vtype_t array_push(vtype_t a,vtype_t v)
{
  array__enlarge(a,1);
  vtype_t * p = vtype_to_c_array(a);
  int64_t len = array_len(a);
  p[len+1] = v;
  assert(len < UINT32_MAX);
  p[0] = p[0] + 1; // capacity won't be updated. hope so.
  return a;
}
INLINE vtype_t array_shift(vtype_t a,vtype_t v)
{
  array__enlarge(a,1);
  vtype_t * p = vtype_to_c_array(a);
  int64_t len = array_len(a);
  memmove(&p[2], &p[1], len * sizeof(vtype_t));
  p[1] = v;
  assert(len < UINT32_MAX);
  p[0] = p[0] + 1; // capacity won't be updated. hope so.
  return a;
}
INLINE vtype_t array_pop(vtype_t a)
{
  int64_t len = array_len(a);
  if(len == 0){
    //throw_exception(a);
    assert(0 && "cannot pop empty array.");
  }
  vtype_t * p = vtype_to_c_array(a);
  vtype_t ret = p[len];
  p[0] = p[0] - 1; // capacity won't be changed.
  p[len] = NIL ; // clear it, for gc
  return ret;
}
INLINE vtype_t array_unshift(vtype_t a)
{
  int64_t len = array_len(a);
  if(len == 0){
    //throw_exception(a);
    assert(0 && "cannot pop empty array.");
  }
  vtype_t * p = vtype_to_c_array(a);
  vtype_t ret = p[1];
  p[0] = p[0] - 1; // capacity won't be changed.
  memmove(&p[1], &p[2], len * sizeof(vtype_t));
  return ret;
}
// -- closure
INLINE vtype_t  make_closure(vtype_t * p)
{
  return from_pointer((void*)p,V_CLOSURE);
}
INLINE vtype_t  make_closure_f_a(vfunc_t f, vtype_t array)
{
  vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t)*(array_len(array)+2));
  assert(p);
  p[0] = make_func(f);
  memcpy((void*)&p[1], (void*)vtype_to_c_array(array), sizeof(vtype_t)*(array_len(array)+1));
  p[1] = p[1] & 0xFFFFFFFF; // remove capacity if any.
  return from_pointer((void*)p,V_CLOSURE);
}
INLINE vtype_t closure_array(vtype_t c)
{
  return make_array(&(to_pointer(vtype_t*, c)[1]));
}
INLINE vfunc_t closure_func(vtype_t c)
{
  return (vfunc_t)to_pointer(vtype_t*,c)[0];
}
// ---------- pair -----------------------
INLINE vtype_t  voba_make_pair(vtype_t h,vtype_t t)
{
  vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t) * 2);
  assert(p);
  p[0] = h;
  p[1] = t;
  return from_pointer(p,V_PAIR);
}
INLINE vtype_t  is_pair(vtype_t p)
{
  return get_type1(p) == V_PAIR;
}
INLINE vtype_t  head(vtype_t p)
{
  return to_pointer(vtype_t*,p)[0];
}
INLINE vtype_t  tail(vtype_t p)
{
  return to_pointer(vtype_t*,p)[1];
}
INLINE void set_head(vtype_t p,vtype_t v)
{
    to_pointer(vtype_t*,p)[0] = v;
}
INLINE void set_tail(vtype_t p,vtype_t v)
{
    to_pointer(vtype_t*,p)[1] = v;
}
INLINE vtype_t make_class(vclass_t * vclass)
{
    vtype_t ret = make_user_data(NIL,sizeof(vclass_t*));
    *(USER_DATA_AS(vclass_t**,ret)) = vclass;
    return ret;
}
INLINE vtype_t make_user_data(vtype_t cls, size_t size)
{
  // size doesn't include the cls function, so allocate a room for it.
  size = (size+sizeof(vtype_t));
  if(size % 16 !=0){
    // size must be 16 bytes alignment.
    size = (size/16 + 1) * 16;
  }
  vtype_t * p = (vtype_t*)GC_MALLOC(size);
  assert(p);
  p[0] = cls;
  return from_pointer(p,V_USER);
}
INLINE vtype_t user_data_class(vtype_t v)
{
  return (to_pointer(vtype_t*, v)[0]);
}
INLINE void*  user_data_base(vtype_t v)
{
  return (void*)(&(to_pointer(vtype_t*, v)[1]));
}
INLINE vtype_t make_symbol_table()
{
    return make_symbol_table_cpp();
}
INLINE vtype_t make_symbol_internal(vtype_t symbol_name, vtype_t symbol_value)
{
    vtype_t * p = (vtype_t*)GC_MALLOC(sizeof(vtype_t) * 2);
    assert(p);
    p[0] = symbol_name;
    p[1] = symbol_value;
    return from_pointer(p,V_SYMBOL);
}
INLINE vtype_t is_symbol(vtype_t v)
{
  return get_type1(v) == V_SYMBOL;
}
INLINE vtype_t make_symbol(voba_str_t * symbol_name, vtype_t symbol_table)
{
    return make_symbol_cpp(symbol_name,symbol_table);
}
INLINE vtype_t make_symbol_cstr(const char * symbol_name, vtype_t symbol_table)
{
    vtype_t ret =  make_symbol_cpp(voba_str_from_cstr(symbol_name),symbol_table);
    fprintf(stderr,__FILE__ ":%d:[%s] symbol %s 0x%lx table = 0x%lx(%p)\n", __LINE__, __FUNCTION__,
            symbol_name, ret, symbol_table, user_data_base(symbol_table));
    return ret;
}
INLINE vtype_t symbol_name(vtype_t v)
{
    return head(v);
}
INLINE vtype_t symbol_value(vtype_t v)
{
    return tail(v);
}
INLINE vtype_t* symbol_value_ptr(vtype_t v)
{
    return &(to_pointer(vtype_t*,v)[0]);
}
INLINE vtype_t symbol_set_value(vtype_t s,vtype_t v)
{
    set_tail(s,v);
    return v;
}
INLINE vtype_t make_generic_function()
{
    vtype_t ret = make_user_data(voba_cls_generic_function,sizeof(vtype_t));
    *(USER_DATA_AS(vtype_t*,ret)) = make_hash();
    return ret;
}
INLINE vtype_t gf_hash_table(vtype_t gf)
{
    return *(USER_DATA_AS(vtype_t*,gf));
}
INLINE vtype_t gf_add_class(vtype_t gf, vtype_t cls, vtype_t func)
{
    return hash_insert(gf_hash_table(gf),cls,func);
}
INLINE vtype_t gf_lookup(vtype_t gf, vtype_t cls)
{
    vtype_t ret = hash_find(gf_hash_table(gf),cls);
    if(!is_nil(ret)){
        ret = tail(ret);
    }
    return ret;
}
INLINE vtype_t apply(vtype_t f, vtype_t a1)
{
    switch(get_type1(f)){
    case V_FUNC:
        return vtype_to_func(f)(f,a1);
    case V_CLOSURE:
        return closure_func(f)(closure_array(f),a1);
    case V_USER:
        if(user_data_class(f) == voba_cls_generic_function && array_len(a1) >= 1){
            vtype_t vf = gf_lookup(f,get_class_internal(array_at(a1,0)));
            if(get_type1(vf) == V_FUNC){
                return vtype_to_func(vf)(f,a1);
            }else{
                throw_exception(make_string(voba_str_from_cstr("vfunc is not found")));
            }
        }
    }
    vtype_t vf = gf_lookup(gf_apply,get_class_internal(f));
    switch(get_type1(vf)){
    case V_FUNC:
        return vtype_to_func(vf)(f,a1);
    }
    throw_exception(make_string(voba_str_from_cstr("cannot apply")));
    return NIL;
}
// get_class
INLINE vtype_t get_class_internal(vtype_t v)
{
    switch(get_type1(v)){
    case V_SMALL:
        switch(get_type2(v)){
        case V_I8:   return voba_cls_i8;
        case V_I16:  return voba_cls_i16;
        case V_I32:    return voba_cls_i32;
        case V_FLOAT:  return voba_cls_float;
        case V_BOOLEAN: return voba_cls_bool;
        case V_SHORT_SYMBOL: return voba_cls_short_symbol;
        default:
            assert(0); // not implemented.
        }
    case V_SYMBOL:
        return voba_cls_symbol;
    case V_FUNC:
        return voba_cls_func;
    case V_ARRAY:
        return voba_cls_array;
    case V_CLOSURE:
        return voba_cls_closure;
    case V_PAIR:
        return voba_cls_pair;
    case V_USER:
        return user_data_class(v)?user_data_class(v):voba_cls_user;
    case V_STRING:
        return voba_cls_str;
    case V_NIL:
        return voba_cls_nil;
    default:
        assert(0);
    }
    return 0;
}
// small type
// ----------- boolean ------------------
INLINE int is_true(vtype_t v)
{
  return v == TRUE;
}
INLINE int is_false(vtype_t v)
{
  return v == FALSE;
}

// ------------- get type and get class ----
INLINE int64_t get_type1(vtype_t v)
{
  return is_nil(v)?V_NIL:(v&V_TYPE_MASK);
}
INLINE int64_t get_type2(vtype_t v)
{
  return (v&0xff)>>3;
}

#define DEFINE_SMALL_TYPE(tag,name,type)        \
INLINE vtype_t make_##name (type v)             \
{                                               \
    union {                                     \
        vtype_t v1;                             \
        type v2;                                \
    } r;                                        \
    r.v1 = 0;                                   \
    r.v2 = v;                                   \
    return (V_SMALL + tag * 8 + r.v1 * 256);    \
}                                               \
INLINE type vtype_to_##name (vtype_t v)         \
{                                               \
    union {                                     \
        vtype_t v1;                             \
        type v2;                                \
    } r;                                        \
    r.v1 = v;                                   \
    r.v1 = r.v1 >> 8;                           \
    return r.v2;                                \
}

VOBA_SMALL_TYPES(DEFINE_SMALL_TYPE)


