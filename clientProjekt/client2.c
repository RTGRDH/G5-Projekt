#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>                                      
#include <string.h>                                         
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
 //#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include <math.h>
#include "ball.h"
#include "gameLogic.h"

#define SIZE 4
const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;

bool init();
void renderBackground();
bool initPlayField();
bool initMedia();
void sendPacket(Player p,  int movement, IPaddress svr, UDPpacket *packet, UDPsocket s);   

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

int main(int argc, char * argv[])
{
    UDPsocket s;                                                
	IPaddress saddr;                                            
	UDPpacket *pSend;                                           
    UDPpacket *pRecive;                                        
    bool running = true;
    int P1Score = 0;
    int P2Score = 0;
    int loop=0;
    //Player players[SIZE]={0};
    //int nrOfPlayers=0;

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
    
    //while(!loop){                                                                       //Ev loop för att starta kontakt med server och den lägger in klienten i en struct (så klienten får en bil)
    //    printf("Send packet to connect with serverr\n");
    //    int startup=1, i, x, y, dir, speed;
    //    printf("%d\n", startup);
    //    sprintf((char *)pSend->data, "%d\n", (int)startup);    
    //    pSend->address.host = saddr.host;	/* Set the destination host */
	//    pSend->address.port = saddr.port;	/* And destination port */
	//    pSend->len = strlen((char *)pSend->data) + 1;
    //    SDLNet_UDP_Send(s, -1, pSend); 

    //    if (SDLNet_UDP_Recv(s, pRecive)){
    //    sscanf((char * )pRecive->data, "%d %d %d %d %d\n",&i, &x, &y, &dir, &speed);
	//	printf("Bil %d\n", i);
		//printf("%d %d %d %d\n", x, y, dir, speed);
        //player=createPlayer(x, y);
    //    loop=1;
    //    }
    //}
    

    if(init())
    {
        printf("Initialize window and renderer successful.\n");
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
           
                                                                                  
    setPlayerDirection(player, 45);                                              
    setPlayerDirection(player2, 315);                                            
    setPlayerDirection(player3, 135);                                            
    setPlayerDirection(player4, 225);
    
    // keep track of which inputs are given
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

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
                    case SDL_SCANCODE_UP:
                        up = true;
                        //sendPacket( player, 1, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = true;
                        //sendPacket( player, 2, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = true;
                        //sendPacket(player, 3, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = true;
                        //sendPacket(player,  4, saddr, pSend, s ); 
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
                        //sendPacket(player, 5, saddr, pSend, s ); 
                        break;

                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = false;
                        //sendPacket( player, 6, saddr, pSend, s );
                        break;

                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = false;
                        //sendPacket(player, 7, saddr, pSend, s );
                        break;

                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = false;
                        //sendPacket(player, 8, saddr, pSend, s );
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
        if (SDLNet_UDP_Recv(s, pRecive)){
            float x1, y1, d1, x2, y2, d2, x3, y3, d3, x4, y4, d4, ballx, bally; 
            sscanf((char * )pRecive->data, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&x1, &y1, &d1, &x2, &y2, &d2, &x3, &y3, &d3, &x4, &y4, &d4, &ballx, &bally);

            gBall.x=ballx;
            gBall.y=bally;

            gPlayer.y = y1;
            gPlayer.x = x1;
            setPlayerDirection(player, d1);

            gPlayer2.y = y2;
            gPlayer2.x = x2;
            setPlayerDirection(player, d2);

            gPlayer3.y = y3;
            gPlayer3.x = x3;
            setPlayerDirection(player, d3);

            gPlayer4.y = y4;
            gPlayer4.x = x4;
            setPlayerDirection(player, d4);

            printf("Incoming x1: %f, y1: %f d1:%f \n x2: %f y2: %f d2: %f \n x3: %f, y3: %f d3:%f \n x4: %f y4: %f d4: %f \n bollx: %f bolly: %f\n",x1, y1, d1, x2, y2, d2, x3, y3, d3, x4, y4, d4, ballx, bally);
        }
        
        
        SDL_RenderClear(renderer);
        renderBackground();
     
        SDL_RenderCopy(renderer,mBall,NULL,&gBall);
        SDL_RenderCopyEx(renderer, mPlayer, NULL, &gPlayer, -getPlayerDirection(player), NULL, SDL_FLIP_NONE);

        SDL_RenderCopyEx(renderer, mPlayer2, NULL, &gPlayer2, -getPlayerDirection(player2), NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, mPlayer3, NULL, &gPlayer3, -getPlayerDirection(player3), NULL, SDL_FLIP_NONE);

        SDL_RenderCopyEx(renderer, mPlayer4, NULL, &gPlayer4, -getPlayerDirection(player4), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
     // SDL_RenderCopy(renderer, texture, NULL, &dstrect);
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
    sPlayer3 = IMG_Load("images/Car3.png");
    sPlayer4 = IMG_Load("images/Car4.png");
    sBall = IMG_Load("images/SoccerBall.png");
 
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, sPlayer);

    mPlayer2 = SDL_CreateTextureFromSurface(renderer,sPlayer2);

    mPlayer3 = SDL_CreateTextureFromSurface(renderer, sPlayer3);

    mPlayer4 = SDL_CreateTextureFromSurface(renderer,sPlayer4);

    mBall = SDL_CreateTextureFromSurface(renderer,sBall);


    player = createPlayer(50, 50);                                                                        //EV ska detta vara i server?? Vid ny klient
    player2 = createPlayer(880, 50);
    player3 = createPlayer(50, 450);                                                                        //EV ska detta vara i server?? Vid ny klient
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

void sendPacket(Player p, int movement, IPaddress svr, UDPpacket *packet, UDPsocket s)
{
   // printf("Player move: %d\n", (int) movement);
    float positionX, positionY, direction, speed;
   // positionX=getPlayerPositionX(p);
    //positionY=getPlayerPositionY(p);
    //direction=getPlayerDirection(p);
    //speed=getPlayerSpeed(p);
    printf("%d \n",movement);//(int)positionX, (int) positionY, (int)direction, (int) speed, movement);
    sprintf((char *)packet->data, "%d\n",movement);// (int)positionX, (int)positionY, (int)direction, (int)speed, (int)movement);    
    packet->address.host = svr.host;	/* Set the destination host */
	packet->address.port = svr.port;	/* And destination port */
	packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(s, -1, packet);
}
