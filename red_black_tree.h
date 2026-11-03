/*
 * Project: Red-Black Tree
 * Name: MB GAGANA
 * Batch: int26001_005
 * Description: Implements a self-balancing binary search tree for integer keys.
 * Supports insertion, deletion, and search operations.
 * Provides minimum/maximum lookup and deletion operations.
 * Maintains balance using node colors and tree rotations.
 */
#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

/* Key type stored in each tree node. */
typedef int data_t;

/* Status returned by operations that may fail. */
typedef enum {
    FAILURE = 0,
    SUCCESS = 1
} status_t;

/* Red-black tree node colors. */
typedef enum {
    BLACK = 0,
    RED = 1
} color_t;

/* Tree node: parent/child links, color, and integer key. */
typedef struct tree_node {
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
    color_t color;
    data_t key;
} tree_t;

/* Insert a unique key; root may change after balancing. */
int insert(tree_t **root, data_t item);
/* Remove the matching key; return FAILURE if it is absent. */
int delete(tree_t **root, data_t item);
/* Return SUCCESS if the key exists in the tree. */
int search(const tree_t *root, data_t item);
/* Store the smallest key in min. */
int find_minimum(tree_t **root, data_t *min);
/* Store the largest key in max. */
int find_maximum(tree_t **root, data_t *max);
/* Remove the smallest key. */
int delete_minimum(tree_t **root);
/* Remove the largest key. */
int delete_maximum(tree_t **root);
/* Display keys in ascending order with their colors. */
void display_tree(const tree_t *root);
/* Free all nodes and set the root to NULL. */
void destroy_tree(tree_t **root);

#endif
