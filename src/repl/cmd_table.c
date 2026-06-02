/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Command dispatch table mapping names to handlers
*/
#include "global_includes.h"

int cmd_cook    (repl_ctx_t *ctx, char **args, int argc);
int cmd_wait    (repl_ctx_t *ctx, char **args, int argc);
int cmd_status  (repl_ctx_t *ctx, char **args, int argc);
int cmd_list    (repl_ctx_t *ctx, char **args, int argc);
int cmd_info    (repl_ctx_t *ctx, char **args, int argc);
int cmd_restock (repl_ctx_t *ctx, char **args, int argc);
int cmd_mode    (repl_ctx_t *ctx, char **args, int argc);
int cmd_reset   (repl_ctx_t *ctx, char **args, int argc);
int cmd_help    (repl_ctx_t *ctx, char **args, int argc);
int cmd_exit    (repl_ctx_t *ctx, char **args, int argc);

cmd_entry_t cmd_table[] = {
    {"cook",    "cook <recipe>",                         cmd_cook},
    {"wait",    "wait <N>",                              cmd_wait},
    {"status",  "status",                                cmd_status},
    {"list",    "list <recipes|ingredients|appliances>", cmd_list},
    {"info",    "info <recipe>",                         cmd_info},
    {"restock", "restock <ingredient> <qty>",            cmd_restock},
    {"mode",    "mode <real|sim>",                       cmd_mode},
    {"reset",   "reset",                                 cmd_reset},
    {"help",    "help",                                  cmd_help},
    {"exit",    "exit",                                  cmd_exit}
};
int cmd_table_size = 10;
