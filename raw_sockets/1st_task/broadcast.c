#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>


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
    int flag;
    if(setsockopt(broadcastfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1){
        perror("setsockopt");
        exit(1);
    }
    char buff[20] = "hello!!!";

    while(1) {
        if (-1 == sendto(broadcastfd, buff, sizeof(buff), 0, \
    (struct sockaddr *) &broadcast_serv, sizeof(broadcast_serv))) {
            perror("sendto");
        }
        printf("sent to everyone\n");
    sleep(3);
    }

    close(broadcastfd);
    return 0;
}
