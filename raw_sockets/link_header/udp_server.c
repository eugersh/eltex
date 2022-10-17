#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOURCE_PORT 6000

int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SOURCE_PORT);
    serv_addr.sin_addr.s_addr=inet_addr("192.168.0.9");

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        perror("socket");
        exit(1);
    }

    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){
        perror("bind");
        exit(1);
    }
    struct sockaddr_in client;
    socklen_t cl_size=sizeof(client);
    char buff[20];
    if(recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&client,&cl_size)==-1){
        perror("recvfrom");
        exit(1);
    }
    printf("%s\n",buff);
    strcat(buff,"Client???");
    if(sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&client,cl_size)==-1){
        perror("sendto");
        exit(1);
    }
    close(sockfd);
    exit(0);
}