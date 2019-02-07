all: app_man

app_man: main.c main_display.o shm_ini.o dev.o out.o map.o main_game.o main_enemy.o main_player.o
	gcc main.c main_display.o shm_ini.o dev.o out.o map.o main_game.o main_enemy.o  main_player.o -pthread -lncurses -o app_man

main_display.o: main_display.c
	gcc -c main_display.c

main_game.o: main_game.c
	gcc -c main_game.c

main_enemy.o: main_enemy.c
	gcc -c main_enemy.c

main_player.o: main_player.c
	gcc -c main_player.c

shm_ini.o: shm_ini.c
	gcc -c shm_ini.c

dev.o: dev.c debug/debug
	gcc -c dev.c

out.o: out.c
	gcc -c out.c

map.o: map.c
	gcc -c map.c



#DEBUG
debug/debug: debug/debug.c debug/debugappman
	gcc debug/debug.c -o debug/debug

debug/debugappman:
	mkfifo -m0777 debug/debugappman


clean:
	rm *.o app_man debug/debugappman debug/debug