#ifndef	__PROTO_H__
#define	__PROTO_H__

#include		<unistd.h>
#include		<pthread.h>

#include		<arpa/inet.h>
#include		<sys/types.h>
#include		<sys/time.h>
#include		<sys/select.h>
#include		<sys/socket.h>

#include		<player.h>

#ifdef __linux__
	#define	NO_SIG	MSG_NOSIGNAL
#else
	#define NO_SIG	SO_NOSIGPIPE
#endif

#define	IS_SERVER_FULL(srv)	(srv->players_count >= 4)
#define	CAN_START_GAME(srv)	(srv->players_count >= 2 && srv->players_count < 5)

#define MAX_CLIENT 4

#define LOCK_SRV(srv)	(pthread_mutex_lock(srv->lock))
#define UNLOCK_SRV(srv)	(pthread_mutex_unlock(srv->lock))

typedef enum		e_game_status
{
			WAITING_PLAYERS,
			RUNNING,
			FINISHED,
}			t_game_status;

typedef struct		s_server
{
	int		fd;
	int		max_clients;
	int		players_count;
	fd_set	fds;
	int		fd_len;
	int		*map;
	int		map_size;
	t_player	players[GAME_CAPACITY];
	int		status;
	pthread_mutex_t	*lock;
}			t_server;

typedef enum		e_req_status
{
	NOT_STARTED	= -2,
	BAD_CMD		= -1,
	SUCCESS		= 0,
}			t_req_status;

typedef enum		e_req_clt
{
	QUIT		= 0,
	CONNECTION,
	GET_PLAYERS,
	GAME_STATUS,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	PLANT_BOMB,
}			t_req_clt;

typedef struct		s_message
{
	unsigned int	magic;
	int		_x;
	int		_y;
	int		direction;
	int		cmd;
	int		speed;
	int		checksum;
}			t_message;

t_server		*ctor_server(char *);
char			**get_server_ip(t_server *);
void			dtor_server(t_server *);
void			add_player(t_server *, int);
int				server_loop(t_server *);
void			refresh_clts(t_server *);
int				is_socket_broken(int);
void			*server_thread_entry(void *);

#endif	/* !__PROTO_H__ */
