/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** mode command: switch between real and sim stock modes
*/
#include "global_includes.h"

int cmd_mode(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: mode <real|sim>\n");
        return SUCCESS;
    }
    kitchen_t *k = ctx->k;
    if (strcmp(args[1], "real") == 0) {
        k->stock_mode = 1;
        for (int i = 0; i < k->nb_ingredients; i++)
            k->ingredients[i].unlimited = 0;
        printf("Mode: real stock\n");
    } else if (strcmp(args[1], "sim") == 0) {
        k->stock_mode = 0;
        for (int i = 0; i < k->nb_ingredients; i++)
            k->ingredients[i].unlimited = 1;
        printf("Mode: unlimited (simulation)\n");
    } else {
        printf("Unknown mode '%s'. Use: real, sim\n", args[1]);
    }
    return SUCCESS;
}
