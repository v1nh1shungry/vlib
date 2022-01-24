#ifndef __V_ITERATOR_H__
#define __V_ITERATOR_H__

#include "vtypes.h"

#define v_iterator_swap(type, lit, rit) v_iterator_swap_impl(lit, rit, sizeof(type))
#define v_iterator_get(type, iter) (*(type*)v_iterator_get_impl(iter))
#define v_iterator_set(type, iter, value) \
    do { \
        type __v_iterator_set_temp__ = value; \
        v_iterator_set_impl(iter, &__v_iterator_set_temp__, sizeof(type)); \
    } while (0)
#define c_array_begin(type, carray) c_array_begin_impl(carray, sizeof(type))
#define c_array_end(type, carray, count) c_array_end_impl(carray, count, sizeof(type))

struct _VIterator;
typedef struct _VIterator* VIterator;
typedef vpointer (*videref)(vpointer); // alias VIterator-DEREference-Function=viderf
typedef vpointer (*vistepf)(vpointer); // alias VIterator-STEP-Function=vistepf (step => one-step move)
typedef vpointer (*vimovef)(vpointer, vi32); // alias VIterator-MOVE_Function=vimovef
typedef enum { forward_iterator_tag, bidirectional_iterator_tag, random_access_iterator_tag } VIteratorTag;

/**
 * @brief Construct a forward iterator
 */
VIterator v_forward_iterator_new(vpointer data, videref dere_func, vistepf next_func);

/**
 * @brief Construct a bidirectional iterator
 */
VIterator v_bidirectional_iterator_new(vpointer data, videref dere_func, vistepf prev_func, vistepf next_func);

/**
 * @brief Construct a random-access iterator
 */
VIterator v_random_access_iterator_new(vpointer data, videref dere_func, vi32 step_size, vimovef move_func);

/**
 * @brief Destroy a VIterator (for all kinds of VIterator)
 */
void v_iterator_destroy(VIterator iterator);

/**
 * @brief Judge whether the two VIterators are equal
 */
vboolean v_iterator_equal(VIterator lhs, VIterator rhs);

/**
 * @brief Swap the two VIterators' values
 */
void v_iterator_swap_impl(VIterator lhs, VIterator rhs, vsize element_size);

/**
 * @brief The implementation of the macro `v_iterator_get`
 * 
 * @details Get the value the VIterator holds
 */
vpointer v_iterator_get_impl(VIterator iterator);

/**
 * @brief The implementation of the macro `v_iterator_set`
 * 
 * @details Set the value the VIterator holds
 */
void v_iterator_set_impl(VIterator iterator, vpointer pvalue, vsize value_size);

/**
 * @brief Move the VIterator to the previous
 */
void v_iterator_prev(VIterator iterator);

/**
 * @brief Move the VIterator to the next
 */
void v_iterator_next(VIterator iterator);

/**
 * @brief Move the VIterator
 */
void v_iterator_move(VIterator iterator, vi32 steps);

/**
 * @brief The wrapper for `v_iterator_destroy` to fit `vdtor` type
 */
void v_iterator_dtor(vpointer ptr);

/**
 * @brief The implementation of the macro `c_array_begin`
 * 
 * @details Construct a random-access iterator pointing to the first element in the C array
 */
VIterator c_array_begin_impl(vpointer carray, vsize element_size);

/**
 * @brief The implementation of the macro `c_array_end`
 * 
 * @details Construct a random-access iterator pointing to the next element of the last element in the C array
 */
VIterator c_array_end_impl(vpointer carray, vsize count, vsize element_size);

#endif // !__V_ITERATOR_H__