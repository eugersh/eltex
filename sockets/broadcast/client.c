#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>



int main() {
    int broadcastfd;
    struct sockaddr_in broadcast_serv;
    memset(&broadcast_serv,0, sizeof(broadcast_serv));
    broadcast_serv.sin_family = AF_INET;
    broadcast_serv.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_serv.sin_port = htons(7777);

    broadcastfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == broadcastfd){
        perror("socket");
        exit(1);
    }

    if(bind(broadcastfd, (struct sockaddr*)&broadcast_serv,sizeof(broadcast_serv)) == -1){
        perror("bind");
        exit(1);
    }

    char buff[20];
    while(1) {
        memset(buff, 0, sizeof(buff));
        socklen_t server_length = sizeof(broadcast_serv);
        if (-1 == recvfrom(broadcastfd, buff, sizeof(buff), 0, \
    (struct sockaddr *) &broadcast_serv, &server_length)) {
            perror("recvfrom");
            exit(1);
        }
        printf("%s\n", buff);
    }

    close(broadcastfd);
    return 0;
}
