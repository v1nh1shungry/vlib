#include <stdio.h>
#include "varray.h"
#include "test.h"

#define print_int_varray(array) \
    do { \
        printf("%s: {\n", #array); \
        printf("    count: %d,\n", v_array_count(array)); \
        printf("    capacity: %d,\n", v_array_capacity(array)); \
        printf("    data: [ "); \
        for (vsize i = 0; i < v_array_count(array); ++i) { \
            printf("%d ", v_array_get(int, array, i)); \
        } \
        puts("]"); \
        puts("}"); \
    } while (0)

int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

static void int_dtor(vpointer ptr)
{
    printf("%d\n", *(int*)ptr);
}

static void test_ctors_dtor()
{
    VArray a1 = v_array_new(int);
    VArray a2 = v_array_sized_new(int, 10);
    VArray a3 = v_array_from_c_array(int, values, 10);
    VArray a4 = v_array_copy(a3);
    VArray a5 = v_array_from_c_array_with_dtor(int, values, 10, int_dtor);
    print_int_varray(a1);
    print_int_varray(a2);
    print_int_varray(a3);
    print_int_varray(a4);
    print_int_varray(a5);
    v_array_destroy(a1);
    v_array_destroy(a2);
    v_array_destroy(a3);
    v_array_destroy(a4);
    printf("When a5 is destroyed,\n");
    v_array_destroy(a5);
}

static void test_assign()
{
    VArray a1 = v_array_new(int);
    print_int_varray(a1);
    v_array_assign_c_array(a1, values, 10);
    print_int_varray(a1);
    VArray a2 = v_array_sized_new(int, 10);
    print_int_varray(a2);
    v_array_assign(a2, a1);
    print_int_varray(a2);
    v_array_destroy(a1);
    v_array_destroy(a2);
}

static void test_get_set()
{
    VArray a1 = v_array_sized_new(int, 10);
    print_int_varray(a1);
    for (int i = 0; i < 10; ++i) {
        v_array_set(int, a1, i, i);
    }
    for (int i = 0; i < 10; ++i) {
        printf("v_array_get(int, a1, %d): %d\n", i, v_array_get(int, a1, i));
    }
    printf("v_array_front(int, a1): %d\n", v_array_front(int, a1));
    printf("v_array_back(int, a1): %d\n", v_array_back(int, a1));
    v_array_destroy(a1);
}

static void test_insert_remove()
{
    VArray a1 = v_array_new_with_dtor(int, int_dtor);
    v_array_push_back(int, a1, 1);
    v_array_push_front(int, a1, 2);
    v_array_insert(int, a1, 1, 3);
    print_int_varray(a1);
    printf("When a1 removes the second element, ");
    v_array_remove(a1, 1);
    printf("When a1 removes the back, ");
    v_array_pop_back(a1);
    print_int_varray(a1);
    printf("When a1 is destroyed, ");
    v_array_destroy(a1);
}

static void test_clear()
{
    VArray a1 = v_array_sized_new(int, 100);
    print_int_varray(a1);
    v_array_clear(a1);
    print_int_varray(a1);
    v_array_destroy(a1);
}

static void test_swap()
{
    VArray a1 = v_array_new(int);
    VArray a2 = v_array_from_c_array(int, values, 10);
    print_int_varray(a1);
    print_int_varray(a2);
    v_array_swap(a1, a2);
    print_int_varray(a1);
    print_int_varray(a2);
    v_array_destroy(a1);
    v_array_destroy(a2);
}

static void test_reserve_shrink()
{
    VArray a1 = v_array_new(int);
    print_int_varray(a1);
    v_array_reserve(a1, 100);
    print_int_varray(a1);
    v_array_push_back(int, a1, 1);
    print_int_varray(a1);
    v_array_shrink(a1);
    print_int_varray(a1);
    v_array_destroy(a1);
}

static void test_nest_array()
{
    VArray a1 = v_array_new_with_dtor(VArray, v_array_dtor);
    v_array_push_back(VArray, a1, v_array_new(int));
    v_array_push_back(VArray, a1, v_array_from_c_array(int, values, 10));
    print_int_varray(v_array_get(VArray, a1, 0));
    print_int_varray(v_array_get(VArray, a1, 1));
    v_array_destroy(a1);
}

int main()
{
    v_test(test_ctors_dtor);
    v_test(test_assign);
    v_test(test_get_set);
    v_test(test_insert_remove);
    v_test(test_swap);
    v_test(test_clear);
    v_test(test_reserve_shrink);
    v_test(test_nest_array);
    return 0;
}