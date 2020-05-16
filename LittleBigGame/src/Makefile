CXX			= g++
CXXFLAGS	= -Wall -Werror -Wextra -g3 -std=c++11
SRC			= 	main.cpp			\
				AbstractEntity.cpp	\
				Scene.cpp			\
				MonsterFactory.cpp	\
				ZombieMonster.cpp	\
				SprinterMonster.cpp	\
				Character.cpp		\
				TimeManager.cpp		\
				WindowManager.cpp	\
				TileMap.cpp		\
				BobMonster.cpp 		\
				RonflexMonster.cpp      \
				PlanktonMonster.cpp      \
				Player.cpp              \
				Comparator.cpp          \
				json.cpp
OBJS		= $(SRC:%.cpp=%.o)
NAME		= my_little_big_game
RM			= rm -f

all:		$(NAME)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

$(NAME):	$(OBJS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
