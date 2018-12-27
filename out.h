#include <ncurses.h>
#include "map.h"

#define WALL_PAIR 100
#define VOID_PAIR 101
#define COIN_PAIR 102
#define APP_PAIR 110
#define B1_PAIR 111
#define B2_PAIR 112
#define B3_PAIR 113
#define B4_PAIR 114

void penv();
void init_colors();