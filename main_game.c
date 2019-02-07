#include "main_game.h"

#define NB_ENEMIES 4

/**
 * SHM
 * @see shm_ini.c
 */
extern int shm_overview_id, shm_lastkey_id, shm_map_id;
//Semaphores et SHM attachés
static sem_t *sem_map;
static sem_t *sem_overview;
static sem_t *sem_keyboard;
static t_map_unit *shared_map;
static t_overview *shared_overview;
static char *lastkeypressed;


/**
 * Gestion des positions des joueurs et gestion des IA
 * @return
 */
int main_game() {

    /**
     * DECLARATIONS
     */

    int i;
    //Threads
    pthread_t th_burglar[4];




    /**
     * INITIALISATIONS
     */


    //SIGNAUX

    //SIGUSR1 : loop
    struct sigaction action_usr1;
    action_usr1.sa_flags = 0;
    action_usr1.sa_handler = handler;
    sigaction(SIGUSR1, &action_usr1, NULL);

    //TERM : Fin du jeu
    struct sigaction action_term;
    sigset_t mask;
    sigfillset(&mask);
    action_term.sa_mask = mask;
    action_term.sa_flags = 0;
    action_term.sa_handler = handler_term;
    sigaction(SIGTERM, &action_term, NULL);

    //Pour quitter en douceur
    atexit(handler_atexit);

    //Pour le sigwait
    //Ne réagit qu'aux signaux SIGUSR1
    //handler() sera ignoré si le processus est bloqué sur un sigwait
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    if (sigismember(&sigset, SIGUSR1)) DEV("[GAME] SIGUSR1 en fait partie");
    int sigreturned;


    //t_enemis

    //Création des 4 "pensées" des énemies (renommé ici enemies_strategy pour faire la distinction avec overview)
    //Positions et directions initiales des énemies
    t_enemy enemies_strategy[4] = {
            {0, {1,  1},  {0, 0}},
            {1, {26, 1},  {0, 0}},
            {2, {1,  28}, {0, 0}},
            {3, {26, 28}, {0, 0}}
    };




    //SHM

    //Initialisation de la mémoire partagée et des sémaphores
    //OVERVIEW
    shared_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);
    //KEYBOARD
    lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);
    //MAP
    shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_map = sem_open(SHM_SEM_MAP, O_RDWR);

    //Initialisation du compteur de score, nombre de $ à récolleter
    int beforeVictoire = 0, score = 0;
    int8_t c, l;
    sem_wait(sem_map);
    for (l = 0; l < ENV_N_LINES; l++) {
        for (c = 0; c < ENV_N_COLS; c++) {
            if (get_pos(c, l, shared_map) == 2) beforeVictoire++;
        }
    }
    sem_post(sem_map);


    /**
     * TRAVAIL
     */

    while (1) {


        //Attente d'un signal d'alarme
        //On attend le feux vert
        sigwait(&sigset, &sigreturned);


        /**
         * IA
         */

        //Création (passage de enemies_strategy en parametre)
        for (i = 0; i < NB_ENEMIES; i++) {
            pthread_create(&(th_burglar[i]), NULL, main_enemy, &(enemies_strategy[i]));
        }
        //... on attend la fin du traitement des IA
        //Destruction du thread, synchronisation
        for (i = 0; i < NB_ENEMIES; i++) {
            pthread_join(th_burglar[i], NULL);
        }



        /**
         * RECAPITULATIF DES POSITIONS
         */

        //Event : Traitement particulier à faire en fin de boucle
        uint8_t event = EVENT_NULL;



        /**
         * OPERATION SUR SHM_KEYBOARD
         */

        //Je récupère la dernière direction voulu par le joueur
        sem_wait(sem_keyboard);
        char choice = (*lastkeypressed);
        sem_post(sem_keyboard);



        /**
         * OPERATION SUR SHM_OVERVIEW
         */

        sem_wait(sem_overview);


        // JOUEUR


        //Le calcul de la position du joueur se fait à partir de sa dernière position
        t_pos new_pos_joueur = shared_overview->app;

        //Quels chemin peut-il emprunter
        t_cross crossr;
        sem_wait(sem_map);
        get_crossroad(&new_pos_joueur, &crossr, shared_map);
        sem_post(sem_map);

        //Calcul de sa nouvelle position
        switch (choice) {
            case 'H':
                if (crossr.top) new_pos_joueur.y -= 1;
                break;
            case 'B':
                if (crossr.bottom) new_pos_joueur.y += 1;
                break;
            case 'G':
                if (crossr.left) new_pos_joueur.x -= 1;
                break;
            case 'D':
                if (crossr.right) new_pos_joueur.x += 1;
                break;
            default:
                break;
        }

        //Ecriture sur overviex
        shared_overview->app = new_pos_joueur;


        // IA
        for (i = 0; i < NB_ENEMIES; i++) {
            //J'inscris les décisions individuelles des IA dans le shm
            shared_overview->enemy[i] = enemies_strategy[i].pos;

            //Un énemie a-t'il rattrapé le joueur ?
            if (enemies_strategy[i].pos.x == new_pos_joueur.x && enemies_strategy[i].pos.y == new_pos_joueur.y)
                event |= EVENT_ECHEC;
        }

        sem_post(sem_overview);


        //Calcul des points
        sem_wait(sem_map);
        if (get_coin(&new_pos_joueur, shared_map))
            event |= EVENT_SCORE;
        sem_post(sem_map);




        /**
         * EVENTS
         */

        //Ramassage d'un $
        if (event & EVENT_SCORE) {
            score += 100;
            beforeVictoire--;
            if (beforeVictoire == 0) event |= EVENT_VICTOIRE;
        }

        //FIN DE JEU
        if (event & EVENT_ECHEC) {
            kill(getppid(), SIGTERM);
        } else if (event & EVENT_VICTOIRE) {
            kill(getppid(), SIGTERM);
        }

    }


}

static void handler(int sig) {}

static void handler_term(int sig) {
    DEV("[GAME] Je meurs...");
    exit(EXIT_SUCCESS);
}


static void handler_atexit() {
    DEV("[GAME] AT_EXIT");
    sem_close(sem_map);
    sem_close(sem_overview);
    sem_close(sem_keyboard);
    shmdt(shared_map);
    shmdt(shared_overview);
    shmdt(lastkeypressed);
}
