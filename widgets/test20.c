
#include <ncurses.h>

#include "when.h"
#include "event.h"
#include "colors.h"
#include "errors.h"
#include "tracer.h"
#include "window.h"
#include "workbench.h"
#include "errors_ncurses.h"
#include "windows/query_window.h"

/*---------------------------------------------------------------------------*/

window_t *win = NULL;
tracer_t *dump = NULL;
errors_t *errs = NULL;
event_t *events = NULL;
workbench_t *workbench = NULL;

/*---------------------------------------------------------------------------*/

int logout(int yesno, error_trace_t *errors) {

    int stat = OK;

fprintf(stderr, "enterng logout(): %d\n", yesno);
    when_error_in {

        if (yesno) {

fprintf(stderr, "event_break()\n");
            stat = event_break(events);
            check_return(stat, events);

        } else {

fprintf(stderr, "workbench_remove()\n");
            stat = workbench_remove_async(workbench, win);
            check_return(stat, workbench);

        }

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving logout() - stat: %d\n", stat);
    return stat;

}

int build_window(error_trace_t *errors) {

    int stat = OK;
    char *title = "Logout";
    char *value = "Are you sure you want to logoff?";

    when_error_in {

        win = query_window(title, logout, "%s", value);
        check_creation(win);

        stat = workbench_add(workbench, win);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int print_dump(char *string) {

    fprintf(stderr, "%s\n",string);
    return OK;

}

int build_ui(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = build_window(&error);
        check_status2(stat, OK, error);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int process_stdin(void *data) {

    int stat = OK;
    int again = FALSE;

    when_error_in {

        stat = workbench_dispatch(workbench, &again);
        check_return(stat, workbench);

        if (again) {

            stat = event_register_worker(events, FALSE, process_stdin, NULL);
            check_return(stat, events);

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

int read_stdin(void *data) {

    int stat = OK;

    when_error_in {

        stat = workbench_capture(workbench);
        check_return(stat, workbench);

        stat = event_register_worker(events, FALSE, process_stdin, NULL);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

        event_break(events);

    } end_when;

    return stat;

}

int run(error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

        stat = event_loop(events);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int cleanup(void) {

    tracer_destroy(dump);
    errors_destroy(errs);
    event_destroy(events);
    workbench_destroy(workbench);

    return OK;

}

int setup(error_trace_t *errors) {

    int stat = OK;

    when_error_in {

        /* setup error handling */

        errs = errors_create();
        check_creation(errs);

        stat = errors_load(errs, ncurses_codes, sizeof(ncurses_codes));
        check_return(stat, errs);

        dump = tracer_create(errs);
        check_creation(dump);

        /* create the workbench */

        workbench = workbench_create(NULL);
        check_creation(workbench);

        /* create event polling */

        events = event_create();
        check_creation(events);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int main(void) {

    int stat = OK;
    error_trace_t errors;

    when_error_in {

        stat = setup(&errors);
        check_status2(stat, OK, errors);

        stat = build_ui(&errors);
        check_status2(stat, OK, errors);

        stat = run(&errors);
        check_status2(stat, OK, errors);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, print_dump);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

