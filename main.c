#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Signaux
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//main
#include "main_display.h"
#include "main_game.h"
#include "main_player.h"

//shm
#include <sys/shm.h>
#include <wait.h>

//Headers
#include "shm_ini.h"

//dev
#include "dev.h"


int debug;
static int run;

static void handler_main(int sig);

int main(int argc, char const *argv[]) {

    pid_t game_pid, disp_pid, user_pid;

    run = 1;

    // Mode debug = d* en seconde position
    if (argc >= 2 && (argv[1][0] == 'd' || argv[1][0] == 'D')) {
        debug = 1;
        ini_fifo();
        DEV("FIFO IS ON");
    }


    //Gestionnaire de signaux
    struct sigaction action_quit;
    action_quit.sa_flags = 0;
    action_quit.sa_handler = handler_main;
    sigaction(SIGTERM, &action_quit, NULL);




    //prs
    ini_common_shm();



    //FORKS
    game_pid = fork();
    if (game_pid == 0) {
        //traitement fils
        //processus Jeu
        main_game();
        exit(EXIT_SUCCESS);
    }

    // INITIALISATION NCURSES
    initscr();            // Start curses mode
    cbreak();            // Line buffering disabled, Pass on everty thing to me
    keypad(stdscr, TRUE);        // I need that nifty F1
    curs_set(0);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit_fifo();
        exit(EXIT_FAILURE);
    }

    disp_pid = fork();
    if (disp_pid == 0) {
        //traitement fils
        //processus Jeu
        main_display();
        exit(EXIT_SUCCESS);
    }


    user_pid = fork();
    if (user_pid == 0) {
        //traitement fils
        //processus Jeu
        main_player();
        exit(EXIT_SUCCESS);
    }



    //On attend 1s que tt soit initialisé
    sleep(2);


    //WORK
    //timer
    int i;
    struct timespec timergame = {0, 200000000}; //0.2s
    struct timespec timerdisp = {0, 200000000}; //0.2s
    while (run) {
        //Envoi de NBITER*2 signauxde test
        nanosleep(&timergame, NULL);
        DEV("SIG! (GAME)");
        kill(game_pid, SIGUSR1);
        //Envoi de NBITER*2 signauxde test
        nanosleep(&timerdisp, NULL);
        DEV("SIG! (DISP)");
        kill(disp_pid, SIGUSR1);
    }

    //Envoi des signaux de terminaison
    DEV("Veuillez mourrir...");
    kill(game_pid, SIGTERM);
    kill(disp_pid, SIGTERM);
    kill(user_pid, SIGTERM);

    DEV("J'attend vos morts");
    waitpid(disp_pid, NULL, 0);
    waitpid(game_pid, NULL, 0);
    waitpid(user_pid, NULL, 0);

    //Fin de ncurses
    endwin();


    if (debug) exit_fifo();


    return 0;
}


static void handler_main(int sig) {

    DEV("[HANDLER]");
    if (sig == SIGTERM) {
        DEV("[HANDLER] Arret demandé");
        run = 0;
    }

}
