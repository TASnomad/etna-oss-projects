/*
** blk.c for malloc in /home/nomad/ETNA-MASTER/DVC4/briand_c
** 
** Made by BARREAU Martin
** Login   <barrea_m_@etna-alternance.net>
** 
** Started on  Fri Jan 18 10:20:14 2019 BARREAU Martin
** Last update Thu Feb  7 11:25:01 2019 BARREAU Martin
*/

#include	"my_malloc.h"

static t_blk	*blks = NULL;

/*
** Find a free blk_t struct with at least size bytes
*/
t_blk		*find_blk(t_blk *l, size_t size)
{
  t_blk		*iter;

  iter = l;
  while (iter)
    {
      if (iter->size >= (size + BLK_SIZE) && iter->freed == BLK_FREE)
	return (iter);
      iter = iter->next;
    }
  return (NULL);
}

/*
** Find the blk_t struct containing the corresponding ptr address
*/
t_blk		*find_blk_ptr(t_blk *l, void *ptr)
{
  t_blk		*iter;

  iter = l;
  while (iter)
    {
      if (iter->data == ptr)
	return (iter);
      iter = iter->next;
    }
  return (NULL);
}

/*
** same as find_blk_ptr but mark the found blk as free
*/
t_blk		*find_free_blk(t_blk *l, void *ptr)
{
  t_blk		*iter;

  iter = l;
  while (iter)
    {
      if (iter->data == ptr && iter->freed == BLK_USED)
	{
	  iter->freed = BLK_FREE;
	  return (iter);
	}
      iter = iter->next;
    }
  return (NULL);
}

/*
** Split a blk_t struct if the size is bigger than the size parameter
** NOTE: allocate a new memory page, if all bytes are used
*/
void		*split_blk(t_blk *b, size_t size)
{
  t_blk		*n_blk;

  n_blk = (t_blk *) sbrk(size + BLK_SIZE);
  if (n_blk == SBRK_FAIL)
    return (NULL);
  n_blk->next = b->next;
  n_blk->freed = BLK_USED;
  n_blk->data = n_blk + 1;
  n_blk->size = size + BLK_SIZE;
  b->next = n_blk;
  b->size = b->size - n_blk->size;
  return (n_blk->data);
}

/*
** Merge free blocks into a big block
*/
int		merge_blk(t_blk *b)
{
  t_blk		*iter;

  iter = NULL;
  if (!b)
    return (0);
  if (b->freed == BLK_FREE && b->next)
    {
      iter = b->next;
      if (iter->freed == BLK_FREE)
	{
	  b->size = b->size + iter->size;
	  b->next = iter->next;
	  merge_blk(b);
	}
    }
  return (merge_blk(b->next));
}
