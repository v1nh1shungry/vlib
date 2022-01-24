#include <string.h>
#include "valloc.h"
#include "viterator.h"

#define SRT_V_FORW_ITER_SIZE (sizeof(struct _VForwardIterator))
#define SRT_V_BIDI_ITER_SIZE (sizeof(struct _VBidirectionalIterator))
#define SRT_V_RAND_ITER_SIZE (sizeof(struct _VRandomAccessIterator))

struct _VIterator
{
    vpointer data_;
    VIteratorTag tag_;
    videref dere_func_;
};

typedef struct _VForwardIterator
{
    struct _VIterator base_;
    vistepf next_func_;
} *VForwardIterator;

typedef struct _VBidirectionalIterator
{
    struct _VIterator base_;
    vistepf prev_func_;
    vistepf next_func_;
} *VBidirectionalIterator;

typedef struct _VRandomAccessIterator
{
    struct _VIterator base_;
    vi32 step_size_;
    vimovef move_func_;
} *VRandomAccessIterator;

/**
 * @brief Construct a base VIterator
 */
static VIterator v_iterator_new(vsize size, vpointer data, videref dere_func, VIteratorTag tag)
{
#ifndef __V_ENABLE_GC__
    VIterator iterator = (VIterator)v_alloc(size);
#else // __V_ENABLE_GC__
    VIterator iterator = (VIterator)v_alloc_with_dtor(size, v_iterator_dtor);
#endif // !__V_ENABLE_GC

    if (iterator == vnull) {
        // TODO: Log here
        return (VIterator)vnull;
    }

    iterator->data_ = data;
    iterator->dere_func_ = dere_func;
    iterator->tag_ = tag;

    return iterator;
}

VIterator v_forward_iterator_new(vpointer data, videref dere_func, vistepf next_func)
{
    VForwardIterator iterator = (VForwardIterator)v_iterator_new(SRT_V_FORW_ITER_SIZE, data, dere_func, forward_iterator_tag);
    if (iterator == vnull) return (VIterator)vnull;
    iterator->next_func_ = next_func;
    return (VIterator)iterator;
}

VIterator v_bidirectional_iterator_new(vpointer data, videref dere_func, vistepf prev_func, vistepf next_func)
{
    VBidirectionalIterator iterator = (VBidirectionalIterator)v_iterator_new(SRT_V_BIDI_ITER_SIZE, data, dere_func, bidirectional_iterator_tag);
    if (iterator == vnull) return (VIterator)vnull;
    iterator->prev_func_ = prev_func;
    iterator->next_func_ = next_func;
    return (VIterator)iterator;
}

VIterator v_random_access_iterator_new(vpointer data, videref dere_func, vsize step_size, vimovef move_func)
{
    VRandomAccessIterator iterator = (VRandomAccessIterator)v_iterator_new(SRT_V_RAND_ITER_SIZE, data, dere_func, random_access_iterator_tag);
    if (iterator == vnull) return (VIterator)vnull;
    iterator->move_func_ = move_func;
    iterator->step_size_ = step_size;
    return (VIterator)iterator;
}

void v_iterator_destroy(VIterator iterator)
{
    v_free(iterator);
}

vboolean v_iterator_equal(VIterator lhs, VIterator rhs)
{
    return lhs->data_ == rhs->data_;
}

void v_iterator_swap_impl(VIterator lhs, VIterator rhs, vsize element_size)
{
    vpointer buffer = v_alloc(element_size);
    if (buffer == vnull) {
        // TODO: Log here
        return;
    }
    memcpy(buffer, v_iterator_get_impl(lhs), element_size);
    memcpy(v_iterator_get_impl(lhs), v_iterator_get_impl(rhs), element_size);
    memcpy(v_iterator_get_impl(rhs), buffer, element_size);
    v_free(buffer);
}

vpointer v_iterator_get_impl(VIterator iterator)
{
    return iterator->dere_func_(iterator->data_);
}

void v_iterator_set_impl(VIterator iterator, vpointer pvalue, vsize value_size)
{
    memcpy(iterator->dere_func_(iterator->data_), pvalue, value_size);
}

void v_iterator_prev(VIterator iterator)
{
    switch (iterator->tag_)
    {
    case bidirectional_iterator_tag: 
        iterator->data_ = ((VBidirectionalIterator)iterator)->prev_func_(iterator->data_);
        break;
    case random_access_iterator_tag:
        iterator->data_ = ((VRandomAccessIterator)iterator)->move_func_(iterator->data_, -((VRandomAccessIterator)iterator)->step_size_);
        break;
    }
}

void v_iterator_next(VIterator iterator)
{
    switch (iterator->tag_)
    {
    case forward_iterator_tag:
        iterator->data_ = ((VForwardIterator)iterator)->next_func_(iterator->data_);
        break;
    case bidirectional_iterator_tag:
        iterator->data_ = ((VBidirectionalIterator)iterator)->next_func_(iterator->data_);
        break;
    case random_access_iterator_tag:
        iterator->data_ = ((VRandomAccessIterator)iterator)->move_func_(iterator->data_, ((VRandomAccessIterator)iterator)->step_size_);
        break;
    }
}

void v_iterator_move(VIterator iterator, vi32 steps)
{
    iterator->data_ = ((VRandomAccessIterator)iterator)->move_func_(iterator->data_, steps * ((VRandomAccessIterator)iterator)->step_size_);
}

void v_iterator_dtor(vpointer ptr)
{
    v_iterator_destroy(*(VIterator*)ptr);
}

static vpointer c_array_dere_func(vpointer ptr)
{
    return ptr;
}

static vpointer c_array_move_func(vpointer ptr, vi32 distance)
{
    return ptr + distance;
}

VIterator c_array_begin_impl(vpointer carray, vsize element_size)
{
    return v_random_access_iterator_new(carray, c_array_dere_func, element_size, c_array_move_func);
}

VIterator c_array_end_impl(vpointer carray, vsize count, vsize element_size)
{
    return v_random_access_iterator_new(carray + count * element_size, c_array_dere_func, element_size, c_array_move_func);
}