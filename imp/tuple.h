#pragma once
/** @file 
Tuple
=====

\a Tuple is a fixed size array. 


\verbatim
+------------------------------------------------+-------+
| data                                           | type1 |
+------------------------------------------------+-------+
| .    value (61 bits)                           |  010  |
+-|----------------------------------------------+-------+
  |
  |       +----------------+------------+
  `---->  |         size(64bits)        | 
          +----------------+------------+
          |         voba_value_t a1     |
          +-----------------------------+
          |         voba_value_t a2     |
          +-----------------------------+
          |            ....             |
          +-----------------------------+
\endverbatim

It is usually used for function call as below
@code{.c}
voba_value_t args[] = {2, arg1, arg2};
voba_value_t a_tuple = voba_make_tuple(args);
@endcode

*/
extern voba_value_t  voba_cls_tuple; /*!< @brief the class object associated with \a tuple*/
/** @brief make a tuple.  
   
   @param p the first element is not a ::voba_value_t, but a size of
   the array.
   
*/
INLINE voba_value_t  voba_make_tuple(voba_value_t* p);
/** @brief create a tuple with varadic arguments */
INLINE voba_value_t  voba_make_tuple_n(int64_t n,...);
/** @brief the length of a tuple */
INLINE int64_t       voba_tuple_len(voba_value_t v);
/** @brief the base address of a tuple.*/
INLINE voba_value_t* voba_tuple_base(voba_value_t v);
/** @brief The element at `i` for a tuple*/
INLINE voba_value_t  voba_tuple_at(voba_value_t v,int64_t i);
/** @brief set the element in a tuple*/
INLINE voba_value_t  voba_tuple_set(voba_value_t a,int64_t i,voba_value_t v);
/** @fn voba_make_tuple_0
    @brief a set of functions to create a tuple, voba_make_tuple_<N>, N = 0,1,2,...,20


@code{.c}
voba_value_t voba_make_tuple_0(voba_value_t a0);
voba_value_t voba_make_tuple_1(voba_value_t a0, voba_value_t a1);
....
voba_value_t voba_make_tuple_20(voba_value_t a0, voba_value_t a1,..., voba_value_t a19);
@endcode
*/

/** @brief allocate a number of memory for voba_value */
INLINE voba_value_t* voba_alloc(size_t n_of_elt);

// I really hate to write these macros, but I hate more duplicate
// code. I used to use python or M4 generate the following code, but
// it isn't more readable and it is not so good to depend on tools
// other than standard compilers.
#define DEFINE_VOBA_MAKE_TUPLE_N(n)                                     \
    INLINE voba_value_t                                                 \
    voba_make_tuple_##n (VOBA_FOR_EACH_N(n)(VOBA_MACRO_ARG, COMMA)) {   \
        voba_value_t * p = (voba_value_t*)                              \
            GC_MALLOC(sizeof(voba_value_t) * clz_long(n+1));            \
        p[0] =  (n);                                                    \
        VOBA_FOR_EACH_N(n)(DEFINE_VOBA_MAKE_TUPLE_ASSIGN,SEMI_COMMA);   \
        return voba_make_tuple(p);                                      \
    }
#define DEFINE_VOBA_MAKE_TUPLE_ASSIGN(n) p[n + 1] = a##n
VOBA_FOR_EACH(DEFINE_VOBA_MAKE_TUPLE_N,SPACE)

