#ifndef GameLogic_h
#define GameLogic_h
#include <stdio.h>
#include "ball.h"
#include "player.h"
#include <SDL2/SDL.h>



bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall);
bool ballRightGoalCollision(SDL_Rect* gBall);
bool ballLeftGoalCollision(SDL_Rect* gBall);
bool BallRightPostCollision(SDL_Rect* gBall);
bool BallLeftPostCollision(SDL_Rect* gBall);
void speedLimit(Player p);
float distanceBallPlayer(Ball b, Player p);
float distancePlayerPlayer(Player p1, Player p2);
float angleBallPlayer(Ball b, Player p);
bool PlayerCollision(Player player, Player player2);
void colissionDetectionPlayerArena(Player p);
void colissionDetectionBallArena(Ball b);
float yInvertDirection(float direction);
float xInvertDirection(float direction);

#endif
