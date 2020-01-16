
#include <stdio.h>
#include <ncurses.h>
#include <errno.h>

#include "when.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "workbench.h"
#include "components/label.h"
#include "components/hline.h"
#include "containers/vertical.h"

window_t *create_window(int row, int col, int height, int width, char *text, int *stat) {

    window_t *win = NULL;
    component_t *label = NULL;
    component_t *hline = NULL;
    container_t *vertical = NULL;
    char *value = "testing value";

    when_error {

        win = window_create(row, col, height, width);
        check_creation(win);

        *stat = window_box(win, text);
        check_return(*stat, win);

        label = label_create(15, value, strlen(value));
        check_creation(label);

        hline = hline_create(15);
        check_creation(hline);

        vertical = vertical_create(1, 1, height - 2, width - 2);
        check_creation(vertical);

        *stat = container_add_component(vertical, label);
        check_return(*stat, vertical);

        *stat = container_add_component(vertical, hline);
        check_return(*stat, vertical);

        *stat = window_add_container(win, vertical);
        check_return(*stat, win);

        exit_when;

    } use {

        *stat = ERR;
        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    return win;

}

int main(int argc, char **argv) {

    int stat = OK;
    window_t win2;
    window_t *win1 = NULL;
    workbench_t *wb = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle windows\n");
        refresh();

        errno = 0;
        win1 = create_window(2, 4, 10, 40, "window 1", &stat);
        check_status(stat, OK, errno);

        stat = workbench_add_window(wb, win1);
        check_return(stat, wb);

        stat = workbench_set_focus(wb, win1);
        check_return(stat, wb);

        stat = workbench_get_focus(wb, &win2);
        check_return(stat, wb);

        if ((window_compare(win1, &win2)) == OK) {

            printw("windows are the same\n");
            refresh();

        } else {

            printw("windows are not the same\n");
            refresh();

        }

        workbench_loop(wb);

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

    } end_when;

    workbench_destroy(wb);

    return stat;

}

