
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <ncurses.h>
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "window.h"
#include "item_list.h"
#include "error_codes.h"
#include "components/text.h"
#include "components/hline.h"

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *base_window(char *title, int startx, int starty, int height, int width) {

    int col = 0;
    int row = 0;
    int stat = OK;
    window_t *win = NULL;
    component_t *text = NULL;
    component_t *hline = NULL;
    char *value = "F10=Main  F11=Cycle  F12=Quit";

    when_error_in {

        win = window_create(title, startx, starty, height, width, TRUE);
        check_creation(win);

        row = height - 2;
        hline = hline_create(win, row, col, width);
        check_creation(hline);

        row = height - 1;
        col = ((width - strlen(value)) / 2);
        width = strlen(value) + 1;
        text = text_create(win, row, col, width, value, strlen(value));
        check_creation(text);

        stat = window_add(win, hline);
        check_return(stat, win);

        stat = window_add(win, text);
        check_return(stat, win);

        exit_when;

    } use {

        object_set_error2(win, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return win;

}

