#ifndef __V_ARRAY_H__
#define __V_ARRAY_H__

#include "vtypes.h"
#include "viterator.h"

#define v_array_new(type) v_array_new_impl(sizeof(type), vnull)
#define v_array_new_with_dtor(type, dtor) v_array_new_impl(sizeof(type), dtor)
#define v_array_sized_new(type, count) v_array_sized_new_impl(count, sizeof(type), vnull)
#define v_array_sized_new_with_dtor(type, count, dtor) v_array_sized_new_impl(count, sizeof(type), dtor)
#define v_array_from_c_array(type, carray, count) v_array_from_c_array_impl(carray, count, sizeof(type), vnull)
#define v_array_from_c_array_with_dtor(type, carray, count, dtor) v_array_from_c_array_impl(carray, count, sizeof(type), dtor)
#define v_array_empty(array) (v_array_count(array) == 0)
#define v_array_get(type, array, idx) (*(type*)v_array_get_impl(array, idx))
#define v_array_set(type, array, idx, value) \
    do { \
        type __v_array_set_temp__ = value; \
        v_array_set_impl(array, idx, &__v_array_set_temp__); \
    } while (0)
#define v_array_front(type, array) v_array_get(type, array, 0)
#define v_array_back(type, array) v_array_get(type, array, v_array_count(array)-1)
#define v_array_insert(type, array, idx, value) \
    do { \
        type __v_array_insert_temp__ = value; \
        v_array_insert_impl(array, idx, &__v_array_insert_temp__); \
    } while (0)
#define v_array_push_front(type, array, value) v_array_insert(type, array, 0, value)
#define v_array_push_back(type, array, value) v_array_insert(type, array, v_array_count(array), value)
#define v_array_pop_front(array) v_array_remove(array, 0)
#define v_array_pop_back(array) v_array_remove(array, v_array_count(array)-1)

struct _VArray;
typedef struct _VArray* VArray;

/**
 * @brief The implementation of the macro `v_array_new` and `v_array_new_with_dtor`
 * 
 * @details It creates an empty VArray
 * 
 * @param element_size The size of each element in bytes
 * @param element_dtor The destructor for each element. When the VArray is destroyed,
 *                     it will apply the destructor to every element. Assign this to vnull
 *                     to disable
 * @return An empty VArray or vnull if it runs out of memory
 */
VArray v_array_new_impl(vsize element_size, vdtor element_dtor);

/**
 * @brief The implementation of the macro `v_array_sized_new` and `v_array_sized_new_with_dtor`
 * 
 * @details It creates a VArray with $count elements allocated
 * 
 * @param count The count of elements required to allocate
 * @param element_size The size of each element in bytes
 * @param element_dtor The destructor for each element. When the VArray is destroyed,
 *                     it will apply the destructor to every element. Assign this to vnull
 *                     to disable
 * @return A VArray with $count elements or vnull if it runs out of memory
 */
VArray v_array_sized_new_impl(vsize count, vsize element_size, vdtor element_dtor);

/**
 * @brief The copy constructor for VArray
 * 
 * @param array The VArray to copy
 * @return A VArray or vnull if it runs out of memory
 */
VArray v_array_copy(VArray array);

/**
 * @brief Construct a VArray with a classical C array
 * 
 * @param carray The C array to initialize the VArray
 * @param count The count of the elements
 * @param element_size The size of each element in bytes
 * @param element_dtor The destructor for each element. When the VArray is destroyed,
 *                     it will apply the destructor to every element. Assign this to vnull
 *                     to disable
 * @return A VArray with datas same as the C array or vnull when it runs out of memory or $carray == vnull
 */
VArray v_array_from_c_array_impl(vpointer carray, vsize count, vsize element_size, vdtor element_dtor);

/**
 * @brief The destructor of VArray
 * 
 * @details When this is called, it will apply the element's destructor to each element if required
 * 
 * @param array The array to destroy
 */
void v_array_destroy(VArray array);

/**
 * @brief Return a VIterator pointing to the first element in the VArray
 */
VIterator v_array_begin(VArray array);

/**
 * @brief Return a VIterator pointing to the element next to the last element in the VArray
 */
VIterator v_array_end(VArray array);

/**
 * @brief Count of elements in the VArray
 */
vsize v_array_count(VArray array);

/**
 * @brief Capacity of the VArray
 */
vsize v_array_capacity(VArray array);

/**
 * @brief The assign operator for VArray
 * 
 * @attention Although it doesn't stop you from assigning a VArray<T> to a VArray<U>,
 *            it is very unrecommended and dangerous
 */
void v_array_assign(VArray dst, VArray src);

/**
 * @brief Assign a classical C array to a VArray
 * 
 * @attention It assumes that the size of elements of the C array is same as the VArray
 */
void v_array_assign_c_array(VArray dst, vpointer src, vsize count);

/**
 * @brief The implementation of the macro `v_array_get`
 * 
 * @details Return the ($index)th element of the VArray
 * 
 * @return vpointer A pointer to the ($index)th element
 */
vpointer v_array_get_impl(VArray array, vsize index);

/**
 * @brief The implementation of the macro `v_array_set`
 * 
 * @details Set the ($index)th element of the VArray to the value in $pvalue
 */
void v_array_set_impl(VArray array, vsize index, vpointer pvalue);

/**
 * @brief The implementation of the macro `v_array_insert`
 * 
 * @details Insert an new element into the index place with value same as what $pvalue points to
 */
void v_array_insert_impl(VArray array, vsize index, vpointer pvalue);

/**
 * @brief Remove the element
 */
void v_array_remove(VArray array, vsize index);

/**
 * @brief Swap the two VArrays
 */
void v_array_swap(VArray lhs, VArray rhs);

/**
 * @brief Clear the data in the VArray
 */
void v_array_clear(VArray array);

/**
 * @brief Change the capacity of the VArray
 */
void v_array_reserve(VArray array, vsize size);

/**
 * @brief Reduce the capacity of the VArray to fit the count to save the memory
 */
void v_array_shrink(VArray array);

/**
 * @brief A wrapper for function `v_array_destroy` to fit the `vdtor` type
 */
void v_array_dtor(vpointer ptr);

#endif // !__V_ARRAY_H__