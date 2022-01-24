#include <string.h>
#include "valloc.h"
#include "vlist.h"

#define SRT_VLTNODE_SIZE sizeof(struct _VListNode)
#define SRT_VLIST_SIZE sizeof(struct _VList)

typedef struct _VListNode
{
    vpointer data_;
    struct _VListNode* prev_;
    struct _VListNode* next_;
} *VListNode;

struct _VList
{
    VListNode head_;
    VListNode tail_;
    vsize count_;
    vsize element_size_;
    vdtor element_dtor_;
};

/**
 * @brief Create a VListNode
 * 
 * @param data The data that the VListNode will contain
 * @param element_size The size of the data in bytes
 * @param prev The previous node
 * @param next The next node
 * @return A new VListNode or vnull if it runs out of memory
 */
static VListNode v_list_node_new(vpointer data, vsize element_size, VListNode prev, VListNode next)
{
    VListNode node = (VListNode)v_alloc(SRT_VLTNODE_SIZE);
    if (node == vnull) {
        return (VListNode)vnull;
    }
    node->data_ = v_alloc(element_size);
    if (node->data_ == vnull) {
        v_free(node);
        return (VListNode)vnull;
    }
    if (data != vnull) {
        memcpy(node->data_, data, element_size);
    }
    node->prev_ = prev;
    node->next_ = next;
    return node;
}

/**
 * @brief The destructor for VListNode
 */
static void v_list_node_destroy(VListNode node)
{
    v_free(node->data_);
    v_free(node);
}

/**
 * @brief Get a basically initialized list
 * 
 * @return A new list or vnull if it runs out of memory
 */
static VList v_list_get_init_list()
{
#ifndef __V_ENABLE_GC__
    VList list = (VList)v_alloc(SRT_VLIST_SIZE);
#else // __V_ENABLE_GC__
    VList list = (VList)v_alloc_with_dtor(SRT_VLIST_SIZE, v_list_dtor);
#endif // !__V_ENABLE_GC__

    if (list == vnull) {
        return (VList)vnull;
    }

    list->head_ = v_list_node_new(vnull, 0, vnull, vnull);
    list->tail_ = v_list_node_new(vnull, 0, vnull, vnull);
    if (list->head_ == vnull || list->tail_ == vnull) {
        v_free(list->head_);
        v_free(list->tail_);
        v_free(list);
        return (VList)vnull;
    }

    list->head_->next_ = list->tail_;
    list->tail_->prev_ = list->head_;
    list->count_ = 0;

    return list;
}

VList v_list_new_impl(vsize element_size, vdtor element_dtor)
{
    VList list = v_list_get_init_list();
    if (list == vnull) {
        return (VList)vnull;
    }
    list->element_size_ = element_size;
    list->element_dtor_ = element_dtor;
    return list;
}

VList v_list_copy(VList other)
{
    VList list = v_list_get_init_list();
    if (list == vnull) {
        return (VList)vnull;
    }
    list->element_size_ = other->element_size_;
    list->element_dtor_ = other->element_dtor_;
    VListNode node = other->head_->next_;
    while (node != other->tail_) {
        v_list_insert_impl(list, list->count_, node->data_);
        node = node->next_;
    }
    return list;
}

void v_list_destroy(VList list)
{
    if (list == vnull) return;
    v_list_clear(list);
    v_list_node_destroy(list->head_);
    v_list_node_destroy(list->tail_);
    v_free(list);
}

vsize v_list_count(VList list)
{
    return list->count_;
}

static vpointer v_list_dere_func(vpointer ptr)
{
    return ((VListNode)ptr)->data_;
}

static vpointer v_list_prev_func(vpointer ptr)
{
    return ((VListNode)ptr)->prev_;
}

static vpointer v_list_next_func(vpointer ptr)
{
    return ((VListNode)ptr)->next_;
}

VIterator v_list_begin(VList list)
{
    return v_bidirectional_iterator_new(list->head_->next_, v_list_dere_func, v_list_prev_func, v_list_next_func);
}

VIterator v_list_end(VList list)
{
    return v_bidirectional_iterator_new(list->tail_, v_list_dere_func, v_list_prev_func, v_list_next_func);
}

/**
 * @brief Find the index place in the list with the shortest step
 */
static VListNode v_list_find(VList list, vsize index)
{
    VListNode node = vnull;
    if (index < list->count_ / 2) {
        node = list->head_;
        for (vsize i = 0; i <= index; ++i) {
            node = node->next_;
        }
    }
    else {
        node = list->tail_;
        for (vsize i = 0; i < list->count_ - index; ++i) {
            node = node->prev_;
        }
    }
    return node;
}

vpointer v_list_get_impl(VList list, vsize index)
{
    return v_list_find(list, index)->data_;
}

void v_list_set_impl(VList list, vsize index, vpointer pvalue)
{
    memcpy(v_list_find(list, index)->data_, pvalue, list->element_size_);
}

void v_list_insert_impl(VList list, vsize index, vpointer pvalue)
{
    VListNode pivot = v_list_find(list, index);
    VListNode node = v_list_node_new(pvalue, list->element_size_, pivot->prev_, pivot);
    if (node == vnull) {
        return;
    }
    pivot->prev_->next_ = node;
    pivot->prev_ = node;
    ++list->count_;
}

void v_list_remove(VList list, vsize index)
{
    VListNode abandoned = v_list_find(list, index);
    abandoned->prev_->next_ = abandoned->next_;
    abandoned->next_->prev_ = abandoned->prev_;
    if (list->element_dtor_ != vnull) {
        list->element_dtor_(abandoned->data_);
    }
    v_list_node_destroy(abandoned);
    --list->count_;
}

void v_list_clear(VList list)
{
    if (list == vnull) return;
    VListNode node = list->head_->next_, temp;
    if (list->element_dtor_ == vnull) {
        while (node != list->tail_) {
            temp = node;
            node = node->next_;
            v_list_node_destroy(temp);
        }
    }
    else {
        while (node != list->tail_) {
            list->element_dtor_(node->data_);
            temp = node;
            node = node->next_;
            v_list_node_destroy(temp);
        }
    }
    list->head_->next_ = list->tail_;
    list->tail_->prev_ = list->head_;
    list->count_ = 0;
}

void v_list_swap(VList lhs, VList rhs)
{
    struct _VList tmp;
    memcpy(&tmp, lhs, SRT_VLIST_SIZE);
    memcpy(lhs, rhs, SRT_VLIST_SIZE);
    memcpy(rhs, &tmp, SRT_VLIST_SIZE);
}

void v_list_assign(VList dst, VList src)
{
    if (dst == src) return;
    VList tmp = v_list_copy(src);
    if (tmp != vnull) {
        v_list_swap(tmp, dst);
        v_list_destroy(tmp);
    }
}

void v_list_dtor(vpointer ptr)
{
    v_list_destroy(*(VList*)ptr);
}