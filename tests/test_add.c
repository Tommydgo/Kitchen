/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for cmd_add interactive command
*/
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "global_includes.h"

int cmd_add(repl_ctx_t *ctx, char **args, int argc);

static repl_ctx_t make_ctx(void)
{
    kitchen_t *k = kitchen_create();
    json_load_config("kitchen.json", k);
    datasource_t *ds = datasource_init_json();
    repl_ctx_t ctx;
    ctx.k = k;
    ctx.ds = ds;
    strncpy(ctx.config_path, "kitchen.json", 255);
    strncpy(ctx.save_path, "kitchen_save.json", 255);
    return ctx;
}

int main(void)
{
    /* --- add ingredient --- */
    repl_ctx_t ctx = make_ctx();

    char ing_input[] = "salt\n200\ng\n";
    FILE *fake = fmemopen(ing_input, sizeof(ing_input) - 1, "r");
    stdin = fake;
    char *args_ing[] = {"add", "ingredient", NULL};
    assert(cmd_add(&ctx, args_ing, 2) == SUCCESS);
    fclose(fake);

    assert(ingredient_find(ctx.k, "salt") != NULL);
    assert(ingredient_find(ctx.k, "salt")->quantity == 200.0f);

    kitchen_free_all(ctx.k);
    datasource_free(ctx.ds);

    /* --- add appliance --- */
    ctx = make_ctx();

    char app_input[] = "blender\n";
    fake = fmemopen(app_input, sizeof(app_input) - 1, "r");
    stdin = fake;
    char *args_app[] = {"add", "appliance", NULL};
    assert(cmd_add(&ctx, args_app, 2) == SUCCESS);
    fclose(fake);

    assert(appliance_find(ctx.k, "blender") != NULL);

    kitchen_free_all(ctx.k);
    datasource_free(ctx.ds);

    /* --- add recipe --- */
    ctx = make_ctx();

    char rec_input[] = "toast\nflour 100 g\n\noven 60\n\n\ntoast 1 pcs\n";
    fake = fmemopen(rec_input, sizeof(rec_input) - 1, "r");
    stdin = fake;
    char *args_rec[] = {"add", "recipe", NULL};
    assert(cmd_add(&ctx, args_rec, 2) == SUCCESS);
    fclose(fake);

    recipe_t *toast = recipe_find(ctx.k, "toast");
    assert(toast != NULL);
    assert(toast->ingredients != NULL);
    assert(toast->appliances != NULL);
    assert(strcmp(toast->output_ingredient, "toast") == 0);

    kitchen_free_all(ctx.k);
    datasource_free(ctx.ds);

    /* --- unknown type --- */
    ctx = make_ctx();
    char *args_bad[] = {"add", "robot", NULL};
    assert(cmd_add(&ctx, args_bad, 2) == SUCCESS);
    kitchen_free_all(ctx.k);
    datasource_free(ctx.ds);

    printf("test_add: OK\n");
    return 0;
}
