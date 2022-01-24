#ifndef __V_ARRAY_H__
#define __V_ARRAY_H__

#include "vtype.h"

struct _VArray;

typedef struct _VArray* VArray;

#define v_array_new(type) v_array_new_impl(sizeof(type), NULL)
#define v_array_new_ext(type, dtor) v_array_new_impl(sizeof(type), dtor)
#define v_array_sized_new(type, count) v_array_sized_new_impl(sizeof(type), count, NULL)
#define v_array_sized_new_ext(type, count, dtor) v_array_sized_new_impl(sizeof(type), count, dtor)
#define v_array_copy_c_array_new(type, data, count) v_array_copy_c_array_new_impl(sizeof(type), data, count, NULL)
#define v_array_copy_c_array_new_ext(type, data, count, dtor) v_array_copy_c_array_new_impl(sizeof(type), data, count, dtor)
#define v_array_is_empty(array) (v_array_count_of(array) == 0)
#define v_array_get_index_of(type, array, index) *((type*)v_array_get_index_of_impl(array, index))
#define v_array_front_of(type, array) v_array_get_index_of(type, array, 0)
#define v_array_back_of(type, array) v_array_get_index_of(type, array, v_array_count_of(array) - 1)
#define v_array_set_index_of(type, array, index, value) \
    do { \
        type __v_array_set_index_of_temp__ = value; \
        v_array_set_index_of_impl(array, index, &__v_array_set_index_of_temp__); \
    } while (0)
#define v_array_insert(type, array, index, value) \
    do { \
        type __v_array_insert_temp__ = value; \
        v_array_insert_impl(array, index, &__v_array_insert_temp__); \
    } while (0)
#define v_array_push_back(type, array, value) v_array_insert(type, array, v_array_count_of(array), value)
#define v_array_push_front(type, array, value) v_array_insert(type, array, 0, value)
#define v_array_pop_back(array) v_array_erase(array, v_array_count_of(array) - 1)
#define v_array_pop_front(array) v_array_erase(array, 0)

VArray v_array_new_impl(vsize element_size, vdtor element_dtor);
VArray v_array_sized_new_impl(vsize element_size, vsize count, vdtor element_dtor);
VArray v_array_copy_new(VArray other);
VArray v_array_copy_c_array_new_impl(vsize element_size, vpointer data, vsize count, vdtor element_dtor);
void v_array_destroy(VArray array);
vsize v_array_count_of(VArray array);
vsize v_array_capacity_of(VArray array);
vpointer v_array_get_index_of_impl(VArray array, vsize index);
void v_array_set_index_of_impl(VArray array, vsize index, vpointer pvalue);
void v_array_assign(VArray dst, VArray src);
void v_array_assign_c_array(VArray array, vpointer src, vsize count);
void v_array_clear(VArray array);
void v_array_swap(VArray lhs, VArray rhs);
void v_array_insert_impl(VArray array, vsize index, vpointer pvalue);
void v_array_erase(VArray array, vsize index);
void v_array_resize(VArray array, vsize size);
void v_array_shrink_to_fit(VArray array);

#endif // !__V_ARRAY_H__