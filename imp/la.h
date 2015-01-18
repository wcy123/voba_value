#pragma once
/* listarray
   =========

   In LISP, we often have the `push and reverse` idiom, `list array`
   is the optimization (pre-mature, sorry) for it. Using `array` to
   represent `list` is more space efficient.

interface:

    - VOBA_NIL is the empty `listarray`
    - `la_car(LA)`
    - `la_cdr(LA)`

internal implementation

    for non-empty `listarray`;

+--------------------+------------------------+
| .     61 bits value                   | 100 |
+-|------------------+------------------------+          voba_cls_la
  |    +----------------+------------+          +----------------+------------+
  `->  |  voba_class_t  *        .---|--------> |       size_t size           |
       +--------------+--------------+          +----------------+------------+
       |uint32_t cur  | uint32_t end |          |  const char * name          |
       +-----------------------------+          +-----------------------------+
       | voba_value_t array          |           
       +-----------------------------+  

for efficiency purpose, `la_cdr` modify `la` in-place,
and returns `la`. to save `la`, use `la_copy`.

*/
extern voba_value_t voba_cls_la;
/// to be done
typedef struct voba_la_s {
    uint32_t cur;
    uint32_t end;
    voba_value_t array;
} voba_la_t;
#define VOBA_LA(s) VOBA_USER_DATA_AS(voba_la_t *,s)
INLINE voba_value_t voba_la_from_array0(voba_value_t array);
INLINE voba_value_t voba_la_from_array1(voba_value_t array, uint32_t cur);
INLINE voba_value_t voba_la_from_array2(voba_value_t array, uint32_t cur,uint32_t len);
INLINE voba_value_t voba_la_car(voba_value_t la);
INLINE voba_value_t voba_la_cdr(voba_value_t la);
INLINE voba_value_t voba_la_nil();
INLINE int voba_la_is_nil(voba_value_t la);
INLINE voba_value_t voba_la_copy(voba_value_t la);
// copy array under la1 and la2, create a new array, and return the la
// associated with the new array
INLINE uint32_t     voba_la_len(voba_value_t la);
INLINE voba_value_t voba_la_to_array(voba_value_t la);


