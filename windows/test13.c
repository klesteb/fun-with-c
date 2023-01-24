
#include <ncurses.h>
#include <unistd.h>
#include "colors.h"

int main(int argc, char **argv) {

    int fg = 0;
    int bg = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    erase();
    refresh();

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        return(1);

    }

    init_colorpairs();

    mvaddstr(0, 35, "COLOR DEMO");
    mvaddstr(2, 0, "low intensity text colors (0-7)");
    mvaddstr(12, 0, "high intensity text colors (8-15)");

    for (bg = 0; bg <= 7; bg++) {

        for (fg = 0; fg <= 7; fg++) {

            colorset(fg, bg);
            mvaddstr(fg + 3, bg * 10, "...test...");
            coloroff(fg, bg);

        }

        for (fg = 8; fg <= 15; fg++) {

            colorset(fg, bg);
            mvaddstr(fg + 5, bg * 10, "...test...");
            coloroff(fg, bg);

        }

    }

    printw("\npress any key to exit\n");
    refresh();
    getch();
    endwin();

    return 0;

}

