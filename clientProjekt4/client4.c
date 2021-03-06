#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>                                      
#include <string.h>                                         
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Player.h"
#include "ball.h"
#include "menu.h"
#include "gameLogic.h"

#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 6
#define TURNING_SPEED 10
#define ACCELERATION 0.2

const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;

bool init();
void renderBackground();
bool initPlayField();
bool initMedia(Player player, Player player2, Player player3, Player player4, Ball b);
void sendPacket(Player p,  int movement, IPaddress svr, UDPpacket *packet, UDPsocket s);
void Quit();   


SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Surface *imageSurface =  NULL;
SDL_Surface *sPlayer = NULL;
SDL_Surface *sPlayer2 = NULL;
SDL_Surface *sPlayer3 = NULL;
SDL_Surface *sPlayer4 = NULL;
SDL_Surface *sBall = NULL;
SDL_Surface *sGoal_Left = NULL;
SDL_Surface *sGoal_Right = NULL;
SDL_Surface *surface = NULL;

SDL_Texture *mField;
SDL_Texture *mBall = NULL;
SDL_Texture *mPlayer = NULL;
SDL_Texture *mPlayer2 = NULL;
SDL_Texture *mPlayer3 = NULL;
SDL_Texture *mPlayer4 = NULL;
SDL_Texture *mGoal_Left = NULL;
SDL_Texture *mGoal_Right = NULL;
SDL_Texture *texture = NULL;

SDL_Rect gField;
SDL_Rect gPlayer;
SDL_Rect gPlayer2;
SDL_Rect gPlayer3;
SDL_Rect gPlayer4;
SDL_Rect gBall;
SDL_Rect gGoal_Left;
SDL_Rect gGoal_Right;

TTF_Font *fontClient = NULL;
SDL_Rect dstrect;
SDL_Color color = {0,0,0};
char inputText[40] = "";

int main(int argc, char * argv[])
{
    Player player = NULL;
    Player player2 = NULL;
    Player player3 = NULL;
    Player player4 = NULL;
    Ball b = NULL;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *backgroundSound = Mix_LoadMUS("Sounds/backgroundSound.wav");
    //https://opengameart.org/

    UDPsocket s;                                                
	IPaddress saddr;                                            
	UDPpacket *pSend;                                           
    UDPpacket *pRecive;                                        
    bool running = true;
    int P1Score = 0;
    int P2Score = 0;
    const int FPS = 30;
    Uint32 startTime;
    //Init SDL and start menu
    if(init())
    {
        printf("Initialize window and renderer successful.\n");
          if(!menu(renderer, WINDOW_WIDTH, WINDOW_HEIGTH))
        {
            running = false;
            Quit();
        }
    }
    //Check if SDL_net is initialized. Inspired by Jonas Willén movingTwoMenWithUDP.c 
    if (SDLNet_Init() < 0)            
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Check if random port is open. Inspired by Jonas Willén movingTwoMenWithUDP.c     
    if (!(s = SDLNet_UDP_Open(0)))                                          
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Resolve servername. Inspired by Jonas Willén movingTwoMenWithUDP.c      
	if (SDLNet_ResolveHost(&saddr, getIP(), 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s : 2000): %s\n",getIP() ,SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Check if it's possible to allocate memory for send and recive packets. Inspired by Jonas Willén movingTwoMenWithUDP.c //Net
    if (!((pSend = SDLNet_AllocPacket(512))&& (pRecive = SDLNet_AllocPacket(512)))) 
	{                                                                               
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    //Init backround here
    if(!initPlayField())
    {
        Quit();
        return 1;
    }
    else
    {
        printf("Initialize playfield successful.\n");
    }
    //Init other graphical media.
    player = createPlayer(50, 50);                                                                        
    player2 = createPlayer(880, 50);
    player3 = createPlayer(50, 450);                                                                        
    player4 = createPlayer(880, 450);
    b = createBall(470,260);
    if(!initMedia(player, player2, player3, player4, b))
    {
        free(player);
        free(player2);
        free(player3);
        free(player4);
        free(b);
        Quit();
        return 1;
    }
    else
    {
        printf("Initialize media successful.\n");
    }
                                                              
    setPlayerDirection(player, 45);                                              
    setPlayerDirection(player2, 135);                                            
    setPlayerDirection(player3, 315);                                            
    setPlayerDirection(player4, 225);

    // keep track of which inputs are given
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool musicStart = false;
    bool musicStop = false;
    bool musicPlaying = false;

//------------------------------------------------------SCAN KEYBOARD--------------------------------------------------------------------------
    while(running)
    {
        startTime = SDL_GetTicks();

        //Audio control - Only start music if not already playing.
        if(musicStart == true && musicPlaying == false)
        {
            Mix_PlayMusic(backgroundSound, -1);
            musicPlaying = true;
        }
        
        if(musicStop == true && musicPlaying == true)
        {
            Mix_HaltMusic();
            musicPlaying = false;
        }


        //While loop checking if an event occured.
        //Code inspired by Jonas Willén, SDL_net.zip
     
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = true;
                        break;

                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = true;
                        break;

                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = true;
                        break;

                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = true;
                        break;
                    case SDL_SCANCODE_M:
                        musicStart = true;
                        break;
                    case SDL_SCANCODE_N:
                        musicStop = true;
                        break;

                   default:
                       break;
                }
                break;
                case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = false;
                        break;

                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = false;
                        break;

                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = false;
                        break;

                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = false;
                        break;

                    
                    case SDL_SCANCODE_M:
                        musicStart = false;
                        break;
                    case SDL_SCANCODE_N:
                        musicStop = false;
                        break;

                    default:
                        break;
                }
                break;
            }
            
           
        }
        int turn, accelerate;
        //logic for handling conflicting input
        if (up && !down)
            accelerate = 1;
        if (up && down || !up && !down)
            accelerate = 0;
        if (!up && down)
            accelerate = -1;

        if(left && !right)
            turn = 1;
        if(left && right || !left && !right)
            turn = 0;
        if (!left && right)
            turn = -1;
        //logic for coding two three-choice variables into one nine-choice variable
        int movementCodedInOneVariable;
        if (accelerate == 1 && turn == 1)
            movementCodedInOneVariable = 1;
        if (accelerate == 1 && turn == 0)
            movementCodedInOneVariable = 2;
        if (accelerate == 1 && turn == -1)
            movementCodedInOneVariable = 3;
        if (accelerate == 0 && turn == 1)
            movementCodedInOneVariable =4;
        if (accelerate == 0 && turn == 0)
            movementCodedInOneVariable = 5;     //5 is in the middle row in the middle column, so do nothing
        if (accelerate == 0 && turn == -1)
            movementCodedInOneVariable = 6;
        if (accelerate == -1 && turn == 1)
            movementCodedInOneVariable = 7;
        if (accelerate == -1 && turn == 0)
            movementCodedInOneVariable = 8;
        if (accelerate == -1 && turn == -1)
            movementCodedInOneVariable = 9;
        if(movementCodedInOneVariable!=5){      //if movement variable is not 5, send packet
            sendPacket(player, movementCodedInOneVariable, saddr, pSend, s );
        }

        //Recive packet, put incoming data in graphic variables to visualize players and balls new location and update scoreboard
        while (SDLNet_UDP_Recv(s, pRecive)){
            float x1, y1, d1, x2, y2, d2, x3, y3, d3, x4,y4,d4, ballx, bally;
            float P1Speed,P2Speed,P3Speed,P4Speed;
            sscanf((char * )pRecive->data, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d",&x1, &y1, &d1, &x2, &y2, &d2, &x3, &y3, &d3, &x4, &y4, &d4, &ballx, &bally,&P1Speed,&P2Speed,&P3Speed,&P4Speed,&P1Score,&P2Score);

            gBall.x=ballx;
            gBall.y=bally;

            gPlayer.y = y1;
            gPlayer.x = x1;
            setPlayerDirection(player, d1);
            setPlayerSpeed(player,P1Speed);

            gPlayer2.y = y2;
            gPlayer2.x = x2;
            setPlayerDirection(player2, d2);
            setPlayerSpeed(player2,P2Speed);

            gPlayer3.y = y3;
            gPlayer3.x = x3;
            setPlayerDirection(player3, d3);
            setPlayerSpeed(player3,P3Speed);
            
            gPlayer4.y = y4;
            gPlayer4.x = x4;
            setPlayerDirection(player4, d4);
            setPlayerSpeed(player4,P4Speed);
        }
        //print gametext string
        sprintf(inputText,"%d-%d",P1Score,P2Score);
        if(P1Score==3)
        {
            sprintf(inputText,"Blue team wins.");
            running = false;
        }
        else if(P2Score==3)
        {
            sprintf(inputText,"Orange team wins.");
            running = false;
        }

        //render gametext string
        SDL_DestroyTexture(texture);
        surface = TTF_RenderText_Solid(fontClient,inputText, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
        dstrect.h = dstrect.h;
        dstrect.w = dstrect.w;
        dstrect.x = 50;
        dstrect.y = 5;
        //render game
        SDL_RenderClear(renderer);
        renderBackground();
     
        SDL_RenderCopy(renderer,mBall,NULL,&gBall);
        SDL_RenderCopyEx(renderer, mPlayer, NULL, &gPlayer, getPlayerDirection(player)-90, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, mPlayer2, NULL, &gPlayer2, getPlayerDirection(player2)-90, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, mPlayer3, NULL, &gPlayer3, getPlayerDirection(player3)-90, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, mPlayer4, NULL, &gPlayer4, getPlayerDirection(player4)-90, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
        
        //control FPS
        if(1000/FPS>SDL_GetTicks()-startTime){
            SDL_Delay(1000/FPS-(SDL_GetTicks()-startTime));
        }
        if(running == false) //Delay to show results and then shut down.
        {
            SDL_Delay(5000);
        }
    }
    //free resources
    Mix_FreeMusic(backgroundSound);
    free(player);
    free(player2);
    free(player3);
    free(player4);
    free(b);
    Quit();
    return 0;
}

//------------------------------------------------------FUNCTIONS: INITIALIZING WINDOW, SURFACE, RENDERER & GRAPHICAL OBJECTS--------------------------------------------------------------------------
/**
 Init other media
 */
bool initMedia(Player player, Player player2, Player player3, Player player4, Ball b)
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    sPlayer = IMG_Load("images/Car.png"); //Källa: https://ya-webdesign.com/explore/race-car-sprite-png/
    sPlayer2 = IMG_Load("images/Car2.png"); //Källa: https://ya-webdesign.com/explore/race-car-sprite-png/
    sPlayer3 = IMG_Load("images/Car3.png"); //Källa: https://www.1001freedownloads.com/free-clipart/pink-racing-car-top-view
    sPlayer4 = IMG_Load("images/Car4.png"); //Källa: https://www.clipart.email/clipart/car-png-clipart-game-297399.html
    sBall = IMG_Load("images/SoccerBall.png");
     if(imageSurface == NULL)
     {
         printf("\nCould not load image. Error: %s",SDL_GetError());
         printf("\n");
         flag = false;
     }
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, sPlayer);
    mPlayer2 = SDL_CreateTextureFromSurface(renderer,sPlayer2);
    mPlayer3 = SDL_CreateTextureFromSurface(renderer, sPlayer3);
    mPlayer4 = SDL_CreateTextureFromSurface(renderer,sPlayer4);
    mBall = SDL_CreateTextureFromSurface(renderer,sBall);
    
    SDL_FreeSurface(sPlayer);
    SDL_FreeSurface(sPlayer2);
    SDL_FreeSurface(sPlayer3);
    SDL_FreeSurface(sPlayer4);
    SDL_FreeSurface(sBall);

    gPlayer.x = getPlayerPositionX(player);
    gPlayer.y = getPlayerPositionY(player);
    gPlayer.h = getPlayerHeight();
    gPlayer.w = getPlayerWidth();

    gPlayer2.x = getPlayerPositionX(player2);
    gPlayer2.y = getPlayerPositionY(player2);
    gPlayer2.h = getPlayerHeight();
    gPlayer2.w = getPlayerWidth();

    gPlayer3.x = getPlayerPositionX(player3);
    gPlayer3.y = getPlayerPositionY(player3);
    gPlayer3.h = getPlayerHeight();
    gPlayer3.w = getPlayerWidth();

    gPlayer4.x = getPlayerPositionX(player4);
    gPlayer4.y = getPlayerPositionY(player4);
    gPlayer4.h = getPlayerHeight();
    gPlayer4.w = getPlayerWidth();

    gBall.x = getBallPositionX(b);
    gBall.y = getBallPositionY(b);
    gBall.h = getBallHeight();
    gBall.w = getBallWidth();
    
    fontClient = TTF_OpenFont("Images/arial.ttf", 40);
    return flag;
}
/**
 Init background football field.
 Returns false if image could not be loaded or if SDL_Image could not be initialized.
 */
bool initPlayField()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    imageSurface = IMG_Load("images/SoccerField.png");
    sGoal_Left = IMG_Load("images/Goal_Left.png");
    sGoal_Right = IMG_Load("images/Goal_Right.png");
    
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);
    mGoal_Left = SDL_CreateTextureFromSurface(renderer, sGoal_Left);
    mGoal_Right = SDL_CreateTextureFromSurface(renderer, sGoal_Right);
    
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(sGoal_Left);
    SDL_FreeSurface(sGoal_Right);

    //Left goal rect init position and define width and heigth
    gGoal_Left.h=370;
    gGoal_Left.w = 50;
    gGoal_Left.x = 0;
    gGoal_Left.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;
    
    //Right goal rect init position and define width and heigth
    gGoal_Right.h=370;
    gGoal_Right.w = 50;
    gGoal_Right.x = WINDOW_WIDTH-gGoal_Right.w;
    gGoal_Right.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;

    gField.x = 0; gField.y = 0; gField.h = WINDOW_HEIGTH; gField.w = WINDOW_WIDTH;
    if(imageSurface == NULL)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    return flag;
}
/**
 Rendering the background, AKA. Football field and goals.
 */
void renderBackground()
{
    SDL_RenderCopy(renderer, mField, NULL, &gField);
    SDL_RenderCopy(renderer,mGoal_Left, NULL, &gGoal_Left);
    SDL_RenderCopy(renderer, mGoal_Right, NULL, &gGoal_Right);
}

/**
 Init code inspired by Jonas Willén.
 Creates window and a renderer.
 Gets windowSurface from window to present background (play field).
 Returns false if init failed.
 */
bool init()
{
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    window = SDL_CreateWindow("Not Rocket_League", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_SHOWN);
      
    if(window == NULL)
    {
        printf("Could not create window. Error: %s ",SDL_GetError());
        printf("\n");
        test = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Could not create renderer. Error: %s",SDL_GetError());
        printf("\n");
        test = false;
    }
    return test;
}

void sendPacket(Player p, int movement, IPaddress svr, UDPpacket *packet, UDPsocket s)
{
    sprintf((char *)packet->data, "%d\n",movement); 
    packet->address.host = svr.host;	/* Set the destination host */
	packet->address.port = svr.port;	/* And destination port */
	packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(s, -1, packet);
}

void Quit()
{
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mField);
    SDL_DestroyTexture(mPlayer);
    SDL_DestroyTexture(mPlayer2);
    SDL_DestroyTexture(mPlayer3);
    SDL_DestroyTexture(mPlayer4);
    SDL_DestroyTexture(mBall);
    SDL_DestroyTexture(mGoal_Left);
    SDL_DestroyTexture(mGoal_Right);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    TTF_Quit();
    SDLNet_Quit();
    IMG_Quit();
    SDL_Quit();

}
