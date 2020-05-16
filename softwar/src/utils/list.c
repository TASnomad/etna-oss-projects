#include	<stdlib.h>
#include	<stdio.h>

#include	<list.h>

t_list		*ctor_list()
{
	t_list	*list;

	list = (t_list *) malloc(sizeof(t_list));
	if (!list)
		return (0x0);
	list->first = 0x0;
	list->last = 0x0;
	list->index = 0;
	return (list);
}

int		dtor_list(t_list **list)
{
	while ((*list)->first)
	{
		if (remove_list(list, (*list)->last))
			return (1);
	}
	free((*list));
	return (0);
}

int		is_list_empty(t_list **list)
{
	return (!list || !(*list) || !(*list)->first) ? (1) : (0);
}

int		is_list_null(t_list **list)
{
	return (!list || !(*list)) ? (1) : (0);
}

static void	init_chain(t_list **list, t_link **l)
{
	(*list)->first = (*l);
	(*list)->last = (*l);
	(*l)->next = 0x0;
	(*l)->prev = 0x0;
}

int		count(t_list *l)
{
	int	i;
	t_link	*base;

	i = 0;
	base = l->first;
	while (l->first)
	{
		l->first = l->first->next;
		i++;
	}
	l->first = base;
	return (i);
}

/**
 * @param list - doubly linked list
 * @param value - raw value to add
 * @return memory pointer representing the inserted link value
 */
t_link		*add_to_list(t_list **list, void *value)
{
	t_link	*l;

	l = (t_link *) malloc(sizeof(t_link));
	if (!l)
		return (0x0);
	l->value = value;
	if (is_list_empty(list))
	{
		init_chain(list, &l);
		(*list)->index = 0;
	}
	else
	{
		(*list)->last->next = l;
		l->next = 0x0;
		l->prev = (*list)->last;
		(*list)->last = l;
	}
	(*list)->index++;
	return (l);
}

int		remove_list(t_list **list, t_link *l)
{
	/*if (is_list_empty(list))
		return (-1);*/
	if (l == (*list)->first && l == (*list)->last)
	{
		(*list)->first = 0x0;
		(*list)->last = 0x0;
		free(l);
		return (-1);
	}
	else if (l == (*list)->first)
	{
		(*list)->first->next->prev = 0x0;
		(*list)->first = (*list)->first->next;
	}
	else if (l == (*list)->last)
	{
		(*list)->last->prev->next = 0x0;
		(*list)->last = (*list)->last->prev;
	}
	else
	{
		l->next->prev = l->prev;
		l->prev->next = l->next;
	}
	(*list)->index--;
	free(l);
	return (0);
}

// DEPRECATED
t_link		*get_link_by_index(t_list **list, int index)
{
	t_list	*base;
	t_link	*found;
	int	i;

	base = (*list);
	i = 0;
	found = 0x0;

	if (i > count(*list) + 1)
		return (found);

	while ((*list)->first)
	{
		if (i == index)
		{
			found = (*list)->first;
			break;
		}
		(*list)->first = (*list)->first->next;
		i++;
	}
	(*list) = base;
	return (found);
}

t_link		*get_link_by_value(t_list **list, void *value)
{
	t_list	*base;
	t_link	*found;

	base = (*list);
	found = 0x0;
	while ((*list)->first)
	{
		if ((*list)->first->value == value)
		{
			found = (*list)->first;
			break;
		}
		(*list)->first = (*list)->first->next;
	}
	(*list) = base; // Restoring initial position
	return (found);
}

