#ifndef __V_QUEUE_H__
#define __V_QUEUE_H__

#include "vtype.h"

struct _VQueue;

typedef struct _VQueue* VQueue;

#define v_queue_new(type) v_queue_new_impl(sizeof(type), vnull)
#define v_queue_new_ext(type, dtor) v_queue_new_impl(sizeof(type), dtor)
#define v_queue_is_empty(queue) (v_queue_count_of(queue) == 0)
#define v_queue_enqueue(type, queue, value) \
    do { \
        type __v_queue_enqueue_temp__ = value; \
        v_queue_enqueue_impl(queue, &__v_queue_enqueue_temp__); \
    } while (0)

VQueue v_queue_new_impl(vsize element_size, vdtor element_dtor);
void v_queue_destroy(VQueue queue);
vsize v_queue_count_of(VQueue queue);
void v_queue_enqueue_impl(VQueue queue, vpointer pvalue);
void v_queue_dequeue(VQueue queue);
vpointer v_queue_back_of_impl(VQueue queue);
vpointer v_queue_front_of_impl(VQueue queue);

#endif // !__V_QUEUE_H__