/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** REPL main loop and command dispatcher
*/
#include "global_includes.h"

#define REPL_PROMPT "kitchen> "
#define REPL_MAXLINE 256
#define REPL_MAXTOK  16

void repl_run(repl_ctx_t *ctx)
{
    char line[REPL_MAXLINE];
    char *tokens[REPL_MAXTOK];
    int nb;

    printf("Virtual Kitchen - type 'help' for commands\n");
    while (1) {
        printf(REPL_PROMPT);
        fflush(stdout);
        if (!fgets(line, REPL_MAXLINE, stdin))
            break;
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0')
            continue;
        nb = 0;
        char *tok = strtok(line, " \t");
        while (tok && nb < REPL_MAXTOK) {
            tokens[nb++] = tok;
            tok = strtok(NULL, " \t");
        }
        if (nb == 0)
            continue;
        if (repl_dispatch(ctx, tokens, nb) == REPL_EXIT)
            break;
    }
}

int repl_dispatch(repl_ctx_t *ctx, char **args, int argc)
{
    for (int i = 0; i < cmd_table_size; i++) {
        if (strcmp(cmd_table[i].name, args[0]) == 0)
            return cmd_table[i].handler(ctx, args, argc);
    }
    printf("Unknown command '%s'. Type 'help' for commands.\n", args[0]);
    return SUCCESS;
}
