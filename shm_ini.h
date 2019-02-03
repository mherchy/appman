//Global
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include <semaphore.h>

//Signaux
#include <signal.h>

//shm
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <pthread.h>

#include <unistd.h>


#include "def/shared_struct.h"
#include "def/shm_sem.h"

//dev
#include "dev.h"


void ini_map();

void ini_overview();

void ini_key();

void ini_common_shm();
