/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for ingredient find, consume, restock and add
*/
#include <assert.h>
#include "global_includes.h"

int main(void)
{
    kitchen_t *k = kitchen_create();
    assert(json_load_config("kitchen.json", k) == SUCCESS);

    /* ingredient_find */
    ingredient_t *flour = ingredient_find(k, "flour");
    assert(flour != NULL);
    assert(flour->quantity == 1000.0f);
    assert(ingredient_find(k, "nonexistent") == NULL);

    /* ingredient_consume — real mode */
    assert(ingredient_consume(k, "flour", 200.0f) == SUCCESS);
    assert(ingredient_find(k, "flour")->quantity == 800.0f);
    assert(ingredient_consume(k, "flour", 900.0f) == EXIT_FAIL); /* insufficient */

    /* ingredient_restock */
    assert(ingredient_restock(k, "flour", 500.0f) == SUCCESS);
    assert(ingredient_find(k, "flour")->quantity == 1300.0f);

    /* ingredient_add new ingredient */
    assert(ingredient_add(k, "sugar", 300.0f, "g") == SUCCESS);
    assert(ingredient_find(k, "sugar") != NULL);
    assert(ingredient_find(k, "sugar")->quantity == 300.0f);

    /* ingredient_add existing: accumulates quantity */
    assert(ingredient_add(k, "sugar", 100.0f, "g") == SUCCESS);
    assert(ingredient_find(k, "sugar")->quantity == 400.0f);

    /* unlimited mode */
    k->stock_mode = 0;
    ingredient_t *eggs = ingredient_find(k, "eggs");
    eggs->unlimited = 1;
    assert(ingredient_consume(k, "eggs", 99999.0f) == SUCCESS); /* never fails when unlimited */

    kitchen_free(k);
    printf("test_ingredient: OK\n");
    return 0;
}
