/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Core data structures: ingredient, appliance, recipe and kitchen
*/
#ifndef STRUCTS_H_
    #define STRUCTS_H_

#define MAX_VISITED 64

typedef struct ingredient_s {
    char   name[64];
    float  quantity;
    char   unit[16];
    int    unlimited;
    struct ingredient_s *next;
} ingredient_t;

typedef struct appliance_s {
    char   name[64];
    int    busy;
    int    time_left;
    char   current_recipe[64];
    struct appliance_s *next;
} appliance_t;

typedef struct recipe_ingredient_s {
    char   ingredient[64];
    float  quantity;
    char   unit[16];
    struct recipe_ingredient_s *next;
} recipe_ingredient_t;

typedef struct recipe_appliance_s {
    char   appliance[64];
    int    duration;
    struct recipe_appliance_s *next;
} recipe_appliance_t;

typedef struct recipe_s {
    char                 name[64];
    recipe_ingredient_t *ingredients;
    recipe_appliance_t  *appliances;
    char               **sub_recipes;
    int                  nb_sub_recipes;
    char                 output_ingredient[64];
    float                output_quantity;
    char                 output_unit[16];
    struct recipe_s     *next;
} recipe_t;

typedef struct {
    ingredient_t *ingredients;
    int           nb_ingredients;
    appliance_t  *appliances;
    int           nb_appliances;
    recipe_t     *recipes;
    int           nb_recipes;
    int           simulated_time;
    int           stock_mode;
} kitchen_t;

#endif /* STRUCTS_H_ */
