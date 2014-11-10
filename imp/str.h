#pragma once
/* string
   ======

+--------------------+------------------------+
| .     61 bits value                   | 100 |
+-|------------------+------------------------+
  |      voba_str_t, see voba_str for detail
  |      +----------------+------------+
  `----> |  const char * data          |
         +----------------+------------+
         | capcity(32bits)| len(32bit) |
         +-----------------------------+
 */
//
#include "voba_str.h"
extern voba_value_t voba_cls_str;
INLINE voba_value_t  voba_make_string(voba_str_t* s);
INLINE voba_value_t  voba_make_cstr(const char * s);
INLINE voba_str_t* voba_value_to_str(voba_value_t s);


