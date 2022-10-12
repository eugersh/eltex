#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define TEXT_SIZE 20
#define UDP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20
#define SOURCE_PORT 7777
#define DEST_PORT 6000

typedef struct ip_header{
    char version_IHL;           // 69 = [0100 0101. version = [0100] = 4, IHL = [0101] = 5
    char tos;                   // 0 = normal delay, throughput, relibility.
    short total_length;         // 0 = autofill
    short identification;       // 0 = autofill
    short flags_FO;             // 0 = may fragment & last fragment
    unsigned char TTL;          // 255
    char protocol;              // 17 = udp
    short checksum;             // 0 = autofill
    int source_ip;              // 0 = autofill
    int dest_ip;
}ip_header;

void ip_header_fill(ip_header *);


void header_strcat(char *, char *, char *, char *);

int main() {
    int raw_fd;
    struct sockaddr_in raw_struct;
    socklen_t raw_struct_size = sizeof(raw_struct);
    memset(&raw_struct, 0, sizeof(raw_struct));

    raw_fd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    if(raw_fd==-1){
        perror("socket");
        exit(1);
    }

    int flag = 1;
    if(setsockopt(raw_fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag))==-1){
        perror("setsocketopt");
        exit(1);
    }

    raw_struct.sin_family=AF_INET;
    raw_struct.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    raw_struct.sin_port = htons(DEST_PORT);

    char buff[TEXT_SIZE+UDP_HEADER_SIZE+IP_HEADER_SIZE];
    char message[TEXT_SIZE];
    memset(message, 0, sizeof(message));
    strcpy(message, "Server???");

    short header[4];
    header[0] = htons(SOURCE_PORT);      //source port
    header[1] = htons(DEST_PORT);      //dest port
    header[2] = htons(TEXT_SIZE+UDP_HEADER_SIZE);       //length
    header[3] = htons(0);                      //checksum

    ip_header ipHeader;

    ip_header_fill(&ipHeader);

    header_strcat((char *)header, (char *)&ipHeader, buff, message);

    if(sendto(raw_fd,buff,sizeof(buff),0,(struct sockaddr*)&raw_struct,sizeof(raw_struct))==-1){
        perror("sendto");
        exit(1);
    }

    char ans_buff[100];
    while(1){
        if(recvfrom(raw_fd,ans_buff,sizeof(ans_buff),0,\
        (struct sockaddr*)&raw_struct,&raw_struct_size)==-1){
            perror("recvfrom");
        }
        printf("DATA:\n%s\n",ans_buff+28);
    }

    return 0;
}

void ip_header_fill(ip_header *header){
    header->version_IHL = 69;
    header->tos = 0;
    header->total_length = 0;
    header->identification = 0;
    header-> flags_FO = 0;
    header->TTL = 255;
    header->protocol = 17;
    header->checksum = 0;
    header->source_ip = 0;
    header->dest_ip = htonl(INADDR_LOOPBACK);

}

void header_strcat(char *header_udp, char *header_ip, char *buff, char *message){
    memset(buff, 0, sizeof(*buff));

    int j;
    for (j=0; j<IP_HEADER_SIZE; j++){
        buff[j]=header_ip[j];
    }

    for (; j<UDP_HEADER_SIZE+IP_HEADER_SIZE;j++){
        buff[j]=header_udp[j-IP_HEADER_SIZE];
    }

    for (; j<TEXT_SIZE+UDP_HEADER_SIZE+IP_HEADER_SIZE;j++){
        buff[j]=message[j-IP_HEADER_SIZE-UDP_HEADER_SIZE];
    }
}