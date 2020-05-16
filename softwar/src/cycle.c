#include	<pthread.h>
#include	<unistd.h>
#include	<signal.h>

#include	<cell.h>
#include	<cycle.h>
#include	<player.h>
#include	<gmanager.h>
#include	<enums.h>
#include	<notif.h>

void				refresh_cycle(t_list *players)
{
	t_link			*base;
	player_t		*p;

	base = players->first;
	while (players->first && players->index > 1)
	{
		p = (player_t *) players->first->value;
		// Each cycle will decrease the stun counter if necessary
		if (p->disabled > 0)
			p->disabled--;
		if (p->energy <= 0 || p->energy >= MAX_ENERGY) {
			p->disabled = -1; // No actions allowed if it's not removed from the list
			if (players->first->next) {
				remove_list(&players, players->first);
				base = players->first;
			} else {
				remove_list(&players, players->first);
				players->first = 0x0;
				base = players->first;
			}
			my_logger(REGULAR_LOG, "Player %s is dead (%s)\n", p->id, __func__);
			send_notif(CLIENT_DEAD, p->id, 0x0); // Death notif send to the player
			dtor_player(p);
		} else {
			p->energy -= ENERGY_RETAIN;
			p->action = ACTION_STARTER;
			players->first = players->first->next;
		}

	}
	players->first = base;
	p = 0x0;
}

/**
 * @description cycle thread entry point
 * The thread sleep xxx ms (depending on cmdline arguments) & send notifications to players
 * - Make players loose 2 energy points
 * - Notifications send: CYCLE
 */
void				*tick_thread(void *args)
{
	(void) args;
	game_instance_t		*manager;
	EnergyCell_t		*new_cell;
	struct json_object	*obj;
	int			cycle_count;

	cycle_count = 0;
	manager = get_instance();
	manager->ready = GAME_STARTED;
	notify_game_started();
	while (!zsys_interrupted)
	{
		// waiting until cycle is ended
		usleep(manager->cycle_duration);
		cycle_count++;
		my_logger(VERBOSE_LOG, "Cycle number: %d (%s)\n", cycle_count, __func__);
		refresh_cycle(manager->players);
		if (manager->players->index < 2) {
			player_t *tmp = manager->players->first->value;
			my_logger(REGULAR_LOG, "Player %s is the last entity standing ! {%p}(%s)\n", tmp->id, manager->players->first->next, __func__);
			send_notif(CLIENT_WIN, tmp->id, 0x0);
			send_notif(GAME_END, GLOBAL_TOPIC, 0x0);
			pthread_kill(manager->cycle_thread_id, SIGINT);
			pthread_exit(0x0);
		}
		new_cell = ctor_cell();
		if (new_cell)
			add_to_list(&manager->energy_cells, new_cell);
		obj = json_object_new_object();
		json_object_object_add(obj, "players", players_list_to_json(manager->players));
		json_object_object_add(obj, "energy_cells", cells_to_json(manager->energy_cells));
		json_object_object_add(obj, "map_size", json_object_new_int(manager->map_size));
		json_object_object_add(obj, "game_status", json_object_new_int(manager->ready));
		send_notif(CYCLE, GLOBAL_TOPIC, obj);
		obj = 0x0;
		new_cell = 0x0;
	}
	pthread_exit(0x0);
}
