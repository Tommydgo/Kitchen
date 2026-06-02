/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON kitchen state load and save (ingredients, appliances, time, recipes)
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
            const char *unit_str = cJSON_IsString(unit) ? unit->valuestring : "pcs";
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
            cJSON *item  = cJSON_GetArrayItem(apps, i);
            cJSON *name  = cJSON_GetObjectItemCaseSensitive(item, "name");
            cJSON *busy  = cJSON_GetObjectItemCaseSensitive(item, "busy");
            cJSON *tl    = cJSON_GetObjectItemCaseSensitive(item, "time_left");
            cJSON *cur_r = cJSON_GetObjectItemCaseSensitive(item, "current_recipe");
            if (!cJSON_IsString(name))
                continue;
            for (appliance_t *app = k->appliances; app; app = app->next) {
                if (strcmp(app->name, name->valuestring) == 0 && app->busy == 0) {
                    if (cJSON_IsNumber(busy))
                        app->busy = (int)busy->valuedouble;
                    if (cJSON_IsNumber(tl))
                        app->time_left = (int)tl->valuedouble;
                    if (cJSON_IsString(cur_r))
                        strncpy(app->current_recipe, cur_r->valuestring, 63);
                    break;
                }
            }
        }
    }

    /* Replace recipes with saved ones if present */
    cJSON *saved_recipes = cJSON_GetObjectItemCaseSensitive(root, "recipes");
    if (cJSON_IsArray(saved_recipes)) {
        recipe_t *r = k->recipes;
        while (r) {
            recipe_t *next = r->next;
            recipe_free(r);
            r = next;
        }
        k->recipes = NULL;
        k->nb_recipes = 0;
        json_parse_recipes(saved_recipes, k);
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
    for (ingredient_t *ing = k->ingredients; ing; ing = ing->next) {
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "name", ing->name);
        cJSON_AddNumberToObject(obj, "quantity", ing->quantity);
        cJSON_AddStringToObject(obj, "unit", ing->unit);
        cJSON_AddItemToArray(ings, obj);
    }
    cJSON_AddItemToObject(root, "ingredients", ings);

    cJSON *apps = cJSON_CreateArray();
    for (appliance_t *app = k->appliances; app; app = app->next) {
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "name", app->name);
        cJSON_AddNumberToObject(obj, "busy", app->busy);
        cJSON_AddNumberToObject(obj, "time_left", app->time_left);
        cJSON_AddStringToObject(obj, "current_recipe", app->current_recipe);
        cJSON_AddItemToArray(apps, obj);
    }
    cJSON_AddItemToObject(root, "appliances", apps);

    cJSON *recipes = cJSON_CreateArray();
    for (recipe_t *r = k->recipes; r; r = r->next) {
        cJSON *rec = cJSON_CreateObject();
        cJSON_AddStringToObject(rec, "name", r->name);

        cJSON *r_ings = cJSON_CreateArray();
        for (recipe_ingredient_t *ri = r->ingredients; ri; ri = ri->next) {
            cJSON *obj = cJSON_CreateObject();
            cJSON_AddStringToObject(obj, "name", ri->ingredient);
            cJSON_AddNumberToObject(obj, "quantity", ri->quantity);
            cJSON_AddStringToObject(obj, "unit", ri->unit);
            cJSON_AddItemToArray(r_ings, obj);
        }
        cJSON_AddItemToObject(rec, "ingredients", r_ings);

        cJSON *r_apps = cJSON_CreateArray();
        for (recipe_appliance_t *ra = r->appliances; ra; ra = ra->next) {
            cJSON *obj = cJSON_CreateObject();
            cJSON_AddStringToObject(obj, "name", ra->appliance);
            cJSON_AddNumberToObject(obj, "duration", ra->duration);
            cJSON_AddItemToArray(r_apps, obj);
        }
        cJSON_AddItemToObject(rec, "appliances", r_apps);

        cJSON *r_subs = cJSON_CreateArray();
        for (int i = 0; i < r->nb_sub_recipes; i++)
            cJSON_AddItemToArray(r_subs, cJSON_CreateString(r->sub_recipes[i]));
        cJSON_AddItemToObject(rec, "sub_recipes", r_subs);

        cJSON *r_out = cJSON_CreateObject();
        cJSON_AddStringToObject(r_out, "name", r->output_ingredient);
        cJSON_AddNumberToObject(r_out, "quantity", r->output_quantity);
        cJSON_AddStringToObject(r_out, "unit", r->output_unit);
        cJSON_AddItemToObject(rec, "output", r_out);

        cJSON_AddItemToArray(recipes, rec);
    }
    cJSON_AddItemToObject(root, "recipes", recipes);

    char *str = cJSON_Print(root);
    cJSON_Delete(root);
    if (!str)
        return EXIT_FAIL;

    FILE *f = fopen(path, "w");
    if (!f) { free(str); return EXIT_FAIL; }
    fputs(str, f);
    fclose(f);
    free(str);
    return SUCCESS;
}
