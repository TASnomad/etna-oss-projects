NAME=http-server
NAME_TEST=http-server-test
CC=gcc
MAIN_FILES=src/main.c
CHEROKEE_FILES=src/cherokee.c
PROFILING_FILES=src/cherokee_profiling.c
TEST_FILES=test/parse_request.c \
           test/make_response.c \
	   test/router_internal.c

SOURCE_FILES=src/MegaMimes.c \
			 src/io.c \
			 src/routes.c \
			 src/router.c \
			 src/syntax.c \
			 src/http.c \
			 src/response.c \
			 src/list.c	\
		  	 src/server.c \
			 src/node.c
SRC=$(MAIN_FILES) \
	$(CHEROKEE_FILES) \
	$(SOURCE_FILES)
TEST=$(TEST_FILES) \
     $(SOURCE_FILES)
SRC_PROFILING=$(MAIN_FILES) \
		$(PROFILING_FILES) \
		$(SOURCE_FILES)
OBJ=$(SRC:%.c=%.o)
OBJ_PROFILING=$(SRC_PROFILING:%.c=%.o)
OBJ_TEST=$(TEST:%.c=%.o)
CFLAGS=-W -Wall -Werror -Wextra -std=c99 -I./include -D_GNU_SOURCE
STD_DIR=std/

all: $(OBJ)
	 make -C $(STD_DIR)
	 $(CC) $(CFLAGS) $(OBJ) -L $(STD_DIR) -lstd -lpthread -o $(NAME)

clean:
	   rm -rf $(OBJ)
	   make clean -C $(STD_DIR)

fclean:	clean
		make fclean -C $(STD_DIR)
		rm -rf $(NAME)

re: fclean all

test: $(OBJ_TEST)
	  make -C $(STD_DIR)
	  $(CC) $(OBJ_TEST) -L $(STD_DIR) -lstd -lcriterion -lpthread -o $(NAME_TEST)

clean-test: clean
			rm -rf $(OBJ_TEST)

fclean-test: clean-test fclean
			 rm -rf $(NAME_TEST)

re-test: fclean-test test

profiling:	$(OBJ_PROFILING)
		make -C $(STD_DIR)
		$(CC) $(CFLAGS) $(OBJ_PROFILING) -pg -L $(STD_DIR) -lstd -lpthread -o $(NAME)

callgrind: re
		valgrind --tool=callgrind ./http-server

kcachegrind:
		kcachegrind ${1}

run\:infra:
	cd .docker && sudo docker-compose up --build --remove-orphans

.PHONY:	all clean fclean re test clean-test fclean-test profiling callgrind kcachegrind run\:infra
