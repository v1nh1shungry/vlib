#ifndef __V_ALLOC_H__
#define __V_ALLOC_H__

#include "vtypes.h"

vpointer v_alloc(vsize size);
vpointer v_alloc_with_dtor(vsize size, vdtor dtor);
vpointer v_alloc_static(vsize size);
void v_free(vpointer pobj);

#endif // !__V_ALLOC_H__