#pragma once
/** @file 
pair
====

The famous \a pair, i.e. \a cons, a very simple and powerful data structure.

@verbatim
+--------------------+------------------------+
| .     61 bits value                   | 000 |
+-|------------------+------------------------+
  |      +----------------+------------+
  `----> |        head(64bits)         |
         +----------------+------------+
         |        tail(64bits)         |
         +-----------------------------+
@endverbatim

*/
extern voba_value_t voba_cls_pair; /*!< the class object associated with \a pair*/
#define voba_cons voba_make_pair
#define voba_car  voba_head
#define voba_cdr voba_tail
#define voba_setcar voba_set_head
#define voba_setcdr voba_setcdr
/** @brief cons */
VOBA_VALUE_INLINE voba_value_t  voba_make_pair(voba_value_t h,voba_value_t t);
/** @brief car */
VOBA_VALUE_INLINE voba_value_t  voba_head     (voba_value_t p);
/** @brief cdr */
VOBA_VALUE_INLINE voba_value_t  voba_tail     (voba_value_t p);
/** @brief setcar */
VOBA_VALUE_INLINE void          voba_set_head (voba_value_t p,voba_value_t v);
/** @brief setcdr */
VOBA_VALUE_INLINE void          voba_set_tail (voba_value_t p,voba_value_t v);


