#ifndef __V_QUEUE_H__
#define __V_QUEUE_H__

#include "vtypes.h"

struct _VQueue;
typedef struct _VQueue* VQueue;

#define v_queue_new(type) v_queue_new_impl(sizeof(type), vnull)
#define v_queue_new_with_dtor(type, dtor) v_queue_new_impl(sizeof(type), dtor)
#define v_queue_empty(queue) (v_queue_count(queue) == 0)
#define v_queue_front(type, queue) (*(type*)v_queue_front_impl(queue))
#define v_queue_back(type, queue) (*(type*)v_queue_back_impl(queue))
#define v_queue_enqueue(type, queue, value) \
    do { \
        type __v_queue_enqueue_temp__ = value; \
        v_queue_enqueue_impl(queue, &__v_queue_enqueue_temp__); \
    } while (0)

/**
 * @brief The implementation of the macro `v_queue_new` and `v_queue_new_with_dtor`
 * 
 * @details Construct an empty VQueue
 * @return An empty VQueue or vnull if it runs out of memory
 */
VQueue v_queue_new_impl(vsize element_size, vdtor element_dtor);

/**
 * @brief The destructor of VQueue
 */
void v_queue_destroy(VQueue queue);

/**
 * @brief The count of elements in the VQueue
 */
vsize v_queue_count(VQueue queue);

/**
 * @brief The implementation of the macro `v_queue_front`
 * 
 * @details The front element of the VQueue
 */
vpointer v_queue_front_impl(VQueue queue);

/**
 * @brief The implementation of the macro `v_queue_back`
 * 
 * @details The back element of the VQueue
 */
vpointer v_queue_back_impl(VQueue queue);

/**
 * @brief The implementation of the macro `v_queue_enqueue`
 * 
 * @details Push a new element into the back of the VQueue
 */
void v_queue_enqueue_impl(VQueue queue, vpointer pvalue);

/**
 * @brief Pop out the front element of the VQueue
 */
void v_queue_dequeue(VQueue queue);

/**
 * @brief The wrapper of `v_queue_destroy` to fit `vdtor` type
 */
void v_queue_dtor(vpointer ptr);

#endif // !__V_QUEUE_H__