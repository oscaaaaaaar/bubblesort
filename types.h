#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

typedef size_t usize;

typedef uint8_t byte;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef  int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

#if CHAR_BIT != 8
#error "Expects 8 bits per byte"
#endif

#if __SIZEOF_FLOAT__  == 4 && \
    __SIZEOF_DOUBLE__ == 8

typedef float    f32;
typedef double   f64;

#else
#error "Floating point types are a different size than expected, edit this file to fix"
#endif //Floating point types

#if __SIZEOF_LONG_DOUBLE__ == 16
#warning "Type f128 has become available"
typedef long double f128;
#else
//#warning "Floating point 128-bit type is not available"
#endif

//No padding between member values
#define PACKED_STRUCT __attribute__((gcc_struct)) __attribute__((packed))

#endif //TYPES_H