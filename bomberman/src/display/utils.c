#include	<unistd.h>
#include	<arpa/inet.h>
#include	<sys/socket.h>
#include	<sys/types.h>
#include	<stdlib.h>

#include	<proto.h>
#include	<utils.h>

int			connect_to_server(t_window *win, char *host, char *port)
{
	int		sock;
	t_response_connection c;
	struct sockaddr_in con;
	struct timeval tv;

	/* Timeout: 2.5s */
	tv.tv_sec = 2;
	tv.tv_usec = 5;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return (-1);
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *) &tv, sizeof(struct timeval));
	con.sin_family = AF_INET;
	con.sin_port = htons(atoi(port));
	inet_pton(AF_INET, host, &con.sin_addr.s_addr);
	if (connect(sock, (struct sockaddr *) &con, sizeof(con)) < 0)
	{
		close(sock);
		return (-1);
	}
	win->sock = sock;
	if (recv(win->sock, &c, sizeof(c), MSG_WAITALL) < 0)
		perror("connect_to_server recv");
	else
	{
		win->id = c.player_num;
		printf("Connected as player %d\n", win->id);
	}
	return (sock);
}

int			wait_until_game_start(t_window *win)
{
	int		ready;
	t_message t = { 0, 0, 0, GET_PLAYERS, 0, 0, 0 };
	t_response_players res;

	ready = 0;
	send(win->sock, &t, sizeof(t), 0);
	recv(win->sock, &res, sizeof(res), 0);
	if (res.cmd_status != NOT_STARTED)
		ready = 1;
	return ready;
}

t_response_status		get_map(t_window *win)
{
	t_message msg = { 0, 0, 0, GAME_STATUS, 0, 0, 0 };
	t_response_status st;
	int i;

	for (i = 0; i < 256; i++)
		memset(&st.map[i], 0, sizeof(st.map));
	send(win->sock, &msg, sizeof(msg), 0);
	if (recv(win->sock, &st, sizeof(st), MSG_WAITALL) < 0)
		perror("get_map recv");
	st.type = GAME_STATUS;
	st.cmd_status = SUCCESS;
	st.map_size = st.map_size;
	st.status = st.status;
	return (st);
}