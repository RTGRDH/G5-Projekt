#include <stdlib.h>
#include <math.h>
#include "ball.h"
#include "player.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include "gameLogic.h"

#define PUBLIC
#define PRIVATE static
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define WINDOW_WIDTH 960
#define WINDOW_HEIGTH 540


#ifndef M_PI
#define M_PI acos(-1.0)
#endif


bool PlayerCollision(Player player, Player player2)
{
    if(distancePlayerPlayer(player,player2)<27)
    {
        if(getPlayerDirection(player) >=40 && getPlayerDirection(player) <=140 ||
        getPlayerDirection(player2) >=40 && getPlayerDirection(player2) <=140 )
        {
            if(getPlayerDirection(player2) >=0 && getPlayerDirection(player2) <=30 
            || getPlayerDirection(player2) >=330 && getPlayerDirection(player2)<=360
            
            || getPlayerDirection(player) >=0 && getPlayerDirection(player) <=30 
            || getPlayerDirection(player) >=330 && getPlayerDirection(player)<=360)
            {
                if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                {
                    setPlayerDirection(player2,getPlayerDirection(player)-getPlayerDirection(player2));
                    setPlayerDirection(player,getPlayerDirection(player)-180);
                    return 1;

                } 
                else if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                {
                    setPlayerDirection(player,getPlayerDirection(player2)-getPlayerDirection(player));
                    setPlayerDirection(player2,getPlayerDirection(player)-180);
                    return 1;

                }
                    
            }
            else if(getPlayerDirection(player2) >=180 && getPlayerDirection(player2) <=210 
                    || getPlayerDirection(player2) >=150 && getPlayerDirection(player2)<=180

                    || getPlayerDirection(player) >=180 && getPlayerDirection(player) <=210 
                    || getPlayerDirection(player) >=150 && getPlayerDirection(player)<=180)
                    {
                        if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                        {
                            setPlayerDirection(player2,getPlayerDirection(player2)-getPlayerDirection(player));
                            setPlayerDirection(player,getPlayerDirection(player)-180);
                            return 1;
                        }
                        else if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                        {
                            setPlayerDirection(player,getPlayerDirection(player)-getPlayerDirection(player2));
                            setPlayerDirection(player2,getPlayerDirection(player)-180);
                            return 1;

                        }
                    }
        }  
        if(getPlayerDirection(player) >=220 && getPlayerDirection(player) <=320
            || getPlayerDirection(player2) >=220 && getPlayerDirection(player2) <=320)
            {
                if(getPlayerDirection(player2) >=0 && getPlayerDirection(player2) <=30 
                || getPlayerDirection(player2) >=330 && getPlayerDirection(player2)<=360

                || getPlayerDirection(player) >=0 && getPlayerDirection(player) <=30 
                || getPlayerDirection(player) >=330 && getPlayerDirection(player)<=360)
                {
                    if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                    {
                        setPlayerDirection(player2,getPlayerDirection(player)+getPlayerDirection(player2));
                        setPlayerDirection(player,getPlayerDirection(player)-180);
                        return 1;

                    } 
                    else if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                    {
                        setPlayerDirection(player,getPlayerDirection(player2)+getPlayerDirection(player));
                        setPlayerDirection(player2,getPlayerDirection(player2)-180);
                        return 1;

                    }

                    
                }
                else if(getPlayerDirection(player2) >=180 && getPlayerDirection(player2) <=210 
                    || getPlayerDirection(player2) >=150 && getPlayerDirection(player2)<=180

                    || getPlayerDirection(player) >=180 && getPlayerDirection(player) <=210 
                    || getPlayerDirection(player) >=150 && getPlayerDirection(player)<=180)
                    {
                        if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                        {
                            setPlayerDirection(player,-getPlayerDirection(player));
                            setPlayerDirection(player2,getPlayerDirection(player2)+90);
                            return 1;
                        } 
                        else if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                        {
                            setPlayerDirection(player2,-getPlayerDirection(player2));
                            setPlayerDirection(player,getPlayerDirection(player)+90);
                            return 1;
                        
                        }
                    }

            }
            
        if(getPlayerDirection(player) >=220 && getPlayerDirection(player) <=320 && 
            getPlayerDirection(player2) >=40 && getPlayerDirection(player2) <=140 ||

            getPlayerDirection(player2) >=220 && getPlayerDirection(player2) <=320 &&
            getPlayerDirection(player) >=40 && getPlayerDirection(player) <=140 )
            {
                if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2)-180);
                    setPlayerDirection(player,getPlayerDirection(player)+180);
                    return 1;
                        
                }
                else if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2)+180);
                    setPlayerDirection(player,getPlayerDirection(player)-180);
                    return 1;
                }
                
                
            }  
                
        if(getPlayerDirection(player) >=220 && getPlayerDirection(player) <=320 && 
            getPlayerDirection(player2) >=220 && getPlayerDirection(player2) <=320)
            {
                if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2)-180);
                    setPlayerDirection(player,getPlayerDirection(player));
                    return 1;
                }
                else if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2));
                    setPlayerDirection(player,getPlayerDirection(player)-180);
                    return 1;
                }
            }
            else if(getPlayerDirection(player) >=40 && getPlayerDirection(player) <=140 &&
            getPlayerDirection(player2) >=40 && getPlayerDirection(player2) <=140 )
            {
                if(getPlayerPositionY(player2) > getPlayerPositionY(player))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2));
                    setPlayerDirection(player,getPlayerDirection(player)+180);
                    return 1;
                }

                else if(getPlayerPositionY(player) > getPlayerPositionY(player2))
                {
                    setPlayerDirection(player2,getPlayerDirection(player2)+180);
                    setPlayerDirection(player,getPlayerDirection(player));
                    return 1;
                }
            }
         
        if(getPlayerDirection(player) >= 0 && getPlayerDirection(player) < 40 || 
            getPlayerDirection(player) >= 330 && getPlayerDirection(player) <= 360 )
            {
                if(getPlayerDirection(player2) >= 0 && getPlayerDirection(player2) < 40 ||
                getPlayerDirection(player2) >330 && getPlayerDirection(player2) <=360)
                {
                    if(getPlayerPositionX(player)<getPlayerPositionX(player2))
                    {
                        setPlayerDirection(player,getPlayerDirection(player)+180);
                        setPlayerDirection(player2,getPlayerDirection(player2));
                        return 1;

                    } 
                    else if(getPlayerPositionX(player)>getPlayerPositionX(player2))
                    {
                        setPlayerDirection(player2,getPlayerDirection(player2)+180);
                        setPlayerDirection(player,getPlayerDirection(player));
                        return 1;

                    }
                }
            }
            
        if(getPlayerDirection(player2) >=180 && getPlayerDirection(player2) <=210 || 
            getPlayerDirection(player2) >=150 && getPlayerDirection(player2) <=180 ||

            getPlayerDirection(player) >=180 && getPlayerDirection(player) <=210  ||
            getPlayerDirection(player) >=150 && getPlayerDirection(player) <=180)
            {
                if(getPlayerDirection(player) >=0  && getPlayerDirection(player) <= 30 || 
                getPlayerDirection(player) >= 330 && getPlayerDirection(player) <=360  ||

                getPlayerDirection(player2) >=0 && getPlayerDirection(player2) <=30 || 
                getPlayerDirection(player2) >=330 && getPlayerDirection(player2) <=360 )
                {
                    if(getPlayerPositionX(player)<getPlayerPositionX(player2))
                    {
                        setPlayerDirection(player,getPlayerDirection(player)+180);
                        setPlayerDirection(player2,getPlayerDirection(player2)-180);
                        return 1;

                    }
                    else if(getPlayerPositionX(player)>getPlayerPositionX(player2))
                    {
                        setPlayerDirection(player2,getPlayerDirection(player2)+180);
                        setPlayerDirection(player,getPlayerDirection(player)-180);
                        return 1;

                    }
                }
            }
                 
 
        if(getPlayerDirection(player) >=150  && getPlayerDirection(player) <= 180 || 
            getPlayerDirection(player) >= 180 && getPlayerDirection(player) <=210  )
            {
                if(getPlayerDirection(player2) >=180 && getPlayerDirection(player2) <=210 
                || getPlayerDirection(player2) >=150 && getPlayerDirection(player2) <=180)
                {
                    if(getPlayerPositionX(player2)>getPlayerPositionX(player))
                    {
                        setPlayerDirection(player,getPlayerDirection(player));
                        setPlayerDirection(player2,getPlayerDirection(player2)-180);
                        return 1;

                    }
                    else if(getPlayerPositionX(player2)<getPlayerPositionX(player))
                    {
                        setPlayerDirection(player,getPlayerDirection(player)-180);
                        setPlayerDirection(player2,getPlayerDirection(player2));
                        return 1;

                    }
                } 
              
            }

    setPlayerDirection(player,getPlayerDirection(player)-180);
    setPlayerDirection(player2,getPlayerDirection(player2)-180);
    }

    return 0; 

}


bool BallRightPostCollision(SDL_Rect* gBall){
  if(gBall->x >900){
        if(gBall ->y > 395 && gBall->y < 450){
             return 1;
        }else if(gBall ->y > 60 && gBall->y < 125){
            return 1;
        }
    }
    return 0;
}

bool BallLeftPostCollision(SDL_Rect* gBall){
    if(gBall->x <45){
          if(gBall ->y > 395 && gBall->y < 450){
             return 1;
        }else if(gBall ->y > 60 && gBall->y < 125){
            return 1;
        }
    }
    return 0;
}



bool ballRightGoalCollision(SDL_Rect* gBall){
    if(gBall->x > 920 ){
        if(gBall ->y > 120 && gBall->y < 395){
             return 1;
        }

    }
    return 0;
}

bool ballLeftGoalCollision(SDL_Rect* gBall){
    if(gBall->x < 20 ){
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

float distancePlayerPlayer(Player p1, Player p2)
{
    float squared_X_distance, squared_Y_distance, squared_hypotenuse, distance;

    squared_X_distance = pow((getPlayerPositionX(p1) + getPlayerWidth()/2) - (getPlayerPositionX(p2) + getPlayerWidth()/2), 2);
    squared_Y_distance = pow((getPlayerPositionY(p1) + getPlayerHeight()/2) - (getPlayerPositionY(p2) + getPlayerHeight()/2), 2);
    
    squared_hypotenuse = squared_X_distance + squared_Y_distance;
    //hypotenuse IS the distance
    distance = sqrt(squared_hypotenuse);
    
    return distance;
}

float angleBallPlayer(Ball b, Player p)
{
    float x_distance, y_distance, direction;

    x_distance = (getBallPositionX(b) + getBallWidth()/2) - (getPlayerPositionX(p) + getPlayerWidth()/2);
    y_distance = (getBallPositionY(b) + getBallHeight()/2) - (getPlayerPositionY(p) + getPlayerHeight()/2);

    if (x_distance == 0)
    {
        if (y_distance > 0)
            return 90;
        else
            return -90;
    }
    else
    {
    if (x_distance > 0)
        {
            direction = atan(y_distance/x_distance)*180/M_PI;
            return direction;
        }
        else
        {
            direction = atan(y_distance/x_distance)*180/M_PI +180;
            return direction;
        }
        direction = atan(y_distance/x_distance)*180/M_PI;
        return direction;
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
    direction = -direction;
    return direction;
}
float xInvertDirection(float direction)
{
    direction = -direction + 180;
    return direction;
}