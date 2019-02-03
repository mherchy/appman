#ifndef SH_STRUCT_H
#define SH_STRUCT_H

#include <stdint.h>

#include "dimensions.h"
#include "coord.h"

//OVERVIEW
typedef struct struct_shm {
    t_pos enemy[4];
    t_pos app;
} t_overview;

//MAP
typedef int8_t t_map_unit;
typedef t_map_unit t_map[ENV_N_LINES * ENV_N_COLS];

//CROSS
typedef struct struct_crossroads {
    unsigned int top:1;
    unsigned int bottom:1;
    unsigned int left:1;
    unsigned int right:1;
} t_cross;


#endif
