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
        for (int i = 0; i < k->nb_recipes; i++)
            printf("  - %s\n", k->recipes[i].name);
    } else if (strcmp(args[1], "ingredients") == 0) {
        for (int i = 0; i < k->nb_ingredients; i++) {
            if (k->ingredients[i].unlimited)
                printf("  - %s: unlimited\n", k->ingredients[i].name);
            else
                printf("  - %s: %.1f %s\n", k->ingredients[i].name,
                    k->ingredients[i].quantity, k->ingredients[i].unit);
        }
    } else if (strcmp(args[1], "appliances") == 0) {
        for (int i = 0; i < k->nb_appliances; i++) {
            appliance_t *a = &k->appliances[i];
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
