#include <stdio.h>
#include "test.h"
#include "vlist.h"

#define print_int_list(list) \
    do { \
        printf("%s: {\n", #list); \
        printf("    count: %d,\n", v_list_count(list)); \
        printf("    data: [ "); \
        for (vsize i = 0; i < v_list_count(list); ++i) { \
            printf("%d ", v_list_get(int, list, i)); \
        } \
        puts("]"); \
        puts("}"); \
    } while (0)

int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

static void int_dtor(vpointer ptr)
{
    printf("%d\n", *(int*)ptr);
}

static VList c_array2v_list(int data[], vsize count, vdtor dtor)
{
    VList list = v_list_new_with_dtor(int, dtor);
    for (vsize i = 0; i < count; ++i) {
        v_list_push_back(int, list, data[i]);
    }
    return list;
}

static void test_ctor_dtor()
{
    VList l1 = v_list_new(int);
    VList l2 = c_array2v_list(values, 10, vnull);
    VList l3 = v_list_copy(l2);
    VList l4 = c_array2v_list(values, 10, int_dtor);
    print_int_list(l1);
    print_int_list(l2);
    print_int_list(l3);
    print_int_list(l4);
    v_list_destroy(l1);
    v_list_destroy(l2);
    v_list_destroy(l3);
    printf("When l4 is destroyed,\n");
    v_list_destroy(l4);
}

static void test_get_set()
{
    VList l1 = c_array2v_list(values, 10, vnull);
    print_int_list(l1);
    for (vsize i = 0; i < v_list_count(l1); ++i) {
        v_list_set(int, l1, i, i);
    }
    for (vsize i = 0; i < v_list_count(l1); ++i) {
        printf("v_list_get(int, l1, %d): %d\n", i, v_list_get(int, l1, i));
    }
    printf("v_list_front(int, l1): %d\n", v_list_front(int, l1));
    printf("v_list_back(int, l1): %d\n", v_list_back(int, l1));
    v_list_destroy(l1);
}

static void test_insert_remove()
{
    VList l1 = v_list_new_with_dtor(int, int_dtor);
    print_int_list(l1);
    v_list_push_back(int, l1, 1);
    v_list_push_front(int, l1, 2);
    v_list_insert(int, l1, 1, 3);
    print_int_list(l1);
    printf("When l1 removes the second element, ");
    v_list_remove(l1, 1);
    printf("When l1 removes the back, ");
    v_list_pop_back(l1);
    print_int_list(l1);
    printf("When l1 is destroyed, ");
    v_list_destroy(l1);
}

static void test_clear()
{
    VList l1 = c_array2v_list(values, 10, vnull);
    print_int_list(l1);
    v_list_clear(l1);
    print_int_list(l1);
    v_list_destroy(l1);
}

static void test_swap()
{
    VList l1 = v_list_new(int);
    VList l2 = c_array2v_list(values, 10, vnull);
    print_int_list(l1);
    print_int_list(l2);
    v_list_swap(l1, l2);
    print_int_list(l1);
    print_int_list(l2);
    v_list_destroy(l1);
    v_list_destroy(l2);
}

static void test_assign()
{
    VList l1 = v_list_new(int);
    VList l2 = c_array2v_list(values, 10, vnull);
    print_int_list(l1);
    print_int_list(l2);
    v_list_assign(l1, l2);
    print_int_list(l1);
    v_list_destroy(l1);
    v_list_destroy(l2);
}

int main()
{
    v_test(test_ctor_dtor);
    v_test(test_get_set);
    v_test(test_insert_remove);
    v_test(test_clear);
    v_test(test_swap);
    v_test(test_assign);
    return 0;
}