#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>

#define TEXT_SIZE 20
#define UDP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20
#define ETH_HEADER_SIZE 14
#define SOURCE_PORT 7777
#define DEST_PORT 6000

typedef struct link_header{
    char dest_mac[6];
    char source_mac[6];
    short type;
}link_header;

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

void link_header_fill(link_header *);

void ip_header_fill(ip_header *);

void header_strcat(char *, char *, char *, char *, char *);

int main() {
    int raw_fd;
    struct sockaddr_ll raw_struct;
    socklen_t raw_struct_size = sizeof(raw_struct);
    memset(&raw_struct, 0, sizeof(raw_struct));

    raw_fd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if(raw_fd==-1){
        perror("socket");
        exit(1);
    }

//    int flag = 1;
//    if(setsockopt(raw_fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag))==-1){
//        perror("setsocketopt");
//        exit(1);
//    }

    raw_struct.sll_family=AF_PACKET;
    raw_struct.sll_ifindex = if_nametoindex("enp3s0");
    if(raw_struct.sll_ifindex==0){
        perror("if_nametoindex");
        exit(EXIT_FAILURE);
    }
    raw_struct.sll_halen=6;

    raw_struct.sll_addr[0]=0x08;
    raw_struct.sll_addr[1]=0x00;
    raw_struct.sll_addr[2]=0x27;
    raw_struct.sll_addr[3]=0xe8;
    raw_struct.sll_addr[4]=0x52;
    raw_struct.sll_addr[5]=0xae;

    char buff[TEXT_SIZE+UDP_HEADER_SIZE+IP_HEADER_SIZE+ETH_HEADER_SIZE];
    char message[TEXT_SIZE];
    memset(message, 0, sizeof(message));
    strcpy(message, "Server???");

    short header[4];
    header[0] = htons(SOURCE_PORT);      //source port
    header[1] = htons(DEST_PORT);      //dest port
    header[2] = htons(TEXT_SIZE+UDP_HEADER_SIZE);       //length
    header[3] = 0;                               //checksum

    link_header linkHeader;
    ip_header ipHeader;

    link_header_fill(&linkHeader);
    ip_header_fill(&ipHeader);

    header_strcat((char *)&linkHeader, (char *)header, (char *)&ipHeader, buff, message);

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
        short *dest_port = (short *)(ans_buff+36);
        if(htons(*dest_port)==7777){
            printf("DATA:\n%s\n",ans_buff+42);
        }
    }

    return 0;
}

void link_header_fill(link_header *header){
    header->dest_mac[0]=0x08;
    header->dest_mac[1]=0x00;
    header->dest_mac[2]=0x27;
    header->dest_mac[3]=0xe8;
    header->dest_mac[4]=0x52;
    header->dest_mac[5]=0xae;

    header->source_mac[0]=0x30;
    header->source_mac[1]=0x5a;
    header->source_mac[2]=0x3a;
    header->source_mac[3]=0x7e;
    header->source_mac[4]=0x95;
    header->source_mac[5]=0x7a;

    header->type=htons(0x0800);
}

short CRC16(char* buf){

    unsigned int csum=0;
    unsigned short* ptr=(short*)buf;
    for(int i=0;i<10;i++){
        csum+=*ptr;
//        printf("%i: %x\n", i+1, csum);
        ptr++;
    }
    while((csum>>16)!=0){
        short tmp=csum>>16;
        csum=(csum&0xFFFF)+tmp;
    }
    csum=~csum;
//    printf("\nSum = %d\n",csum);

    return csum;
}

void ip_header_fill(ip_header *header){
    memset(header,0, sizeof(*header));
    header->version_IHL = 69;
    header->tos = 0;
    header->total_length = htons(IP_HEADER_SIZE+UDP_HEADER_SIZE+TEXT_SIZE);
    header->identification = 10;
    header-> flags_FO = 0;
    header->TTL = 255;
    header->protocol = 17;
    header->source_ip = inet_addr("192.168.0.105");
    header->dest_ip = inet_addr("192.168.0.9");
    header->checksum = 0;
    header->checksum = CRC16((char *) header);


}

void header_strcat(char *link_header, char *header_udp, char *header_ip, char *buff, char *message){
    memset(buff, 0, sizeof(*buff));

    int j;
    for (j=0; j<ETH_HEADER_SIZE; j++){
        buff[j]=link_header[j];
    }

    for (; j<IP_HEADER_SIZE+ETH_HEADER_SIZE; j++){
        buff[j]=header_ip[j-ETH_HEADER_SIZE];
    }

    for (; j<UDP_HEADER_SIZE+IP_HEADER_SIZE+ETH_HEADER_SIZE;j++){
        buff[j]=header_udp[j-IP_HEADER_SIZE-ETH_HEADER_SIZE];
    }

    for (; j<TEXT_SIZE+UDP_HEADER_SIZE+IP_HEADER_SIZE+ETH_HEADER_SIZE;j++){
        buff[j]=message[j-IP_HEADER_SIZE-UDP_HEADER_SIZE-ETH_HEADER_SIZE];
    }

//    for (int i=ETH_HEADER_SIZE; i<ETH_HEADER_SIZE+IP_HEADER_SIZE; i++){
//        printf("%x\n",buff[i]);
//    }
}
