#include		<cmd.h>
#include		<proto.h>
#include		<map.h>
#include		<response.h>

#include		<errno.h>
#include		<string.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<signal.h>
#include		<pthread.h>
#include		<signal.h>
#include		<ifaddrs.h>

#define			MAX_CLIENT 4

void			init_players(t_server *srv)
{
	int		i;

	i = 0;
	while (i < 4)
	{
		srv->players[i].alive = 0;
		srv->players[i].connected = 0;
		srv->players[i].num = i;
		srv->players[i]._x = 0;
		srv->players[i]._y = 0;
		srv->players[i].direction = 0;
		srv->players[i].speed = 0;
		srv->players[i].bombs = 0;
		srv->players[i].max_bombs = 0;
		srv->players[i].frags = 0;
		srv->players[i].fd = -1;
		i++;
	}
}

char			**get_server_ip(t_server *srv)
{
	char			**res;
	struct sockaddr_in	addr;
	struct ifaddrs	*desc;
	struct ifaddrs	*saveDesc;
	socklen_t		l;
	int				i;

	if (!srv)
		return (NULL);

	i = 0;
	desc = NULL;
	res = NULL;
	l = sizeof(addr);
	getsockname(srv->fd, (struct sockaddr *) &addr, &l);
	getifaddrs(&desc);
	if (!desc)
		return (NULL);
	saveDesc = desc;
	while (desc)
	{
		if (desc->ifa_addr && desc->ifa_addr->sa_family == AF_INET)
		{
			i++;
		}
		desc = desc->ifa_next;
	}

	res = (char **) malloc((i + 1) * sizeof(char *));
	if (!res)
	{
		freeifaddrs(desc);
		return (NULL);
	}

	i = 0;
	desc = saveDesc;

	while (desc)
	{
		if (desc->ifa_addr && desc->ifa_addr->sa_family == AF_INET)
		{
			*(res + i) = strdup(inet_ntoa(((struct sockaddr_in *) desc->ifa_addr)->sin_addr));
			i++;
		}
		desc = desc->ifa_next;
	}
	desc = saveDesc;
	*(res + i) = NULL;
	freeifaddrs(desc);
	return (res);
}

t_server		*ctor_server(char *port)
{
	t_server		*server;
	int			opts;
	struct sockaddr_in	srv;

	server = (t_server *) malloc(sizeof(t_server));
	if (!server)
		return (NULL);
	server->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	if (!server->lock)
	{
		free(server);
		return (NULL);
	}
	server->fd = -1;
	server->map = NULL;
	server->max_clients = MAX_CLIENT;
	server->players_count = 0;
	opts = 1;

	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_port = htons(atoi(port));
	server->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->fd < 0) {
		perror("ctor_server socket:");
		free(server);
		return (NULL);
	}
	if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(int)) < 0)
		perror("ctor_server setsockopt:");
	if (bind(server->fd, (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		perror("ctor_server bind:");
		close(server->fd);
		free(server);
		return (NULL);
	}
	if (listen(server->fd, FD_SETSIZE) < 0)
	{
		perror("ctor_server listen:");
		close(server->fd);
		free(server);
		return (NULL);
	}
	init_players(server);
	server->map_size = MAP_SIZE * MAP_SIZE;
	server->map = ctor_map(server->map, server->map_size);
	server->fd_len = server->fd;
	FD_ZERO(&server->fds);
	FD_SET(server->fd, &server->fds);
	server->status = WAITING_PLAYERS;
	pthread_mutex_init(server->lock, NULL);
	return (server);
}

void			dtor_server(t_server *srv)
{
	int			i;

	i = 0;
	if (srv)
	{
		if (srv->map)
		{
			free(srv->map);
		}
		while (i < srv->max_clients)
		{
			if (srv->players[i].connected)
				close(srv->players[i].fd);
			i++;
		}
		close(srv->fd);
		free(srv);
	}
}

void			add_player(t_server *srv, int fd)
{
	t_player	p;
	t_response_connection c;
	// socklen_t	t;

	p.alive = 1;
	p.connected = 1;
	p.bombs = 1;
	p.fd = fd;
	p.num = srv->players_count;
	p._x = 0;
	p._y = 0;
	p.direction = DOWN;
	/* TODO: set more fields in player struct */
	srv->players[srv->players_count] = p;
	srv->players_count++;
	set_player_coords(srv, p.num);
	c.type = CONNECTION;
	c.player_num = p.num;
	printf("Player %d added\n", p.num);
	send(fd, &c, sizeof(c), NO_SIG);
}

void			refresh_clts(t_server *srv)
{
	int		i;

	i = 0;
	while (i < 4)
	{
		if (srv->players[i].connected == 1)
		{
			FD_SET(srv->players[i].fd, &srv->fds);
			if (srv->players[i].fd > srv->fd_len)
				srv->fd_len = srv->players[i].fd;
		}
		i++;
	}
}

int			is_socket_broken(int fd)
{
	int		res;
	int		ret;
	int		err;
	socklen_t	t;

	t = sizeof(err);
	ret = 0;
	err = 0;
	res = 0;
	ret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &t);
	if (ret != 0 || err != 0)
	{
		printf("%s %s\n", __func__, strerror(err));
		res = 1;
	}
	return (res);
}

int			server_loop(t_server *srv)
{
	int		i;
	int		clt;
	int		res;
	socklen_t	tmp;
	struct sockaddr	rep;
	struct timeval	idle;
	t_response_error err_res;

	tmp = 0;
	idle.tv_sec = 0;
	idle.tv_usec = 100000;
	FD_ZERO(&srv->fds);
	FD_SET(srv->fd, &srv->fds);
	refresh_clts(srv);

	res = select(srv->fd_len + 1, &srv->fds, NULL, NULL, &idle);
	if (res < 0)
	{
		perror("server_loop select:");
		return (0);
	}
	if (!IS_SERVER_FULL(srv))
	{
		if (FD_ISSET(srv->fd, &srv->fds) != 0)
		{
			clt = accept(srv->fd, &rep, &tmp);
			if (clt < 0)
			{
				perror("server_loop accept");
				return (0);
			}
			add_player(srv, clt);
		}
	}
	if (CAN_START_GAME(srv) && srv->status != RUNNING)
	{
		puts("Starting game!\n");
		srv->status = RUNNING;
		/* NOTE: maybe we should not send the game status here */
		/*for (i = 0; i < srv->players_count; i++)
			get_game_status(srv, i);*/
		// return (1);
	}

	// comment this control flow to debug the server
	// if (srv->players_count < 2 && srv->status == RUNNING)
	// 	return (0);

	for (i = 0; i < 4; i++)
	{
		if (!srv->players[i].connected)
			continue;
		if (is_socket_broken(srv->players[i].fd))
		{
			printf("Connection with %d has an error\n", i);
			srv->players[i].connected = 0;
			// comment this control flow to debug the server
			// srv->players_count--;
			continue;
		}
		if (FD_ISSET(srv->players[i].fd, &srv->fds) != 0)
		{
			t_message t;
			int rbytes = recv(srv->players[i].fd, &t, sizeof(t_message), MSG_WAITALL);
			if (rbytes > 0)
			{
				printf("(%s) player %d asking cmd %d\n", __func__, i, t.cmd);
				if (srv->status != RUNNING)
				{
					printf("(%s) ignoring cmd %d from %d player: game not started\n", __func__, i, t.cmd);
					err_res.type = t.cmd;
					err_res.error = NOT_STARTED;
					err_res.cmd_status = NOT_STARTED;
					send(srv->players[i].fd, &err_res, sizeof(t_response_error), NO_SIG);
					continue;
				}
				int cmd_res = run_cmd(srv, i, t);
				if (cmd_res == BAD_CMD)
					printf("(%s) cmd %d failed for %d\n", __func__, t.cmd, i);
				get_game_status(srv, i, GAME_STATUS);
			}
		}
	}
	return (1);
}

void		*server_thread_entry(void *arg)
{
	int				run;

	t_server			*srv;
	run = 1;
	srv = (t_server *) arg;
	if (!srv)
		return (NULL);
	while (run)
		run = server_loop(srv);
	printf("Server teardown\n");
	dtor_server(srv);
	return (srv);
}
