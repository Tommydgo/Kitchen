/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Kitchen, ingredient, appliance and recipe function declarations
*/
#ifndef KITCHEN_H_
    #define KITCHEN_H_
    #include "structs.h"

kitchen_t       *kitchen_create(void);
void             kitchen_free(kitchen_t *k);

ingredient_t    *ingredient_find(kitchen_t *k, const char *name);
int              ingredient_consume(kitchen_t *k, const char *name, float qty);
int              ingredient_restock(kitchen_t *k, const char *name, float qty);
int              ingredient_add(kitchen_t *k, const char *name, float qty,
                                const char *unit);

appliance_t     *appliance_find(kitchen_t *k, const char *name);
appliance_t     *appliance_find_free(kitchen_t *k, const char *name);
void             appliances_tick(kitchen_t *k, int seconds);

void             kitchen_advance_time(kitchen_t *k, int seconds);

recipe_t        *recipe_find(kitchen_t *k, const char *name);
int              recipe_cook(kitchen_t *k, const char *name,
                             char visited[][64], int *nb_visited);

#endif /* KITCHEN_H_ */
