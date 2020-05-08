
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "common.h"
#include "workbench.h"
#include "containers/menus/bar.h"
#include "components/menu/menu_items.h"

workbench_t *wb = NULL;

int menu_callback(void *data, int size) {

    int stat;
    window_t *win = NULL;

    when_error {

        win = window_create(6, 4, 10, 40);
        check_creation(win);

        stat = window_box(win, "window 1");
        check_return(stat, win);

        if (data != NULL) {

            stat = window_output(win, 1, 0, "%*s", size, data);

        } else {

            stat = window_output(win, 1, 0, "no data");

        }

        check_return(stat, win);

        stat = workbench_add_window(wb, win);
        check_return(stat, wb);

        stat = workbench_set_focus(wb, win);
        check_return(stat, wb);

        stat = workbench_refresh(wb);
        check_return(stat, wb);

        exit_when;

    } use {

        fprintf(stderr, "Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

window_t *create_menu(int *stat) {

    int col = 4;
    int row = 1;
    int width = 60;
    int height = 2;
    menus_t *menu = NULL;
    window_t *window = NULL;
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

        row = getbegy(stdscr);
        col = getbegx(stdscr);
        width = getmaxx(stdscr) - 2;
        
        window = window_create(row, col, height, width);
        check_creation(window);

        *stat = window_box(window, "");
        check_return(*stat, window);

        menu = bar_menu_create(width - 2);
        check_creation(menu);

        item1_data.label = "item 1";
        item1_data.description = "this is description 1";
        item1_data.callback = menu_callback;
        item1_data.data = "this is menu_callback 1";
        item1_data.data_size = strlen(item1_data.data);

        item1 = menu_item_create(&item1_data);
        check_creation(item1);

        *stat = menus_add_component(menu, item1);
        check_return(*stat, menu);

        item2_data.label = "item 2";
        item2_data.description = "this is description 2";

        item2 = menu_item_create(&item2_data);
        check_creation(item2);

        *stat = menus_add_component(menu, item2);
        check_return(*stat, menu);

        item3_data.label = "item 3";
        item3_data.description = "this is description 3";

        item3 = menu_item_create(&item3_data);
        check_creation(item3);

        *stat = menus_add_component(menu, item3);
        check_return(*stat, menu);

        item4_data.label = "item 4";
        item4_data.description = "this is description 4";

        item4 = menu_item_create(&item4_data);
        check_creation(item4);

        *stat = menus_add_component(menu, item4);
        check_return(*stat, menu);

        item5_data.label = "item 5";
        item5_data.description = "this is description 5";

        item5 = menu_item_create(&item5_data);
        check_creation(item5);

        *stat = menus_add_component(menu, item5);
        check_return(*stat, menu);

        item6_data.label = "item 6";
        item6_data.description = "this is description 6";

        item6 = menu_item_create(&item6_data);
        check_creation(item6);

        *stat = menus_add_component(menu, item6);
        check_return(*stat, menu);
        
        *stat = window_add_container(window, (container_t *)menu);
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

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

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

