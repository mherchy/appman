#include "main_display.h"

//PARTIE DEBUG
#include "dev.h"

extern int shm_overview_id, shm_map_id;

static void handler(int sig) {}


int main_display() {

    //Initialisation
    init_colors();

    //Initialisation de la shm et des sémaphores attachés
    //MAP
    t_map_unit *m = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_t *sem_map = sem_open(SHM_SEM_MAP, O_RDWR);
    //OVERVIEW
    t_overview *ov = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_t *sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);

    //Ini des signaux
    //TODO essayer des trucs avec masque.c du TP1

    struct sigaction action;
    struct sigaction old_action;
    action.sa_flags = 0;
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, &old_action);

    //On ne réagit qu'au signaux d'alarme
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    //if (sigismember(&sigset, SIGUSR1)) //DEV("[AFFICHAGE] SIGUSR1 en fait partie");
    int sigreturned, i;

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
        print_env(m);
        sem_post(sem_map);
        //DEV("[AFFICHAGE] -Sortie");


        //Affichage des personnages
        //DEV("[AFFICHAGE] Semaphore overview");
        sem_wait(sem_overview);
        //DEV("[AFFICHAGE] -Entrée");
        print_personnages(ov);
        sem_post(sem_overview);
        //DEV("[AFFICHAGE] -Sortie");

        //Apparition à l'écran
        refresh_screen();


    }


}
