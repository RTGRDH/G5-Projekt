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
	Uint32 IP;
	Uint32 port;
	Player player;
	SDL_Rect gPlayer;
}; 
typedef struct clients Clients;
 
const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
//server funktioner
void clients_null(Clients c[]);
void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount);
void client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a);
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

Ball boll = NULL;
SDL_Rect gField;
// struct to hold the position and size of the sprite
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


int main(int argc, char **argv)
{
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Clients client[4];
	clients_null(client);

    int quit, a, b, x; 
	int clientCount=0;
	int* pClientCount;
	pClientCount=&clientCount;

 
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
 
	//bil start positioner
	/* Main loop */
	quit = 0;
	while (!quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pRecive))
		{
			//printf("UDP Packet incoming\n");
			//printf("\tData:    %s\n", (char *)pRecive->data);
			//printf("\tAddress: %x %x\n", pRecive->address.host, pRecive->address.port);
			int tmpClient;

			if((*pClientCount)==4)
			{
				changePlayerSpeed(client[tmpClient].player, ACCELERATION);
				speedLimit(client[tmpClient].player);
				changePlayerDirection(client[tmpClient].player, TURNING_SPEED-getPlayerSpeed(client[tmpClient].player));

				if(PlayerBallCollision(&client[tmpClient].gPlayer, &gBall))
				{
					setBallDirection(boll,angleBallPlayer(boll,client[tmpClient].player));
					setBallDirection(boll, getPlayerDirection(client[tmpClient].player));
					setBallSpeed(boll, getBallSpeed(boll)*0.7 + getPlayerSpeed(client[tmpClient].player)+2);
				}

				updateBallPosition(boll,1);

				if(distanceBallPlayer(boll,client[tmpClient].player)<1)
				{
					setBallPositionX(boll,(float)WINDOW_WIDTH/2);
					setBallPositionY(boll,(float)WINDOW_WIDTH/2);
				}
				
				client[tmpClient].gPlayer.y=getPlayerPositionY(client[tmpClient].player);
				client[tmpClient].gPlayer.x=getPlayerPositionX(client[tmpClient].player);

				if(ballRightGoalCollision(&gBall))
				{

					setBallPositionX(boll,470);
					setBallPositionY(boll,260);
					setBallSpeed(boll,0);
				//   P1Score++;	
				}

				if(ballLeftGoalCollision(&gBall))
				{
					setBallPositionX(boll,470);
					setBallPositionY(boll,260);
					setBallSpeed(boll,0);
				}
				
				gBall.y = getBallPositionY(boll);
       			gBall.x = getBallPositionX(boll);

			}
			x=0;

			for(int i=0; i<=*pClientCount; i++)
			{
				if(pRecive->address.port == client[i].port) //&& *pClintcount==4
				{
					client_send(client, pRecive, pSent, sd, i, pClientCount, a);
					x=1;
				}
				
				if(x==0)

				{
					if(client[i].IP == 0 && client[i].port == 0)
					{	
						client_create(client, pRecive, i, pClientCount);
						break;
					}
				}
			}
			
			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}
 
	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pRecive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 


void clients_null(Clients c[])
{
	for(int i=1; i<=4; i++)
	{
		c[i].IP=0;
		c[i].port=0;
		c[i].player=NULL;
		//switch(i)
	}
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
				sent->address.host = c[j].IP;	/* Set the destination host */
				sent->address.port = c[j].port;
				sscanf((char * )recive->data, "%d\n", &a);
				printf("%d\n", a);
				sprintf((char *)sent->data, "%d\n", a);
				sent->len = strlen((char *)sent->data) + 1;
				SDLNet_UDP_Send(sd2, -1, sent);		
			}
		}		

}