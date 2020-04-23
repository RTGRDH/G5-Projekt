//
//  Player.h
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright © 2020 Ernst. All rights reserved.
//

#ifndef Ball_h
#define Ball_h

#include <stdio.h>
typedef struct Ball_type *Ball;
float getBallPositionX(Ball b);
float getBallPositionY(Ball b);
float getBallDirection(Ball b);
float getBallSpeed(Ball b);
Ball createBall(float x, float y);
void setBallPositionY(Ball b, float y);
void setBallPositionX(Ball b, float x);
void setBallDirection(Ball b, float d);
void setBallSpeed(Ball b, float s);
void updateBallPosition(Ball b, float GAMESPEED);
int getBallWidth();
int getBallHeight();

#endif //* Player_h *