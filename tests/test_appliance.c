/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for appliance find, tick, busy logic and remove
*/
#include <assert.h>
#include <string.h>
#include "global_includes.h"

int main(void)
{
    kitchen_t *k = kitchen_create();
    assert(json_load_config("kitchen.json", k) == SUCCESS);

    /* appliance_find */
    appliance_t *oven = appliance_find(k, "oven");
    assert(oven != NULL);
    assert(oven->busy == 0);
    assert(appliance_find(k, "nonexistent") == NULL);

    /* appliance_find_free: oven is free */
    appliance_t *free_oven = appliance_find_free(k, "oven");
    assert(free_oven != NULL);

    /* Mark oven busy, find_free should return NULL */
    free_oven->busy = 1;
    free_oven->time_left = 300;
    strncpy(free_oven->current_recipe, "cake", 63);
    assert(appliance_find_free(k, "oven") == NULL);

    /* appliances_tick: advance 200s, oven still busy */
    kitchen_advance_time(k, 200);
    assert(appliance_find(k, "oven")->time_left == 100);
    assert(appliance_find(k, "oven")->busy == 1);

    /* appliances_tick: advance 100s, oven finishes */
    kitchen_advance_time(k, 100);
    assert(appliance_find(k, "oven")->busy == 0);
    assert(appliance_find(k, "oven")->time_left == 0);

    /* Two microwaves: both should be findable as free */
    appliance_t *m1 = appliance_find_free(k, "microwave");
    assert(m1 != NULL);
    m1->busy = 1;
    appliance_t *m2 = appliance_find_free(k, "microwave");
    assert(m2 != NULL);
    assert(m1 != m2);
    m2->busy = 1;
    assert(appliance_find_free(k, "microwave") == NULL);

    /* appliance_remove: free appliance */
    m1->busy = 0;
    m2->busy = 0;
    assert(appliance_remove(k, "microwave") == SUCCESS);
    assert(appliance_find(k, "microwave") != NULL);
    assert(appliance_remove(k, "microwave") == SUCCESS);
    assert(appliance_find(k, "microwave") == NULL);
    assert(appliance_remove(k, "nonexistent") == EXIT_FAIL);

    kitchen_free_all(k);
    printf("test_appliance: OK\n");
    return 0;
}
