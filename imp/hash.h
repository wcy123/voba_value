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
extern voba_value_t voba_cls_hashtable;
/** create a hash table */
voba_value_t voba_make_hash();// implemented in voba_value.cc
/** @returns a pair if succesfull, NIL otherwise. 
   head is the key and tail is the value. */
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v); // implemented in voba_value.cc
/** @returns a pair if successful, NIL otherwise. */
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k);// implemented in voba_value.cc
size_t voba_sizeof_hashtable(); // implemented in voba_value.cc
size_t voba_hashtable_size(voba_value_t h); // implemented in voba_value.cc


