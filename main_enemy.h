//Global
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

//Semaphore
#include <semaphore.h>

//Signaux
#include <signal.h>

//shm
#include <sys/shm.h>

//thread
#include <pthread.h>

#include <unistd.h>


//Game
#include "def/shared_struct.h"
#include "def/coord.h"
#include "map.h"


//dev
#include "dev.h"


/**
 * Struct t_enemy
 * Correspond au résonnement et aux choix des énemies
 * Sa position est par la suite copié dans la variable partagée overview
 */
typedef struct s_burglar {
    uint8_t id;
    t_pos pos;
    t_vit vit;
} t_enemy;


void *main_enemy(void *param);

t_vit *choose_next_direction(t_enemy *e, t_pos *obj, t_map map, sem_t *sem_map);

t_vit *set_vit(int8_t x, int8_t y, t_vit *v);