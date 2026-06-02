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

    assert(k->nb_ingredients == 3);
    assert(strcmp(k->ingredients[0].name, "flour") == 0);
    assert(k->ingredients[0].quantity == 1000.0f);
    assert(strcmp(k->ingredients[0].unit, "g") == 0);

    assert(k->nb_appliances == 4); /* oven x1 + microwave x2 + fridge x1 */
    assert(strcmp(k->appliances[0].name, "oven") == 0);
    assert(k->appliances[0].busy == 0);

    assert(k->nb_recipes == 2);
    assert(strcmp(k->recipes[0].name, "dough") == 0);
    assert(k->recipes[0].nb_ingredients == 2);
    assert(k->recipes[0].nb_appliances == 1);
    assert(k->recipes[0].appliances[0].duration == 120);
    assert(strcmp(k->recipes[0].output_ingredient, "dough") == 0);

    assert(strcmp(k->recipes[1].name, "cake") == 0);
    assert(k->recipes[1].nb_sub_recipes == 1);
    assert(strcmp(k->recipes[1].sub_recipes[0], "dough") == 0);

    assert(k->stock_mode == 1); /* "real" */

    kitchen_free(k);
    printf("test_json_config: OK\n");
    return 0;
}
