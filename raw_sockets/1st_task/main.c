#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>


void display_udp(char *, long);

int main() {
    int raw_fd;
    struct sockaddr_in raw_struct;
    socklen_t raw_struct_size = sizeof(raw_struct);
    memset(&raw_struct, 0, sizeof(raw_struct));

    raw_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (-1 == raw_fd) {
        perror("socket");
        exit(1);
    }
    char buff[100];
    long num_bytes;

    while (1) {
        num_bytes = recvfrom(raw_fd, buff, sizeof(buff), 0,
        (struct sockaddr *) &raw_struct, &raw_struct_size);
        if (num_bytes < 0){
            perror("recvfrom");
        }
        display_udp(buff, num_bytes);
    }
    close(raw_fd);
    return 0;
}

void display_udp(char *buff, long n){
    char header[28];
    memset(header,0,28);
    for (int i=0; i<28; i++){
        strcpy(&header[i],buff+i);
    }
    short header2[14];
    char buffer[2];
    for (int j=0; j<14; j++){
        if (header[j*2] == NULL && header[(j*2)+1] != NULL) {
            strcpy(&buffer[0],&header[(j*2)+1]);
            strcpy(&buffer[1],"\0");
        }
        else {
            strcpy(&buffer[0], &header[j * 2]);
            strcpy(&buffer[1], &header[(j * 2) + 1]);
        }
        strcpy(&header2[j],buffer);
        memset(buffer,0,2);
    }
    printf("_______________________________\n");
    printf("Source Port: %d\n",ntohs(header2[10]));
    printf("Destination Port %d\n",ntohs(header2[11]));
    printf("Length: %d\n",(header2[12]));
    printf("Checksum: %d\n",ntohs(header2[13]));
    printf("Data (%ld bytes)\n%s\n", n-28, buff+28);
    printf("_______________________________\n");
}

