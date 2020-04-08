/********************************************************************
*   File Name: fifo.h
*   Description: Contains variable definitions and declarations
*   Others: Header file used by server.c client.c fifo.c
*   Author: Zhang Houjin
*   Date: 2020/04/06
*********************************************************************/

#ifndef __MYFIFO__
#define __MYFIFO__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define PUBLIC_FIFO "Server_FIFO"
#define PRIVATE_FIFO "Client_FIFO_"
#define IS_NEW_CLIENT "handshake\n"
#define CLIENT_QUIT "quit\n"

struct FIFO_Data{
    int client_pid;
    char message[100];
};

void sigcatch(int signum);
void Create_FIFO(char *FIFO_Name);
void Send_and_Recive_Message(void);
char* Get_Private_FIFO_Name(int Client_PID);

int PublicFd, PrivateFd;
char Private_Name[20];
char* Private_FIFO_Name;
struct FIFO_Data Client_to_Server, Server_to_Client;

#endif  // __MYFIFO__
