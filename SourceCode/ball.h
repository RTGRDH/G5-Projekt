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
void setBallPosition_X_and_Y(Ball b, float new_x, float new_y);
void setBallEverything(Ball b, float new_position_on_X_axis, float new_position_on_Y_axis, float new_direction, float new_speed);
void updateBallPosition(Ball b, float GAMESPEED);
int getBallWidth();
int getBallHeight();

#endif //* Player_h *