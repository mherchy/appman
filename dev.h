#ifndef APPMAN_DEV_H
#define APPMAN_DEV_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>
#include <error.h>
#include <errno.h>



// MACROS
#define DEV(str)    debug_fifo(str)
#define DEVD(str, i)    debug_fifo_int(str,i)
#define DEVDD(str, i, j)    debug_fifo_2int(str,i,j)
#define DEVF(str, i)    debug_fifo_float(str,i)
#define DEVC(str, c) debug_fifo_char(str,c)
#define DEVS(str, s) debug_fifo_string(str,s)
#define DEVP(str, s) debug_fifo_ptr(str,s)
#define ERRNG(var)  if((var)==-1)debug_fifo(strerror(errno));
#define ERRSI(var, forb)  if((var)==forb)debug_fifo(strerror(errno));



int ini_fifo();

int exit_fifo();

void debug_fifo(char *str);

void debug_fifo_int(char *txt, int i);

void debug_fifo_2int(char *txt, int i, int j);

void debug_fifo_float(char *txt, float i);

void debug_fifo_char(char *txt, char i);

void debug_fifo_string(char *txt, char *i);

void debug_fifo_ptr(char *txt, void *i);


#endif
