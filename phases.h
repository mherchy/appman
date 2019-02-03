#ifndef PHASES_H
#define PHASES_H

#define PHASE_ATTACK 1
#define PHASE_PATROL 0

typedef struct s_phase {
	int8_t type;
	int duree;
} t_phase;


const t_phase PHASES_JEU[4] = {
	{PHASE_ATTACK, 10},
	{PHASE_PATROL, 10},
	{PHASE_ATTACK, 10},
	{PHASE_PATROL, 10}
};

#endif
