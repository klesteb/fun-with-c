
#include "windows.h"

int demo(void);
int howto(void);
int printer(void);

int main(void) {

    Windows *win;
    int ch;
    int return_code;
    int i;

    struct pd_struct pd_menu[5] = {
        "Sort", 'S',
        "Ascending  ", demo, 0, 'A',
        "Descending ", demo, 0, 'D',
        "",            NULL, 0, 0,
        "",            NULL, 0, 0,

        "Print", 'P',
        "Printer   ", printer, 0, 'P',
        "Text-file ", demo,    0, 'T',
        "",           NULL,    0,  0,
        "",           NULL,    0,  0,

        "Delete", 'D',
        "Data-file    ", demo, 0, 'D',
        "Program-file ", demo, 0, 'T',
        "Programmer   ", demo, 0, 'P',
        "",              NULL, 0, 0,

        "Copy", 'C',
        "Block ", demo, 0, 'B',
        "Line  ", demo, 0, 'L',
        "",       NULL, 0,  0,
        "",       NULL, 0,  0,

        "Quit", 'Q',
        "Exit program", NULL, 1, 'E',
        "",             NULL, 0, 0,
        "",             NULL, 0, 0,
        "",             NULL, 0, 0
        };


    win_init();

    win = win_open("PD Demo", 16, 75, 5, 2, FALSE);

    win_output(win, 4, 1, "INSTRUCTIONS");
    win_output(win, 6, 1, "EXPERT MODE: Select by touching the key which \
represents each option");
    win_output(win, 7, 14, "(the capitial letter)");
    win_output(win, 10, 1, "ASSIST MODE: Pull down menu by touching \
Enter or a cursor key.");
    win_output(win, 11, 14, "Select by highlighting with cursor keys - \
then touch Enter.");
    win_output(win, 13, 1, "EXIT: To exit to OS press ^Z");

    return_code = win_pulldown_menu(pd_menu, A_NORMAL, A_REVERSE);

    win_close(win);
    
    win_quit();

}

int demo(void) {

    Windows *win;
    int ch;

    win = win_open("Demo Window", 6, 30, 6, 6, FALSE);

    win_output(win, 1, 1, "Make believe window...");

    win_set_attr(win, A_BOLD);
    win_output(win, 3, 1, "Touch any key to exit!");

    ch = win_getch(win);

    win_close(win);

    return(0);

}

int printer(void) {

    struct pop_struct pop_menu[3] = {
        "Lpt1:", NULL, 0, '1',
        "Lpt2:", NULL, 0, '2',
        "\0"
        };

    return(win_popup_menu(pop_menu, 25, 5, A_NORMAL, A_REVERSE));

}


