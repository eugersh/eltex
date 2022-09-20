#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define COMMAND_SIZE 64



char *scommand(char* command, int *pipes) {

    int i = 0;
    char ch;
    ch=getchar();

    while(ch != EOF && ch != '\n'){
        if ('|'==ch) (*pipes)++;

        command[i]=ch;
        i++;
        ch = getchar();
    }

    return command;
}

char **parse_command(char *str){
    int i = 0;

    char **args = malloc(sizeof(char *));
    char *lexeme = strtok_r(str, " ", &str);

    while(lexeme){
        args[i]= malloc(30);
        strcpy(args[i],lexeme);
        i++;
        lexeme = strtok_r(str, " ", &str);
    }


    return args;
}



int main(void) {
    printf("SELF-MADE INTERPRETER\n");
    char command[COMMAND_SIZE];
    int pipefd[2];
    pid_t pid;

    while (1) {
        int pipes=0;
        memset(command, 0, COMMAND_SIZE);
        if (-1 == pipe(pipefd)){
            perror("Pipe");
            exit(EXIT_FAILURE);
        }

        printf("> ");
        scommand(command, &pipes);
//        printf("%s\tPipes = %d\n",command, pipes);

        char *lexeme = strtok(command, "|");
        char **cmd = malloc((pipes+1) * sizeof(char*));
        char *buffer = malloc(30);

        for (int i=0; lexeme; i++){

            *cmd = realloc(*cmd, (i+1)* sizeof(char*));
//            printf("%s\t size = %ld\n", lexeme, sizeof(cmd));
            strcpy(buffer,lexeme);
            cmd = parse_command(buffer);
            cmd[2] = NULL;
            lexeme = strtok(NULL, "|");

            pid = fork();
            if (-1 == pid){
                perror("Fork");
                exit(EXIT_FAILURE);
            }
            if (0 == pid){
                if (i>0) dup2(pipefd[0],0);
                if (lexeme) dup2(pipefd[1],1);
                if(execvp(cmd[0],cmd)==-1){
                    perror("Command not found");
                }
            }
            else {
                wait(NULL);
                char eof=EOF;
                write(pipefd[1],&eof,1);
            }
        }
    }
    exit(EXIT_SUCCESS);
}