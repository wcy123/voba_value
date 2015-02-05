#pragma once
/** @file */
/** \file 
string
======

\verbatim
+--------------------+------------------------+
| .     61 bits value                   | 001 |
+-|------------------+------------------------+
  |                           voba_str_t, see voba_str for detail   
  |                           +----------------+------------+       
  `------------------------>  |  const char * data          |       
                              +----------------+------------+       
                              | capcity(32bits)| len(32bit) |       
                              +-----------------------------+       
\endverbatim

A string object is a point to a ::voba_str_t object.

 */
//
#include "voba_str.h"
extern voba_value_t voba_cls_str; /*!< @brief the class object associated with string object*/
/** @brief create a string voba_str_t */
INLINE voba_value_t  voba_make_string(voba_str_t* s);
/** @brief create a string from a const C string*/
INLINE voba_value_t  voba_make_cstr(const char * s);
/** @brief convert a voba_value_t to a voba_str_t*/ 
INLINE voba_str_t* voba_value_to_str(voba_value_t s);


