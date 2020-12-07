#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)

#define NOT_ENOUGH_MEMORY 1

typedef struct node
{
  struct node* left, * right, * parent;
  int key,subTreeWidth;
  char data[200];
}NODE;

NODE* firstNode = NULL;

void ChangeWidthParents(NODE* start, int value)
{
  NODE* tmp = start;
  while (tmp)
  {
    tmp->subTreeWidth += value;
    tmp = tmp->parent;
  }
}

NODE** SearchPlaceToAddNode(int key, NODE** start)
{
  NODE** tmp = start;
  while ((*tmp))
  {
    if (key < (*tmp)->key)
    {
      if ((*tmp)->left == NULL)
      {
        if (!((*tmp)->left = (NODE*)malloc(sizeof(NODE))))
          return NULL;

        (*tmp)->left->parent = *tmp;
        tmp = &(*tmp)->left;
        break;
      }
      else
        tmp = &(*tmp)->left;
    }
    else if ((*tmp)->key == key)
      return 0;
    else
    {
      if ((*tmp)->right == NULL)
      {
        if (!((*tmp)->right = (NODE*)malloc(sizeof(NODE))))
          return NULL;

        (*tmp)->right->parent = *tmp;
        tmp = &(*tmp)->right;
        break;
      }
      else
        tmp = &(*tmp)->right;
    }
  }

  if (!(*tmp))
    if (*tmp = (NODE*)malloc(sizeof(NODE)))
      (*tmp)->parent = NULL;
    else
      return NULL;
  return tmp;
}

int AddNode(int key, const char* data)
{
  int width = strlen(data);
  NODE** tmp = SearchPlaceToAddNode(key, &firstNode);

  if (!tmp)
    return NOT_ENOUGH_MEMORY;

  (*tmp)->key = key;
  (*tmp)->left = NULL;
  (*tmp)->right = NULL;
  (*tmp)->subTreeWidth = 0;
  strcpy((*tmp)->data, data);
  ChangeWidthParents((*tmp)->parent, width);
  return 0;
}

NODE** FindNode(int key)
{
  NODE** tmp = &firstNode;
  while ((*tmp))
  {
    if ((*tmp)->key == key)
      break;
    else if (key < (*tmp)->key)
      tmp = &(*tmp)->left;
    else
      tmp = &(*tmp)->right;
  }
  return tmp;
}

void DeleteNode(int key)
{
  NODE** tmp = FindNode(key),
    ** newNode = NULL,
    * buf;

  if (!(*tmp))
    return;

  ChangeWidthParents((*tmp)->left->parent, 0-  strlen((*tmp)->left->data));
  ChangeWidthParents((*tmp)->parent, 0 - strlen((*tmp)->data));

  if ((*tmp)->left == NULL)
  {
    *tmp = (*tmp)->right;
    return;
  }
  else if ((*tmp)->right == NULL)
  {
    *tmp = (*tmp)->left;
    return;
  }

  newNode = SearchPlaceToAddNode((*tmp)->left->key, &(*tmp)->right);
  if (!newNode)
    return;

  buf = (*newNode)->parent;
  free((*newNode));
  *newNode = (*tmp)->left;
  (*newNode)->parent = buf;

  ChangeWidthParents((*newNode)->parent,strlen((*newNode)->data));
  (*tmp)->right->parent = (*tmp)->parent;
  buf = (*tmp)->right;
  free(*tmp);
  *tmp = buf;
}

void PrintTree(NODE* start)
{
  if (!start)
    return;
  printf("%s\nwidth:%d  subtree width:%d\n",start->data,strlen(start->data),start->subTreeWidth);
  if (start->left)
    PrintTree(start->left);
  if (start->right)
    PrintTree(start->right);
}

int main()
{
  char c = getchar();
  int number;
  while (c != EOF)
  {
    scanf("%d", &number);
    switch (c)
    {
    case 'a':
      AddNode(number, "hello");
      break;
    case 'r':
      DeleteNode(number);
      break;
    case 'f':
      if (*FindNode(number))
        printf("yes\n");
      else
        printf("no\n");
      break;
    default:
      printf("Incorrect input\n");
      PrintTree(firstNode);
      return 0;
    }

    c = getchar();
    if (c == '\r')
      c = getchar();
    if (c == '\n')
      c = getchar();
  }
  
  system("pause");
}