#pragma once
/** @file 
hashtable
=========

A hashtable is a user define class.

@verbatim
+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  101  |
+-|----------------------------------------------+-------+
  |
  |     +------------------------------+--------------------+
  `->   |           voba_cls_hashtable                      |
        +------------------------------+--------------------+
        |           hash_table_c ...                        |
        +------------------------------+--------------------+
@endverbatim

::voba_hash_insert and ::voba_hash_find return a pair(see pair.h), DOT
NOT modify the head of the pair, otherwise, the integrity of the whole
hash table is destroyed.

most of them are implemented in c++, using vhash, see http://github.com/wcy123/vhash

*/
/** @brief The class object for hashtable.*/
extern voba_value_t voba_cls_hashtable;
/** @brief create a hash table */
voba_value_t voba_make_hash(); /// implemented in voba_value.cc @todo rename it to voba_make_hashtable
/** @brief insert an element into a hash table.
    @param h The hash table
    @param k The key 
    @param v The value
    @returns a pair if succesfull, ::VOBA_NIL otherwise.  The head is
    the key and the tail is the value.
*/
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v); // implemented in voba_value.cc
/** @brief look up a hash table 
    @param h The hash table
    @param k The key
    @returns a pair if successful, ::VOBA_NIL otherwise.
*/
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k);// implemented in voba_value.cc
/** private function */
size_t voba_sizeof_hashtable(); // implemented in voba_value.cc
/** @brief return the number of element in a hash table
 */
size_t voba_hashtable_size(voba_value_t h); // implemented in voba_value.cc


