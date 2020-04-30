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
SDL_Rect playButton;
SDL_Rect exitButton;
void initMenu(SDL_Renderer* renderer)
{
    playButton.h = 50; playButton.w = 100;
    playButton.x = 300; playButton.y = 300;
    
    exitButton.h = 50; exitButton.w = 100;
    exitButton.x = 500; exitButton.y = 300;
    //menu(renderer);
}
bool menu(SDL_Renderer* renderer)
{
    initMenu(renderer);
    SDL_Event event;
    bool running = true;
    bool flag = false;
    while(running)
    {
       displayMenu(renderer);
       while (SDL_PollEvent(&event))
       {
           switch (event.type)
           {
               case SDL_QUIT:
                   running = false;
                   flag = false;
                   break;
               case SDL_MOUSEBUTTONDOWN:
                   running = false;
                   flag = true;
                   break;
               default:
                   break;
           }
        }
    }
    return flag;
}
void displayMenu(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); //Set background to white
    SDL_RenderDrawRect(renderer, &playButton); //Draw playButton
    SDL_RenderDrawRect(renderer, &exitButton);
    SDL_RenderClear(renderer); //Clear renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);//Draw playerButton black
    SDL_RenderFillRect(renderer, &playButton);//Fill the playerButton
    SDL_SetRenderDrawColor(renderer, 12, 233, 123, 213);
    SDL_RenderFillRect(renderer, &exitButton);
    SDL_RenderPresent(renderer);
}
