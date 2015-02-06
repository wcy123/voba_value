#pragma once
/** @file
generator
=========

see <https://github.com/wcy123/cg> for more details.
\sa ::voba_make_generator
*/
#include "cg.h"
#define VOBA_GENERATOR(s) VOBA_USER_DATA_AS(cg_t *,s)
/** @brief The class object for generators. */
extern voba_value_t voba_cls_generator;
/** @brief Create a generator

 @return a callable object which capture a function pointer \a f, \a self and \a args.

When the object is callled via \a voba_apply, \a cg_invoke is used to
start the generator, i.e. start f(self,args). In the function \a f, \a
cg_yield could be invoked to suspend the generator.

@todo explain generator in details

*/
INLINE voba_value_t voba_make_generator(voba_func_t f, voba_value_t self, voba_value_t args);
