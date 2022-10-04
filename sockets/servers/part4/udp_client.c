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

    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1){
        perror("socket udp");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buff[20] = "Server???";
    socklen_t server_length = sizeof(serv_addr);
    if(sendto(sfd, buff, strlen(buff), 0,\
    (struct sockaddr *)&serv_addr, server_length) == -1) {
        perror("sendto udp");
        exit(1);
    }

    if(recvfrom(sfd, buff, sizeof(buff), 0,\
    (struct sockaddr *)&serv_addr, &server_length) == -1) {
        perror("recvfrom udp");
        exit(1);
    }

    printf("%s\n",buff);
    printf("Bye...\n");

//    close(sfd);

    return 0;


}