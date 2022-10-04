#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main() {
    int multicastfd;
    struct sockaddr_in multicast_serv;
    memset(&multicast_serv, 0, sizeof(multicast_serv));
    multicast_serv.sin_family = AF_INET;
    multicast_serv.sin_addr.s_addr = inet_addr("224.0.0.1");
    multicast_serv.sin_port = htons(7777);

    multicastfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == multicastfd){
        perror("socket");
        exit(1);
    }
    int flag;
    if(setsockopt(multicastfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1){
        perror("setsockopt");
        exit(1);
    }
    char buff[20] = "hello!!!";

    while(1) {
        if (-1 == sendto(multicastfd, buff, sizeof(buff), 0, \
    (struct sockaddr *) &multicast_serv, sizeof(multicast_serv))) {
            perror("sendto");
        }
        printf("sent to everyone\n");
        sleep(3);
    }

    close(multicastfd);
    return 0;
}
