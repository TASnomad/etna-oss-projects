#ifndef __ENUMS_H__
#define __ENUMS_H__

typedef enum 	position
{
	LEFT = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
}		position_t;

typedef enum	NotificationType
{
	CYCLE = 0,
	GAME_START = 1,
	GAME_END = 2,
	CLIENT_DEAD = 3,
	CLIENT_WIN = 4,
}		NotificationType_t;

#endif /* ! __ENUMS_H__ */
