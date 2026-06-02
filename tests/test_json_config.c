/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for JSON kitchen config loading
*/
#include <assert.h>
#include <string.h>
#include "global_includes.h"

int main(void)
{
    kitchen_t *k = kitchen_create();
    assert(k != NULL);

    int ret = json_load_config("kitchen.json", k);
    assert(ret == SUCCESS);

    /* Counts */
    assert(k->nb_ingredients == 3);
    assert(k->nb_appliances == 4); /* oven x1 + microwave x2 + fridge x1 */
    assert(k->nb_recipes == 2);
    assert(k->stock_mode == 1); /* "real" */

    /* Ingredients accessible by name */
    ingredient_t *flour = ingredient_find(k, "flour");
    assert(flour != NULL);
    assert(flour->quantity == 1000.0f);
    assert(strcmp(flour->unit, "g") == 0);

    /* Appliances accessible by name */
    appliance_t *oven = appliance_find(k, "oven");
    assert(oven != NULL);
    assert(oven->busy == 0);

    /* Recipe dough */
    recipe_t *dough = recipe_find(k, "dough");
    assert(dough != NULL);
    int ni = 0;
    for (recipe_ingredient_t *ri = dough->ingredients; ri; ri = ri->next) ni++;
    assert(ni == 2);
    assert(dough->appliances != NULL);
    assert(dough->appliances->duration == 120);
    assert(strcmp(dough->output_ingredient, "dough") == 0);

    /* Recipe cake */
    recipe_t *cake = recipe_find(k, "cake");
    assert(cake != NULL);
    assert(cake->nb_sub_recipes == 1);
    assert(strcmp(cake->sub_recipes[0], "dough") == 0);

    kitchen_free_all(k);
    printf("test_json_config: OK\n");
    return 0;
}
