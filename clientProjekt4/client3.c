#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>                                      
#include <string.h>                                         
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
 #include <SDL2/SDL_ttf.h>
#include "Player.h"
#include <math.h>
#include "ball.h"
#include "gameLogic.h"
#include "menu.h"
#include <SDL2/SDL_mixer.h>

#define SIZE 4
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 6
#define TURNING_SPEED 10
#define ACCELERATION 0.2

const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;

bool init();
void renderBackground();
bool initPlayField();
bool initMedia();
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

Player player = NULL;
Player player2 = NULL;
Player player3 = NULL;
Player player4 = NULL;
Ball b = NULL;
SDL_Rect gField;

// struct to hold the position and size of the sprite
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


int main(int argc, char * argv[])
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *backgroundSound = Mix_LoadMUS("backgroundSound.wav");

    UDPsocket s;                                                
	IPaddress saddr;                                            
	UDPpacket *pSend;                                           
    UDPpacket *pRecive;                                        
    bool running = true;
    int P1Score = 0;
    int P2Score = 0;
    int loop=0;
    const int FPS = 30;
    Uint32 startTime;
    char inputText[40] = "";
    

    //Check if SDL_net is initialized, Jonas Willén movingTwoMenWithUDP.c 
    if (SDLNet_Init() < 0)            
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Check if random port is open,  Jonas Willén movingTwoMenWithUDP.c     
    if (!(s = SDLNet_UDP_Open(0)))                                          
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Resolve servername, Jonas Willén movingTwoMenWithUDP.c      
	if (SDLNet_ResolveHost(&saddr, getIP(), 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s : 2000): %s\n",getIP() ,SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Check if it's possible to allocate memory for send and recive packetsJonas Willén movingTwoMenWithUDP.c //Net
    if (!((pSend = SDLNet_AllocPacket(512))&& (pRecive = SDLNet_AllocPacket(512)))) 
	{                                                                               
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    
    if(init())
    {
        printf("Initialize window and renderer successful.\n");
          if(!menu(window, renderer, WINDOW_WIDTH, WINDOW_HEIGTH))
        {
            running = false;
        }
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
    if(!initMedia())
    {
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


    /**
    While loop checking if an event occured.
     Code taken from Jonas Willén, SDL_net.zip
     */
     
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
            movementCodedInOneVariable = 5;
        if (accelerate == 0 && turn == -1)
            movementCodedInOneVariable = 6;
        if (accelerate == -1 && turn == 1)
            movementCodedInOneVariable = 7;
        if (accelerate == -1 && turn == 0)
            movementCodedInOneVariable = 8;
        if (accelerate == -1 && turn == -1)
            movementCodedInOneVariable = 9;
        if(movementCodedInOneVariable!=5){
            sendPacket(player, movementCodedInOneVariable, saddr, pSend, s ); 
        }
//------------------------------------------------------FORWARD LOGICAL OBJECTS TO GRAPHICAL OBJECTS--------------------------------------------------------------------------
        //Recive packet, for now just recive mirroring from server                  //Net
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

        sprintf(inputText,"%d-%d",P1Score,P2Score);

        if(P1Score==3)
        {
            sprintf(inputText,"Blaa laget vann.Grattis!");
        }
        else if(P2Score==3)
        {
            sprintf(inputText,"Oranga laget vann.Grattis!");
        }

        fontClient = TTF_OpenFont("Images/arial.ttf", 40);

        surface = TTF_RenderText_Solid(fontClient,
        inputText, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
    
        dstrect.h = dstrect.h; 
        dstrect.w = dstrect.w;
        dstrect.x = 50;
        dstrect.y = 5;

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
        
        if(1000/FPS>SDL_GetTicks()-startTime){
            SDL_Delay(1000/FPS-(SDL_GetTicks()-startTime));
        }
    }

    Mix_FreeMusic(backgroundSound);
    Quit();
    return 0;
}

//------------------------------------------------------FUNCTIONS: INITIALIZING WINDOW, SURFACE, RENDERER & GRAPHICAL OBJECTS--------------------------------------------------------------------------
/**
 Init other media
 */
bool initMedia()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    sPlayer = IMG_Load("images/Car.png");
    sPlayer2 = IMG_Load("images/Car2.png");
    sPlayer3 = IMG_Load("images/Car3.png");
    sPlayer4 = IMG_Load("images/Car4.png");
    sBall = IMG_Load("images/SoccerBall.png");
     if(NULL == imageSurface)
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

    player = createPlayer(50, 50);                                                                        
    player2 = createPlayer(880, 50);
    player3 = createPlayer(50, 450);                                                                        
    player4 = createPlayer(880, 450);

    b = createBall(470,260);

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
    if(NULL == imageSurface)
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
 Init code taken from Jonas Willén.
 Creates window and a renderer.
 Gets windowSurface from window to present background (play field).
 Returns false if init failed.
 */
bool init()
{
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
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
    SDL_FreeSurface(sPlayer);
    SDL_FreeSurface(sPlayer2);
    SDL_FreeSurface(sPlayer3);
    SDL_FreeSurface(sPlayer4);
    SDL_FreeSurface(sBall);
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sGoal_Left);
    SDL_FreeSurface(sGoal_Right);
    Mix_Quit();
    free(player);
    free(player2);
    free(player3);
    free(player4);
    free(b);
    TTF_Quit();
    SDLNet_Quit();
    IMG_Quit();
    SDL_Quit();

}