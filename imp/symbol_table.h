#pragma once
/*  symbol table
    ============
`symbol_table` is a user defined class as below

+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  |     +------------------------------+--------------------+
  `->   |           voba_cls_hashtable                      |
        +------------------------------+--------------------+
        |           symbol_table_c ...                      |
        +------------------------------+--------------------+

When`symbol_table is nil, `voba_make_symbol` return a uniqu symbol, an
un-interned symbol.

To intern a symbol, `voba_make_symbol(voba_symbol_name(sym),
the_symbol_table)`, but it returns a new symbol. The old symbol `sym`
is always un-interned.

the following functions are implemented in voba_value.cc
 */
extern voba_value_t voba_cls_symbol_table;
voba_value_t        voba_make_symbol_table();
size_t              voba_symbol_table_size(voba_value_t h);
voba_value_t        voba_intern_symbol(voba_value_t symbol, voba_value_t h);
voba_value_t        voba_unintern_symbol(voba_value_t symbol, voba_value_t h);
voba_value_t        voba_lookup_symbol(voba_value_t str, voba_value_t h);
size_t              voba_sizeof_symbol_table();
size_t              voba_symbol_table_size(voba_value_t h);

