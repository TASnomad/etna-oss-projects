#ifndef	__RESPONSE_H__
#define	__RESPONSE_H__

#include			"proto.h"
#include			"map.h"

typedef struct			s_response_connection
{
	t_req_clt			type;
	t_req_status		cmd_status;
	int					player_num;
}						t_response_connection;

typedef struct			s_response_players
{
	t_req_clt		type;
	t_req_status	cmd_status;
	t_player		players[4];
	int				players_count;
}				t_response_players;


typedef struct			s_game_status
{
	t_req_clt		type;
	t_req_status	cmd_status;
	int			map[MAP_SIZE * MAP_SIZE];
	int			map_size;
	t_game_status	status;
}				t_response_status;

typedef struct			s_response_bomb
{
	t_req_clt		type;
	t_req_status	cmd_status;
	int			cell;
	char			*map;
}				t_response_bomb;

typedef struct			s_response_error
{
	t_req_clt		type;
	t_req_status	cmd_status;
	t_req_status		error;
}				t_response_error;

#endif	/* !__RESPONSE_H__ */
