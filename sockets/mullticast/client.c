#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>



int main() {
    struct ip_mreqn multicast_group;
    if(inet_aton("224.0.0.1",&multicast_group.imr_multiaddr) == 0){
        perror("inet_aton");
        exit(1);
    }
    multicast_group.imr_address.s_addr = INADDR_ANY;
    multicast_group.imr_ifindex = 0;

    int multicastfd;
    struct sockaddr_in multicast_serv;
    memset(&multicast_serv,0, sizeof(multicast_serv));
    multicast_serv.sin_family = AF_INET;
    multicast_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    multicast_serv.sin_port = htons(7777);

    multicastfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == multicastfd){
        perror("socket");
        exit(1);
    }

    if(setsockopt(multicastfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&multicast_group,sizeof(multicast_group))==-1){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if(bind(multicastfd, (struct sockaddr*)&multicast_serv,sizeof(multicast_serv)) == -1){
        perror("bind");
        exit(1);
    }

    char buff[20];
    while(1) {
        memset(buff, 0, sizeof(buff));
        socklen_t server_length = sizeof(multicast_serv);
        if (-1 == recvfrom(multicastfd, buff, sizeof(buff), 0, \
    (struct sockaddr *) &multicast_serv, &server_length)) {
            perror("recvfrom");
            exit(1);
        }
        printf("%s\n", buff);
    }

    close(multicastfd);
    return 0;
}
