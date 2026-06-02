/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for cmd_remove command
*/
#include <assert.h>
#include <string.h>
#include "global_includes.h"

int cmd_remove(repl_ctx_t *ctx, char **args, int argc);

int main(void)
{
    kitchen_t *k = kitchen_create();
    json_load_config("kitchen.json", k);
    datasource_t *ds = datasource_init_json();
    repl_ctx_t ctx;
    ctx.k = k;
    ctx.ds = ds;
    strncpy(ctx.config_path, "kitchen.json", 255);
    strncpy(ctx.save_path, "kitchen_save.json", 255);

    /* remove ingredient */
    char *args_ri[] = {"remove", "ingredient", "flour", NULL};
    assert(cmd_remove(&ctx, args_ri, 3) == SUCCESS);
    assert(ingredient_find(k, "flour") == NULL);

    /* remove nonexistent ingredient */
    char *args_ri_bad[] = {"remove", "ingredient", "nonexistent", NULL};
    assert(cmd_remove(&ctx, args_ri_bad, 3) == SUCCESS);

    /* remove appliance */
    char *args_ra[] = {"remove", "appliance", "fridge", NULL};
    assert(cmd_remove(&ctx, args_ra, 3) == SUCCESS);
    assert(appliance_find(k, "fridge") == NULL);

    /* remove busy appliance — should refuse */
    appliance_t *oven = appliance_find(k, "oven");
    assert(oven != NULL);
    oven->busy = 1;
    char *args_ra_busy[] = {"remove", "appliance", "oven", NULL};
    assert(cmd_remove(&ctx, args_ra_busy, 3) == SUCCESS);
    assert(appliance_find(k, "oven") != NULL); /* still there */

    /* remove recipe */
    char *args_rr[] = {"remove", "recipe", "dough", NULL};
    assert(cmd_remove(&ctx, args_rr, 3) == SUCCESS);
    assert(recipe_find(k, "dough") == NULL);

    /* remove recipe being cooked — should refuse */
    strncpy(oven->current_recipe, "cake", 63);
    char *args_rr_busy[] = {"remove", "recipe", "cake", NULL};
    assert(cmd_remove(&ctx, args_rr_busy, 3) == SUCCESS);
    assert(recipe_find(k, "cake") != NULL); /* still there */

    /* missing args */
    char *args_noargs[] = {"remove", NULL};
    assert(cmd_remove(&ctx, args_noargs, 1) == SUCCESS);

    kitchen_free_all(k);
    datasource_free(ds);
    printf("test_remove: OK\n");
    return 0;
}
