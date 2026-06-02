/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** add command: interactive field-by-field entity creation
*/
#include "global_includes.h"

static int add_ingredient(repl_ctx_t *ctx)
{
    char name[64], unit[16], qbuf[32];
    float qty;

    printf("  Name: ");
    fflush(stdout);
    if (!fgets(name, sizeof(name), stdin))
        return EXIT_FAIL;
    name[strcspn(name, "\n")] = '\0';
    printf("  Quantity: ");
    fflush(stdout);
    if (!fgets(qbuf, sizeof(qbuf), stdin))
        return EXIT_FAIL;
    qty = strtof(qbuf, NULL);
    printf("  Unit: ");
    fflush(stdout);
    if (!fgets(unit, sizeof(unit), stdin))
        return EXIT_FAIL;
    unit[strcspn(unit, "\n")] = '\0';
    if (ingredient_add(ctx->k, name, qty, unit) != SUCCESS)
        return EXIT_FAIL;
    printf("Ingredient '%s' added.\n", name);
    return SUCCESS;
}

static int add_appliance(repl_ctx_t *ctx)
{
    char name[64];

    printf("  Name: ");
    fflush(stdout);
    if (!fgets(name, sizeof(name), stdin))
        return EXIT_FAIL;
    name[strcspn(name, "\n")] = '\0';
    appliance_t *app = appliance_create(name);
    if (!app)
        return EXIT_FAIL;
    app->next = ctx->k->appliances;
    ctx->k->appliances = app;
    ctx->k->nb_appliances++;
    printf("Appliance '%s' added.\n", name);
    return SUCCESS;
}

static int add_recipe(repl_ctx_t *ctx)
{
    char name[64], buf[128];

    printf("  Name: ");
    fflush(stdout);
    if (!fgets(name, sizeof(name), stdin))
        return EXIT_FAIL;
    name[strcspn(name, "\n")] = '\0';

    recipe_t *r = recipe_create(name);
    if (!r)
        return EXIT_FAIL;

    while (1) {
        printf("  Add ingredient (enter to skip): ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] == '\0')
            break;
        char ing_name[64], ing_unit[16];
        float ing_qty;
        if (sscanf(buf, "%63s %f %15s", ing_name, &ing_qty, ing_unit) != 3)
            continue;
        recipe_ingredient_t *ri = recipe_ingredient_create(ing_name, ing_qty, ing_unit);
        if (ri) {
            ri->next = r->ingredients;
            r->ingredients = ri;
        }
    }

    while (1) {
        printf("  Add appliance (enter to skip): ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] == '\0')
            break;
        char app_name[64];
        int duration;
        if (sscanf(buf, "%63s %d", app_name, &duration) != 2)
            continue;
        recipe_appliance_t *ra = recipe_appliance_create(app_name, duration);
        if (ra) {
            ra->next = r->appliances;
            r->appliances = ra;
        }
    }

    while (1) {
        printf("  Add sub-recipe (enter to skip): ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] == '\0')
            break;
        char **new_subs = realloc(r->sub_recipes,
            (r->nb_sub_recipes + 1) * sizeof(char *));
        if (!new_subs)
            break;
        r->sub_recipes = new_subs;
        r->sub_recipes[r->nb_sub_recipes] = strdup(buf);
        if (r->sub_recipes[r->nb_sub_recipes])
            r->nb_sub_recipes++;
    }

    printf("  Output (name qty unit, enter to skip): ");
    fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] != '\0') {
            char out_name[64], out_unit[16];
            float out_qty;
            if (sscanf(buf, "%63s %f %15s", out_name, &out_qty, out_unit) == 3) {
                strncpy(r->output_ingredient, out_name, 63);
                r->output_quantity = out_qty;
                strncpy(r->output_unit, out_unit, 15);
            }
        }
    }

    r->next = ctx->k->recipes;
    ctx->k->recipes = r;
    ctx->k->nb_recipes++;
    printf("Recipe '%s' added.\n", name);
    return SUCCESS;
}

int cmd_add(repl_ctx_t *ctx, char **args, int argc)
{
    if (argc < 2) {
        printf("Usage: add <ingredient|appliance|recipe>\n");
        return SUCCESS;
    }
    if (strcmp(args[1], "ingredient") == 0)
        return add_ingredient(ctx);
    if (strcmp(args[1], "appliance") == 0)
        return add_appliance(ctx);
    if (strcmp(args[1], "recipe") == 0)
        return add_recipe(ctx);
    printf("Unknown type '%s'. Use: ingredient, appliance, recipe\n", args[1]);
    return SUCCESS;
}
