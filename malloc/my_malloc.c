/*
** my_malloc.c for malloc in /home/nomad/ETNA-MASTER/DVC4
** 
** Made by BARREAU Martin
** Login   <barrea_m_@etna-alternance.net>
** 
** Started on  Mon Jan  7 09:39:57 2019 BARREAU Martin
** Last update Thu Feb  7 11:30:00 2019 BARREAU Martin
*/


#include	"my_malloc.h"

/*
** Global reference containing all memory blocks
*/
static	t_blk	*blks = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void		*malloc(size_t size)
{
  t_blk		*blk;
  void		*tmp;

  tmp = NULL;
  LOCK_THREAD();
  blk = find_blk(blks, size);
  if (blk)
    {
      tmp = split_blk(blk, size);
      UNLOCK_THREAD();
      return (tmp);
    }
  blk = (t_blk *) sbrk(size + BLK_SIZE);
  if (blk == SBRK_FAIL)
    {
      UNLOCK_THREAD();
      errno = ENOMEM;
      return (NULL);
    }
  blk->next = NULL;
  blk->freed = BLK_USED;
  blk->data =  blk + 1;
  blk->size = size + BLK_SIZE;
  if (!blks)
      blks = blk;
  else
    push_back(blks, blk);
  UNLOCK_THREAD();
  return (blk->data);
}

void		*realloc(void *ptr, size_t size)
{
  t_blk		*lookup;
  void		*n_blk;
  size_t	cp_size;

  lookup = NULL;
  n_blk = NULL;
  cp_size = 0;
  if (ptr && !size)
    {
      free(ptr);
      return (NULL);
    }
  if (!ptr)
    return (malloc(size));
  lookup = find_blk_ptr(blks, ptr);
  if (!lookup)
    {
      n_blk = malloc(size);
      free(ptr);
      return (n_blk);
    }
  n_blk = malloc(size);
  cp_size = MIN((lookup->size - BLK_SIZE), size);
  LOCK_THREAD();
  my_memcpy(n_blk, ptr, cp_size);
  UNLOCK_THREAD();
  free(ptr);
  return (n_blk);
}

void		*calloc(size_t nbelem, size_t size)
{
  void		*ptr;

  ptr = NULL;
  if (!nbelem || !size)
    return (ptr);
  ptr = malloc(nbelem * size);
  if (ptr)
    {
      LOCK_THREAD();
      my_memset(ptr, 0, nbelem * size);
      UNLOCK_THREAD();
    }
  else
    errno = ENOMEM;
  return (ptr);
}

void		free(void *ptr)
{
  void		*blk;

  LOCK_THREAD();
  blk = find_free_blk(blks, ptr);
  if (blk)
    push_head(blks, blk);
  merge_blk(blks);
  UNLOCK_THREAD();
}
