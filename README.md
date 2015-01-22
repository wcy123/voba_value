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
  - string, see str.h
  - symbol, see symbol.h
  - tuple, see tuple.h
  - closure, see closure.h
  - pair, see pair.h
  - small types, see small.h
  - user defined, see usr.h
     - array, see array.h
     - hash, see hash.h
     - cls, see cls.h
     - generic function, see gf.h
     - symbol table, see symbol_table.h
     - list array view, la, see la.h
     - generator, generator.h
     - other user defined object
  - small types, see small.h


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
 

