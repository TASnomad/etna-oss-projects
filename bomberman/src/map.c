#include	<map.h>
#include	<proto.h>
#include	<player.h>
#include	<window.h>

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<assert.h>
#include	<unistd.h>

#define __discard	__attribute__((unused))

#define		CELL_BY_COORD(map, x, y, width)	(map[(x) * width + y])

int		*ctor_map(int *map, int size)
{
	int		i;
	int		j;

	/* NOTE: the map is always like a square */
	// cell = 0;
	i = 0;
	j = 0;
	map = (int *) realloc(map, size * sizeof(int) + 1);
	memset(map, 0, size * sizeof(int) + 1);
	if (!map)
		return (NULL);

	/* Solid blocks on map boundaries */
	for (i = 0; i < MAP_SIZE; i++)
	{
		if (i < MAP_SIZE) map[i] = SOLID_WALL;
		else if (i % MAP_SIZE == 0) map[i] = SOLID_WALL;
		else if (i % (MAP_SIZE -1) == 0) map[i] = SOLID_WALL;
		else if (i > ( size - MAP_SIZE)) map[i] = SOLID_WALL;
	}
	/* TODO: generate random blocks */
	return (map);
}

static int		is_in_wall(t_server *srv, int x, int y)
{
	int		cell;

	cell = CELL_BY_COORD(srv->map, x, y, MAP_SIZE);
	// TODO: can we move in a bomb / a player?
	return (IS_WEAK_WALL(cell) || IS_SOLID_WALL(cell) || HAS_BOMB(cell));
}

void			set_player_coords(t_server *srv, int num)
{
	switch (num)
	{
		case 0:
			srv->players[num]._x = 1;
			srv->players[num]._y = 1;
			srv->players[num].direction = LEFT;
			break;
		case 1:
			srv->players[num]._x = MAP_SIZE - 1;
			srv->players[num]._y = 1;
			srv->players[num].direction = DOWN;
			break;
		case 2:
			srv->players[num]._x = 2;
			srv->players[num]._y = 2;
			srv->players[num].direction = UP;
			break;
		case 3:
			srv->players[num]._x = 3;
			srv->players[num]._y = 3;
			srv->players[num].direction = RIGHT;
			break;
	}
}

/* 0 means impossible */
int		move_player(t_server *srv, int player, t_direction move)
{
	int	_x;
	int	_y;

	if (!srv->players[player].connected || !srv->players[player].alive)
		return (0);
	switch (move)
	{
		case UP:
			_x = srv->players[player]._x;
			_y = srv->players[player]._y - 1;
			srv->players[player].direction = UP;
			break;
		case DOWN:
			_x = srv->players[player]._x;
			_y = srv->players[player]._y + 1;
			srv->players[player].direction = DOWN;
			break;
		case LEFT:
			_x = srv->players[player]._x - 1;
			_y = srv->players[player]._y;
			srv->players[player].direction = LEFT;
			break;
		case RIGHT:
			_x = srv->players[player]._x + 1;
			_y = srv->players[player]._y;
			srv->players[player].direction = RIGHT;
			break;
	}

	/* OOB check */
	if ((_x < 0 || _x > MAP_SIZE) || (_y < 0 || _y > MAP_SIZE))
		return (0);
	if (is_in_wall(srv, _x, _y))
		return (0);
	if (srv->players[player].alive)
	{
		srv->players[player]._x = _x;
		srv->players[player]._y = _y;
		// TODO: handle bonus / malus
		// cell = CELL_BY_COORD(srv->map, _x, _y);
		// if (HAS_BONUS_OR_MALUS(cell)) {
		// }
	}
	return (srv->players[player].alive);
}

int		plant_bomb(t_server *srv, int player)
{
	int			cell_idx;
	int	res;
	int		cell;
	t_player	*p;

	res = 0;
	cell = 0;
	p = &srv->players[player];
	if ((!p->connected) || (p->bombs < 1) || (!p->alive))
		return (res);
	cell_idx = p->_x * srv->map_size + p->_y;
	cell = srv->map[cell_idx];
	CLEAR_BOMB(cell);
	SET_BOMB_OWNER(cell, player);
	srv->map[cell_idx] = cell;
	p->bombs--;
	res = 1;
	// pthread_create(NULL, NULL, bomb_blast_handler, NULL);
	return (res);
}

// void	*bomb_blast_handler(void *bundle)
// {
// 	t_bundle	*t;

// 	t = (t_bundle *) bundle;
// 	if (!t)
// 		pthread_exit(NULL);
// 	sleep(3);
// 	LOCK_SRV(t->srv);
// 	// TODO: blast here
// 	// TODO: Fire cells around the blast
// 	// TODO: kill users here
// 	FIRE_CELL(t->srv->map[t->cell_idx]);
// 	sleep(1);
// 	CLEAR_BOMB(t->srv->map[t->cell_idx]);
// 	CLEAR_FIRE(t->srv->map[t->cell_idx]);
// 	UNLOCK_SRV(t->srv);
// 	// TODO: clear cells around
// 	return (NULL);
// }