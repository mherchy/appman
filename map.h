#include <stdint.h>

#define E_WALL 0
#define E_VOID 1
#define E_COIN 2

#define ENV_N_LINES 30
#define ENV_N_COLS 28

typedef const uint8_t t_map[ENV_N_LINES][ENV_N_COLS];

extern t_map MAP;