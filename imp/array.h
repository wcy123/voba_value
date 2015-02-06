#pragma once
/** @file
Array
====

Unlike tuple(see tuple.h), \a array is variable size of a continuous memory.
see ::voba_array_s

Many APIs modify the first argument in place with side effect. It will
destory the first argument, so use the return value instead. for example
~~~{.c}
voba_value_t a = voba_array_from_tuple(tuple);
a = voba_array_push(a,xx);
~~~

*/
typedef struct voba_array_s voba_array_t;
/** 
  the main structure of an array
*/
struct voba_array_s {
    uint32_t capacity;
    uint32_t len;
    voba_value_t * data;
};
#define VOBA_ARRAY(s)  VOBA_USER_DATA_AS(voba_array_t *,s)
/** @brief The class object for `array` */
extern voba_value_t voba_cls_array;
/** @brief Low level function, it should not be used directly */
INLINE voba_value_t voba_make_array(uint32_t capacity, uint32_t len, voba_value_t* p);
/** @brief Create an array from a tuple
    @sa ::voba_cls_tuple
 */
INLINE voba_value_t voba_array_from_tuple(voba_value_t tuple);
/** @brief Create array from it arguments for C language
 */
INLINE voba_value_t voba_make_array_n(uint32_t n,...);
/** @brief Return the base address of an array */
INLINE voba_value_t * voba_array_base(voba_value_t v);
/** @brief Return the capacity of an array */
INLINE uint32_t voba_array_capacity(voba_value_t v);
/** @brief Return the length of an array */
INLINE uint32_t voba_array_len(voba_value_t v);
/** @brief Return the element in an array */
INLINE voba_value_t voba_array_at(voba_value_t v,uint32_t i);
/** @brief Set the element in an array */
INLINE voba_value_t  voba_array_set(voba_value_t a,uint32_t i,voba_value_t v);
/** @brief Append an element into the end of an array
    @return A new array

`a` might be destoyed

*/
INLINE voba_value_t  voba_array_push(voba_value_t a, voba_value_t v);
/** @brief Remove an element from the end of an array 
    @return A new array

`a` might be destoyed
*/
INLINE voba_value_t  voba_array_pop(voba_value_t a);
/** @brief Insert an element at the beginning of an array
    @return A new array

`a` might be destoyed
*/
INLINE voba_value_t  voba_array_shift(voba_value_t a, voba_value_t v);
/** @brief Remove an element from the beginning of an array
    @return A new array

`a` might be destoyed
*/
INLINE voba_value_t  voba_array_unshift(voba_value_t a);
/** @brief Concatenate two arrays
    @param a The first array
    @param b The second array

`b` is not touched, but `a` is potentially destroyed when capacity is enlarged. 

 */
INLINE voba_value_t  voba_array_concat(voba_value_t a, voba_value_t b);

