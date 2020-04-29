#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>                                         //Netw
#include <string.h>                                         //Net
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
 //#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include <math.h>
#include "ball.h"
#include "gameLogic.h"
#include "menu.h"


const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
bool init();
void renderBackground();
bool initPlayField();
bool initMedia();

bool ballRightGoalCollision(SDL_Rect* gBall);
bool ballLeftGoalCollision(SDL_Rect* gBall);

bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall);

void speedLimit(Player p);
void colissionDetectionPlayerArena(Player p);
void colissionDetectionBallArena(Ball b);
float xInvertDirection(float direction);
float yInvertDirection(float direction);
float angleBallPlayer(Ball b, Player p);
float distanceBallPlayer(Ball b, Player p);

void sendPacket(int movement, IPaddress svr, UDPpacket *packet, UDPsocket s);            //Net

SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface *imageSurface =  NULL;
SDL_Surface *sPlayer = NULL;
SDL_Surface *sPlayer2 = NULL;
SDL_Surface *sBall = NULL;
SDL_Surface *sGoal_Left = NULL;
SDL_Surface *sGoal_Right = NULL;
SDL_Surface *surface = NULL;

SDL_Texture *mField;
SDL_Texture *mBall = NULL;
SDL_Texture *mPlayer = NULL;
SDL_Texture *mPlayer2 = NULL;
SDL_Texture *mGoal_Left = NULL;
SDL_Texture *mGoal_Right = NULL;
SDL_Texture *texture = NULL;

Player player = NULL;
Player player2 = NULL;
Ball b = NULL;
SDL_Rect gField;
// struct to hold the position and size of the sprite
SDL_Rect gPlayer;
SDL_Rect gPlayer2;
SDL_Rect gBall;
SDL_Rect gGoal_Left;
SDL_Rect gGoal_Right;
//SDL_Rect dstrect;

//TTF_Font * font = NULL;


#define SPEED (75); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 10
#define ACCELERATION 0.1

int main(int argc, char * argv[])
{
    /**
     Implement into player object?
     Not done yet.
     */

    UDPsocket s;                                                //Net
	IPaddress saddr;                                            //Net
	UDPpacket *pSend;                                           //Net
    UDPpacket *pRecive;                                         //Net
    bool running = true;
    int P1Score = 0;
    int P2Score = 0;

    //Check if SDL_net is initialized, Jonas Willén movingTwoMenWithUDP.c    //Net
    if (SDLNet_Init() < 0)            
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Check if random port is open,  Jonas Willén movingTwoMenWithUDP.c     //Net
    if (!(s = SDLNet_UDP_Open(0)))      //Net
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    //Resolve servername, Jonas Willén movingTwoMenWithUDP.c                //Net
	if (SDLNet_ResolveHost(&saddr, "127.0.0.1", 2000) == -1) 
	{
		fprintf(stderr, "SDLNet_ResolveHost(127.0.0.1 2000): %s\n", SDLNet_GetError());
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
        //Init menu
        initMenu(renderer);
    }
    
    //Init backround here
    if(!initPlayField())
    {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize playfield successful.\n");
    }
    //Init other graphical media.
    if(!initMedia())
    {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize media successful.\n");
    }
    
    setPlayerPositionX(player, 0);
    setPlayerDirection(player, 90);
    setPlayerPositionY(player, (WINDOW_HEIGTH - gPlayer.h) / 2);

    setPlayerPositionX(player2, 800);
    setPlayerDirection(player2, -90);
    setPlayerPositionY(player2, (WINDOW_HEIGTH - gPlayer.h) / 2);


    float x_pos = getPlayerPositionX(player);
    float y_pos = getPlayerPositionY(player);

    float ball_xPos = 470;
    float ball_yPos = 260;
 
    // keep track of which inputs are given
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
        
    bool upP2 = false;
    bool downP2 = false;
    bool leftP2 = false;
    bool rightP2 = false;
//------------------------------------------------------SCAN KEYBOARD--------------------------------------------------------------------------
    while(running)
    {
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
                        up = true;
                        sendPacket(1, saddr, pSend, s );                        //Net
                        break;

                    case SDL_SCANCODE_UP:
                        upP2 = true;
                        sendPacket(1, saddr, pSend, s );                        //Net
                        break;

                    case SDL_SCANCODE_A:
                        left = true;
                        sendPacket(2, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_LEFT:
                        leftP2 = true;
                        sendPacket(2, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_S:
                        down = true;
                        sendPacket(3, saddr, pSend, s ); 
                        break;

                      case SDL_SCANCODE_DOWN:
                        downP2 = true;
                        sendPacket(3, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_D:
                        right = true;
                        sendPacket(4, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_RIGHT:
                        rightP2 = true;
                        sendPacket(4, saddr, pSend, s ); 
                        break;

                   default:
                       break;
                }
                break;
                case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                        up = false;
                        break;

                    case SDL_SCANCODE_UP:
                        upP2 = false;
                   //     sendPacket(1, saddr, pSend, s );                        //Net
                        break;

                    case SDL_SCANCODE_A:
                        left = false;
                        break;

                    case SDL_SCANCODE_LEFT:
                        leftP2 = false;
                    //    sendPacket(2, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_S:
                        down = false;
                        break;

                    case SDL_SCANCODE_DOWN:
                        downP2 = false;
                    //    sendPacket(3, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_D:
                        right = false;
                        break;

                    case SDL_SCANCODE_RIGHT:
                        rightP2 = false;
               //         sendPacket(4, saddr, pSend, s ); 
                        break;

                    default:
                        break;
                }
                break;
            }
             
           
        }
//------------------------------------------------------UPDATE LOGICAL OBJECTS--------------------------------------------------------------------------

         //Update attributes of the struct
        if (up == true)
            changePlayerSpeed(player, ACCELERATION);
        if (down == true)
            changePlayerSpeed(player, -ACCELERATION);
            speedLimit(player);
        if (left == true)
            changePlayerDirection(player, TURNING_SPEED - getPlayerSpeed(player));      //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
        if (right == true)
            changePlayerDirection(player, -TURNING_SPEED + getPlayerSpeed(player));     //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds


         if (upP2 == true)
             changePlayerSpeed(player2, ACCELERATION);
         if (downP2 == true)
             changePlayerSpeed(player2, -ACCELERATION);
             speedLimit(player2);
         if (leftP2 == true)
             changePlayerDirection(player2, TURNING_SPEED - getPlayerSpeed(player2));      //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
         if (rightP2 == true)
            changePlayerDirection(player2, -TURNING_SPEED + getPlayerSpeed(player2));     

        
        //Recive packet, for now just recive mirroring from server                  //Net
        if (SDLNet_UDP_Recv(s, pRecive)){
            int a; 
            sscanf((char * )pRecive->data, "%d\n", &a);
            printf("Incoming movement: %d\n", a);
        }     
      
        //Update position of the struct
        updatePlayerPosition(player, 1);
        colissionDetectionPlayerArena(player);
        colissionDetectionBallArena(b);

        updatePlayerPosition(player2, 1);
        colissionDetectionPlayerArena(player2);
        colissionDetectionBallArena(b);

        //  if(distanceBallPlayer(b, player) < sqrt( (pow (getBallHeight()/2 - getPlayerHeight()/2, 2) + pow (getBallWidth()/2 - getPlayerWidth()/2, 2))))
        // {
        //     setBallDirection(b, angleBallPlayer(b, player));
        //     setBallDirection(b, getPlayerDirection(player));
        //     setBallSpeed(b, getBallSpeed(b)*0.7 + getPlayerSpeed(player)+2);
        // }

         if(PlayerBallCollision(&gPlayer,&gBall))
        {
            setBallDirection(b, angleBallPlayer(b, player));
            setBallDirection(b, getPlayerDirection(player));
            setBallSpeed(b, getBallSpeed(b)*0.7 + getPlayerSpeed(player)+2);
        }

          if(PlayerBallCollision(&gPlayer2,&gBall))
        {
            setBallDirection(b, angleBallPlayer(b, player2));
            setBallDirection(b, getPlayerDirection(player2));
            setBallSpeed(b, getBallSpeed(b)*0.7 + getPlayerSpeed(player2)+2);
        }

        updateBallPosition(b, 1);
  
        if(distanceBallPlayer(b, player) < 1)
        {
            setBallPositionX(b, (float)WINDOW_WIDTH/2);
            setBallPositionY(b, (float)WINDOW_WIDTH/2);
        } 
//------------------------------------------------------FORWARD LOGICAL OBJECTS TO GRAPHICAL OBJECTS--------------------------------------------------------------------------
        gPlayer.y = getPlayerPositionY(player);
        gPlayer.x = getPlayerPositionX(player);

        gBall.y = getBallPositionY(b);
        gBall.x = getBallPositionX(b);

        
        gPlayer2.y = getPlayerPositionY(player2);
        gPlayer2.x = getPlayerPositionX(player2);

        
        if(ballRightGoalCollision(&gBall))
        {

            setBallPositionX(b,470);
            setBallPositionY(b,260);
            setBallSpeed(b,0);
         //   P1Score++;
            
        }

         if(ballLeftGoalCollision(&gBall))
        {
            setBallPositionX(b,470);
            setBallPositionY(b,260);
            setBallSpeed(b,0);
            
        }

        
        SDL_RenderClear(renderer);
        renderBackground();
     
        SDL_RenderCopy(renderer,mBall,NULL,&gBall);
        SDL_RenderCopyEx(renderer, mPlayer, NULL, &gPlayer, -getPlayerDirection(player), NULL, SDL_FLIP_NONE);

        SDL_RenderCopyEx(renderer, mPlayer2, NULL, &gPlayer2, -getPlayerDirection(player2), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
     //  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(1000/50);
    }
    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mField);
    SDL_DestroyTexture(mPlayer);
    SDL_DestroyTexture(mBall);
    SDL_DestroyRenderer(renderer);
   // TTF_Quit();
    SDL_Quit();
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
    sBall = IMG_Load("images/SoccerBall.png");
 
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, sPlayer);

    mPlayer2 = SDL_CreateTextureFromSurface(renderer,sPlayer2);

    mBall = SDL_CreateTextureFromSurface(renderer,sBall);


    player = createPlayer(100, 455);
    player2 = createPlayer(810, 455);

    b = createBall(470,260);

    gPlayer.x = getPlayerPositionX(player);
    gPlayer.y = getPlayerPositionY(player);
    gPlayer.h = getPlayerHeight();
    gPlayer.w =getPlayerWidth();

    gPlayer2.x = getPlayerPositionX(player2);
    gPlayer2.y = getPlayerPositionY(player2);
    gPlayer2.h = getPlayerHeight();
    gPlayer2.w =getPlayerWidth();

    gBall.x = getBallPositionX(b);
    gBall.y = getBallPositionY(b);
    gBall.h = getBallHeight();
    gBall.w = getBallWidth();

    if(NULL == imageSurface)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
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
    
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);

    sGoal_Left = IMG_Load("images/Goal_Left.png");
    sGoal_Right = IMG_Load("images/Goal_Right.png");
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);
    mGoal_Left = SDL_CreateTextureFromSurface(renderer, sGoal_Left);
    mGoal_Right = SDL_CreateTextureFromSurface(renderer, sGoal_Right);
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
//    TTF_Init();
    window = SDL_CreateWindow("Under production", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("Could not create window. Error: %s ",SDL_GetError());
        printf("\n");
        test = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  //  font = TTF_OpenFont("Images/arial.ttf", 60);
  //  SDL_Color color = { 144, 77, 255,255 };
  //  surface = TTF_RenderText_Solid(font,
    //    "Välkommen", color);

  //  texture = SDL_CreateTextureFromSurface(renderer, surface);

  //  int texW = 50;
  //  int texH = 50;
  //  SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
  //  SDL_Rect dstrect = { 0, 0, texW, texH };

    if(renderer == NULL)
    {
        printf("Could not create renderer. Error: %s",SDL_GetError());
        printf("\n");
        test = false;
    }
    return test;
}

void sendPacket(int movement, IPaddress svr, UDPpacket *packet, UDPsocket s)
{
   // printf("Player move: %d\n", (int) movement);
    sprintf((char *)packet->data, "%d\n", (int) movement);    
    packet->address.host = svr.host;	/* Set the destination host */
	packet->address.port = svr.port;	/* And destination port */
	packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(s, -1, packet);
}

