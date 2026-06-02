/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** help command: print available commands and usage
*/
#include "global_includes.h"

int cmd_help(repl_ctx_t *ctx, char **args, int argc)
{
    (void)ctx; (void)args; (void)argc;
    printf("Available commands:\n");
    for (int i = 0; i < cmd_table_size; i++)
        printf("  %-10s %s\n", cmd_table[i].name, cmd_table[i].usage);
    return SUCCESS;
}
