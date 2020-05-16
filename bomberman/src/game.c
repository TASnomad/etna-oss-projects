#include		<game.h>

#include		<stdlib.h>

static t_game_stat	*holder = NULL;

t_game_stat		*get_game_stat(void)
{
	return (holder);
}

void 			update_game_stat(t_game_stat *st)
{
	holder = st;
}

void			ctor_game_stat(void)
{
	if (!holder)
	{
		holder = (t_game_stat *) malloc(sizeof(t_game_stat));
		if (!holder)
			return ;
	}
	holder->status = WAITING_PLAYERS;
	holder->clt_count = 0;
}
