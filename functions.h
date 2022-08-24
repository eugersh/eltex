#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define LINE_SIZE 500
#define DEFAULT_SIZE_LINE 100

int x, y;

typedef struct {
    int size;
    char *line; //text in line
}LINE;

typedef struct {
    int lines_amount;
    int size;
    char *filename;
    LINE *text;       //ptr to line text
}PAGE;


void sig_which(int);

void open_file(PAGE *,char *);

void load_text(PAGE *);

void save_file(PAGE *);

void add_ch(LINE *, char);

void remove_ch(LINE *, int);

void ins_ch(LINE *, char, int);

void insert_string(LINE *, char *, int);

void ins_line(PAGE *, int);

void init_page(PAGE *, int);

void init_line(LINE *);

int count_lines(FILE *);

void up(PAGE *, int *, int *);

void down(PAGE *, int *, int *);

void left(PAGE *, int *, int *);

void right(PAGE *, int *, int *);

void print_page(PAGE *, int *, int *);

void delete_empty_lines(PAGE *, int);

void destructor(PAGE *);