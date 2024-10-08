CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./include -g
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
NAME = build/ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -rf obj

fclean: clean
	rm -f $(NAME)
	rm -rf build

re: fclean all

.PHONY: all clean fclean re