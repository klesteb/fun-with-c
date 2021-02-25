
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "menus/box.h"
#include "menus/menus.h"
#include "menus/menus_list.h"

int print_description(const char *description) {

    move(1, 1);
    clrtoeol();
    printw("description: %s", (char *)description);
    refresh();

    return OK;

}

int print_result(void *data, int size) {

    move(1, 2);
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
    menus_t *bmenu = NULL;
    menus_list_t list[3];
    char *data1 = "this is data for test1";
    char *data2 = "this is data for test2";
    char *data3 = "this is data fors test3";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        setup();

        SET_MENU(list[0], "test 1", "this is test 1", data1, strlen(data1), print_result);
        SET_MENU(list[1], "test 2", "this is test 2", data2, strlen(data2), print_result);
        SET_MENU(list[2], "test 3", "this is test 3", data3, strlen(data3), print_result);

        bmenu = box_menu_create("testing", 10, 4, 10, 40, print_description, list, sizeof(list));
        check_creation(bmenu);

        stat = menus_set_theme(bmenu, &theme);
        check_return(stat, bmenu);

        stat = menus_draw(bmenu);
        check_return(stat, bmenu);

        doupdate();

        while ((ch = getch()) != KEY_F(12)) {

            events_t *event = calloc(1, sizeof(events_t));
            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            menus_event(bmenu, event);

            free(kevent);
            free(event);

            doupdate();

        }

        stat = menus_erase(bmenu);
        check_return(stat, bmenu);
        refresh();

        stat = menus_destroy(bmenu);
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

