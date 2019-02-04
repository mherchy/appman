#include "main_player.h"

extern int shm_lastkey_id;


// DEAMON DE GESTION DU CLAVIER
// RANGE LE RESULTAT DANS LA MEMOIRE PARTAGÉ
int main_player() {
    int ch;
    int run = 1;

    //KEYBOARD
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_t *sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);

    char key = '0';

    while (run) {
        ch = getch();
        DEV("[KEYBOARD] touche préssée");

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
            case KEY_F(1):
                DEV("[KEYBOARD] ARRET demandé par l'user");
                kill(getppid(), SIGTERM);
                //exit(EXIT_SUCCESS);
                run = 0;
                break;
            default:
                key = '0';
                break;
        }

        DEVC("[KEYBOARD] Touche : %c", key);

        // Inscription dans shm
        sem_wait(sem_keyboard);
        *lastkeypressed = key;
        sem_post(sem_keyboard);
    }

    DEV("[KEYBOARD] Je me détruit");
    shmdt(lastkeypressed);
    sem_close(sem_keyboard);

}
