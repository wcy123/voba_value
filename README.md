# A value system

This is a rich value system to representing a lot of useful data
structure, e.g. list, array, hash etc.

This is the corner stone for a new language voba, but it could be used
independently without the language.

Basically, a value of `voba_value_t` is a 64-bit unsigned integer.

```c
typedef uint64_t voba_value_t;
```

The least significant 3 bits is used to represent the type info, so
that there are 8 basic types. Most of time, it is a pointer to 8-byte
aligned region, so that the least 3 significant bits could be used as
a type tag.

```c
#define    VOBA_TYPE_FUNC    0 
#define    VOBA_TYPE_STRING  1
#define    VOBA_TYPE_SYMBOL  2
#define    VOBA_TYPE_TUPLE   3
#define    VOBA_TYPE_CLOSURE 4
#define    VOBA_TYPE_PAIR    5
#define    VOBA_TYPE_USER    6
#define    VOBA_TYPE_SMALL   7
```

## function type.

It is a function pointer, `voba_func_t` as below

```
typedef voba_value_t (*voba_func_t)(voba_value_t fun, voba_value_t args);
```

### APIs
```
INLINE voba_value_t  voba_make_func(voba_func_t);
INLINE voba_func_t  voba_value_to_func(voba_value_t v);
```

## string 

```
+---------------+---------------------+
| .     61 bits value           | 001 |
+-|-------------+---------------------+
  |      voba_str_t, see voba_str for detail
  |      +----------------+------------+
  `----> |  const char * data          |
         +----------------+------------+
         | capcity(32bits)| len(32bit) |
         +-----------------------------+
```

string.

#### APIs

```
INLINE voba_value_t  voba_make_string(voba_str_t* s);
INLINE voba_value_t  voba_make_cstr(const char * s);
INLINE voba_str_t* voba_value_to_str(voba_value_t s);
```

### Symbol

```
+----------------------------+-------+
| data                       | type1 |
+----------------------------+-------+
| .  value (61 bits)         |  010  |
+-|--------------------------+-------+
  |
  `-->  +----------------------------+------------------------+
        |           symbol name ( a string )                  |
        +----------------------------+------------------------+
        |           symbol value (any voba value)             |
        +----------------------------+------------------------+
```

`Symbol` is a pointer which points to a pair, whose first value is the
symbol name and the second value is the symbol value.

Symbol is created in a symbol table, and a symbol table is a user
defined class as below.

