#ifndef __CELL_H__
#define __CELL_H__

#include		<list.h>
#include		<json-c/json.h>

typedef 		struct EnergyCell
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	value;
}			EnergyCell_t;

EnergyCell_t		*ctor_cell();
void			dtor_cell(t_list *, EnergyCell_t *);

struct json_object	*cells_to_json(t_list *);

#endif /* !__CELL_H__ */
