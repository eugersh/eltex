
#include "helper.h"


void *ctos(void *arg){
    mq buf = *(mq *)arg;
    int msqid;
    key_t key;
    char filename[30];
    char temp_fid[10];

    strcpy(filename,buf.user);
    filename[strlen(filename)-1]='\0';
    sprintf(temp_fid, "%d", buf.fid);
    strcat(filename, temp_fid);

    pid_t pid = fork();
    if(!pid){
        char *args[] = {"touch",filename,NULL};
        execvp(args[0],args);
    }
    else {
        usleep(500000);                   // giving time to executing child process
        key = ftok(filename, PROJECT_ID_CTOS);
        mq ctosbuf;

        ctosbuf.mtype = 1;

        if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
            perror("msgget");
            exit(1);
        }
//        printf("filename = %s, ctos = %d\n", filename, msqid);

        while (1) {
            fgets(ctosbuf.mesg, MSGSZ, stdin);
            size_t buf_length = strlen(ctosbuf.mesg) + 1;
            if ((msgsnd(msqid, &ctosbuf, buf_length, IPC_NOWAIT)) < 0) {
                printf("%d, %ld, %ld, %s\n", msqid, ctosbuf.mtype, buf_length, ctosbuf.mesg);
                perror("msgsnd");
                exit(1);
            }
            /* in case ".exit" just exit(0) program */
            if (0 == (strcmp(ctosbuf.mesg, ".exit\n"))) {
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void *stoc(void *arg){
    mq buf = *(mq *)arg;
    int msqid;
    key_t key;
    char filename[30];
    char temp_fid[10];

    strcpy(filename,buf.user);
    filename[strlen(filename)-1]='\0';
    sprintf(temp_fid, "%d", buf.fid);
    strcat(filename, temp_fid);

    pid_t pid = fork();
    if(!pid){
        char *args[] = {"touch",filename,NULL};
        execvp(args[0],args);
    }
    else {
        usleep(500000);                  // giving time to executing child process
        key = ftok(filename, PROJECT_ID_STOC);
        mq stocbuf;
        stocbuf.mtype = 1;

        if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
            perror("msgget");
            exit(1);
        }
//        printf("filename = %s, stoc = %d\n", filename, msqid);

        stocbuf.msqid_stoc = msqid;

        while (1) {
            if ((msgrcv(stocbuf.msqid_stoc, &stocbuf, MSGSZ, 1, 0)) < 0) {
                perror("msgrcv1");
                exit(1);
            }
            printf("%s\n", stocbuf.mesg);
            memset(stocbuf.mesg, 0, (MSGSZ));
            usleep(100000);          //to not load CPU (0.1 sec)
        }
    }
}

void mqname(mq *buf){
    char temp_fid[10];
    sprintf(temp_fid, "%d", buf->fid);
    strcpy(buf->mesg,temp_fid);
    strcat(buf->mesg, " ");
    strcat(buf->mesg, buf->user);
}


int main() {
    printf("Enter your name\n");

    int msqid;
    key_t key;
    key = ftok(COMMON_MQ, PROJECT_ID);
    mq buf;

    fgets(buf.user, USER_NAME, stdin);
    buf.user[strlen(buf.user)-1]=' ';

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    buf.fid=getpid();
    buf.mtype=1;
    buf.msqid = msqid;

    /* make 1st message to register on server */
    mqname(&buf);

//    printf("%s\n", buf.mesg);

    size_t  buf_length = strlen(buf.mesg) + 1;
    if ((msgsnd(msqid, &buf, buf_length, IPC_NOWAIT)) < 0) {
        printf("%d, %ld, %ld, %s\n", msqid, buf.mtype, buf_length, buf.mesg);
        perror("msgsnd");
        exit(1);
    }
//    printf("Sent: %s\n",buf.mesg);
//    printf("msqid = %d\ttype = %d\n",buf.msqid, buf.mtype);


    pthread_t pt_s;                     //client to server
    pthread_t pt_r;                     //server to client

    pthread_create(&pt_s, NULL, ctos, &buf);
    pthread_create(&pt_r, NULL, stoc, &buf);

    pthread_join(pt_s, NULL);
    pthread_join(pt_r, NULL);

    return 0;
}
