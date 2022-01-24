#ifndef __V_TYPE_H__
#define __V_TYPE_H__

#include <stdint.h>

#define vnull ((void*)0)

typedef char vchar;
typedef int8_t vi8;
typedef int32_t vi32;
typedef int64_t vi64;
typedef uint8_t vu8;
typedef uint32_t vu32;
typedef uint64_t vu64;
typedef float vfloat;
typedef double vdouble;
typedef enum {
    false = 0,
    true = 1
} vboolean;
typedef void* vpointer;
typedef const void* vconstpointer;
typedef vi32 vsize;
typedef void (*vdtor)(vpointer pobj);
typedef vboolean (*vcompar)(vconstpointer lhs, vconstpointer rhs);

#endif // !__V_TYPE_H__