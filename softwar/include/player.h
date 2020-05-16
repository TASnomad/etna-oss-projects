#ifndef __PLAYER_H__
#define __PLAYER_H__

// Definition of a game player

#include	<list.h>
#include	<json-c/json.h>


#define		MAX_ENERGY	100	// Max energy count for one player
#define		ENERGY_STARTER	50	// Energy count when the game start
#define		ACTION_STARTER	10	// Max allowed actions by cycle
#define		ENERGY_RETAIN	2

typedef	unsigned int uint;

typedef struct	s_player
{
	char	*id;
	uint	x;	// X position
	uint	y;	// Y position
	int	disabled; // -1 => dead, 0 => normal, 1 => stun, 2 => stun
	uint	energy;
	uint	looking;
	uint	action;
}		player_t;

player_t	*ctor_player(char *, int);
void		dtor_player(player_t *);

/**
 * @param:
 * 1- the player struct pointer
 *
 * @return:
 *  The JSON Object struct representing the player
 */
struct json_object	*player_to_json(player_t *);

/**
 * @param:
 * 1- The doubly linked list representing all players to transformation in JSON
 *
 * @return;
 *  The JSON Object struct representing an player JSON array
 */
struct json_object	*players_list_to_json(t_list *);

#endif /* ! __PLAYER_H__ */
