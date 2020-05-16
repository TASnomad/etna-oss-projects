/*
** my_malloc.h for malloc in /home/nomad/ETNA-MASTER/DVC4/briand_c
** 
** Made by BARREAU Martin
** Login   <barrea_m_@etna-alternance.net>
** 
** Started on  Mon Jan  7 20:04:45 2019 BARREAU Martin
** Last update Thu Feb  7 11:25:20 2019 BARREAU Martin
*/

#ifndef	__MY_MALLOC_H__
#define	__MY_MALLOC_H__

#include	<unistd.h>
#include	<pthread.h>
#include	<errno.h>

#define	SBRK_FAIL	(void *)(-1)

#define	BLK_FREE	0
#define	BLK_USED	1

#define	BLK_SIZE	sizeof(t_blk)

#define	MIN(x, y)	(((x) < (y)) ? (x) : (y))

extern	pthread_mutex_t		lock;
#define	LOCK_THREAD()	pthread_mutex_lock(&lock);
#define	UNLOCK_THREAD()	pthread_mutex_unlock(&lock);

typedef struct		s_blk
{
  struct s_blk		*next;
  size_t		size;
  char			freed;
  void			*data;
}			t_blk;

void		push_back(t_blk *, t_blk *);
void		push_head(t_blk *, t_blk *);

void		*split_blk(t_blk *, size_t);
int		merge_blk(t_blk *);

t_blk		*find_blk_ptr(t_blk *, void *);
t_blk		*find_blk(t_blk *, size_t);
t_blk		*find_free_blk(t_blk *, void *);

void		*malloc(size_t);
void		free(void *);

void		*my_memset(void *, int, size_t);
void		*my_memcpy(void *, const void *, size_t);

#endif	/* !__MY_MALLOC_H__ */
