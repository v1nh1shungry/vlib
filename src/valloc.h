#ifndef __V_ALLOC_H__
#define __V_ALLOC_H__

#include "vtype.h"

vpointer v_malloc(vsize size);
vpointer v_calloc(vsize element_size, vsize count);
vpointer v_malloc_ext(vsize size, vdtor dtor);
vpointer v_malloc_static(vsize size);
void v_free(vpointer ptr);

#endif // !__V_ALLOC_H__