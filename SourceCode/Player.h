//
//  Player.h
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright � 2020 Ernst. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
typedef struct Player_type *Player;
Player createPlayer(float x, float y);
float getPlayerPositionX(Player p);
float getPlayerPositionY(Player p);
float getPlayerDirection(Player p);
float getPlayerSpeed(Player p);
void setPlayerPositionX(Player p, float x);
void setPlayerPositionY(Player p, float y);
void setPlayerDirection(Player p, float x);
void setPlayerSpeed(Player p, float x);
void setPlayerPosition_X_and_Y(Player p, float new_x, float new_y);
void setPlayerEverything(Player p, float new_position_on_X_axis, float new_position_on_Y_axis, float new_direction, float new_speed);
void changePlayerDirection(Player p, float c);
void changePlayerSpeed(Player p, float c);
void updatePlayerPosition(Player p, float S);
int getPlayerWidth();
int getPlayerHeight();

#endif /* Player_h */