#include		<list.h>
#include		<enums.h>
#include		<gmanager.h>
#include		<player.h>
#include		<map.h>
#include		<cell.h>
#include		<command.h>

#include		<stdio.h>
#include		<stdlib.h>
#include		<string.h>
#include		<json-c/json.h>

const command_t		cmds[] = {
	{ IDENTITY_CMD, identify },
	{ SELFID_CMD, selfid },
	{ SELFSTATS_CMD	, selfstats },
	{ INSPECT_CMD, inspect },
	{ LOOKING_CMD, looking },
	{ LEFT_CMD, left },
	{ RIGHT_CMD, right },
	{ FORWARD_CMD, forward },
	{ BACKWARD_CMD, backward },
	{ LEFTFWD_CMD, leftfwd },
	{ RIGHTFWD_CMD, rightfwd },
	{ NEXT_CMD, next },
	{ JUMP_CMD, jump },
	{ GATHER_CMD, gather },
	{ WATCH_CMD, watch },
	{ ATTACK_CMD, attack },
	{ "commands", commands_to_string },
	{ 0x0, 0x0 }
};

char			*commands_to_string()
{
	json_object	*tmp;
	char		*buf;
	char		*ret;
	int		i;

	i = 0;
	ret = buf = 0x0;
	tmp = json_object_new_array();
	while (cmds[i].name)
	{
		json_object_array_put_idx(tmp, i, json_object_new_string(cmds[i].name));
		i++;
	}

	buf = strdup(json_object_to_json_string(tmp));
	json_object_put(tmp);
	ret = (char *) malloc(strlen(CMD_SUCCESS) + 1 + strlen(buf));
	if (ret) {
		sprintf(ret, "%s%c%s", CMD_SUCCESS, CMD_SEP, buf);
	} else {
		ret = CMD_FAILURE"|"ISE_ERR;
	}
	free(buf);
	buf = 0x0;
	return (ret);
}

char			*exec_command(char *cmd, char *player_id)
{
	char	*ret;
	char	**tmp;
	int	i;

	tmp = 0x0;
	ret = 0x0;
	i = 0;

	if (!cmd)
		return (CMD_FAILURE"|"ISE_ERR);

	tmp = split(cmd, CMD_SEP);

	if (!tmp)
		return (CMD_FAILURE"|"ISE_ERR); //Internal Server Error if the function can't return an array

	while (1)
	{
		if (!cmds[i].name)
			break;
		if (strncmp(cmds[i].name, cmd, strlen(cmds[i].name)) == 0)
		{
			ret = strdup(cmds[i].cmd(tmp, player_id));
			break;
		}
		i++;
	}
	if (!ret)
		ret = strdup(CMD_FAILURE"|"NO_CMD);
	i = 0;
	while (*(tmp + i++))
		free(*(tmp + i));
	free(tmp);
	return (ret);
}

player_t		*is_id_taken(t_list *players, char *id)
{
	t_link		*iterator;
	t_link		*base;
	player_t	*found;

	base = players->first;
	iterator = players->first;
	while (iterator)
	{
		found = (player_t *) iterator->value;
		if (!strcmp(found->id, id))
		{
			players->first = base;
			return (found);
		}
		iterator = iterator->next;
	}
	players->first = base;
	return (0x0);
}

char			*attack(char **cmd, char *player_id)
{
	char		*ret;
	char		**watch_res;
	player_t	*p;
	player_t	*v;
	int		i;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	watch_res = 0x0;
	p = v = 0x0;
	i = 0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->action < HALF_ACT_COST) {
		ret = CMD_FAILURE"|"COST_ERR;
	} else {
		watch_res = get_watch_view(m->players, m->energy_cells, p->x, p->y, p->looking);
		while (i++ < 4)
		{
			if ((strcmp(*(watch_res + i), "error") != 0) && (strcmp(*(watch_res + i), "wall") != 0) && (strcmp(*(watch_res + i), "energy") != 0) && (strcmp(*(watch_res + i), "empty") != 0))
			{
				v = is_id_taken(m->players, *(watch_res + i));
				v->disabled = 2;
			}
		}
		p->energy--;
		p->action -= HALF_ACT_COST;
		ret = CMD_SUCCESS"|"NO_RET;
	}
	return (ret);
}

char			*watch(char **cmd, char *player_id)
{
	char		*ret;
	char		*watch_res;
	char		**tmp;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	tmp = 0x0;
	ret = watch_res = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		tmp = get_watch_view(m->players, m->energy_cells, p->x, p->y, p->looking);
		watch_res = arr_to_json_string(tmp);
		ret = (char *) malloc(strlen(CMD_SUCCESS) + strlen(watch_res) + 1);
		if (ret && watch_res) {
			sprintf(ret, "%s%c%s", (watch_res) ? CMD_SUCCESS : CMD_FAILURE, CMD_SEP, (watch_res) ? watch_res : NO_RET);
		} else {
			ret = CMD_FAILURE"|"NO_RET;
		}
	}
	free(watch_res);
	return (ret);
}

char			*gather(char **cmd, char *player_id) {
	char		*ret;
	player_t	*p;
	t_link		*link;
	game_instance_t	*m;

	(void) cmd;
	link = 0x0;
	ret = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < FULL_ACT_COST) {
			ret = CMD_FAILURE"|"COST_ERR;
		} else if (is_energy_cell_in(m->energy_cells, p->x, p->y)) {
			link = get_energy_by_coords(m->energy_cells, p->x, p->y);
			if (link) {
				p->action -= FULL_ACT_COST;
				p->energy += ((EnergyCell_t *) link->value)->value;
				remove_list(&m->energy_cells, link);
				ret = CMD_SUCCESS"|"NO_RET;
			} else {
				ret = CMD_FAILURE"|"NO_RET;
			}
		} else {
			ret = CMD_FAILURE"|"NO_RET;
		}
	}
	return (ret);
}

char			*jump(char **cmd, char *player_id) {
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->energy < 2) {
			ret = CMD_FAILURE"|"LOW_ENERGY;
		} else {
			switch (p->looking) {
				case LEFT:
					if (can_move(m->players, m->energy_cells, p->x - 2, p->y)) {
						p->x -= 2;
						p->energy -= 2;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case RIGHT:
					if (can_move(m->players, m->energy_cells, p->x + 2, p->y)) {
						p->x += 2;
						p->energy -= 2;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case UP:
					if (can_move(m->players, m->energy_cells, p->x, p->y - 2)) {
						p->y -= 2;
						p->energy -= 2;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case DOWN:
					if (can_move(m->players, m->energy_cells, p->x, p->y + 2)) {
						p->y += 2;
						p->energy -= 2;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
			}
		}
	}
	return (ret);
}

char			*next(char **cmd, char *player_id) {
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		p->action = 0;
		ret = CMD_SUCCESS"|"NO_RET;
	}
	return (ret);
}

char			*leftfwd(char **cmd, char *player_id) {
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	ret = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < FULL_ACT_COST) {
			ret = CMD_FAILURE"|"NO_RET;
		} else {
			if (strncmp(left(cmd, player_id), CMD_SUCCESS, strlen(CMD_SUCCESS))) {
				if (strncmp(forward(cmd, player_id), CMD_SUCCESS, strlen(CMD_SUCCESS))) {
					ret = CMD_SUCCESS"|"NO_RET;
				}
			} else {
				ret = CMD_FAILURE"|"NO_RET;
			}
		}
	}
	return (ret);
}

char			*rightfwd(char **cmd, char *player_id)
{
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	ret = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	}  else {
		if (p->action < FULL_ACT_COST ) {
			ret = CMD_FAILURE"|"NO_RET;
		} else {
			if (strncmp(right(cmd, player_id), CMD_SUCCESS, strlen(CMD_SUCCESS))) {
				if (strncmp(forward(cmd, player_id), CMD_SUCCESS, strlen(CMD_SUCCESS))) {
					ret = CMD_SUCCESS"|"NO_RET;
				}
			} else {
				ret = CMD_FAILURE"|"NO_RET;
			}
		}
	}
	return (ret);
}

char			*backward(char **cmd, char *player_id)
{
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < HALF_ACT_COST) {
			ret = CMD_FAILURE"|"COST_ERR;
		} else {
			switch (p->looking) {
				case LEFT:
					if (can_move(m->players, m->energy_cells, p->x + 1, p->y)) {
						p->x += 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;

				case RIGHT:
					if (can_move(m->players, m->energy_cells, p->x - 1, p->y)) {
						p->x -= 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;

				case UP:
					if (can_move(m->players, m->energy_cells, p->x, p->y + 1)) {
						p->y += 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;

				case DOWN:
					if (can_move(m->players, m->energy_cells, p->x, p->y - 1)) {
						p->y -= 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
			}
		}
	}
	return (ret);
}

char			*forward(char **cmd, char *player_id)
{
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < HALF_ACT_COST) {
			ret = CMD_FAILURE"|"COST_ERR;
		} else {
			switch (p->looking) {
				case LEFT:
					if (can_move(m->players, m->energy_cells, p->x - 1, p->y)) {
						p->x -= 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case RIGHT:
					if (can_move(m->players, m->energy_cells, p->x + 1, p->y)) {
						p->x += 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case UP:
					if (can_move(m->players, m->energy_cells, p->x, p->y - 1)) {
						p->y -= 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
				case DOWN:
					if (can_move(m->players, m->energy_cells, p->x, p->y + 1)) {
						p->y += 1;
						p->action -= HALF_ACT_COST;
						ret = CMD_SUCCESS"|"NO_RET;
					} else {
						ret = CMD_FAILURE"|"NO_RET;
					}
				break;
			}
		}
	}
	return (ret);
}

char			*looking(char **cmd, char *player_id)
{
	char		*ret;
	player_t	*p;
	game_instance_t	*m;

	(void) cmd;
	ret = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	// Does the player is in list ?
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		ret = (char *) malloc(strlen(CMD_SUCCESS) + 2);
		if (!ret)
			ret = CMD_FAILURE"|"ISE_ERR;
		else
			sprintf(ret, "%s%c%d", CMD_SUCCESS, CMD_SEP, p->looking);
	}
	return (ret);
}


char			*left(char **cmd, char *player_id)
{
	char		*ret;
	game_instance_t	*m;
	player_t	*p;

	(void) cmd;
	ret = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < HALF_ACT_COST) {
			ret = CMD_FAILURE"|"COST_ERR;
		} else {
			p->action -= HALF_ACT_COST;
			p->looking = (p->looking == LEFT) ? DOWN : (p->looking - 1);
			ret = CMD_SUCCESS"|"NO_RET;
		}
	}
	return (ret);
}

char			*right(char **cmd, char *player_id)
{
	char		*ret;
	game_instance_t	*m;
	player_t	*p;

	(void) cmd;
	ret = 0x0;
	p = 0x0;
	m = get_instance();
	p = is_id_taken(m->players, player_id);
	if (!p) {
		ret = CMD_FAILURE"|"NO_RET;
	} else if (p->disabled != 0) {
		ret = CMD_FAILURE"|"NO_RET;
	} else {
		if (p->action < HALF_ACT_COST) {
			ret = CMD_FAILURE"|"COST_ERR;
		} else {
			p->action -= HALF_ACT_COST;
			p->looking = (p->looking == DOWN) ? LEFT : (p->looking + 1);
			ret = CMD_SUCCESS"|"NO_RET;
		}
	}
	return (ret);

}

char			*inspect(char **cmd, char *player_id)
{
	player_t	*target;
	player_t	*player;
	game_instance_t	*manager;
	char		*ret;

	target = player = 0x0;
	ret = 0x0;
	manager = get_instance();
	player = is_id_taken(manager->players, player_id);
	target = is_id_taken(manager->players, cmd[1]); // Trying to found the player from request argument
	if (target && player)
	{
		if ((player->action - HALF_ACT_COST) > 0) {
			player->action -= HALF_ACT_COST;
			ret = (char *) malloc((strlen(CMD_SUCCESS) + 4) + 1);
			sprintf(ret, "%s%c%d", CMD_SUCCESS, CMD_SEP, target->energy);
		} else {
			ret = (char *) malloc(strlen(CMD_FAILURE) + strlen(COST_ERR) + 1);
			sprintf(ret, "%s%c%s", CMD_FAILURE, CMD_SEP, COST_ERR);
		}
	}
	else
	{
		ret = (char *) malloc((strlen(CMD_FAILURE) + strlen(NO_RET)) + 1);
		sprintf(ret, "%s%c%s", CMD_SUCCESS, CMD_SEP, NO_RET);
	}
	return (ret);
}

char			*selfstats(char **cmd, char *player_id)
{
	(void) cmd;
	player_t	*player;
	game_instance_t	*manager;
	char		*ret;

	player = 0x0;
	ret = 0x0;
	manager = get_instance();
	player = is_id_taken(manager->players, player_id);
	if (player)
		ret = (char *) malloc((strlen(CMD_SUCCESS) + 4) + 1);
	else
		ret = (char *) malloc((strlen(CMD_FAILURE) + 4) + 1);
	sprintf(ret, "%s%c%d", (player) ? CMD_SUCCESS : CMD_FAILURE, CMD_SEP, (player) ? player->energy : 0);
	return (ret);
}

char			*selfid(char **cmd, char *player_id)
{
	(void) cmd;
	player_t	*player;
	game_instance_t	*manager;
	char		*ret;

	player = 0x0;
	ret = 0x0;
	manager = get_instance();
	player = is_id_taken(manager->players, player_id);
	if (player)
		ret = (char *) malloc((strlen(CMD_SUCCESS) + strlen(player->id)) + 1);
	else
		ret = (char *) malloc((strlen(CMD_FAILURE) + 4) + 1);

	if (ret)
		sprintf(ret, "%s%c%s", (player) ? CMD_SUCCESS : CMD_FAILURE, CMD_SEP, (player) ? player->id : "null");
	return (ret);
}

char			*identify(char **cmd)
{
	player_t	*new_player;
	game_instance_t *manager;

	new_player = 0x0;
	manager = get_instance();

	char *ret;
	ret = 0x0;

	if (manager->players->index >= 4)
	{
		ret = (char *) malloc((strlen(CMD_FAILURE) + strlen(GAME_FULL) + 1));
		sprintf(ret, "%s%c%s", CMD_FAILURE, CMD_SEP, GAME_FULL);
		return (ret);
	}

	if ((is_id_taken(manager->players, cmd[1])))
	{
		my_logger(REGULAR_LOG, "ID %s is already taken ! (%s)\n", cmd[1], __func__);
		ret = (char *) malloc((strlen(CMD_FAILURE) + strlen(ID_TAKEN)) + 1);
		sprintf(ret, "%s%c%s", CMD_FAILURE, CMD_SEP, ID_TAKEN);
		return (ret);
	}
	else
	{
		t_list *l = manager->players;
		new_player = ctor_player(cmd[1], manager->players->index + 1);
		add_to_list(&l, new_player);
	}

	ret = (char *) malloc((strlen(CMD_SUCCESS) + strlen(NO_RET)) + 1);
	sprintf(ret, "%s%c%s", (new_player) ? CMD_SUCCESS : CMD_FAILURE, CMD_SEP, NO_RET);
	return (ret);
}

