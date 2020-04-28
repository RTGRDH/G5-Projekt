#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` udps.c -o udps `sdl-config --libs` -lSDL_net
 
exit
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL_net.h"

struct clients {
	Uint32 IP;
	Uint32 port;
}; 
typedef struct clients Clients;
 

void clients_null(Clients c[]);
void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount);
int client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a);


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
			x=0;

			for(int i=0; i<=*pClientCount; i++)
			{
				if(pRecive->address.port == clients[i].port)
				{
					x=client_send(client, pRecive, pSent, sd, i, pClientCount, a);
				}
				
				if(x==0)
				{
					client_create(client, pRecive, i, pClientCount);
					break;
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
	for(int i=0; i<=4; i++)
	{
		c[i].IP=0;
		c[i].port=0;
	}
}

void client_create(Clients c[], UDPpacket *recive, int i, int* pClientCount)
{
	if(c[i].IP == 0 && c[i].port == 0)
	{
		printf("Client %d\n", *pClientCount+1);
        c[i].IP = recive->address.host;
		c[i].port = recive->address.port;
		(*pClientCount)++;
		//printf("for1 c%d ip: %d port: %d \n", clientCount, &client[i].IP, &client[i].port);
		
	}
}

int client_send(Clients c[], UDPpacket *recive, UDPpacket *sent, UDPsocket sd2, int i, int* pClientCount, int a)
{
	int xReturn=0;

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
		xReturn=1;		
	return xReturn;

}