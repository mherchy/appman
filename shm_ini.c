#include "shm_ini.h"

#include <stdio.h>
#include <errno.h>

/**
 * EXPLICATIONS : SHM
 * Il y a en tout 3 shm
 *  - overview (structure*) : placement des personnages sur la matrice
 *  - map (tableau) : configuration du terrain (mur, espace vide, coin ...)
 *  - lastkey (char*) : dernière touche préssée par le joueur
 */

/**
 * SHM ID
 * GLOBAUX POUR TOUS LES PROCESSUS : SHMs (int)
 */
int shm_overview_id, shm_lastkey_id, shm_map_id;

/**
 * IPC keys
 */
static key_t key1, key2, key3;

/**
 * Sémaphores attachés aux shm
 */
static sem_t *sem_overview;
static sem_t *sem_key;
static sem_t *sem_map;


/**
 * Fonction d'initialisation de la mémoire partagée
 * et des sémaphores attachés
 */
void ini_common_shm() {

    //CLES
    key1 = ftok(__FILE__, 1);
    key2 = ftok(__FILE__, 2);
    key3 = ftok(__FILE__, 3);



    /**
     * Déclarations des shm
     */

    shm_overview_id = shmget(key1, sizeof(t_overview), 0666 | IPC_CREAT);
    ERRNG(shm_overview_id); //Tester si erreur

    shm_lastkey_id = shmget(key2, sizeof(char), 0666 | IPC_CREAT);
    ERRNG(shm_lastkey_id);

    shm_map_id = shmget(key3, sizeof(t_map), 0666 | IPC_CREAT);
    ERRNG(shm_map_id);



    /**
     * Initialisations des semaphores attachés
     */

    sem_overview = sem_open(SHM_SEM_OVERVIEW, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_overview, SEM_FAILED); //Tester si erreur
    sem_init(sem_overview, 0, 1);

    sem_key = sem_open(SHM_SEM_LASTKEY, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_key, SEM_FAILED);
    sem_init(sem_key, 0, 1);

    sem_map = sem_open(SHM_SEM_MAP, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_map, SEM_FAILED);
    sem_init(sem_map, 0, 1);


    //Initialisation des variables
    //DEV("Initialisation des variables");
    ini_map();
    ini_overview();
    ini_key();
}





/**
 * Fonctions d'Initialisation des données
 */


/**
 * Initialisation des données de la map
 */
void ini_map() {

    int l, c;

    // Map local, contiens les couloirs, murs et $
    t_map_unit map_init[ENV_N_LINES][ENV_N_COLS] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0},
            {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
            {0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0},
            {0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0},
            {0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0},
            {0, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0},
            {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
            {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    /**
     * Déclaration d'un pointeur vers la shm map
     */
    t_map_unit *shared_map;
    shared_map = (t_map_unit *) shmat(shm_map_id, NULL, 0);


    /**
     * Copie des données
     * de map_init vers shared_map
     */
    sem_wait(sem_map);
    int i = 0;
    for (l = 0; l < ENV_N_LINES; l++) {
        for (c = 0; c < ENV_N_COLS; c++) {
            shared_map[i] = map_init[l][c];
            i++;
        }
    }
    sem_post(sem_map);


    //Détachement du shm
    shmdt(shared_map);

}


/**
 * Initialisation des données de la structure overview
 */
void ini_overview() {

    //Données en local
    t_overview ov_init = {{{1, 1}, {26, 1}, {1, 28}, {26, 28}},
                          {13,     22}};

    //Attachement du shm
    t_overview *ov = (t_overview *) shmat(shm_overview_id, NULL, 0);

    //Copie des données
    sem_wait(sem_overview);
    *ov = ov_init;
    sem_post(sem_overview);

    //Déttachement du shm
    shmdt(ov);

}


/**
 * Initialisation des données d'interraction utilisateur
 */
void ini_key() {

    //Attachement du shm
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);

    //Initialisation de la donnée
    sem_wait(sem_key);
    *lastkeypressed = '0';
    sem_post(sem_key);

    //Déttachement du shm
    shmdt(lastkeypressed);
}
