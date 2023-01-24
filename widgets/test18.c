
#include <ncurses.h>

#include "include/when.h"
#include "include/errors_ncurses.h"

#include "events/event.h"
#include "errors/errors.h"
#include "tracer/tracer.h"

#include "widgets/colors.h"
#include "widgets/workbench.h"
#include "cclibs/misc/misc.h"
#include "widgets/components/more.h"
#include "widgets/components/text.h"

/*---------------------------------------------------------------------------*/

tracer_t *dump = NULL;
errors_t *errs = NULL;
event_t *events = NULL;
workbench_t *workbench = NULL;

/*---------------------------------------------------------------------------*/

int build_window(error_trace_t *errors) {

    queue lines;
    int stat = OK;
    theme_t theme;
    char *line = NULL;
    window_t *window = NULL;
    component_t *text = NULL;
    component_t *more = NULL;
    char *latin = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis.\n Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum.\n Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula.\n Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum.\n Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci.\n Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra.\n Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula.\n Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = wordwrap(latin, 40, &lines);
        check_status(stat, OK, E_INVOPS);

        window = window_create("", 10, 4, 10, 40, TRUE);
        check_creation(window);

        stat = window_set_theme(window, &theme);
        check_return(stat, window);

        line = "this is a test line to show";
        text = text_create(window, 1, 1, 30, line, strlen(line));
        check_creation(text);

        more = more_create(window, 3, 3, 7, 34, 1, &lines);
        check_creation(more);

        stat = window_add(window, text);
        check_return(stat, window);

        stat = window_add(window, more);
        check_return(stat, window);

        stat = workbench_add(workbench, window);
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

