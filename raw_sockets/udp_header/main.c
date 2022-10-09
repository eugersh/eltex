#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TEXT_SIZE 20
#define UDP_HEADER_SIZE 8

void header_strcat(char *, char *, char *);

int main() {
    int raw_fd;
    struct sockaddr_in raw_struct;
    socklen_t raw_struct_size = sizeof(raw_struct);
    memset(&raw_struct, 0, sizeof(raw_struct));

    raw_fd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    if(raw_fd==-1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    raw_struct.sin_family=AF_INET;
    raw_struct.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    raw_struct.sin_port = htons(5000);

    char buff[TEXT_SIZE+UDP_HEADER_SIZE];
    char message[TEXT_SIZE];
    memset(message, 0, sizeof(message));
    strcpy(message, "Server???");

    short header[4];
    header[0] = htons(7777);      //source port
    header[1] = htons(5000);      //dest port
    header[2] = htons(TEXT_SIZE+UDP_HEADER_SIZE);       //length
    header[3] = htons(0);                      //checksum

    header_strcat((char *)header,buff, message);

    if(sendto(raw_fd,buff,sizeof(buff),0,(struct sockaddr*)&raw_struct,sizeof(raw_struct))==-1){
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    char ans_buff[100];
    while(1){
        if(recvfrom(raw_fd,ans_buff,sizeof(ans_buff),0,\
        (struct sockaddr*)&raw_struct,&raw_struct_size)==-1){
            perror("recvfrom");
        }
        printf("%s\n",ans_buff+28);
    }

    return 0;
}

void header_strcat(char *header, char *buff, char *message){
    memset(buff, 0, sizeof(*buff));

    for (int j=0; j<UDP_HEADER_SIZE;j++){
        buff[j]=header[j];
    }

    for (int j=UDP_HEADER_SIZE; j<TEXT_SIZE+UDP_HEADER_SIZE;j++){
        buff[j]=message[j-UDP_HEADER_SIZE];
    }
//    printf("_________________\nheader:\n");
//    for (int i=0; i<UDP_HEADER_SIZE+TEXT_SIZE;i++){
//        if (i==8) printf("*******\nData\n");
//        printf("%d\n",buff[i]);
//    }
}