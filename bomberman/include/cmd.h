#ifndef	__CMD_H__
#define	__CMD_H__

#include		"proto.h"

typedef struct		s_cmd_runner
{
	t_req_clt	cmd;
	int		(*handler)(t_server *, int, t_req_clt);
}			t_cmd_runner;

int			run_cmd(t_server *, int, t_message);
int			player_quit(t_server *, int, t_req_clt);
int			get_players(t_server *, int, t_req_clt);
int			get_game_status(t_server *, int, t_req_clt);
int			move_player_cmd(t_server *, int, t_req_clt);
int			add_bomb(t_server *, int, t_req_clt);

#endif /* !__CMD_H__ */
