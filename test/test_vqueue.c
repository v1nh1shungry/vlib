#include <stdio.h>
#include "valloc.h"
#include "vqueue.h"

#define SRT_TREE_SIZE (sizeof(struct _BinTree))

typedef struct _BinTree
{
    int value;
    struct _BinTree* left;
    struct _BinTree* right;
}* BinTree;

static BinTree bin_tree_new(int value, BinTree left, BinTree right)
{
    BinTree tree = (BinTree)v_alloc(SRT_TREE_SIZE);
    tree->value = value;
    tree->left = left;
    tree->right = right;
    return tree;
}

static void bin_tree_destroy(BinTree tree)
{
    if (tree != vnull) {
        bin_tree_destroy(tree->left);
        bin_tree_destroy(tree->right);
        v_free(tree);
    }
}

// traverse the tree in the level order
static void levelorder(BinTree tree)
{
    VQueue queue = v_queue_new(BinTree);
    v_queue_enqueue(BinTree, queue, tree);
    while (!v_queue_empty(queue)) {
        tree = v_queue_front(BinTree, queue);
        v_queue_dequeue(queue);
        if (tree != vnull) {
            printf("%d ", tree->value);
            v_queue_enqueue(BinTree, queue, tree->left);
            v_queue_enqueue(BinTree, queue, tree->right);
        }
    }
    v_queue_destroy(queue);
}

int main()
{
    BinTree tree = 
        bin_tree_new(1,
            bin_tree_new(2,
                bin_tree_new(4, vnull, vnull),
                bin_tree_new(5, vnull, vnull)),
            bin_tree_new(3,
                bin_tree_new(6, vnull, vnull),
                bin_tree_new(7, vnull, vnull))
        );
    levelorder(tree);
    bin_tree_destroy(tree);
    return 0;
}