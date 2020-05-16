#ifndef __LIST_H__
#define __LIST_H__

// Generic doubly linked list
// Used to store players & energy cells

typedef	struct		s_link
{
	int		index;
	struct	s_link	*next;
	struct	s_link	*prev;
	void		*value;
}			t_link;

typedef struct		s_list
{
	int		index;
	t_link		*first;
	t_link		*last;
}			t_list;

t_list			*ctor_list();
int			dtor_list(t_list **);

int			count(t_list *l);

void			print(t_list *);

t_link			*add_to_list(t_list **, void *);
int			rm_in_list(t_list *, t_link *);
int			remove_list(t_list **, t_link *);

/**
 * Getters
 */
t_link			*get_link_by_value(t_list **, void *);
t_link			*get_link_by_index(t_list **, int);

/**
 * Utils
 */

int			is_list_empty(t_list **);
int			is_list_null(t_list **);

#endif /* ! __LIST_H__ */
