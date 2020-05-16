#include <stdio.h>
#include <stdlib.h>
#include "list.h"

t_list	*new_list()
{
  t_list	*list;

  list = NULL;
  if ((list = (t_list *)malloc(sizeof(*list))) == 0)
    {
      printf("Fail to allocate memory\n");
      return (NULL);
    }
  list->size = 0;
  list->head = NULL;
  list->queue = NULL;
  return (list);
}

int  insert_list(t_list *list, void *data)
{
    t_node  *node;

    node = create_node(data);
    if (!node)
    {
      perror("create_node()");
      return (0);
    }
    insert_node(list, node);
    return (1);
}

void	delete_item_in_list(t_list *list, t_node *node, action handle)
{
    delete_node(list, node, handle);
}

void  *search_item_in_list(t_list *list, void *data, int (*cmp)(void *, void *))
{
    t_node  *node;

    node = search_node(list, data, cmp);
    if (!node)
    {
      return (NULL);
    }
    return (node->data);
}

void  *node_val(t_node *node)
{
    return (node->data);
}

void	delete_list(t_list *list)
{
  t_node	*first;
  t_node	*tmp;
  
  first = list->head;
  while (first)
    {
      tmp = first;
      first = first->next;
      free(tmp);
    }
  free(list);
}

void	ft_list_left(t_list *list, action func)
{
  t_node	*first;

  first = list->head;
  while (first)
    {
      func(first);
      first = first->next;
    }
}

void	ft_list_right(t_list *list, action func)
{
  t_node	*bottom;

  bottom = list->queue;
  while (bottom)
    {
      func(bottom);
      bottom = bottom->prev;
    }
}
		      
void  delete_list_exec_action(t_list *list, action handle)
{
  t_node  *tmp;

  while (list->head)
  {
    tmp = list->head;
    list->head = list->head->next;
    handle(tmp);
    free(tmp);
   }
}