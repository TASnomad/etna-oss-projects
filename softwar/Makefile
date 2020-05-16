CC	= gcc
NAME	= server

OS := $(shell uname)


CFLAGS = -W -Wextra -Werror -Wall -Iinclude -g3 -pthread
LDFLAGS = -lzmq -lczmq -g3

ifeq ($(OS), Linux)
	CFLAGS += -rdynamic $(shell pkg-config --cflags json-c)
	LDFLAGS += -gstabs $(shell pkg-config --libs json-c)
else
	CFLAGS +=  -I/usr/local/include/json-c
	LDFLAGS += -L/usr/local/lib/ -ljson-c
endif

SRC 	= src/manager.c		\
	  src/utils/map.c	\
	  src/cell.c		\
	  src/cycle.c		\
	  src/utils/parser.c	\
	  src/utils/notif.c	\
	  src/utils/network.c	\
	  src/player.c		\
	  src/command.c		\
	  src/game_ctx.c	\
	  src/utils/list.c	\
	  src/utils/args.c	\
	  main.c

OBJS	= $(SRC:%.c=%.o)
RM	= rm -f

all:	$(OBJS) $(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

.PHONY:	all clean fclean

re:	fclean all

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<
