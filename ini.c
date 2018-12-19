#include "ini.h"

void init_game()
{
    initscr();

    start_color();
    init_pair(E_WALL, COLOR_GREEN, COLOR_GREEN);
    init_pair(E_COIN, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(E_VOID, COLOR_RED, COLOR_RED);
}

