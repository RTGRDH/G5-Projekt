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
#include <string.h>
#define PUBLIC
#define PRIVATE static
SDL_Rect gPlayButton;
SDL_Rect gExitButton;
SDL_Rect gMenuBackground;
SDL_Rect gCancelButton;
SDL_Rect gContinueButton;
SDL_Rect ipLabelRect;
SDL_Rect ipInputRect;

SDL_Surface *sPlayButton = NULL;
SDL_Surface *sExitButton = NULL;
SDL_Surface *sContinueButton = NULL;
SDL_Surface *sCancelButton = NULL;
SDL_Surface *sConnectionSceneSurface = NULL;
SDL_Surface *sMenuBackground = NULL;
SDL_Surface *sIpLabel = NULL;
SDL_Surface *sInputField = NULL;

SDL_Texture *mPlayButton = NULL;
SDL_Texture *mExitButton = NULL;
SDL_Texture *mContinueButton = NULL;
SDL_Texture *mCancelButton = NULL;
SDL_Texture *mConnectionSceneTexture = NULL;
SDL_Texture *mMenuBackground = NULL;
SDL_Texture *mIpLabel = NULL;
SDL_Texture *mInputField = NULL;

SDL_Window *connectWindow = NULL;
TTF_Font *font = NULL;
PRIVATE char inputText[30] = "";
SDL_Color fontColor={0,0,0}, backgroundColor={255,255,255};

/*
 Init method for menu such as buttons and background image.
 */
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
        SDL_FreeSurface(sPlayButton);
        SDL_FreeSurface(sExitButton);
        SDL_FreeSurface(sMenuBackground);
        //Background link:
       // http://blog.anytimefitness.com/develop-soccer-strength-like-world-cup-athlete/
    }
}
/*
 Eventhandler for the menu. Calls display menu-method and returns true if player wants to play.
 The flag is sent back to main.
 */
PUBLIC bool menu(SDL_Window *window, SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
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
                       if(!connectionScene(window,renderer, WINDOW_WIDTH, WINDOW_HEIGTH))
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
/*
 Displays menu content such as buttons and background.
 */
void displayMenu(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer); //Clear renderer
    SDL_RenderCopy(renderer, mMenuBackground, NULL, &gMenuBackground);
    SDL_RenderCopy(renderer, mPlayButton, NULL, &gPlayButton);
    SDL_RenderCopy(renderer, mExitButton, NULL, &gExitButton);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}
/*
 Clean up-method for menu init. Removes resources.
 */
void cleanUpInit()
{
    SDL_DestroyTexture(mPlayButton);
    SDL_DestroyTexture(mExitButton);
    SDL_DestroyTexture(mMenuBackground);
}
/*
 Clean up-method for connection scene. Removes resources.
 */
void cleanUpConnectionScene()
{
    SDL_DestroyTexture(mIpLabel);
    SDL_DestroyTexture(mInputField);
    SDL_DestroyTexture(mContinueButton);
    SDL_DestroyTexture(mCancelButton);
    SDL_DestroyTexture(mConnectionSceneTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}
/*
 Inits content to be shown such as buttons, texts and background image.
 */
void initConnectionScene(SDL_Renderer *renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    if(TTF_Init() == false)
    {
        printf("%s\n",TTF_GetError());
    }
    strcpy(inputText, "");
    font = TTF_OpenFont("font/arial.ttf", 25);
    gCancelButton.h = 100; gCancelButton.w = 300;
    gCancelButton.x = WINDOW_WIDTH/4-gCancelButton.w/2; gCancelButton.y = WINDOW_HEIGTH-130;
    gContinueButton.h = 100; gContinueButton.w = 300;
    gContinueButton.x = WINDOW_WIDTH/2+gContinueButton.w/4; gContinueButton.y = WINDOW_HEIGTH-130;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
    }
    else
    {
        sContinueButton = IMG_Load("images/Continue_Button.png");
        sCancelButton = IMG_Load("images/Cancel_Button.png");
        sConnectionSceneSurface = IMG_Load("images/Menu_Background.png");
        mContinueButton = SDL_CreateTextureFromSurface(renderer, sContinueButton);
        mCancelButton = SDL_CreateTextureFromSurface(renderer, sCancelButton);
        mConnectionSceneTexture = SDL_CreateTextureFromSurface(renderer, sConnectionSceneSurface);
        SDL_FreeSurface(sContinueButton);
        SDL_FreeSurface(sCancelButton);
        SDL_FreeSurface(sConnectionSceneSurface);
        //Background link:
       // http://blog.anytimefitness.com/develop-soccer-strength-like-world-cup-athlete/
    }
    char labelText[30] = "Enter IP-address:";
    printf("%s\n", labelText);
    printf("before TTF_RenderText_Shaded\n");
    if(!font)
        printf("no font");
    sIpLabel = TTF_RenderText_Shaded(font,labelText,fontColor,backgroundColor);
    printf("after TTF_RenderText_Shaded\n");
    
    mIpLabel = SDL_CreateTextureFromSurface(renderer, sIpLabel);
    
    SDL_QueryTexture(mIpLabel, NULL, NULL, &ipLabelRect.w, &ipLabelRect.h);
    
    ipLabelRect.h = ipLabelRect.h; ipLabelRect.w = ipLabelRect.w;
    ipLabelRect.x = WINDOW_WIDTH/2-ipLabelRect.w/2; ipLabelRect.y = WINDOW_HEIGTH/4;

    if(mInputField == NULL)
    {
        printf("Could not create Input field, SDL_ Error: %s\n",SDL_GetError());
    }
    if(mIpLabel == NULL)
    {
        printf("Could not create Label, SDL_ Error: %s\n",SDL_GetError());
    }
    SDL_FreeSurface(sIpLabel);
}
/*
 Method for init and updating IP-input field
 */
void updateText(SDL_Renderer* renderer,const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    //SDL_DestroyTexture(mInputField);
    sInputField =TTF_RenderText_Shaded(font,inputText,fontColor,backgroundColor);
    mInputField = SDL_CreateTextureFromSurface(renderer, sInputField);
    SDL_QueryTexture(mInputField, NULL, NULL, &ipInputRect.w, &ipInputRect.h);
    ipInputRect.h = ipInputRect.h; ipInputRect.w = ipInputRect.w;
    ipInputRect.x = WINDOW_WIDTH/2-ipInputRect.w/2; ipInputRect.y = WINDOW_HEIGTH/3;
    SDL_FreeSurface(sInputField);
}
/*
 Displays content on the connection scene.
 */
void DisplayConnectionScene(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, mConnectionSceneTexture, NULL, &gMenuBackground);
    SDL_RenderCopy(renderer, mContinueButton, NULL, &gContinueButton);
    SDL_RenderCopy(renderer, mCancelButton, NULL, &gCancelButton);
    SDL_RenderCopy(renderer, mIpLabel, NULL, &ipLabelRect);
    SDL_RenderCopy(renderer, mInputField, NULL, &ipInputRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}
/*
 Eventhandler that takes input to the connection scene. Also calls Display-method.
 */
bool connectionScene(SDL_Window *window, SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGTH)
{
    initConnectionScene(renderer,WINDOW_WIDTH, WINDOW_HEIGTH);
    SDL_Event event;
    int mouseX, mouseY;
    bool running = true;
    bool flag = false;
    int ipLength = 0;
    while(running)
    {
        SDL_GetMouseState(&mouseX, &mouseY);
        while(SDL_PollEvent(&event))
        {
            updateText(renderer, WINDOW_WIDTH, WINDOW_HEIGTH);
            DisplayConnectionScene(renderer);
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    flag = false;
                    cleanUpConnectionScene();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(mouseX >= gCancelButton.x && mouseX <= gCancelButton.x+gCancelButton.w && mouseY >= gCancelButton.y && mouseY <= gCancelButton.y + gCancelButton.h)//Checks if mouse pointer coordination is within the button
                    {
                        running = false;
                        flag = false;
                        cleanUpConnectionScene();
                    }
                    if(mouseX >= gContinueButton.x && mouseX <= gContinueButton.x+gContinueButton.w && mouseY >= gContinueButton.y && mouseY <= gContinueButton.y + gContinueButton.h)//Checks if mouse pointer coordination is within the button
                    {
                        running = false;
                        flag = true;
                        cleanUpConnectionScene();
                    }
                    break;
                    case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_0:inputText[ipLength]='0'; ipLength++;
                            break;
                        case SDL_SCANCODE_1:inputText[ipLength]='1'; ipLength++;
                            break;
                        case SDL_SCANCODE_2:inputText[ipLength]='2'; ipLength++;
                            break;
                        case SDL_SCANCODE_3:inputText[ipLength]='3'; ipLength++;
                            break;
                        case SDL_SCANCODE_4:inputText[ipLength]='4'; ipLength++;
                            break;
                        case SDL_SCANCODE_5:inputText[ipLength]='5'; ipLength++;
                            break;
                        case SDL_SCANCODE_6:inputText[ipLength]='6'; ipLength++;
                            break;
                        case SDL_SCANCODE_7:inputText[ipLength]='7'; ipLength++;
                            break;
                        case SDL_SCANCODE_8:inputText[ipLength]='8'; ipLength++;
                            break;
                        case SDL_SCANCODE_9:inputText[ipLength]='9'; ipLength++;
                            break;
                        case SDL_SCANCODE_BACKSPACE:
                            if(ipLength != 0 && ipLength < 31)
                            {
                                inputText[ipLength]=' ';
                                inputText[ipLength-1]=' ';
                                ipLength--;
                            }
                            break;
                        case SDL_SCANCODE_PERIOD:
                            inputText[ipLength]='.';
                            ipLength++;
                            break;
                        case SDL_SCANCODE_RETURN:
                            break;

                        default:
                            break;
                    }

                    if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                    {
                        strcpy(inputText,SDL_GetClipboardText());
                        ipLength = strlen(inputText);
                    }
                    default:
                    break;
                }
                        
        }
    }
        return flag;
}

PUBLIC const char *getIP()
{
    static char IP[30];
    strcpy(IP, inputText);
    return IP;
}
