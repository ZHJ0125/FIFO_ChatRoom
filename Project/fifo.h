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
#define PRIVATE_MSG_HEADER "to:"
#define MAX_Client_Number 5
#define IS_PARENT 1

struct FIFO_Data{
    int client_pid;
    int private_pid;
    char message[100];
};

void sigcatch(int signum);
void Create_FIFO(char *FIFO_Name);
char* Get_Private_FIFO_Name(int Client_PID);
void Store_Private_FIFO_Name(void);
void Server_Send_Message(void);
void Client_Write_Data(int Process, int Child_PID);
void Client_Read_Data(void);
void Private_Chat_Filter(char* Client_Message);

// int New_Client_Flag;
int Client_Number;
int Client_PID_Box[MAX_Client_Number];
int PublicFd, PrivateFd;
char Private_Name[20];
char* Private_FIFO_Name;
struct FIFO_Data Client_to_Server, Server_to_Client;

#endif  // __MYFIFO__
