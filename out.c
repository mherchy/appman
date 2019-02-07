#include "out.h"


/**
 * INTERACTIONS AVEC LE TERMINAL
 * UTILISATION DE NCURSES
 */


/**
 * Affichage d'un message d'accueurl
 * @param str
 */
void print_home_message(char *str) {
    clear();
    addstr(str);
    refresh();
}

/**
 * Affichage de la map
 * @param map
 */
void print_env(t_map map) {

    int l, c;

    //On nettoie l'écran
    clear();

    //Boucle
    for (l = 0; l < ENV_N_LINES; l++) {
        for (c = 0; c < ENV_N_COLS; c++) {
            t_map_unit unit = get_pos(c, l, map);
            switch (unit) {
                case E_WALL:
                    attron(COLOR_PAIR(WALL_PAIR));
                    addch(' ');
                    attroff(COLOR_PAIR(WALL_PAIR));
                    break;

                case E_COIN:
                    attron(COLOR_PAIR(COIN_PAIR));
                    addch('$');
                    attroff(COLOR_PAIR(COIN_PAIR));
                    break;

                case E_VOID:
                    attron(COLOR_PAIR(APP_PAIR));
                    addch(' ');
                    attroff(COLOR_PAIR(APP_PAIR));
                    break;
            }

        }
        addch('\n');
    }

}

/**
 * Affiche les 4 enemies
 * @param ov
 */
void print_enemies(t_overview *ov) {
    int pairs[4] = {B1_PAIR, B2_PAIR, B3_PAIR, B4_PAIR};
    int i;
    for (i = 0; i < 4; ++i) {
        print_enemie(&(ov->enemy[i]), pairs[i]);
    }
}

/**
 * Affiche un énemie
 * @param pos
 * @param cpair
 */
void print_enemie(t_pos *pos, int cpair) {
    attron(COLOR_PAIR(cpair));
    mvaddch(pos->y, pos->x, '#');
    attroff(COLOR_PAIR(cpair));
}

/**
 * Affiche le joueur
 * @param pos
 */
void print_app(t_pos *pos) {
    attron(COLOR_PAIR(APP_PAIR));
    mvaddch(pos->y, pos->x, 'A');
    attroff(COLOR_PAIR(APP_PAIR));

}


/**
 * Affiche le joueur et les énemies
 * @param ov
 */
void print_personnages(t_overview *ov) {
    print_enemies(ov);
    print_app(&(ov->app));
}


/**
 * refresh
 */
void refresh_screen() {
    refresh();
}


/**
 * Affiche un message sour la map
 * @param str
 */
void print_status(char *str) {
    mvaddstr(ENV_N_LINES + 1, 1, str);
    refresh();
}


/**
 * Initialisation des informations de couleurs
 */
void init_colors() {
    /*  COULEURS DISPO :
    COLOR_BLACK
    COLOR_RED
    COLOR_GREEN
    COLOR_YELLOW
    COLOR_BLUE
    COLOR_MAGENTA
    COLOR_CYAN
    COLOR_WHITE
    */
    start_color();

    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(VOID_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(COIN_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(APP_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(B1_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(B2_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(B3_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(B4_PAIR, COLOR_BLUE, COLOR_BLACK);
}
