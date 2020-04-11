/********************************************************************
*   File Name: client.c
*   Function: Implemented group chat and private chat functions.
*   Description: \
*   This program passes the client's pid to the server, \
*   writes data to the server with the public FIFO, \
*   and reads data from server with the private FIFO.
*   Author: Zhang Houjin
*   Init Date: 2020/04/06
*   Modified Date: 2020/04/11
*********************************************************************/

#include "fifo.h"

int main(){

    pid_t pid;

    /* Initial handshake message */
    printf("Please input your name: ");
    scanf("%s", Client_to_Server.client_name);
    getchar();
    Client_to_Server.client_pid = getpid();
    strcpy(Client_to_Server.message, IS_NEW_CLIENT);
    strcpy(Client_to_Server.target_name, BROADCAST_TO_ALL);

    /* Get private FIFO name */
    Private_FIFO_Name = Get_Private_FIFO_Name(getpid());
    printf("Client PID is: %d\nClient Message is: %s\n", \
    Client_to_Server.client_pid, Client_to_Server.message);

    /* Client handshake with server, make a communication link */
    Client_Write_Data(!IS_PARENT, -1);
    Client_Read_Data();

    /* Keep communication */
    if((pid = fork()) < 0){
        printf("Fail to call fork()\n");
        exit(1);
    }

    /* Parent process sends data */
    else if(pid > 0){
        while(1){
            // printf("Please input message: ");
            fgets(Client_to_Server.message, 60, stdin);
            Client_to_Server.client_pid = getpid();
            Private_Chat_Filter_By_Name(Client_to_Server.message);
            Client_Write_Data(IS_PARENT, pid);
        }
    }

    /* Child process receives data */
    else{
        while(1){
            /* Read server struct data */
            Client_Read_Data();
        }
    }

}
