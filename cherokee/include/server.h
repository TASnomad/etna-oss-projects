#ifndef	__CHEROKEE_SERVER_H__
#define	__CHEROKEE_SERVER_H__

#include	<stdlib.h>
#include	<unistd.h>
#include	<arpa/inet.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include    <string.h>
#include    "cherokee.h"
#include    "./list.h"

#define BUFSIZE	8192000 /* a request can't exceed 8kb */
#define CR 	"\r"
#define LF	"\n"

int			create_server_socket(int);
void        handle_request(void *);
char        *pathname(char *, char *);
// void		handle_request();

#endif	/* !__CHEROKEE_SERVER_H__ */
