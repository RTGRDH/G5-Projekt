#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>   //compile warning
#include <math.h>
#include "ball.h"
#include "player.h"
#include "gameLogic.h"

struct clients {
    Player player;
    Uint32 IP;
    Uint32 port;
    SDL_Rect* gPlayer;  
};
typedef struct clients Clients;
 
const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
//server funktioner
void clients_null(Clients c[]);
void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount);
void clientPos_send(Clients c[], Ball b, UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount);
//main funktioner
bool ballRightGoalCollision(SDL_Rect* gBall);
bool ballLeftGoalCollision(SDL_Rect* gBall);
bool PlayerBallCollision(SDL_Rect* gPlayer, SDL_Rect* gBall);
void speedLimit(Player p);
void colissionDetectionPlayerArena(Player p);
void colissionDetectionBallArena(Ball boll);
float xInvertDirection(float direction);
float yInvertDirection(float direction);
float angleBallPlayer(Ball boll, Player p);
float distanceBallPlayer(Ball boll, Player p);
void movementDecoder(Clients c[], int tmp, int movement);
void gameEngine (Clients c[], Ball b, SDL_Rect* gB, int* pt1NrOfGoals, int* pt2NrOfGoals);
void clients_start(Clients c[]);
void INIT_ALL();

#define SPEED (75); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 15
#define ACCELERATION 0.5

int main(int argc, char **argv)
{
    Ball boll = createBall(470,260); 
    SDL_Rect gField; SDL_Rect gBall; SDL_Rect gGoal_Left; SDL_Rect gGoal_Right;
    /*SDL_Rect dstrect;*//*TTF_Font * font = NULL;*/
    UDPsocket sd;/* Socket descriptor */UDPpacket *pRecive;/* Pointer to packet memory */UDPpacket *pSent;
    Clients client[4] ={    {malloc(sizeof(Player)),0,0,SDL_malloc(sizeof(SDL_Rect))},
                            {malloc(sizeof(Player)),0,0,SDL_malloc(sizeof(SDL_Rect))},
                            {malloc(sizeof(Player)),0,0,SDL_malloc(sizeof(SDL_Rect))},
                            {malloc(sizeof(Player)),0,0,SDL_malloc(sizeof(SDL_Rect))}};
    clients_null(client);
    
    int quit,a,b,x,clientCount=0;int* pClientCount; pClientCount=&clientCount;
    int t1NrOfGoals=0;
    int t2NrOfGoals=0;
    int* pt1NrOfGoals;
    int* pt2NrOfGoals;
    pt1NrOfGoals=&t1NrOfGoals;
    pt2NrOfGoals=&t2NrOfGoals;

    if (SDLNet_Init() < 0)                                                                  {/* Initialize SDL_net */
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);}
    if (!(sd = SDLNet_UDP_Open(2000))){                                                      /* Open a socket */
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);}
    if (!((pSent = SDLNet_AllocPacket(512))&&(pRecive = SDLNet_AllocPacket(512)))){         /* Make space for the packet */
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);}
    printf("Waiting for clients\n");
    /* Main loop */
    quit = 0;
    int count = 0;
    while (!quit)
    {
        SDL_Delay(6);
        /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
        while (SDLNet_UDP_Recv(sd, pRecive))
        {
            int tmpClient=0;
            int i,x=0, movement;
            sscanf((char * )pRecive->data, "%d \n", &movement);

            if((*pClientCount)==4)
            {
                for(i=0; i<4; i++)
                {
                    if(pRecive->address.port==client[i].port)
                    {
                        tmpClient=i;
                    }
                }
//-----------------------------------------------------READ FROM CLIENT---------------------------------------------------------------
                //skelettkod:
                //if 1: left + gas        ||    if 2: gas        ||    if 3: right + gas
                //if 4: left            ||    if 5: no send    ||    if 6: right
                //if 7: left + brake    ||    if 7: brake        ||    if 9: right + brake
                //hypotetisk kod:
                movementDecoder(client, tmpClient, movement);
                speedLimit(client[tmpClient].player);
//-----------------------------------------------------UPDATE ON SERVER-------------------------------------------------------------------
            }

            for(int m=0; m<=*pClientCount; m++)
            {
                if(pRecive->address.port == client[m].port)
                {
                    sscanf((char * )pRecive->data, "%d \n", &movement);
                    
                    x=1;
                }
                if(x==0)
                {
                    if(client[m].IP == 0 && client[m].port == 0)
                    {
                        
                        sscanf((char * )pRecive->data, "%d \n", &movement);
                        client_create(client, pRecive, m, pClientCount);
                        break;
                    }
                }
            }
            /* Quit if packet contains "quit" */
            if (strcmp((char *)pSent->data, "quit") == 0)
                quit = 1;   
            if(*pt1NrOfGoals==3)
            {
                printf("Blue team has won.");
                quit=1;
            }
            if (*pt2NrOfGoals==3)
            {
                printf("Orange team has won.");
                quit=1;
            }
        }
        if((*pClientCount)==4)
        {
            if (count == 2){
                clientPos_send(client, boll, pRecive, pSent, sd, 1, pClientCount);
                count = 0;
            }
            count++;
			gameEngine (client, boll, &gBall, pt1NrOfGoals,pt2NrOfGoals);
//            SDL_Delay(30);
        }
    }
    /* Clean and exit */
    SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
    SDLNet_Quit();
    return EXIT_SUCCESS;
}

void clientPos_send(Clients c[], Ball b, UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount)
{
    float p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY;
    p1X = getPlayerPositionX(c[0].player);
    p1Y = getPlayerPositionY(c[0].player);
    p1D = getPlayerDirection(c[0].player);
    p2X = getPlayerPositionX(c[1].player);
    p2Y = getPlayerPositionY(c[1].player);
    p2D = getPlayerDirection(c[1].player);
    p3X = getPlayerPositionX(c[2].player);
    p3Y = getPlayerPositionY(c[2].player);
    p3D = getPlayerDirection(c[2].player);
    p4X = getPlayerPositionX(c[3].player);
    p4Y = getPlayerPositionY(c[3].player);
    p4D = getPlayerDirection(c[3].player);
    bX = getBallPositionX(b);
    bY = getBallPositionY(b);

    sprintf((char *)sent->data, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",  p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY);
    sent->len = strlen((char *)sent->data) + 1;

    sent->address.host = c[0].IP;    /* Set the destination host */
    sent->address.port = c[0].port;
    SDLNet_UDP_Send(sd2, -1, sent);

    sent->address.host = c[1].IP;    /* Set the destination host */
    sent->address.port = c[1].port;
    SDLNet_UDP_Send(sd2, -1, sent);
                
    sent->address.host = c[2].IP;    /* Set the destination host */
    sent->address.port = c[2].port;
    SDLNet_UDP_Send(sd2, -1, sent);
                
    sent->address.host = c[3].IP;    /* Set the destination host */
    sent->address.port = c[3].port;
    SDLNet_UDP_Send(sd2, -1, sent);
}
void clients_null(Clients c[])
{
    for(int i=0; i<4; i++)
    {
        c[i].IP=0;
        c[i].port=0;
        c[0].player = createPlayer(50, 50); setPlayerDirection(c[0].player, 45); setPlayerSpeed(c[0].player, 0);
        c[1].player = createPlayer(880, 50); setPlayerDirection(c[1].player, 135); setPlayerSpeed(c[1].player, 0);
        c[2].player = createPlayer(50, 450); setPlayerDirection(c[2].player, 315); setPlayerSpeed(c[2].player, 0);
        c[3].player = createPlayer(880, 450); setPlayerDirection(c[3].player, 225); setPlayerSpeed(c[3].player, 0);
    }
}

void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount)
{
        printf("Client %d\n", *pClientCount+1);
        c[i].IP = recive->address.host;
        c[i].port = recive->address.port;
        (*pClientCount)++;
        printf("createfunktion\n");
}

void movementDecoder(Clients c[], int tmp, int movement)
{
    if (movement == 1 || movement == 4 || movement==7)
    {
        changePlayerDirection(c[tmp].player, (float) TURNING_SPEED - getPlayerSpeed(c[tmp].player));
    }
    if (movement == 3 || movement == 6 || movement == 9)
    {
        changePlayerDirection(c[tmp].player, -TURNING_SPEED + getPlayerSpeed(c[tmp].player));
    }
    if (1 <= movement && movement <= 3)
    {
        changePlayerSpeed(c[tmp].player, ACCELERATION);
    }
    if (7 <= movement && movement <= 9)
    {
        changePlayerSpeed(c[tmp].player, -ACCELERATION);
    }
}

void gameEngine (Clients c[], Ball b, SDL_Rect* gB, int* pt1NrOfGoals, int* pt2NrOfGoals)
{
    for(int k=0; k<4; k++)
    {
        updatePlayerPosition(c[k].player, 1);
		colissionDetectionPlayerArena(c[k].player);
        if(distanceBallPlayer(b,c[k].player)<27)
        {
            setBallDirection(b,angleBallPlayer(b,c[k].player));
            setBallSpeed(b, getBallSpeed(b)*0.7 + getPlayerSpeed(c[k].player)+2);
        }
    }
    //collision detection between players and players
    updateBallPosition(b,1);
	colissionDetectionBallArena(b);
    if(ballRightGoalCollision(gB))
    {
        setBallPositionX(b,470);
        setBallPositionY(b,260);
        setBallSpeed(b,0);
        (*pt1NrOfGoals)++;
        printf("\nBlue team scores! Blue team has %d goals.", *pt1NrOfGoals);
        clients_start(c);
    //   P1Score++;
    }
    else if(ballLeftGoalCollision(gB))
    {
        setBallPositionX(b,470);
        setBallPositionY(b,260);
        setBallSpeed(b,0);
        (*pt2NrOfGoals)++;
        printf("\nOrange team scores! Orange team has %d goals.", *pt2NrOfGoals);
        clients_start(c);
    }
                
    gB->y = getBallPositionY(b);
    gB->x = getBallPositionX(b);
    for(int l=0;l<4;l++)
    {
        c[l].gPlayer->y=getPlayerPositionY(c[l].player);
        c[l].gPlayer->x=getPlayerPositionX(c[l].player);
    }
}

void clients_start(Clients c[])
{
    c[0].player = createPlayer(50, 50); setPlayerDirection(c[0].player, 45); setPlayerSpeed(c[0].player, 0);
    c[1].player = createPlayer(880, 50); setPlayerDirection(c[1].player, 315); setPlayerSpeed(c[1].player, 0);
    c[2].player = createPlayer(50, 450); setPlayerDirection(c[2].player, 135); setPlayerSpeed(c[2].player, 0);
    c[3].player = createPlayer(880, 450); setPlayerDirection(c[3].player, 225); setPlayerSpeed(c[3].player, 0);
}