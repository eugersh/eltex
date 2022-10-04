#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define SERVER_PORT 5000
#define MAX_EVENTS 1000

void tcp_accepted(int);
void udp_accepted(int);



int main() {

    int tcpfd;
    struct sockaddr_in server_tcp;
    memset(&server_tcp, 0, sizeof(server_tcp));

    tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpfd == -1) {
        perror("socket tcp");
        exit(1);
    }

    int flag_tcp = 1;
    if (-1 == setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &flag_tcp, sizeof(flag_tcp))) {
        perror("setsockopt tcp fail");
    }

    server_tcp.sin_family = AF_INET;
    server_tcp.sin_port = htons(SERVER_PORT);
    server_tcp.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(tcpfd, (struct sockaddr *) &server_tcp, sizeof(server_tcp)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(tcpfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int udpfd;
    struct sockaddr_in server_udp;
    memset(&server_udp, 0, sizeof(server_udp));

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpfd == -1) {
        perror("socket udp");
        exit(1);
    }

    server_udp.sin_family = AF_INET;
    server_udp.sin_port = htons(SERVER_PORT);
    server_udp.sin_addr.s_addr = inet_addr("127.0.0.1");

    int flag_udp = 1;
    if (-1 == setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &flag_udp, sizeof(flag_udp))) {
        perror("setsockopt udp fail");
    }

    if (bind(udpfd, (struct sockaddr *) &server_udp, sizeof(server_udp)) == -1) {
        perror("bind udp");
        exit(1);
    }

    struct epoll_event events[MAX_EVENTS];
    int epolllfd = epoll_create(5);
    if (epolllfd < 0){
        perror("epollfd");
        exit(1);
    }

    struct epoll_event event1, event2;
    event1.data.fd = tcpfd;
    event2.data.fd = udpfd;
    event1.events = EPOLLIN;
    event2.events = EPOLLIN;
    epoll_ctl(epolllfd, EPOLL_CTL_ADD, tcpfd, &event1);
    epoll_ctl(epolllfd, EPOLL_CTL_ADD, udpfd, &event2);

    while(1){
        int numb = epoll_wait(epolllfd, events, MAX_EVENTS, -1);
        if (numb < 0){
            perror("epoll_wait");
            exit(1);
        }
        for (int i=0; i<numb; i++){
            int fd = events[i].data.fd;
            if (fd == tcpfd) {
                tcp_accepted(fd);
            }
            else if (fd == udpfd) {
                udp_accepted(fd);
            }
        }
    }


    return 0;
}

void tcp_accepted(int fd){

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd == -1) {
        perror("accept tcp");
        exit(1);
    }
    char buff_tcp[20];
    if(recv(client_fd, buff_tcp, sizeof(buff_tcp), 0) == -1){
        perror("recv tcp");
        return;
    }
    strcat(buff_tcp, "Client???");
    if (send(client_fd, buff_tcp, sizeof(buff_tcp), 0) == -1) {
        perror("send tcp");
        return;
    }
    printf("Tcp ask sent\n");
    close(client_fd);
    /* sleep process to check how it is being worked under multiple clients */
    sleep(1);

}

void udp_accepted(int fd){

    struct sockaddr_in client_addr;
    char buff_udp[30];
    memset(buff_udp, 0, sizeof(buff_udp));
    socklen_t client_length = sizeof(client_addr);
    if (recvfrom(fd, buff_udp, sizeof(buff_udp), 0,\
    (struct sockaddr *)&client_addr, &client_length) == -1){
        perror("recvfrom udp");
    }
    strcat(buff_udp, "Client???");
    if (sendto(fd, buff_udp, sizeof(buff_udp), 0,\
    (struct sockaddr *)&client_addr, client_length) == -1) {
        perror("sendto");
        exit(1);
    }
    printf("Udp ask sent\n");
//    close(fd);
    /* sleep process to check how it is being worked under multiple clients */
    sleep(1);

}


