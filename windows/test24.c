
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "colors.h"
#include "workbench.h"
#include "error_codes.h"
#include "containers/form.h"
#include "components/vline.h"
#include "containers/menus/box.h"
#include "components/menu/menus.h"
#include "components/forms/forms.h"

workbench_t *wb = NULL;

window_t *create_form(int *stat) {

    int row = 5;
    int col = 10;
    int width = 40;
    int height = 10;

    window_t *window = NULL;
    container_t *form = NULL;
    component_t *hline = NULL;
    component_t *field1 = NULL;
    component_t *field2 = NULL;
    component_t *label1 = NULL;
    component_t *label2 = NULL;
    static field_data_t label_fd1;
    static field_data_t field_fd1;
    static field_data_t label_fd2;
    static field_data_t field_fd2;
    static field_data_t hline_fd1;

    when_error {

        window = window_create(row, col, height, width);
        check_creation(window);

        *stat = window_box(window, "form");
        check_return(*stat, window);

        label_fd1.row = 0;
        label_fd1.col = 1;
        label_fd1.width = 15;
        label_fd1.height = 1;
        label_fd1.value = "label 1";

        label1 = form_label_create(&label_fd1);
        check_creation(label1);

        *stat = component_set_padding(label1, TRUE);
        check_return(*stat, label1);

        *stat = component_set_colors(label1, RED, WHITE);
        check_return(*stat, label1);

        hline_fd1.row = 1;
        hline_fd1.col = 1;
        hline_fd1.width = 30;
        hline_fd1.height = 1;

        hline = form_seperator_create(&hline_fd1);
        check_creation(hline);

        label_fd2.row = 2;
        label_fd2.col = 1;
        label_fd2.width = 15;
        label_fd2.height = 1;
        label_fd2.value = "label 2";

        label2 = form_label_create(&label_fd2);
        check_creation(label2);

        *stat = component_set_padding(label1, TRUE);
        check_return(*stat, label1);

        *stat = component_set_attribute(label1, A_BOLD);
        check_return(*stat, label1);

        field_fd1.row = 0;
        field_fd1.col = 17;
        field_fd1.width = 15;
        field_fd1.height = 1;
        field_fd1.value = "current value";

        field1 = form_field_create(&field_fd1);
        check_creation(field1);

        field_fd2.row = 2;
        field_fd2.col = 17;
        field_fd2.width = 15;
        field_fd2.height = 1;
        field_fd2.value = "another value";

        field2 = form_field_create(&field_fd2);
        check_creation(field2);

        form = form_create(1, 1, 9, 39);
        check_creation(form);

        *stat = container_add_component(form, label1);
        check_return(*stat, form);

        *stat = container_add_component(form, field1);
        check_return(*stat, form);

        *stat = container_add_component(form, hline);
        check_return(*stat, form);

        *stat = container_add_component(form, label2);
        check_return(*stat, form);

        *stat = container_add_component(form, field2);
        check_return(*stat, form);

        *stat = window_add_container(window, form);
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
        item2_data.description = "this is a description 2";

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
    window_t *form = NULL;
    window_t *menu = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C or F12 to exit, F11 to cycle and F10 for menu\n");
        refresh();

        menu = create_menu(&stat);
        check_status(stat, OK, E_INVOPS);

        stat = workbench_add_window(wb, menu);
        check_return(stat, wb);
        
        form = create_form(&stat);
        check_status(stat, OK, E_INVOPS);

        stat = workbench_add_window(wb, form);
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

