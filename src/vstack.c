#include "valloc.h"
#include "varray.h"
#include "vstack.h"

#define SRT_VSTK_SIZE (sizeof(struct _VStack))

struct _VStack
{
    VArray array_;
};

VStack v_stack_new_impl(vsize element_size, vdtor element_dtor)
{
#ifndef __V_ENABLE_GC__
    VStack stack = (VStack)v_alloc(SRT_VSTK_SIZE);
#else // __V_ENABLE_GC__
    VStack stack = (VStack)v_alloc_with_dtor(SRT_VSTK_SIZE, v_stack_dtor);
#endif // !__V_ENABLE_GC__

    if (stack == vnull) {
        return (VStack)vnull;
    }

    stack->array_ = v_array_new_impl(element_size, element_dtor);
    if (stack->array_ == vnull) {
        v_free(stack);
        return (VStack)vnull;
    }

    return stack;
}

void v_stack_destroy(VStack stack)
{
    if (stack == vnull) return;
    v_array_destroy(stack->array_);
    v_free(stack);
}

vsize v_stack_count(VStack stack)
{
    return v_array_count(stack->array_);
}

vpointer v_stack_top_impl(VStack stack)
{
    return v_array_get_impl(stack->array_, v_array_count(stack->array_) - 1);
}

void v_stack_push_impl(VStack stack, vpointer pvalue)
{
    v_array_insert_impl(stack->array_, v_array_count(stack->array_), pvalue);
}

void v_stack_pop(VStack stack)
{
    v_array_remove(stack->array_, v_array_count(stack->array_) - 1);
}

void v_stack_dtor(vpointer ptr)
{
    v_stack_destroy(*(VStack*)ptr);
}