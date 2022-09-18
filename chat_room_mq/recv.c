#include "helper.h"

/* splitting into fid and username separately */
void split(char *text, mq *temp_user){
    char numb_seq[10];
    char char_seq[20];
    int len_of_fid=0;
    for (int i=0; i< strlen(text); i++){
        if(text[i]==' ' && isdigit(text[i-1])){
            len_of_fid++;
            continue;
        }
        if(isdigit(text[i])) {
            numb_seq[i] = text[i];
            len_of_fid++;
        }
        else char_seq[i-len_of_fid]=text[i];
    }
    int number = atoi(numb_seq);
    char_seq[strlen(char_seq)-1]='\0';
    printf("fid = %d\tname = %s\n",number,char_seq);
    temp_user->fid = number;
    strcpy(temp_user->user,char_seq);
}

/* initialization of temporary user struct */
void init_temp_user(mq *temp_user, mq *buf, char *filename){
    char temp_fid[10];
    split(buf->mesg, temp_user);

    strcpy(filename,temp_user->user);
    sprintf(temp_fid, "%d", temp_user->fid);
    strcat(filename,temp_fid);
}

void mq_creating(char *filename, mq *temp_user){
    int msqid_ctos, msqid_stoc;
    key_t key_ctos, key_stoc;
    key_ctos = ftok(filename, PROJECT_ID_CTOS);
    key_stoc = ftok(filename, PROJECT_ID_STOC);
    printf("%d\t%d\n", key_ctos, key_stoc);
    if ((msqid_ctos = msgget(key_ctos, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    if ((msqid_stoc = msgget(key_stoc, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    temp_user->msqid_ctos = msqid_ctos;
    temp_user->msqid_stoc = msqid_stoc;
    printf("CTOS = %d\tSTOC = %d\n",msqid_ctos, msqid_stoc);
    temp_user->mtype = 1;
    temp_user->exist = 1;

    int i;
    for (i=0; i<USERS_INCHAT; i++){
        if(0 == users[i].exist) {
            users[i] = *temp_user;
            break;
        }
    }
    printf("user: %s, fid: %d, msqid_stoc = %d, msqid_ctos = %d, under pos = %d\n", \
    users[i].user, users[i].fid, users[i].msqid_stoc, users[i].msqid_ctos, i);
    users_in_chat++;
    printf("USERS IN CHAT = %d\n", users_in_chat);

}

void info_message(){
    char whoishere[200];
    sprintf(whoishere, "Users in chat = %d:\n", users_in_chat);
    for (int i=0; i<USERS_INCHAT; i++){
        if (users[i].exist) {
            strcat(whoishere,users[i].user);
            strcat(whoishere," ");
        }
    }
    mq informer;
    informer.mtype=1;
    strcpy(informer.mesg, whoishere);
    printf("%s\n",whoishere);
    for (int j=0; j<USERS_INCHAT; j++){
        if (users[j].exist) {
//            printf("user %s has msqid_stoc = %d\n",users[j].user, users[j].msqid_stoc);
            if ((msgsnd(users[j].msqid_stoc, &informer, strlen(informer.mesg)+1, IPC_NOWAIT))<0) {
                printf("%d, %ld, %d, %s\n", \
                            users[j].msqid_stoc, users[j].mtype, 200, users[j].mesg);
                perror("msgsnd2");
                exit(1);
            }
        }
    }
}

 /* Func for creating a dedicated mqs(from/to server) for certain user */
void *entering (void *arg){
    mq buf = *(mq *)arg;
    users = malloc(USERS_INCHAT * sizeof(mq));
    printf("msqid = %d\n", buf.msqid);

    while(1){

        if ((msgrcv(buf.msqid, &buf, MSGSZ, 1, 0)) < 0) {
            perror("msgrcv");
            exit(1);
        }
        printf("%s\n", buf.mesg);

        mq temp_user;
        char filename[30];

        init_temp_user(&temp_user, &buf, filename);

        int pid = fork();
        if(!pid){
            char *args[] = {"touch",filename,NULL};
            execvp(args[0],args);
        }
        sleep(1);                   // giving time to executing child process
        mq_creating(filename, &temp_user);
        info_message();

    }
}


 /* exit from chat */
void exit_from_chat(mq *user){
    for (int i=0; i<USERS_INCHAT; i++){
        if ((!strcmp(user->user,users[i].user)) && (user->fid == users[i].fid)){
            msgctl(user->msqid_stoc, IPC_RMID, NULL);
            msgctl(user->msqid_ctos, IPC_RMID, NULL);
        }
    }
    /* if exist == 0, then user[i] can be rewritten, no need to free(), I guess...
     *                       only delete file for msgget()                          */
    user->exist = 0;


    char filename[30];
    char temp_fid[10];
    strcpy(filename,user->user);
    sprintf(temp_fid, "%d", user->fid);
    strcat(filename,temp_fid);

    pid_t pid = fork();
    if (!pid){
        char *args[] = {"rm",filename,NULL};
        execvp(args[0],args);
    }
    users_in_chat--;
    printf("User exited chat\n");
    mq informer;
    informer.mtype=1;

    /* informing about someone left the chat */
    char farewell[100];
    sprintf(farewell, "User %s has left the chat\n",user->user);
    strcpy(informer.mesg, farewell);

    for (int j=0; j<USERS_INCHAT; j++){
        if (users[j].exist) {
            if ((msgsnd(users[j].msqid_stoc, &informer, strlen(informer.mesg)+1, IPC_NOWAIT))<0) {
                printf("%d, %ld, %d, %s\n", \
                           users[j].msqid_stoc, users[j].mtype, 200, users[j].mesg);
                perror("msgsnd2");
                exit(1);
            }
        }
    }
}

void *chatting(void *arg){

    char *message_buffer[USERS_INCHAT];
    for (int i=0; i<USERS_INCHAT; i++){
        message_buffer[i]= malloc(MSGSZ + USER_NAME);
    }
    
    while (1) {
        /* Server get message and broadcast to every connected user */
        for (int i = 0; i < USERS_INCHAT; i++) {
            if (users[i].exist) {
                if ((msgrcv(users[i].msqid_ctos, &users[i], MSGSZ, 1, IPC_NOWAIT)) < 0) {
                    if (errno == ENOMSG) continue;
                    perror("msgrcv");
                    exit(1);
                }
                printf("message = %s\n",users[i].mesg);
                if (0 == (strcmp(users[i].mesg, ".exit\n"))) {
                    exit_from_chat(&users[i]);
                    continue;
                }
                /* reform message from "text" to "user: text" */
                strcpy(message_buffer[i], users[i].user);
                strcat(message_buffer[i], ": ");
                strcat(message_buffer[i], users[i].mesg);
                strcpy(users[i].mesg, message_buffer[i]);

                /* kinda broadcast */
                for (int j = 0; j < USERS_INCHAT; j++) {
                    if (users[j].exist == 1) {
                        size_t buf_length = strlen(users[i].mesg) + 1;
                        if ((msgsnd(users[j].msqid_stoc, &users[i], buf_length, IPC_NOWAIT))<0) {
                            printf("%d, %ld, %ld, %s\n", \
                            users[j].msqid_stoc, users[j].mtype, buf_length, users[j].mesg);
                            perror("msgsnd1");
                            exit(1);
                        }
                        printf("Sent to %s: %s", users[j].user,users[i].mesg);
                    }
                }
                usleep(100000);          //to not load CPU (0.1 sec)
            }
        }


    }
}


int main(){

    int msqid;
    key_t key;
    key = ftok(COMMON_MQ, PROJECT_ID);
    mq buf;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    buf.msqid = msqid;

    pthread_t pt_e;  // e = entering
    pthread_t pt_c;  // c = chatting
    pthread_create(&pt_e, NULL, entering, &buf);

    /* when server starts there is no users. just wait to someone */
    while (1) {
        if (users_in_chat) {
            pthread_create(&pt_c, NULL, chatting, NULL);
            break;
        }
        sleep(1);
    }

    pthread_join(pt_e, NULL);
    pthread_join(pt_c, NULL);


    exit(0);
}