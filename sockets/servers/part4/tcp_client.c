#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>

#define SERVER_PORT 5000


int main(){

    int sfd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    /* Asking listen server for port to connect a new server */

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket tcp");
        exit(1);
    }

    int flag = 1;
    if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {
        perror("setsockopt tcp");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t server_length = sizeof(serv_addr);
    if(connect(sfd,(struct sockaddr*)&serv_addr,server_length)==-1){
        perror("connect tcp");
        exit(EXIT_FAILURE);
    }

    char buff[20]="Server???";

    if(send(sfd,buff,sizeof(buff),0)==-1){
        perror("send tcp");
        exit(EXIT_FAILURE);
    }

    if(recv(sfd,buff,sizeof(buff),0)==-1){
        perror("recv tcp");
        exit(EXIT_FAILURE);
    }

    printf("%s\n",buff);
    printf("Bye...\n");

    close(sfd);

    return 0;

}