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
    size_t operator() (const vtype_t key)
        {
            if(!is_string(key)){
                return (key>>3);
            }else{
                voba_str_t * s = vtype_to_str(key);
                return SuperFastHash(s->data,s->len);
            }
        }
};
struct vtype_equal {
    bool operator() (const vtype_t k1, const vtype_t k2) const
        {
            if(k1 == k2){
                return true;
            }
            if(!(is_string(k1) && is_string(k2))){
                return false;
            }
            return voba_strcmp(vtype_to_str(k1),vtype_to_str(k2)) == 0;
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
typedef voba::unordered_map<vtype_t, vtype_t, vtype_hasher, vtype_equal, my_allocator<pair<const vtype_t, vtype_t> > > hash_table_c;
#define HASH(r)  (USER_DATA_AS(hash_table_c*,(r)))
vtype_t make_hash()
{
    vtype_t r = make_user_data(NIL, sizeof(hash_table_c));
    ::new(HASH(r)) hash_table_c();
    return r;
}
vtype_t hash_insert(vtype_t h, vtype_t k, vtype_t v)
{
    hash_table_c::value_type* pair = HASH(h)->insert(make_pair(k,v));
    return from_pointer(pair,V_PAIR);
}
vtype_t hash_find(vtype_t h, vtype_t k)
{
    hash_table_c::iterator it = HASH(h)->find(k);
    if(it == HASH(h)->end()){
        return NIL;
    }
    return from_pointer(&(*it),V_PAIR);
}

struct symbol_table_hasher {
    size_t operator() (const vtype_t k1)
        {
            assert(is_symbol(k1));
            return SuperFastHash(vtype_to_str(symbol_name(k1))->data,
                                 vtype_to_str(symbol_name(k1))->len);
        }
};
struct symbol_table_equal {
    bool operator() (const vtype_t k1, const vtype_t k2) const
        {
            assert(k1==0 || k1 == 1 || is_symbol(k1));
            assert(k2==0 || k2 == 1 || is_symbol(k2));
            if(k1 == k2){
                return true;
            }
            if(k1 == 0 || k1 == 1) return false;
            if(k2 == 0 || k2 == 1) return false;
            if(0) cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
                 << "vtype_to_str(symbol_name(k1))->data "  << vtype_to_str(symbol_name(k1))->data << " "
                 << "vtype_to_str(symbol_name(k2))->data "  << vtype_to_str(symbol_name(k2))->data << " "
                 << "k1 "  << hex << k1 << dec << " "
                 << endl;
            return voba_strcmp(vtype_to_str(symbol_name(k1)),
                               vtype_to_str(symbol_name(k2))) == 0;
        }
};
typedef voba::set<vtype_t, symbol_table_hasher, symbol_table_equal, my_allocator<vtype_t> > voba_symbol_table_c;
#define VOBA_SET(r)  (USER_DATA_AS(voba_symbol_table_c*,(r)))
extern "C" vtype_t make_symbol_table_cpp()
{
    vtype_t r = make_user_data(NIL, sizeof(voba_symbol_table_c));
    ::new(VOBA_SET(r)) voba_symbol_table_c();
    return r;
}
extern "C" vtype_t make_symbol_cpp(voba_str_t * name, vtype_t h)
{
    // create an un-interned symbol, with a symbol value NIL
    vtype_t v = make_symbol_internal(make_string(voba_strdup(name)),NIL);
    if(!is_nil(h)){
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
    explicit exception(vtype_t v):value(v){}
    virtual const char* what() const throw();
    vtype_t value;
  };
  const char* exception::what() const throw()
  {
    static char buf[128];
    snprintf(buf,128,"exception %ld(0x%lx)",value,value);
    return buf;
  }
}
extern "C" 
void throw_exception(vtype_t v)
{
  throw voba::exception(v);
}

#include "hash_paul.inc"

