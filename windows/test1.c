
#include <ncurses.h>
#include "window.h"
#include "colors.h"

int main(int argc, char **argv) {

    int stat = ERR;
    int row = 5;
    int col = 5;
    int width = 40;
    int height = 10;
    window_t *window = NULL;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();
    erase();
    refresh();

    if ((window = window_create(row, col, height, width))) {

        window_box(window, "test1");

        wprintw(window->inner, "testing...");

        if ((stat = window_draw(window)) != OK) {

            printw("unable to draw window\n");
            goto fini;

        }

        doupdate();
        printw("press any key\n");
        refresh();
        getch();

    } else {

        printw("unable to create window\n");

    }

    fini:

    printw("press any key to exit\n");
    refresh();
    getch();

    window_destroy(window);
    endwin();

    return 0;

}

