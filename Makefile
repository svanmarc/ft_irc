CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./include
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
