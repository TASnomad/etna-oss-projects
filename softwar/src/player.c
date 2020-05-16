#include			<string.h>
#include			<stdio.h>
#include			<json-c/json.h>

#include			<player.h>
#include			<default.h>
#include			<enums.h>
#include			<gmanager.h>
#include			<list.h>

player_t			*ctor_player(char *id, int num)
{
	game_instance_t		*m;
	player_t		*p;

	m = get_instance();
	p = 0x0;
	p = (player_t *) malloc(sizeof(player_t));
	if (!p)
		return (0x0);
	p->id = 0x0;
	p->id = strndup(id, strlen(id));
	if (!p->id)
	{
		free(p);
		my_logger(VERBOSE_LOG, "Cannot allocate memory at: %s\n", __func__);
		return (0x0);
	}
	p->x = 0;
	p->y = 0;
	p->disabled = 0;
	if (num % 2 == 0) {
		p->looking = DOWN;
		if (num == 2) {
			p->x = m->map_size - 1;
		} else if (num == 4) {
			p->x = p->y = m->map_size - 1;
			p->looking = DOWN;
		}
	} else {
		p->looking = UP;
		if (num == 1) {
			p->x = p->y = 0;
		} else if (num == 3) {
			p->y = m->map_size - 1;
		}
	}

	p->energy = ENERGY_STARTER;
	p->action = ACTION_STARTER;
	my_logger(REGULAR_LOG, "Player %s is created (%s)\n", p->id, __func__);
	return (p);
}

void				dtor_player(player_t *p)
{
	if (!p)
		return ;
	if (p->id)
	{
		free(p->id);
		p->id = 0x0;
	}
	free(p);
}

struct json_object		*player_to_json(player_t *p)
{
	struct json_object	*ret;

	ret = 0x0;
	ret = json_object_new_object();
	if (!p)
		return (ret);
	json_object_object_add(ret, "id", json_object_new_string(p->id));
	json_object_object_add(ret, "x", json_object_new_int(p->x));
	json_object_object_add(ret, "y", json_object_new_int(p->y));
	json_object_object_add(ret, "energy", json_object_new_int(p->energy));
	json_object_object_add(ret, "looking", json_object_new_int(p->looking));
	return (ret);
}

struct json_object		*players_list_to_json(t_list *l)
{
	struct json_object	*players;
	t_link			*base;
	int 			i;
	int			len;

	players = 0x0;
	i = len = 0;
	players = json_object_new_array();
	base = l->first;
	if (!l)
		return (players);
	len = count(l);
	while (l->first)
	{
		json_object_array_put_idx(players, i, player_to_json((player_t *) l->first ? l->first->value : 0x0));
		l->first = l->first->next;
		i++;
	}
	l->first = base;

	return (players);
}
