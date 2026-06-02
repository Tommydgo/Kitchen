/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Ingredient find, consume, restock, add, create and remove operations
*/
#include "global_includes.h"

ingredient_t *ingredient_create(const char *name, float qty,
                                const char *unit, int unlimited)
{
    ingredient_t *ing = malloc(sizeof(ingredient_t));

    if (!ing)
        return NULL;
    strncpy(ing->name, name, 63);
    ing->name[63] = '\0';
    ing->quantity = qty;
    strncpy(ing->unit, unit, 15);
    ing->unit[15] = '\0';
    ing->unlimited = unlimited;
    ing->next = NULL;
    return ing;
}

ingredient_t *ingredient_find(kitchen_t *k, const char *name)
{
    for (ingredient_t *ing = k->ingredients; ing; ing = ing->next) {
        if (strcmp(ing->name, name) == 0)
            return ing;
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
    ingredient_t *ing = ingredient_find(k, name);

    if (ing) {
        ing->quantity += qty;
        return SUCCESS;
    }
    ingredient_t *new_ing = ingredient_create(name, qty, unit,
        (k->stock_mode == 0) ? 1 : 0);
    if (!new_ing)
        return EXIT_FAIL;
    new_ing->next = k->ingredients;
    k->ingredients = new_ing;
    k->nb_ingredients++;
    return SUCCESS;
}

int ingredient_remove(kitchen_t *k, const char *name)
{
    ingredient_t *prev = NULL;

    for (ingredient_t *cur = k->ingredients; cur; cur = cur->next) {
        if (strcmp(cur->name, name) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                k->ingredients = cur->next;
            free(cur);
            k->nb_ingredients--;
            return SUCCESS;
        }
        prev = cur;
    }
    return EXIT_FAIL;
}
