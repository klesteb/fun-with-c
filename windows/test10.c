
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "containers/form.h"
#include "components/forms/forms.h"

int main(int argc, char **argv) {

    int ch;
    int row = 5;
    int col = 10;
    int stat = ERR;
    int width = 40;
    int height = 10;

    window_t *window = NULL;
    container_t *form = NULL;
    component_t *field1 = NULL;
    component_t *field2 = NULL;
    component_t *label1 = NULL;
    component_t *label2 = NULL;
    field_data_t label_fd1;
    field_data_t field_fd1;
    field_data_t label_fd2;
    field_data_t field_fd2;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();
    erase();
    refresh();
    curs_set(0);

    if ((window = window_create(row, col, height, width))) {

        window_box(window, "test 10");

        label_fd1.row = 0;
        label_fd1.col = 1;
        label_fd1.width = 15;
        label_fd1.height = 1;
        label_fd1.value = "label 1";

        if ((label1 = form_label_create(&label_fd1, sizeof(field_data_t))) == NULL) {

            printw("unable to create form label 1\n");
            goto fini;

        }

        component_set_padding(label1, TRUE);
        component_set_colors(label1, COLOR_RED, COLOR_WHITE);

        label_fd2.row = 1;
        label_fd2.col = 1;
        label_fd2.width = 15;
        label_fd2.height = 1;
        label_fd2.value = "label 2";

        if ((label2 = form_label_create(&label_fd2, sizeof(field_data_t))) == NULL) {

            printw("unable to create form label 2\n");
            goto fini;

        }

        component_set_padding(label1, TRUE);
        component_set_attribute(label1, A_BOLD);

        field_fd1.row = 0;
        field_fd1.col = 17;
        field_fd1.width = 15;
        field_fd1.height = 1;
        field_fd1.value = "current value";

        if ((field1 = form_field_create(&field_fd1, sizeof(field_data_t))) == NULL) {

            printw("unable to create form field 1\n");
            goto fini;

        }

        field_fd2.row = 1;
        field_fd2.col = 17;
        field_fd2.width = 15;
        field_fd2.height = 1;
        field_fd2.value = "current value";

        if ((field2 = form_field_create(&field_fd2, sizeof(field_data_t))) == NULL) {

            printw("unable to create form field 2\n");
            goto fini;

        }

        if ((form = form_create(1, 1, 9, 39)) == NULL) {

            printw("unable to create form container\n");
            goto fini;

        }

        if ((stat = container_add_component(form, label1)) != OK) {

            printw("unable to add label 1 to form\n");
            goto fini;

        }

        if ((stat = container_add_component(form, field1)) != OK) {

            printw("unable to add field 1 to form\n");
            goto fini;

        }

        if ((stat = container_add_component(form, label2)) != OK) {

            printw("unable to add label 2 to form\n");
            goto fini;

        }

        if ((stat = container_add_component(form, field2)) != OK) {

            printw("unable to add field 2 to form\n");
            goto fini;

        }

        if ((stat = window_add_container(window, form)) != OK) {

            printw("unable to add form container to window\n");
            goto fini;

        }

        if ((stat = window_draw(window)) != OK) {

            printw("unable to draw window\n");
            goto fini;

        }

        doupdate();

        printw("press F12 to quit\n");
        refresh();

        while ((ch = getch()) != KEY_F(12)) {

            event_t *event = calloc(1, sizeof(event_t));
            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            window_event(window, event);

            free(kevent);
            free(event);

            doupdate();

        }

        char text[32];

        if ((stat = form_field_results(field1, &text, sizeof(text))) == OK) {

            printw("entry 1 = %s\n", text);

        } else {

            printw("no entry\n");

        }

        if ((stat = form_field_results(field2, &text, sizeof(text))) == OK) {

            printw("entry 2 = %s\n", text);

        } else {

            printw("no entry\n");

        }

        refresh();
        
    } else {

        printw("unable to create window\n");

    }

    fini:

    printw("press any key to exit\n");
    refresh();
    getch();

    window_destroy(window);
    endwin();

    return 0;

}

