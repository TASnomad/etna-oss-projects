##
## Makefile for malloc in /home/nomad/ETNA-MASTER/DVC4
## 
## Made by BARREAU Martin
## Login   <barrea_m_@etna-alternance.net>
## 
## Started on  Mon Jan  7 08:43:16 2019 BARREAU Martin
## Last update Wed Jan 30 08:22:26 2019 BARREAU Martin
##

CC		= gcc
CFLAGS		= -W -Werror -Wextra -fPIC -g3 -pthread
LIBNAME		= libmy_malloc_$(shell uname).so
LNNAME		= libmy_malloc.so
LIBFLAGS	= -shared -Wl,-soname,$(LIBNAME)
SRC		= 	utils.c	\
			blk.c	\
			my_malloc.c
OBJS		= $(SRC:%.c=%.o)
RM		= rm -f
LN		= ln -s

## Preventing sbrk deprecated error while compiling on Darwin platform
ifeq ($(shell uname), Darwin)
	CFLAGS += -Wno-deprecated-declarations
	LIBFLAGS = -shared -dynamiclib
endif

all:	$(OBJS) $(LIBNAME)

$(LIBNAME):	$(OBJS)
	$(CC) $(LIBFLAGS) -o $(LIBNAME) $(OBJS)
	$(LN) $(LIBNAME) $(LNNAME)
clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(LIBNAME)
	$(RM) $(LNNAME)

.PHONY:		all clean fclean

re:	fclean all

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<
