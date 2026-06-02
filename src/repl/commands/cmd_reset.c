/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** reset command: reload kitchen config from file
*/
#include "global_includes.h"

int cmd_reset(repl_ctx_t *ctx, char **args, int argc)
{
    (void)args; (void)argc;
    kitchen_clear(ctx->k);
    if (ctx->ds->load_config(ctx->config_path, ctx->k) != SUCCESS) {
        printf("Error: failed to reload '%s'\n", ctx->config_path);
        return EXIT_FAIL;
    }
    printf("Kitchen reset to initial state.\n");
    return SUCCESS;
}
