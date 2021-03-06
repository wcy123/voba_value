#include <iostream>
#include <iomanip>
#include <vhash.h>
#include <exec_once.h>
#include "value.h"
using namespace std;
static uint32_t SuperFastHash (const char * data, int len);
/// todo
struct vtype_hasher {
    size_t operator() (const voba_value_t key)
        {
            if(!voba_is_a(key,voba_cls_str)){
                return (key>>3);
            }else{
                voba_str_t * s = voba_value_to_str(key);
                return SuperFastHash(s->data,s->len);
            }
        }
};
/// todo
struct vtype_equal {
    bool operator() (const voba_value_t k1, const voba_value_t k2) const
        {
            if(k1 == k2){
                return true;
            }
            if(!(voba_is_a(k1,voba_cls_str) && voba_is_a(k2,voba_cls_str))){
                return false;
            }
            bool ret = voba_strcmp(voba_value_to_str(k1),voba_value_to_str(k2)) == 0;
            if(0) cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
                 << "ret "  << ret << " "
                 << "voba_str_to_cstr(voba_value_to_str(k1)) "  << voba_str_to_cstr(voba_value_to_str(k1)) << " "
                 << "voba_str_to_cstr(voba_value_to_str(k2)) "  << voba_str_to_cstr(voba_value_to_str(k2)) << " "
                 << endl;
            return ret;
        }
};
/// todo
template<typename T>
struct my_allocator : public std::allocator< T >{
    T * allocate(size_t s) {
        return static_cast<T*>(GC_MALLOC(s*sizeof(T)));
    }
    void deallocate(T * p,size_t s) {
        //return GC_FREE(static_cast<void*>(p));
    }
};
typedef voba::unordered_map<voba_value_t, voba_value_t, vtype_hasher, vtype_equal, my_allocator<pair<const voba_value_t, voba_value_t> > > hash_table_c;
#define HASH(r)  (VOBA_USER_DATA_AS(hash_table_c*,(r)))
voba_value_t voba_make_hash()
{
    voba_value_t r = voba_make_user_data(voba_cls_hashtable);
    ::new(HASH(r)) hash_table_c();
    return r;
}
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v)
{
    assert(h);
    hash_table_c::value_type* pair = HASH(h)->insert(make_pair(k,v));
    if(0) cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
         << hex
         << "h "  << h << " "
         << "k "  << k << " "
         << "v "  << v << " "
         << "pair "  << pair << " "
         << "pair->first "  << pair->first << " "
         << "pair->second "  << pair->second << " "
         << dec
         << endl;
    return voba_from_pointer(pair,VOBA_TYPE_PAIR);
}
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k)
{
    hash_table_c::iterator it = HASH(h)->find(k);
    if(0) cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
         << hex
         << "h "  << h << " "
         << "k "  << k << " "
         << dec
         << endl;
    if(it == HASH(h)->end()){
        return VOBA_NIL;
    }
    return voba_from_pointer(&(*it),VOBA_TYPE_PAIR);
}
extern "C" size_t voba_sizeof_hashtable()
{
    return sizeof(hash_table_c);
}
extern "C" size_t voba_hashtable_size(voba_value_t h)
{
    assert(voba_is_a(h,voba_cls_hashtable));
    return HASH(h)->size();
}
/// hash the symbol name
struct symbol_table_hasher {
    size_t operator() (const voba_value_t k1)
        {
            assert(voba_is_a(k1,voba_cls_symbol));
            return SuperFastHash(voba_value_to_str(voba_symbol_name(k1))->data,
                                 voba_value_to_str(voba_symbol_name(k1))->len);
        }
};
/// symbols with a same symbol name in a symbol table are same symbols
struct symbol_table_equal {
    bool operator() (const voba_value_t k1, const voba_value_t k2) const
        {
            assert(k1==0 || k1 == 1 || voba_is_a(k1,voba_cls_symbol));
            assert(k2==0 || k2 == 1 || voba_is_a(k2,voba_cls_symbol));
            if(k1 == k2){
                return true;
            }
            if(k1 == 0 || k1 == 1) return false;
            if(k2 == 0 || k2 == 1) return false;
            if(0) cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
                 << "voba_value_to_str(symbol_name(k1))->data "  << voba_value_to_str(voba_symbol_name(k1))->data << " "
                 << "voba_value_to_str(symbol_name(k2))->data "  << voba_value_to_str(voba_symbol_name(k2))->data << " "
                 << "k1 "  << hex << k1 << dec << " "
                 << endl;
            return voba_strcmp(voba_value_to_str(voba_symbol_name(k1)),
                               voba_value_to_str(voba_symbol_name(k2))) == 0;
        }
};
typedef voba::set<voba_value_t, symbol_table_hasher, symbol_table_equal, my_allocator<voba_value_t> > voba_symbol_table_c;
#define VOBA_SET(r)  (VOBA_USER_DATA_AS(voba_symbol_table_c*,(r)))
extern "C" voba_value_t voba_make_symbol_table()
{
    voba_value_t r = voba_make_user_data(voba_cls_symbol_table);
    ::new(VOBA_SET(r)) voba_symbol_table_c();
    return r;
}
extern "C" voba_value_t voba_make_symbol(voba_str_t * name, voba_value_t h)
{
    assert(voba_is_nil(h) || voba_is_a(h,voba_cls_symbol_table));
    // create an un-interned symbol, with a symbol value NIL
    voba_value_t v = voba__make_symbol_lowlevel(voba_make_string(voba_strdup(name)),VOBA_NIL);
    if(voba_is_nil(h)){
        h = voba_default_symbol_table;
    }
    voba_symbol_table_c::iterator it = VOBA_SET(h)->find(v);
    if(it != VOBA_SET(h)->end()){
        v = *it;
    }else{
        voba_value_t v2 = *(VOBA_SET(h)->insert(v));
        assert(v == v2);
        v = v2;
    }
    // a symbol is a pair, but with different type tag.
    return v;
}
extern "C" voba_value_t voba_intern_symbol(voba_value_t symbol, voba_value_t h)
{
    assert(voba_is_a(h,voba_cls_symbol_table));
    assert(voba_is_a(symbol,voba_cls_symbol));
    voba_value_t ret = VOBA_NIL;
    voba_symbol_table_c::iterator it = VOBA_SET(h)->find(symbol);
    if(it != VOBA_SET(h)->end()){
        voba_throw_exception(
            voba_make_string(
                voba_vstrcat(
                    VOBA_CONST_CHAR("intern duplicated symbol. symbol = 0x"),
                    voba_str_fmt_int64_t(symbol,16),
                    VOBA_CONST_CHAR(", symbol table = 0x"),
                    voba_str_fmt_int64_t(h,16),
                    NULL)));
    }else{
#ifndef NDEBUG
        voba_value_t v = *(VOBA_SET(h)->insert(symbol));
#else
        VOBA_SET(h)->insert(symbol);
#endif
        assert(v == symbol);
        ret = symbol;
    }
    return ret;
}
extern "C" voba_value_t voba_unintern_symbol(voba_value_t symbol, voba_value_t h)
{
    assert(voba_is_a(h,voba_cls_symbol_table));
    assert(voba_is_a(symbol,voba_cls_symbol));
    voba_value_t ret = VOBA_NIL;
    VOBA_SET(h)->erase(symbol);
    return ret;
}
extern "C" voba_value_t voba_lookup_symbol(voba_value_t s, voba_value_t h)
{
    assert(voba_is_a(h,voba_cls_symbol_table));
    assert(voba_is_a(s,voba_cls_str));
    voba_value_t ret = VOBA_NIL;
    voba_symbol_table_c::iterator it = VOBA_SET(h)->find(voba_make_symbol(voba_value_to_str(s),VOBA_NIL));
    if(it != VOBA_SET(h)->end()){
        ret = *it;
    }
    return ret;
}
extern "C" size_t voba_sizeof_symbol_table()
{
    return sizeof(voba_symbol_table_c);
}
extern "C" size_t voba_symbol_table_size(voba_value_t h)
{
    assert(voba_is_a(h,voba_cls_symbol_table));
    return VOBA_SET(h)->size();
}
extern "C" 
void voba_throw_exception(voba_value_t v)
{
    cerr << static_cast<const char*>(voba_value_to_str(v)->data)
         << endl;
    throw voba::exception(v);
}
extern "C" voba_value_t voba_try_catch(voba_value_t fun_body, voba_value_t fun_catch)
{
    voba_value_t args[] = {1,VOBA_NIL,VOBA_BOX_END};
    try{
        return voba_apply(fun_body,voba_make_tuple(args));
    }catch(voba::exception e){
        args[1] = e.value;
        return voba_apply(fun_catch,voba_make_tuple(args));
    }
}



#include "imp/hash_paul.inc"

