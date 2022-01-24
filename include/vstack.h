#ifndef __V_STACK_H__
#define __V_STACK_H__

#include "vtypes.h"

struct _VStack;
typedef struct _VStack* VStack;

#define v_stack_new(type) v_stack_new_impl(sizeof(type), vnull)
#define v_stack_new_with_dtor(type, dtor) v_stack_new_impl(sizeof(type), dtor)
#define v_stack_empty(stk) (v_stack_count(stk) == 0)
#define v_stack_top(type, stk) (*(type*)v_stack_top_impl(stk))
#define v_stack_push(type, stk, value) \
    do { \
        type __v_stack_push_temp__ = value; \
        v_stack_push_impl(stk, &__v_stack_push_temp__); \
    } while (0)

/**
 * @brief The implementation of the macro `v_stack_new` and `v_stack_new_with_dtor`
 * 
 * @details Construct an empty VStack
 * @return An empty VStack or vnull if it runs out of memory
 */
VStack v_stack_new_impl(vsize element_size, vdtor element_dtor);

/**
 * @brief The destructor of VStack
 */
void v_stack_destroy(VStack stack);

/**
 * @brief The count of elements in the VStack
 */
vsize v_stack_count(VStack stack);

/**
 * @brief The implementation of the macro `v_stack_top`
 * 
 * @details The top element of the VStack
 */
vpointer v_stack_top_impl(VStack stack);

/**
 * @brief The implementation of the macro `v_stack_push`
 * 
 * @details Push a new element on the top of the VStack
 */
void v_stack_push_impl(VStack stack, vpointer pvalue);

/**
 * @brief Pop out the top element of the VStack
 */
void v_stack_pop(VStack stack);

/**
 * @brief The wrapper of `v_stack_destroy` to fit `vdtor` type
 */
void v_stack_dtor(vpointer ptr);

#endif // !__V_STACK_H__