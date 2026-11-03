/*
 * Project: Red-Black Tree
 * Name: MB GAGANA
 * Batch: int26001_005
 * Description: Interactive menu for using the red-black tree operations.
 */
#include "red_black_tree.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Read a full integer value and distinguish invalid input from end-of-file. */
static int read_integer(const char *prompt, int *value)
{
    char input[128];
    char *end;
    long parsed;

    printf("%s", prompt);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    errno = 0;
    parsed = strtol(input, &end, 10);
    if (end == input || errno == ERANGE || parsed < INT_MIN ||
        parsed > INT_MAX) {
        fprintf(stderr, "Invalid input. Please enter an integer.\n");
        return 0;
    }

    while (*end != '\0' && isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0') {
        fprintf(stderr, "Invalid input. Please enter an integer.\n");
        return 0;
    }

    *value = (int)parsed;
    return 1;
}

/* Print the operations available in the interactive program. */
static void print_menu(void)
{
    printf("\nRed-Black Tree Menu\n"
           "1. Create a tree (insert values)\n"
           "2. Display\n"
           "3. Search a node\n"
           "4. Find maximum node\n"
           "5. Find minimum node\n"
           "6. Delete a node\n"
           "7. Delete minimum node\n"
           "8. Delete maximum node\n"
           "9. Exit\n");
}

/* Keep prompting until the user enters a valid yes/no response. */
static int read_yes_no(void)
{
    char answer[32];
    char *cursor;
    int choice;

    for (;;) {
        printf("Do you want to insert another value? (y/n): ");
        if (fgets(answer, sizeof(answer), stdin) == NULL) {
            return -1;
        }

        cursor = answer;
        while (isspace((unsigned char)*cursor)) {
            ++cursor;
        }
        if (*cursor == '\0') {
            fprintf(stderr, "Please enter y or n.\n");
            continue;
        }
        choice = tolower((unsigned char)*cursor++);
        while (isspace((unsigned char)*cursor)) {
            ++cursor;
        }

        if (*cursor == '\0' && choice == 'y') {
            return 1;
        }
        if (*cursor == '\0' && choice == 'n') {
            return 0;
        }

        fprintf(stderr, "Please enter y or n.\n");
    }
}

/* Insert one or more values, asking whether to continue after each attempt. */
static int insert_values(tree_t **root)
{
    int item;
    int input_status;

    for (;;) {
        input_status = read_integer("Enter value to insert: ", &item);
        if (input_status < 0) {
            return 0;
        }
        if (input_status == 0) {
            return 1;
        }

        if (insert(root, item) == SUCCESS) {
            printf("Inserted %d.\n", item);
        } else {
            printf("Could not insert %d (duplicate key or allocation failure).\n",
                   item);
        }

        input_status = read_yes_no();
        if (input_status < 0) {
            return 0;
        }
        if (input_status == 0) {
            return 1;
        }
    }
}

/* Run the menu loop and release all allocated tree nodes before exiting. */
int main(void)
{
    tree_t *root = NULL;
    int choice;
    int item;
    int result;
    int running = 1;

    while (running) {
        print_menu();
        result = read_integer("Enter choice: ", &choice);
        if (result < 0) {
            break;
        }
        if (result == 0) {
            continue;
        }

        switch (choice) {
        /* Build the tree by inserting user-provided keys. */
        case 1:
            running = insert_values(&root);
            break;
        /* Display all keys in sorted order with their colors. */
        case 2:
            if (root == NULL) {
                printf("Tree is empty.\n");
            } else {
                display_tree(root);
            }
            break;
        /* Search for a user-provided key. */
        case 3:
            result = read_integer("Enter value to search: ", &item);
            if (result < 0) {
                running = 0;
            } else if (result > 0) {
                printf("%d is %s.\n", item,
                       search(root, item) == SUCCESS ? "found" : "not found");
            }
            break;
        /* Report the largest key currently in the tree. */
        case 4:
            if (find_maximum(&root, &item) == SUCCESS) {
                printf("Maximum: %d\n", item);
            } else {
                printf("Tree is empty.\n");
            }
            break;
        /* Report the smallest key currently in the tree. */
        case 5:
            if (find_minimum(&root, &item) == SUCCESS) {
                printf("Minimum: %d\n", item);
            } else {
                printf("Tree is empty.\n");
            }
            break;
        /* Delete a user-selected key. */
        case 6:
            result = read_integer("Enter value to delete: ", &item);
            if (result < 0) {
                running = 0;
            } else if (result > 0) {
                printf(delete(&root, item) == SUCCESS
                           ? "Deleted %d.\n"
                           : "%d was not found.\n",
                       item);
            }
            break;
        /* Delete the smallest key. */
        case 7:
            if (delete_minimum(&root) == SUCCESS) {
                printf("Minimum node deleted.\n");
            } else {
                printf("Tree is empty.\n");
            }
            break;
        /* Delete the largest key. */
        case 8:
            if (delete_maximum(&root) == SUCCESS) {
                printf("Maximum node deleted.\n");
            } else {
                printf("Tree is empty.\n");
            }
            break;
        case 9:
            running = 0;
            break;
        default:
            printf("Invalid choice. Select a number from 1 to 9.\n");
            break;
        }
    }

    destroy_tree(&root);
    return 0;
}
