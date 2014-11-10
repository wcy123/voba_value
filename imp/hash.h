#pragma once
/* hashtable
   =========

A hashtable is a user define class.

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

voba_hash_insert returns a pair if succesfull, NIL otherwise.

voba_hash_find returns a pair if successful, NIL otherwise.

head is the key and tail is the value. 


 */
extern voba_value_t voba_cls_hashtable;
voba_value_t voba_make_hash();// implemented in voba_value.cc
voba_value_t voba_hash_insert(voba_value_t h, voba_value_t k, voba_value_t v); // implemented in voba_value.cc
voba_value_t voba_hash_find(voba_value_t h, voba_value_t k);// implemented in voba_value.cc
size_t voba_sizeof_hashtable(); // implemented in voba_value.cc
size_t voba_hashtable_size(voba_value_t h); // implemented in voba_value.cc


