#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    int sfd;
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1){
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        exit(1);
    }

    char buff[100];
    socklen_t client_length = sizeof(server_addr);
    if (recvfrom(sfd, buff, sizeof(buff), 0,\
    (struct sockaddr *)&client_addr, &client_length) == -1){
        perror("recvfrom");
        exit(1);
    }

    printf("Request from IP: %s and port %d\n",\
    inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    printf("Message: %s\n", buff);

    strcat(buff, " yes");

    if (sendto(sfd, buff, sizeof(buff), 0,\
    (struct sockaddr *)&client_addr, client_length) == -1) {
        perror("sendto");
        exit(1);
    }

    close(sfd);


    return 0;
}
