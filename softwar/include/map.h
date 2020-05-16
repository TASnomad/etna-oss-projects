#ifndef __MAP_H__
#define __MAP_H__

#include	<list.h>
#include	<player.h>

int		is_player_in(t_list *, unsigned int, unsigned int);
int		is_energy_cell_in(t_list *, unsigned int, unsigned int);
int		is_wall_in(unsigned int, unsigned int);
int		is_free_cell(t_list *, t_list *, int, int);

int		can_move(t_list *, t_list *, int, int);

t_link		*get_energy_by_coords(t_list *, int, int);
player_t	*get_player_by_coords(t_list *, int, int);

char		**get_watch_view(t_list *, t_list *, int, int, int);

#endif /* ! __MAP_H__ */
