//Global
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

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
#include "main_enemy.h"
#include "def/shared_struct.h"
#include "def/coord.h"
#include "map.h"

//dev
#include "dev.h"

#define EVENT_NULL      0
#define EVENT_SCORE     1
#define EVENT_VICTOIRE  2
#define EVENT_ECHEC     4

int main_game();


//SIGNAUX
static void handler(int sig);

static void handler_term(int sig);

static void handler_atexit();

