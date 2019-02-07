#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <wait.h>

//Signaux
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//main
#include "main_display.h"
#include "main_game.h"
#include "main_player.h"

//ini
#include "shm_ini.h"

//dev
#include "dev.h"

//Permet d'initier le fifo de debug
int debug;

//Indicateur de l'état du jeu
static int run;

//Indispensable pour attraper les signaux
static void handler_main(int sig);

/**
 * Fonction main
 * Processus principale
 * Appelle les precessus fils GAME, DISP, ET KEYBOARD
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char const *argv[]) {

    /**
     * DECLARATIONS
     */

    // Les pid fils
    pid_t game_pid, disp_pid, user_pid;


    /**
     * INITIALISATION
     */

    //Le jeu est initialement en route
    run = 1;

    // Mode debug = "d*" en seconde position
    if (argc >= 2 && (argv[1][0] == 'd' || argv[1][0] == 'D')) {
        debug = 1;
        ini_fifo();
        //Macro permettant le debug sur un autre terminal
        DEV("FIFO IS ON");
    }


    //Gestionnaire de signaux, attrape les SIGTERM
    struct sigaction action_quit;
    action_quit.sa_flags = 0;
    action_quit.sa_handler = handler_main;
    sigaction(SIGTERM, &action_quit, NULL);


    //Initialisation des shm et de leurs sémaphores associés
    ini_common_shm();


    /**
     * FORKS
     */

    //PROC SYSTEME DE JEU
    game_pid = fork();
    if (game_pid == 0) {
        //traitement fils
        //processus Jeu
        main_game();
        exit(EXIT_SUCCESS);
    }

    // NCURSES
    initscr();              // Start
    cbreak();               // Line buffering disabled
    keypad(stdscr, TRUE);   // I need that F1
    curs_set(0);            //Cacher le curseur

    //Vérification du tty
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit_fifo();
        exit(EXIT_FAILURE);
    }

    //PROC AFFICHAGE
    disp_pid = fork();
    if (disp_pid == 0) {
        //traitement fils
        //processus Jeu
        main_display();
        exit(EXIT_SUCCESS);
    }

    //PROC LECTURE CLAVIER
    user_pid = fork();
    if (user_pid == 0) {
        //traitement fils
        //processus Jeu
        main_player();
        exit(EXIT_SUCCESS);
    }


    //On attend 2s que tout soit initialisé
    sleep(2);



    /**
     * INITIALISATIONS PROPRES AU MAIN
     */

    //Initialisation des timers

    //Temps avant le traitement du système de jeu
    struct timespec timergame = {0, 200000000}; //0.2s
    //Temps estimé du traitement du système de jeu avant affichage
    struct timespec timerdisp = {0, 200000000}; //0.2s



    /**
     * TRAVAIL
     *
     *
     * Le travail principale du main est de fournir un signal périodique aux
     * processus fils et de gérer ainsi le rythme du jeu et la synchronisation globale
     * SIGUSR1 : signal de loop
     */
    while (run) {

        //tempo avant envoi d'un signal au processus jeu
        nanosleep(&timergame, NULL);
        kill(game_pid, SIGUSR1);
        DEV("SIG! (GAME)");

        //tempo avant envoi d'un signal au processus d'affichage
        nanosleep(&timerdisp, NULL);
        kill(disp_pid, SIGUSR1);
        DEV("SIG! (DISP)");

    }

    /**
     * run == 0
     * fin du jeu
     */

    //Envoi des signaux de terminaison
    DEV("Veuillez mourrir...");
    kill(game_pid, SIGTERM);
    kill(disp_pid, SIGTERM);
    kill(user_pid, SIGTERM);

    //Attente des zombies
    DEV("J'attend vos morts");
    waitpid(disp_pid, NULL, 0);
    waitpid(game_pid, NULL, 0);
    waitpid(user_pid, NULL, 0);

    //Fin de ncurses
    endwin();


    //Fermeture du fifo
    if (debug) exit_fifo();


    return 0;
}

/**
 * Gestionnaire de signaux
 * Attrape les signaux SIGTERM envoyé par un processus fils
 * @param sig
 */
static void handler_main(int sig) {
    if (sig == SIGTERM) {
        DEV("[MAIN HANDLER] Arret demandé");
        //Fin de la boucle de traitement principale
        run = 0;
    }
}
