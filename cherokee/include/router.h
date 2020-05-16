#ifndef __CHEROKEE_ROUTER_H__
#define __CHEROKEE_ROUTER_H__

#include    "http.h"
#include    "list.h"
#include    "my_bool.h"

typedef enum    e_http_verb
{
    GET,
    POST,
    PUT,
    HEAD,
    OPTIONS,
    ALL,
}               t_http_verb;

typedef int     (*route_t)(t_request *, t_response *);

typedef struct  s_route_handler
{
    char        *route;
    t_list      *methods;
    route_t     handler;
}               t_route_handler;

typedef t_list  t_routes;

t_route_handler *get_route_by_path(t_routes *, char *);
t_bool          is_file_exist(char *, char *);
t_bool          is_method_handled(t_route_handler *, char *);

void            cherokee_init_routes(t_routes *);
void            cherokee_delete_route(t_node *node);
void            cherokee_router(void *);

#endif  /* !__CHEROKEE_ROUTER_H__ */