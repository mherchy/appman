#ifndef SH_STRUCT_H
#define SH_STRUCT_H

#include "dimensions.h"
#include "coord.h"

/**
 * CONTIENT LA DECLARATIONS DES STRUCTURES
 */

/**
 * t_overview
 * Contient les positions de tous les joueurs et énemis
 */
typedef struct struct_ov {
    t_pos enemy[4];
    t_pos app;
} t_overview;

/**
 * t_map
 * Tableau unidirectionnel (pour des raisons pratiques)
 * Contient la structure de la map (MUR, $, ESPACES VIDES)
 */
typedef int8_t t_map_unit;
typedef t_map_unit t_map[ENV_N_LINES * ENV_N_COLS];


#endif
