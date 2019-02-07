#include "map.h"

/**
 * Utilitaire de données de la map
 * Un sémaphore doit étre ouvert avant utilisation de ces fonctions
 */

/**
 * Récupérer l'information de ma map correspondant à la position donné
 * @param x
 * @param y
 * @param m
 * @return
 */
t_map_unit get_pos(int8_t x, int8_t y, t_map m) {
    if (x < 0 || y < 0) return -1;
    else if (x >= ENV_N_COLS || y >= ENV_N_LINES) return -1;
    else return m[get_uni_coord_id(x, y)];
}

/**
 * Réecrire l'information de ma map correspondant à la position donné
 * @param x
 * @param y
 * @param val
 * @param m
 */
void set_pos(int8_t x, int8_t y, t_map_unit val, t_map m) {
    m[get_uni_coord_id(x, y)] = val;
}

/**
 * La case x,y est-elle un chemin (ou bien un mur ?)
 * @param x
 * @param y
 * @param m
 * @return
 */
int8_t is_path(int8_t x, int8_t y, t_map m) {
    return (get_pos(x, y, m) > 0);
}

/**
 * Récupération des chemins disponibles pour la coordonnée pos
 */
t_cross *get_crossroad(t_pos *p, t_cross *crossr, t_map m) {
    if (is_path(p->x, p->y, m)) {
        crossr->top = is_path(p->x, p->y - 1, m);
        crossr->bottom = is_path(p->x, p->y + 1, m);
        crossr->left = is_path(p->x - 1, p->y, m);
        crossr->right = is_path(p->x + 1, p->y, m);
    }
    return crossr;
}

/**
 * Récupération de l'index du t_map_unit dans la le t_map[] en fonction de ses coordonnées
 * @param x
 * @param y
 * @return
 */
int get_uni_coord_id(int x, int y) {
    return (y * ENV_N_COLS) + x;
}


/**
 * Si la position p contient un $, le ramasser
 * @param p
 * @param m
 * @return 1 si coin, 0 si rien, -1 si erreur
 */
int get_coin(t_pos *p, t_map m) {
    t_map_unit unit = get_pos(p->x, p->y, m);
    if (unit == E_COIN) {
        set_pos(p->x, p->y, E_VOID, m);
        return 1;
    } else if (unit == E_VOID) return 0;
    else return -1;
}