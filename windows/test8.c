
#include <stdio.h>
#include <ncurses.h>

#include "colors.h"
#include "window.h"
#include "container.h"
#include "component.h"
#include "components/textarea.h"
#include "containers/scrollable.h"

int main(int argc, char **argv) {

    int ch = 0;
    int col = 10;
    int row = 5;
    int stat = ERR;
    int width = 48;
    int height = 10;
    char *value = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";
    window_t *window = NULL;
    component_t *textarea = NULL;
    container_t *scrollable = NULL;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors() == FALSE) {

        endwin();
        printf("Your terminal does not support color\n");
        return(1);

    }

    init_colorpairs();
    erase();
    refresh();

    if ((window = window_create(row, col, height, width))) {

        window_box(window, "scrollable");

        if ((textarea = textarea_create(width - 2, value, strlen(value))) == NULL) {

            printw("unable to create textarea\n");
            goto fini;

        }

        if ((scrollable = scrollable_create(1, 1, height - 1 , width - 1)) == NULL) {

            printw("unable to create scrollable container\n");
            goto fini;

        }

        if ((stat = container_add_component(scrollable, textarea)) != OK) {

            printw("unable to attach textarea to scrollable container\n");
            goto fini;

        }

        if ((stat = window_add_container(window, scrollable)) != OK) {

            printw("unable to attach scrollable container to window\n");
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

