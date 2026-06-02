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

    for (int i = 0; i < len; i++) {
        cJSON *item = cJSON_GetArrayItem(arr, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *qty  = cJSON_GetObjectItemCaseSensitive(item, "quantity");
        cJSON *unit = cJSON_GetObjectItemCaseSensitive(item, "unit");
        if (!cJSON_IsString(name) || !cJSON_IsNumber(qty) || !cJSON_IsString(unit))
            return EXIT_FAIL;
        ingredient_t *ing = ingredient_create(name->valuestring,
            (float)qty->valuedouble, unit->valuestring,
            (k->stock_mode == 0) ? 1 : 0);
        if (!ing)
            return EXIT_FAIL;
        ing->next = k->ingredients;
        k->ingredients = ing;
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
        for (int j = 0; j < n; j++) {
            appliance_t *app = appliance_create(name->valuestring);
            if (!app)
                return EXIT_FAIL;
            app->next = k->appliances;
            k->appliances = app;
            k->nb_appliances++;
        }
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
    if (ret == SUCCESS && cJSON_IsArray(recipes)) ret = json_parse_recipes(recipes, k);

    cJSON_Delete(root);
    return ret;
}
