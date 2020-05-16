#include		<ginfo.h>
#include		<cell.h>
#include		<gmanager.h>
#include		<enums.h>

#include		<stdlib.h> // malloc & free
#include		<string.h> // memset

#define			WALL_CELL "wall"
#define			ENERGY_CELL "energy"
#define			EMPTY_CELL "empty"
#define			ERROR_CELL "error"

int			is_player_in(t_list *l, int x, int y)
{
	t_link		*base;
	t_link		*iter;
	player_t	*p;

	base = l->first;
	iter = l->first;
	while (iter)
	{
		p = (player_t *) iter->value;
		if ((int) p->x == x && (int) p->y == y)
		{
			l->first = base;
			return (1);
		}
		iter = iter->next;
	}
	l->first = base;
	return (0);
}

int			is_energy_cell_in(t_list *l, int x, int y)
{
	t_link		*base;
	t_link		*iter;
	EnergyCell_t	*e;

	base = l->first;
	iter = l->first;
	while (iter)
	{
		e = (EnergyCell_t *) iter->value;
		if ((int) e->x == x && (int) e->y == y)
		{
			l->first = base;
			return (1);
		}
		iter = iter->next;
	}
	l->first = base;
	return (0);
}


t_link			*get_energy_by_coords(t_list *l, int x, int y)
{
	t_link		*base;
	t_link		*iter;
	EnergyCell_t	*e;

	base = l->first;
	iter = l->first;
	while (iter)
	{
		e = (EnergyCell_t *) iter->value;
		if ((int) e->x == x && (int) e->y == y)
		{
			l->first = base;
			return (iter);
		}
		iter = iter->next;
	}
	l->first = base;
	return (0x0);
}

player_t		*get_player_by_coords(t_list *l, int x, int y)
{
	t_link		*base;
	t_link		*iter;
	player_t	*p;

	base = l->first;
	iter = l->first;
	while (iter)
	{
		p = (player_t *) iter->value;
		if ((int) p->x == x && (int) p->y == y)
		{
			l->first = base;
			return (p);
		}
		iter = iter->next;
	}
	l->first = base;
	return (0x0);
}

int			is_wall_in(int x, int y)
{
	int		status;
	game_instance_t	*manager;

	manager = get_instance();
	if (x >= manager->map_size || y >= manager->map_size || x < 0 || y < 0)
		status = 1;
	else
		status = 0;
	return (status);
}

int			is_free_cell(t_list *l, t_list *s, int x, int y)
{
	return ((!is_player_in(l, x, y) && !is_energy_cell_in(s, x, y)) ? 1 : 0);
}

int			can_move(t_list *l, t_list *s, int x, int y)
{
	int		status;

	status = 0;

	status = is_free_cell(l, s, x, y) ? 1 : 0;
	status = is_wall_in(x, y) ? 0 : 1;
	return (status);
}


char			**get_watch_view(t_list *p, t_list *e, int x, int y, int looking)
{
	char		**ret;
	int		*x_s;
	int		*y_s;
	int		i;
	int		view_size;
	player_t	*pl;

	pl = 0x0;
	ret = 0x0;
	x_s = y_s = 0x0;
	i = view_size = 0;
	x_s = (int *) malloc(4 * sizeof(int));
	y_s = (int *) malloc(4 * sizeof(int));
	ret = (char **) malloc(4 * sizeof(char *));
	if (!x_s || !y_s || !ret)
		return (0x0);
	memset(x_s, 0, 4 * sizeof(*x_s)); // filling with 0
	memset(y_s, 0, 4 * sizeof(*y_s)); // filling with 0
	// Filling all arrays with x/y coords, depending on looking value
	switch (looking)
	{
		case DOWN:
			*(x_s + 0) = x;
			*(y_s + 0) = y + 1;
			*(x_s + 1) = x + 1;
			*(y_s + 1) = y + 2;
			*(x_s + 2) = x;
			*(x_s + 2) = y + 2;
			*(x_s + 3) = x - 1;
			*(y_s + 3) = y + 2;
		break;
		case UP:
			*(x_s + 0) = x;
			*(y_s + 0) = y - 1;
			*(x_s + 1) = x;
			*(y_s + 1) = y - 2;
			*(x_s + 2) = x - 1;
			*(x_s + 2) = y - 2;
			*(x_s + 3) = x + 1;
			*(y_s + 3) = y - 2;
		break;
		case RIGHT:
			*(x_s + 0) = x + 1;
			*(y_s + 0) = y;
			*(x_s + 1) = x + 2;
			*(y_s + 1) = y;
			*(x_s + 2) = x + 2;
			*(x_s + 2) = y - 1;
			*(x_s + 3) = x + 2;
			*(y_s + 3) = y + 1;
		break;
		case LEFT:
			*(x_s + 0) = x - 1;
			*(y_s + 0) = y;
			*(x_s + 1) = x - 2;
			*(y_s + 1) = y;
			*(x_s + 2) = x - 2;
			*(x_s + 2) = y - 1;
			*(x_s + 3) = x - 2;
			*(y_s + 3) = y + 1;
		break;
	}
	while (i < 4)
	{
		if (is_wall_in(*(x_s + i), *(y_s + i)))
			*(ret + i) = WALL_CELL;
		else if (is_player_in(p, *(x_s + i), *(y_s + i)))
		{
			pl = get_player_by_coords(p, *(x_s + i), *(y_s + i));
			*(ret + i) = (pl) ? strdup(pl->id) : ERROR_CELL;
		}
		else if (is_energy_cell_in(e, *(x_s + i), *(y_s + i)))
		{
			*(ret + i) = ENERGY_CELL;
		}
		else

			*(ret + i) = EMPTY_CELL;
		i++;
	}
	free(x_s);
	free(y_s);
	return (ret);
}
