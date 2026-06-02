/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** exit command: save state and terminate REPL
*/
#include "global_includes.h"

int cmd_exit(repl_ctx_t *ctx, char **args, int argc)
{
    (void)args; (void)argc;
    if (ctx->ds->save_state(ctx->save_path, ctx->k) != SUCCESS)
        printf("Warning: could not save kitchen state.\n");
    else
        printf("Kitchen saved. Goodbye!\n");
    return REPL_EXIT;
}
