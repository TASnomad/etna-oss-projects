#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<czmq.h>

#include	<gmanager.h>
#include	<game_ctx.h>
#include	<network.h>

t_socks		*init_sockets(int rep, int pub)
{
	t_socks	*socks;

	socks = (t_socks *) malloc(sizeof(t_socks));
	if (!socks)
	{
		my_logger(VERBOSE_LOG, "Can't allocate memory (%s)\n", __func__);
		return (0x0);
	}

	socks->rep_sock = zsock_new(ZMQ_ROUTER);
	zsock_bind(socks->rep_sock, "tcp://*:%d", rep);
	my_logger(VERBOSE_LOG, "Router socket listening port: %d\n", rep);

	socks->pub_sock = zsock_new(ZMQ_PUB);
	zsock_bind(socks->pub_sock, "tcp://*:%d", pub);
	my_logger(VERBOSE_LOG, "Pub socket listening port: %d\n", pub);

	socks->rep_port = rep;
	socks->pub_port = pub;
	socks->poll = zpoller_new(0x0);
	zpoller_add(socks->poll, socks->rep_sock);
	my_logger(VERBOSE_LOG, "zpoller created (%s)\n", __func__);
	return (socks);
}

void		clean_sockets(t_socks *bundle)
{
	if (!bundle)
	{
		my_logger(VERBOSE_LOG, "Can't clean sockets with null pointer at:%s\n", __func__);
		return ;
	}
	zsock_destroy(&bundle->rep_sock);
	zsock_destroy(&bundle->pub_sock);

	my_logger(VERBOSE_LOG, "Router & Pub sockets are destroyed (%s)\n", __func__);
	free(bundle);
}

int		wait_msg(t_socks *socks)
{
	zsock_t *active_sock;

	active_sock = (zsock_t *) zpoller_wait(socks->poll, -1);
	if (zpoller_expired(socks->poll) == true)
		return (-1);
	if (zpoller_terminated(socks->poll) == true)
		return (-1);


	my_logger(VERBOSE_LOG, "Router socket is active at: %s\n", __func__);
	zmsg_t *msg;

	msg = 0x0;
	msg = zmsg_recv(active_sock);

	if (!msg)
	{
		my_logger(VERBOSE_LOG, "No message retrieved at: %s\n", __func__);
		return (-1);
	}
	zmsg_print(msg);

	zmsg_t *to_send = forge_msg(msg, "This is a router test");

	zmsg_send(&to_send, active_sock);
	return (0);
}

