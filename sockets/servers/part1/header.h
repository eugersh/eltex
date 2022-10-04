#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 2000


typedef struct user{
    int cfd;   //client desc
    int sfd;   //server desc
    struct sockaddr_in new_serv_addr;
}user;