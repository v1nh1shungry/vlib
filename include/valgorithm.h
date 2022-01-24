#ifndef __V_ALGORITHM_H__
#define __V_ALGORITHM_H__

#include "vtypes.h"
#include "viterator.h"

/**
 * @brief Sort the range [begin, end)
 */
void sort(VIterator begin, VIterator end, vcmpf compare_func);

#endif // !__V_ALGORITHM_H__