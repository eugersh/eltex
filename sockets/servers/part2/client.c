#include "header.h"


int main(){

    int sfd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    /* Asking listen server for port to connect a new server */

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket1");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t server_length = sizeof(serv_addr);
    if(connect(sfd,(struct sockaddr*)&serv_addr,server_length)==-1){
        perror("connect1");
        exit(EXIT_FAILURE);
    }

    char str[10];

    while (recv(sfd,str,sizeof(str),0)==-1){
        perror("recv1");
        usleep(100000); //not to load CPU
    }

    serv_addr.sin_port = atoi(str);
    /* Ended with listen server */
    close(sfd);

    /* Communication with new server */
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket2");
        exit(1);
    }
    printf("%d\n",serv_addr.sin_port);
    while(connect(sfd,(struct sockaddr*)&serv_addr,server_length)==-1){
        perror("connect2");
        usleep(100000);
    }
    char buff[20]="Server???";
    if(send(sfd,buff,sizeof(buff),0)==-1){
        perror("send2");
        exit(EXIT_FAILURE);
    }
    printf("%d\n",serv_addr.sin_port);

    if(recv(sfd,buff,sizeof(buff),0)==-1){
        perror("recv2");
        exit(EXIT_FAILURE);
    }
    printf("%s\n",buff);


    printf("Bye...\n");
    close(sfd);
    return 0;

}