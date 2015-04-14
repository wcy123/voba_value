#pragma once
/** @file
listarray
=========

In LISP, we often have the `push and reverse` idiom, `list array` is
the optimization (pre-mature, sorry) for it. Using `array` to
represent `list` is more space efficient.

interface:
 - `la_is_nil(LA)`
 - `la_car(LA)`
 - `la_cdr(LA)`

for example:
~~~{.c}
    voba_value_t x = voba_make_array_0();
    voba_value_t cur = voba_la_from_array0(x);
    while(!voba_la_is_nil(cur)){
        voba_value_t a = voba_la_car(cur);
        // do something with \a a
        cur = voba_la_cdr(cur);
    }
~~~

@todo, LA should be a view for both `::voba_cls_tuple` and
`::voba_cls_array`
@todo design pattern matching for `la`, `tuple` and `array`
*/
/** @brief The class object for `la` */
extern voba_value_t voba_cls_la;

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
INLINE voba_value_t voba_la_last(voba_value_t la);
INLINE int voba_la_is_nil(voba_value_t la);
INLINE voba_value_t voba_la_copy(voba_value_t la);
// copy array under la1 and la2, create a new array, and return the la
// associated with the new array
INLINE uint32_t     voba_la_len(voba_value_t la);
INLINE voba_value_t voba_la_to_array(voba_value_t la);


