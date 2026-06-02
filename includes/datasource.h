/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Datasource interface and JSON datasource declarations
*/
#ifndef DATASOURCE_H_
    #define DATASOURCE_H_
    #include "structs.h"

typedef struct {
    int (*load_config)(const char *path, kitchen_t *k);
    int (*load_state) (const char *path, kitchen_t *k);
    int (*save_state) (const char *path, kitchen_t *k);
} datasource_t;

datasource_t *datasource_init_json(void);
void datasource_free(datasource_t *ds);
char *json_read_file(const char *path);
int json_load_config(const char *path, kitchen_t *k);
int json_load_state(const char *path, kitchen_t *k);
int json_save_state(const char *path, kitchen_t *k);
int json_parse_recipes(void *cjson_arr, kitchen_t *k);

#endif /* DATASOURCE_H_ */
