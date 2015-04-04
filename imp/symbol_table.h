#pragma once
/** @file
symbol table
============

\a symbol_table is a user defined class as below

@verbatim
+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  |     +------------------------------+--------------------+
  `->   |           voba_cls_hashtable  (64bits)            |
        +------------------------------+--------------------+
        |           symbol_table_c ...voba_symbol_table_size|
        +------------------------------+--------------------+
@endverbatim

When `symbol_table` is nil, ::voba_make_symbol return a uniqu symbol,
an un-interned symbol.

To intern a symbol, 

@code{.c}
voba_make_symbol(voba_symbol_name(sym), the_symbol_table);
@endcode

but it returns a new symbol. The old symbol `sym`
is always un-interned.

One symbol object could be interned into more than one symbol table.

@sa symbol.h


*/
// the following functions are implemented in voba_value.cc
/** @brief The class object for symbol tables. */
extern voba_value_t voba_cls_symbol_table;
/** @brief Create a symbol table*/
voba_value_t        voba_make_symbol_table();
/** @brief Return the number of symbols in a symbol table */
size_t              voba_symbol_table_size(voba_value_t h);
/** @brief Insert a symbol into a symbol table 

A symbol could be inserted into more than one symbol table. It is an
error if a symbol is already interned.

A symbol must have a unique symbol name in the symbol table. Two
symbols with a same symbol name cannot be interned into a same symbol
table.

*/
voba_value_t        voba_intern_symbol(voba_value_t symbol, voba_value_t h);
/** @brief Look up a symbl in a symbol table

    @bug two symbols with a same symbol name, say `s1` and `s2`, if
    `s1` in the symbol but `s2` doesn't, `voba_uninter_symbol(s2,h)`
    will remove `s1` by accident

    @todo intern symbol and unintern symbol should use string as a
    uniqe identifier? At least from API interface point of view.
 */
voba_value_t        voba_unintern_symbol(voba_value_t symbol, voba_value_t h);
/** @brief Look up a symbol in a symbol table, via symbol name
    @return The symbol in the symbol table if found, otherwise ::VOBA_NIL
 */
voba_value_t        voba_lookup_symbol(voba_value_t str, voba_value_t h);
/// internal function
size_t              voba_sizeof_symbol_table();


/** @brief the default symbol table.

    when create a symbol without a symbol table, the default symbol
    table is used.
 */
extern voba_value_t voba_default_symbol_table ;

/** @brief the symbol table for constant symbol whose symbol name
 * starts with a colon.
 */
extern voba_value_t voba_constant_symbol_table ;

