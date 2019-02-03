#include "main_game.h"

#define NB_ENEMIES 4


extern int shm_overview_id, shm_lastkey_id, shm_map_id;

//SIGNAUX
static void handler(int sig);

static void handler_term(int sig);

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

    //On ne réagit qu'au signaux d'alarme
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
    t_overview *sharerd_overview = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_t *sem_overview = sem_open(SHM_SEM_OVERVIEW, O_RDWR);
    //KEYBOARD
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_t *sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);
    //MAP
    t_map_unit *shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    sem_t *sem_map = sem_open(SHM_SEM_MAP, O_RDWR);


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
        t_pos new_pos_joueur = sharerd_overview->app;
        t_cross crossr;
        sem_wait(sem_map);
        get_crossroad(&new_pos_joueur, &crossr, shared_map);
        sem_post(sem_map);
        DEV("[GAME] Routes possibles :");
        DEVD("[GAME] H:%d", crossr.top);
        DEVD("[GAME] B:%d", crossr.bottom);
        DEVD("[GAME] G:%d", crossr.left);
        DEVD("[GAME] D:%d", crossr.right);
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
        sharerd_overview->app = new_pos_joueur;

        //IA
        for (i = 0; i < NB_ENEMIES; i++) {
            //J'inscris les décisions individuelles des IA dans le shm
            sharerd_overview->enemy[i] = enemies_strategy[i].pos;
        }

        sem_post(sem_overview);

    }


}

static void handler(int sig) {}