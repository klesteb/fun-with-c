
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "widget.h"
#include "item_list.h"

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
    item_list_t items[3];
    widget_t *widget = NULL;
    theme_t theme;
    theme_t junk1;
    coordinates_t junk2;
    coordinates_t coordinates;

    theme.attribute  = A_NORMAL;
    theme.foreground = WHITE;
    theme.background = BLACK;

    coordinates.cols = 40;
    coordinates.rows = 10;
    coordinates.startx = 10;
    coordinates.starty = 10;

    SET_ITEM(items[0], WIDGET_K_THEME, &theme, sizeof(theme_t), NULL);
    SET_ITEM(items[1], WIDGET_K_COORDINATES, &coordinates, sizeof(coordinates_t), NULL);
    SET_ITEM(items[2], 0, 0, 0, 0);

    when_error_in {

        setup();

        widget = widget_create(items);
        check_creation(widget);

        stat = widget_get_theme(widget, &junk1);
        check_return(stat, widget);

        printw("attribute: %d, background: %d, foreground: %d\n", 
               junk1.attribute, junk1.background, junk1.foreground);
        refresh();

        getch();

        stat = widget_get_coordinates(widget, &junk2);
        check_return(stat, widget);

        printw("rows: %d, cols: %d, startx: %d, starty: %d\n", 
               junk2.rows, junk2.cols, junk2.startx, junk2.starty);
        refresh();

        getch();

        stat = widget_draw(widget);
        check_return(stat, widget);
fprintf(stderr, "after draw\n");
        
        getch();

        stat = widget_erase(widget);
        check_return(stat, widget);
fprintf(stderr, "after erase\n");

        getch();

        stat = widget_destroy(widget);
        check_return(stat, widget);
fprintf(stderr, "after destroy\n");

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    endwin();

    return 0;

}

