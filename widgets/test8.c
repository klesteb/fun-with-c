
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "window.h"
#include "item_list.h"
#include "components/hline.h"
#include "components/vline.h"

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
    window_t *window = NULL;
    component_t *vline = NULL;
    component_t *hline = NULL;

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        setup();

        window = window_create("", 10, 4, 10, 40, TRUE);
        check_creation(window);

        stat = window_set_theme(window, &theme);
        check_return(stat, window);

        hline = hline_create(window, 2, 0, 40);
        check_creation(hline);

        stat = window_add(window, hline);
        check_return(stat, window);

        vline = vline_create(window, 0, 20, 10);
        check_creation(vline);

        stat = window_add(window, vline);
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

