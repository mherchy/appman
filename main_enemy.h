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


#define DIR_LEFT -1
#define DIR_RIGHT 1

#define DIR_TOP -1
#define DIR_BOTTOM 1

typedef struct s_burglar {
    uint8_t id;
    t_pos pos;
    t_vit vit;
} t_enemy;

void *main_enemy(void *param);

t_vit *choose_next_direction(t_enemy *e, t_pos *obj, t_map map, sem_t *sem_map);

t_vit *set_vit(int8_t x, int8_t y, t_vit *v);