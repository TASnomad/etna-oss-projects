#include            "cherokee.h"
#include            "http.h"
#include            "response.h"
#include            "router.h"
#include            "syntax.h"
#include            "server.h"

#include            <fcntl.h>
#include            <unistd.h>
#include            <stdio.h>
#include            <signal.h>
#include			<sys/ioctl.h>

static char         *get_absolute_path(char *cherokee_root, char *path)
{
    return (pathname(path, cherokee_root));
}

static int          route_comparator(void *route, void *path)
{
    char            *p;
    t_route_handler *r;

    p = (char *) path;
    r = (t_route_handler *) route;
    if (r && r->route && p) {
        return strcmp(r->route, p);
    }
    return (-1);
}

static int          method_comparator(void *iterator, void *method)
{
    char            *m;
    char            *it;

    it = (char *) iterator;
    m = (char *) method;
    return strcmp(m, it);
}

static t_request    *create_request_from_raw(char *raw)
{
    t_request		*request;
	t_http_lexemes  lexemes;

    lexemes = parse_request(raw);
    if (!lexemes)
        return (NULL);
    request = lexeme_to_request(lexemes);
    return (request);
}

t_bool              is_file_exist(char *cherokee_root, char *path)
{
    char            *location;
    t_bool          res;

    res = FALSE;
    location = pathname(path, cherokee_root);
    res = (access(location, F_OK) == 0);
    free(location);
    return (res);
}

t_bool              is_method_handled(t_route_handler *handler, char *method)
{
    void            *res;

    if (!handler || !method)
        return (FALSE);
    res = search_item_in_list(handler->methods, method, method_comparator);
    return (!res) ? (FALSE) : (TRUE);
}

t_route_handler     *get_route_by_path(t_routes *list, char *path)
{
    return search_item_in_list(list, path, route_comparator);
}

static void         end_request(t_response *res, int fd, cherokee_module_t *cbs)
{
    cbs->on_send(fd, res);
    close(fd);
}

int 				request_size(int fd)
{	
	ssize_t			rsize = 0;

	ioctl(fd, FIONREAD, &rsize);
	return (rsize);
}

void                cherokee_router(void *data)
{
    t_router_dto    *args = (t_router_dto *) data;
    t_request       *req;
    t_response      *http_res;
    t_route_handler *h;
    t_string        raw;
    int             size;

    http_res = init_response();
    if (!http_res)
    {
        close(args->client);
        return;
    }
    size = request_size(args->client);
	if (!size)
    {
        close(args->client);
        delete_response(http_res);
		return;
    }
	if (size < 0)
	{
		perror("cherokee_on_receive() -> recv");
        delete_response(http_res);
        close(args->client);
		return;
	}
    if ((raw = (char *)malloc(sizeof(*raw) * (size + 1))) == NULL)
    {
        perror("cherokee_on_receive() -> malloc");
        delete_response(http_res);
        close(args->client);
        return;
    }
    args->lifecycle->on_receive(args->client, size, raw);
    if (size ==  REQUEST_ENTITY_TOO_LARGE)
    {
        free(raw);
        create_response(http_res, REQUEST_ENTITY_TOO_LARGE);
        return end_request(http_res, args->client, args->lifecycle);
    }
    else
    {
        req = create_request_from_raw(raw);
        if (!req)
        {
            create_response(http_res, INTERNAL_SERVER_ERROR);
            free(raw);
            return end_request(http_res, args->client, args->lifecycle);
        }
        else
        {
            free(raw);
            h = get_route_by_path(args->routes, req->path);
            if (!h)
            {
                if (is_file_exist(args->document_root, req->path))
                {
                    char *l = get_absolute_path(args->document_root, req->path);
                    http_res->file_fd = open(l, O_RDONLY);
                    create_response(http_res, OK);
                    set_response_content_type(http_res, l);
                    free(l);
                    return end_request(http_res, args->client, args->lifecycle);
                }
		        not_found(http_res);
                return end_request(http_res, args->client, args->lifecycle);
            }
            if (!is_method_handled(h, req->method))
            {
                create_response(http_res, METHOD_NOT_ALLOWED);
                return end_request(http_res, args->client, args->lifecycle);
            }
            h->handler(req, http_res);
        }
    }
    args->lifecycle->on_send(args->client, http_res);
    close(args->client);
    delete_response(http_res);
    free(data);
}
