#ifndef	__CHEROKEE_H__
#define	__CHEROKEE_H__

#include	<stdio.h>
#include	<stdlib.h>

#include	"./thpool.h"
#include	"./http.h"
#include	"list.h"
#include	"router.h"

#define 	PORT	1111

typedef int					(*connect_hook)(int);
typedef int					(*receive_hook)(int, int, char *);
typedef int					(*request_parser_hook)(t_request *);
typedef int					(*response_builder_hook)(t_request *, t_response *);
typedef int					(*send_hook)(int, t_response *);

typedef void 				(*exec_callback)(void *);

#define	DOCUMENT_ROOT_CONF "conf/document_root.conf"

typedef struct				s_cherokee_module
{
	connect_hook			on_connection;
	receive_hook			on_receive;
	request_parser_hook		on_request_parse;
	response_builder_hook	on_response_build;
	send_hook				on_send;
}							cherokee_module_t;

typedef t_list t_clients;

typedef struct				s_cherokee
{
	cherokee_module_t		*life_cycle;
	t_clients				*clients;
	t_string				document_root;
	int						fd;
	int						running;
	threadpool				pool;
	t_routes				*routes;
}							cherokee_t;

typedef struct  s_router_dto
{
    t_routes    *routes;
    t_string    document_root;
    cherokee_module_t   *lifecycle;
    int         client;
}               t_router_dto;

cherokee_t					*cherokee_create_instance(int);
void						delete_cherokee_instance(cherokee_t *);
int							cherokee_run(cherokee_t *);

int							cherokee_on_connect(int);
int							cherokee_on_receive(int, int, char *);
int							cherokee_on_parsing(t_request *);
int							cherokee_on_res_builder(t_request *, t_response *);
int							cherokee_on_send(int, t_response *);

int 						request_size(int);

#endif	/* !__CHEROKEE_H__ */
