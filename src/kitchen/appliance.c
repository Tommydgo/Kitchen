/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Appliance creation, lookup, tick and removal logic
*/
#include "global_includes.h"

appliance_t *appliance_create(const char *name)
{
    appliance_t *app = malloc(sizeof(appliance_t));

    if (!app)
        return NULL;
    strncpy(app->name, name, 63);
    app->name[63] = '\0';
    app->busy = 0;
    app->time_left = 0;
    app->current_recipe[0] = '\0';
    app->next = NULL;
    return app;
}

appliance_t *appliance_find(kitchen_t *k, const char *name)
{
    for (appliance_t *app = k->appliances; app; app = app->next) {
        if (strcmp(app->name, name) == 0)
            return app;
    }
    return NULL;
}

appliance_t *appliance_find_free(kitchen_t *k, const char *name)
{
    for (appliance_t *app = k->appliances; app; app = app->next) {
        if (strcmp(app->name, name) == 0 && !app->busy)
            return app;
    }
    return NULL;
}

void appliances_tick(kitchen_t *k, int seconds)
{
    for (appliance_t *app = k->appliances; app; app = app->next) {
        if (!app->busy)
            continue;
        app->time_left -= seconds;
        if (app->time_left <= 0) {
            recipe_t *r = recipe_find(k, app->current_recipe);
            if (r && r->output_ingredient[0] != '\0')
                ingredient_add(k, r->output_ingredient,
                               r->output_quantity, r->output_unit);
            printf("[done] %s is ready (on %s)!\n",
                app->current_recipe, app->name);
            app->busy = 0;
            app->time_left = 0;
            app->current_recipe[0] = '\0';
        }
    }
}

int appliance_remove(kitchen_t *k, const char *name)
{
    appliance_t *prev = NULL;

    for (appliance_t *cur = k->appliances; cur; cur = cur->next) {
        if (strcmp(cur->name, name) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                k->appliances = cur->next;
            free(cur);
            k->nb_appliances--;
            return SUCCESS;
        }
        prev = cur;
    }
    return EXIT_FAIL;
}
