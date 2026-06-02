/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Ingredient find, consume, restock and add operations
*/
#include "global_includes.h"

ingredient_t *ingredient_find(kitchen_t *k, const char *name)
{
    for (int i = 0; i < k->nb_ingredients; i++) {
        if (strcmp(k->ingredients[i].name, name) == 0)
            return &k->ingredients[i];
    }
    return NULL;
}

int ingredient_consume(kitchen_t *k, const char *name, float qty)
{
    ingredient_t *ing = ingredient_find(k, name);
    if (!ing)
        return EXIT_FAIL;
    if (ing->unlimited)
        return SUCCESS;
    if (ing->quantity < qty)
        return EXIT_FAIL;
    ing->quantity -= qty;
    return SUCCESS;
}

int ingredient_restock(kitchen_t *k, const char *name, float qty)
{
    ingredient_t *ing = ingredient_find(k, name);
    if (!ing)
        return EXIT_FAIL;
    ing->quantity += qty;
    return SUCCESS;
}

int ingredient_add(kitchen_t *k, const char *name, float qty, const char *unit)
{
    if (k->nb_ingredients >= MAX_INGREDIENTS)
        return EXIT_FAIL;
    ingredient_t *ing = ingredient_find(k, name);
    if (ing) {
        ing->quantity += qty;
        return SUCCESS;
    }
    ingredient_t *new_ing = &k->ingredients[k->nb_ingredients];
    strncpy(new_ing->name, name, 63);
    new_ing->name[63] = '\0';
    new_ing->quantity = qty;
    strncpy(new_ing->unit, unit, 15);
    new_ing->unit[15] = '\0';
    new_ing->unlimited = (k->stock_mode == 0) ? 1 : 0;
    k->nb_ingredients++;
    return SUCCESS;
}
