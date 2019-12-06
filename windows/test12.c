
#include <ncurses.h>
#include <unistd.h>
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
    noecho();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();
    erase();
    refresh();
    curs_set(0);

    if ((window = window_create(row, col, height, width))) {

        window_set_colors(window, COLOR_WHITE, COLOR_BLUE);
        window_box(window, "test12");

        window_output(window, 1, 1, "testing...");
        curs_set(1);

        doupdate();
        sleep(10);

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

