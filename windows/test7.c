
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "components/textarea.h"
#include "containers/vertical.h"

int main(int argc, char **argv) {

    int stat = ERR;
    int width = 48;
    int height = 23;
    int row = 1;
    int col = 10;
    char *value = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";
    window_t *window = NULL;
    component_t *textarea = NULL;
    container_t *vertical = NULL;

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

    if ((window = window_create(row, col, height, width))) {

        window_box(window, "test7");

        if ((textarea = textarea_create(width - 1, value, strlen(value))) == NULL) {

            printw("unable to create textarea\n");
            refresh();
            goto fini;

        }

        if ((vertical = vertical_create(0, 0, height, width)) == NULL) {

            printw("unable to create container\n");
            refresh();
            goto fini;

        }

        if ((stat = container_add_component(vertical, textarea)) != OK) {

            printw("unable to attach component to container\n");
            refresh();
            goto fini;

        }

        if ((stat = window_add_container(window, vertical)) != OK) {

            printw("unable to attach container to window\n");
            refresh();
            goto fini;

        }

        if ((stat = window_draw(window)) != OK) {

            printw("unable to draw window\n");
            refresh();
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

