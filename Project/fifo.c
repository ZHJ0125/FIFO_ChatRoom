/********************************************************************
*   File Name: fifo.c
*   Description: This program contains API interface for FIFO function.
*   Author: Zhang Houjin
*   Date: 2020/04/06
*********************************************************************/

#include "fifo.h"

/********************************************************************
*   Function Name: void Create_FIFO(char *FIFO_Name)
*   Description: Create a FIFO file based on the string "FIFO_Name".
*   Called By: server.c[main]
*   Input: char *FIFO_Name -> Use to represent FIFO file name
*   Author: Zhang Houjin
*   Date: 2020/04/05
*********************************************************************/
void Create_FIFO(char *FIFO_Name){

    int TempFd;

    if((TempFd = open(FIFO_Name, O_RDONLY)) == -1){
        umask(0);
        mknod(FIFO_Name, S_IFIFO|0666, 0);
        printf("%s has been bulid\n", FIFO_Name);
    }
    else{
        close(TempFd);
    }

}

/********************************************************************
*   Function Name: char* Get_Private_FIFO_Name(int Client_PID)
*   Description: Generate FIFO file name based on Client PID.
*   Called By: server.c[main] client.c[main]
*   Input: int Client_PID -> Used to identify the client
*   Author: Zhang Houjin
*   Date: 2020/04/05
*********************************************************************/
char* Get_Private_FIFO_Name(int Client_PID){

    char TempBuffer[6];

    strcpy(Private_Name, PRIVATE_FIFO);
    sprintf(TempBuffer, "%d", Client_PID);
    //printf("TempBuffer is : %s\n", TempBuffer);
    strcat(Private_Name, TempBuffer);
    return Private_Name;

}

/********************************************************************
*   Function Name: void sigcatch(int num)
*   Description: Register SIGINT signal processing function.
*   Called By: server.c[main]
*   Input: int num -> Signal value
*   Author: Zhang Houjin
*   Date: 2020/04/05
*********************************************************************/
void sigcatch(int num){

    printf("\nServer is exiting...\n");
    unlink(PUBLIC_FIFO);
    printf("Removed %s\nSee you again 😉\n\n", PUBLIC_FIFO);
    exit(0);

}

/********************************************************************
*   Function Name: void Send_and_Recive_Message(void)
*   Description: Receive and send data between client and server.
*   Called By: client.c[main]
*   Author: Zhang Houjin
*   Date: 2020/04/05
*********************************************************************/
void Send_and_Recive_Message(void){
    
    /* Open PUBLIC_FIFO with Write-Only mode */
    if((PublicFd = open(PUBLIC_FIFO, O_WRONLY)) > 0){

        /* Write struct date from Client to Server */
        if(write(PublicFd, &Client_to_Server, sizeof(struct FIFO_Data)) > 0){
            //printf("Success to write client_%d message to server!\n", getpid());
            close(PublicFd);
        }
        else{
            printf("Fail to write client data\n");
        }
        usleep(200000);

        /* Client exits communication */
        if(strcmp(Client_to_Server.message, CLIENT_QUIT) == 0){
            printf("Client_%d exit\n", Client_to_Server.client_pid);
            printf("Removed %s\n", Private_FIFO_Name);
            exit(0);
        }

        /* Read server struct data */
        if((PrivateFd = open(Private_FIFO_Name, O_RDONLY)) > 0){
            if(read(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
                printf("Recive from Server: 👇\n%s\n", Server_to_Client.message);
                close(PrivateFd);
            }
        }
    }
    else{
        printf("Fail to open PUBLIC_FIFO\n");
        exit(1);
    }

}
