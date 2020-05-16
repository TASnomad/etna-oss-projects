#ifndef __GAME_CTX_H__
#define	__GAME_CTX_H__

// ZMQ sockets bundle definitions
// This structs helds the ROUTER, PUB sockets & the zpoller

#include		<czmq.h>

typedef	struct		s_socks
{
	unsigned int	rep_port;
	unsigned int	pub_port;

	zsock_t		*rep_sock;
	zsock_t		*pub_sock;

	zpoller_t	*poll;
}			t_socks;

t_socks			*init_sockets(int, int);
void			clean_sockets(t_socks *);

int			wait_msg(t_socks *);

#endif /* ! __GAME_CTX_H__ */
