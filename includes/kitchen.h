/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Kitchen, ingredient, appliance and recipe function declarations
*/
#ifndef KITCHEN_H_
    #define KITCHEN_H_
    #include "structs.h"

kitchen_t           *kitchen_create(void);
void                 kitchen_clear(kitchen_t *k);
void                 kitchen_free_all(kitchen_t *k);

ingredient_t        *ingredient_create(const char *name, float qty,
                                       const char *unit, int unlimited);
ingredient_t        *ingredient_find(kitchen_t *k, const char *name);
int                  ingredient_consume(kitchen_t *k, const char *name, float qty);
int                  ingredient_restock(kitchen_t *k, const char *name, float qty);
int                  ingredient_add(kitchen_t *k, const char *name, float qty,
                                    const char *unit);
int                  ingredient_remove(kitchen_t *k, const char *name);

appliance_t         *appliance_create(const char *name);
appliance_t         *appliance_find(kitchen_t *k, const char *name);
appliance_t         *appliance_find_free(kitchen_t *k, const char *name);
void                 appliances_tick(kitchen_t *k, int seconds);
int                  appliance_remove(kitchen_t *k, const char *name);

void                 kitchen_advance_time(kitchen_t *k, int seconds);

recipe_ingredient_t *recipe_ingredient_create(const char *name, float qty,
                                              const char *unit);
recipe_appliance_t  *recipe_appliance_create(const char *name, int duration);
recipe_t            *recipe_create(const char *name);
void                 recipe_free(recipe_t *r);
recipe_t            *recipe_find(kitchen_t *k, const char *name);
int                  recipe_cook(kitchen_t *k, const char *name,
                                 char visited[][64], int *nb_visited);
int                  recipe_remove(kitchen_t *k, const char *name);

#endif /* KITCHEN_H_ */
