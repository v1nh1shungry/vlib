#ifndef __V_STACK_H__
#define __V_STACK_H__

#include "vtype.h"

struct _VStack;

typedef struct _VStack* VStack;

#define v_stack_new(type) v_stack_new_impl(sizeof(type), vnull)
#define v_stack_new_ext(type, dtor) v_stack_new_impl(sizeof(type), dtor)
#define v_stack_is_empty(stack) (v_stack_count_of(stack) == 0)
#define v_stack_top_of(type, stack) (*(type*)v_stack_top_of_impl(stack))
#define v_stack_push(type, stack, value) \
    do { \
        type __v_stack_push_temp__ = value; \
        v_stack_push_impl(stack, &__v_stack_push_temp__); \
    } while (0)

VStack v_stack_new_impl(vsize element_size, vdtor element_dtor);
void v_stack_destroy(VStack stack);
vsize v_stack_count_of(VStack stack);
vpointer v_stack_top_of_impl(VStack stack);
void v_stack_push_impl(VStack stack, vpointer pvalue);
void v_stack_pop(VStack stack);

#endif // !__V_STACK_H__