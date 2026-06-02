/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** restock command: add quantity to an ingredient
*/
#include "global_includes.h"

int cmd_restock(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 3) {
        printf("Usage: restock <ingredient> <quantity>\n");
        return SUCCESS;
    }
    float qty = (float)atof(args[2]);
    if (qty <= 0.0f) {
        printf("Error: quantity must be positive\n");
        return SUCCESS;
    }
    if (ingredient_restock(ctx->k, args[1], qty) == EXIT_FAIL) {
        printf("Ingredient '%s' not found.\n", args[1]);
        return SUCCESS;
    }
    ingredient_t *ing = ingredient_find(ctx->k, args[1]);
    printf("Restocked %s: now %.1f %s\n", args[1], ing->quantity, ing->unit);
    return SUCCESS;
}
