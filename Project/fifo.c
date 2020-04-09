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

void Store_Private_FIFO_Name(void){
    //Client_Number ++;
    //strcpy(Private_FIFO_Box[Client_Number], Private_FIFO_Name);
    Client_PID_Box[Client_Number] = Client_to_Server.client_pid;
    printf("The %d has been recorded\n", Client_PID_Box[Client_Number]);
    Client_Number ++;
    printf("Client Number is : %d\n", Client_Number);
}

void Server_Send_Message(void){

    int count;

    /* Private message */
    if(Client_to_Server.private_pid > 0){
        printf("Private FIFO Name : %s\n", Get_Private_FIFO_Name(Client_to_Server.private_pid));
        if((PrivateFd = open(Get_Private_FIFO_Name(Client_to_Server.private_pid), O_WRONLY)) > 0){
            Server_to_Client.client_pid = Client_to_Server.client_pid;
            sprintf(Server_to_Client.message, "Client_%d said to you : ", Client_to_Server.client_pid);
            strcat(Server_to_Client.message, Client_to_Server.message);
            if(write(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
                printf("Write message to Client_%d Success!\n", Client_to_Server.private_pid);
                close(PrivateFd);
            }
        }
    }
    /* Public message */
    else{
        for(count = 0; count < Client_Number; count ++){
            /* Skip the data sent by the client */
            // if((Client_PID_Box[count] == Client_to_Server.client_pid) && !New_Client_Flag){
            //     continue;
            // }
            if((PrivateFd = open(Get_Private_FIFO_Name(Client_PID_Box[count]), O_WRONLY)) > 0){
                Server_to_Client.client_pid = Client_to_Server.client_pid;
                sprintf(Server_to_Client.message, "Client_%d said : ", Client_to_Server.client_pid);
                strcat(Server_to_Client.message, Client_to_Server.message);
                if(write(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
                    printf("Write message to Client_%d Success!\n", Client_PID_Box[count]);
                    close(PrivateFd);
                }
            }
            usleep(100000);
        }
        // New_Client_Flag = 0;
    }
    
    printf("\n");

}

void Client_Write_Data(int Process, int Child_PID){

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
            /* When the parent process calls the function, kill the child process */
            if(Process){
                kill(Child_PID, SIGSTOP);
                usleep(1000);
                printf("Child process has been stoped\n");
            }
            exit(0);
        }
    }
    else{
        printf("Fail to open PUBLIC_FIFO\n");
        exit(1);
    }
}

void Client_Read_Data(void){
    /* Read server struct data */
    if((PrivateFd = open(Private_FIFO_Name, O_RDONLY)) > 0){
        if(read(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
            printf("Recive from Server: 👇\n%s\n", Server_to_Client.message);
            close(PrivateFd);
        }
    }
    else{
        printf("Fail to open %s\n", Private_FIFO_Name);
        exit(1);
    }
}

void Private_Chat_Filter(char* Client_Message){

    int offset;
    int pid_num;
    int count = 0;      // Control PID length
    char buffer[2] = {0};
    char pid_num_string[5] = {0};

    // printf("Client Message : %s\n", Client_Message);

    if(strncmp(Client_Message, PRIVATE_MSG_HEADER, strlen(PRIVATE_MSG_HEADER)) == 0){
        offset = strlen(PRIVATE_MSG_HEADER);
        /* Filter spaces of client pid */
        while(*(Client_Message + offset) == ' '){
            offset += 1;
        }
        while((offset < strlen(Client_Message)) && (count < 5)){
            // printf("%c\n", *(Client_Message + offset));
            // printf("Entry while\n");
            if((*(Client_Message + offset) >= '0') && (*(Client_Message + offset) <= '9')){
                sprintf(buffer, "%c", *(Client_Message + offset));
                strcat(pid_num_string, buffer);
                offset ++;
                count ++;
            }
            else{
                break;
            }
        }
        /* Filter spaces in messages */
        while(*(Client_Message + offset) == ' '){
            offset += 1;
        }
        // printf("pid_num_string is : %s\n", pid_num_string);
        pid_num = atoi(pid_num_string);
        printf("Private send to client_%d\n", pid_num);
        printf("Private Message : %s\n", (Client_Message + offset));

        /* Intercept valid data of private messages */
        strcpy(Client_to_Server.message, (Client_Message + offset));
    }
    else{
        printf("Send mseeage to all clients\n");
        pid_num = -1;
    }
    Client_to_Server.private_pid = pid_num;

}
