#ifndef	__CYCLE_H__
#define	__CYCLE_H__

#include	<pthread.h>
#include	<list.h>
#include	<player.h>

void		refresh_cyclce(t_list *);
void 		kill_player(t_list *, player_t *);

void		*tick_thread(void *);

#endif /* ! __CYCLE_H__ */
