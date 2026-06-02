/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests for REPL command dispatch and unknown command handling
*/
#include <assert.h>
#include <string.h>
#include "global_includes.h"

static int dummy_called = 0;

static int dummy_cmd(repl_ctx_t *ctx, char **args, int argc)
{
    (void)ctx; (void)args; (void)argc;
    dummy_called = 1;
    return SUCCESS;
}

int main(void)
{
    kitchen_t *k = kitchen_create();
    datasource_t *ds = datasource_init_json();
    repl_ctx_t ctx;
    ctx.k = k;
    ctx.ds = ds;
    strncpy(ctx.config_path, "kitchen.json", 255);
    strncpy(ctx.save_path, "kitchen_save.json", 255);

    /* Inject dummy into cmd_table */
    strncpy(cmd_table[0].name, "dummy", 31);
    cmd_table[0].handler = dummy_cmd;
    cmd_table_size = 1;

    char *args[] = {"dummy", NULL};
    assert(repl_dispatch(&ctx, args, 1) == SUCCESS);
    assert(dummy_called == 1);

    /* Unknown command returns SUCCESS (prints message, does not crash) */
    dummy_called = 0;
    char *bad[] = {"unknowncmd", NULL};
    assert(repl_dispatch(&ctx, bad, 1) == SUCCESS);
    assert(dummy_called == 0);

    kitchen_free_all(k);
    datasource_free(ds);
    printf("test_repl_dispatch: OK\n");
    return 0;
}
