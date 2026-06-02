/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** info command: display recipe details
*/
#include "global_includes.h"

int cmd_info(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: info <recipe>\n");
        return SUCCESS;
    }
    recipe_t *r = recipe_find(ctx->k, args[1]);
    if (!r) {
        printf("Recipe '%s' not found.\n", args[1]);
        return SUCCESS;
    }
    printf("Recipe: %s\n", r->name);
    printf("Ingredients:\n");
    for (recipe_ingredient_t *ri = r->ingredients; ri; ri = ri->next)
        printf("  - %.1f %s %s\n", ri->quantity, ri->unit, ri->ingredient);
    printf("Appliances:\n");
    for (recipe_appliance_t *ra = r->appliances; ra; ra = ra->next)
        printf("  - %s (%ds)\n", ra->appliance, ra->duration);
    if (r->nb_sub_recipes > 0) {
        printf("Sub-recipes:\n");
        for (int i = 0; i < r->nb_sub_recipes; i++)
            printf("  - %s\n", r->sub_recipes[i]);
    }
    printf("Output: %.1f %s %s\n",
        r->output_quantity, r->output_unit, r->output_ingredient);
    return SUCCESS;
}
