#pragma once
#define COMMA  ,
#define SEMI_COMMA ;
#define SPACE
// for i in $(seq 0 20); do
//     echo -n "#define VOBA_FOR_EACH_${i}(XX,SEP) ";
//     for n in `seq 0 $((i-1))`; do
//         if [ x$n == x0 ]; then
//             echo -n XX\($n\);
//         else
//             echo -n ' ' SEP XX\($n\);
//         fi
//     done;
//     echo
// done
#define VOBA_FOR_EACH_0(XX,SEP) 
#define VOBA_FOR_EACH_1(XX,SEP) XX(0)
#define VOBA_FOR_EACH_2(XX,SEP) XX(0)  SEP XX(1)
#define VOBA_FOR_EACH_3(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)
#define VOBA_FOR_EACH_4(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)
#define VOBA_FOR_EACH_5(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)
#define VOBA_FOR_EACH_6(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)
#define VOBA_FOR_EACH_7(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)
#define VOBA_FOR_EACH_8(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)
#define VOBA_FOR_EACH_9(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)
#define VOBA_FOR_EACH_10(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)
#define VOBA_FOR_EACH_11(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)
#define VOBA_FOR_EACH_12(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)
#define VOBA_FOR_EACH_13(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)
#define VOBA_FOR_EACH_14(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)
#define VOBA_FOR_EACH_15(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)
#define VOBA_FOR_EACH_16(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)  SEP XX(15)
#define VOBA_FOR_EACH_17(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)  SEP XX(15)  SEP XX(16)
#define VOBA_FOR_EACH_18(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)  SEP XX(15)  SEP XX(16)  SEP XX(17)
#define VOBA_FOR_EACH_19(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)  SEP XX(15)  SEP XX(16)  SEP XX(17)  SEP XX(18)
#define VOBA_FOR_EACH_20(XX,SEP) XX(0)  SEP XX(1)  SEP XX(2)  SEP XX(3)  SEP XX(4)  SEP XX(5)  SEP XX(6)  SEP XX(7)  SEP XX(8)  SEP XX(9)  SEP XX(10)  SEP XX(11)  SEP XX(12)  SEP XX(13)  SEP XX(14)  SEP XX(15)  SEP XX(16)  SEP XX(17)  SEP XX(18)  SEP XX(19)

#define VOBA_FOR_EACH_N(n) VOBA_FOR_EACH_##n
#define VOBA_FOR_EACH_PASTE(x,y)   x##_##y
#define VOBA_FOR_EACH(XX,SEP) VOBA_FOR_EACH_PASTE(VOBA_FOR_EACH, 20)(XX,SEP)
    
#define VOBA_MACRO_ARG(n) voba_value_t a##n
#define VOBA_MACRO_ARG2(n) ,voba_value_t a##n

VOBA_VALUE_INLINE voba_value_t clz_long(uint32_t s) 
{
    int a = 0;
#if ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
    a = __builtin_clz(s);
#else
#error("TODO implement clzl")
#endif
    return (1l<<(32 - a));
}
