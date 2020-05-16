#ifndef _LIST_H_
# define _LIST_H_

#include "./node.h"

struct s_list
{
  t_node	*head;
  t_node	*queue;
  int		size;
};

t_list	*new_list();
void	ft_list_right(t_list *, action);
void	ft_list_left(t_list *, action);
void	delete_list(t_list *);
void  delete_list_exec_action(t_list *, action);
int   insert_list(t_list *, void *);
void	delete_item_in_list(t_list *, t_node *, action);
void  *search_item_in_list(t_list *, void *, int (*cmp)(void *, void *));
void  *node_val(t_node *);

#endif
