#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>


int main() {

    char *sock_path = "localsocket";

    int sfd, cfd;
    struct sockaddr_un serv, client;
    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket server");
        exit(1);
    }
    memset(&serv, 0, sizeof(struct sockaddr_un));
    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, sock_path, sizeof(serv.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &serv, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(1);
    }
     if (listen(sfd, 5) == -1){
         perror("listen");
         exit(1);
     }

     socklen_t client_size = sizeof(struct sockaddr_un);
     cfd = accept(sfd, (struct sockaddr *) &client, &client_size);
    if (cfd == -1){
        perror("accept client");
        exit(1);
    }

    char buff[20];
    if(recv(cfd, buff, sizeof(buff),0) == -1){
        perror("recv");
        exit(1);
    }
    strcat(buff," answer");
    printf("%s\n",buff);

    if(send(cfd, buff, sizeof(buff), 0) == -1){
        perror("send");
        exit(1);
    }

    close(cfd);
    close(sfd);
    unlink(sock_path);

    return 0;
}
