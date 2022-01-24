#include <stdio.h>
#include "vstack.h"
#include "varray.h"

typedef struct _Pair
{
    vsize left;
    vsize right;
} Pair;

// a simple implementation of quick sort
static void quicksort(VArray values)
{
    if (v_array_count(values) < 2) return;
    vsize i, j;
    VStack stk = v_stack_new(Pair);
    Pair range = { 0, v_array_count(values) };
    v_stack_push(Pair, stk, range);
    while (!v_stack_empty(stk)) {
        range = v_stack_top(Pair, stk);
        v_stack_pop(stk);
        if (range.left >= range.right - 1) continue;
        i = range.left;
        j = range.right - 1;
        while (i < j) {
            while (v_array_get(int, values, range.left) <= v_array_get(int, values, j) && i < j) --j;
            while (v_array_get(int, values, i) <= v_array_get(int, values, range.left) && i < j) ++i;
            if (i != j) {
                int temp = v_array_get(int, values, i);
                v_array_set(int, values, i, v_array_get(int, values, j));
                v_array_set(int, values, j, temp);
            }
        }
        int temp = v_array_get(int, values, i);
        v_array_set(int, values, i, v_array_get(int, values, range.left));
        v_array_set(int, values, range.left, temp);
        Pair p = { range.left, i };
        v_stack_push(Pair, stk, p);
        p.left = i + 1, p.right = range.right;
        v_stack_push(Pair, stk, p);
    }
    v_stack_destroy(stk);
}

int main()
{
    int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    VArray array = v_array_from_c_array(int, values, 10);
    quicksort(array);
    for (vsize i = 0; i < v_array_count(array); ++i) {
        printf("%d ", v_array_get(int, array, i));
    }
    putchar('\n');
    v_array_destroy(array);
    return 0;
}