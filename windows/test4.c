
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "components/label.h"
#include "components/vline.h"
#include "containers/horizontal.h"

int main(int argc, char **argv) {

    int col = 10;
    int row = 5;
    int stat = -1;
    int width = 38;
    int height = 10;
    char *value1 = "testing value 1";
    char *value2 = "testing value 2";
    window_t *window = NULL;
    component_t *vline = NULL;
    component_t *label1 = NULL;
    component_t *label2 = NULL;
    container_t *horizontal = NULL;

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

        window_box(window, "test4");

        if ((label1 = label_create(15, value1, strlen(value1))) == NULL) {

            printw("unable to create label\n");
            goto fini;

        }

        if ((label2 = label_create(15, value2, strlen(value2))) == NULL) {

            printw("unable to create label\n");
            goto fini;

        }

        if ((vline = vline_create(1)) == NULL) {

            printw("unable to create vline\n");
            goto fini;

        }

        if ((horizontal = horizontal_create(1, 1, 1, width - 2)) == NULL) {

            printw("unable to create horizontal container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal, label1)) != OK) {

            printw("unable to attach label to horizontal container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal, vline)) != OK) {

            printw("unable to attach vline to horiziontal container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal, label2)) != OK) {

            printw("unable to attach label to horiztonal container\n");
            goto fini;

        }

        if ((stat = window_add_container(window, horizontal)) != OK) {

            printw("unable to attach hotizontal container to window\n");
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

