CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./include
SRC = $(shell find src -name '*.cpp')
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -rf obj

fclean: clean
	rm -f $(NAME)
	rm -rf build

re: fclean all

run: re
	./$(NAME) 6668 pw

.PHONY: all clean fclean re run