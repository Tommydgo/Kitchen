/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** cook command: trigger recipe cooking
*/
#include "global_includes.h"

int cmd_cook(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: cook <recipe>\n");
        return SUCCESS;
    }
    char visited[MAX_VISITED][64];
    int nb = 0;
    return recipe_cook(ctx->k, args[1], visited, &nb);
}
