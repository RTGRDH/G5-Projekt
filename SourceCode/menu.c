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
#include <SDL2/SDL_image.h>
SDL_Rect gPlayButton;
SDL_Rect gExitButton;
SDL_Rect gMenuBackground;
SDL_Surface *sPlayButton = NULL;
SDL_Surface *sExitButton = NULL;
SDL_Surface *sMenuBackground = NULL;

SDL_Texture *mPlayButton = NULL;
SDL_Texture *mExitButton = NULL;
SDL_Texture *mMenuBackground = NULL;

SDL_Window* connectWindow = NULL;
void initMenu(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    gPlayButton.h = 100; gPlayButton.w = 300;
    gPlayButton.x = WINDOW_WIDTH/2-gPlayButton.w/2; gPlayButton.y = WINDOW_HEIGTH/2-130;
    
    gExitButton.h = 100; gExitButton.w = 300;
    gExitButton.x = WINDOW_WIDTH/2-gExitButton.w/2; gExitButton.y = WINDOW_HEIGTH/2;
    
    gMenuBackground.h = WINDOW_HEIGTH; gMenuBackground.w = WINDOW_WIDTH;
    gMenuBackground.x = 0; gMenuBackground.y = 0;
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
    }
    else
    {
        sPlayButton = IMG_Load("images/Play_Button.png");
        sExitButton = IMG_Load("images/Exit_Button.png");
        sMenuBackground = IMG_Load("images/Menu_Background.png");
        mPlayButton = SDL_CreateTextureFromSurface(renderer, sPlayButton);
        mExitButton = SDL_CreateTextureFromSurface(renderer, sExitButton);
        mMenuBackground = SDL_CreateTextureFromSurface(renderer, sMenuBackground);
        //Background link:
       // http://blog.anytimefitness.com/develop-soccer-strength-like-world-cup-athlete/
    }
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
                   cleanUpInit();
                   flag = false;
                   break;
               case SDL_MOUSEBUTTONDOWN:
                   if(mouseX >= gPlayButton.x && mouseX <= gPlayButton.x+gPlayButton.w && mouseY >= gPlayButton.y && mouseY <= gPlayButton.y + gPlayButton.h)//Checks if mouse pointer coordination is within the button
                   {
                       running = false;
                       cleanUpInit();
                       connection();
                       flag = true;
                   }
                   else if(mouseX >= gExitButton.x && mouseX <= gExitButton.x+gPlayButton.w && mouseY >= gExitButton.y && mouseY <= gExitButton.y + gExitButton.h) //Checks if mouse pointer coordination is within the button
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
    SDL_RenderClear(renderer); //Clear renderer
    SDL_RenderCopy(renderer, mMenuBackground, NULL, &gMenuBackground);
    SDL_RenderCopy(renderer, mPlayButton, NULL, &gPlayButton);
    SDL_RenderCopy(renderer, mExitButton, NULL, &gExitButton);
    SDL_RenderPresent(renderer);
}

void cleanUpInit()
{
    SDL_FreeSurface(sPlayButton);
    SDL_FreeSurface(sExitButton);
    SDL_FreeSurface(sMenuBackground);
    mPlayButton = NULL;
    mExitButton = NULL;
    sMenuBackground = NULL;
    SDL_DestroyTexture(mPlayButton);
    SDL_DestroyTexture(mExitButton);
    SDL_DestroyTexture(mMenuBackground);
}
void connection()
{
    /*
    connectWindow = SDL_CreateWindow("Under production", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 500, 200, SDL_WINDOW_SHOWN);
    if(connectWindow == NULL)
    {
        printf("Could not create window. Error: %s ",SDL_GetError());
        printf("\n");
    }
    temp();
     */
}

void temp()
{
    /*
    SDL_Event event;
     bool running = true;
     while(running)
     {
        //SDL_GetMouseState(&mouseX, &mouseY);
        //displayMenu(renderer);
        while (SDL_PollEvent(&event))
        {
           switch (event.type)
           {
               case SDL_QUIT:
                   printf("Testtttt");
                   running = false;
                   break;
           }
        }
    }
     */
}
