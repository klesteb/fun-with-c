
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"

int main(int argc, char **argv) {

    int stat = 0;
    window_t *win1 = NULL;
    window_t *win2 = NULL;
    window_t *win3 = NULL;
    workbench_t *wb = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C to exit\n");
        refresh();

        win1 = window_create(2, 4, 10, 40);
        check_creation(win1);

        stat = window_box(win1, "window 1");
        check_return(stat, win1);

        win2 = window_create(4, 6, 10, 40);
        check_creation(win2);

        stat = window_box(win2, "window 2");
        check_return(stat, win2);

        stat = window_output(win2, 1, 1, "testing 2");
        check_return(stat, win2);

        win3 = window_create(6, 50, 10, 20);
        check_creation(win3);

        stat = window_box(win3, "window 3");
        check_return(stat, win3);

        stat = window_output(win3, 1, 1, "testing 3");
        check_return(stat, win3);

        stat = workbench_add_window(wb, win1);
        check_return(stat, wb);

        stat = workbench_add_window(wb, win2);
        check_return(stat, wb);

        stat = workbench_add_window(wb, win3);
        check_return(stat, wb);

        stat = workbench_loop(wb);
        check_return(stat, wb);

        exit_when;

    } use {

        stat = ERR;
        printf("error: %d\n", trace_errnum);

    } end_when;

    return stat;

}

