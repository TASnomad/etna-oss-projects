#include			"cherokee.h"
#include			"list.h"
#include			"http.h"
#include			"router.h"
#include			"server.h"
#include 			"my_integer.h"
#include			"io.h"

#include 			<fcntl.h>
#include 			<pthread.h>
#include			<stdlib.h>
#include			<string.h>
#include 			<unistd.h>
#include			<sys/ioctl.h>

static t_string		read_cherokee_config(char *path)
{
	int				fd;
	off_t			size;
	t_string		buf;
	int				count;

	fd = open(path, 0);
	if (fd < 0)
	{
		perror("read_cherokee_config() -> open()");
		close(fd);
		return (NULL);
	}
	size = filesize(fd);
	if (!(buf = malloc(sizeof(char) * (size + 1))))
	{
		perror("read_cherokee_config() -> malloc()");
		close(fd);
		return (NULL);
	}
	count = read(fd, buf, size);
	buf[count] = 0;
	close(fd);
	return (buf);
}

cherokee_t			*cherokee_create_instance(int port)
{
	cherokee_t	*instance;

	instance = NULL;
	instance = (cherokee_t *) malloc(sizeof(cherokee_t));
	if (!instance)
		return (NULL);
	instance->life_cycle = (cherokee_module_t *) malloc(sizeof(cherokee_module_t *));
	if (!instance->life_cycle) {
		free(instance);
		return (NULL);
	}
	instance->fd = create_server_socket(port);
	instance->running = 0;
	instance->document_root = strdup(read_cherokee_config(DOCUMENT_ROOT_CONF));
	instance->clients = new_list();
	instance->routes = new_list();
	instance->pool = (threadpool) thpool_init(4);
	instance->life_cycle->on_connection = &cherokee_on_connect;
	instance->life_cycle->on_receive = &cherokee_on_receive;
	instance->life_cycle->on_request_parse = &cherokee_on_parsing;
	instance->life_cycle->on_response_build = &cherokee_on_res_builder;
	instance->life_cycle->on_send = &cherokee_on_send;
	cherokee_init_routes(instance->routes);
	return (instance);
}

int					cherokee_on_connect(int fd)
{
	printf("fd: %d\n", fd);
	return (1);
}

int					cherokee_on_receive(int fd, int rsize, char *buffer)
{
	rsize = recv(fd, buffer, rsize, 0);
	if (!rsize)
		return (0);
	if (rsize < 0)
	{
		perror("cherokee_on_receive() -> recv");
		return (-1);
	}
	return (rsize);
}

int					cherokee_on_parsing(t_request *req)
{
	printf("version: %s\n", req->http_version);
	return (1);
}

int					cherokee_on_res_builder(t_request *req, t_response *res)
{
	printf("%s\n", req->http_version);
	printf("%s\n", res->body);
	return (1);
}

int					cherokee_on_send(int fd, t_response *res)
{
	t_string to_send;

#if defined EXPERIMENT
	if (res->file_fd > -1)
	{
		cherokee_sendfile(res, fd);
		close(res->file_fd);
	}
	else
	{
		to_send = toResponseString(res);
		send(fd, to_send, strlen(to_send), 0);
		free(to_send);
	}
#else
	to_send = response_headers_to_string(res);

	if (res->file_fd > -1)
	{
		int 		offset;
		char 		buf[1024];

		offset = 0;
		send(fd, to_send, strlen(to_send), 0);
		while ((offset = read(res->file_fd, buf, 1024)) > 0)
		{
			buf[offset] = 0;
			send(fd, buf, offset, 0);
		}
		if (offset == -1)
		{
			perror("read()");
		}
	}
	else
	{
		to_send = toResponseString(res);
		send(fd, to_send, strlen(to_send), 0);
		free(to_send);
	}
#endif
	return (0);
}

void 				delete_client(t_node *node)
{
	free(node->data);
}

void				delete_cherokee_instance(cherokee_t *cherokee)
{
	thpool_destroy(cherokee->pool);
	delete_list_exec_action(cherokee->clients, &delete_client);
	delete_list_exec_action(cherokee->routes, &cherokee_delete_route);
	free(cherokee->document_root);
	free(cherokee);
}

void				accept_conn(cherokee_t *cherokee)
{
	int			client;
	t_integer	*integer;

	client = 0;
	while (cherokee->running)
	{
		client = accept(cherokee->fd, NULL, NULL);
		if (client < 0)
		{
			perror("accept()");
			return;
		}
		printf("new connection: %d\n", client);
		integer = create_int(client);
		if (integer)
		{
			insert_list(cherokee->clients, integer);
		}
	}
}

int 	cmp(t_integer *a, t_integer *b)
{
	if (a->value > b->value)
	{
		return (1);
	}
	if (a->value < b->value)
	{
		return (-1);
	}
	return (0);
}

static t_router_dto			*create_router_dto_bundle(cherokee_t *instance)
{
	t_router_dto			*bundle;

	bundle = (t_router_dto *)malloc(sizeof(t_router_dto));
	if (!bundle) {
		return (NULL);
	}
	bundle->client = -1;
	bundle->document_root = instance->document_root;
	bundle->lifecycle = instance->life_cycle;
	bundle->routes = instance->routes;
	return (bundle);
}

int							cherokee_run(cherokee_t *cherokee)
{
	fd_set	read_fs;
	int		count;
	struct 	timeval time;
	int		max;
	t_integer	*tmp;
	t_integer	*tmp2;
	t_node 		*node;
	t_node 		*before;
	t_router_dto *bundle;

    signal(SIGPIPE, SIG_IGN);
	cherokee->running = 1;
	time.tv_usec = 0;
	time.tv_sec = 1;
	bundle = NULL;
	thpool_add_work(cherokee->pool, (exec_callback)&accept_conn, cherokee);
	while (cherokee->running)
	{
		FD_ZERO(&read_fs);
		node = cherokee->clients->head;
		count = 0;
		max = -1;
		while (node)
		{
			tmp = (t_integer *)node_val(node);
			FD_SET(tmp->value, &read_fs);
			if (tmp->value > max)
			{
				max = tmp->value;
			}
			node = node->next;
		}
		count = select(max + 1, &read_fs, NULL, NULL, &time);
		if (count > 0)
		{
			node = cherokee->clients->head;
			while (node)
			{
				tmp = (t_integer *)node_val(node);
				tmp2 = create_int(tmp->value);
				if (tmp2)
				{
					if (FD_ISSET(tmp2->value, &read_fs))
					{
						bundle = create_router_dto_bundle(cherokee);
						if (bundle) {
							bundle->client = tmp2->value;
						}
						thpool_add_work(cherokee->pool, (exec_callback) &cherokee_router, bundle);
						before = node;
						node = node->next;
						delete_item_in_list(cherokee->clients, before, &delete_client);
					}
					else
					{
						node = node->next;
					}
				}
				else
				{
					node = node->next;
				}
			}
		}
	}
	return (1);
}
