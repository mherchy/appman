#include "out.h"


void penv()
{
    int l,c;

    char no = '0';

    printw("XX");
    //Impression des n de col
    for (c = 0; c < ENV_N_COLS; c++)
    {
        addch(no+(c % 10));
    }

    addch('\n');
    no = '0';

	for(l=0; l<ENV_N_LINES; l++)
    {
        printw("%2d", l);
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


