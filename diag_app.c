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
int CreateServer( const unsigned int iPort )
{
    int ierror = -1;
    int iserverFD = socket( AF_INET, SOCK_STREAM, 0 );
    if( iserverFD < 0 )
    {
        ierror = errno;
        printf( "[Socket::CreateServer] Failed CreateServer errno = %d\n", ierror);
        return ierror;
    }
 
    struct sockaddr_in serverAddr, clientAddr;
    // socklen_t clnt_addr_size;
 
    memset( &serverAddr, 0, sizeof(serverAddr) );
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(iPort);
 
    int option = 1;
    setsockopt( iserverFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) );
    int iret = bind( iserverFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if( iret < 0 )
    {
        ierror = errno;
        printf( "[Socket::CreateServer] Failed bind errno = %d\n", ierror);
        close( iserverFD );
        return ierror;
    }
    
        iret = listen( iserverFD, 5 );
        if( iret < 0 )
        {
            ierror = errno;
            printf( "[Socket::CreateServer] Failed listen errno = %d\n", ierror);
            close( iserverFD );
            return ierror;
        }

    printf("Create Server()!!\n");
    // clnt_addr_size= sizeof(clientAddr);
    // int cliendFD = accept(iserverFD,(struct sockaddr*)&clientAddr,&clnt_addr_size);
    // printf("Client Connected!!\n");
    // int rec_len = 0;
    // char buf[1024];
    // memset(buf,0,sizeof(buf));
    
    // while((rec_len=read(cliendFD,buf,sizeof(buf)))!=0){
    //     printf("length: %d\n",rec_len);
    //     for(int i=0 ; i<rec_len-1; i++){
    //    // printf("received data!!\n");
    //     printf("receive data: %08x\n",buf[i]);
    //     if(buf[i]==0x61){
    //         printf("get session Control message!!\n");
    //     }
    //     }
    //     write(cliendFD,buf,rec_len);
    //     memset(buf,0,sizeof(buf));
    
    // }

    return iserverFD;
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


int main(){
    pthread_t diag_th1;
    pthread_create(&diag_th1, NULL, Run_Diag_Server, NULL);

    pthread_join(diag_th1,NULL);
}

