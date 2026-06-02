/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Appliance lookup, tick and completion logic
*/
#include "global_includes.h"

appliance_t *appliance_find(kitchen_t *k, const char *name)
{
    for (int i = 0; i < k->nb_appliances; i++) {
        if (strcmp(k->appliances[i].name, name) == 0)
            return &k->appliances[i];
    }
    return NULL;
}

appliance_t *appliance_find_free(kitchen_t *k, const char *name)
{
    for (int i = 0; i < k->nb_appliances; i++) {
        if (strcmp(k->appliances[i].name, name) == 0
            && !k->appliances[i].busy)
            return &k->appliances[i];
    }
    return NULL;
}

void appliances_tick(kitchen_t *k, int seconds)
{
    for (int i = 0; i < k->nb_appliances; i++) {
        if (!k->appliances[i].busy)
            continue;
        k->appliances[i].time_left -= seconds;
        if (k->appliances[i].time_left <= 0) {
            recipe_t *r = recipe_find(k, k->appliances[i].current_recipe);
            if (r && r->output_ingredient[0] != '\0')
                ingredient_add(k, r->output_ingredient,
                               r->output_quantity, r->output_unit);
            printf("[done] %s is ready (on %s)!\n",
                k->appliances[i].current_recipe,
                k->appliances[i].name);
            k->appliances[i].busy = 0;
            k->appliances[i].time_left = 0;
            k->appliances[i].current_recipe[0] = '\0';
        }
    }
}
