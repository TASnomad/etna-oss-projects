#ifndef	__PLAYER_H__
#define	__PLAYER_H__

#define	GAME_CAPACITY	4

typedef struct	s_player
{
	char	alive;
	char	connected;
	int	num;
	int	_x;
	int	_y;
	int	direction;
	int	speed;
	int	bombs;
	int	max_bombs;
	int	frags;
	int	fd;
}		t_player;

#endif	/* !__PLAYER_H__ */
