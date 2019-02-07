#ifndef MAP_H
#define MAP_H

#define E_OUT (-1)
#define E_WALL 0
#define E_VOID 1
#define E_COIN 2


#include <stdint.h>

//shm
#include <sys/shm.h>

#include <pthread.h>
#include <semaphore.h>

#include "def/dimensions.h"
#include "def/shared_struct.h"
#include "def/coord.h"
#include "def/shm_sem.h"

//dev
#include "dev.h"

t_map_unit get_pos(int8_t x, int8_t y, t_map m);
int8_t is_path(int8_t x, int8_t y, t_map m);
t_cross *get_crossroad(t_pos *p, t_cross *crossr, t_map m);
int get_uni_coord_id(int l, int c);
int get_coin(t_pos *p, t_map m);

#endif
