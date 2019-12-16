
#include <ncurses.h>
#include <unistd.h>
#include "colors.h"

int main(int argc, char **argv) {

    int bg = 0;
    int fg = 0;
    int color = 0;

    for (bg = 0; bg <= 7; bg++) {

        for (fg = 0; fg <= 7; fg++) {

            color = colornum(fg, bg);
            printf("colornum = %d, fg = %d, bg = %d\n", color, fg, bg);

        }

    }

    return 0;

}

