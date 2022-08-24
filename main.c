#include "functions.h"

//int y, x;

int main(int argc, char *argv[]) {
    PAGE p;

    if (argc > 1){
        open_file(&p,argv[1]);
    }
    else open_file(&p,"untitled.txt");

    /* Intarface creating */

    initscr();
    signal(SIGWINCH, sig_which);
    noecho();
    cbreak();
    keypad(stdscr, 1);

    curs_set(TRUE);
    refresh();

    print_page(&p, &x, &y);

    getyx(stdscr, y, x);

    while(1){
        int ch = getch();
        switch (ch){
            case KEY_F(1):
                save_file(&p);
                break;
            case KEY_F(2):
                endwin();
                destructor(&p);
                exit(EXIT_SUCCESS);
            case KEY_UP:
                up(&p, &x, &y);
                break;
            case KEY_DOWN:
                down(&p, &x, &y);
                break;
            case KEY_LEFT:
                left(&p, &x, &y);
                break;
            case KEY_RIGHT:
                right(&p, &x, &y);
                break;
            case '\n':
                ins_line(&p, y+1);
                print_page(&p, &x, &y);
                break;
            case KEY_BACKSPACE:
                if(0 == strlen(p.text[y].line)){
                    delete_empty_lines(&p, y);
                    left(&p, &x, &y);
                }
                else {
                    remove_ch(&p.text[y], x - 1);
                    left(&p, &x, &y);
                }
                print_page(&p, &x, &y);
                move(y,x);
                break;
            default:
                ins_ch(&p.text[y], ch, x);
                print_page(&p, &x, &y);
                right(&p,&x,&y);
        }
    }

    endwin();
    destructor(&p);
    exit(EXIT_SUCCESS);
    return 0;
}







