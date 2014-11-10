#pragma once
/*   symbol
     ======

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  `-->  +----------------------------+------------------------+
        |           symbol name ( a string )                  |
        +----------------------------+------------------------+
        |           symbol value (any voba value)             |
        +----------------------------+------------------------+

`symbol` is similar to `pair`, but the address of the `pair` is
important. `voba_make_symbol` always returns the same address for the
same symbol name in a symbol table.
*/
extern voba_value_t voba_cls_symbol;
// implemented in voba_value.cc
voba_value_t voba_make_symbol(
    voba_str_t * symbol_name,
    voba_value_t symbol_table); 
INLINE voba_value_t voba_make_symbol_cstr(
    const char * symbol_name,
    voba_value_t symbol_table);
INLINE voba_value_t
voba_make_symbol_cstr_with_value(
    const char * symbol_name,
    voba_value_t symbol_table,
    voba_value_t v);
INLINE voba_value_t voba_make_symbol_data(
    const char * data,
    uint32_t len,
    voba_value_t symbol_table);
#define VOBA_SYMBOL(s,table) voba_make_symbol_data(#s,sizeof(#s) -1 , table)
INLINE voba_value_t voba_symbol_name(voba_value_t v);
INLINE voba_value_t voba_symbol_value(voba_value_t v);
INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v);
