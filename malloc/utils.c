/*
** utils.c for malloc in /home/nomad/ETNA-MASTER/DVC4/briand_c
** 
** Made by BARREAU Martin
** Login   <barrea_m_@etna-alternance.net>
** 
** Started on  Thu Jan 10 09:16:49 2019 BARREAU Martin
** Last update Thu Feb  7 11:25:44 2019 BARREAU Martin
*/

#include	"my_malloc.h"

void		push_back(t_blk *list, t_blk *b)
{
  t_blk		*iter;

  iter = list;
  while (iter->next)
    iter = iter->next;
  iter->next = b;
}

void		push_head(t_blk *list, t_blk *b)
{
  t_blk		*front;
  t_blk		*tmp;

  tmp = NULL;
  front = (t_blk *) sbrk(BLK_SIZE + b->size);
  if (front == SBRK_FAIL)
    return ;
  front->size = b->size;
  front->data = front + 1;
  front->next = list;
  list = front;
  tmp = list;
  while (tmp->next && tmp->next != b)
    tmp = tmp->next;
  tmp->next = b->next;
}

void		*my_memset(void *src, int c, size_t size)
{
  unsigned char	*s;

  s = (unsigned char *) src;
  if (s)
    while(--size)
      *s++ = (unsigned char) c;
  return (src);
}

void		*my_memcpy(void *dest, const void *src, size_t size)
{
  char		*d;
  char		*s;

  d = (char *) dest;
  s = (char *) src;

  while(size--)
    *d++ = *s++;
  return (dest);
}
