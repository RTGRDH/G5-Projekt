#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <math.h>
#include "ball.h"
#include "player.h"
#include "gameLogic.h"

struct clients {
    Player player;
    Uint32 IP;
    Uint32 port;
    SDL_Rect gPlayer; 
};
typedef struct clients Clients;
 
const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
//server funktioner
void clients_null(Clients c[]);
void clients_start(Clients c[]);
void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount);
void client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a);
void clientPos_send(Clients c[], Ball b, UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount);
void gameEngine (Clients c[], Ball b, int* pClientCount, SDL_Rect* gBoll, int* pt1NrOfGoals, int* pt2NrOfGoals);
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

#define SPEED (2); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 4
#define ACCELERATION 0.2

int main(int argc, char **argv)
{
    Ball boll = createBall(470,260);SDL_Rect gField;SDL_Rect gBall;SDL_Rect gGoal_Left;SDL_Rect gGoal_Right;/*SDL_Rect dstrect;*//*TTF_Font * font = NULL;*/
    UDPsocket sd;/* Socket descriptor */UDPpacket *pRecive;/* Pointer to packet memory */UDPpacket *pSent;
    Clients client[4] =
    {
        {malloc(sizeof(Player)),0,0,0},
        {malloc(sizeof(Player)),0,0,0},
        {malloc(sizeof(Player)),0,0,0},
        {malloc(sizeof(Player)),0,0,0}
    };
    clients_null(client);

    int quit, a, b;int clientCount=0;int* pClientCount;
    int t1NrOfGoals=0;
    int t2NrOfGoals=0;
    int* pt1NrOfGoals;
    int* pt2NrOfGoals;
    pClientCount=&clientCount;
    pt1NrOfGoals=&t1NrOfGoals;
    pt2NrOfGoals=&t2NrOfGoals;

 
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Open a socket */
    if (!(sd = SDLNet_UDP_Open(2000)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    /* Make space for the packet */
    if (!((pSent = SDLNet_AllocPacket(512))&&(pRecive = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Main loop */
    quit = 0;
    while (!quit)
    {
        int slow=0;
        /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
        if (SDLNet_UDP_Recv(sd, pRecive))
        {
            int tmpClient=0;
            int i,x=0, movement;
            float tmpMovement;
        
            sscanf((char * )pRecive->data, "%d \n", &movement);

            if((*pClientCount)==4)
            {
                for(i=0; i<4; i++)
                {
                    if(pRecive->address.port==client[i].port)
                    {
                        tmpClient=i;
                    }
                    //else movement5?
                }
                if(pRecive->address.port == client[tmpClient].port)
                {
                       
//-----------------------------------------------------READ FROM CLIENT---------------------------------------------------------------
                    //skelettkod:
                    //if 1: left + gas        ||    if 2: gas        ||    if 3: right + gas
                    //if 4: left            ||    if 5: no send    ||    if 6: right
                    //if 7: left + brake    ||    if 7: brake        ||    if 9: right + brake
                    //hypotetisk kod:

                    if (movement == 1 || movement == 4 || movement==7)
                    {
                        changePlayerDirection(client[tmpClient].player, (float) TURNING_SPEED - getPlayerSpeed(client[tmpClient].player));
                    }
                    if (movement == 3 || movement == 6 || movement == 9)
                    {
                        changePlayerDirection(client[tmpClient].player, -TURNING_SPEED + getPlayerSpeed(client[tmpClient].player));
                    }
                    if (1 <= movement && movement <= 3)
                    {
                        changePlayerSpeed(client[tmpClient].player, ACCELERATION);
                    }
                    if (7 <= movement && movement <= 9)
                    {
                        changePlayerSpeed(client[tmpClient].player, -ACCELERATION);
                    }
                    movement = 5;         //reset the movement variable to base case
                    speedLimit(client[tmpClient].player);   
                }
            }   
            for(int i=0; i<=*pClientCount; i++)
            {
                if(pRecive->address.port == client[i].port)
                {
                    sscanf((char * )pRecive->data, "%d \n", &movement);
                    if((*pClientCount)==4)
                    {
                        clientPos_send(client, boll, pRecive, pSent, sd, i, pClientCount);
                    }
                    x=1;
                }
            }
            for(int i=0; i<=*pClientCount; i++)
            {
                if(x==0)
                {
                    if(client[i].IP == 0 && client[i].port == 0)
                    {
                        
                        sscanf((char * )pRecive->data, "%d \n", &movement);
                        client_create(client, pRecive, i, pClientCount);
                        break;
                    }
                }
            }
            if(*pt1NrOfGoals==3 || *pt2NrOfGoals==3)
            {
                quit=1;
            }
            /* Quit if packet contains "quit" */
            if (strcmp((char *)pSent->data, "quit") == 0)
                quit = 1;
        }
            gameEngine (client, boll, pClientCount, &gBall, pt1NrOfGoals,pt2NrOfGoals);

    }

    /* Clean and exit */
    SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
    SDLNet_Quit();
    return EXIT_SUCCESS;
}

void clients_null(Clients c[])
{
    for(int i=0; i<4; i++)
    {
        c[i].IP=0;
        c[i].port=0;
    }
    clients_start(c);
}

void clients_start(Clients c[])
{
    c[0].player = createPlayer(50, 50); setPlayerDirection(c[0].player, 45); setPlayerSpeed(c[0].player, 0);
    c[1].player = createPlayer(880, 50); setPlayerDirection(c[1].player, 315); setPlayerSpeed(c[1].player, 0);
    c[2].player = createPlayer(50, 450); setPlayerDirection(c[2].player, 135); setPlayerSpeed(c[2].player, 0);
    c[3].player = createPlayer(880, 450); setPlayerDirection(c[3].player, 225); setPlayerSpeed(c[3].player, 0);

}

void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount)
{
        printf("Client %d\n", *pClientCount+1);
        c[i].IP = recive->address.host;
        c[i].port = recive->address.port;
        (*pClientCount)++;
        printf("createfunktion\n");
        //printf("for1 c%d ip: %d port: %d \n", clientCount, &client[i].IP, &client[i].port);
}

void client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a)
{

    for(int j=0; j<*pClientCount;j++)
    {
        if (c[j].port != 0)// && client[j].port != client[i].port
        {
            printf("Send to Client %d \n", j+1);
            sent->address.host = c[j].IP;    /* Set the destination host */
            sent->address.port = c[j].port;
            sscanf((char * )recive->data, "%d\n", &a);
            printf("%d\n", a);
            sprintf((char *)sent->data, "%d\n", a);
            sent->len = strlen((char *)sent->data) + 1;
            SDLNet_UDP_Send(sd2, -1, sent);
        }
    }
}

void clientPos_send(Clients c[], Ball b, UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount)
{
            for(int j=0; j<*pClientCount;j++)
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
                printf("\nSend to Client %d \n", i+1);
                sent->address.host = c[j].IP;    /* Set the destination host */
                sent->address.port = c[j].port;
                printf("clientPos_send data\nBil1: (%.0f,%.0f),%.0f, \nBil2: (%.0f,%.0f),%.0f, \nBil3: (%.0f,%.0f),%.0f, \nBil4: (%.0f,%.0f),%.0f, \nBoll: (%.0f,%.0f)\nEnd loop\n", p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY);
                sprintf((char *)sent->data, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",  p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY);
                sent->len = strlen((char *)sent->data) + 1;
                SDLNet_UDP_Send(sd2, -1, sent);
            }
}
void gameEngine (Clients c[], Ball b, int* pClientCount, SDL_Rect* gBoll, int* pt1NrOfGoals, int* pt2NrOfGoals)
{
     for(int i=0; i<*pClientCount; i++)
                {
					updatePlayerPosition(c[i].player, 0.5);
					colissionDetectionPlayerArena(c[i].player);
                    if(distanceBallPlayer(b,c[i].player)<27)
                    {
                        setBallDirection(b,angleBallPlayer(b,c[i].player));
                        setBallSpeed(b, getBallSpeed(b)*0.7 + getPlayerSpeed(c[i].player)+2);
                    }
                }
                //collision detection between players and players
                updateBallPosition(b,0.5);
				colissionDetectionBallArena(b);
                
                if(ballRightGoalCollision(gBoll))
                {
                    setBallPositionX(b,470);
                    setBallPositionY(b,260);
                    setBallSpeed(b,0);
                    (*pt1NrOfGoals)++;
                    clients_start(c);
                }
                else if(ballLeftGoalCollision(gBoll))
                {
                    setBallPositionX(b,470);
                    setBallPositionY(b,260);
                    setBallSpeed(b,0);
                    (*pt2NrOfGoals)++;
                    clients_start(c);
                }
                
                gBoll->y = getBallPositionY(b);
                gBoll->x = getBallPositionX(b);
                
}