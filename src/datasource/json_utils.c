/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON file read utility
*/
#include "global_includes.h"
#include "cJSON.h"

char *json_read_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
}

int json_parse_recipes(void *arr_ptr, kitchen_t *k)
{
    cJSON *arr = (cJSON *)arr_ptr;
    int len = cJSON_GetArraySize(arr);

    for (int i = 0; i < len; i++) {
        cJSON *item   = cJSON_GetArrayItem(arr, i);
        cJSON *name   = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *ings   = cJSON_GetObjectItemCaseSensitive(item, "ingredients");
        cJSON *apps   = cJSON_GetObjectItemCaseSensitive(item, "appliances");
        cJSON *subs   = cJSON_GetObjectItemCaseSensitive(item, "sub_recipes");
        cJSON *output = cJSON_GetObjectItemCaseSensitive(item, "output");

        if (!cJSON_IsString(name))
            return EXIT_FAIL;
        recipe_t *r = recipe_create(name->valuestring);
        if (!r)
            return EXIT_FAIL;

        if (cJSON_IsArray(ings)) {
            int ilen = cJSON_GetArraySize(ings);
            for (int j = 0; j < ilen; j++) {
                cJSON *it = cJSON_GetArrayItem(ings, j);
                cJSON *iname = cJSON_GetObjectItemCaseSensitive(it, "name");
                cJSON *iqty  = cJSON_GetObjectItemCaseSensitive(it, "quantity");
                cJSON *iunit = cJSON_GetObjectItemCaseSensitive(it, "unit");
                if (!cJSON_IsString(iname) || !cJSON_IsNumber(iqty)
                    || !cJSON_IsString(iunit)) {
                    recipe_free(r);
                    return EXIT_FAIL;
                }
                recipe_ingredient_t *ri = recipe_ingredient_create(
                    iname->valuestring, (float)iqty->valuedouble,
                    iunit->valuestring);
                if (!ri) { recipe_free(r); return EXIT_FAIL; }
                ri->next = r->ingredients;
                r->ingredients = ri;
            }
        }

        if (cJSON_IsArray(apps)) {
            int alen = cJSON_GetArraySize(apps);
            for (int j = 0; j < alen; j++) {
                cJSON *at    = cJSON_GetArrayItem(apps, j);
                cJSON *aname = cJSON_GetObjectItemCaseSensitive(at, "name");
                cJSON *adur  = cJSON_GetObjectItemCaseSensitive(at, "duration");
                if (!cJSON_IsString(aname) || !cJSON_IsNumber(adur)) {
                    recipe_free(r);
                    return EXIT_FAIL;
                }
                recipe_appliance_t *ra = recipe_appliance_create(
                    aname->valuestring, (int)adur->valuedouble);
                if (!ra) { recipe_free(r); return EXIT_FAIL; }
                ra->next = r->appliances;
                r->appliances = ra;
            }
        }

        if (cJSON_IsArray(subs)) {
            int slen = cJSON_GetArraySize(subs);
            for (int j = 0; j < slen; j++) {
                cJSON *s = cJSON_GetArrayItem(subs, j);
                if (!cJSON_IsString(s))
                    continue;
                char **new_subs = realloc(r->sub_recipes,
                    (r->nb_sub_recipes + 1) * sizeof(char *));
                if (!new_subs) { recipe_free(r); return EXIT_FAIL; }
                r->sub_recipes = new_subs;
                r->sub_recipes[r->nb_sub_recipes] = strdup(s->valuestring);
                if (!r->sub_recipes[r->nb_sub_recipes]) {
                    recipe_free(r);
                    return EXIT_FAIL;
                }
                r->nb_sub_recipes++;
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
        r->next = k->recipes;
        k->recipes = r;
        k->nb_recipes++;
    }
    return SUCCESS;
}
