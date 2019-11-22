
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "components/label.h"
#include "containers/vertical.h"

int main(int argc, char **argv) {

    int stat = -1;
    int width = 30;
    int height = 10;
    int row = 5;
    int col = 10;
    char *value = "testing value";
    window_t *window = NULL;
    component_t *label = NULL;
    container_t *vertical = NULL;

    initscr();
    cbreak();

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        exit(1);

    }

    init_colorpairs();
    erase();
    refresh();

    if ((window = window_create(row, col, height, width))) {

        window_box(window, "test2");

        if ((label = label_create(15, value, strlen(value))) == NULL) {

            printw("unable to create label\n");
            goto fini;

        }

        if ((vertical = vertical_create(1, 1, height - 1, (width / 2))) == NULL) {

            printw("unable to create container\n");
            goto fini;

        }

        if ((stat = container_add_component(vertical, label)) != OK) {

            printw("unable to attach component to container\n");
            goto fini;

        }

        if ((stat = window_add_container(window, vertical)) != OK) {

            printw("unable to attach container to window\n");
            goto fini;

        }

        if ((stat = window_draw(window)) != OK) {

            printw("unable to draw window\n");
            goto fini;

        }

        doupdate();
        printw("press any key\n");
        refresh();
        getch();

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

