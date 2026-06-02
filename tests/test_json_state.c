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

    /* Simulate some state changes */
    k->simulated_time = 3600;
    k->ingredients[0].quantity = 600.0f; /* flour used */
    k->appliances[0].busy = 1;
    k->appliances[0].time_left = 900;
    strncpy(k->appliances[0].current_recipe, "cake", 63);

    /* Save state */
    assert(json_save_state("kitchen_save_test.json", k) == SUCCESS);

    /* Reload fresh kitchen and apply saved state */
    kitchen_t *k2 = kitchen_create();
    assert(json_load_config("kitchen.json", k2) == SUCCESS);
    assert(json_load_state("kitchen_save_test.json", k2) == SUCCESS);

    assert(k2->simulated_time == 3600);
    assert(k2->ingredients[0].quantity == 600.0f);
    assert(k2->appliances[0].busy == 1);
    assert(k2->appliances[0].time_left == 900);
    assert(strcmp(k2->appliances[0].current_recipe, "cake") == 0);

    kitchen_free(k);
    kitchen_free(k2);
    remove("kitchen_save_test.json");
    printf("test_json_state: OK\n");
    return 0;
}
