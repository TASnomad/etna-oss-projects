#ifndef _RESPONSE_H_
# define _RESPONSE_H_

#include "./http.h"
#include "./my_string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

t_string    defaultStatusText(int);
void        success(t_response *);
void        internal_error(t_response *);
void        not_found(t_response *);
void        create_response(t_response *, http_status_code);
void        set_response_content_type(t_response *, char *);
void        add_response_header(t_response *, char *, char *);

#endif