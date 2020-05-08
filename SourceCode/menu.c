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
#include <SDL2/SDL_ttf.h>
SDL_Rect gPlayButton;
SDL_Rect gExitButton;
SDL_Rect gMenuBackground;
SDL_Rect gCancelButton;
SDL_Rect gContinueButton;
SDL_Rect ipLabel;

SDL_Surface *sPlayButton = NULL;
SDL_Surface *sExitButton = NULL;
SDL_Surface *sMenuBackground = NULL;
SDL_Surface *sInputLabel = NULL;

SDL_Texture *mPlayButton = NULL;
SDL_Texture *mExitButton = NULL;
SDL_Texture *mMenuBackground = NULL;
SDL_Texture *mIpLabel = NULL;

SDL_Window *connectWindow = NULL;
TTF_Font *font = NULL;

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
                       //flag = temp(renderer, WINDOW_WIDTH, WINDOW_HEIGTH);
                       if(!connectionScene(renderer, WINDOW_WIDTH, WINDOW_HEIGTH))
                       {
                           initMenu(renderer, WINDOW_WIDTH, WINDOW_HEIGTH);
                           running = true;
                           flag = false;
                       }
                       else
                       {
                           running = false;
                           flag = true;
                       }
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
    SDL_RenderClear(renderer); //Clear renderer
    SDL_RenderCopy(renderer, mMenuBackground, NULL, &gMenuBackground);
    SDL_RenderCopy(renderer, mPlayButton, NULL, &gPlayButton);
    SDL_RenderCopy(renderer, mExitButton, NULL, &gExitButton);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
    TTF_Quit();
}
void initConnectionScene(SDL_Renderer *renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    TTF_Init();
    font = TTF_OpenFont("arial.ttf", 25);
    SDL_Color color={0,0,0}, bgcolor={255,255,255};
    gCancelButton.h = 100; gCancelButton.w = 300;
    gCancelButton.x = WINDOW_WIDTH/4-gCancelButton.w/2; gCancelButton.y = WINDOW_HEIGTH-130;
    gContinueButton.h = 100; gContinueButton.w = 300;
    gContinueButton.x = WINDOW_WIDTH/2+gContinueButton.w/4; gContinueButton.y = WINDOW_HEIGTH-130;
    
    ipLabel.h = 100; ipLabel.w = 300;
    ipLabel.x = WINDOW_WIDTH/2-ipLabel.w/2; ipLabel.y = WINDOW_HEIGTH/2;
    char inputtext[30] = "Enter IP-address:";
    sInputLabel=TTF_RenderText_Shaded(font,inputtext,color,bgcolor);

    mIpLabel=SDL_CreateTextureFromSurface(renderer, sInputLabel);
}
void DisplayConnectionScene(SDL_Renderer* renderer)
{
    
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer, &gCancelButton);
    SDL_RenderDrawRect(renderer, &gCancelButton);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    SDL_RenderFillRect(renderer, &gContinueButton);
    SDL_RenderDrawRect(renderer, &gContinueButton);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(renderer, mIpLabel, NULL, &ipLabel);
    SDL_RenderPresent(renderer);
}

bool connectionScene(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    initConnectionScene(renderer,WINDOW_WIDTH, WINDOW_HEIGTH);
    SDL_Event event;
    int mouseX, mouseY;
    bool running = true;
    bool flag = false;
    while(running)
    {
        SDL_GetMouseState(&mouseX, &mouseY);
        while(SDL_PollEvent(&event))
        {
            DisplayConnectionScene(renderer);
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    flag = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(mouseX >= gCancelButton.x && mouseX <= gCancelButton.x+gCancelButton.w && mouseY >= gCancelButton.y && mouseY <= gCancelButton.y + gCancelButton.h)//Checks if mouse pointer coordination is within the button
                    {
                        running = false;
                        flag = false;
                        //menu(renderer, WINDOW_WIDTH, WINDOW_HEIGTH);
                    }
                    if(mouseX >= gContinueButton.x && mouseX <= gContinueButton.x+gContinueButton.w && mouseY >= gContinueButton.y && mouseY <= gContinueButton.y + gContinueButton.h)//Checks if mouse pointer coordination is within the button
                    {
                        //if(inmatningen är rätt och server finns)
                        running = false;
                        flag = true;
                    }
                default:
                    break;
            }
        }
        
    }
    return flag;
}
