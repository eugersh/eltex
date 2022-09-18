#include <stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>



#define COMMON_MQ "commonmq"
#define PROJECT_ID 16       // common mq
#define PROJECT_ID_CTOS 20  // mq from client to server
#define PROJECT_ID_STOC 30  // mq from server to client
#define USER_NAME 24
#define USERS_INCHAT 10
#define MSGSZ 128

int users_in_chat=0;



typedef struct message_queue {
    long mtype;                 // type/prior flag to send and get by mq (default)
    char mesg[MSGSZ];           // size of message to translate (default)
    char user[USER_NAME];       // name of user [stdin] at start of main()
    int fid;                    // id of calling process
    int msqid;                  // id of common mq
    int msqid_stoc;             // id of private recv mq
    int msqid_ctos;             // id of private send mq
    int exist;                  // 1 or 0

} mq;

mq* users;