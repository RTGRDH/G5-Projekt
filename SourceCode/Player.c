//
//  player.c
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright � 2020 Ernst. All rights reserved.
//
#include <stdlib.h>
#include <math.h>
#include "player.h"

#define PUBLIC
#define PRIVATE static
#ifndef M_PI
#define M_PI acos(-1.0)
#endif

PRIVATE int PLAYER_WIDTH = 30;
PRIVATE int PLAYER_HEIGTH = 30;

struct Player_type
{
    float PLAYER_POSITION_X;
    float PLAYER_POSITION_Y;
    float direction;
    float speed;
};

PUBLIC Player createPlayer(float x, float y)
{
    Player p = malloc(sizeof(struct Player_type));
    p->PLAYER_POSITION_X = x;
    p->PLAYER_POSITION_Y = y;
    p->direction = 0;
    float speed = 0;
    return p;
}

PUBLIC void setPlayerPositionX(Player p, float x){
    p->PLAYER_POSITION_X = x;
}
PUBLIC void setPlayerPositionY(Player p, float y){
    p->PLAYER_POSITION_Y = y;
}
PUBLIC void setPlayerDirection(Player p, float d){
    p->direction = d;
}
PUBLIC void setPlayerSpeed(Player p, float s){
    p->speed = s;
}
PUBLIC void setPlayerPosition_X_and_Y(Player p, float new_x, float new_y){
    p->PLAYER_POSITION_X = new_x;
    p->PLAYER_POSITION_Y = new_y;
}
PUBLIC void setPlayerEverything(Player p, float new_position_on_X_axis, float new_position_on_Y_axis, float new_direction, float new_speed){
    p->PLAYER_POSITION_X = new_position_on_X_axis;
    p->PLAYER_POSITION_Y = new_position_on_Y_axis;
    p->direction = new_direction;
    p->speed = new_speed;

}

PUBLIC float getPlayerPositionX(Player p){
    return p->PLAYER_POSITION_X;
}
PUBLIC float getPlayerPositionY(Player p){
    return p->PLAYER_POSITION_Y;
}
PUBLIC float getPlayerDirection(Player p){
    return p->direction;
}
PUBLIC float getPlayerSpeed(Player p){
    return p->speed;
}

PUBLIC void changePlayerSpeed(Player p, float change){
    p->speed = p->speed + change;
}
PUBLIC void changePlayerDirection(Player p, float change){
    p->direction = p-> direction - change;
    while (p->direction >= 360)
        p->direction = p->direction - 360;
    while (p-> direction < 0 )
        p->direction = p->direction +360;
}
PUBLIC void updatePlayerPosition(Player p, float GAMESPEED)
{
    p->PLAYER_POSITION_X = p->PLAYER_POSITION_X + cos(p->direction*M_PI/180)*p->speed*GAMESPEED;
    p->PLAYER_POSITION_Y = p->PLAYER_POSITION_Y + sin(p->direction*M_PI/180)*p->speed*GAMESPEED;
    p->speed = p->speed *0.995;
    if(p->speed < 0.1 && p-> speed > -0.1)
        p->speed = 0;
}


PUBLIC int getPlayerWidth(){
    return PLAYER_WIDTH;
}
PUBLIC int getPlayerHeight(){
    return PLAYER_HEIGTH;
}
