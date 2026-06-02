/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON datasource initialization and cleanup
*/
#include "global_includes.h"

datasource_t *datasource_init_json(void)
{
    datasource_t *ds = malloc(sizeof(datasource_t));
    if (!ds)
        return NULL;
    ds->load_config = json_load_config;
    ds->load_state  = json_load_state;
    ds->save_state  = json_save_state;
    return ds;
}

void datasource_free(datasource_t *ds)
{
    free(ds);
}
