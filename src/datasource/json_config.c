/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON kitchen config loader (ingredients, appliances, recipes)
*/
#include "global_includes.h"
#include "cJSON.h"

static int parse_ingredients(cJSON *arr, kitchen_t *k)
{
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len && k->nb_ingredients < MAX_INGREDIENTS; i++) {
        cJSON *item = cJSON_GetArrayItem(arr, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *qty  = cJSON_GetObjectItemCaseSensitive(item, "quantity");
        cJSON *unit = cJSON_GetObjectItemCaseSensitive(item, "unit");
        if (!cJSON_IsString(name) || !cJSON_IsNumber(qty) || !cJSON_IsString(unit))
            return EXIT_FAIL;
        ingredient_t *ing = &k->ingredients[k->nb_ingredients];
        strncpy(ing->name, name->valuestring, 63);
        ing->quantity = (float)qty->valuedouble;
        strncpy(ing->unit, unit->valuestring, 15);
        ing->unlimited = (k->stock_mode == 0) ? 1 : 0;
        k->nb_ingredients++;
    }
    return SUCCESS;
}

static int parse_appliances(cJSON *arr, kitchen_t *k)
{
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len; i++) {
        cJSON *item  = cJSON_GetArrayItem(arr, i);
        cJSON *name  = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *count = cJSON_GetObjectItemCaseSensitive(item, "count");
        if (!cJSON_IsString(name) || !cJSON_IsNumber(count))
            return EXIT_FAIL;
        int n = (int)count->valuedouble;
        for (int j = 0; j < n && k->nb_appliances < MAX_APPLIANCES; j++) {
            appliance_t *app = &k->appliances[k->nb_appliances];
            strncpy(app->name, name->valuestring, 63);
            app->busy = 0;
            app->time_left = 0;
            app->current_recipe[0] = '\0';
            k->nb_appliances++;
        }
    }
    return SUCCESS;
}

static int parse_recipe_ingredients(cJSON *arr, recipe_t *r)
{
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len && r->nb_ingredients < MAX_INGREDIENTS; i++) {
        cJSON *item = cJSON_GetArrayItem(arr, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *qty  = cJSON_GetObjectItemCaseSensitive(item, "quantity");
        cJSON *unit = cJSON_GetObjectItemCaseSensitive(item, "unit");
        if (!cJSON_IsString(name) || !cJSON_IsNumber(qty) || !cJSON_IsString(unit))
            return EXIT_FAIL;
        recipe_ingredient_t *ri = &r->ingredients[r->nb_ingredients];
        strncpy(ri->ingredient, name->valuestring, 63);
        ri->quantity = (float)qty->valuedouble;
        strncpy(ri->unit, unit->valuestring, 15);
        r->nb_ingredients++;
    }
    return SUCCESS;
}

static int parse_recipe_appliances(cJSON *arr, recipe_t *r)
{
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len && r->nb_appliances < MAX_APPLIANCES; i++) {
        cJSON *item = cJSON_GetArrayItem(arr, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *dur  = cJSON_GetObjectItemCaseSensitive(item, "duration");
        if (!cJSON_IsString(name) || !cJSON_IsNumber(dur))
            return EXIT_FAIL;
        recipe_appliance_t *ra = &r->appliances[r->nb_appliances];
        strncpy(ra->appliance, name->valuestring, 63);
        ra->duration = (int)dur->valuedouble;
        r->nb_appliances++;
    }
    return SUCCESS;
}

static int parse_recipes(cJSON *arr, kitchen_t *k)
{
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len && k->nb_recipes < MAX_RECIPES; i++) {
        cJSON *item   = cJSON_GetArrayItem(arr, i);
        cJSON *name   = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *ings   = cJSON_GetObjectItemCaseSensitive(item, "ingredients");
        cJSON *apps   = cJSON_GetObjectItemCaseSensitive(item, "appliances");
        cJSON *subs   = cJSON_GetObjectItemCaseSensitive(item, "sub_recipes");
        cJSON *output = cJSON_GetObjectItemCaseSensitive(item, "output");
        if (!cJSON_IsString(name))
            return EXIT_FAIL;
        recipe_t *r = &k->recipes[k->nb_recipes];
        strncpy(r->name, name->valuestring, 63);
        if (cJSON_IsArray(ings) && parse_recipe_ingredients(ings, r) != SUCCESS)
            return EXIT_FAIL;
        if (cJSON_IsArray(apps) && parse_recipe_appliances(apps, r) != SUCCESS)
            return EXIT_FAIL;
        if (cJSON_IsArray(subs)) {
            int slen = cJSON_GetArraySize(subs);
            for (int j = 0; j < slen && r->nb_sub_recipes < MAX_RECIPES; j++) {
                cJSON *s = cJSON_GetArrayItem(subs, j);
                if (cJSON_IsString(s))
                    strncpy(r->sub_recipes[r->nb_sub_recipes++], s->valuestring, 63);
            }
        }
        if (cJSON_IsObject(output)) {
            cJSON *oname = cJSON_GetObjectItemCaseSensitive(output, "name");
            cJSON *oqty  = cJSON_GetObjectItemCaseSensitive(output, "quantity");
            cJSON *ounit = cJSON_GetObjectItemCaseSensitive(output, "unit");
            if (cJSON_IsString(oname))
                strncpy(r->output_ingredient, oname->valuestring, 63);
            if (cJSON_IsNumber(oqty))
                r->output_quantity = (float)oqty->valuedouble;
            if (cJSON_IsString(ounit))
                strncpy(r->output_unit, ounit->valuestring, 15);
        }
        k->nb_recipes++;
    }
    return SUCCESS;
}

int json_load_config(const char *path, kitchen_t *k)
{
    char *raw = json_read_file(path);
    if (!raw)
        return EXIT_FAIL;
    cJSON *root = cJSON_Parse(raw);
    free(raw);
    if (!root)
        return EXIT_FAIL;

    cJSON *mode = cJSON_GetObjectItemCaseSensitive(root, "stock_mode");
    if (cJSON_IsString(mode) && strcmp(mode->valuestring, "sim") == 0)
        k->stock_mode = 0;
    else
        k->stock_mode = 1;

    cJSON *ings    = cJSON_GetObjectItemCaseSensitive(root, "ingredients");
    cJSON *apps    = cJSON_GetObjectItemCaseSensitive(root, "appliances");
    cJSON *recipes = cJSON_GetObjectItemCaseSensitive(root, "recipes");

    int ret = SUCCESS;
    if (cJSON_IsArray(ings))    ret = parse_ingredients(ings, k);
    if (ret == SUCCESS && cJSON_IsArray(apps))    ret = parse_appliances(apps, k);
    if (ret == SUCCESS && cJSON_IsArray(recipes)) ret = parse_recipes(recipes, k);

    cJSON_Delete(root);
    return ret;
}
