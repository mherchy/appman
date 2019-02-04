#include "map.h"

//NO DATA HERE

/**
 * Utilisation avec un sémaphore
 * @param x
 * @param y
 * @return
 */
t_map_unit get_pos(int8_t x, int8_t y, t_map m) {
    int8_t retour;
	if(x < 0 || y < 0) return -1;
    else if (x >= ENV_N_COLS || y >= ENV_N_LINES) return -1;
    else return m[get_uni_coord_id(x, y)];
}

void set_pos(int8_t x, int8_t y, t_map_unit val, t_map m) {
    m[get_uni_coord_id(x, y)] = val;
}

/**
 * Utilisatio avec un sémaphore
 */
int8_t is_path(int8_t x, int8_t y, t_map m) {
    return (get_pos(x, y, m) > 0);
}

/**
 * Utilisatio avec un sémaphore
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

int get_uni_coord_id(int x, int y) {
    return (y * ENV_N_COLS) + x;
}




//Actions sur le terrain

int get_coin(t_pos *p, t_map m) {
    t_map_unit unit = get_pos(p->x, p->y, m);
    if (unit == E_COIN) {
        set_pos(p->x, p->y, E_VOID, m);
        //score++
        return 1;
    } else if (unit != E_VOID) return -1;
}