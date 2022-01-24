#include "valloc.h"
#include "vlist.h"
#include "vqueue.h"

#define SRT_VQUEUE_SIZE (sizeof(struct _VQueue))

struct _VQueue
{
    VList list_;
};

VQueue v_queue_new_impl(vsize element_size, vdtor element_dtor)
{
#ifndef __V_ENABLE_GC__
    VQueue queue = (VQueue)v_alloc(SRT_VQUEUE_SIZE);
#else // __V_ENABLE_GC__
    VQueue queue = (VQueue)v_alloc_with_dtor(SRT_VQUEUE_SIZE, v_queue_dtor);
#endif // !__V_ENABLE_GC__

    if (queue == vnull) {
        return (VQueue)vnull;
    }

    queue->list_ = v_list_new_impl(element_size, element_dtor);
    if (queue->list_ == vnull) {
        v_free(queue);
        return (VQueue)vnull;
    }

    return queue;
}

void v_queue_destroy(VQueue queue)
{
    if (queue == vnull) return;
    v_list_destroy(queue->list_);
    v_free(queue);
}

vsize v_queue_count(VQueue queue)
{
    return v_list_count(queue->list_);
}

vpointer v_queue_front_impl(VQueue queue)
{
    return v_list_get_impl(queue->list_, 0);
}

vpointer v_queue_back_impl(VQueue queue)
{
    return v_list_get_impl(queue->list_, v_list_count(queue->list_) - 1);
}

void v_queue_enqueue_impl(VQueue queue, vpointer pvalue)
{
    v_list_insert_impl(queue->list_, v_list_count(queue->list_), pvalue);
}

void v_queue_dequeue(VQueue queue)
{
    v_list_remove(queue->list_, 0);
}

void v_queue_dtor(vpointer ptr)
{
    v_queue_destroy(*(VQueue*)ptr);
}