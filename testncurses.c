
#include <ncurses.h>
#include <string.h>

int main(void) {
	
    WINDOW *boite;
    char *msg;
    int taille;
    
    initscr();

    while(1) {

        clear();    // Efface la fenêtre (donc l'ancien message)

        //Si la dimention n'est pas assez grande, afficher un message different
        if(COLS < 80 || LINES < 30)
        {
        	strcpy(msg,"MERCI D'AGRANDIR LA VUE");
        }
        else
        {
        	strcpy(msg,"Texte au centre");
        }

        //Ecriture du message
        taille=strlen(msg);
        mvprintw(LINES/2, (COLS / 2) - (taille / 2), msg);
        refresh();

        // Prévention du redimentionnement
        if(getch() != 410)
            break;
    }
    
    endwin();
    
    free(boite);
    
    return 0;
}