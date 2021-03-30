
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "error_trace.h"
#include "windows/bar_menu.h"
#include "components/menus/menus.h"

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
    int width = 0;
    int height = 2;
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

        setup();

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

        bmenu = bar_menu(startx, starty, height, width, list, sizeof(list));
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

