/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** list command: display recipes, ingredients or appliances
*/
#include "global_includes.h"

int cmd_list(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: list <recipes|ingredients|appliances>\n");
        return SUCCESS;
    }
    kitchen_t *k = ctx->k;
    if (strcmp(args[1], "recipes") == 0) {
        for (recipe_t *r = k->recipes; r; r = r->next)
            printf("  - %s\n", r->name);
    } else if (strcmp(args[1], "ingredients") == 0) {
        for (ingredient_t *ing = k->ingredients; ing; ing = ing->next) {
            if (ing->unlimited)
                printf("  - %s: unlimited\n", ing->name);
            else
                printf("  - %s: %.1f %s\n",
                    ing->name, ing->quantity, ing->unit);
        }
    } else if (strcmp(args[1], "appliances") == 0) {
        for (appliance_t *a = k->appliances; a; a = a->next) {
            if (a->busy)
                printf("  - %s: busy (%s, %ds left)\n",
                    a->name, a->current_recipe, a->time_left);
            else
                printf("  - %s: free\n", a->name);
        }
    } else {
        printf("Unknown target '%s'. Use: recipes, ingredients, appliances\n",
            args[1]);
    }
    return SUCCESS;
}
