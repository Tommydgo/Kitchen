/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON kitchen state load and save (ingredients, appliances, time)
*/
#include "global_includes.h"
#include "cJSON.h"

int json_load_state(const char *path, kitchen_t *k)
{
    char *raw = json_read_file(path);
    if (!raw)
        return EXIT_FAIL;
    cJSON *root = cJSON_Parse(raw);
    free(raw);
    if (!root)
        return EXIT_FAIL;

    cJSON *time = cJSON_GetObjectItemCaseSensitive(root, "simulated_time");
    if (cJSON_IsNumber(time))
        k->simulated_time = (int)time->valuedouble;

    cJSON *mode = cJSON_GetObjectItemCaseSensitive(root, "stock_mode");
    if (cJSON_IsString(mode))
        k->stock_mode = (strcmp(mode->valuestring, "real") == 0) ? 1 : 0;

    cJSON *ings = cJSON_GetObjectItemCaseSensitive(root, "ingredients");
    if (cJSON_IsArray(ings)) {
        int len = cJSON_GetArraySize(ings);
        for (int i = 0; i < len; i++) {
            cJSON *item = cJSON_GetArrayItem(ings, i);
            cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
            cJSON *qty  = cJSON_GetObjectItemCaseSensitive(item, "quantity");
            cJSON *unit = cJSON_GetObjectItemCaseSensitive(item, "unit");
            const char *unit_str = (cJSON_IsString(unit)) ? unit->valuestring : "pcs";
            if (!cJSON_IsString(name) || !cJSON_IsNumber(qty))
                continue;
            ingredient_t *ing = ingredient_find(k, name->valuestring);
            if (ing)
                ing->quantity = (float)qty->valuedouble;
            else
                ingredient_add(k, name->valuestring,
                    (float)qty->valuedouble, unit_str);
        }
    }

    cJSON *apps = cJSON_GetObjectItemCaseSensitive(root, "appliances");
    if (cJSON_IsArray(apps)) {
        int len = cJSON_GetArraySize(apps);
        for (int i = 0; i < len; i++) {
            cJSON *item    = cJSON_GetArrayItem(apps, i);
            cJSON *name    = cJSON_GetObjectItemCaseSensitive(item, "name");
            cJSON *busy    = cJSON_GetObjectItemCaseSensitive(item, "busy");
            cJSON *tl      = cJSON_GetObjectItemCaseSensitive(item, "time_left");
            cJSON *cur_r   = cJSON_GetObjectItemCaseSensitive(item, "current_recipe");
            if (!cJSON_IsString(name))
                continue;
            /* Find first matching appliance not yet restored (busy==0) */
            for (int j = 0; j < k->nb_appliances; j++) {
                if (strcmp(k->appliances[j].name, name->valuestring) == 0
                    && k->appliances[j].busy == 0) {
                    if (cJSON_IsNumber(busy))
                        k->appliances[j].busy = (int)busy->valuedouble;
                    if (cJSON_IsNumber(tl))
                        k->appliances[j].time_left = (int)tl->valuedouble;
                    if (cJSON_IsString(cur_r))
                        strncpy(k->appliances[j].current_recipe,
                                cur_r->valuestring, 63);
                    break;
                }
            }
        }
    }

    cJSON_Delete(root);
    return SUCCESS;
}

int json_save_state(const char *path, kitchen_t *k)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "simulated_time", k->simulated_time);
    cJSON_AddStringToObject(root, "stock_mode",
        k->stock_mode == 1 ? "real" : "sim");

    cJSON *ings = cJSON_CreateArray();
    for (int i = 0; i < k->nb_ingredients; i++) {
        cJSON *ing = cJSON_CreateObject();
        cJSON_AddStringToObject(ing, "name", k->ingredients[i].name);
        cJSON_AddNumberToObject(ing, "quantity", k->ingredients[i].quantity);
        cJSON_AddStringToObject(ing, "unit", k->ingredients[i].unit);
        cJSON_AddItemToArray(ings, ing);
    }
    cJSON_AddItemToObject(root, "ingredients", ings);

    cJSON *apps = cJSON_CreateArray();
    for (int i = 0; i < k->nb_appliances; i++) {
        cJSON *app = cJSON_CreateObject();
        cJSON_AddStringToObject(app, "name", k->appliances[i].name);
        cJSON_AddNumberToObject(app, "busy", k->appliances[i].busy);
        cJSON_AddNumberToObject(app, "time_left", k->appliances[i].time_left);
        cJSON_AddStringToObject(app, "current_recipe",
            k->appliances[i].current_recipe);
        cJSON_AddItemToArray(apps, app);
    }
    cJSON_AddItemToObject(root, "appliances", apps);

    char *str = cJSON_Print(root);
    cJSON_Delete(root);
    if (!str)
        return EXIT_FAIL;

    FILE *f = fopen(path, "w");
    if (!f) {
        free(str);
        return EXIT_FAIL;
    }
    fputs(str, f);
    fclose(f);
    free(str);
    return SUCCESS;
}
