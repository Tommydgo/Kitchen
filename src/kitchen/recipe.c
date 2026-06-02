/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Recipe creation, free, lookup, removal and recursive cooking with cycle detection
*/
#include "global_includes.h"

recipe_ingredient_t *recipe_ingredient_create(const char *name, float qty,
                                              const char *unit)
{
    recipe_ingredient_t *ri = malloc(sizeof(recipe_ingredient_t));

    if (!ri)
        return NULL;
    strncpy(ri->ingredient, name, 63);
    ri->ingredient[63] = '\0';
    ri->quantity = qty;
    strncpy(ri->unit, unit, 15);
    ri->unit[15] = '\0';
    ri->next = NULL;
    return ri;
}

recipe_appliance_t *recipe_appliance_create(const char *name, int duration)
{
    recipe_appliance_t *ra = malloc(sizeof(recipe_appliance_t));

    if (!ra)
        return NULL;
    strncpy(ra->appliance, name, 63);
    ra->appliance[63] = '\0';
    ra->duration = duration;
    ra->next = NULL;
    return ra;
}

recipe_t *recipe_create(const char *name)
{
    recipe_t *r = malloc(sizeof(recipe_t));

    if (!r)
        return NULL;
    memset(r, 0, sizeof(recipe_t));
    strncpy(r->name, name, 63);
    r->name[63] = '\0';
    return r;
}

void recipe_free(recipe_t *r)
{
    recipe_ingredient_t *ri = r->ingredients;
    recipe_appliance_t *ra;

    while (ri) {
        recipe_ingredient_t *next = ri->next;
        free(ri);
        ri = next;
    }
    ra = r->appliances;
    while (ra) {
        recipe_appliance_t *next = ra->next;
        free(ra);
        ra = next;
    }
    for (int i = 0; i < r->nb_sub_recipes; i++)
        free(r->sub_recipes[i]);
    free(r->sub_recipes);
    free(r);
}

recipe_t *recipe_find(kitchen_t *k, const char *name)
{
    for (recipe_t *r = k->recipes; r; r = r->next) {
        if (strcmp(r->name, name) == 0)
            return r;
    }
    return NULL;
}

int recipe_remove(kitchen_t *k, const char *name)
{
    recipe_t *prev = NULL;

    for (recipe_t *cur = k->recipes; cur; cur = cur->next) {
        if (strcmp(cur->name, name) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                k->recipes = cur->next;
            recipe_free(cur);
            k->nb_recipes--;
            return SUCCESS;
        }
        prev = cur;
    }
    return EXIT_FAIL;
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
            recipe_t *sub_r = recipe_find(k, sub);
            if (sub_r && sub_r->appliances != NULL) {
                int max_t = 0;
                for (recipe_appliance_t *ra = sub_r->appliances; ra; ra = ra->next) {
                    appliance_t *app = appliance_find(k, ra->appliance);
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

    /* Check ingredient stocks (real mode only) */
    if (k->stock_mode == 1) {
        for (recipe_ingredient_t *ri = recipe->ingredients; ri; ri = ri->next) {
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
    for (recipe_appliance_t *ra = recipe->appliances; ra; ra = ra->next) {
        if (!appliance_find_free(k, ra->appliance)) {
            printf("Error: no free '%s' available\n", ra->appliance);
            (*nb_visited)--;
            return EXIT_FAIL;
        }
    }

    /* Consume ingredients */
    if (k->stock_mode == 1) {
        for (recipe_ingredient_t *ri = recipe->ingredients; ri; ri = ri->next)
            ingredient_consume(k, ri->ingredient, ri->quantity);
    }

    /* Mark appliances busy */
    for (recipe_appliance_t *ra = recipe->appliances; ra; ra = ra->next) {
        appliance_t *app = appliance_find_free(k, ra->appliance);
        app->busy = 1;
        app->time_left = ra->duration;
        strncpy(app->current_recipe, name, 63);
    }

    /* Instant recipe: add output immediately */
    if (recipe->appliances == NULL && recipe->output_ingredient[0] != '\0') {
        ingredient_add(k, recipe->output_ingredient,
            recipe->output_quantity, recipe->output_unit);
        printf("[done] %s is ready!\n", name);
    } else if (recipe->appliances != NULL) {
        printf("[start] cooking %s (%s, %ds)... Use 'wait <N>' to advance time.\n",
            name,
            recipe->appliances->appliance,
            recipe->appliances->duration);
    }

    (*nb_visited)--;
    return SUCCESS;
}
