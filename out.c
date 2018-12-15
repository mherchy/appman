#include "out.h"


void penv()
{
    int l,c;

	for(l=0; l<ENV_N_LINES; l++)
    {
        for (c = 0; c < ENV_N_COLS; c++)
        {
            /*
            switch (MAP[l][c])
            {
                case E_WALL:
                attron(COLOR_PAIR(PLAYER_PAIR));
                mvaddch(y, x, PLAYER);
                attroff(COLOR_PAIR(PLAYER_PAIR));
                    break;

                case E_COIN:
                    break;

                case E_VOID:
                    break;
            }*/

            attron(COLOR_PAIR(MAP[l][c]));
            addch(' ');
            attroff(COLOR_PAIR(MAP[l][c]));
        }
        addch('\n');
    }

    refresh();
}