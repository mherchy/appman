#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define E_OUT -0
#define E_WALL 0
#define E_VOID 1
#define E_COIN 2

#define ENV_N_LINES 30
#define ENV_N_COLS 28

typedef const uint8_t t_map[ENV_N_LINES][ENV_N_COLS];

typedef struct struct_crossroads {
   unsigned int top:1;
   unsigned int bottom:1;
   unsigned int left:1;
   unsigned int right:1;
} t_cross;

typedef struct s_position {
	int8_t x;
	int8_t y;
} t_pos;

typedef struct s_velocity {
	int8_t x;
	int8_t y;
} t_vit;

extern t_map MAP;


int8_t get_pos(int8_t x, int8_t y);
int8_t is_path(int8_t x, int8_t y);
void get_crossroad(t_pos* p, t_cross* crossr);


#endif