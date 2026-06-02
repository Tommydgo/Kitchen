/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for JSON kitchen state load and save
*/
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "global_includes.h"

int main(void)
{
    kitchen_t *k = kitchen_create();
    assert(json_load_config("kitchen.json", k) == SUCCESS);

    /* Simulate state changes */
    k->simulated_time = 3600;
    ingredient_find(k, "flour")->quantity = 600.0f;
    appliance_t *oven = appliance_find(k, "oven");
    oven->busy = 1;
    oven->time_left = 900;
    strncpy(oven->current_recipe, "cake", 63);

    /* Save state */
    assert(json_save_state("kitchen_save_test.json", k) == SUCCESS);

    /* Reload fresh kitchen and apply saved state */
    kitchen_t *k2 = kitchen_create();
    assert(json_load_config("kitchen.json", k2) == SUCCESS);
    assert(json_load_state("kitchen_save_test.json", k2) == SUCCESS);

    assert(k2->simulated_time == 3600);
    assert(ingredient_find(k2, "flour")->quantity == 600.0f);
    appliance_t *oven2 = appliance_find(k2, "oven");
    assert(oven2 != NULL);
    assert(oven2->busy == 1);
    assert(oven2->time_left == 900);
    assert(strcmp(oven2->current_recipe, "cake") == 0);

    /* Recipes should be preserved after save/load */
    assert(recipe_find(k2, "dough") != NULL);
    assert(recipe_find(k2, "cake") != NULL);

    kitchen_free_all(k);
    kitchen_free_all(k2);
    remove("kitchen_save_test.json");
    printf("test_json_state: OK\n");
    return 0;
}
