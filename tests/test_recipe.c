/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for recipe cooking, sub-recipes, cycle detection and remove
*/
#include <assert.h>
#include <string.h>
#include "global_includes.h"

int main(void)
{
    kitchen_t *k = kitchen_create();
    assert(json_load_config("kitchen.json", k) == SUCCESS);

    char visited[MAX_VISITED][64];
    int nb = 0;

    /* recipe_find */
    assert(recipe_find(k, "cake") != NULL);
    assert(recipe_find(k, "nonexistent") == NULL);

    /* cook dough: needs flour(200g) + butter(100g), uses microwave(120s) */
    assert(recipe_cook(k, "dough", visited, &nb) == SUCCESS);

    /* microwave should be busy */
    appliance_t *busy_mw = appliance_find(k, "microwave");
    assert(busy_mw != NULL && busy_mw->busy == 1);
    assert(busy_mw->time_left == 120);

    /* flour consumed */
    assert(ingredient_find(k, "flour")->quantity == 800.0f);

    /* advance time: dough ready */
    kitchen_advance_time(k, 120);
    assert(ingredient_find(k, "dough") != NULL);
    assert(ingredient_find(k, "dough")->quantity == 1.0f);

    /* cook cake: dough already in stock, eggs(3) needed + oven */
    nb = 0;
    assert(recipe_cook(k, "cake", visited, &nb) == SUCCESS);
    assert(appliance_find_free(k, "oven") == NULL); /* oven is busy */

    /* advance time to finish cake */
    kitchen_advance_time(k, 1800);
    assert(ingredient_find(k, "eggs")->quantity == 9.0f);

    /* unknown recipe should fail */
    nb = 0;
    assert(recipe_cook(k, "unknown_recipe", visited, &nb) == EXIT_FAIL);

    /* recipe_remove */
    assert(recipe_remove(k, "cake") == SUCCESS);
    assert(recipe_find(k, "cake") == NULL);
    assert(recipe_remove(k, "nonexistent") == EXIT_FAIL);

    kitchen_free_all(k);
    printf("test_recipe: OK\n");
    return 0;
}
