#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>

#define DEBUG 0
#define input_lenth 2
#define max_lenth 1400

uint8_t Physical_Req_ECU[2] = {0x05,0xB1};
uint8_t Physical_Res_ECU[2] = {0x05,0xB9};

uint8_t Physical_Req_CCU[2] = {0x05,0xD1};
uint8_t Physical_Res_CCU[2] = {0x05,0xD9};

struct sockaddr_in serverAddr;
int ret = -1;

void Start_Menu(){
    printf("------- Diag Message Start -------\n");
    printf("0. press 'q' Ehternet Connect\n");
    printf("1. Press 's' Session Control Message\n");
    printf("2. Press 'a' Security Access Message\n");
    printf("3. Press 'k' SendKey Message\n");
    printf("4. press 'c' Ethernet DisConnect\n");

}

// void* Run_Diag_Server(){
//     struct sockaddr_in clientAddr;
//     socklen_t clnt_addr_size;
//     printf("Server Start!!\n");
//     int port = 9003;
//     int diag_server_FD = CreateServer(port);

//     //Wait for Client & Accpet 
//     clnt_addr_size= sizeof(clientAddr);
//     int cliendFD = accept(diag_server_FD,(struct sockaddr*)&clientAddr,&clnt_addr_size);
//     printf("Client Connected!!\n");
//     int rec_len = 0;
//     char buf[1024];
//     uint8_t res_message[1024];
//     memset(buf,0,sizeof(buf));
//     if(access("server_seokho",0)==-1){
//     printf("Make Directory...\n");
//     system("mkdir server_seokho");
//     }
//     else{

//         printf("Directory Exist!!\n");
//     }
    
//     while((rec_len=read(cliendFD,buf,sizeof(buf)))!=0){
//         printf("length: %d\n",rec_len);
//         for(int i=0 ; i<rec_len-1; i++){
//        // printf("received data!!\n");
//         printf("receive data: %04x\n",buf[i]);
//         if(buf[i]==0x61){
//             printf("Session Control message!!\n");
//         }
//         }
//         #if DEBUG
//         printf("Debug message !! \n");
//         #endif
//         res_message[0]=0xCA;
//         res_message[1]=0xFC;
//         res_message[2]=0xBC;
//         write(cliendFD,res_message,sizeof(char)*3);
//         memset(buf,0,sizeof(buf));
    
//     }

// }

int Create_SOCK_Client (char* ip_adrress, uint32_t port, struct sockaddr_in* serverAddr){
    int clnt_sock;
    clnt_sock = socket(AF_INET,SOCK_STREAM,0);
    if(clnt_sock==-1){
    printf("Client SOCKET Create Fail\n");
    }

    memset(serverAddr,0,sizeof(*serverAddr));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_addr.s_addr = inet_addr(ip_adrress);   
    serverAddr->sin_port=htons(port);
    return clnt_sock;
}

void* Receive_from_Server(void* socket){
    uint8_t res_message[max_lenth];
    uint32_t receive_lenth;
    int sock = *(int*)socket;
    printf("Receive Thread Start\n");
    while((receive_lenth=read(sock,res_message,max_lenth))!=0){
        printf("Respose Message lenth: %d\n",receive_lenth);
        for(int i=0; i<receive_lenth;i++){
            printf("Receive Data: %04x\n",res_message[i]);
        }

    }
}

void Send_Session_Control(int sock){
    uint8_t message[max_lenth];
    int index = 0;
    message[index++] = 0xCA;
    message[index++] = 0xFC;
    message[index++] = 0xBC;
    message[index++] = 0x00;
    message[index++] = 0x00;
    message[index++] = 0x00;
    message[index++] = 0x0A;
    message[index++] = Physical_Req_CCU[0];
    message[index++] = Physical_Req_ECU[1];
    message[index++] = 0x10;
    message[index++] = 0x03;

    write(sock,message,index+1);

}

void* RUN_MENU_AND_CONNECT(void* socket){
    char Key_input[input_lenth];
    int clnt_sock = *(int*)socket;
    // int ret=-1;
    pthread_t thr_read;

    while(1){
    Start_Menu();
    fputs("Input message on Key Board!! \n",stdout);
    fgets(Key_input, input_lenth, stdin);

    switch(Key_input[0]){
        case 'q':
        ret = connect(clnt_sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
        if(ret!=-1){
            printf("Cnnected Sever....\n");
        }
        else{
            printf("Connect() Fail\n");
        }
        pthread_create(&thr_read,NULL,Receive_from_Server,(void*)&clnt_sock);
        break;

        case 's':
        printf("Session Control Start!!\n");
        Send_Session_Control(clnt_sock);
        break;

        case 'c':
        printf("Disconnected Ehtnernet Connection!\n");
        pthread_cancel(thr_read);
        close(clnt_sock);
        break;
    }
  }
  pthread_join(thr_read,NULL);
}

int main(int argc, char* argv[]){
    char Key_input[input_lenth];
    if(argc!=3){
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }
  
    /* server port string -> integer */
    uint32_t serv_port = atoi(argv[2]);
    //struct sockaddr_in serverAddr;
    int sockaddr_len;
    int clnt_sock = Create_SOCK_Client(argv[1],serv_port,&serverAddr);
    // int ret=-1;
    sockaddr_len = sizeof(serverAddr);
    pthread_t thr_read;
    pthread_t thr_menu;
    if(clnt_sock==-1){
        printf("Create_SOCK_Client() fail\n");
    }
    else{
        printf("Create Sock Success\n");
    }

    // if(connect(clnt_sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==-1){
    //     printf("Connect() Fail\n");
    // }
    // else{
    //     printf("Connected Server....\n");
    // }

    // while(1){
    
    // Start_Menu();
    // fputs("Input message on Key Board!! \n",stdout);
    // fgets(Key_input, input_lenth, stdin);

    // switch(Key_input[0]){
    //     case 'q':
    //     ret = connect(clnt_sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    //     if(ret!=-1){
    //         printf("Cnnected Sever....\n");
    //     }
    //     else{
    //         printf("Connect() Fail\n");
    //     }
    //    // pthread_create(&thr_read,NULL,Receive_from_Server,(void*)&clnt_sock);
    //     break;

    //     case 's':
    //     printf("Session Control Start!!\n");
    //     Send_Session_Control(clnt_sock);
    //     break;

    //     case 'c':
    //     printf("Disconnected Ehtnernet Connection!\n");
    //     close(clnt_sock);
    //     pthread_join(thr_read,NULL);
    // }
    pthread_create(&thr_menu,NULL,RUN_MENU_AND_CONNECT,(void*)&clnt_sock);
    
//    if(ret!=-1){
//        pthread_create(&thr_read,NULL,Receive_from_Server,(void*)&clnt_sock);
//     }
    
    pthread_join(thr_menu,NULL);
   // pthread_join(thr_read,NULL);





}

