#include    <errno.h>
#include    <io.h>
#include    <http.h>

off_t 			filesize(int fd)
{
	off_t		origin;
	off_t		size;

	size  = 0;
	/**
	 * restoring seek should not be required
	 * but maybe in future we could need to only fetch filesize from a specific offset
	 */
	origin = lseek(fd, 0, SEEK_CUR);
	lseek(fd, 0, SEEK_SET);
	size = lseek(fd, 0, SEEK_END);
	lseek(fd, origin, SEEK_SET);
	return (size);
}

#if defined(__linux__)

// FIXME: need tests
void            cherokee_sendfile(t_response *res, int fd)
{
    t_string    h_str;
    off_t       offset;
    size_t      count;
    size_t      total;

    offset = 0;
    h_str = response_headers_to_string(res);
    if (!h_str)
        return ;
    count = filesize(res->file_fd);
    total = count;
    send(fd, h_str, strlen(h_str), MSG_MORE);
    free(h_str);
    while (1)
    {
        /**
         * NOTE: Linux kernel will only send (INT_MAX - _SC_PAGESIZE - 1) bytes per call
         */
        ssize_t written = sendfile(fd, res->file_fd, &offset, count);
        if (written < 0)
        {
            perror("cherokee_sendfile():");
            return ;
        }
        total -= written;
        if (total < 1)
            break;
    }
}

#elif defined(__APPLE__) || defined(__FreeBSD__)

void            cherokee_sendfile(t_response *res, int fd)
{
    t_string    h_str;
    size_t      total;
    off_t       offset;
    off_t       len;
    size_t      count;

    total = 0;
    offset = len = 0;
    h_str = response_headers_to_string(res);
    if (!h_str)
        return ;
    count = filesize(res->file_fd);
    struct sf_hdtr headers = {
        .headers = (struct iovec[]){{
            .iov_base = (void *) h_str,
            .iov_len = strlen(h_str),
        }},
        .hdr_cnt = 1,
    };
    do {
        int r;
#ifdef __APPLE__
        r = sendfile(res->file_fd, fd, offset, &len, &headers, 0);
#else
        r = sendfile(res->file_fd, fd, offset, count, &headers, &len, SF_MNOWAIT);
#endif
        if (r < 0)
        {
            perror("cherokee_sendfile()");
            free(h_str);
            return ;
        }
        total += (size_t) len;
    } while (total < count);
    free(h_str);
}

#else

#define BSIZE   512

void            cherokee_sendfile(t_response *res, int fd)
{
    ssize_t     seek;
    char        buf[BSIZE];
    t_string    headers;

    headers = response_headers_to_string(res);
    if (!headers)
        return ;
    send(fd, headers, strlen(headers), 0);
    free(headers);
    while ((seek = read(res->file_fd, buf, BSIZE)) > 0)
    {
        buf[seek] = 0;
        send(fd, buf, seek, 0);
    }
    if (seek < 0)
        perror("cherokee_sendfile");
}

#endif