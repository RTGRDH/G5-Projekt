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
bool menu(SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH);
void displayMenu(SDL_Renderer* renderer);
void cleanUpInit();
#endif /* menu_h */
