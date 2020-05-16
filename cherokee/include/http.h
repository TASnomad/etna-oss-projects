#ifndef _HTTP_H_
# define _HTTP_H_

#include "./my_string.h"
#include "./my_bool.h"
#include "./list.h"

typedef enum			e_http_status_code
{
	CONTINUE			= 100,
	SWITCHING_PROTOCOL,
	OK				= 200,
	CREATED,
	ACCEPTED,
	NON_AUTHORITATIVE_INFORMATION,
	NO_CONTENT,
	PARTIAL_CONTENT,
	RESET_CONTENT,
	MULTIPLE_CHOICES		= 300,
	MOVED_PERMATENTLY,
	FOUND,
	SEE_OTHER,
	NOT_MODIFIED,
	USE_PROXY,
	TEMPORARY_REDIRECT		= 307,
	BAD_REQUEST			= 400,
	UNAUTHORIZED,
	PAYEMENT_REQUIRED,
	FORBIDDEN,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	PROXY_AUTHENTICATION_REQUIRED,
	REQUEST_TIMEOUT,
	CONFLICT,
	GONE,
	LENGTH_REQUIRED,
	PRECONDITION_FAILED,
	REQUEST_ENTITY_TOO_LARGE,
	REQUEST_URI_TOO_LONG,
	UNSUPPORTED_MEDIA_TYPE,
	REQUESTED_RANGE_NOT_SATISFIABLE,
	EXPECTATION_FAILED,
	INTERNAL_SERVER_ERROR		= 500,
	NOT_IMPLEMENTED,
	BAD_GATEWAY,
	SERVICE_UNAVAILABLE,
	GATEWAY_TIMEOUT,
	HTTP_VERSION_NOT_SUPPORTED,
}		http_status_code;

typedef struct s_header {
    t_string    key;
    t_string    value;
}              t_header;

typedef t_list *t_headers;

typedef struct s_server {
    t_string    document_root;
}              t_server;

typedef struct s_request {
    t_headers   headers;
    t_server    *server;
    t_string    http_version;
    t_string    method;
    t_string    path;
    t_string    body;
}              t_request;

typedef struct s_response {
    t_headers   headers;
    t_string    http_version;
    t_string    reason_phrase;
    t_string    body;
    int         file_fd;
    int         status;
}               t_response;

void        default_handle(t_request *, t_response *);

t_header    *create_header(t_string, t_string);
t_request   *s_to_request(t_string *);
t_string    *response_to_s(t_response *);

void        delete_request(t_request *request);

int         cmp_header(t_header *, t_header *);
t_header    *find_header(t_headers, t_string);

void        display_header(t_headers);

t_string    response_headers_to_string(t_response *);

t_string    toResponseString(t_response *);
void        remove_header(t_node *);

t_response *init_response();
void        delete_response(t_response *);

#endif
