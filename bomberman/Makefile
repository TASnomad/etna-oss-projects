CC	= gcc
CFLAGS	= -Wall -Werror -Wextra -pthread -g3 -I./include
SRC	= src/server/cmd.c				\
	  src/map.c					\
	  src/server/server.c 				\
	  main.c					\
	  src/display/window.c				\
	  src/display/gameScreen.c			\
	  src/display/utils.c				\
	  src/display/button.c				\
	  src/display/text.c				\
	  src/display/display.c				\
	  src/display/loadingScreen.c		\
	  src/display/homeScreen.c			\
	  src/display/chooseServerScreen.c	\
	  src/display/LoadingPartyScreen.c	\
	  src/display/ServerWaitingScreen.c	\
	  src/display/textures.c			\
	  src/display/initSDL.c				\
	  src/event/gameEvent.c				\
	  src/event/HomeScreenEvents.c		\
	  src/event/ChooseServerEvents.c	\
	  src/event/MouseClick.c
OBJS	= $(SRC:%.c=%.o)
NAME	= my_bomberman
RM	= rm -f

all:		$(NAME)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -rdynamic

$(NAME):	$(OBJS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

test:
	$(CC) $(CFLAGS) -o test tests/test.c

.PHONY:		all clean fclean re test

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<
