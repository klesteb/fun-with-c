
#include <stdio.h>
#include <ncurses.h>
#include <errno.h>

#include "when.h"
#include "event.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "workbench.h"
#include "components/label.h"
#include "components/hline.h"
#include "containers/vertical.h"

event_t *events = NULL;
workbench_t *wb = NULL;

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

int process_stdin(void *data) {

    int stat = OK;
    int again = FALSE;

    when_error_in {

        stat = workbench_dispatch(wb, &again);
        check_return(stat, wb);

        if (again) {

            stat = event_register_worker(events, FALSE, process_stdin, NULL);
            check_return(stat, events);

        }

        exit_when;

    } use {

        stat = ERR;
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

int read_stdin(void *data) {

    int stat = OK;

    when_error_in {

        stat = workbench_capture(wb);
        check_return(stat, wb);

        stat = event_register_worker(events, FALSE, process_stdin, NULL);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;
    window_t win2;
    window_t *win1 = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        events = event_create();
        check_creation(events);

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

        stat = workbench_refresh(wb);
        check_return(stat, wb);

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

        stat = event_loop(events);
        check_return(stat, events);

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

    } end_when;

    workbench_destroy(wb);

    return stat;

}

