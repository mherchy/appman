#include "pacman.h"

int main(void) {
	
    WINDOW *boite;
    char *msg;
    int taille,l,c;

    init_game();


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