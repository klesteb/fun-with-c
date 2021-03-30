
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "error_trace.h"
#include "windows/box_menu.h"
#include "components/menus/menus.h"

int print_description(const char *description, error_trace_t *errors) {

    move(1, 1);
    clrtoeol();
    printw("description: %s", (char *)description);
    refresh();

    return OK;

}

int print_result(void *data, int size, error_trace_t *errors) {

    move(20, 1);
    clrtoeol();
    printw("result: %s", (char *)data);
    refresh();

    return OK;

}

void setup(void) {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();
    erase();
    refresh();

}

int main(void) {

    int ch = 0;
    int stat = OK;
    theme_t theme;
    int list_size = 0;
    window_t *bmenu = NULL;
    menus_list_t list[6];
    char *data1 = "this is data for test1";
    char *data2 = "this is data for test2";
    char *data3 = "this is data for test3";
    char *data4 = "this is data for test4";
    char *data5 = "this is data for test5";
    char *data6 = "this is data for test6";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        setup();

        list_size = 6 * sizeof(menus_list_t);
        SET_MENU(list[0], "test 1", "this is test 1", MENUS_T_ITEM, data1, strlen(data1), print_result);
        SET_MENU(list[1], "test 2", "this is test 2", MENUS_T_ITEM, data2, strlen(data2), print_result);
        SET_MENU(list[2], "test 3", "this is test 3", MENUS_T_ITEM, data3, strlen(data3), print_result);
        SET_MENU(list[3], "test 4", "this is test 4", MENUS_T_ITEM, data4, strlen(data4), print_result);
        SET_MENU(list[4], "test 5", "this is test 5", MENUS_T_ITEM, data5, strlen(data5), print_result);
        SET_MENU(list[5], "test 6", "this is test 6", MENUS_T_ITEM, data6, strlen(data6), print_result);

        bmenu = box_menu("testing", 10, 4, 10, 40, print_description, list, list_size);
        check_creation(bmenu);

        stat = window_set_theme(bmenu, &theme);
        check_return(stat, bmenu);

        stat = window_draw(bmenu);
        check_return(stat, bmenu);

        doupdate();

        while ((ch = getch()) != KEY_F(12)) {

            events_t *event = calloc(1, sizeof(events_t));
            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            stat = window_event(bmenu, event);
            check_return(stat, bmenu);

            free(kevent);
            free(event);

            doupdate();

        }

        stat = window_erase(bmenu);
        check_return(stat, bmenu);
        refresh();

        stat = window_destroy(bmenu);
        check_return(stat, bmenu);

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    endwin();

    return 0;

}

