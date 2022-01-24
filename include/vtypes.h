#ifndef __V_TYPES_H__
#define __V_TYPES_H__

#include <stdint.h>

#define vnull ((void*)0)

typedef enum { false = 0, true = 1 } vboolean;
typedef int8_t vi8;
typedef int32_t vi32;
typedef int64_t vi64;
typedef uint8_t vu8;
typedef uint32_t vu32;
typedef uint64_t vu64;
typedef vi32 vsize;
typedef char vchar;
typedef void* vpointer;
typedef void(*vdtor)(vpointer);
typedef vi32(*vcmpf)(vpointer, vpointer);

#endif // !__V_TYPES_H__