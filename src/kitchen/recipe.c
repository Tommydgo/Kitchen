/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Recipe lookup and recursive cooking logic with cycle detection
*/
#include "global_includes.h"

recipe_t *recipe_find(kitchen_t *k, const char *name)
{
    for (int i = 0; i < k->nb_recipes; i++) {
        if (strcmp(k->recipes[i].name, name) == 0)
            return &k->recipes[i];
    }
    return NULL;
}

static int is_visited(const char *name, char visited[][64], int nb)
{
    for (int i = 0; i < nb; i++) {
        if (strcmp(visited[i], name) == 0)
            return 1;
    }
    return 0;
}

int recipe_cook(kitchen_t *k, const char *name,
                char visited[][64], int *nb_visited)
{
    if (is_visited(name, visited, *nb_visited)) {
        printf("Error: circular dependency detected at '%s'\n", name);
        return EXIT_FAIL;
    }
    recipe_t *recipe = recipe_find(k, name);
    if (!recipe) {
        printf("Error: recipe '%s' not found\n", name);
        return EXIT_FAIL;
    }

    strncpy(visited[*nb_visited], name, 63);
    (*nb_visited)++;

    /* Resolve sub-recipes */
    for (int i = 0; i < recipe->nb_sub_recipes; i++) {
        const char *sub = recipe->sub_recipes[i];
        ingredient_t *sub_ing = ingredient_find(k, sub);
        int need_cook = (!sub_ing
            || (!sub_ing->unlimited && sub_ing->quantity < 1.0f));
        if (need_cook) {
            printf("[auto] cooking %s...\n", sub);
            if (recipe_cook(k, sub, visited, nb_visited) != SUCCESS) {
                (*nb_visited)--;
                return EXIT_FAIL;
            }
            /* Auto-advance time if the sub-recipe started an appliance */
            recipe_t *sub_r = recipe_find(k, sub);
            if (sub_r && sub_r->nb_appliances > 0) {
                int max_t = 0;
                for (int j = 0; j < sub_r->nb_appliances; j++) {
                    appliance_t *app = appliance_find(k,
                        sub_r->appliances[j].appliance);
                    if (app && app->busy && app->time_left > max_t)
                        max_t = app->time_left;
                }
                if (max_t > 0) {
                    printf("[auto] waiting %ds for %s...\n", max_t, sub);
                    kitchen_advance_time(k, max_t);
                }
            }
        }
    }

    /* Check direct ingredient stocks (real mode only) */
    if (k->stock_mode == 1) {
        for (int i = 0; i < recipe->nb_ingredients; i++) {
            recipe_ingredient_t *ri = &recipe->ingredients[i];
            ingredient_t *ing = ingredient_find(k, ri->ingredient);
            if (!ing || (!ing->unlimited && ing->quantity < ri->quantity)) {
                printf("Error: insufficient '%s' (need %.1f %s)\n",
                    ri->ingredient, ri->quantity, ri->unit);
                (*nb_visited)--;
                return EXIT_FAIL;
            }
        }
    }

    /* Check appliances */
    for (int i = 0; i < recipe->nb_appliances; i++) {
        if (!appliance_find_free(k, recipe->appliances[i].appliance)) {
            printf("Error: no free '%s' available\n",
                recipe->appliances[i].appliance);
            (*nb_visited)--;
            return EXIT_FAIL;
        }
    }

    /* Consume ingredients */
    if (k->stock_mode == 1) {
        for (int i = 0; i < recipe->nb_ingredients; i++) {
            recipe_ingredient_t *ri = &recipe->ingredients[i];
            ingredient_consume(k, ri->ingredient, ri->quantity);
        }
    }

    /* Mark appliances busy */
    for (int i = 0; i < recipe->nb_appliances; i++) {
        appliance_t *app = appliance_find_free(k,
            recipe->appliances[i].appliance);
        app->busy = 1;
        app->time_left = recipe->appliances[i].duration;
        strncpy(app->current_recipe, name, 63);
    }

    /* Instant recipe (no appliances): add output immediately */
    if (recipe->nb_appliances == 0 && recipe->output_ingredient[0] != '\0') {
        ingredient_add(k, recipe->output_ingredient,
            recipe->output_quantity, recipe->output_unit);
        printf("[done] %s is ready!\n", name);
    } else if (recipe->nb_appliances > 0) {
        printf("[start] cooking %s (%s, %ds)... Use 'wait <N>' to advance time.\n",
            name,
            recipe->appliances[0].appliance,
            recipe->appliances[0].duration);
    }

    (*nb_visited)--;
    return SUCCESS;
}
