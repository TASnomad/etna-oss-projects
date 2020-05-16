#ifndef __CHEROKEE_IO_H__
#define __CHEROKEE_IO_H__

#include    "http.h"

#include    <sys/types.h>
#include    <sys/socket.h>

#if defined(__linux__)
    #include    <sys/sendfile.h>
#elif defined(__APPLE__) || defined(__FreeBSD__)
    #include    <sys/uio.h>
#endif

off_t       filesize(int);
void        cherokee_sendfile(t_response *, int);

#endif  /* !__CHEROKEE_IO_H__ */