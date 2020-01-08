
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"

int main(int argc, char **argv) {

    int stat = 0;
    int row = 5;
    int col = 10;
    int width = 40;
    int height = 10;
    workbench_t *wb = NULL;
    window_t *window = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C to exit\n");
        refresh();

        window = window_create(row, col, height, width);
        check_creation(window);

        stat = window_box(window, "test 17");
        check_status(stat, OK, E_INVOPS);

        stat = workbench_add_window(wb, window);
        check_status(stat, OK, E_INVOPS);

        stat = workbench_loop(wb);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        printf("error: %d\n", trace_errnum);

    } end_when;

    return stat;

}

