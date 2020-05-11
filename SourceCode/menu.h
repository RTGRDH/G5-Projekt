//
//  menu.h
//  G5_Build
//
//  Created by Ernst on 2020-04-29.
//  Copyright © 2020 Ernst. All rights reserved.
//

#ifndef menu_h
#define menu_h

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
void initMenu(SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
bool menu(SDL_Window *window, SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
void displayMenu(SDL_Renderer* renderer);
void cleanUpInit();
void cleanUpConnectionScene();
void DisplayConnectionScene(SDL_Renderer* renderer);
void updateText(SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
void initConnectionScene(SDL_Renderer *renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
bool connectionScene(SDL_Window *window, SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
#endif /* menu_h */
