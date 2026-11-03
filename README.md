# Red-black tree

**Name:** MB GAGANA  
**Batch:** int26001_005  
**Project:** Red-Black Tree

## Project description

This project implements a menu-driven red-black tree in C. A red-black tree is
a self-balancing binary search tree that uses node colors and rotations to keep
search, insertion, and deletion efficient as the tree changes. The program
supports integer-key insertion, deletion, search, minimum and maximum lookup,
and deletion of the minimum and maximum keys. It displays the keys in sorted
order together with each node's color. Duplicate keys are rejected, and empty
tree operations are reported.

## Build and run

With GCC on Windows:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c red_black_tree.c -o red_black_tree.exe
.\red_black_tree.exe
```

With GNU Make:

```text
make
make test
```

The program provides the numbered menu for inserting, displaying, searching,
finding extrema, and deleting keys. Display is an inorder traversal and prints
each key with its color (`RED->0` or `BLACK->1`).

The test executable checks the binary-search-tree ordering, parent links, root
color, red-node rule, and equal black height while exercising all operations.
The test entry point is enabled by the Makefile only, so compiling all project
`.c` files together does not introduce a second `main` function.

## API

Include `red_black_tree.h`. A tree starts as `tree_t *root = NULL`. Operations
that can change the root accept `tree_t **root`; `search` accepts the root node
directly. Every operation returns `SUCCESS` or `FAILURE`. `find_minimum` and
`find_maximum` write the result to the supplied output pointer. Call
`destroy_tree(&root)` when the tree is no longer needed.
