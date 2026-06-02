/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** remove command: delete entity with busy-state guards
*/
#include "global_includes.h"

int cmd_remove(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 3) {
        printf("Usage: remove <ingredient|appliance|recipe> <name>\n");
        return SUCCESS;
    }
    const char *type = args[1];
    const char *name = args[2];

    if (strcmp(type, "ingredient") == 0) {
        if (ingredient_remove(ctx->k, name) != SUCCESS)
            printf("Error: ingredient '%s' not found.\n", name);
        else
            printf("Ingredient '%s' removed.\n", name);
        return SUCCESS;
    }

    if (strcmp(type, "appliance") == 0) {
        appliance_t *app = appliance_find(ctx->k, name);
        if (!app) {
            printf("Error: appliance '%s' not found.\n", name);
            return SUCCESS;
        }
        if (app->busy) {
            printf("Error: '%s' is currently busy.\n", name);
            return SUCCESS;
        }
        appliance_remove(ctx->k, name);
        printf("Appliance '%s' removed.\n", name);
        return SUCCESS;
    }

    if (strcmp(type, "recipe") == 0) {
        if (!recipe_find(ctx->k, name)) {
            printf("Error: recipe '%s' not found.\n", name);
            return SUCCESS;
        }
        for (appliance_t *app = ctx->k->appliances; app; app = app->next) {
            if (app->busy && strcmp(app->current_recipe, name) == 0) {
                printf("Error: recipe '%s' is currently being cooked.\n", name);
                return SUCCESS;
            }
        }
        recipe_remove(ctx->k, name);
        printf("Recipe '%s' removed.\n", name);
        return SUCCESS;
    }

    printf("Unknown type '%s'. Use: ingredient, appliance, recipe\n", type);
    return SUCCESS;
}
