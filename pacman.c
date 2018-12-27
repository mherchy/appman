#include "pacman.h"

int debug = 0;

int main(int argc, char const *argv[])
{
    // Mode debug = d en seconde position
    if(argc >= 2 && argv[1][0] == 'd') debug = 1;
	
    WINDOW *boite;
    char *msg;
    int taille,l,c;

    init_game();
    init_colors();


    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    penv();


    getch();
    endwin();
    
    free(boite);
    
    return 0;
}