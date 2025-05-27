#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer05.h"

void pre_order_scan_tree(FILE *fptr, tree_node *tree, bFILE *bfp);
void post_order_scan_tree(FILE *fptr, tree_node *tree, bFILE *bfp);

FILE *file_open_input(char *filename)
{
  FILE *fptr;
  fptr = fopen(filename, "rb");
  if(!fptr)
  {
    fprintf(stderr, "filename invalid\n");
    return NULL;
  }
  return fptr;
}

FILE *file_open_output(char *filename)
{
  FILE *fptr;
  fptr = fopen(filename, "wb");
  if(!fptr)
  { 
    fprintf(stderr, "filename invalid\n");
    return NULL;
  }
  return fptr;
}

bFILE *b_fopen(char *filename, char *mode)
{
  bFILE *bfp = (bFILE *)malloc(sizeof(bFILE));
  if(strcmp(mode, "r") == 0)
  {
    bfp->mode = 0;
  }
  else if(strcmp(mode, "w") == 0)
  {
    bfp->mode = 1;
  }
  bfp->fp = fopen(filename, mode);
  if(!bfp->fp)
  {
    free(bfp);
    return NULL;
  }
  bfp->buffer = 0;
  bfp->bit_index = 0;
  return bfp;
}

void b_fclose(bFILE *bfp)
{
  if(bfp->mode == 1 && bfp->bit_index > 0)
  {
    fwrite(&bfp->buffer, 1, 1, bfp->fp);
  }
  fclose(bfp->fp);
  free(bfp);
}

int freadbit(bFILE *bfp)
{
  if(bfp->bit_index == 0)
  {
    if(fread(&bfp->buffer, 1, 1, bfp->fp) == 0)
    {
      return -1;
    }
  }
  int bit = (bfp->buffer >> (7 - bfp->bit_index)) & 1;
  bfp->bit_index = (bfp->bit_index + 1) % 8;

  return bit;
}


int fwritebit(bFILE *bfp, int bit)
{
  if(bit != 0 && bit != 1)
  {
    return 0;
  }
  if(bit == 1)
  {
    bfp->buffer |= (1 << (bfp->bit_index));
  }
  bfp->bit_index++;
  
  if(bfp->bit_index == 8)
  {
    fwrite(&bfp->buffer, 1, 1, bfp->fp);
    bfp->buffer = 0;
    bfp->bit_index = 0;
  }
  return 1;
}

long *create_binary_list(FILE *fptr)
{
  if(!fptr)
  {
    return NULL;
  }
  long *out = calloc(256, sizeof(long));
  int letter;
  while((letter = fgetc(fptr)) != EOF)
  {
    out[letter]++;
  }
  return out;
}

int count_unique(tree_node *info) //utilizes linked list
{
  if(!info)
  {
    return 0;
  }
  int unique = 0;
  while(info)
  {
    info = info->next;
    unique++;
  }
  return unique;
}

void pre_order_binary(FILE *fptr, tree_node *tree, int unique, bFILE *bfp) //recursive
{
  if(!(fptr && tree && unique))
  {
    return;
  }
  pre_order_scan_tree(fptr, tree, bfp);
  return;
}

void pre_order_scan_tree(FILE *fptr, tree_node *tree, bFILE *bfp)
{
  if(tree->letter == BLANK_VAL)
  {
    fwritebit(bfp, 0);
    pre_order_scan_tree(fptr, tree->left, bfp);
    pre_order_scan_tree(fptr, tree->right, bfp);
  }
  else
  {
    fwritebit(bfp, 1);
    int bit;
    for(int i = 0; i < 8; i++)
    {
      bit = (tree->letter >> i) & 1;
      fwritebit(bfp, bit); 
    }
  }
  return;
}

void read_bFILE_input(bFILE *bfp)
{
  int byte, letter;
  while((byte = bfp->fp) != EOF)
  {
    for(int i = 0; i < 8; i++)
    {
      byte |= freadbit(bfp) << (8 - i);
    }
  }
}

void post_order_binary(FILE *fptr, tree_node *tree, int unique, bFILE *bfp)
{
  if(!(fptr && tree && unique))
  {
    return;
  }
  post_order_scan_tree(fptr, tree, bfp);
  return;
}

void post_order_scan_tree(FILE *fptr, tree_node *tree, bFILE *bfp)
{
  if(tree->letter == BLANK_VAL)
  {
    post_order_scan_tree(fptr, tree->left, bfp);
    post_order_scan_tree(fptr, tree->right, bfp);
    fprintf(fptr, "0");
    // fwritebit(bfp, 0);
  }
  else
  {
    fprintf(fptr, "1%c", tree->letter);
    // fwritebit(bfp, 1);
    // int bit;
    // for(int i = 0; i < 8; i++)
    // {
    //   bit = (tree->letter >> i) & 1;
    //   fwritebit(bfp, bit);
    // }
  }
  return;
}