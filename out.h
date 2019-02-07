#include <ncurses.h>
#include "map.h"
#include "def/shared_struct.h"

//dev
#include "dev.h"

#define WALL_PAIR 100
#define VOID_PAIR 101
#define COIN_PAIR 102
#define APP_PAIR 110
#define B1_PAIR 111
#define B2_PAIR 112
#define B3_PAIR 113
#define B4_PAIR 114

void print_home_message(char *str);

void print_env(t_map map);

void print_enemies(t_overview *ov);

void print_enemie(t_pos *pos, int cpair);

void print_personnages(t_overview *ov);

void refresh_screen();

void init_colors();