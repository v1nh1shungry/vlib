#ifndef __V_LIST_H__
#define __V_LIST_H__

#include "vtypes.h"
#include "viterator.h"

struct _VList;
typedef struct _VList* VList;

#define v_list_new(type) v_list_new_impl(sizeof(type), vnull)
#define v_list_new_with_dtor(type, dtor) v_list_new_impl(sizeof(type), dtor)
#define v_list_empty(list) (v_list_count(list) == 0)
#define v_list_get(type, list, idx) (*(type*)v_list_get_impl(list, idx))
#define v_list_set(type, list, idx, value) \
    do { \
        type __v_list_set_temp__ = value; \
        v_list_set_impl(list, idx, &__v_list_set_temp__); \
    } while (0)
#define v_list_front(type, list) v_list_get(type, list, 0)
#define v_list_back(type, list) v_list_get(type, list, v_list_count(list)-1)
#define v_list_insert(type, list, idx, value) \
    do { \
        type __v_list_insert_temp__ = value; \
        v_list_insert_impl(list, idx, &__v_list_insert_temp__); \
    } while (0)
#define v_list_push_back(type, list, value) v_list_insert(type, list, v_list_count(list), value)
#define v_list_push_front(type, list, value) v_list_insert(type, list, 0, value)
#define v_list_pop_back(list) v_list_remove(list, v_list_count(list)-1)
#define v_list_pop_front(list) v_list_remove(list, 0)

/**
 * @brief The implementation of macro `v_list_new`
 * 
 * @details Construct an empty VList
 * 
 * @param element_size The size of each element in bytes
 * @param element_dtor The destructor of elements. When the VList is destroyed, it will 
 *                     apply this destructor to every element in the VList. Assign it to vnull
 *                     to disable it
 * @return A brand new VList or vnull if it runs out of memory
 */
VList v_list_new_impl(vsize element_size, vdtor element_dtor);

/**
 * @brief The copy constructor for VList
 * 
 * @return A new VList or vnull if it runs out of memory
 */
VList v_list_copy(VList list);

/**
 * @brief The destructor for VList
 * 
 * @details When this is called, it will apply the elements' destructor to every element in it if required
 */
void v_list_destroy(VList list);

/**
 * @brief Count of elements in the VList
 */
vsize v_list_count(VList list);

/**
 * @brief Return a VIterator pointing to the first element of the VList
 */
VIterator v_list_begin(VList list);

/**
 * @brief Return a VIterator pointing to the element next to the last element of the VList
 */
VIterator v_list_end(VList list);

/**
 * @brief The implementation of macro `v_list_get`
 */
vpointer v_list_get_impl(VList list, vsize index);

/**
 * @brief The implementation of macro `v_list_set`
 */
void v_list_set_impl(VList list, vsize index, vpointer pvalue);

/**
 * @brief The implementation of macro `v_list_insert`
 * 
 * @details Insert a new element into the VList
 */
void v_list_insert_impl(VList list, vsize index, vpointer pvalue);

/**
 * @brief Remove the certain element
 */
void v_list_remove(VList list, vsize index);

/**
 * @brief Clear the data stored in the VList
 */
void v_list_clear(VList list);

/**
 * @brief Swap the two VList
 */
void v_list_swap(VList lhs, VList rhs);

/**
 * @brief The assign operator for VList
 */
void v_list_assign(VList dst, VList src);

/**
 * @brief The wrapper of `v_list_destroy` to fit the `vdtor` type
 */
void v_list_dtor(vpointer ptr);

#endif // !__V_LIST_H__