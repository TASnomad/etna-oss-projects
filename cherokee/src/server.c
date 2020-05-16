#include	<stdio.h>
#include	<stdlib.h>
#include	"server.h"
#include	"io.h"
#include 	"cherokee.h"
#include 	"my_integer.h"
#include 	"http.h"
#include 	"response.h"
#include	"syntax.h"
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include 	<fcntl.h>
#include 	<unistd.h>

int		create_server_socket(int port)
{
	struct sockaddr_in srv;
	int opts;
	int fd;

	opts = 1;
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_port = htons(port);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("create_server_socket socket:");
		return (fd);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(socklen_t)) < 0)
	{
		perror("create_server_socket cant set SO_REUSEADDR flags:");
	}
	if (bind(fd, (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		perror("create_server_socket bind:");
		close(fd);
		return (-1);
	}
	if (listen(fd, SOMAXCONN) < 0)
	{
		perror("create_server_socket listen:");
		close(fd);
		return (-1);
	}
	return (fd);
}

/*
t_string 	read_file(char *filename)
{
	int 		fd;
	off_t		size;
	t_string	buf;
	int 		count;

	fd = open(filename, 0);
	if (fd == -1)
	{
		perror("open()");
		return (NULL);
	}
	size = filesize(fd);
	if ((buf = malloc(sizeof(*buf) * (size + 1))) == 0)
	{
		perror("malloc()");
		return (NULL);
	}
	count =	read(fd, buf, size);
	buf[count] = 0;
	return (buf);
}

t_response		*defaultHandle(t_request *request)
{
	int			fd;
	t_response	*response;
	t_string	document_root;
	t_string 	server_path;

	response = init_response();
	document_root = read_file(DOCUMENT_ROOT_CONF);
	if (!document_root)
	{
		printf("Fail to read conf file %s\n", DOCUMENT_ROOT_CONF);
		return (NULL);
	}
	if (!response)
	{
		printf("Fail to init memory");
		return (NULL);
	}
	if (strcmp(request->path, "/") == 0)
	{
		printf("%s\n", request->path);
		free(request->path);
		request->path = strdup("/index.html");
		printf("after\n");
	}
	server_path = (t_string)malloc((sizeof(char) * (strlen(document_root) + strlen(request->path) + 1)));
	if (!server_path)
	{
		internal_error(response);
		return (response);
	}
	strcpy(server_path, document_root);
	strcpy(server_path + strlen(document_root), request->path);
	fd = open(server_path, O_RDWR);
	if (fd > 0)
	{
		success(response);
		response->file_fd = fd;
	}
	else
	{
		not_found(response);
	}
	return (response);
}
*/

char	*pathname(char *request_path, char *document_root)
{
	char 	*server_path;

	server_path = (t_string)malloc((sizeof(char) * (strlen(document_root) + strlen(request_path) + 1)));
	if (!server_path)
	{
		return (NULL);
	}
	strcpy(server_path, document_root);
	strcpy(server_path + strlen(document_root), request_path);
	return (server_path);
}

/*
t_response		*handle(char *protocol)
{
	t_request		*request;
	t_http_lexemes	lexemes;
	t_response		*response;

	lexemes = parse_request(protocol);
	if (!lexemes)
	{
		printf("fail to parse request\n");
		return (NULL);
	}
	request = lexeme_to_request(lexemes);
	if (!request)
	{
		printf("get object request\n");
		delete_list_exec_action(lexemes, &remove_lexeme);
		return (NULL);
	}
	response = defaultHandle(request);
	delete_list_exec_action(lexemes, &remove_lexeme);
	delete_request(request);
	if (!response)
	{
		printf("fail to create response\n");
		return (NULL);
	}
	return (response);
}
*/

void	s_send(t_integer *fd, t_response *response)
{
#if defined EXPERIMENT
	/*size = filesize(response->file_fd);*/
	if (fd->value > -1)
		cherokee_sendfile(response, fd->value);
#else
	t_string	str;

	str = toResponseString(response);
	send(fd->value, str, strlen(str), 0);
	free(str);
	if (fd->value != -1)
	{
		//sendfile(fd->value, response->file_fd, &offset, size);
		int 		offset;
		char 		buf[1024];
		offset = 0;
		while ((offset = read(response->file_fd, buf, 1024)) > 0)
		{
			buf[offset] = 0;
			send(fd->value, buf, offset, 0);
		}
		if (offset == -1)
		{
			perror("read()");
		}
	}
#endif	/* !EXPERIMENT */
		close(response->file_fd);
}
/*

void 	handle_request(void *data)
{
	char 		buff[BUFSIZE];
	int 		rsize;
	t_integer 	*fd;
	t_response	*response;

	fd = (t_integer *)data;
	rsize = recv(fd->value, buff, BUFSIZE, 0);
	if (rsize <= 0)
	{
		perror("handle_request() recv:");
		close(fd->value);
		free(fd);
		return;
	}
	response = handle(buff);
	if (!response)
	{
		close(fd->value);
		free(fd);
		return;
	}
	s_send(fd, response);
	delete_response(response);
	close(fd->value);
	free(fd);
}


void 	routingHandle(t_request *request)
{
	printf("%p\n", request);
}

void		launch_server(int fd)
{
	int	client_fd;
	struct sockaddr_in	client_addr;
	socklen_t	size;
	char	*IP = NULL;

	while (1)
	{
		client_fd = accept(fd, (struct sockaddr *) &client_addr, &size);
		if (client_fd < 0)
		{
			perror("launch_server accept:");
			continue;
		}
		IP = inet_ntoa(client_addr.sin_addr);
		if (IP)
		{
			printf("Got new connection from : %s\n", IP);
		}
		//handle_request(&client_fd);
		close(client_fd);
	}
}


*/