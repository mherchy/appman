#ifndef APPMAN_MAIN_DISPLAY_H
#define APPMAN_MAIN_DISPLAY_H

#include <unistd.h>
#include <semaphore.h>
#include "shm_ini.h"

#include "map.h"

#include "out.h"


//dev
#include "dev.h"


int main_display();

static void handler(int sig);

static void handler_term(int sig);

#endif
