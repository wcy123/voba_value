#include <vhash.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cstdint>
#include <cstring>
#include <gc.h>
#include "voba_value.h"
using namespace std;
static uint32_t SuperFastHash (const char * data, int len);
struct vtype_hasher {
    size_t operator() (const voba_value_t key)
        {
            if(!voba_is_string(key)){
                return (key>>3);
            }else{
                voba_str_t * s = voba_value_to_str(key);
                return SuperFastHash(s->data,s->len);
            }
        }
};
struct vtype_equal {
    bool operator() (const voba_value_t k1, const voba_value_t k2) const
        {
            if(k1 == k2){
                return true;
            }
            if(!(voba_is_string(k1) && voba_is_string(k2))){
                return false;
            }
            return voba_strcmp(voba_value_to_str(k1),voba_value_to_str(k2)) == 0;
        }
};
template<typename T>
struct my_allocator : public std::allocator< T >{
    T * allocate(size_t s) {
        return static_cast<T*>(GC_MALLOC(s*sizeof(T)));
    }
    void deallocate(T * p,size_t s) {
        return GC_FREE(static_cast<void*>(p));
    }
};
typedef voba::unordered_map<voba_value_t, voba_value_t, vtype_hasher, vtype_equal, my_allocator<pair<const voba_value_t, voba_value_t> > > hash_table_c;
#define HASH(r)  (VOBA_USER_DATA_AS(hash_table_c*,(r)))
voba_value_t voba_make_hash()
{
    voba_value_t r = voba_make_user_data(VOBA_NIL, sizeof(hash_table_c));
    ::new(HASH(r)) hash_table_c();
    return r;
}
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v)
{
    hash_table_c::value_type* pair = HASH(h)->insert(make_pair(k,v));
    return voba_from_pointer(pair,VOBA_TYPE_PAIR);
}
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k)
{
    hash_table_c::iterator it = HASH(h)->find(k);
    if(it == HASH(h)->end()){
        return VOBA_NIL;
    }
    return voba_from_pointer(&(*it),VOBA_TYPE_PAIR);
}

struct symbol_table_hasher {
    size_t operator() (const voba_value_t k1)
        {
            assert(voba_is_symbol(k1));
            return SuperFastHash(voba_value_to_str(voba_symbol_name(k1))->data,
                                 voba_value_to_str(voba_symbol_name(k1))->len);
        }
};
struct symbol_table_equal {
    bool operator() (const voba_value_t k1, const voba_value_t k2) const
        {
            assert(k1==0 || k1 == 1 || voba_is_symbol(k1));
            assert(k2==0 || k2 == 1 || voba_is_symbol(k2));
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
extern "C" voba_value_t voba_make_symbol_table_cpp()
{
    voba_value_t r = voba_make_user_data(VOBA_NIL, sizeof(voba_symbol_table_c));
    ::new(VOBA_SET(r)) voba_symbol_table_c();
    return r;
}
extern "C" voba_value_t voba_make_symbol_cpp(voba_str_t * name, voba_value_t h)
{
    // create an un-interned symbol, with a symbol value NIL
    voba_value_t v = voba_make_symbol_internal(voba_make_string(voba_strdup(name)),VOBA_NIL);
    if(!voba_is_nil(h)){
        voba_symbol_table_c::iterator it = VOBA_SET(h)->find(v);
        if(it != VOBA_SET(h)->end()){
            v = *it;
        }else{
            v = *(VOBA_SET(h)->insert(v));
        }
    }
    // a symbol is a pair, but with different type tag.
    return v;
}

namespace voba {
  class exception : std::exception{
  public:
    explicit exception(voba_value_t v):value(v){}
    virtual const char* what() const throw();
    voba_value_t value;
  };
  const char* exception::what() const throw()
  {
    static char buf[128];
    snprintf(buf,128,"exception %ld(0x%lx)",value,value);
    return buf;
  }
}
extern "C" 
void voba_throw_exception(voba_value_t v)
{
    cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
         << static_cast<const char*>(voba_value_to_str(v)->data)
         << endl;
    throw voba::exception(v);
}

#include "hash_paul.inc"

