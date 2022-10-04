#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 5000
#define SERVERS 3

typedef struct user{
    int cfd;   //client desc
    int sfd;   //server desc
    short amount_users;      //to check how many users right now on server (processing + waiting)
    short set;
    struct sockaddr_in new_serv_addr;
    }user;

user *servers;

void *server_creating(void *);

void process(user *);