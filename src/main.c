/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Entry point: init kitchen, load config/state and run REPL
*/
#include "global_includes.h"

static int file_exists(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return 0;
    fclose(f);
    return 1;
}

int main(int argc, char **argv)
{
    const char *config_path = (argc > 1) ? argv[1] : KITCHEN_CONFIG;
    const char *save_path   = KITCHEN_SAVE;

    datasource_t *ds = datasource_init_json();
    if (!ds) {
        fprintf(stderr, "Error: could not init datasource\n");
        return EXIT_FAIL;
    }

    kitchen_t *k = kitchen_create();
    if (!k) {
        fprintf(stderr, "Error: could not create kitchen\n");
        datasource_free(ds);
        return EXIT_FAIL;
    }

    if (ds->load_config(config_path, k) != SUCCESS) {
        fprintf(stderr, "Error: failed to load config '%s'\n", config_path);
        kitchen_free(k);
        datasource_free(ds);
        return EXIT_FAIL;
    }

    if (file_exists(save_path)) {
        if (ds->load_state(save_path, k) != SUCCESS)
            fprintf(stderr,
                "Warning: could not load save '%s', starting fresh\n",
                save_path);
    }

    repl_ctx_t ctx;
    ctx.k  = k;
    ctx.ds = ds;
    strncpy(ctx.config_path, config_path, 255);
    strncpy(ctx.save_path,   save_path,   255);

    repl_run(&ctx);

    kitchen_free(k);
    datasource_free(ds);
    return SUCCESS;
}
