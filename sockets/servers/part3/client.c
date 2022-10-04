#include "header.h"


int main(){

    int sfd, cfd;
    struct sockaddr_in serv_addr, client_addr, process_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    /* Asking listen server for port to connect a new server */

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket1");
        exit(1);
    }

    int flag = 1;
    if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {
        perror("setsockopt fail");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t server_length = sizeof(serv_addr);
    if(connect(sfd,(struct sockaddr*)&serv_addr,server_length)==-1){
        perror("connect1");
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(client_addr);
    getsockname(sfd, (struct sockaddr *)&client_addr, &addr_size);
    printf("[%s:%u] > \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    /* client address                  client port      */

    char buff[20]="Server???";

    if(send(sfd,buff,sizeof(buff),0)==-1){
        perror("send2");
        exit(EXIT_FAILURE);
    }

    close(sfd);
    /* Ended with listen server */


    /* Communication with new server */
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket2");
        exit(1);
    }
    flag = 1;
    if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {
        perror("setsockopt fail");
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client_addr.sin_port);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sfd, (struct sockaddr *) &client_addr, addr_size) == -1) {
        perror("bind2");
        exit(1);
    }

    if (listen(sfd, 5) == -1) {
        perror("listen2");
        exit(EXIT_FAILURE);
    }

    int pfd; //desc from processing server

    socklen_t ps_size = sizeof(process_addr);
    pfd = accept(sfd, (struct sockaddr *) &process_addr, &ps_size);
    if (pfd == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if(recv(pfd,buff,sizeof(buff),0)==-1){
        perror("recv2");
        exit(EXIT_FAILURE);
    }

    printf("%s\n",buff);



    printf("Bye...\n");
    close(sfd);
    return 0;

}