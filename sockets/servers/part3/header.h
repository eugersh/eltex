#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_PORT 7000
#define SERVERS 3


typedef struct user{
    int cfd;   //client desc
    int sfd;   //server desc
    short amount_users;      //to check how many users right now on server (processing + waiting)
    short set;
    struct sockaddr_in new_serv_addr;
}user;

typedef struct Node{
    struct Node *next;
    struct sockaddr_in client_addr;
    char buff[20];
}Node;

/* a whole process of processing servers (get task and send to user according to client's info) */
void *server_creating(void *);

/* create node with message and info about client */
void create_node(Node **, struct sockaddr_in *, char *);

/* pop first task (message + info about client) */
Node *pop(Node **);