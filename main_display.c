#include "main_display.h"

/**
 * SHM
 * @see shm_ini.c
 */
// id shm
extern int shm_overview_id, shm_map_id;
// SHM et Semaphores attachés
static sem_t *sem_map;
static sem_t *sem_overview;
static t_map_unit *shared_map;
static t_overview *shared_overview;

/**
 * Fonction principale du processus d'affichage
 * @return
 */
int main_display() {

    /**
     * INITIALISATIONS
     */


    //Initialisation des couleurs
    init_colors();


    //Attachement des shm et des sémaphores attachés
    //MAP
    shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_map = sem_open(SHM_SEM_MAP, O_RDWR);
    //OVERVIEW
    shared_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);



    //SIGNAUX

    //SIGUSR1 : loop
    struct sigaction usr1_action;
    struct sigaction old_action;
    usr1_action.sa_flags = 0;
    usr1_action.sa_handler = handler;
    sigaction(SIGUSR1, &usr1_action, &old_action);

    //TERM : Fin du jeu
    struct sigaction term_action;
    sigset_t mask;
    sigfillset(&mask);
    term_action.sa_mask = mask;
    term_action.sa_flags = 0;
    term_action.sa_handler = handler_term;
    sigaction(SIGTERM, &term_action, NULL);

    //Pour quitter en douceur
    atexit(handler_atexit);

    //Pour le sigwait
    //Ne réagit qu'aux signaux SIGUSR1
    //handler() sera ignoré si le processus est bloqué sur un sigwait
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    int sigreturned;




    //message de début
    print_home_message("Appuyez sur F1 pour quitter en cours de partie");




    /**
     * TRAVAIL
     */
    while (1) {

        //On attend le feux vert du processus père : SIGUSR1
        sigwait(&sigset, &sigreturned);


        //Affichage de l'environnement (SHM)
        sem_wait(sem_map);
        print_env(shared_map);
        sem_post(sem_map);


        //Affichage des personnages (SHM)
        sem_wait(sem_overview);
        print_personnages(shared_overview);
        sem_post(sem_overview);


        //Apparition à l'écran
        refresh_screen();

    }


}

/**
 * Handler vide car remplacé par des sigwait
 * @param sig
 */
static void handler(int sig) {}

/**
 * SIGTERM
 * @param sig
 */
static void handler_term(int sig) {
    DEV("[DISPLAY] Je meurs...");
    exit(EXIT_SUCCESS);
}

/**
 * Appelé par atexit()
 * Détachement des shm et sem
 */
static void handler_atexit() {
    DEV("[DISPLAY] AT_EXIT");
    sem_close(sem_map);
    sem_close(sem_overview);
    shmdt(shared_map);
    shmdt(shared_overview);
}
