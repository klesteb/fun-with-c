
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "common.h"
#include "containers/menus/box.h"
#include "components/menu/menus.h"


workbench_t *wb = NULL;


window_t *create_menu(int *stat) {

    int col = 4;
    int row = 2;
    int width = 40;
    int height = 10;
    window_t *window = NULL;
    container_t *menu = NULL;
    component_t *item1 = NULL;
    component_t *item2 = NULL;
    static item_data_t item1_data;
    static item_data_t item2_data;

    when_error {

        window = window_create(row, col, height, width);
        check_creation(window);

        *stat = window_box(window, "menu");
        check_return(*stat, window);

        menu = box_menu_create(1, 1, height - 2, width - 2);
        check_creation(menu);

        item1_data.label = "item 1";
        item1_data.description = "this is a description 1";

        item1 = menu_item_create(&item1_data);
        check_creation(item1);

        item2_data.label = "item 2";
        item2_data.description = "this is description 2";

        item2 = menu_item_create(&item2_data);
        check_creation(item1);

        *stat = container_add_component(menu, item1);
        check_return(*stat, menu);

        *stat = container_add_component(menu, item2);
        check_return(*stat, menu);

        *stat = window_add_container(window, menu);
        check_return(*stat, window);

        exit_when;

    } use {

        *stat = ERR;
        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return window;

}

int main(int argc, char **argv) {

    int stat = OK;
    window_t *window = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle and F10 for menu\n");
        refresh();

        window = create_menu(&stat);
        check_status(stat, OK, E_INVOPS);

        stat = workbench_add_window(wb, window);
        check_return(stat, wb);

        stat = workbench_loop(wb);
        check_return(stat, wb);

        exit_when;

    } use {

        stat = ERR;
        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();
        workbench_destroy(wb);

    } end_when;

    return stat;

}

