#ifndef	__MAP_H__
#define	__MAP_H__

#include		"proto.h"

#define MAP_SIZE 16

/**
 * Cell is char type
 * bits usage:
 * [0] 0: nothing / 1: bomb blast
 * [1 & 2] 00: empty cell / 10: solid wall (bomb can't explose) / 11: weak wall (when explose become empty cell)
 * [3] 1: cell has a bomb / 0: no bomb in cell
 * [4-5]: Bomb owner if 3rd bit is set otherwise bonus
 * [5-7]: Bonus if 3rd bit is clear
 */

typedef enum		e_direction
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
}			t_direction;

#define	EMPTY_CELL	00
#define	SOLID_WALL	10
#define	WEAK_WALL	11
#define	PLAYER_IN_CELL	01
#define	BOMB_IN_CELL	10

#define	BOMB_RANGE_INC	000
#define	BOMB_RANGE_DEC	001
#define	SLOT_INC		010
#define	SLOT_DEC		011
#define	SPEED_INC		100
#define	SPEED_DEC		101
#define	ADD_BOMB		110

#define	CLEAR_BIT(val, bit)	(val = val & ~(bit << 1))
#define	GET_BIT(cell, num)	((cell >> num) & 1)
#define	SET_BIT(cell, num, val)	(cell ^= (-val ^ cell) & (1 << num))

#define	HAS_BOMB(cell) ((cell >> 3) & 1)

#define	IS_NOT_WALL(cell)	(!(GET_BIT(cell, 1)) && !(GET_BIT(cell, 2)))
#define	IS_PLAYER_IN(cell)	(!GET_BIT(cell, 1) && GET_BIT(cell, 2))

#define	CLEAR_CELL(cell)		\
	CLEAR_BIT(cell, 1);			\
	CLEAR_BIT(cell, 2);


#define	REMOVE_WALL(cell)		\
	if (IS_WEAK_WALL(cell))		\
	{				\
		CLEAR_BIT(cell, 1);	\
		CLEAR_BIT(cell, 2);	\
	}

#define	SET_BOMB_OWNER(cell, num)	\
	SET_BIT(cell, 3, 1);			\
	switch (num)					\
	{								\
		case 0:						\
			CLEAR_BIT(cell, 4);		\
			CLEAR_BIT(cell, 5);		\
		break;						\
		case 1:						\
			CLEAR_BIT(cell, 4);		\
			SET_BIT(cell, 5, 1);	\
		break;						\
		case 2:						\
			SET_BIT(cell, 4, 1);	\
			CLEAR_BIT(cell, 5);		\
		break;						\
		case 3:						\
			SET_BIT(cell, 4, 1);	\
			SET_BIT(cell, 5, 1);	\
		break;						\
	}

#define CLEAR_BOMB(cell)			\
	CLEAR_BIT(cell, 0);				\
	CLEAR_FIRE(cell);				\
	CLEAR_BIT(cell, 3);				\
	CLEAR_BIT(cell, 4);				\
	CLEAR_BIT(cell, 5);				\

#define	BOMB_BLAST(cell)	(cell = cell | (1 << 0))

#define	FIRE_CELL(cell)		SET_BIT(cell, 0, 1)
#define	CLEAR_FIRE(cell)	SET_BIT(cell, 0, 0)

#define	HAS_BONUS_OR_MALUS(cell)		((!HAS_BOMB(cell)) ? (GET_BIT(cell, 4) == 1) : (0))
#define	IS_BONUS_CELL(cell)	((HAS_BONUS_OR_MALUS(cell)) ? (GET_BIT(cell, 7) == 1) : (0))
#define	IS_MALUS_CELL(cell)	((HAS_BONUS_OR_MALUS(cell)) ? (!GET_BIT(cell, 7)) : (0))s

int			*ctor_map(int *, int size);
void		set_player_coords(t_server *, int);
int			move_player(t_server *, int, t_direction);
int			plant_bomb(t_server *, int);

/* Using a different thread to wait until the explosion will happen */
void		*bomb_blast_handler(void *);

#endif	/* !__MAP_H__ */
