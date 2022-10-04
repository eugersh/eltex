#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    char *sock_path = "localsocket";

    struct sockaddr_un serv;
    int sfd;

    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket server");
        exit(1);
    }
    memset(&serv, 0, sizeof(struct sockaddr_un));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, sock_path, sizeof(serv.sun_path) - 1);

    if(connect(sfd, (struct sockaddr *) &serv, sizeof(struct sockaddr_un)) == -1){
        perror("connect");
        exit(1);
    }

    char buff[20]="ping server";

    if(send(sfd, buff, sizeof(buff), 0) == -1){
        perror("send");
        exit(1);
    }
    if(recv(sfd, buff, sizeof(buff), 0) == -1){
        perror("recv");
        exit(1);
    }
    printf("%s\n", buff);

    close(sfd);


    return 0;
}
