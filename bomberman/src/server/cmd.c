#include	<cmd.h>
#include	<map.h>
#include	<proto.h>
#include	<response.h>

#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>

#define		__discard	__attribute__((unused))

static const t_cmd_runner cmds[] =
{
	{ QUIT, &player_quit },
	{ GET_PLAYERS, &get_players },
	{ GAME_STATUS, &get_game_status },
	{ MOVE_UP, &move_player_cmd },
	{ MOVE_DOWN, &move_player_cmd },
	{ MOVE_LEFT, &move_player_cmd },
	{ MOVE_RIGHT, &move_player_cmd },
	{ PLANT_BOMB, &add_bomb },
	{ 0x0, 0x0 }
};

int		player_quit(t_server *srv, int player, __discard t_req_clt cmd)
{
	t_response_players	p;
	t_player	t;
	int			i;

	t = srv->players[player];
	p.type = QUIT;
	if (!t.connected)
		return (0);
	t.alive = 0;
	t.connected = 0;
	for (i = 0; i < srv->players_count; i++)
		p.players[i] = srv->players[i];
	if (send(t.fd, &p, sizeof(p) + 1, NO_SIG) < 0)
	{
		perror("players_count write");
		return (0);
	}
	return (1);
}

int		get_players(t_server *srv, int player, __discard t_req_clt cmd)
{
	t_response_players	p;
	int			i;

	p.type = GET_PLAYERS;
	p.cmd_status = SUCCESS;
	p.players_count = srv->players_count;
	for (i = 0; i < srv->players_count; i++)
		p.players[i] = srv->players[i];
	if (send(srv->players[player].fd, &p, sizeof(p) + 1, NO_SIG) < 0)
	{
		perror("get_players write");
		return (0);
	}
	return (1);
}

int		get_game_status(t_server *srv, int player, __discard t_req_clt cmd)
{
	t_response_status	st;

	if (!srv->players[player].connected)
		return (0);

	st.type = GAME_STATUS;
	st.cmd_status = SUCCESS;
	st.map_size = srv->map_size;
	st.status = srv->status;
	memset(st.map, 0, sizeof(st.map));
	memcpy(st.map, srv->map, st.map_size);
	printf("%s\n", __func__);
	if (send(srv->players[player].fd, (void *) &st, sizeof(st), NO_SIG) < 0)
	{
		perror("get_game_status write");
		return (0);
	}
	return (1);
}

int			move_player_cmd(t_server *srv, int player, t_req_clt cmd)
{
	int			i;
	int			res;
	t_response_players	p;
	t_direction		req;

	res = -1;
	switch (cmd)
	{
		case MOVE_UP:
			req = UP;
			break;
		case MOVE_DOWN:
			req = DOWN;
			break;
		case MOVE_LEFT:
			req = LEFT;
			break;
		case MOVE_RIGHT:
			req = RIGHT;
			break;
		default:
			req = -1;
			break;
	}
	p.type = cmd;
	if (req != (t_direction) -1)
	{
		res = move_player(srv, player, req);
		p.cmd_status = !res ? BAD_CMD : SUCCESS;
	}
	else
		p.cmd_status = BAD_CMD;
	for (i = 0; i < 4; i++)
		p.players[i] = srv->players[i];
	if (send(srv->players[player].fd, &p, sizeof(p), NO_SIG) < 0)
	{
		perror("move_player_up write");
		return (0);
	}
	return (res);
}

int		add_bomb(t_server *srv, int player, __discard t_req_clt cmd)
{
	t_response_error err;

	err.cmd_status = BAD_CMD;
	err.error = BAD_CMD;
	err.type = ADD_BOMB;
	if (send(srv->players[player].fd, &err, sizeof(err) + 1, NO_SIG) <0)
	{
		perror("add_bomb write");
		return (0);
	}
	return (BAD_CMD);
}

int		run_cmd(t_server *srv, int id, t_message req)
{
	int	res;
	int	i;

	i = 0;
	res = BAD_CMD;
	if (is_socket_broken(srv->players[id].fd))
		return (res);
	while (cmds[i].handler)
	{
		if (cmds[i].cmd == (t_req_clt) req.cmd)
		{
			res = cmds[i].handler(srv, id, (t_req_clt) req.cmd);
			return (res);
		}
		i++;
	}
	return (res);
}
