#pragma once
#include "cg.h"
#define VOBA_GENERATOR(s) VOBA_USER_DATA_AS(cg_t *,s)
extern voba_value_t voba_cls_generator;
INLINE voba_value_t voba_make_generator(voba_func_t f, voba_value_t self, voba_value_t args);
