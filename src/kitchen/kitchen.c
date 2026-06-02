/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Kitchen lifecycle: create, clear and free
*/
#include "global_includes.h"

kitchen_t *kitchen_create(void)
{
    kitchen_t *k = malloc(sizeof(kitchen_t));

    if (!k)
        return NULL;
    memset(k, 0, sizeof(kitchen_t));
    k->stock_mode = 1;
    return k;
}

void kitchen_clear(kitchen_t *k)
{
    ingredient_t *ing = k->ingredients;
    appliance_t *app;
    recipe_t *r;

    while (ing) {
        ingredient_t *next = ing->next;
        free(ing);
        ing = next;
    }
    app = k->appliances;
    while (app) {
        appliance_t *next = app->next;
        free(app);
        app = next;
    }
    r = k->recipes;
    while (r) {
        recipe_t *next = r->next;
        recipe_free(r);
        r = next;
    }
    memset(k, 0, sizeof(kitchen_t));
    k->stock_mode = 1;
}

void kitchen_free_all(kitchen_t *k)
{
    kitchen_clear(k);
    free(k);
}
