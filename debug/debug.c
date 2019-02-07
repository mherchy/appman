#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char const *argv[]) {

    //Ouvre un canal FIFO existant

    int sortieServeur = open("debugappman", O_RDONLY);
    char str[100];

    while (1) {
        if (read(sortieServeur, str, 100) != 0) {
            printf("%s\n", str);
        }
    }

}
