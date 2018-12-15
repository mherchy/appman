#include "ini.h"

void init_game()
{
    initscr();

    start_color();
    init_pair(E_WALL, COLOR_YELLOW, COLOR_GREEN);
    init_pair(E_COIN, COLOR_CYAN, COLOR_BLUE);
    init_pair(E_VOID, COLOR_BLACK, COLOR_WHITE);
}

