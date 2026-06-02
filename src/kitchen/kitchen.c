/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Kitchen create and free lifecycle management
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

void kitchen_free(kitchen_t *k)
{
    free(k);
}
