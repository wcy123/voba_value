#pragma once
/** @file

symbol
======

\verbatim
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
\endverbatim

\a symbol is similar to \a pair. A interned \a symbol is inserted into
a symbol table. When looking up a symbol with a same name, we always
get the same symbol object. One symbol could be interned into more
than one symbol table.

see symbol_table.h
*/

/** @brief The class object for symbol */
extern voba_value_t voba_cls_symbol;

/** @brief create a symbol
 *  
 *  @param symbol_name the name of the symbol
 *  @param symbol_table the symbol table object, \a symbol_table is
 *  ::VOBA_NIL for a un-interned symbol. see symbol_table.h
 */
// implemented in voba_value.cc
/** @brief create a symbol 
    
    @param symbol_name Symbol name
    @param symbol_table a symbol table, see symbol_table.h, VOBA_NIL to create an un-interned symbol
    @return a new symbol
 */
voba_value_t voba_make_symbol(
    voba_str_t * symbol_name,
    voba_value_t symbol_table);
/** @brief create a symbol */
VOBA_VALUE_INLINE voba_value_t voba_make_symbol_cstr(
    const char * symbol_name,
    voba_value_t symbol_table);
/** @brief create a symbol*/
VOBA_VALUE_INLINE voba_value_t
voba_make_symbol_cstr_with_value(
    const char * symbol_name,
    voba_value_t symbol_table,
    voba_value_t v);
#define VOBA_SYMBOL(s,table) voba_make_symbol_data(#s,sizeof(#s) -1 , table)
/** @brief access symbol name*/
VOBA_VALUE_INLINE voba_value_t voba_symbol_name(voba_value_t v);
/** @brief access symbol value*/
VOBA_VALUE_INLINE voba_value_t voba_symbol_value(voba_value_t v);
/** @brief set the symbol value */
VOBA_VALUE_INLINE voba_value_t voba_symbol_set_value(voba_value_t s,voba_value_t v);


/** 
 @todo support for keyword symbol
 @todo create a symbol table object for keyword symbol
*/
