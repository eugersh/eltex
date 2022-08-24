#include "functions.h"

void sig_which(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
}

void init_page(PAGE * p, int size){
    p->text = (LINE *) malloc(size*sizeof(LINE));
    for (int i=0; i<size; i++)
        init_line(p->text + i);
    p->size = size;
}

void init_line(LINE *l) {
    l->size = DEFAULT_SIZE_LINE;
    l->line = (char *) malloc(DEFAULT_SIZE_LINE*sizeof(char));
    l->line[0]='\0';
}

int count_lines(FILE *fp){
    char ch;
    int count=0;
    while ((ch = fgetc(fp)) !=EOF){
        if(ch=='\n') count++;
    }
    fseek(fp,0,SEEK_SET);//
    return count;
}


/* Открыть файл по имени или дефолт "untitled.txt" */
void open_file(PAGE *p, char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("File is not existing\n");
        fp = fopen(filename, "w");
        printf("File is created with name '%s'\n",filename);
    }
    printf("File is opened\n");
    p->filename=filename;
    fclose(fp);
    if(fp!=NULL) load_text(p);
}

void load_text(PAGE *p){
    FILE *fp = fopen(p->filename, "r");
    if(fp!=NULL) printf("File is opened to load\n");

    int size = count_lines(fp) * 2;
    if(size < LINE_SIZE) size = LINE_SIZE; //выделяем память по LINE_SIZE строк или больше.
    p->lines_amount = count_lines(fp);
    printf("Amount of lines in file = %d\n",p->lines_amount);
    if(0 == p->lines_amount){
        p->lines_amount=1;
    }
    init_page(p, size);

    char ch;
    for (int line = 0;(line < size) && (ch != EOF); line++) {
        ch = fgetc(fp);

        while (ch != '\n' && ch != EOF) {
            LINE *l = &(p->text[line]);
            ins_ch(l, ch, strlen(l->line));
            ch = fgetc(fp);
        }
    }
    fclose(fp);
}

void save_file(PAGE *p){
    FILE *fp = fopen(p->filename, "w");
    int y, x;

    for (y=0; y<p->lines_amount; y++){
        x=0;
        while(p->text[y].line[x] != '\0'){
            fputc(p->text[y].line[x], fp);
            x++;
        }
        fputc('\n', fp);
    }
    fclose(fp);
}

/* printing from file to screen */
void add_ch(LINE *l, char ch){
    int line_len = strlen(l->line);
    l->line[line_len]=ch;
}

/* printing char in screen (editor) */
void ins_ch(LINE *l, char ch, int ch_index){
    int new_size_line = strlen(l->line);
    if (new_size_line >= l->size-2) {
        char *temp = (char *) malloc(new_size_line * sizeof(char));
        strcpy(temp, l->line);
        free(l->line);
        l->line = temp;
        l->size = new_size_line;
    }

    for (int i=strlen(l->line);i>=ch_index; i--){
        l->line[i+1]=l->line[i];
    }
    l->line[ch_index]=ch;

}

/* deleting char in screen (editor) */
void remove_ch(LINE *l, int ch_index){
    for (int i=ch_index; i < strlen(l->line); i++){
        l->line[i] = l->line[i+1];
    }
}

void ins_line(PAGE *p, int row_index){
    LINE new_l;
    init_line(&new_l);
    new_l.line[0]='\0';

    for (int i=p->lines_amount-1; i>=row_index; i--){
        p->text[i+1]=p->text[i];
    }
    p->text[row_index]=new_l;

    (p->lines_amount)++;
    y++;
    x=0;
    move(y,0);
    clrtoeol();
}

void print_page(PAGE *p, int *x, int *y) {
    int i;
    for (i = 0; i < p->lines_amount; i++){
        move(i,0);
        clrtoeol();
        printw("%s", p->text[i].line);
    }
    move(*y, *x);
    refresh();
}

void delete_empty_lines(PAGE *p, int row_index){
    if (p->lines_amount > 1){
        free(p->text[row_index].line);

        for (int i=row_index; i<p->lines_amount; i++){
            p->text[i] = p->text[i+1];
        }
        (p->lines_amount) --;
    }
}

void destructor(PAGE *p){
    for (int i = 0; i < p->lines_amount; i++){
        free(p->text[i].line);
    }
    free(p->text);
    printf("memory freed\n");
}

void up(PAGE *p, int *x, int *y) {
    if (*y > 0) {
        --(*y);
    }
    move(*y, *x);
}

void down(PAGE *p, int *x, int *y) {
    if (*y < p->lines_amount-1) {
        ++(*y);
    }
    move(*y, *x);
}

void left(PAGE *p, int *x, int *y) {
    if (*x > 0) {
        --(*x);
    }
    else if (*y != 0) {
        --(*y);
        *x=strlen(p->text[*y].line)+1;
    }
    move(*y, *x);
}

void right(PAGE *p, int *x, int *y) {
    if (*x < strlen(p->text[*y].line)) {
        ++(*x);
        if ((*x == strlen(p->text[*y].line)+1) && (*y < p->lines_amount-1)){
            ++(*y);
            *x = 0;
        }
    }
    move(*y, *x);
}