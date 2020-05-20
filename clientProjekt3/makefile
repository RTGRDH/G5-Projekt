# A simple Makefile for compiling small SDL projects //200424 För det som ligger i repository

theGame: client3.o ball.o Player.o GameLogic.o menu.o
	gcc -o theGame client3.o ball.o Player.o GameLogic.o menu.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf
client3.o: client3.c
	gcc -c client3.c
menu.o: menu.c
	gcc -c menu.c
ball.o: ball.c
	gcc -c ball.c
Player.o: Player.c
	gcc -c Player.c
GameLogic.o: GameLogic.c
	gcc -c GameLogic.c