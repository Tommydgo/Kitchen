/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** wait command: advance simulated kitchen time
*/
#include "global_includes.h"

int cmd_wait(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: wait <seconds>\n");
        return SUCCESS;
    }
    int seconds = atoi(args[1]);
    if (seconds <= 0) {
        printf("Error: wait time must be a positive integer\n");
        return SUCCESS;
    }
    kitchen_advance_time(ctx->k, seconds);
    printf("Time advanced by %ds (total: %ds)\n",
        seconds, ctx->k->simulated_time);
    return SUCCESS;
}
