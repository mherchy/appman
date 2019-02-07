#ifndef COORD_H
#define COORD_H


/**
 * Position d'un joueur / ennemie
 */
typedef struct s_position {
    int8_t x;
    int8_t y;
} t_pos;


/**
 * "vitesse" d'un joueur / ennemie
 * Correspond à son sens de déplacement
 * (-1 ou 1)
 */
typedef struct s_velocity {
    int8_t x;
    int8_t y;
} t_vit;


/**
 * CROSSROAD
 * Etat des chemins disponibles entourant une position (x,y)
 * (0:Mur, 1:Chemin possible)
 */
typedef struct struct_crossroads {
    unsigned int top:1;
    unsigned int bottom:1;
    unsigned int left:1;
    unsigned int right:1;
} t_cross;

#endif
