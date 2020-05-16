#ifndef _NODE_H_
# define _NODE_H_

typedef struct s_list	t_list;
typedef struct s_node	t_node;

typedef void (*action)(t_node *);

typedef int (*void_cmp)(void *, void *);

struct s_node
{
  void		*data;
  t_node	*next;
  t_node	*prev;
};


void	insert_node(t_list *, t_node *);
void	delete_node(t_list *, t_node *, action);
t_node	*create_node(void *);
t_node	*search_node(t_list *, void *, int (*cmp)(void *, void *));
void	print_node(t_node *);

#endif
