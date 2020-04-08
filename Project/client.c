/********************************************************************
*   File Name: client.c
*   Description: \
*   This program passes the client's pid to the server, \
*   writes data to the server with the public FIFO, \
*   and reads data from server with the private FIFO.
*   Author: Zhang Houjin
*   Date: 2020/04/06
*********************************************************************/

#include "fifo.h"

int main(){

    /* Initial handshake message */
    Client_to_Server.client_pid = getpid();
    strcpy(Client_to_Server.message, IS_NEW_CLIENT);

    /* Get private FIFO name */
    Private_FIFO_Name = Get_Private_FIFO_Name(getpid());
    printf("Client PID is: %d\nClient Message is: %s\n", \
    Client_to_Server.client_pid, Client_to_Server.message);

    /* Client handshake with server, make a communication link */
    Send_and_Recive_Message();

    /* Keep communication */
    while(1){
        printf("Please input message: ");
        fgets(Client_to_Server.message, 60, stdin);
        Client_to_Server.client_pid = getpid();
        Send_and_Recive_Message();
    }

}
