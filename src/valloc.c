#include <stdlib.h>
#include "valloc.h"

#ifndef __V_ENABLE_GC__

/**
 * @brief Allocate memory
 * 
 * @param size The size of memory required in bytes
 * @return A pointer to the allocated memory
 */
vpointer v_alloc(vsize size)
{
    return malloc(size);
}

/**
 * @brief Free the object
 * 
 * @param pobj A pointer to the object
 */
void v_free(vpointer pobj)
{
    free(pobj);
}

#else // __V_ENABLE_GC__

#endif // !__V_ENABLE_GC__