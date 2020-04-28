#ifndef GameLogic_h
#define GameLogic_h
#include <stdio.h>

bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall);
bool ballRightGoalCollision(SDL_Rect* gBall);
bool ballLeftGoalCollision(SDL_Rect* gBall);
void speedLimit(Player p);
float distanceBallPlayer(Ball b, Player p);
float angleBallPlayer(Ball b, Player p);
void colissionDetectionPlayerArena(Player p);
void colissionDetectionBallArena(Ball b);
float yInvertDirection(float direction);
float xInvertDirection(float direction);