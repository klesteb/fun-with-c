
#include <ncurses.h>
#include <errno.h>

#include "include/when.h"
#include "include/errors_ncurses.h"

#include "events/event.h"
#include "errors/errors.h"
#include "tracer/tracer.h"

#include "widgets/colors.h"
#include "widgets/workbench.h"
#include "widgets/windows/alert.h"
#include "widgets/components/text.h"
#include "widgets/windows/bar_menu.h"
#include "widgets/windows/box_menu.h"
#include "widgets/components/hline.h"
#include "widgets/components/vline.h"
#include "widgets/components/menus/list.h"
#include "widgets/components/menus/menus.h"

/*---------------------------------------------------------------------------*/

tracer_t *dump = NULL;
errors_t *errs = NULL;
event_t *events = NULL;
workbench_t *workbench = NULL;

/*---------------------------------------------------------------------------*/

int print_description(const char *message, error_trace_t *errors) {

    int stat = OK;
    events_t *event = NULL;

    when_error_in {

        errno = 0;
        event = calloc(1, sizeof(events_t));
        if (event == NULL) cause_error(errno);

        event->type = EVENT_K_MESSAGE;

        if (message != NULL) {

            event->data = (void *)strndup(message, strlen(message));

        } else {

            event->data = (void *)strdup("no data provided");

        }

        stat = workbench_inject_event(workbench, event);
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

int create_window(char *title, int startx, int starty, int height, int width, window_t **win, error_trace_t *errors) {

    int col = 0;
    int row = 0;
    int stat = OK;
    component_t *text = NULL;
    component_t *hline = NULL;
    char *value = "F10=Main  F11=Cycle  F12=Quit";

    when_error_in {

        *win = window_create(title, startx, starty, height, width, TRUE);
        check_creation(*win);

        row = height - 2;
        hline = hline_create(*win, row, col, width);
        check_creation(hline);

        row = height - 1;
        col = ((width - strlen(value)) / 2);
        width = strlen(value) + 1;
        text = text_create(*win, row, col, width, value, strlen(value));
        check_creation(text);

        stat = window_add(*win, hline);
        check_return(stat, *win);

        stat = window_add(*win, text);
        check_return(stat, *win);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int print_result(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    int width = 0;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    error_trace_t error;
    menus_list_t list[5];
    window_t *temp = NULL;
    component_t *vline = NULL;
    component_t *bmenu = NULL;

    when_error_in {

        memset(title, '\0', 40);
        snprintf(title, 38, "Room: %s", "Testing"); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        SET_MENU(list[0], "Forward", "forward retrieval of messages", MENUS_T_ITEM, NULL, 0, NULL);
        SET_MENU(list[1], "New", "read new messages", MENUS_T_ITEM, NULL, 0, NULL);
        SET_MENU(list[2], "Old", "read old messages", MENUS_T_ITEM, NULL, 0, NULL);
        SET_MENU(list[3], "Write", "create a new message", MENUS_T_ITEM, NULL, 0, NULL);
        SET_MENU(list[4], "Reverse", "reverse retrieval of messages", MENUS_T_ITEM, NULL, 0, NULL);

        stat = create_window(title, startx, starty, height, width, &temp, &error);
        check_status2(stat, OK, error);

        bmenu = list_menu_create(temp, 0, 0, height - 3, 9, 1, print_description, list, sizeof(list));
        check_creation(bmenu);

        vline = vline_create(temp, 0, 9, height - 2);
        check_creation(vline);

        stat = window_add(temp, vline);
        check_return(stat, temp);

        stat = window_add(temp, bmenu);
        check_return(stat, temp);

        stat = workbench_add(workbench, temp);
        check_return(stat, workbench);

        stat = workbench_set_focus(workbench, temp);
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

int build_menu2(void *data, int size, error_trace_t *errors) {

    int stat = OK;
    int width = 0;
    int startx = 0;
    int starty = 0;
    theme_t theme;
    menus_list_t list[9];
    window_t *bmenu = NULL;
    char *data1 = "this is data for test1";
    char *data2 = "this is data for test2";
    char *data3 = "this is data for test3";
    char *data4 = "this is data for test4";
    char *data5 = "this is data for test5";
    char *data6 = "this is data for test6";
    char *data7 = "this is data for test7";
    char *data8 = "this is data for test8";
    char *data9 = "this is data for test9";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        startx = getbegx(stdscr);
        starty = getbegy(stdscr);
        width  = getmaxx(stdscr) - 2;

        SET_MENU(list[0], "test 1", "this is test 1", MENUS_T_ITEM, data1, strlen(data1), print_result);
        SET_MENU(list[1], "test 2", "this is test 2", MENUS_T_ITEM, data2, strlen(data2), print_result);
        SET_MENU(list[2], "test 3", "this is test 3", MENUS_T_ITEM, data3, strlen(data3), print_result);
        SET_MENU(list[3], "test 4", "this is test 4", MENUS_T_ITEM, data4, strlen(data4), print_result);
        SET_MENU(list[4], "test 5", "this is test 5", MENUS_T_ITEM, data5, strlen(data5), print_result);
        SET_MENU(list[5], "test 6", "this is test 6", MENUS_T_ITEM, data6, strlen(data6), print_result);
        SET_MENU(list[6], "test 7", "this is test 7", MENUS_T_ITEM, data7, strlen(data7), print_result);
        SET_MENU(list[7], "test 8", "this is test 8", MENUS_T_ITEM, data8, strlen(data8), print_result);
        SET_MENU(list[8], "test 9", "this is test 9", MENUS_T_ITEM, data9, strlen(data9), print_result);

        bmenu = bar_menu(startx, starty, 2, width, list, sizeof(list));
        check_creation(bmenu);

        stat = window_set_theme(bmenu, &theme);
        check_return(stat, bmenu);

        stat = workbench_set_menu(workbench, bmenu);
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

int build_menu(error_trace_t *errors) {

    int stat = OK;
    int width = 0;
    int startx = 0;
    int starty = 0;
    theme_t theme;
    menus_list_t list[9];
    window_t *bmenu = NULL;
    char *data1 = "this is data for test1";
    char *data2 = "this is data for test2";
    char *data3 = "this is data for test3";
    char *data4 = "this is data for test4";
    char *data5 = "this is data for test5";
    char *data6 = "this is data for test6";
    char *data7 = "this is data for test7";
    char *data8 = "this is data for test8";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        startx = getbegx(stdscr);
        starty = getbegy(stdscr);
        width  = getmaxx(stdscr) - 2;

        SET_MENU(list[0], "test 1", "this is test 1", MENUS_T_ITEM, data1, strlen(data1), print_result);
        SET_MENU(list[1], "test 2", "this is test 2", MENUS_T_ITEM, data2, strlen(data2), print_result);
        SET_MENU(list[2], "test 3", "this is test 3", MENUS_T_ITEM, data3, strlen(data3), print_result);
        SET_MENU(list[3], "test 4", "this is test 4", MENUS_T_ITEM, data4, strlen(data4), print_result);
        SET_MENU(list[4], "test 5", "this is test 5", MENUS_T_ITEM, data5, strlen(data5), print_result);
        SET_MENU(list[5], "test 6", "this is test 6", MENUS_T_ITEM, data6, strlen(data6), print_result);
        SET_MENU(list[6], "test 7", "this is test 7", MENUS_T_ITEM, data7, strlen(data7), print_result);
        SET_MENU(list[7], "test 8", "this is test 8", MENUS_T_ITEM, data8, strlen(data8), print_result);
        SET_MENU(list[8], "menu 2", "this is menu 2", MENUS_T_ITEM, NULL, 0, build_menu2);

        bmenu = bar_menu(startx, starty, 2, width, list, sizeof(list));
        check_creation(bmenu);

        stat = window_set_theme(bmenu, &theme);
        check_return(stat, bmenu);

        stat = workbench_set_menu(workbench, bmenu);
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

int build_ui(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = build_menu(&error);
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

