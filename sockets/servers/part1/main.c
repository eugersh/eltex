#include "header.h"

void *new_serv(void *);

int main() {

    /* Initialize listen server */

    int lfd, cfd;
    struct sockaddr_in listen_server_addr, client_addr;
    memset(&listen_server_addr, 0, sizeof(listen_server_addr));
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1){
        perror("socket");
        exit(1);
    }
    listen_server_addr.sin_family = AF_INET;
    listen_server_addr.sin_port = htons(SERVER_PORT);
    listen_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(lfd, (struct sockaddr *)&listen_server_addr, sizeof(listen_server_addr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(lfd,5)==-1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1) {
        socklen_t cl_size = sizeof(client_addr);
        cfd = accept(lfd, (struct sockaddr *) &client_addr, &cl_size);
        if (cfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int sfd = socket(AF_INET, SOCK_STREAM, 0);
        printf("desc = %d\n", sfd);

        struct user temp;
        temp.new_serv_addr.sin_family = AF_INET;
        temp.new_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        temp.cfd = cfd;
        temp.sfd = sfd;

        /* Finding free port to bind() */
        int i = SERVER_PORT+1;
        int check_bind = -1;
        while (check_bind < 0) {
            temp.new_serv_addr.sin_port = htons(i);
            check_bind = bind(sfd, (struct sockaddr *) &temp.new_serv_addr, sizeof(temp.new_serv_addr));
            if (check_bind < 0) {
                printf("busy %d\t", i);
                perror("bind new server");
                i++;
            }
        }
        printf("\n");

        char msg[10];
        sprintf(msg, "%d", i);
        if(send(cfd,&msg,sizeof(msg),0)==-1){
            perror("send");
        }

        /* make thread which make server for temp_user */
        pthread_t pt;
        pthread_create(&pt, NULL, new_serv, &temp);
        pthread_join(pt, NULL);
    }


    close(lfd);


    return 0;
}


void *new_serv(void *arg){
    user client = *(user*)arg;
    int new_cfd;

    printf("port = %d\tdesc = %d\n",client.new_serv_addr.sin_port, client.sfd);

    if(listen(client.sfd,5)==-1){
        perror("listen new server");
        exit(EXIT_FAILURE);
    }

    socklen_t cl_size = sizeof(client.new_serv_addr);
    new_cfd = accept(client.sfd, (struct sockaddr *) &client.new_serv_addr, &cl_size);
    if (new_cfd == -1) {
        perror("accept new server");
        exit(EXIT_FAILURE);
    }
    char buff[20];
    if(recv(new_cfd,buff,sizeof(buff),0)==-1){
        perror("recv");
        exit(EXIT_FAILURE);
    }
    printf("%s\n",buff);
    strcat(buff, "Client???");

    if(send(new_cfd,buff,sizeof(buff),0)==-1){
        perror("send");
        exit(EXIT_FAILURE);
    }
    close(new_cfd);
    close(client.sfd);
    close(client.cfd);

}
