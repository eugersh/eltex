#include "header.h"



int main() {
    /* Initialize listen server */

    int lfd, cfd;
    struct sockaddr_in listen_server_addr, client_addr;
    memset(&listen_server_addr, 0, sizeof(listen_server_addr));
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(1);
    }
    listen_server_addr.sin_family = AF_INET;
    listen_server_addr.sin_port = htons(SERVER_PORT);
    listen_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(lfd, (struct sockaddr *) &listen_server_addr, sizeof(listen_server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    /* creating processing servers */
    pthread_t pt[SERVERS];
    servers = malloc(sizeof(struct user) * SERVERS);

    for (int i = 0; i < SERVERS; i++) {
        pthread_create(&pt[i], NULL, server_creating, &servers[i]);
    }

    if (listen(lfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* lock until servers are setting up */
    for (int i = 0; i < SERVERS; ) {
        if (1 == servers[i].set) i++;
    }

    for (int i = 0; i < SERVERS; i++) {
        printf("port = %d\n",servers[i].new_serv_addr.sin_port);
    }

    user *chosen_server;
    chosen_server = malloc(sizeof(user));
    short min;

    while (1) {
        socklen_t cl_size = sizeof(client_addr);
        cfd = accept(lfd, (struct sockaddr *) &client_addr, &cl_size);
        if (cfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /* server with min users on it will be chosen as top priority */
        min=10000;
        for (int i=0; i<SERVERS; i++){
            if(servers[i].amount_users < min) {
                min = servers[i].amount_users;
                chosen_server = &servers[i];
            }
        }
        printf("port of chosen server: %d and users in it = %d\n",\
        chosen_server->new_serv_addr.sin_port, chosen_server->amount_users);
        char msg[10];
        sprintf(msg, "%d", chosen_server->new_serv_addr.sin_port);
        if(send(cfd,&msg,sizeof(msg),0)==-1){
            perror("send");
        }
        memset(msg, 0, strlen(msg));
        chosen_server->amount_users++;
    }

    free(chosen_server);
    free(servers);
    for (int i = 0; i < SERVERS; i++) {
        pthread_join(pt[i], NULL);
    }

    return 0;
}

/* Initialize processing server */
void *server_creating(void *arg){
    user *server = (user*)arg;

    struct sockaddr_in client_addr;
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    server->new_serv_addr.sin_family = AF_INET;
    server->new_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sfd = sfd;
    server->amount_users=0;

    /* Finding free port to bind() */
    int i = SERVER_PORT+1;
    int check_bind = -1;
    while (check_bind < 0) {
        server->new_serv_addr.sin_port = htons(i);
        check_bind = bind(server->sfd, (struct sockaddr *) &server->new_serv_addr, sizeof(server->new_serv_addr));
        if (check_bind < 0) {
            printf("busy %d\t", i);
            perror("bind new server");
            i++;
        }
        printf("\n");
    }

    if (listen(server->sfd, 5) == -1) {
        perror("listen");
        pthread_exit(NULL);
    }
    server->set=1;

    while (1) {
        socklen_t cl_size = sizeof(client_addr);
        server->cfd = accept(server->sfd, (struct sockaddr *) &client_addr, &cl_size);
        if (server->cfd == -1) {
            perror("server_accept");
            pthread_exit(NULL);
        }
        process(server);
        printf("from server (portt = %d) user left, now them = %d\n",\
        server->new_serv_addr.sin_port, server->amount_users);
        close(server->cfd);
    }
    close(server->sfd);
}

void process(user *server){
    char buff[20];
    if(recv(server->cfd,buff,sizeof(buff),0)==-1){
        perror("recv");
//        pthread_exit(NULL);
    }
    printf("%s\n",buff);
    strcat(buff, "Client???");

    if(send(server->cfd,buff,sizeof(buff),0)==-1){
        perror("send");
//        pthread_exit(NULL);
    }

    /* give some time to process to track it */
    sleep(5);

    server->amount_users--;
}