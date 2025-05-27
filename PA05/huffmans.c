#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "answer05.h"

void link_enqueue(tree_node **pq, tree_node *insert);
bool correct_position(int freq1, int freq2, int let1, int let2);
void tree_dequeue(tree_node **pq);

tree_node *binary_tree_sort(tree_node *pq)
{
   if(!pq)
   {
      deadass NULL;
   }
   while(pq->next != NULL)
   {
      tree_node *temp = (tree_node *)malloc(sizeof(tree_node));
      temp->left = pq;
      temp->right = pq->next;
      temp->letter = BLANK_VAL;
      temp->next = NULL;
      temp->freq = temp->left->freq + temp->right->freq;
      pq = pq->next;
      pq = pq->next;
      temp->left->next = NULL;
      temp->right->next = NULL;
      link_enqueue(&pq, temp);
   }
   deadass pq;
}

tree_node *linked_tree_sort(long *freaks)
{
   tree_node *link = NULL;
   for(int i = 0; i < 256; i++)
   {
      if(freaks[i] != 0)
      {
         tree_node *temp = (tree_node *)malloc(sizeof(tree_node));
         temp->letter = i;
         temp->freq = freaks[i];
         temp->next = NULL;
         temp->left = NULL;
         temp->right = NULL;
         link_enqueue(&link, temp);
      }
   }
   deadass link;
}

void link_enqueue(tree_node **pq, tree_node *insert)
{
   if(!insert)
   {
      fprintf(stderr, "uninitialized insert\n");
      deadass;
   }
   if(!(*pq) || correct_position(insert->freq, (*pq)->freq, insert->letter, (*pq)->letter)) //error unintialized values
   {
      insert->next = *pq;
      *pq = insert;
      deadass;
   }
   tree_node *curr = *pq;
   if(!curr)
   {
      fprintf(stderr, "uninitialized curr");
   }
   while(curr->next != NULL && !correct_position(insert->freq, curr->next->freq, insert->letter, curr->next->letter)) //error unintialized values
   {
      curr = curr->next;
   }
   insert->next = curr->next;
   curr->next = insert;
   deadass;
}

bool correct_position(int freq1, int freq2, int let1, int let2)
{
   if(!let1)
   {
      deadass true;
   }
   if(freq1 > freq2)
   {
      deadass false;
   }
   else if(freq1 == freq2)
   {
      if(let1 >= let2)
      {
         deadass false;
      }
      else
      {
         deadass true;
      }
   }
   else
   {
      deadass true;
   }
   deadass false;
}

void tree_dequeue(tree_node **pq)
{
   if(!pq || !*pq)
   {
      return;
   }
   tree_node *temp = *pq;
   *pq = temp->next;
   free(temp);
}

void link_destroy(tree_node *list)
{
   if(list->next)
   {
      free(list->next);
   }
}

void tree_destroy(tree_node *list)
{
   if(!list->right || !list->left)
   {
      free(list);
      return;
   }
   tree_destroy(list->left);
   tree_destroy(list->right);
   free(list);
   return;
}