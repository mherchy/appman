#include "shm_ini.h"

#include <stdio.h>
#include <errno.h>

//IPC key
static key_t key1, key2, key3;

static sem_t *sem_overview;
static sem_t *sem_key;
static sem_t *sem_map;

//GLOBAL POUR TOUS LES PROCESSUS : SHMs (int)
int shm_overview_id, shm_lastkey_id, shm_map_id;


//main
void ini_common_shm() {

    key1 = ftok(__FILE__, 1);
    //DEVD("KEY %d",key1);
    key2 = ftok(__FILE__, 2);
    //DEVD("KEY %d",key2);
    key3 = ftok(__FILE__, 3);
    //DEVD("KEY %d",key3);

    // Déclarations des shm
    //DEV("Déclarations des shm");

    shm_overview_id = shmget(key1, sizeof(t_overview), 0666 | IPC_CREAT);
    ERRNG(shm_overview_id);

    shm_lastkey_id = shmget(key2, sizeof(char), 0666 | IPC_CREAT);
    ERRNG(shm_lastkey_id);

    shm_map_id = shmget(key3, sizeof(t_map), 0666 | IPC_CREAT);
    ERRNG(shm_map_id);

    //Initialisations des semaphores attachés
    //DEV("Initialisations des semaphores attachés");

    //DEV("overview");
    sem_overview = sem_open(SHM_SEM_OVERVIEW, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_overview, SEM_FAILED);
    sem_init(sem_overview, 0, 1);

    //DEV("lastkey");
    sem_key = sem_open(SHM_SEM_LASTKEY, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_key, SEM_FAILED);
    sem_init(sem_key, 0, 1);

    //DEV("map");
    sem_map = sem_open(SHM_SEM_MAP, O_CREAT | O_RDWR, 0777, 1);
    ERRSI(sem_map, SEM_FAILED);
    sem_init(sem_map, 0, 1);


    //Initialisation des variables
    //DEV("Initialisation des variables");
    ini_map();
    ini_overview();
    ini_key();
}





// Fonctions d'Initialisation


void ini_map() {

    //DEV("Fonction ini map");


    int l, c;

    // Map local
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

    //DEV("Map locale faite");


    t_map_unit *map;
    map = (t_map_unit *) shmat(shm_map_id, NULL, 0);
    //DEVP("map ptr = %d",map);


    //DEV("Map part ini");


    sem_wait(sem_map);
    int i = 0;
    for (l = 0; l < ENV_N_LINES; l++) {
        for (c = 0; c < ENV_N_COLS; c++) {
            map[i] = map_init[l][c];
            i++;
        }
    }
    sem_post(sem_map);

    //DEV("fini");


    shmdt(map);

}

void ini_overview() {

    t_overview ov_init = {{{1, 1}, {26, 1}, {1, 28}, {26, 28}},
                          {13,     22}};

    t_overview *ov = (t_overview *) shmat(shm_overview_id, NULL, 0);
    sem_wait(sem_overview);
    *ov = ov_init;
    sem_post(sem_overview);
    shmdt(ov);

}

void ini_key() {
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_t *sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);

    sem_wait(sem_keyboard);
    *lastkeypressed = '0';
    DEVC("[INI] le char initialisé est %c", *lastkeypressed);
    sem_post(sem_keyboard);
    shmdt(lastkeypressed);

}
