
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <ncurses.h>
#include <menu.h>
#include <errno.h>

#include "include/when.h"
#include "widgets/colors.h"
#include "widgets/events.h"
#include "objects/object.h"
#include "widgets/widget.h"
#include "include/item_list.h"
#include "include/error_codes.h"
#include "include/error_trace.h"
#include "widgets/components/hline.h"
#include "widgets/components/menus/bar.h"

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *bar_menu(int startx, int starty, int height, int width, menus_list_t *list, int list_size) {

    int tab = 1;
    int stat = OK;
    window_t *window = NULL;
    component_t *menu = NULL;
    component_t *hline = NULL;

    when_error_in {

        window = window_create("", startx, starty, height, width, TRUE);
        check_creation(window);

        startx += 0;
        starty += 0;

        menu = bar_menu_create(window, startx, starty, height, width, tab, list, list_size);
        check_creation(menu);

        hline = hline_create(window, startx, starty, width);
        check_creation(hline);

        stat = window_add(window, menu);
        check_return(stat, window);

        stat = window_add(window, hline);
        check_return(stat, window);

        stat = window_set_tab(window, tab);
        check_return(stat, window);

        exit_when;

    } use {

        object_set_error2(window, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return window;

}

