#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <cstdint>
#include <cstring>
#include <gc.h>
#include <vhash.h>
#include <exec_once.h>
#include "value.h"
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
    voba_value_t r = voba_make_user_data(voba_cls_hashtable, sizeof(hash_table_c));
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
    assert(voba_is_hashtable(h));
    return HASH(h)->size();
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
extern "C" voba_value_t voba_make_symbol_table()
{
    voba_value_t r = voba_make_user_data(voba_cls_symbol_table, sizeof(voba_symbol_table_c));
    ::new(VOBA_SET(r)) voba_symbol_table_c();
    return r;
}
extern "C" voba_value_t voba_make_symbol(voba_str_t * name, voba_value_t h)
{
    assert(voba_is_nil(h) || voba_is_symbol_table(h));
    // create an un-interned symbol, with a symbol value NIL
    voba_value_t v = voba__make_symbol_lowlevel(voba_make_string(voba_strdup(name)),VOBA_NIL);
    if(!voba_is_nil(h)){
        voba_symbol_table_c::iterator it = VOBA_SET(h)->find(v);
        if(it != VOBA_SET(h)->end()){
            v = *it;
        }else{
            voba_value_t v2 = *(VOBA_SET(h)->insert(v));
            assert(v == v2);
            v = v2;
        }
    }
    // a symbol is a pair, but with different type tag.
    return v;
}
extern "C" voba_value_t voba_intern_symbol(voba_value_t symbol, voba_value_t h)
{
    assert(voba_is_symbol_table(h));
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
        voba_value_t v = *(VOBA_SET(h)->insert(symbol));
        assert(v == symbol);
        ret = symbol;
    }
    return ret;
}
extern "C" voba_value_t voba_unintern_symbol(voba_value_t symbol, voba_value_t h)
{
    assert(voba_is_symbol_table(h));
    assert(voba_is_symbol(symbol));
    voba_value_t ret = VOBA_NIL;
    VOBA_SET(h)->erase(symbol);
    return ret;
}
extern "C" voba_value_t voba_lookup_symbol(voba_value_t s, voba_value_t h)
{
    assert(voba_is_symbol_table(h));
    assert(voba_is_symbol(s));
    voba_value_t ret = VOBA_NIL;
    voba_symbol_table_c::iterator it = VOBA_SET(h)->find(s);
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
    assert(voba_is_symbol_table(h));
    return VOBA_SET(h)->size();
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


// module related begin
static inline int is_file_readable(voba_str_t* path)
{
    const char * p = voba_str_to_cstr(path);
    struct stat buf;
    int r = stat(p,&buf);
    if(r != 0) {
        return 0;
    }
    if(! (S_ISREG(buf.st_mode))) {
        return 0;
    }
    FILE * fp = fopen(p,"r");
    if(fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}
static inline voba_value_t split(voba_str_t* s)
{
    voba_value_t ret = voba_make_array_0();
    uint32_t i  = 0, j = 0;
    s = voba_str_replace(s,':','\0');
    for(j = 0, i = 0; i < s->len + 1; ++i){
        if(s->data[i] == '\0'){
            uint32_t to = s->data[i-1] == '/'?i-1:i;
            voba_str_t * sj =
                voba_substr(s,j,to - j);
            j = i + 1;
            voba_array_push(ret,voba_make_string(sj));
        }
    }
    return ret;
}
voba_value_t voba_module_path()
{
    static voba_value_t voba_path = VOBA_NIL;
    if(!voba_is_nil(voba_path)){
        return voba_path;
    }
    const char * env = getenv("VOBA_PATH");
    if(env == NULL) {
        env = ".";
    }
    voba_path = split(voba_str_from_cstr(env));
    return voba_path;
}
static inline voba_str_t* find_file(voba_str_t * module_name, voba_str_t * pwd)
{
    voba_str_t * ret = NULL;
    module_name = voba_str_replace(module_name,'.','/');
    if(module_name->data[0] == '/'){
        ret = voba_strcat(voba_strdup(pwd),module_name);
        ret = voba_strcat_data(ret,".so",3);
        if(!is_file_readable(ret)){
            ret = NULL;
        }
    }else {
        voba_value_t voba_path = voba_module_path();
        int64_t len = voba_array_len(voba_path);
        voba_value_t * p = voba_array_base(voba_path);
        for(int64_t i = 0; i < len ; ++i){
            ret = voba_strdup(voba_value_to_str(p[i]));
            ret = voba_strcat_char(ret,'/');
            ret = voba_strcat(ret,module_name);
            ret = voba_strcat_data(ret,".so",3);
            if(is_file_readable(ret)){
                break;
            }else{
                ret = NULL;
            }
        }
    }
    if(ret != NULL){
        char * p = realpath(voba_str_to_cstr(ret), NULL);
        if(p == NULL){
            VOBA_THROW(
                VOBA_CONST_CHAR("cannot resolve realpath of "),
                ret);
        }
        ret = voba_strdup(voba_str_from_cstr(p));
        free(p);
    }
    return ret;
}
static inline
voba_value_t dir_and_base_name(voba_str_t* filename)
{
    // return a pair, with dir name and basename.
    uint32_t i = 0, j = 0;
    for(i = 0 ; i < filename->len; ++i){
        j = filename->len - i - 1;
        if( filename->data[j] == '/'){
            break;
        }
    }
    if(i == filename->len){
        return voba_make_pair(voba_make_string(voba_str_from_data(".",1)),
                              voba_make_string(filename));
    }
    return voba_make_pair(voba_make_string(voba_substr(filename,0,j)),
                          voba_make_string(voba_substr(filename,j+1, filename->len - j -1)));
}
static inline void voba__builtin_init_module(voba_value_t module,voba_value_t cwd,voba_value_t basename)
{
    voba_symbol_set_value(VOBA_SYMBOL("__dir__",module), cwd);
    voba_symbol_set_value(VOBA_SYMBOL("__file__",module), basename);
}
static voba_value_t module_cwd = VOBA_NIL;
EXEC_ONCE_DO(
    const size_t sz  = 64*1024;
    char * p = (char*)malloc(sz);
    if(!p) abort();
    char * cwd = getcwd(p,sz);
    if(!cwd) abort();
    module_cwd = voba_make_array_0();
    voba_array_push(module_cwd,voba_make_string(voba_strdup(voba_str_from_cstr(cwd))));
    free(p);
    );

extern "C" voba_value_t voba_load_module(const char * module_name,voba_value_t module)
{
    voba_value_t ret = VOBA_NIL;
    int64_t len = voba_array_len(module_cwd);
    assert(len > 0);
    voba_value_t cwd = voba_array_at(module_cwd,len-1);
    voba_str_t* os_file = find_file(voba_str_from_cstr(module_name),voba_value_to_str(cwd));
    cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
         << "module_name "  << module_name << " "
         << "cwd "  << hex << "0x" << cwd << dec << " "
         << "len "  << len << " "
         << "os_file "  << os_file << " "
         << endl;
    if(!os_file){
        VOBA_THROW(
            VOBA_CONST_CHAR("cannot find module so file."
                            " module_name = " ),
            voba_str_from_cstr(module_name)
            );
    }
    voba_value_t filename =  voba_make_string(os_file);
    voba_value_t dir_name_base_name = dir_and_base_name(voba_value_to_str(filename));
    voba_value_t dir_name = voba_head(dir_name_base_name);
    voba_value_t basename = voba_tail(dir_name_base_name);
    // push cwd
    void *handle;
    handle = dlopen(voba_str_to_cstr(voba_value_to_str(filename)), RTLD_LAZY);
    if(!handle){
        VOBA_THROW(VOBA_CONST_CHAR("dlopen failure:"),
                   voba_str_from_cstr(dlerror()));
    }
    dlerror();
    char *error = NULL;
    voba_value_t (*init)(voba_value_t);
    *(void **) (&init) = dlsym(handle, "voba_init");
    if ((error = dlerror()) != NULL)  {
        VOBA_THROW(VOBA_CONST_CHAR("dlsym(voba_init) failure: "),
                   voba_str_from_cstr(dlerror()));
    }
    try{
        cerr <<  __FILE__ << ":" << __LINE__ << ": [" << __FUNCTION__<< "] "
             << "dir_name "  << hex << "0x" << dir_name << dec << " "
             << endl;
        voba_array_push(module_cwd,dir_name);
        voba__builtin_init_module(module,cwd,basename);
        ret = init(module);
        voba_array_pop(module_cwd);
    }catch(voba::exception e) {
        voba_array_pop(module_cwd);
        throw(e);
    }
    return ret;
}
// module related end

#include "hash_paul.inc"

EXEC_ONCE_START
