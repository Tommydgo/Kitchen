/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Kitchen simulated time advancement
*/
#include "global_includes.h"

void kitchen_advance_time(kitchen_t *k, int seconds)
{
    k->simulated_time += seconds;
    appliances_tick(k, seconds);
}
