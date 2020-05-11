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

 /*struct players
 {
	 float x;
	 float y;
	 float d;
 };
 typedef struct players Players*/
 
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
void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount);
void client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a);
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

#define SPEED (75); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 10
#define ACCELERATION 0.1


int main(int argc, char **argv)
{
	Ball boll = createBall(0,0);
	/*setBallPositionX(boll,470);
	setBallPositionY(boll,260);
	setBallDirection(boll,0);
	setBallSpeed(boll,0);*/
	SDL_Rect gField;
	// struct to hold the position and size of the sprite
	SDL_Rect gBall;
	SDL_Rect gGoal_Left;
	SDL_Rect gGoal_Right;
	//SDL_Rect dstrect;

	//TTF_Font * font = NULL;



	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pRecive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Clients client[4] = 
	{
		{malloc(sizeof(Player)),0,0,0},
		{malloc(sizeof(Player)),0,0,0},
		{malloc(sizeof(Player)),0,0,0},
		{malloc(sizeof(Player)),0,0,0}
	};
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
	printf("innan boll");
 	/*setBallPositionX(boll,470);
	setBallPositionY(boll,260);
	setBallDirection(boll,0);
	setBallSpeed(boll,0);*/
	printf("edter boll");
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
			int tmpClient=0;
			int i,x=0, movement;
			float tmpMovement;
            sscanf((char * )pRecive->data, "%d \n", &movement);

			if((*pClientCount)==4)
			{
				printf("movement: %d\n",movement);
				for(i=0; i<4; i++)
				{
					printf("inne i lopen %d, tmp:%d\n", i, tmpClient);
					if(pRecive->address.port==client[i].port)
					{
						tmpClient=i;
						printf("inne i if %d, %d \n", i, tmpClient);
					}
					//else movement5?
				}
				if(pRecive->address.port == client[tmpClient].port)
                {
                    printf("crashsite 1: tmp %d  mov %d", tmpClient, movement);
                       
//-----------------------------------------------------READ FROM CLIENT---------------------------------------------------------------
					//skelettkod:
					//if 1: left + gas		||	if 2: gas		||	if 3: right + gas
					//if 4: left			||	if 5: no send	||	if 6: right
					//if 7: left + brake	||	if 7: brake		||	if 9: right + brake
					//hypotetisk kod:

					if (movement == 1 || movement == 4 || movement==7)
					{
						printf("\ngonna turn left(1,4,7)");
						printf("\nmovement: %d,", movement);
						printf("client[tmpClient].player: %.0f", getPlayerPositionX(client[tmpClient].player));
						printf("\n\n\nCLIENT[TMPCLIENT] FUNGERAR\n\n\n\n");
						changePlayerDirection(client[tmpClient].player, (float) TURNING_SPEED + getPlayerSpeed(client[tmpClient].player));
						printf("\nturned left");
					}
					printf("\npast left turn");

					if (movement == 3 || movement == 6 || movement == 9)
					{
						printf("\ngonna turn right(3,6,9)");
						printf("\n %d", movement);
            			changePlayerDirection(client[tmpClient].player, -TURNING_SPEED + getPlayerSpeed(client[tmpClient].player));
					printf("\nturned right");
					}
					printf("\npast right turn");

					if (1 <= movement && movement <= 3)
					{
						printf("\ngonna accelerate(1,2,3)");
						printf("\n %d", movement);
						changePlayerSpeed(client[tmpClient].player, ACCELERATION);
						printf("\naccelerated");
					}
					printf("\npast acceleration");

					if (7 <= movement && movement <= 9)
					{
						printf("\ngonna brake(7,8,9)");
						printf("\n %d", movement);
						changePlayerSpeed(client[tmpClient].player, -ACCELERATION);
						printf("\nbraked");
					}
					printf("\npast braking");
					movement = 5; 		//reset the movement variable to base case
					updatePlayerPosition(client[tmpClient].player, 0);
					speedLimit(client[tmpClient].player);		//this crashes the server for some reason
					//printf("crashsite 2.2");
				}
				printf("crashsite 2\n");
//-----------------------------------------------------UPDATE ON SERVER-------------------------------------------------------------------
				for(i=0; i<*pClientCount; i++)
				{
					//updatePlayerPosition(client[i].player, 0);
					printf("crashsite 3\n");
					if(distanceBallPlayer(boll,client[i].player)<27)
					{
						setBallDirection(boll,angleBallPlayer(boll,client[i].player));
						setBallSpeed(boll, getBallSpeed(boll)*0.7 + getPlayerSpeed(client[i].player)+2);
					}
				}
				//collision detection between players and players
				updateBallPosition(boll,1);
				
				if(ballRightGoalCollision(&gBall))
				{
					setBallPositionX(boll,470);
					setBallPositionY(boll,260);
					setBallSpeed(boll,0);
				//   P1Score++;	
				}
				else if(ballLeftGoalCollision(&gBall))
				{
					setBallPositionX(boll,470);
					setBallPositionY(boll,260);
					setBallSpeed(boll,0);
				}
				
				gBall.y = getBallPositionY(boll);
       			gBall.x = getBallPositionX(boll);
				
				for(i=0;i<4;i++)
				{
					client[i].gPlayer.y=getPlayerPositionY(client[i].player);
					client[i].gPlayer.x=getPlayerPositionX(client[i].player);
				}
				
				   printf("har ocksa");
			}

			for(int i=0; i<=*pClientCount; i++)
			{
				if(pRecive->address.port == client[i].port)
				{
					sscanf((char * )pRecive->data, "%d \n", &movement);
    				printf("incomming %d\n",movement);
					if((*pClientCount)==4)
					{
						clientPos_send(client, boll, pRecive, pSent, sd, i, pClientCount);
					}
					//client_send(client, pRecive, pSent, sd, i, pClientCount, a);
					x=1;
				}
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
			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				quit = 1;
		}		
	}
 
	printf("kracha inte");
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
		//c[i].player=NULL;
		switch(i)
		{
			case '1': c[i].player = createPlayer(50, 50); setPlayerDirection(c[i].player, 45); setPlayerSpeed(c[i].player, 0);
			break;
			case '2': c[i].player = createPlayer(880, 50); setPlayerDirection(c[i].player, 315); setPlayerSpeed(c[i].player, 0);
			break;
			case '3': c[i].player = createPlayer(50, 450); setPlayerDirection(c[i].player, 135); setPlayerSpeed(c[i].player, 0);
			break;
			case '0': c[i].player = createPlayer(880, 450); setPlayerDirection(c[i].player, 225); setPlayerSpeed(c[i].player, 0);
			break;
		}
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

void clientPos_send(Clients c[], Ball b, UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount)
{
			int p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY;
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
			printf("Send to Client %d \n", i+1);
			sent->address.host = c[i].IP;	/* Set the destination host */	
			sent->address.port = c[i].port;
			//printf("%d\n", a);
			sprintf((char *)sent->data, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",  p1X,p1Y,p1D,p2X,p2Y,p2D,p3X,p3Y,p3D,p4X,p4Y,p4D,bX,bY);
			sent->len = strlen((char *)sent->data) + 1;
			SDLNet_UDP_Send(sd2, -1, sent);	
}