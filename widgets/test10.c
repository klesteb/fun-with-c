
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "window.h"
#include "item_list.h"
#include "components/text.h"
#include "components/textarea.h"

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
    component_t *text = NULL;
    component_t *textarea = NULL;
    char *latin = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";

    theme.attribute  = A_NORMAL;
    theme.foreground = BROWN;
    theme.background = BLACK;

    when_error_in {

        setup();

        window = window_create("", 10, 4, 10, 40, TRUE);
        check_creation(window);

        stat = window_set_theme(window, &theme);
        check_return(stat, window);

        char *line = "this is a test line to show";
        text = text_create(window, 1, 1, 30, line, strlen(line));
        check_creation(text);

        textarea = textarea_create(window, 5, 3, 5, 30, 1, latin, strlen(latin));
        check_creation(textarea);

        stat = window_add(window, text);
        check_return(stat, window);

        stat = window_add(window, textarea);
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

