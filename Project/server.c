/********************************************************************
*   File Name: server.c
*   Description: \
*   1. Bind the SIGINT signal.
*   2. Create a public FIFO and open it with read-only mode.
*   3. Read the client's data, determine whether the client is new or not.
*   4. If the client is new, create a private pipe based on this client's PID.
*   5. Writes some message to client with the private FIFO.
*   6. Reads data from client with the public FIFO.
*   7. If "quit" is read from the client,\
*      close the private FIFO and end the session.
*   Author: Zhang Houjin
*   Date: 2020/04/06
*********************************************************************/

#include "fifo.h"

int main(){

    /* Bind the SIGINT signal */
    if(signal(SIGINT, &sigcatch) == SIG_ERR){
        printf("Couldn't register signal handler\n");
        exit(1);
    }

    Create_FIFO(PUBLIC_FIFO);

    while(1){

        if((PublicFd = open(PUBLIC_FIFO, O_RDONLY)) < 0){
            printf("Fail to open PUBLIC_FIFO\n");
            exit(1);
        }
        //printf("PUBLIC_FIFO has been opened\n");

        /* Read the strcut data of the public FIFO */
        if(read(PublicFd, &Client_to_Server, sizeof(struct FIFO_Data)) > 0){
            
            printf("Client Pid is : %d\n", Client_to_Server.client_pid);
            printf("Client Message is : %s", Client_to_Server.message);
            Private_FIFO_Name = Get_Private_FIFO_Name(Client_to_Server.client_pid);
            
            /* Create the private FIFO for a new client */
            if(strcmp(Client_to_Server.message, IS_NEW_CLIENT) == 0){
                printf("This is a new client!\n");
                Create_FIFO(Private_FIFO_Name);
            }
            
            /* If the client exits, cut off communication */
            if(strcmp(Client_to_Server.message, CLIENT_QUIT) == 0){
                unlink(Private_FIFO_Name);
                printf("Closed Client_%d Private FIFO\n\n", Client_to_Server.client_pid);
            }
            else{
                /* Server send a reply message to the client */
                if((PrivateFd = open(Private_FIFO_Name, O_WRONLY)) > 0){
                    Server_to_Client.client_pid = Client_to_Server.client_pid;
                    sprintf(Server_to_Client.message, "Hello,Client_%d!\nI Received your message: ",\
                     Client_to_Server.client_pid);
                    strcat(Server_to_Client.message, Client_to_Server.message);
                    if(write(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data))){
                        printf("Write message to Client Success!\n\n");
                        close(PrivateFd);
                    }
                }
            }
            
        }
        else{
            printf("Read Date error!\n");
            exit(1);
        }
    }
    
    return 0;
}
