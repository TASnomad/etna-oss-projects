#include "MegaMimes.h"
#include "io.h"
#include "./response.h"

void        add_response_header(t_response *res, char *header, char *value)
{
    if (!res->headers)
        return ;
    insert_node(res->headers, create_node(create_header(header, value)));
}

void         set_response_content_type(t_response *res, char *path)
{
    char      *mime;

    mime = (char *) getMegaMimeType(path);
    if (mime)
        add_response_header(res, "Content-Type", mime);
}

t_string    defaultStatusText(int code)
{
    if (code == 100)
    {
        return (strdup("Continue"));
    }
    if (code == 101)
    {
        return (strdup("Switching Protocols"));
    }
    if (code == 200)
    {
        return (strdup("OK"));
    }
    if (code == 201)
    {
        return (strdup("Created"));
    }
    if (code == 202)
    {
        return (strdup("Accepted"));
    }
    if (code == 203)
    {
        return (strdup("Non-Authoritative Information"));
    }
    if (code == 204)
    {
        return (strdup("No Content"));
    }
    if (code == 205)
    {
        return (strdup("Reset Content"));
    }
    if (code == 206)
    {
        return (strdup("Partial Content"));
    }
    if (code == 300)
    {
        return (strdup("Multiple Choices"));
    }
    if (code == 301)
    {
        return (strdup("Moved Permanently"));
    }
    if (code == 302)
    {
        return (strdup("Found"));
    }
    if (code == 303)
    {
        return (strdup("See Other"));
    }
    if (code == 304)
    {
        return (strdup("Not Modified"));
    }
    if (code == 305)
    {
        return (strdup("Use Proxy"));
    }
    if (code == 307)
    {
        return (strdup("Temporary Redirect"));
    }
    if (code == 400)
    {
        return (strdup("Bad Request"));
    }
    if (code == 401)
    {
        return (strdup("Unauthorized"));
    }
    if (code == 402)
    {
        return (strdup("Payment Required"));
    }
    if (code == 403)
    {
        return (strdup("Forbidden"));
    }
    if (code == 404)
    {
        return (strdup("Not Found"));
    }
    if (code == 405)
    {
        return (strdup("Method Not Allowed"));
    }
    if (code == 406)
    {
        return (strdup("Not Acceptable"));
    }
    if (code == 407)
    {
        return (strdup("Proxy Authentication Required"));
    }
    if (code == 408)
    {
        return( strdup("Request Time-out"));
    }
    if (code == 409)
    {
        return (strdup("Conflict"));
    }
    if (code == 410)
    {
        return (strdup("Gone"));
    }
    if (code == 411)
    {
        return (strdup("Length Required"));
    }
    if (code == 412)
    {
        return (strdup("Precondition Failed"));
    }
    if (code == 413)
    {
        return (strdup("Request Entity Too Large"));
    }
    if (code == 414)
    {
        return (strdup("Request-URI Too Large"));
    }
    if (code == 415)
    {
        return (strdup("Unsupported Media Type"));
    }
    if (code == 416)
    {
        return (strdup("Requested range not satisfiable"));
    }
    if (code == 417)
    {
        return (strdup("Expectation Failed"));
    }
    if (code == 500)
    {
        return (strdup("Internal Server Error"));
    }
    if (code == 501)
    {
        return (strdup("Not Implemented"));
    }
    if (code == 502)
    {
        return (strdup("Bad Gateway"));
    }
    if (code == 503)
    {
        return (strdup("Service Unavailable"));
    }
    if (code == 504)
    {
        return (strdup("Gateway Time-out"));
    }
    if (code == 505)
    {
        return (strdup("HTTP Version not supported"));
    }
    return (NULL);
}

char        *readFile(char *filename)
{
    char    *buf;
    int     fd;
    char    *content;
    int     size;
    int     ret;
    off_t   offset;

    offset = 0;
    ret = 0;
    fd = open(filename, O_RDWR);
    size = filesize(fd);
    buf = (char *)malloc(sizeof(*buf) * (1024 + 1));
    if (!buf)
    {
        close(fd);
        return (NULL);
    }
    content = (char *)malloc(sizeof(*content) * (size + 1));
    if (!content)
    {
        close(fd);
        return (NULL);
    }
    while ((ret = read(fd, buf, 1024)) > 0)
    {
        buf[ret] = 0;
        strcpy(content + offset, buf);
        offset += ret;
    }
    content[offset] = 0;
    free(buf);
    close(fd);
    return (content);
}

void        create_response(t_response *res, http_status_code code)
{
    size_t  size;

    if (!res->headers)
        res->headers = new_list();
    res->status = code;
    res->reason_phrase = defaultStatusText(code);
    if (!res->body)
        res->body = strdup(res->reason_phrase);
    size = (res->file_fd > -1) ? filesize(res->file_fd) : (int) strlen(res->body);
    add_response_header(res, "Server", "Cherokee/0.0.1");
    add_response_header(res, "Content-Length", number_to_string(size));
}


void        success(t_response *response)
{
	response->status = 200;
	response->reason_phrase = defaultStatusText(response->status);
}

void        internal_error(t_response *response)
{
    char *isvLocation = "./error/ui/500.html";

	response->body = readFile(isvLocation);
    create_response(response, INTERNAL_SERVER_ERROR);
    set_response_content_type(response, isvLocation);
}

void        not_found(t_response *response)
{
    char *nfLocation = "./error/ui/404.html";

	response->body = readFile(nfLocation);
    create_response(response, NOT_FOUND);
    set_response_content_type(response, nfLocation);
}