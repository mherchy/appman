#include "main_display.h"

//PARTIE DEBUG
#include "dev.h"

extern int shm_overview_id, shm_map_id;

//Semaphores et SHM attachés
static sem_t *sem_map;
static sem_t *sem_overview;
static t_map_unit *shared_map;
static t_overview *shared_overview;


int main_display() {

    //Initialisation
    init_colors();

    //Initialisation de la shm et des sémaphores attachés
    //MAP
    shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_map = sem_open(SHM_SEM_MAP, O_RDWR);
    //OVERVIEW
    shared_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);

    //Ini des signaux
    //TODO essayer des trucs avec masque.c du TP1

    struct sigaction action;
    struct sigaction old_action;
    action.sa_flags = 0;
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, &old_action);

    //TERM
    struct sigaction action_term;
    sigset_t mask;
    sigfillset(&mask);
    action_term.sa_mask = mask;
    action_term.sa_flags = 0;
    action_term.sa_handler = handler_term;
    sigaction(SIGTERM, &action_term, NULL);

    atexit(handler_atexit);

    //On ne réagit qu'au signaux d'USR1
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    int sigreturned;

    //message de début
    print_home_message("Appuyez sur F1 pour quitter en cours de partie");

    //BOUCLE
    while (1) {

        //DEV("[AFFICHAGE] PAUSE");
        //On attend le feux vert
        sigwait(&sigset, &sigreturned);
        //DEV("[AFFICHAGE] fin de la pause, affichage");



        //Affichage de l'environnement
        //DEV("[AFFICHAGE] Semaphore map");
        sem_wait(sem_map);
        //DEV("[AFFICHAGE] -Entrée");
        print_env(shared_map);
        sem_post(sem_map);
        //DEV("[AFFICHAGE] -Sortie");


        //Affichage des personnages
        //DEV("[AFFICHAGE] Semaphore overview");
        sem_wait(sem_overview);
        //DEV("[AFFICHAGE] -Entrée");
        print_personnages(shared_overview);
        sem_post(sem_overview);
        //DEV("[AFFICHAGE] -Sortie");

        //Apparition à l'écran
        refresh_screen();


    }


}


static void handler(int sig) {}

static void handler_term(int sig) {
    DEV("[DISPLAY] Je meurs...");
    exit(EXIT_SUCCESS);
}

static void handler_atexit() {
    DEV("[DISPLAY] AT_EXIT");
    sem_close(sem_map);
    sem_close(sem_overview);
    shmdt(shared_map);
    shmdt(shared_overview);
}
