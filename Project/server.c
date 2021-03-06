/********************************************************************
*   File Name: server.c
*   Function: Implemented group chat and private chat functions.
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
*   Init Date: 2020/04/06
*   Modified Date: 2020/04/14
*********************************************************************/

#include "fifo.h"

int main(){

    Client_Number = 0;
    Quit_Flag = 0;
    // New_Client_Flag = 0;

    /* Bind the SIGINT signal */
    if(signal(SIGINT, &Server_Sigcatch) == SIG_ERR){
        printf("Couldn't register signal handler\n");
        exit(1);
    }

    /* Create Public FIFO File */
    Create_FIFO(PUBLIC_FIFO);

    while(1){

        if((PublicFd = open(PUBLIC_FIFO, O_RDONLY)) < 0){
            printf("Fail to open PUBLIC_FIFO\n");
            exit(1);
        }

        /* Read the strcut data of the public FIFO */
        if(read(PublicFd, &Client_to_Server, sizeof(struct FIFO_Data)) > 0){

            printf("Client Pid is : %d\n", Client_to_Server.client_pid);
            printf("Client Message is : %s", Client_to_Server.message);
            Private_FIFO_Name = Get_Private_FIFO_Name(Client_to_Server.client_pid);
            
            /* Create the private FIFO for a new client */
            if(strcmp(Client_to_Server.message, IS_NEW_CLIENT) == 0){
                printf("This is a new client!\n");
                New_Client_Flag = 1;
                Create_FIFO(Private_FIFO_Name);
                Store_Private_FIFO_Name();
            }
            
            /* If the client exits, cut off communication */
            if(strcmp(Client_to_Server.message, CLIENT_QUIT) == 0){
                unlink(Private_FIFO_Name);
                Delete_Client_Data();
                Client_Number --;
                Quit_Flag = 1;
                printf("Closed Client_%d Private FIFO\n", Client_to_Server.client_pid);
                printf("Client Number is : %d\n\n", Client_Number);
            }
            
            /* Server send a reply message to the client */
            Server_Send_Message();
            
            
        }
        else{
            printf("Read Date error!\n");
            exit(1);
        }
    }
    
    return 0;

}
