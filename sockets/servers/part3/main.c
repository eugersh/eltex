#include "header.h"

Node *root = NULL;
pthread_mutex_t mutex;

int main() {
    /* Initialize listen server */

    pthread_mutex_init(&mutex, 0);

    int lfd, cfd;
    struct sockaddr_in listen_server_addr, client_addr;
    memset(&listen_server_addr, 0, sizeof(listen_server_addr));

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(1);
    }

    int flag = 1;
    if (-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {
        perror("setsockopt fail");
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
    for (int i = 0; i < SERVERS; i++) {
        pthread_create(&pt[i], NULL, server_creating, NULL);
    }

    if (listen(lfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t cl_size = sizeof(client_addr);
        cfd = accept(lfd, (struct sockaddr *) &client_addr, &cl_size);
        if (cfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char buff[20];
        if(recv(cfd,buff,sizeof(buff),0)==-1){
            perror("recv2");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutex);
        create_node(&root, &client_addr, buff);
        pthread_mutex_unlock(&mutex);

        close(cfd);
    }

    /* ------------------------------free allocated memory------------------------------ */
    free(root);
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < SERVERS; i++) {
        pthread_join(pt[i], NULL);
    }

    return 0;
}

/* Initialize processing server */

void *server_creating(void *arg){
    int sfd;
    socklen_t client_length;
    struct sockaddr_in client_addr;
    Node *task_node;

    while (1) {
        /* check linked list (tasks) is empty or not? */
        pthread_mutex_lock(&mutex);
        /* no = unlock and sleep 0.1 sec to not load cpu */
        if (root == NULL){
            pthread_mutex_unlock(&mutex);
            usleep(100000);
            continue;
        }
        /* yes = pop first task (oldest) and unlock */
        else {
            task_node = pop(&root);
            pthread_mutex_unlock(&mutex);
        }

        sfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sfd == -1){
            perror("socket1");
            exit(1);
        }

        int flag = 1;
        if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {
            perror("setsockopt fail");
        }

        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(task_node->client_addr.sin_port);
        client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(task_node->client_addr.sin_addr));

        printf("%s:%u\t\t=\t", inet_ntoa(client_addr.sin_addr),
               client_addr.sin_port);
        printf("%s\n", task_node->buff);

        client_length = sizeof(client_addr);
        if (connect(sfd, (struct sockaddr *) &client_addr, client_length) == -1) {
            perror("connect3-5");
//            exit(EXIT_FAILURE);
        }
        strcat(task_node->buff, "Client???");
        if (send(sfd, task_node->buff, sizeof(task_node->buff), 0) == -1) {
            perror("send3-5");
//            exit(EXIT_FAILURE);
        }
        printf("Ask sent\n");
        close(sfd);
        /* sleep process to check how it is being worked under multiple clients */
        sleep(5);
    }
    free(task_node);
}


void create_node(Node **head, struct sockaddr_in *client_addr, char *buff){
    Node *tmp = (Node *) malloc(sizeof(Node));
    strcpy(tmp->buff, buff);
    tmp->client_addr = *client_addr;
    tmp->next = NULL;
    if (*head == NULL) {
        *head = tmp;
    } else{
        Node *iterator;
        iterator = *head;
        while (iterator->next){
            iterator = iterator->next;
        }
        iterator->next=tmp;
    }
}

Node *pop(Node **head){
    Node *prev = NULL;
    if (head == NULL)
        return prev;
    prev = (*head);
    (*head)=(*head)->next;
    return prev;
}