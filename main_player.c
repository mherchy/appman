#include "main_player.h"

extern int shm_lastkey_id;

/**
 * ECOUTE CLAVIER
 * @return
 */
int main_player() {

    int ch;
    int run = 1;

    //SHM KEYBOARD ATACHEMENT
    char *lastkeypressed = (char *) shmat(shm_lastkey_id, NULL, 0);
    sem_t *sem_keyboard = sem_open(SHM_SEM_LASTKEY, O_RDWR);

    char key = '0';

    while (run) {
        ch = getch();

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
                kill(getppid(), SIGTERM);
                run = 0;
                break;
            default:
                key = '0';
                break;
        }


        // Inscription dans shm
        sem_wait(sem_keyboard);
        *lastkeypressed = key;
        sem_post(sem_keyboard);
    }

    shmdt(lastkeypressed);
    sem_close(sem_keyboard);

}
