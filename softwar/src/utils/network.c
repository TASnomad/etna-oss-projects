#include		<czmq.h>
#include		<pthread.h>

#include		<network.h>
#include		<command.h>
#include		<game_ctx.h>
#include		<gmanager.h>
#include		<player.h>
#include		<cycle.h>
#include		<list.h>

#define			GET_MSG_AND_ID(raw, id, content)	\
	id = extract_id(raw);					\
	content = get_msg_content(raw);

#define			CLEAN_ZMSGS(in, out)			\
	zmsg_destroy(in);					\
	zmsg_destroy(out);					\

#define			CLEAN_MSG_CONTENT(in, out)		\
	free(in);						\
	free(out);						\
	in = out = 0x0;

static	void		delete_player_on_failure(game_instance_t *m, char *id)
{
	player_t	*p;
	t_link		*link;

	link = 0x0;
	p = 0x0;
	p = is_id_taken(m->players, id);
	if (p) {
		link = get_link_by_value(&m->players, p);
		my_logger(REGULAR_LOG, "Deleting player with %s due to zmsg_send error (%s)\n", id, __func__);
		remove_list(&m->players, link);
	}
}

static	char 		*get_msg_content(zmsg_t *msg)
{
	zmsg_t		*clone;
	char		*ret;

	clone = 0x0;
	ret = 0x0;
	clone = zmsg_dup(msg);
	zmsg_popstr(clone);
	zmsg_pop(clone);
	ret = zmsg_popstr(clone);
	zmsg_destroy(&clone);
	return (ret);
}

static	char 		*extract_id(zmsg_t *msg)
{
	zmsg_t		*clone;
	char		*ret;

	clone = 0x0;
	ret = 0x0;
	clone = zmsg_dup(msg);
	ret = zmsg_popstr(clone);
	zmsg_destroy(&clone);
	return (ret);
}

zmsg_t			*forge_msg(zmsg_t *recv, const char *msg)
{
	zmsg_t		*ret;
	zmsg_t		*tmp;
	zframe_t	*zid;

	ret = tmp = 0x0;
	zid = 0x0;
	ret = zmsg_new();
	tmp = zmsg_dup(recv);
	zid = zmsg_pop(tmp);
	zmsg_pop(tmp);
	zmsg_prepend(ret, &zid);
	zmsg_add(ret, zframe_new_empty());
	zmsg_addstr(ret, msg);
	zframe_destroy(&zid);
	return (ret);
}

int			serve_game()
{
	game_instance_t	*manager;
	pthread_t	thread;
	zmsg_t		*in;
	zmsg_t		*out;
	zsock_t		*sock;
	char		*in_msg;
	char		*out_msg;
	char		*sock_id;
	int		rc;

	rc = 0;
	thread = 0x0;
	in = out = 0x0;
	out_msg = in_msg = 0x0;
	manager = get_instance();
	manager->cycle_thread_id = pthread_self();
	if (wait_players() < 0) {
		my_logger(REGULAR_LOG, "System interruption before the party began (%s)\n", __func__);
		return (-1);
	}
			// Waiting until the game room is full
	pthread_create(&thread, 0x0, tick_thread, 0x0);	// Starting cycle thread when all players are ready
	// Main loop for clients requests, run until the program received a SIGINT/SIGTERM signal
	while (!zsys_interrupted)
	{
		sock = (zsock_t *) zpoller_wait(manager->socks->poll, -1);

		if (zpoller_expired(manager->socks->poll) == true)
			return (-1);
		if (zpoller_terminated(manager->socks->poll) == true)
			return (-1);

		in = zmsg_recv(sock);
		if (!in)
		{
			my_logger(VERBOSE_LOG, "No message found from zpoller activity ! (%s)\n", __func__);
			continue;
		}
		GET_MSG_AND_ID(in, sock_id, in_msg);
		my_logger(VERBOSE_LOG, "\nPlayer %s is asking %s (%s)\n", sock_id, in_msg, __func__);
		out_msg = exec_command(strdup(in_msg), sock_id);
		out = forge_msg(in, out_msg);
		rc = zmsg_send(&out, sock);
		if (rc) {
			delete_player_on_failure(manager, sock_id);
		}
		CLEAN_ZMSGS(&in, &out);
		CLEAN_MSG_CONTENT(in_msg, out_msg);
		in = out = 0x0;
	}
	return (0);
}

int			wait_players()
{
	game_instance_t	*manager;
	t_list		*players;
	zmsg_t		*incoming;
	zmsg_t		*outgoing;
	zsock_t		*active_sock;
	char		*incoming_msg;
	char		*outgoing_msg;
	char		*sock_id;
	int		rc;

	rc = 0;
	incoming = outgoing = 0x0;
	outgoing_msg = 0x0;
	manager = get_instance();
	players = manager->players;
	while (count(players) < 4)
	{
		my_logger(VERBOSE_LOG, "%d players logged in the game (%s)\n", players->index, __func__);
		active_sock = (zsock_t *) zpoller_wait(manager->socks->poll, -1);

		if (zpoller_expired(manager->socks->poll) == true)
			return (-1);
		if (zpoller_terminated(manager->socks->poll) == true)
			return (-1);

		incoming = zmsg_recv(active_sock);

		if (!incoming)
		{
			my_logger(VERBOSE_LOG, "Socket activity detected, but no message found (%s)\n", __func__);
			continue;
		}
		GET_MSG_AND_ID(incoming, sock_id, incoming_msg);
		my_logger(REGULAR_LOG, "Sock ID => [%s] command: %s (%s)\n", sock_id, incoming_msg, __func__);
		outgoing_msg = exec_command(strdup(incoming_msg), sock_id);
		outgoing = forge_msg(incoming, outgoing_msg);
		rc = zmsg_send(&outgoing, active_sock);
		if (rc < 0) {
			delete_player_on_failure(manager, sock_id);
		}
		CLEAN_ZMSGS(&incoming, &outgoing);
		CLEAN_MSG_CONTENT(incoming_msg, outgoing_msg);
		incoming = outgoing = 0x0;
	}
	return (0);
}
