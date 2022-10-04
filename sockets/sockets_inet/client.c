#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int sfd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1){
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(2000);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buff[100] = "server is alive?";
    socklen_t server_length = sizeof(serv_addr);
    if(sendto(sfd, buff, strlen(buff), 0,\
    (struct sockaddr *)&serv_addr, server_length) == -1) {
        perror("sendto");
        exit(1);
    }

    if(recvfrom(sfd, buff, sizeof(buff), 0,\
    (struct sockaddr *)&serv_addr, &server_length) == -1) {
        perror("recvfrom");
        exit(1);
    }
    printf("Message from IP: %s and port %d\n",\
    inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port));
    printf("Message from server: %s\n", buff);

    close(sfd);
    return 0;

}