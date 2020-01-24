
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"
#include "containers/menus/bar.h"
#include "components/menu/menus.h"

window_t *create_menu(int *stat) {

    int col = 4;
    int row = 1;
    int width = 60;
    int height = 2;
    window_t *window = NULL;
    container_t *menu = NULL;
    component_t *item1 = NULL;
    component_t *item2 = NULL;
    component_t *item3 = NULL;
    component_t *item4 = NULL;
    component_t *item5 = NULL;
    component_t *item6 = NULL;
    static item_data_t item1_data;
    static item_data_t item2_data;
    static item_data_t item3_data;
    static item_data_t item4_data;
    static item_data_t item5_data;
    static item_data_t item6_data;

    when_error {

        row = getbegy(stdscr) + 1;
        col = getbegx(stdscr);
        width = getmaxx(stdscr) - 2;
        
        window = window_create(row, col, height, width);
        check_creation(window);

        *stat = window_box(window, "");
        check_return(*stat, window);

        menu = bar_menu_create(width - 2);
        check_creation(menu);

        item1_data.label = "item 1";
        item1_data.description = "this is a description 1";

        item1 = menu_item_create(&item1_data, sizeof(item_data_t));
        check_creation(item1);

        *stat = container_add_component(menu, item1);
        check_return(*stat, menu);

        item2_data.label = "item 2";
        item2_data.description = "this is a description 2";

        item2 = menu_item_create(&item2_data, sizeof(item_data_t));
        check_creation(item2);

        *stat = container_add_component(menu, item2);
        check_return(*stat, menu);

        item3_data.label = "item 3";
        item3_data.description = "this is a description 3";

        item3 = menu_item_create(&item3_data, sizeof(item_data_t));
        check_creation(item3);

        *stat = container_add_component(menu, item3);
        check_return(*stat, menu);

        item4_data.label = "item 4";
        item4_data.description = "this is a description 4";

        item4 = menu_item_create(&item4_data, sizeof(item_data_t));
        check_creation(item4);

        *stat = container_add_component(menu, item4);
        check_return(*stat, menu);

        item5_data.label = "item 5";
        item5_data.description = "this is a description 5";

        item5 = menu_item_create(&item5_data, sizeof(item_data_t));
        check_creation(item5);

        *stat = container_add_component(menu, item5);
        check_return(*stat, menu);

        item6_data.label = "item 6";
        item6_data.description = "this is a description 6";

        item6 = menu_item_create(&item6_data, sizeof(item_data_t));
        check_creation(item6);

        *stat = container_add_component(menu, item6);
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
    window_t *menu = NULL;
    workbench_t *wb = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle and F10 for menu\n");
        refresh();

        menu = create_menu(&stat);
        check_status(stat, OK, E_INVOPS);

        stat = workbench_add_window(wb, menu);
        check_return(stat, wb);

        stat = workbench_set_focus(wb, menu);
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

