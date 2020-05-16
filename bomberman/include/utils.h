#ifndef	__UTILS_H__
#define	__UTILS_H__

#include	"response.h"
#include	"window.h"

int			connect_to_server(t_window *, char *, char *);
t_response_status	get_map(t_window *);
int		wait_until_game_start(t_window *);

#endif	/* !__UTILS_H__ */