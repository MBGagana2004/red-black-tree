/*
 * Project: Red-Black Tree
 * Name: MB GAGANA
 * Batch: int26001_005
 * Description: Tests tree operations and verifies red-black tree invariants.
 */
#include "red_black_tree.h"

#include <assert.h>
#include <stddef.h>

#ifdef RED_BLACK_TREE_TEST_MAIN

/* Verify ordering, parent links, node colors, and equal black height. */
static int check_tree(const tree_t *node, const tree_t *parent,
                      int has_lower, data_t lower,
                      int has_upper, data_t upper)
{
    int left_height;
    int right_height;

    if (node == NULL) {
        return 1;
    }

    assert(node->parent == parent);
    assert(!has_lower || node->key > lower);
    assert(!has_upper || node->key < upper);
    assert(node->color == RED || node->color == BLACK);
    if (node->color == RED) {
        assert(node->left == NULL || node->left->color == BLACK);
        assert(node->right == NULL || node->right->color == BLACK);
    }

    left_height = check_tree(node->left, node, has_lower, lower, 1, node->key);
    right_height = check_tree(node->right, node, 1, node->key, has_upper, upper);
    assert(left_height == right_height);
    return left_height + (node->color == BLACK);
}

static void assert_valid(const tree_t *root)
{
    if (root != NULL) {
        assert(root->parent == NULL);
        assert(root->color == BLACK);
    }
    (void)check_tree(root, NULL, 0, 0, 0, 0);
}

int main(void)
{
    tree_t *root = NULL;
    data_t value;
    int i;

    /* Empty-tree operations must fail without changing the tree. */
    assert(find_minimum(&root, &value) == FAILURE);
    assert(find_maximum(&root, &value) == FAILURE);
    assert(delete_minimum(&root) == FAILURE);
    assert(delete_maximum(&root) == FAILURE);
    assert(delete(&root, 10) == FAILURE);

    /* Insert a varied ordering and validate the tree after every insertion. */
    for (i = 0; i < 100; ++i) {
        assert(insert(&root, (i * 37) % 100) == SUCCESS);
        assert_valid(root);
    }
    assert(insert(&root, 50) == FAILURE);
    assert(search(root, 50) == SUCCESS);
    assert(search(root, 100) == FAILURE);
    assert(find_minimum(&root, &value) == SUCCESS && value == 0);
    assert(find_maximum(&root, &value) == SUCCESS && value == 99);

    /* Repeatedly delete the minimum and verify every resulting tree. */
    for (i = 0; i < 100; ++i) {
        data_t expected = i;
        assert(find_minimum(&root, &value) == SUCCESS && value == expected);
        assert(delete_minimum(&root) == SUCCESS);
        assert(search(root, expected) == FAILURE);
        assert_valid(root);
    }
    assert(root == NULL);

    /* Rebuild and exercise deletion of the maximum. */
    for (i = 99; i >= 0; --i) {
        assert(insert(&root, i) == SUCCESS);
    }
    for (i = 99; i >= 0; --i) {
        assert(find_maximum(&root, &value) == SUCCESS && value == i);
        assert(delete_maximum(&root) == SUCCESS);
        assert_valid(root);
    }
    assert(root == NULL);

    /* Exercise deletion of arbitrary keys, including two-child nodes. */
    for (i = 0; i < 100; ++i) {
        assert(insert(&root, i) == SUCCESS);
    }
    for (i = 0; i < 100; ++i) {
        assert(delete(&root, (i * 53) % 100) == SUCCESS);
        assert_valid(root);
    }
    assert(root == NULL);

    destroy_tree(&root);
    return 0;
}

#else

typedef int red_black_tree_test_translation_unit;

#endif
