
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "event.h"
#include "workbench.h"
#include "error_codes.h"

event_t *events = NULL;
workbench_t *wb = NULL;

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

    int stat;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        events = event_create();
        check_creation(events);

        printw("Press ^C to exit\n");
        refresh();

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

        stat = event_loop(events);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        printf("error: %d\n", trace_errnum);

    } end_when;

    return 0;

}

