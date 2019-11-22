
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "components/text.h"
#include "components/label.h"
#include "containers/horizontal.h"

int main(int argc, char **argv) {

    int stat = -1;
    int width = 30;
    int height = 10;
    int row = 5;
    int col = 10;
    char *label_value_1 = "label 1";
    char *text_value_1 = "testing value";
    char *label_value_2 = "label 2";
    char *text_value_2 = "another testing value";
    
    window_t *window = NULL;
    component_t *text1 = NULL;
    component_t *text2 = NULL;
    component_t *label1 = NULL;
    component_t *label2 = NULL;
    container_t *horizontal1 = NULL;
    container_t *horizontal2 = NULL;

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

        window_box(window, "test6");

        if ((label1 = label_create(15, label_value_1, strlen(label_value_1))) == NULL) {

            printw("unable to create label1\n");
            goto fini;

        }

        component_set_padding(label1, 1);

        if ((label2 = label_create(15, label_value_2, strlen(label_value_2))) == NULL) {

            printw("unable to create label2\n");
            goto fini;

        }

        if ((text1 = text_create(15, text_value_1, strlen(text_value_1))) == NULL) {

            printw("unable to create text1\n");
            goto fini;

        }

        if ((text2 = text_create(15, text_value_2, strlen(text_value_2))) == NULL) {

            printw("unable to create text2\n");
            goto fini;

        }
        
        if ((horizontal1 = horizontal_create(1, 1, 1, width - 1)) == NULL) {

            printw("unable to create horizontal container 1\n");
            goto fini;

        }

        if ((horizontal2 = horizontal_create(2, 1, 1, width - 1)) == NULL) {

            printw("unable to create horizontal container 2\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal1, label1)) != OK) {

            printw("unable to attach label1 component to container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal1, text1)) != OK) {

            printw("unable to attach text1 component to container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal2, label2)) != OK) {

            printw("unable to attach label2 component to container\n");
            goto fini;

        }

        if ((stat = container_add_component(horizontal2, text2)) != OK) {

            printw("unable to attach text2 component to container\n");
            goto fini;

        }

        if ((stat = window_add_container(window, horizontal1)) != OK) {

            printw("unable to attach horizontal1 container to window\n");
            goto fini;

        }

        if ((stat = window_add_container(window, horizontal2)) != OK) {

            printw("unable to attach horizontal2 container to window\n");
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

