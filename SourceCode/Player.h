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
void changePlayerDirection(Player p, float c);
void changePlayerSpeed(Player p, float c);
void updatePlayerPosition(Player p, float S);
int getPlayerWidth();
int getPlayerHeight();

#endif /* Player_h */