#include    "list.h"
#include    "my_string.h"
#include    "response.h"
#include    "router.h"
#include    "route.h"
#include    <string.h>

static char     *verbs[] = { "GET", "POST", "PUT", "HEAD", "OPTIONS", 0x0 };

int      home_route(t_request *req, t_response *res)
{

    res->file_fd = -1;
    success(res);
    asprintf(&res->body, "Hello from %s!", req->path);
    return (res->status);
}

static t_route_handler  *create_route(char *route, t_http_verb method, route_t handler)
{
    t_route_handler     *h = (t_route_handler *)malloc(sizeof(t_route_handler));
    int                 i;

    i = 0;
    if (!h)
        return (NULL);
    h->route = strndup(route, strlen(route) + 1);
    h->handler = handler;
    h->methods = new_list();
    if (method == ALL)
    {
        while (verbs[i])
        {
            insert_list(h->methods, strdup(verbs[i]));
            i++;
        }
    }
    else
        insert_list(h->methods, strdup(verbs[method]));
    return (h);
}

void        cherokee_init_routes(t_routes *list)
{
    t_route_handler *index = create_route("/", GET, home_route);
    if (index) {
        insert_list(list, index);
    }
}

void        cherokee_delete_methods(t_node *node)
{
    free(node->data);
}

void        cherokee_delete_route(t_node *node)
{
    t_route_handler *handler;

    handler = (t_route_handler *)node->data;
    free(handler->route);
    delete_list_exec_action(handler->methods, &cherokee_delete_methods);
}