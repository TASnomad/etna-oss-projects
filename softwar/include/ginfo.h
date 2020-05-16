#ifndef __GINFO_H__
#define __GINFO_H__

// Definition of a struct represeting the game map

#include		<json-c/json.h>

#include		<list.h>

typedef unsigned int 	uint;

typedef struct		s_game_info
{
	uint		map_size;
	uint		status;
	int		ready;
	int		change;
}			game_info_t;

struct json_object	*ginfo_to_json();

#endif /* ! __GINFO_H__ */
