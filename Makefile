# Project: Red-Black Tree
# Name: MB GAGANA
# Batch: int26001_005
# Description: Builds the interactive program and runs red-black tree tests.

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -O2

.PHONY: all test clean

all: red_black_tree

red_black_tree: main.o red_black_tree.o
	$(CC) $(CFLAGS) -o $@ $^

test_red_black_tree: test_red_black_tree.o red_black_tree.o
	$(CC) $(CFLAGS) -o $@ $^

test: test_red_black_tree
	./test_red_black_tree

main.o: main.c red_black_tree.h
	$(CC) $(CFLAGS) -c main.c

red_black_tree.o: red_black_tree.c red_black_tree.h
	$(CC) $(CFLAGS) -c red_black_tree.c

test_red_black_tree.o: test_red_black_tree.c red_black_tree.h
	$(CC) $(CFLAGS) -DRED_BLACK_TREE_TEST_MAIN -c test_red_black_tree.c

clean:
	rm -f red_black_tree test_red_black_tree *.o
