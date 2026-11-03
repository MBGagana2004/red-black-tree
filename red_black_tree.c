/*
 * Project: Red-Black Tree
 * Name: MB GAGANA
 * Batch: int26001_005
 * Description: Red-black tree operations for integer keys, including
 * insertion, deletion, search, extrema lookup, and inorder display.
 */
#include "red_black_tree.h"

#include <stdio.h>
#include <stdlib.h>

/* Treat missing children as black leaves, as required by red-black rules. */
static color_t node_color(const tree_t *node)
{
    return node == NULL ? BLACK : node->color;
}

static tree_t *minimum_node(tree_t *node)
{
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

static tree_t *maximum_node(tree_t *node)
{
    while (node != NULL && node->right != NULL) {
        node = node->right;
    }
    return node;
}

/* Rotate the subtree while keeping parent links and the root pointer valid. */
static void rotate_left(tree_t **root, tree_t *node)
{
    tree_t *pivot = node->right;

    node->right = pivot->left;
    if (pivot->left != NULL) {
        pivot->left->parent = node;
    }

    pivot->parent = node->parent;
    if (node->parent == NULL) {
        *root = pivot;
    } else if (node == node->parent->left) {
        node->parent->left = pivot;
    } else {
        node->parent->right = pivot;
    }

    pivot->left = node;
    node->parent = pivot;
}

static void rotate_right(tree_t **root, tree_t *node)
{
    tree_t *pivot = node->left;

    node->left = pivot->right;
    if (pivot->right != NULL) {
        pivot->right->parent = node;
    }

    pivot->parent = node->parent;
    if (node->parent == NULL) {
        *root = pivot;
    } else if (node == node->parent->left) {
        node->parent->left = pivot;
    } else {
        node->parent->right = pivot;
    }

    pivot->right = node;
    node->parent = pivot;
}

/* Search follows the binary-search ordering and reports whether the key exists. */
int search(const tree_t *root, data_t item)
{
    while (root != NULL) {
        if (item == root->key) {
            return SUCCESS;
        }
        root = item < root->key ? root->left : root->right;
    }
    return FAILURE;
}

/* Insert a unique key, then recolor or rotate to restore red-black properties. */
int insert(tree_t **root, data_t item)
{
    tree_t *parent = NULL;
    tree_t *current;
    tree_t *node;

    if (root == NULL) {
        return FAILURE;
    }

    current = *root;
    while (current != NULL) {
        parent = current;
        if (item == current->key) {
            return FAILURE;
        }
        current = item < current->key ? current->left : current->right;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return FAILURE;
    }

    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    node->color = RED;
    node->key = item;

    if (parent == NULL) {
        *root = node;
    } else if (item < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    while (node != *root && node_color(node->parent) == RED) {
        tree_t *parent_node = node->parent;
        tree_t *grandparent = parent_node->parent;

        if (parent_node == grandparent->left) {
            tree_t *uncle = grandparent->right;
            if (node_color(uncle) == RED) {
                parent_node->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent_node->right) {
                    node = parent_node;
                    rotate_left(root, node);
                    parent_node = node->parent;
                    grandparent = parent_node->parent;
                }
                parent_node->color = BLACK;
                grandparent->color = RED;
                rotate_right(root, grandparent);
            }
        } else {
            tree_t *uncle = grandparent->left;
            if (node_color(uncle) == RED) {
                parent_node->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent_node->left) {
                    node = parent_node;
                    rotate_right(root, node);
                    parent_node = node->parent;
                    grandparent = parent_node->parent;
                }
                parent_node->color = BLACK;
                grandparent->color = RED;
                rotate_left(root, grandparent);
            }
        }
    }

    (*root)->color = BLACK;
    return SUCCESS;
}

/* Replace one subtree with another during deletion. */
static void transplant(tree_t **root, tree_t *old_node, tree_t *new_node)
{
    if (old_node->parent == NULL) {
        *root = new_node;
    } else if (old_node == old_node->parent->left) {
        old_node->parent->left = new_node;
    } else {
        old_node->parent->right = new_node;
    }

    if (new_node != NULL) {
        new_node->parent = old_node->parent;
    }
}

/*
 * Repair the black-height imbalance after removing a black node.
 * node_is_left records the missing child's side when node is a NULL leaf.
 */
static void delete_fixup(tree_t **root, tree_t *node, tree_t *parent,
                         int node_is_left)
{
    while (node != *root && node_color(node) == BLACK) {
        tree_t *sibling;

        if (parent == NULL) {
            break;
        }

        if ((node != NULL && node == parent->left) ||
            (node == NULL && node_is_left)) {
            sibling = parent->right;
            if (node_color(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotate_left(root, parent);
                sibling = parent->right;
            }

            if (sibling == NULL) {
                node = parent;
                parent = node->parent;
                node_is_left = parent != NULL && node == parent->left;
                continue;
            }

            if (node_color(sibling->left) == BLACK &&
                node_color(sibling->right) == BLACK) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
                node_is_left = parent != NULL && node == parent->left;
            } else {
                if (node_color(sibling->right) == BLACK) {
                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rotate_right(root, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->right != NULL) {
                    sibling->right->color = BLACK;
                }
                rotate_left(root, parent);
                node = *root;
                parent = NULL;
            }
        } else {
            sibling = parent->left;
            if (node_color(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotate_right(root, parent);
                sibling = parent->left;
            }

            if (sibling == NULL) {
                node = parent;
                parent = node->parent;
                node_is_left = parent != NULL && node == parent->left;
                continue;
            }

            if (node_color(sibling->right) == BLACK &&
                node_color(sibling->left) == BLACK) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
                node_is_left = parent != NULL && node == parent->left;
            } else {
                if (node_color(sibling->left) == BLACK) {
                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    rotate_left(root, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->left != NULL) {
                    sibling->left->color = BLACK;
                }
                rotate_right(root, parent);
                node = *root;
                parent = NULL;
            }
        }
    }

    if (node != NULL) {
        node->color = BLACK;
    }
}

/* Delete a key using its in-order successor when it has two children. */
int delete(tree_t **root, data_t item)
{
    tree_t *target;
    tree_t *moved;
    tree_t *fixup_node;
    tree_t *fixup_parent;
    color_t removed_color;
    int fixup_node_is_left = 0;

    if (root == NULL) {
        return FAILURE;
    }

    target = *root;
    while (target != NULL && target->key != item) {
        target = item < target->key ? target->left : target->right;
    }
    if (target == NULL) {
        return FAILURE;
    }

    moved = target;
    removed_color = moved->color;
    if (target->left == NULL) {
        fixup_node = target->right;
        fixup_parent = target->parent;
        fixup_node_is_left =
            fixup_parent != NULL && target == fixup_parent->left;
        transplant(root, target, target->right);
        if (fixup_node != NULL) {
            fixup_parent = fixup_node->parent;
        }
    } else if (target->right == NULL) {
        fixup_node = target->left;
        fixup_parent = target->parent;
        fixup_node_is_left =
            fixup_parent != NULL && target == fixup_parent->left;
        transplant(root, target, target->left);
        if (fixup_node != NULL) {
            fixup_parent = fixup_node->parent;
        }
    } else {
        moved = minimum_node(target->right);
        removed_color = moved->color;
        fixup_node = moved->right;

        if (moved->parent == target) {
            fixup_parent = moved;
            fixup_node_is_left = 0;
            if (fixup_node != NULL) {
                fixup_node->parent = moved;
            }
        } else {
            fixup_parent = moved->parent;
            fixup_node_is_left = moved == fixup_parent->left;
            transplant(root, moved, moved->right);
            moved->right = target->right;
            moved->right->parent = moved;
        }

        transplant(root, target, moved);
        moved->left = target->left;
        moved->left->parent = moved;
        moved->color = target->color;
    }

    free(target);
    if (removed_color == BLACK) {
        delete_fixup(root, fixup_node, fixup_parent, fixup_node_is_left);
    }
    return SUCCESS;
}

/* Return the leftmost key; fail if the tree or output pointer is empty. */
int find_minimum(tree_t **root, data_t *min)
{
    tree_t *node;

    if (root == NULL || min == NULL || *root == NULL) {
        return FAILURE;
    }

    node = minimum_node(*root);
    *min = node->key;
    return SUCCESS;
}

/* Return the rightmost key; fail if the tree or output pointer is empty. */
int find_maximum(tree_t **root, data_t *max)
{
    tree_t *node;

    if (root == NULL || max == NULL || *root == NULL) {
        return FAILURE;
    }

    node = maximum_node(*root);
    *max = node->key;
    return SUCCESS;
}

/* Locate the leftmost node and remove it through the standard delete routine. */
int delete_minimum(tree_t **root)
{
    tree_t *node;

    if (root == NULL || *root == NULL) {
        return FAILURE;
    }

    node = minimum_node(*root);
    return delete(root, node->key);
}

/* Locate the rightmost node and remove it through the standard delete routine. */
int delete_maximum(tree_t **root)
{
    tree_t *node;

    if (root == NULL || *root == NULL) {
        return FAILURE;
    }

    node = maximum_node(*root);
    return delete(root, node->key);
}

/* Print keys in ascending order, including each node's color. */
void display_tree(const tree_t *root)
{
    if (root == NULL) {
        return;
    }

    display_tree(root->left);
    printf("(%d)--(%s->%d)\n",
           root->key,
           root->color == RED ? "RED" : "BLACK",
           root->color == RED ? 0 : 1);
    display_tree(root->right);
}

/* Recursively free every node and reset the caller's root pointer. */
void destroy_tree(tree_t **root)
{
    if (root == NULL || *root == NULL) {
        return;
    }

    destroy_tree(&(*root)->left);
    destroy_tree(&(*root)->right);
    free(*root);
    *root = NULL;
}
