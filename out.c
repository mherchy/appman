#include "out.h"

extern int debug;

void penv()
{
    int l,c;

    if(debug)
    {

        char no = '0';

        printw("XX");
        //Impression des n de col
        for (c = 0; c < ENV_N_COLS; c++)
        {
            addch(no+(c % 10));
        }

        addch('\n');
        no = '0';

    }

	for(l=0; l< ENV_N_LINES; l++)
    {
        if(debug) printw("%2d", l);
        for (c = 0; c < ENV_N_COLS; c++)
        {
            switch (MAP[l][c])
            {
                case E_WALL:
                attron(COLOR_PAIR(WALL_PAIR));
                addch(' ');
                attroff(COLOR_PAIR(WALL_PAIR));
                    break;

                case E_COIN:
                    attron(COLOR_PAIR(COIN_PAIR));
                    addch(' ');
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

    refresh();
}


void init_colors()
{
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
    init_pair(COIN_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(APP_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(B1_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(B2_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(B3_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(B4_PAIR, COLOR_BLUE, COLOR_BLACK);
}