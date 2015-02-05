Voba value system, the basic data structure
===========================================

see <http://wcy123.github.io/voba_value/index.html> for more details

This is a rich value system to representing a lot of useful data
structure, e.g. list, array, hash etc.

This is the corner stone for a new language voba, but it could be used
independently without the language.

Basically, a value of `voba_value_t` is a 64-bit unsigned integer.

~~~{.c}
typedef uint64_t voba_value_t;
~~~

The least significant 3 bits is used to represent the type info, so
that there are 8 basic types. Most of time, it is a pointer to 8-byte
aligned region, so that the least 3 significant bits could be used as
a type tag.

~~~{.c}
#define    VOBA_TYPE_FUNC    0 
#define    VOBA_TYPE_STRING  1
#define    VOBA_TYPE_SYMBOL  2
#define    VOBA_TYPE_TUPLE   3
#define    VOBA_TYPE_CLOSURE 4
#define    VOBA_TYPE_PAIR    5
#define    VOBA_TYPE_USER    6
#define    VOBA_TYPE_SMALL   7
~~~

There are 8 basic types:
  - function, see fun.h
       - `::voba_cls_func`
       - `::voba_make_func`
       - `::voba_value_to_func`
  - string, see str.h
       - `::voba_cls_str`
       - `::voba_make_string`
       - `::voba_make_cstr`
       - `::voba_value_to_str`
  - symbol, see symbol.h
      - `::voba_cls_symbol`
      - `::voba_make_symbol`
      - `::voba_make_symbol_cstr`
      - `::voba_make_symbol_cstr_with_value`
      - `::voba_symbol_name`
      - `::voba_symbol_value`
      - `::voba_symbol_set_value`
  - tuple, see tuple.h
      - `::voba_cls_tuple`
      - `::voba_make_tuple`
      - `::voba_make_tuple_n`
      - `::voba_tuple_len`
      - `::voba_tuple_base`
      - `::voba_tuple_at`
      - `::voba_tuple_set`
      - `::voba_make_tuple_0`
  - closure, see closure.h
      - `::voba_cls_closure`
      - `::voba_closure_at`
      - `::voba_closure_func`
      - `::voba_closure_tuple`
      - `::voba_closure_len`
  - pair, see pair.h
      - `::voba_cls_pair`
      - `::voba_make_pair`
      - `::voba_head`
      - `::voba_tail`
      - `::voba_set_head`
      - `::voba_set_tail`
  - small types, see small.h
      - `::voba_cls_nil`
          - `::VOBA_NIL`
          - `::voba_is_nil`
      - `::voba_cls_undef`
          - `::VOBA_UNDEF`
          - `::voba_is_undef`
      - `::voba_cls_done`
          - `::VOBA_DONE`
          - `::voba_is_done`
      - `::voba_cls_bool`
          - `::VOBA_TRUE`
          - `::VOBA_FALSE`
          - `::voba_is_true`
          - `::voba_is_false`
          - `::voba_not`
      - `::voba_cls_short_symbol`
      - integer types
          - `::voba_cls_u8`
          - `::voba_cls_i8`
          - `::voba_cls_u16`
          - `::voba_cls_i16`
          - `::voba_cls_u32`
          - `::voba_cls_i32`
      - `::voba_cls_float`
  - user defined, see usr.h
     - array, see array.h
     - hash, see hash.h
     - cls, see cls.h
     - generic function, see gf.h
     - symbol table, see symbol_table.h
     - list array view, la, see la.h
     - generator, generator.h
     - other user defined object
  - `::voba_apply`, see apply.h
  - predefined generic functions
      - `::voba_gf_apply` 
      - `::voba_gf_iter`
      - `::voba_gf_match`
  

### dependency

<pre>
@dot
digraph example {
node [shape=record, fontname=Helvetica, fontsize=10];
 voba_value [ label="voba_value" URL="https://wcy123.github.io/voba_value"];
 exec_once [ label="exec_once" URL="https://wcy123.github.io/exec_once"];
 voba_str [ label="voba_str" URL="https://wcy123.github.io/voba_str"];
 vhash [ label="vhash" URL="https://wcy123.github.io/vhash"];
 voba_value -> exec_once [ arrowhead="open", style="dashed" ];
 voba_value -> voba_str [ arrowhead="open", style="dashed" ];
 voba_value -> vhash [ arrowhead="open", style="dashed" ];
 
}
@enddot
</pre>
 

