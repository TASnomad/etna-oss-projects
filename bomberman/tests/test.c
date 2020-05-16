#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<arpa/inet.h>
#include	<sys/types.h>
#include	<sys/socket.h>

#include	<proto.h>
#include	<response.h>
#include	<player.h>

// void		get_status(int, int);

int				__connect(const char *ip, int port)
{
	int			sock;
	struct sockaddr_in srv;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("(__connect) socket");
		return (-1);
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	inet_pton(AF_INET, ip, &srv.sin_addr.s_addr);
	if (connect(sock, (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		perror("(__connect) connect");
		close(sock);
		return (-1);
	}
	return (sock);
}

int			get_player_id(int sock)
{
	int		id;
	t_response_connection	c;

	id = -1;
	if (recv(sock, &c, sizeof(c), 0) < 0)
		perror("get_player_id recv");
	else
		id = c.player_num;
	return (id);
}

void		__wait(int sock)
{
	t_message t = { 0, 0, 0, 0, GET_PLAYERS, 0, 0 };
	t_response_players p;

	write(sock, &t, sizeof(t));
	read(sock, &p, sizeof(p));
	puts("Waiting ...\n");
	if (p.cmd_status == NOT_STARTED)
	{
		sleep(1);
		__wait(sock);
	}
}

void		move_right(int sock, t_player p)
{
	t_response_players 	tmp;
	t_message t = { 0, p._x, p._y, p.direction, MOVE_RIGHT, 0, 0 };
	t_response_status	st;
	write(sock, &t, sizeof(t));
	recv(sock, &tmp, sizeof(t_response_players), 0);
	recv(sock, &st, sizeof(t_response_status), 0);
	printf("__TEST__(cmd %d) [%s]\n", t.cmd, tmp.cmd_status == SUCCESS ? "OK" : "KO");
	// get_status(sock, p.num);
}

t_player		get_status(int sock, int player_id)
{
	t_response_players pl = { 0 };
	t_response_status st = { 0 };
	t_player			p = { 0 };

	if (recv(sock, &pl, sizeof(pl), 0) < 0)
		perror("get_status");
	else
		printf("[%s]\n", pl.cmd_status == SUCCESS ? "OK" : "KO");
	recv(sock, &st, sizeof(t_response_status), 0);
	if (pl.cmd_status == SUCCESS)
	{
		p = pl.players[player_id];
		move_right(sock, pl.players[player_id]);
	}
	else
		puts("Can't find ourselves\n");
	return p;
}


int				main(int argc, const char **argv)
{
	int sock;
	int	player_id;
	int	i;

	i = 0;
	sock = __connect("127.0.0.1", 8081);
	if (sock < 0)
		return (-1);

	if (argc == 2 && !strcmp(*(argv + 1), "idle"))
	{
		pause();
		return (1);
	}

	player_id = get_player_id(sock);
	if (player_id < 0)
		return (-1);
	printf("Connected with ID %d\n", player_id);
	__wait(sock);
	t_player p = get_status(sock, player_id);
	while (i < 4)
	{
		move_right(sock, p);
		// get_status(sock, player_id);
		i++;
	}
	close(sock);
	return (0);
}
