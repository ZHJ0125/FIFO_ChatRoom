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

    Client_PID_Box[Client_Number] = Client_to_Server.client_pid;
    strcpy(Client_Name_Box[Client_Number], Client_to_Server.client_name);
    printf("The %d has been recorded\n", Client_PID_Box[Client_Number]);
    Client_Number ++;
    printf("Client Number is : %d\n", Client_Number);
}

void Server_Send_Message(void){

    int count;
    int target_count;
    int source_count;

    printf("Client Target is: %s\n", Client_to_Server.target_name);

    /* Private message */
    if(strcmp(Client_to_Server.target_name, BROADCAST_TO_ALL) != 0){

        for(target_count = 0; target_count < Client_Number; target_count ++){
            if(strcmp(Client_to_Server.target_name, Client_Name_Box[target_count]) == 0)
            break;
        }
        for(source_count = 0; source_count < Client_Number; source_count ++){
            if(Client_to_Server.client_pid == Client_PID_Box[source_count])
            break;
        }

        printf("Private FIFO Name : %s\n", Get_Private_FIFO_Name(Client_PID_Box[target_count]));
        if((PrivateFd = open(Get_Private_FIFO_Name(Client_PID_Box[target_count]), O_WRONLY)) > 0){
            Server_to_Client.client_pid = Client_to_Server.client_pid;
            sprintf(Server_to_Client.message, "[%s] said to you : ", Client_Name_Box[source_count]);
            strcat(Server_to_Client.message, Client_to_Server.message);
            if(write(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
                printf("Write message to Client_%d Success!\n", Client_PID_Box[target_count]);
                close(PrivateFd);
            }
        }
    }

    /* Public message */
    else{

        if(New_Client_Flag){
            sprintf(Server_to_Client.message, "👦 [%s] 进入聊天室 \n", Client_to_Server.client_name);
        }
        else{
            sprintf(Server_to_Client.message, "[%s] said: ", Client_to_Server.client_name);
            strcat(Server_to_Client.message, Client_to_Server.message);
        }

        for(count = 0; count < Client_Number; count ++){
            /* Skip the data sent by the client */
            // if((Client_PID_Box[count] == Client_to_Server.client_pid) && !New_Client_Flag){
            //     continue;
            // }
            if((PrivateFd = open(Get_Private_FIFO_Name(Client_PID_Box[count]), O_WRONLY)) > 0){
                Server_to_Client.client_pid = Client_to_Server.client_pid;
                if(write(PrivateFd, &Server_to_Client, sizeof(struct FIFO_Data)) > 0){
                    printf("Write message to Client_%d Success!\n", Client_PID_Box[count]);
                    close(PrivateFd);
                }
            }
            usleep(100000);
        }
        New_Client_Flag = 0;
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
            Show_Local_Time();
            printf("%s\n", Server_to_Client.message);
            close(PrivateFd);
        }
    }
    else{
        printf("Fail to open %s\n", Private_FIFO_Name);
        exit(1);
    }
}

void Private_Chat_Filter_By_Name(char* Client_Message){

    int offset;
    char buffer[2];
    char client_name_buffer[MAX_CLIENT_NAME_LEN] = {0};

    if(strncmp(Client_Message, PRIVATE_MSG_HEADER, strlen(PRIVATE_MSG_HEADER)) == 0){
        
        printf("\n");
        offset = strlen(PRIVATE_MSG_HEADER);
        /* Filter spaces of client pid */
        while(*(Client_Message + offset) == ' '){
            offset += 1;
        }
        while(offset < strlen(Client_Message) && (*(Client_Message + offset) != ' ')){
            sprintf(buffer, "%c", *(Client_Message + offset));
            strcat(client_name_buffer, buffer);
            offset ++;
        }
        /* Filter spaces of client message */
        while(*(Client_Message + offset) == ' '){
            offset += 1;
        }
        strcpy(Client_to_Server.message, (Client_Message + offset));
        strcpy(Client_to_Server.target_name, client_name_buffer);
    }
    else{
        //printf("BROADCAST_TO_ALL\n");
        strcpy(Client_to_Server.target_name, BROADCAST_TO_ALL);
        strcpy(Client_to_Server.message, Client_Message);
    }
}

void Show_Local_Time(void){
    time_t tmpcal_ptr;
	struct tm *tmp_ptr = NULL;
    time(&tmpcal_ptr);
    tmp_ptr = localtime(&tmpcal_ptr);
	printf ("%d-%02d-%02d ", (1900+tmp_ptr->tm_year), (1+tmp_ptr->tm_mon), tmp_ptr->tm_mday);
	printf("%02d:%02d:%02d 👇\n", tmp_ptr->tm_hour, tmp_ptr->tm_min, tmp_ptr->tm_sec);
}