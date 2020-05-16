NAME=libstd.a
CC=gcc
CFLAGS=-W -Wall -Werror -Wextra -std=c99 -I../include -D_GNU_SOURCE -pg
SRC=	my_string.c \
	my_integer.c \
	thpool.c
OBJ=$(SRC:%.c=%.o)
RAN=ranlib
RM=rm -f
AR=ar rc

all:	$(OBJ)
	$(AR) $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all
