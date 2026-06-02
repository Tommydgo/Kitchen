CC      = gcc
CFLAGS  = -Wall -Iincludes -Ilibs/cjson
OBJS    = src/kitchen/kitchen.o src/kitchen/ingredient.o   \
          src/kitchen/appliance.o src/kitchen/recipe.o     \
          src/kitchen/time.o                               \
          src/datasource/datasource.o                      \
          src/datasource/json_config.o                     \
          src/datasource/json_state.o                      \
          src/datasource/json_utils.o                      \
          libs/cjson/cJSON.o

REPL_OBJS = src/repl/repl.o src/repl/cmd_table.o \
            src/repl/commands/cmd_cook.o src/repl/commands/cmd_wait.o \
            src/repl/commands/cmd_status.o src/repl/commands/cmd_list.o \
            src/repl/commands/cmd_info.o src/repl/commands/cmd_restock.o \
            src/repl/commands/cmd_mode.o src/repl/commands/cmd_reset.o \
            src/repl/commands/cmd_help.o src/repl/commands/cmd_exit.o

test_json_config: tests/test_json_config.c $(OBJS)
	$(CC) $(CFLAGS) tests/test_json_config.c $(OBJS) -o _test_json_config
	./_test_json_config
	rm _test_json_config

test_json_state: tests/test_json_state.c $(OBJS)
	$(CC) $(CFLAGS) tests/test_json_state.c $(OBJS) -o _test_json_state
	./_test_json_state
	rm _test_json_state

test_ingredient: tests/test_ingredient.c $(OBJS)
	$(CC) $(CFLAGS) tests/test_ingredient.c $(OBJS) -o _test_ingredient
	./_test_ingredient
	rm _test_ingredient

test_appliance: tests/test_appliance.c $(OBJS)
	$(CC) $(CFLAGS) tests/test_appliance.c $(OBJS) -o _test_appliance
	./_test_appliance
	rm _test_appliance

test_recipe: tests/test_recipe.c $(OBJS)
	$(CC) $(CFLAGS) tests/test_recipe.c $(OBJS) -o _test_recipe
	./_test_recipe
	rm _test_recipe

test_repl_dispatch: tests/test_repl_dispatch.c $(OBJS) $(REPL_OBJS)
	$(CC) $(CFLAGS) tests/test_repl_dispatch.c $(OBJS) $(REPL_OBJS) \
	      -o _test_repl_dispatch
	./_test_repl_dispatch
	rm _test_repl_dispatch

.PHONY: test_json_config test_json_state test_ingredient test_appliance test_recipe test_repl_dispatch
