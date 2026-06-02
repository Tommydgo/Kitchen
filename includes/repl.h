/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** REPL context, command entry types and dispatch declarations
*/
#ifndef REPL_H_
    #define REPL_H_
    #include "structs.h"
    #include "datasource.h"

typedef struct {
    kitchen_t    *k;
    datasource_t *ds;
    char          config_path[256];
    char          save_path[256];
} repl_ctx_t;

typedef int (*cmd_fn_t)(repl_ctx_t *ctx, char **args, int argc);

typedef struct {
    char        name[32];
    char        usage[64];
    cmd_fn_t    handler;
} cmd_entry_t;

void repl_run(repl_ctx_t *ctx);
int  repl_dispatch(repl_ctx_t *ctx, char **args, int argc);

extern cmd_entry_t  cmd_table[];
extern int          cmd_table_size;

#endif /* REPL_H_ */
