//
//  menu.c
//  G5_Build
//
//  Created by Ernst on 2020-04-29.
//  Copyright © 2020 Ernst. All rights reserved.
//

#include "menu.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
void initMenu(SDL_Renderer* renderer)
{
    menu(renderer);
}
void menu(SDL_Renderer* renderer)
{
    SDL_Event event;
    bool runningMenu = true;
    while(runningMenu)
    {
       displayMenu(renderer);
       
       while (SDL_PollEvent(&event))
       {
           switch (event.type)
           {
               case SDL_QUIT:
                   runningMenu = false;
                   break;
               case SDL_MOUSEBUTTONDOWN:
                   break;
           }
        }
    }
}
void displayMenu(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
