
#include <ncurses.h>

#include "include/when.h"
#include "include/item_list.h"

#include "widgets/colors.h"
#include "widgets/window.h"
#include "widgets/components/text.h"
#include "widgets/components/prompt.h"

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
    component_t *prompt = NULL;

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
        text = text_create(window, 3, 1, 30, line, strlen(line));
        check_creation(text);

        char *stuff = "really long prompt";
        prompt = prompt_create(window, 5, 1, 10, stuff, strlen(stuff));
        check_creation(prompt);

        stat = window_add(window, text);
        check_return(stat, window);

        stat = window_add(window, prompt);
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

