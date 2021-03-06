//
//  player.c
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright © 2020 Ernst. All rights reserved.
//
#include <stdlib.h>
#include <math.h>
#include "ball.h"

#define PUBLIC
#define PRIVATE static
#ifndef M_PI
#define M_PI acos(-1.0)
#endif

PRIVATE int BALL_WIDTH = 20;
PRIVATE int BALL_HEIGTH = 20;

struct Ball_type
{
    float BALL_POSITION_X;
    float BALL_POSITION_Y;
    float speed;
    float direction;
};

PUBLIC Ball createBall(float x, float y)
{
    Ball b = malloc(sizeof(struct Ball_type));
    b->BALL_POSITION_Y = y;
    b->BALL_POSITION_X = x;
    b->speed = 0;
    b->direction = 0;
    return b;
}
PUBLIC void setBallPositionX(Ball b, float x){
    b->BALL_POSITION_X  = x;
}

PUBLIC void setBallPositionY(Ball b, float y){
    b->BALL_POSITION_Y = y;
}

PUBLIC void setBallSpeed(Ball b, float s){
    b->speed = s;
}

PUBLIC void setBallDirection(Ball b, float d){
    b->direction = d;
}

PUBLIC float getBallPositionX(Ball b){
    return b->BALL_POSITION_X;
}

PUBLIC float getBallPositionY(Ball b){
    return b->BALL_POSITION_Y;
}

PUBLIC float getBallDirection(Ball b){
    return b->direction;
}

PUBLIC float getBallSpeed(Ball b){
    return b->speed;
}

PUBLIC void updateBallPosition(Ball b, float GAMESPEED)
{
    b->BALL_POSITION_X = b->BALL_POSITION_X + sin(b->direction * M_PI / 180) * b->speed * GAMESPEED;
    b->BALL_POSITION_Y = b->BALL_POSITION_Y + cos(b->direction * M_PI / 180) * b->speed * GAMESPEED;
    b->speed = b->speed * 0.975;
}

PUBLIC int getBallWidth(){
    return BALL_WIDTH;
}

PUBLIC int getBallHeight(){
    return BALL_HEIGTH;
}