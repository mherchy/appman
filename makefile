main_player.o: main_player.c
	gcc -c main_player.c -lpthread -lncurses

main_enemy.o: main_enemy.c
	gcc -c main_enemy.c -lptread

test_display: main.c main_display.c shm_ini.c dev.c out.c map.c
	gcc main.c main_display.c shm_ini.c dev.c out.c map.c -pthread -lncurses -o test_display

test_game: main.c main_display.c shm_ini.c dev.c out.c map.c main_game.c main_enemy.c main_player.c
	gcc main.c main_display.c shm_ini.c dev.c out.c map.c main_game.c main_enemy.c  main_player.c -pthread -lncurses -o test_game
