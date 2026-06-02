CC      = gcc
CFLAGS  = -Wall -Wextra -Iincludes -Ilibs/cjson
NAME    = kitchen

SRC =   src/main.c                              \
        src/kitchen/kitchen.c                   \
        src/kitchen/ingredient.c                \
        src/kitchen/appliance.c                 \
        src/kitchen/recipe.c                    \
        src/kitchen/time.c                      \
        src/repl/repl.c                         \
        src/repl/cmd_table.c                    \
        src/repl/commands/cmd_cook.c            \
        src/repl/commands/cmd_wait.c            \
        src/repl/commands/cmd_status.c          \
        src/repl/commands/cmd_list.c            \
        src/repl/commands/cmd_info.c            \
        src/repl/commands/cmd_restock.c         \
        src/repl/commands/cmd_mode.c            \
        src/repl/commands/cmd_reset.c           \
        src/repl/commands/cmd_help.c            \
        src/repl/commands/cmd_exit.c            \
        src/repl/commands/cmd_add.c             \
        src/repl/commands/cmd_remove.c          \
        src/datasource/datasource.c             \
        src/datasource/json_config.c            \
        src/datasource/json_state.c             \
        src/datasource/json_utils.c             \
        libs/cjson/cJSON.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

debug:
	$(CC) -g3 $(CFLAGS) $(SRC) -o debug_$(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f *.gcda *.gcno *.pch

fclean: clean
	rm -f $(NAME) debug_$(NAME)

re: fclean all

.PHONY: all debug clean fclean re
