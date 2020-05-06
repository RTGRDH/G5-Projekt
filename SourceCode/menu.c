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
SDL_Surface *sPlayButton = NULL;
SDL_Surface *sExitButton = NULL;
SDL_Surface *backgroundSurface = NULL;

SDL_Texture *mPlayButton = NULL;
SDL_Texture *mExitButton = NULL;
SDL_Texture *mMenuBackground = NULL;
void initMenu(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    playButton.h = 100; playButton.w = 300;
    playButton.x = WINDOW_WIDTH/2-playButton.w/2; playButton.y = WINDOW_HEIGTH/2-130;
    
    exitButton.h = 100; exitButton.w = 300;
    exitButton.x = WINDOW_WIDTH/2-exitButton.w/2; exitButton.y = WINDOW_HEIGTH/2;
    //menu(renderer);
}
bool menu(SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    initMenu(renderer, WINDOW_WIDTH, WINDOW_HEIGTH);
    SDL_Event event;
    int mouseX, mouseY;
    bool running = true;
    bool flag = false;
    while(running)
    {
        SDL_GetMouseState(&mouseX, &mouseY);
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
                   if(mouseX >= playButton.x && mouseX <= playButton.x+playButton.w && mouseY >= playButton.y && mouseY <= playButton.y + playButton.h)//Checks if mouse pointer coordination is within the button
                   {
                       running = false;
                       cleanUpInit();
                       flag = true;
                   }
                   else if(mouseX >= exitButton.x && mouseX <= exitButton.x+playButton.w && mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h) //Checks if mouse pointer coordination is within the button
                   {
                       running = false;
                       cleanUpInit();
                       flag = false;
                   }
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
    SDL_SetRenderDrawColor(renderer, 12, 233, 123, 213);//Draw playerButton green
    SDL_RenderFillRect(renderer, &playButton);//Fill the playerButton
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF); //Draw the exit button black
    SDL_RenderFillRect(renderer, &exitButton);
    SDL_RenderPresent(renderer);
}

void cleanUpInit()
{
    SDL_FreeSurface(sPlayButton);
    SDL_FreeSurface(sExitButton);
    SDL_FreeSurface(backgroundSurface);
    mPlayButton = NULL;
    mExitButton = NULL;
    backgroundSurface = NULL;
    SDL_DestroyTexture(mPlayButton);
    SDL_DestroyTexture(mExitButton);
    SDL_DestroyTexture(mMenuBackground);
}

