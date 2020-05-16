#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "list.h"

void	insert_node(t_list *list, t_node *node)
{
  node->next = list->head;
  if (list->head)
    {
      list->head->prev = node;
    }
  else
    {
      list->queue = node;
    }
  node->prev = NULL;
  list->head = node;
  list->size++;
}

t_node	*create_node(void *data)
{
  t_node	*node;

  if ((node = (t_node *)malloc(sizeof(*node))) == NULL)
    {
      printf("Fail to allocate memory\n");
      return (NULL);
    }
  node->next = NULL;
  node->prev = NULL;
  node->data = data;
  return (node);
}

t_node	*search_node
(t_list *list, void *data, int (*cmp)(void *, void *))
{
  t_node *first;

  first = list->head;
  while (first)
    {
      if (cmp(first->data, data) == 0)
	{
	  return (first);
	}
      first = first->next;
    }
  return (NULL);
}

void	delete_node(t_list *list, t_node *node, action handle)
{
  if (node->prev != NULL)
  {
    node->prev->next = node->next;
  }
  else
  {
    list->head = node->next;
  }
  if (node->next != NULL)
  {
    node->next->prev = node->prev;
  }
  list->size--;
  handle(node);
}

void	print_node(t_node *node)
{
  printf("%s\n", (char *)node->data);
}
