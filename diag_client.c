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

void Start_Menu(){
    printf("------- Diag Message Start -------\n");
    printf("1. Press 's' Session Control Message\n");
    printf("2. Press 'a' Security Access Message\n");
    printf("3. Press 'k' SendKey Message\n");
    printf("4. press 'c' Ethernet DisConnect\n");

}

void* Run_Diag_Server(){
    struct sockaddr_in clientAddr;
    socklen_t clnt_addr_size;
    printf("Server Start!!\n");
    int port = 9003;
    int diag_server_FD = CreateServer(port);

    //Wait for Client & Accpet 
    clnt_addr_size= sizeof(clientAddr);
    int cliendFD = accept(diag_server_FD,(struct sockaddr*)&clientAddr,&clnt_addr_size);
    printf("Client Connected!!\n");
    int rec_len = 0;
    char buf[1024];
    uint8_t res_message[1024];
    memset(buf,0,sizeof(buf));
    if(access("server_seokho",0)==-1){
    printf("Make Directory...\n");
    system("mkdir server_seokho");
    }
    else{

        printf("Directory Exist!!\n");
    }
    
    while((rec_len=read(cliendFD,buf,sizeof(buf)))!=0){
        printf("length: %d\n",rec_len);
        for(int i=0 ; i<rec_len-1; i++){
       // printf("received data!!\n");
        printf("receive data: %04x\n",buf[i]);
        if(buf[i]==0x61){
            printf("Session Control message!!\n");
        }
        }
        #if DEBUG
        printf("Debug message !! \n");
        #endif
        res_message[0]=0xCA;
        res_message[1]=0xFC;
        res_message[2]=0xBC;
        write(cliendFD,res_message,sizeof(char)*3);
        memset(buf,0,sizeof(buf));
    
    }

}

int Create_SOCK_Client (uint8_t* ip_adrress, uint32_t port){
    int clnt_sock;
    clnt_sock = socket(AF_INET,SOCK_STREAM,0);
    if(clnt_sock==-1){
    printf("Client SOCKET Create Fail\n");
    }
    return clnt_sock;
}

int main(int argc, char* argv[]){
   
    if(argc!=3){
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }



}

