#ifndef __NETWORK_H__
#define __NETWORK_H__

// Definition of networking actions with the ZMQ ROUTER socket

#include	<czmq.h>

zmsg_t		*forge_msg(zmsg_t *, const char *);

int		serve_game();
int		wait_players();

#endif /* ! __NETWORK_H__ */
