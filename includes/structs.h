/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Core data structures: ingredient, appliance, recipe and kitchen
*/
#ifndef STRUCTS_H_
    #define STRUCTS_H_

#define MAX_INGREDIENTS 64
#define MAX_APPLIANCES  16
#define MAX_RECIPES     64
#define MAX_VISITED     64

typedef struct {
    char    name[64];
    float   quantity;
    char    unit[16];
    int     unlimited;
} ingredient_t;

typedef struct {
    char    name[64];
    int     busy;
    int     time_left;
    char    current_recipe[64];
} appliance_t;

typedef struct {
    char    ingredient[64];
    float   quantity;
    char    unit[16];
} recipe_ingredient_t;

typedef struct {
    char    appliance[64];
    int     duration;
} recipe_appliance_t;

typedef struct {
    char                name[64];
    recipe_ingredient_t ingredients[MAX_INGREDIENTS];
    int                 nb_ingredients;
    recipe_appliance_t  appliances[MAX_APPLIANCES];
    int                 nb_appliances;
    char                sub_recipes[MAX_RECIPES][64];
    int                 nb_sub_recipes;
    char                output_ingredient[64];
    float               output_quantity;
    char                output_unit[16];
} recipe_t;

typedef struct {
    ingredient_t    ingredients[MAX_INGREDIENTS];
    int             nb_ingredients;
    appliance_t     appliances[MAX_APPLIANCES];
    int             nb_appliances;
    recipe_t        recipes[MAX_RECIPES];
    int             nb_recipes;
    int             simulated_time;
    int             stock_mode;
} kitchen_t;

#endif /* STRUCTS_H_ */
