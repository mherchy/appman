#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "map.h"

//dev
#include <stdio.h>
#define DEV printf

#define DIR_LEFT -1
#define DIR_RIGHT 1

#define DIR_TOP -1
#define DIR_BOTTOM 1

typedef struct s_burglar {
	t_pos pos;
	t_vit vit;
} t_enemy;


void choose_next_direction(t_vit * v);
void set_vit(int8_t x, int8_t y, t_vit * v);

void enemy_fixture(t_enemy * e);
void objectif_fixture(t_pos * o);
void enemy_ask(t_enemy * e);
void objectif_ask(t_pos * o);
