#include "dev.h"

static int out;
static int fifo_initialized = 0;

int ini_fifo() {
    out = open("debug/debugappman", O_WRONLY);
    fifo_initialized = 1;
}

int exit_fifo() {
    close(out);
    fifo_initialized = 0;
}

void debug_fifo(char *str) {
    if (fifo_initialized) write(out, str, 100);
}

void debug_fifo_int(char *txt, int i) {
    char str[100];
    snprintf(str, 100, txt, i);
    debug_fifo(str);
}

void debug_fifo_2int(char *txt, int i, int j) {
    char str[100];
    snprintf(str, 100, txt, i, j);
    debug_fifo(str);
}

void debug_fifo_float(char *txt, float i) {
    char str[100];
    snprintf(str, 100, txt, i);
    debug_fifo(str);
}

void debug_fifo_string(char *txt, char *i) {
    char str[100];
    snprintf(str, 100, txt, i);
    debug_fifo(str);
}

void debug_fifo_char(char *txt, char i) {
    char str[100];
    snprintf(str, 100, txt, i);
    debug_fifo(str);
}

void debug_fifo_ptr(char *txt, void *i) {
    char str[100];
    snprintf(str, 100, txt, i);
    debug_fifo(str);
}

