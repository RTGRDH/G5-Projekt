# A simple Makefile for compiling small SDL projects //200424 För det som ligger i repository

theServer: server3.o ball.o Player.o GameLogic.o
	gcc -o theServer server3.o ball.o Player.o GameLogic.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer
server3.o: server3.c
	gcc -c server3.c 
ball.o: ball.c
	gcc -c ball.c
Player.o: Player.c
	gcc -c Player.c
GameLogic.o: GameLogic.c
	gcc -c GameLogic.c