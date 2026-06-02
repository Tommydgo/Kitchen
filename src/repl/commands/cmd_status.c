/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** status command: show busy appliances and time remaining
*/
#include "global_includes.h"

int cmd_status(repl_ctx_t *ctx, char **args, int argc)
{
    (void)args; (void)argc;
    int any = 0;
    for (int i = 0; i < ctx->k->nb_appliances; i++) {
        appliance_t *a = &ctx->k->appliances[i];
        if (a->busy) {
            printf("  %s: cooking '%s' (%ds remaining)\n",
                a->name, a->current_recipe, a->time_left);
            any = 1;
        }
    }
    if (!any)
        printf("All appliances are free.\n");
    return SUCCESS;
}
