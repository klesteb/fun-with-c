
#include <ncurses.h>
#include <unistd.h>
#include "window.h"
#include "colors.h"

int main(int argc, char **argv) {

    int row = 5;
    int col = 5;
    int width = 40;
    int height = 10;
    window_t *window = NULL;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    erase();
    refresh();

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();

    curs_set(0);

    if ((window = window_create(row, col, height, width))) {

        window_set_colors(window, COLOR_BLUE, COLOR_WHITE);
        window_box(window, "test12");
        window_refresh(window);

        doupdate();
        sleep(10);

    } else {

        printw("unable to create window\n");

    }

    printw("press any key to exit\n");
    refresh();
    getch();

    window_destroy(window);
    endwin();

    return 0;

}

