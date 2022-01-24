#include <stdio.h>
#include "test.h"
#include "viterator.h"
#include "varray.h"
#include "vlist.h"

int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

static void test_v_array_iterator()
{
    VArray array = v_array_from_c_array(int, values, 10);
    VIterator it1 = v_array_begin(array), it2 = v_array_end(array);
    v_iterator_next(it1);
    v_iterator_prev(it2);
    v_iterator_swap(int, it1, it2);
    printf("v_iterator_get(int, it1): %d\n", v_iterator_get(int, it1));
    printf("v_iterator_get(int, it2): %d\n", v_iterator_get(int, it2));
    v_iterator_destroy(it1);
    v_iterator_destroy(it2);
    v_array_destroy(array);
}

static void test_v_list_iterator()
{
    VList list = v_list_new(int);
    for (int i = 0; i < 10; ++i) {
        v_list_push_back(int, list, values[i]);
    }
    VIterator it1 = v_list_begin(list), it2 = v_list_end(list);
    while (!v_iterator_equal(it1, it2)) {
        printf("%d ", v_iterator_get(int, it1));
        v_iterator_next(it1);
    }
    v_iterator_destroy(it1);
    v_iterator_destroy(it2);
    v_list_destroy(list);
}

static void test_c_array_iterator()
{
    VIterator it1 = c_array_begin(int, values);
    VIterator it2 = c_array_begin(int, values);
    v_iterator_move(it2, 5);
    for (int i = 0; i < 5; ++i) {
        v_iterator_swap(int, it1, it2);
        v_iterator_next(it1);
        v_iterator_next(it2);
    }
    for (int i = 0; i < 10; ++i) {
        printf("%d ", values[i]);
    }
    v_iterator_destroy(it1);
    v_iterator_destroy(it2);
}

int main()
{
    v_test(test_v_array_iterator);
    v_test(test_v_list_iterator);
    v_test(test_c_array_iterator);
    return 0;
}