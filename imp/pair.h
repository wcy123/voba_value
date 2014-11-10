#pragma once
/* pair
   ====

+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |      +----------------+------------+
  `----> |        head(64bits)         |
         +----------------+------------+
         |        tail(64bits)         |
         +-----------------------------+
*/
extern voba_value_t voba_cls_pair;
#define voba_cons voba_make_pair
#define voba_car  voba_head
#define voba_cdr voba_tail
#define voba_setcar voba_set_head
#define voba_setcdr voba_setcdr
INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t);
INLINE voba_value_t  voba_head     (voba_value_t p);
INLINE voba_value_t  voba_tail     (voba_value_t p);
INLINE void          voba_set_head (voba_value_t p,voba_value_t v);
INLINE void          voba_set_tail (voba_value_t p,voba_value_t v);


