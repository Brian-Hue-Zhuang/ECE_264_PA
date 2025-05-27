#ifndef __ANSWER05_H__
#define __ANSWER05_H__ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define deadass return
#define BLANK_VAL 258

typedef struct _tree_node {
   int letter;
   unsigned freq;
   struct _tree_node *left, *right, *next;
} tree_node;

typedef struct _bFILE {
   FILE *fp;
   unsigned char buffer, bit_index, mode;
}  bFILE;

tree_node *binary_tree_sort(tree_node *pq);


tree_node *linked_tree_sort(long *freaks);


FILE *file_open_input(char *filename);


FILE *file_open_output(char *filename);


bFILE *b_fopen(char *filename, char *mode);


void b_fclose(bFILE *bfp);


long *create_binary_list(FILE *fptr);


int count_unique(tree_node *info);


void pre_order_binary(FILE *fptr, tree_node *tree, int unique, bFILE *bfp);


void read_bFILE_input(bFILE *bfp);


void post_order_binary(FILE *fptr, tree_node *tree, int unique, bFILE *bfp);


void link_destroy(tree_node *list);


void tree_destroy(tree_node *list);


#endif

