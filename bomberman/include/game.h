#ifndef	__GAME_H__
#define	__GAME_H__

#include		"player.h"
#include		"proto.h"

typedef struct		s_game_stat
{
	int		status;
	int		clt_count;
	int		player_id;
	t_player	player;
}			t_game_stat;

void			ctor_game_stat(void);
t_game_stat		*get_game_stat(void);
void			update_game_stat(t_game_stat *);

#endif	/* !__GAME_H__ */
