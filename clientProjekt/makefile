# A simple Makefile for compiling small SDL projects //200424 För det som ligger i repository

theGame: client2.o ball.o Player.o GameLogic.o
	gcc -o theGame client2.o ball.o Player.o GameLogic.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
client2.o: client2.c
	gcc -c client2.c
ball.o: ball.c
	gcc -c ball.c
Player.o: Player.c
	gcc -c Player.c
GameLogic.o: GameLogic.c
	gcc -c GameLogic.c