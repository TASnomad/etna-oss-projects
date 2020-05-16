#ifndef __GMANAGER_H__
#define __GMANAGER_H__

#include		<list.h>
#include		<ginfo.h>
#include		<player.h>
#include		<game_ctx.h>
#include		<pthread.h>
#include		<stdio.h>

#define			GAME_STARTED	1
#define			GAME_FINISHED	2

#define			REGULAR_LOG	0
#define			VERBOSE_LOG	1

typedef struct		s_game_instance
{
	t_socks		*socks;
	t_list		*players;
	t_list		*energy_cells;
	int		ready;
	unsigned long 	cycle_duration;
	int		map_size;
	FILE		*log_stream;
	int		logging_level;
	pthread_t	cycle_thread_id;
}			game_instance_t;

game_instance_t		*get_instance();
t_list			*init_players_list();

int			my_logger(int, char *, ...);
void			dtor_manager(game_instance_t *);

#endif /* ! __GMANAGER_H__ */
