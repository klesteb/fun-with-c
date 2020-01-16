
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"
#include "components/text.h"
#include "containers/horizontal.h"

window_t *create_window(int row, int col, int height, int width, char *text) {

    window_t *win = NULL;

    win = window_create(row, col, height, width);
    window_box(win, text);

    return win;

}

int main(int argc, char **argv) {

    int stat = 0;
    window_t *win1 = NULL;
    window_t *win2 = NULL;
    window_t *win3 = NULL;
    window_t *win4 = NULL;
    workbench_t *wb = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle windows\n");
        refresh();

        win1 = create_window(2, 4, 10, 40, "window 1");
        check_creation(win1);
        
        win2 = create_window(4, 6, 10, 40, "window 2");
        check_creation(win2);
        
        win3 = create_window(6, 8, 10, 40, "window 3");
        check_creation(win3);
        
        win4 = create_window(10, 10, 10, 65, "window 4");
        check_creation(win4);
        
        stat = workbench_add_window(wb, win1);
        check_return(stat, wb);

        stat = workbench_add_window(wb, win2);
        check_return(stat, wb);

        stat = workbench_add_window(wb, win3);
        check_return(stat, wb);

        stat = workbench_add_window(wb, win4);
        check_return(stat, wb);

        stat = workbench_loop(wb);
        check_return(stat, wb);

        exit_when;

    } use {

        stat = ERR;
        fprintf(stderr, "error: %d\n", trace_errnum);

    } end_when;

    return stat;

}

