
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"
#include "containers/menu.h"
#include "components/menu/menus.h"

int main(int argc, char **argv) {

    int col = 4;
    int row = 2;
    int stat = 0;
    int width = 40;
    int height = 10;
    menu_data_t item1_data;
    menu_data_t item2_data;
    workbench_t *wb = NULL;
    window_t *window = NULL;
    container_t *menu = NULL;
    component_t *item1 = NULL;
    component_t *item2 = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle and F10 for menu\n");
        refresh();

        window = window_create(row, col, height, width);
        check_creation(window);

        stat = window_box(window, "menu");
        check_return(stat, window);

        menu = menu_create(1, 1, height - 2, width - 2);
        check_creation(menu);

        item1_data.label = "item 1";
        item1_data.description = "this is a description 1";

        item1 = menu_item_create(&item1_data, sizeof(menu_data_t));
        check_creation(item1);

        item2_data.label = "item 2";
        item2_data.description = "this is a description 2";
        
        item2 = menu_item_create(&item2_data, sizeof(menu_data_t));
        check_creation(item1);
        
        stat = container_add_component(menu, item1);
        check_return(stat, menu);
        
        stat = container_add_component(menu, item2);
        check_return(stat, menu);
        
        stat = window_add_container(window, menu);
        check_return(stat, window);

        stat = workbench_add_window(wb, window);
        check_return(stat, wb);

        stat = workbench_loop(wb);
        check_return(stat, wb);

        exit_when;

    } use {

        stat = ERR;
        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        workbench_destroy(wb);

    } end_when;

    return stat;

}

