#include "main_player.h"

extern int shm_lastkey_id;


// DEAMON DE GESTION DU CLAVIER
// RANGE LE RESULTAT DANS LA MEMOIRE PARTAGÉ
int main_player() {
    int ch;

    //KEYBOARD
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_t *sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);

    char key;

    while (1) {
        ch = getch();
        DEV("touche préssée");

        switch (ch) {
            case KEY_LEFT:
                key = 'G';
                break;
            case KEY_RIGHT:
                key = 'D';
                break;
            case KEY_UP:
                key = 'H';
                break;
            case KEY_DOWN:
                key = 'B';
                break;
            case KEY_EXIT:
                /*kill(getppid(),SIGTERM);
                exit(EXIT_SUCCESS);*/
                break;
            default:
                key = '0';
                break;
        }

        DEVC("Touche : %c", key);

        // Inscription dans shm
        sem_wait(sem_keyboard);
        *lastkeypressed = key;
        sem_post(sem_keyboard);
    }

}
