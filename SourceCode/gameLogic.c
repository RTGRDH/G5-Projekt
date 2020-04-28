#include <stdlib.h>
#include <math.h>
#include "ball.h"
#include "player.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#define PUBLIC
#define PRIVATE static
#ifndef M_PI
#define M_PI acos(-1.0)
#endif

bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall){
    if(gPlayer ->y >= gBall ->y  + gBall ->h)
        return 0;
    if(gPlayer->x >= gBall ->x + gBall->w)
        return 0;
    if(gPlayer->y + gPlayer->h <= gBall->y)
        return 0;
    if(gPlayer ->x + gPlayer->w <= gBall->x)
        return 0;
    return 1;

}


bool ballRightGoalCollision(SDL_Rect* gBall){
    if(gBall->x >910){
        if(gBall ->y > 120 && gBall->y < 395){
             return 1;
        }

    }
    return 0;
}

bool ballLeftGoalCollision(SDL_Rect* gBall){
    if(gBall->x <5){
        if(gBall ->y > 120 && gBall->y < 395){
             return 1;
        }

    }
    return 0;
}



void speedLimit(Player p)   //how fast do the cars go forward and in reverse? if a player goes beyond the cars limits, this sets the car back to it's limits
{
    if (getPlayerSpeed(p)>MAX_SPEED_FORWARD)
        setPlayerSpeed(p, MAX_SPEED_FORWARD);
    if (getPlayerSpeed(p)<MAX_SPEED_REVERSE)
        setPlayerSpeed(p, MAX_SPEED_REVERSE);
}

float distanceBallPlayer(Ball b, Player p)
{
    float squared_X_distance, squared_Y_distance, squared_hypotenuse, distance;

    squared_X_distance = pow((getBallPositionX(b) + getBallWidth()/2) - (getPlayerPositionX(p) + getPlayerWidth()/2), 2);
    squared_Y_distance = pow((getBallPositionY(b) + getBallHeight()/2) - (getPlayerPositionY(p) + getPlayerHeight()/2), 2);
    
    squared_hypotenuse = squared_X_distance + squared_Y_distance;
    //hypotenuse IS the distance
    distance = sqrt(squared_hypotenuse);
    
    return distance;
}

float angleBallPlayer(Ball b, Player p)
{
    float x_distance, y_distance, direction;

    x_distance = (getBallPositionX(b) - getBallWidth()/2) - (getPlayerPositionX(p) - getPlayerWidth()/2);
    y_distance = (getBallPositionY(b) + getBallHeight()/2) - (getPlayerPositionY(p) + getPlayerHeight()/2);

    if (x_distance == 0)
    {
        if (y_distance > 0)
            return 0;
        else
            return 180;
    }
    else
    {
    if (x_distance >= 0)
        {
            direction = -atan(y_distance/x_distance)*180/M_PI+90;
            return direction;
        }
        else
        {
            direction = atan(y_distance/x_distance)*180/M_PI + 270;
            return direction;
        }
    }
}

void colissionDetectionPlayerArena(Player p)    //keeping the abstract version of the car on the arena prevents you getting visually stuck in the corner while lost far off the map
{
    float slow = 0.96;                          //slow factor for colliding with arena walls helps changing direction when you messed up bad, and in theory slows down people who ram it but it's very neglible for that purpose
    if (getPlayerPositionX(p) < 0)
    {
        setPlayerPositionX(p, 0);
        setPlayerSpeed(p, getPlayerSpeed(p)*slow);
    }
    if (getPlayerPositionY(p) < 0)
    {
        setPlayerPositionY(p, 0);
       setPlayerSpeed(p, getPlayerSpeed(p)*slow);
    }
    if (getPlayerPositionX(p) > WINDOW_WIDTH - getPlayerHeight())
    {
        setPlayerPositionX(p, WINDOW_WIDTH - getPlayerWidth());
        setPlayerSpeed(p, getPlayerSpeed(p)*slow);
    }
    if (getPlayerPositionY(p) > WINDOW_HEIGTH - getPlayerHeight())
    {
        setPlayerPositionY(p, WINDOW_HEIGTH - getPlayerHeight());
        setPlayerSpeed(p, getPlayerSpeed(p)*slow);
    }
}

void colissionDetectionBallArena(Ball b)
{
    float slow = 0.8;
    if (getBallPositionX(b) < 0)
    {
        setBallPositionX(b, 0);
        setBallSpeed(b, getBallSpeed(b)*slow);
        setBallDirection(b, xInvertDirection(getBallDirection(b)));
    }
    if (getBallPositionY(b) < 0)
    {
        setBallPositionY(b, 0);
        setBallSpeed(b, getBallSpeed(b)*slow);
        setBallDirection(b, yInvertDirection(getBallDirection(b)));
    }
    if (getBallPositionX(b) > WINDOW_WIDTH - getBallHeight())
    {
        setBallPositionX(b, WINDOW_WIDTH - getBallWidth());
        setBallSpeed(b, getBallSpeed(b)*slow);
        setBallDirection(b, xInvertDirection(getBallDirection(b)));
    }
    if (getBallPositionY(b) > WINDOW_HEIGTH - getBallHeight())
    {
        setBallPositionY(b, WINDOW_HEIGTH - getBallHeight());
        setBallSpeed(b, getBallSpeed(b)*slow);
        setBallDirection(b, yInvertDirection(getBallDirection(b)));
    }
}

float yInvertDirection(float direction)
{
    direction = -direction + 180;
    return direction;
}
float xInvertDirection(float direction)
{
    direction = -direction;
    return direction;
}<