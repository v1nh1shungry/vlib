#ifndef __V_LIST_H__
#define __V_LIST_H__

#include "vtype.h"

struct _VList;

typedef struct _VList* VList;

#define v_list_new(type) v_list_new_impl(sizeof(type), NULL)
#define v_list_new_ext(type, dtor) v_list_new_impl(sizeof(type), dtor)
#define v_list_copy_c_array_new(type, data, count) v_list_copy_c_array_new_impl(sizeof(type), data, count, NULL)
#define v_list_copy_c_array_new_ext(type, data, count, dtor) v_list_copy_c_array_new_impl(sizeof(type), data, count, dtor)
#define v_list_insert(type, list, index, value) \
    do { \
        type __v_list_insert_temp__ = value; \
        v_list_insert_impl(list, index, &__v_list_insert_temp__); \
    } while (0)
#define v_list_push_back(type, list, value) v_list_insert(type, list, v_list_count_of(list), value)
#define v_list_push_front(type, list, value) v_list_insert(type, list, 0, value)
#define v_list_pop_back(list) v_list_erase(list, v_list_count_of(list))
#define v_list_pop_front(list) v_list_erase(list, 0)
#define v_list_is_empty(list) (v_list_count_of(list) == 0)
#define v_list_get_index_of(type, list, index) (*(type*)v_list_get_index_of_impl(list, index))
#define v_list_set_index_of(type, list, index, value) \
    do { \
        type __v_list_set_index_of_temp__ = value; \
        v_list_set_index_of_impl(list, index, &__v_list_set_index_of_temp__); \
    } while (0)
#define v_list_back_of(type, list) v_list_get_index_of(type, list, v_list_count_of(list) - 1)
#define v_list_front_of(type, list) v_list_get_index_of(type, list, 0)

VList v_list_new_impl(vsize element_size, vdtor element_dtor);
VList v_list_copy_new(VList other);
VList v_list_copy_c_array_new_impl(vsize element_size, vpointer data, vsize count, vdtor element_dtor);
void v_list_destroy(VList list);
void v_list_clear(VList list);
vsize v_list_count_of(VList list);
vpointer v_list_get_index_of_impl(VList list, vsize index);
void v_list_set_index_of_impl(VList list, vsize index, vpointer pvalue);
void v_list_insert_impl(VList list, vsize index, vpointer pvalue);
void v_list_erase(VList list, vsize index);
void v_list_assign(VList dst, VList src);
void v_list_assign_c_array(VList dst, vpointer src, vsize count);
void v_list_swap(VList lhs, VList rhs);

#endif // !__V_LIST_H__