
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "window.h"
#include "item_list.h"
#include "components/label.h"

void setup(void) {

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

}

int main(void) {

    int stat = OK;
    theme_t theme;
    theme_t junk1;
    coordinates_t junk2;
    window_t *window = NULL;
    component_t *label = NULL;

    theme.attribute  = A_NORMAL;
    theme.foreground = CYAN;
    theme.background = BLACK;

    when_error_in {

        setup();

        window = window_create("", 10, 4, 10, 40, TRUE);
        check_creation(window);

        stat = window_set_theme(window, &theme);
        check_return(stat, window);

        stat = window_get_theme(window, &junk1);
        check_return(stat, window);

        printw("attribute: %d, background: %d, foreground: %d\n", 
               junk1.attribute, junk1.background, junk1.foreground);
        refresh();

        getch();

        stat = window_get_coordinates(window, &junk2);
        check_return(stat, window);

        printw("height: %d, width: %d, startx: %d, starty: %d\n", 
               junk2.height, junk2.width, junk2.startx, junk2.starty);
        refresh();

        char *text = "label";
        label = label_create(window, 1, 1, 32, TRUE, text, strlen(text));
        check_creation(label);
        
        stat = window_add(window, label);
        check_return(stat, window);

        stat = window_draw(window);
        check_return(stat, window);
        refresh();

        getch();

        stat = window_erase(window);
        check_return(stat, window);
        refresh();

        getch();

        stat = window_destroy(window);
        check_return(stat, window);

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    endwin();

    return 0;

}

