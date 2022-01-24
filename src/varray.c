#include <string.h>
#include "valloc.h"
#include "varray.h"

#define SRT_VARRAY_SIZE (sizeof(struct _VArray))

struct _VArray
{
    vpointer data_;
    vsize count_;
    vsize capacity_;
    vsize element_size_;
    vdtor element_dtor_;
};

/**
 * @brief The real constructor for VArray
 * 
 * @param data The data to initialize the VArray. Assigning it to vnull means no initialization
 * @param count The count of the elements
 * @param capacity The count of the elements required to allocate
 * @param element_size The size of each element in bytes
 * @param element_dtor The destructor for each element
 * @return A brand new VArray or vnull if it runs out of memory
 */
static VArray v_array_ctor(vpointer data, vsize count, vsize capacity, vsize element_size, vdtor element_dtor)
{
#ifndef __V_ENABLE_GC__
    VArray array = (VArray)v_alloc(SRT_VARRAY_SIZE);
#else // __V_ENABLE_GC__
    VArray array = (VArray)v_alloc_with_dtor(SRT_VARRAY_SIZE, v_array_dtor);
#endif // !__V_ENABLE_GC__

    if (array == vnull) {
        return (VArray)vnull;
    }

    array->data_ = v_alloc(capacity * element_size);
    if (array->data_ == vnull) {
        v_free(array);
        return (VArray)vnull;
    }

    if (data != vnull) {
        memcpy(array->data_, data, count * element_size);
    }
    array->capacity_ = capacity;
    array->count_ = count;
    array->element_size_ = element_size;
    array->element_dtor_ = element_dtor;

    return array;
}

VArray v_array_new_impl(vsize element_size, vdtor element_dtor)
{
    return v_array_ctor(vnull, 0, 0, element_size, element_dtor);
}

VArray v_array_sized_new_impl(vsize count, vsize element_size, vdtor element_dtor)
{
    return v_array_ctor(vnull, count, count, element_size, element_dtor);
}

VArray v_array_copy(VArray array)
{
    return v_array_ctor(array->data_, array->count_, array->capacity_, array->element_size_, array->element_dtor_);
}

VArray v_array_from_c_array_impl(vpointer data, vsize count, vsize element_size, vdtor element_dtor)
{
    if (data == vnull) {
        return (VArray)vnull;
    }
    return v_array_ctor(data, count, count, element_size, element_dtor);
}

void v_array_destroy(VArray array)
{
    v_array_clear(array);
    v_free(array);
}

vsize v_array_count(VArray array)
{
    return array->count_;
}

vsize v_array_capacity(VArray array)
{
    return array->capacity_;
}

static vpointer v_array_dere_func(vpointer ptr)
{
    return ptr;
}

static vpointer v_array_move_func(vpointer ptr, vi32 distance)
{
    return ptr + distance;
}

VIterator v_array_begin(VArray array)
{
    return v_random_access_iterator_new(array->data_, v_array_dere_func, array->element_size_, v_array_move_func);
}

VIterator v_array_end(VArray array)
{
    return v_random_access_iterator_new(array->data_ + array->count_ * array->element_size_, v_array_dere_func, array->element_size_, v_array_move_func);
}

void v_array_assign(VArray dst, VArray src)
{
    if (dst == src) return;
    VArray temp = v_array_copy(src);
    if (temp == vnull) return;
    v_array_swap(dst, temp);
    v_array_destroy(temp);
}

void v_array_assign_c_array(VArray dst, vpointer src, vsize count)
{
    if (src == vnull) {
        return;
    }
    if (count <= dst->capacity_) {
        memcpy(dst->data_, src, count * dst->element_size_);
        dst->count_ = count;
    }
    else {
        VArray temp = v_array_from_c_array_impl(src, count, dst->element_size_, dst->element_dtor_);
        if (temp == vnull) return;
        v_array_swap(dst, temp);
        v_array_destroy(temp);
    }
}

vpointer v_array_get_impl(VArray array, vsize index)
{
    return array->data_ + index * array->element_size_;
}

void v_array_set_impl(VArray array, vsize index, vpointer pvalue)
{
    memcpy(array->data_ + index * array->element_size_, pvalue, array->element_size_);
}

/**
 * @brief Expand the VArray only if the count of the VArray equals to the capacity of the VArray
 */
static void v_array_may_expand(VArray array)
{
    if (array->count_ < array->capacity_) return;
    if (array->capacity_ == 0) ++array->capacity_;
    v_array_reserve(array, array->capacity_ << 1);
}

void v_array_insert_impl(VArray array, vsize index, vpointer pvalue)
{
    v_array_may_expand(array);
    for (vsize i = array->count_; index < i; --i) {
        memcpy(array->data_ + i * array->element_size_,
            array->data_ + (i - 1) * array->element_size_,
            array->element_size_);
    }
    memcpy(array->data_ + index * array->element_size_, pvalue, array->element_size_);
    ++array->count_;
}

void v_array_remove(VArray array, vsize index)
{
    --array->count_;
    if (array->element_dtor_ != vnull) {
        array->element_dtor_(array->data_ + index * array->element_size_);
    }
    for (vsize i = index; i < array->count_; ++i) {
        memcpy(array->data_ + i * array->element_size_,
            array->data_ + (i + 1) * array->element_size_,
            array->element_size_);
    }
}

void v_array_swap(VArray lhs, VArray rhs)
{
    struct _VArray temp;
    memcpy(&temp, lhs, SRT_VARRAY_SIZE);
    memcpy(lhs, rhs, SRT_VARRAY_SIZE);
    memcpy(rhs, &temp, SRT_VARRAY_SIZE);
}

void v_array_clear(VArray array)
{
    if (array == vnull) return;
    if (array->element_dtor_ != vnull) {
        for (vsize i = 0; i < array->count_; ++i) {
            array->element_dtor_(array->data_ + i * array->element_size_);
        }
    }
    v_free(array->data_);
    array->data_ = vnull;
    array->count_ = array->capacity_ = 0;
}

void v_array_reserve(VArray array, vsize size)
{
    if (size < array->count_ || (size == array->count_ && array->count_ == array->capacity_)) return;
    VArray temp = v_array_ctor(array->data_, array->count_, size, array->element_size_, array->element_dtor_);
    if (temp != vnull) {
        v_array_swap(array, temp);
        v_array_destroy(temp);
    }
}

void v_array_shrink(VArray array)
{
    v_array_reserve(array, array->count_);
}

void v_array_dtor(vpointer ptr)
{
    v_array_destroy(*(VArray*)ptr);
}
