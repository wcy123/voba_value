#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "voba_value.h"
#ifndef VERBOSE
#define VERBOSE 0
#endif

#define TEST(type)                                                      \
int test_##type(type x1)                                                \
{                                                                       \
    vtype_t y = make_i8(x1);                                            \
    if(VERBOSE)fprintf(stderr,__FILE__ ":%d:[%s] i8 %ld\n", __LINE__, __FUNCTION__, \
        y);                                                             \
    type x2 = vtype_to_i8(y);                                           \
    if(VERBOSE)fprintf(stderr,__FILE__ ":%d:[%s] i8 %d\n", __LINE__, __FUNCTION__, \
        x2);                                                            \
    assert(x1 == x2);                                                   \
    return 0;                                                           \
}
#define TESTS(XX)                                \
    XX(int8_t)                                   \
    XX(uint8_t)                                  \
    
TESTS(TEST)

int main(int argc, char *argv[])
{
    for(int i = 0 ; i < 256; i ++){
        test_int8_t((int8_t)i);
        test_uint8_t((uint8_t)i);
    }
}
