#include "main_game.h"

#define NB_ENEMIES 4


extern int shm_overview_id, shm_lastkey_id, shm_map_id;

//Semaphores et SHM attachés
static sem_t *sem_map;
static sem_t *sem_overview;
static sem_t *sem_keyboard;
static t_map_unit *shared_map;
static t_overview *shared_overview;
static char *lastkeypressed;

int main_game() {
    //DECLARATIONS
    int i;
    //Threads
    pthread_t th_burglar[4];


    //SIGNAUX
    //Ini des signaux
    //USR1
    struct sigaction action_usr1;
    action_usr1.sa_flags = 0;
    action_usr1.sa_handler = handler;
    sigaction(SIGUSR1, &action_usr1, NULL);

    //TERM
    struct sigaction action_term;
    sigset_t mask;
    sigfillset(&mask);
    action_term.sa_mask = mask;
    action_term.sa_flags = 0;
    action_term.sa_handler = handler_term;
    sigaction(SIGTERM, &action_term, NULL);

    atexit(handler_atexit);

    //On ne réagit qu'au signaux USR1
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    if (sigismember(&sigset, SIGUSR1)) DEV("[GAME] SIGUSR1 en fait partie");
    int sigreturned;

    //Création des 4 énemies
    t_enemy enemies_strategy[4] = {
            {0, {1,  1},  {0, 0}},
            {1, {26, 1},  {0, 0}},
            {2, {1,  28}, {0, 0}},
            {3, {26, 28}, {0, 0}}
    };

    //INITIALISATIONS
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


    //BOUCLE PRINCIPALE
    while (1) {
        //Attente d'un signal d'alarme
        DEV("[GAME] avant la pause");
        //On attend le feux vert
        sigwait(&sigset, &sigreturned);
        DEV("[GAME] fin de la pause");



        // TRAITEMENTS PRELIMIAIRES

        for (i = 0; i < NB_ENEMIES; i++) {
            pthread_create(&(th_burglar[i]), NULL, main_enemy, &(enemies_strategy[i]));
        }
        //... on attend la fin du traitement des IA
        for (i = 0; i < NB_ENEMIES; i++) {
            pthread_join(th_burglar[i], NULL);
        }



        //WORK

        int event = EVENT_NULL;

        //MAJ DES POSITIONS

        //Je récupère la dernière volonté du joueur
        sem_wait(sem_keyboard);
        char choice = (*lastkeypressed);
        sem_post(sem_keyboard);

        DEVC("[GAME] last key pressed = %c", choice);


        //DEV
        //choice = '0';



        // Ecriture sur overview, BLOCAGE JUSQUE FIN DU TRAITEMENT
        sem_wait(sem_overview);

        // JOUEUR
        t_pos new_pos_joueur = shared_overview->app;
        t_cross crossr;
        sem_wait(sem_map);
        get_crossroad(&new_pos_joueur, &crossr, shared_map);
        sem_post(sem_map);
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
        shared_overview->app = new_pos_joueur;

        //IA
        for (i = 0; i < NB_ENEMIES; i++) {
            //J'inscris les décisions individuelles des IA dans le shm
            shared_overview->enemy[i] = enemies_strategy[i].pos;

            //Un énemie a-t'il rattrapé le joueur ?
            if (enemies_strategy[i].pos.x == new_pos_joueur.x && enemies_strategy[i].pos.y == new_pos_joueur.y)
                event |= EVENT_ECHEC;
        }

        sem_post(sem_overview);

        //Points
        sem_wait(sem_map);
        if (get_coin(&new_pos_joueur, shared_map))
            event |= EVENT_SCORE;
        sem_post(sem_map);


        //TRAITEMENTS POST-MOUVEMENTS
        if (event & EVENT_SCORE) {
            //score++
        }

        if (event & EVENT_ECHEC) {
            //ON STOPPE LE JEU AVEC UN MESSAGE
        } else if (event & EVENT_VICTOIRE) {
            //ON STOPPE LE JEU AVEC UN MESSAGE
        }

        //VICTOIRE

        //ECHEC

        //SINON



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
