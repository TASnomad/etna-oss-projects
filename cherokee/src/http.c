#include "http.h"

// char    *strdup(char *);

void        remove_header(t_node *node)
{
    t_header    *header;

    header = (t_header *)node->data;
    free(header->key);
    free(header->value);
    free(header);
}

t_header    *create_header(t_string a, t_string b)
{
    t_header    *header;

    if ((header = (t_header *)malloc(sizeof(*header))) == NULL)
    {
        return (NULL);
    }
    header->key = a;
    header->value = b; 
    return (header);
}

void    delete_request(t_request *request)
{
    delete_list_exec_action(request->headers, &remove_header);
    free(request); 
}

t_header    *find_header(t_headers list, t_string key)
{
    t_header    header;
    t_node      *node;

    header.key = key;
    node = search_node((t_list *)list, &header, (void_cmp)&cmp_header);
    if (!node)
    {
        return (NULL);
    }
    return ((t_header *)node->data);
}

int         cmp_header(t_header *a, t_header *b)
{
    return (strcmp(a->key, b->key));
}

void        display_header(t_headers headers)
{
    t_node  *begin;

    begin = headers->head;
    while (headers->head)
    {
        t_node  *node = headers->head;
        t_header  *header = (t_header *)node->data;
        printf("Header (%s, %s) -> length value: %d\n", header->key, header->value, (int)strlen(header->value));
        headers->head = headers->head->next;
    }
    headers->head = begin;
}

t_string        response_headers_to_string(t_response *res)
{
    t_string    ret;
    int         size;
    int         start;
    t_header    *h;
    t_node      *it;

    h = NULL;
    it = NULL;
    start = 0;
    size = 5;
    size += strlen(res->http_version) + 1;
    size += 4; // HTTP code
    size += strlen(res->reason_phrase) + 2; // http code meaning message + CRLF
    if (res->headers)
        it = res->headers->head;
    while (res->headers && res->headers->head)
    {
        h = (t_header *) res->headers->head->data;
        size += (strlen(h->key) + 2 + strlen(h->value)) + 2;
        res->headers->head = res->headers->head->next;
    }
    size += 2;
    if (res->headers)
        res->headers->head = it;
    if (!(ret = (t_string) malloc(sizeof(char) * (size + 1))))
        return (NULL);
    start = append_alloc_string(ret, "HTTP/", start);
    start = append_alloc_string(ret, res->http_version, start);
    start = append_alloc_string(ret, " ", start);
    start = append_alloc_string(ret, number_to_string(res->status), start);
    start = append_alloc_string(ret, " ", start);
    start = append_alloc_string(ret, res->reason_phrase, start);
    start = append_alloc_string(ret, "\r\n", start);
    while (res->headers && res->headers->head)
    {
        h = (t_header *)res->headers->head->data;
        start = append_alloc_string(ret, h->key, start);
        start = append_alloc_string(ret, ": ", start);
        start = append_alloc_string(ret, h->value, start);
        start = append_alloc_string(ret, "\r\n", start);
        res->headers->head = res->headers->head->next;
    }
    if (res->headers)
        res->headers->head = it;
    start = append_alloc_string(ret, "\r\n", start);
    ret[start] = 0;
    return (ret);
}

t_string    toResponseString(t_response *response)
{
    int         size;
    t_node      *first;
    t_header    *header;
    t_string    ret;
    int         start;

    start = 0;
    header = 0;
    size = 5;
    size += strlen(response->http_version);
    size++;
    size += 3; // size http code
    size++;
    size += strlen(response->reason_phrase);
    size += 2; // size CRLF
    if (response->headers)
        first = response->headers->head;
    while (response->headers && response->headers->head)
    {
        header = (t_header *)response->headers->head->data;
        size += (strlen(header->key) + 2 + strlen(header->value));
        size += 2; //size CRLF
        response->headers->head = response->headers->head->next;
    }
    size += 2;
    if (response->body)
    {
        size += strlen(response->body);
    }
    if (response->headers)
        response->headers->head = first;
    if ((ret = (t_string )malloc(sizeof(*ret) * (size + 1))) == NULL)
    {
        return (NULL);
    }
    start = append_alloc_string(ret, "HTTP/", start);
    start = append_alloc_string(ret, response->http_version, start);
    start = append_alloc_string(ret, " ", start);
    start = append_alloc_string(ret, number_to_string(response->status), start);
    start = append_alloc_string(ret, " ", start);
    start = append_alloc_string(ret, response->reason_phrase, start);
    start = append_alloc_string(ret, "\r\n", start);
    while (response->headers && response->headers->head)
    {
        header = (t_header *)response->headers->head->data;
        start = append_alloc_string(ret, header->key, start);
        start = append_alloc_string(ret, ": ", start);
        start = append_alloc_string(ret, header->value, start);
        start = append_alloc_string(ret, "\r\n", start);
        response->headers->head = response->headers->head->next;
    }
    if (response->headers)
        response->headers->head = first;
    start = append_alloc_string(ret, "\r\n", start);
    if (!response->body)
    {
        ret[start] = 0;
        return (ret);
    }
    start = append_alloc_string(ret, response->body, start);
    ret[start] = 0;
    return (ret);
}

void    delete_response(t_response  *response)
{
    free(response->http_version);
    free(response->reason_phrase);
    if (response->body)
        free(response->body);
    if (response->headers)
        delete_list_exec_action(response->headers, &remove_header);
    free(response);
}

t_response  *init_response()
{
    t_response  *response;

    if ((response = (t_response *)malloc(sizeof(*response))) == NULL)
    {
        return (NULL);
    }
    response->body = NULL;
	response->http_version = strdup("1.1");
    response->reason_phrase = NULL;
    response->status = 0;
    response->headers = NULL;
    response->file_fd = -1;
    return (response);
}