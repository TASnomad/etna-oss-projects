#include			<gmanager.h>
#include			<list.h>
#include			<cell.h>
#include			<map.h>

#include			<json-c/json.h>
#include			<stdlib.h>

EnergyCell_t			*ctor_cell()
{
	EnergyCell_t		*c;
	game_instance_t		*m;

	m = get_instance();
	c = 0x0;
	if (m->energy_cells->index >= m->map_size)
		return (c);
	c = (EnergyCell_t *) malloc(sizeof(c));
	if (!c)
		return (c);
	do {
		c->x = random() % ((m->map_size - 1) + 1 - 0) + 0;
		c->y = random() % ((m->map_size - 1) + 1 - 0) + 0;
		c->value = random() % (15 + 1 - 5) + 5;
	} while (!is_free_cell(m->players, m->energy_cells, (int) c->x, (int) c->y));
	return (c);
}

void				dtor_cell(t_list *l, EnergyCell_t *c)
{
	c->x = c->y = c->value = 0;
	remove_list(&l, get_link_by_value(&l, c));
	free(c);
	c = 0x0;
}

static	struct json_object	*cell_to_json(EnergyCell_t *c)
{
	struct json_object	*json;

	json = json_object_new_object();
	if (!c)
		return (json);
	json_object_object_add(json, "x", json_object_new_int(c->x));
	json_object_object_add(json, "y", json_object_new_int(c->y));
	json_object_object_add(json, "value", json_object_new_int(c->value));
	return (json);
}

struct json_object		*cells_to_json(t_list *l)
{
	struct	json_object	*cells;
	t_link			*base;
	int			i;

	i = 0;
	base = l->first;
	cells = json_object_new_array();
	while (l->first)
	{
		if (l->first && l->first->value)
			json_object_array_put_idx(cells, i, cell_to_json((EnergyCell_t *) l->first->value));
		l->first = l->first->next;
		i++;
	}
	l->first = base;
	return (cells);
}

